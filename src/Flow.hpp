/*
 * Flow.hpp
 *
 *  Created on: Jan 18, 2012
 *      Author: issle
 */

#ifndef FLOW_HPP_
#define FLOW_HPP_

#include <iostream>
#include <string>
#include "boost/lexical_cast.hpp"
#include "Packet.hpp"
#include "Node.hpp"
#include "commons/container/Singleton.hpp"
#include "commons/concurrent/Thread.hpp"
#include "commons/math/Rnd.hpp"
#include "model/impl/MarkovMatrix.hpp"
#include "model/impl/DelayMatrix.hpp"
#include "model/IFlowState.hpp"
using namespace std;

/**
 * A Flow is an Object that consists of all the packets
 * that have the same srcip/dstip/srcport/dstport combination.
 */
class Flow: public AbstractNode
{
protected:
	/*
	 * Unique identifier of each flow.
	 */
	string flowId;

	/*
	 * Identity parts of the flow.
	 */
	int sourcePort_;
	int destinationPort_;
	string sourceIp_;
	string destinationIp_;

	/*
	 * Packets stored in this flow.
	 */
	map<int, Packet*> packets;

	/*
	 * MarkovMatrix of this flow. Either
	 * loaded or generated.
	 */
	IMatrix* group;

	IMatrix* delayMatrix_;
	IMatrix* delayCount_;

	/*
	 * Types of states for fast and
	 * enumerated access.
	 */
	vector<int> types;

	/*
	 * All the past flow states.
	 */
	deque<IFlowState*> flowStates;

	/*
	 * The current state of the flow.
	 */
	IFlowState* activeState;

	long lastTimestamp;
public:

	Flow()
	{
		activeState == NULL;
		group = new MarkovMatrix();
		delayMatrix_ = new DelayMatrix();
		delayCount_ = new DelayMatrix();
	}
	Flow(string s, int sourcePort, int destinationPort)
	{

		activeState == NULL;
		flowId = s;
		sourcePort_ = sourcePort;
		destinationPort_ = destinationPort;

		IFlowType& type =
				CapitService::getInstance().getFlowTypeService().getType(
						destinationPort);

		if (type.isUnknown())
		{
			cout << "There is no such type." << endl;
		}

		group = new MarkovMatrix();
		delayMatrix_ = new DelayMatrix();
		delayCount_ = new DelayMatrix();
		types.resize(type.size());
		lastTimestamp = -1;
		delayMatrix_->resize(type.size());
		group->resize(type.size());
		delayCount_->resize(type.size());

	}

	Flow(string sourceIp, string destinationIp, int sourcePort,
			int destinationPort, IMatrix& _group, IMatrix& delayMatrix)
	{

		sourcePort_ = sourcePort;
		destinationPort_ = destinationPort;
		sourceIp_ = sourceIp;
		destinationIp_ = destinationIp;
		activeState == NULL;
		flowId = Flow::getFlowId(sourceIp, destinationIp, sourcePort,
				destinationPort);

		group = &_group;
		delayMatrix_ = &delayMatrix;
		delayCount_ = new DelayMatrix();

		IFlowType& type =
				CapitService::getInstance().getFlowTypeService().getType(
						destinationPort);

		if (type.isUnknown())
		{
			cout << "There is no such type." << endl;
		}

		if (type.size() != _group.getSize())
		{
			cout << "The defined size for the flow of type: " << destinationPort
					<< " is " << type.size() << endl;
			cout << "User specified: " << _group.getSize() << endl;
		}
		types.resize(type.size());

		deque<IFlowState*> states = type.getFlowStates();

		for (int i = 0; i < states.size(); i++)
		{
			flowStates.push_back(states[i]);
		}
	}

	static string getFlowId(string srcIp, string dstIp, int srcPort,
			int dstPort)
	{
		string s;

		string srcPortS = boost::lexical_cast<string>(srcPort);
		string dstPortS = boost::lexical_cast<string>(dstPort);

		s.append(srcIp).append("_").append(dstIp).append("_").append(
				"_").append(dstPortS);

		return s;
	}

	string getId()
	{
		return flowId;
	}

	void accept(Packet& packet)
	{
		//Get the state that this packet defines.
		IFlowState& state = getFlowState(packet);

		if (lastTimestamp == -1 || activeState == NULL)
		{
			activeState = &state;
			lastTimestamp = packet.getTimestamp();
		}

		//If no state is defined ignore the packet.
		if (state.isUnknown())
		{
			return;
		}

		bool result = activeState->isUnknown();
		//If there is an activeState, post-increase
		//its occurrence.
		if (!result)
		{
			int stateCount = types[activeState->getId()];
			stateCount++;
			types[activeState->getId()] = stateCount;
		}
		//If there is no active state, set this state
		//as the active one and store it in the states
		//of this Flow.
		if (activeState->isUnknown())
		{
			activeState = &state;
			flowStates.push_back(activeState);
		}
		//If we have a state transition, store it and
		//change the state.
		else if (activeState->getId() != state.getId())
		{

			int temp = group->getValue(activeState->getId(), state.getId());
			temp++;
			group->setValue(activeState->getId(), state.getId(), temp);

			long oldDelay = delayMatrix_->getValue(activeState->getId(),
					state.getId());
			oldDelay += (packet.getTimestamp() - lastTimestamp);

			delayMatrix_->setValue(activeState->getId(), state.getId(),
					oldDelay);

			int delayCount = delayCount_->getValue(activeState->getId(),
					state.getId());
			delayCount++;
			delayCount_->setValue(activeState->getId(), state.getId(),
					delayCount);

			activeState = &state;
			flowStates.push_back(activeState);
		}
		//If we have a state repetition, store it and
		//keep the same test.
		else if (activeState->getId() == state.getId())
		{

			int temp = group->getValue(activeState->getId(), state.getId());
			temp++;
			group->setValue(activeState->getId(), state.getId(), temp);

			long oldDelay = delayMatrix_->getValue(activeState->getId(),
					state.getId());
			oldDelay += (packet.getTimestamp() - lastTimestamp);
			delayMatrix_->setValue(activeState->getId(), state.getId(),
					oldDelay);
			int delayCount = delayCount_->getValue(activeState->getId(),
					state.getId());
			delayCount++;
			delayCount_->setValue(activeState->getId(), state.getId(),
					delayCount);

		}

		lastTimestamp = packet.getTimestamp();
		//Feed the packet to the valid state.
		activeState->accept(packet);
	}

	IFlowState& getFlowState(Packet& packet)
	{
		IFlowType& type =
				CapitService::getInstance().getFlowTypeService().getType(
						packet.getDestinationPort());

		if (type.isUnknown())
		{
			IFlowState& state =
					CapitService::getInstance().getFlowStateService().getNewFlowState();
			state.setIsUnknown();

			return state;
		}

		IFlowState& state = type.getFlowStateForPacket(packet);
		state.isUnknown();

		return state;
	}

	void calc()
	{
		cout << "Calculating " << getId() << endl;
		for (int i = 0; i < flowStates.size(); i++)
		{
			flowStates[i]->calc();
		}

		cout << "Markov matrix: " << endl;

		for (int i = 0; i < types.size(); i++)
		{
			for (int j = 0; j < types.size(); j++)
			{
				float oldProb = group->getValue(i, j);

				int divider = types[i];

				if (divider == 0)
					divider = 1;

				group->setValue(i, j, oldProb / divider);

				cout << setw(14) << group->getValue(i, j) << " ";
			}
			cout << endl;
		}

		cout << "Delay matrix: " << endl;
		for (int i = 0; i < types.size(); i++)
		{
			for (int j = 0; j < types.size(); j++)
			{
				float oldProb = delayMatrix_->getValue(i, j);

				int divider = delayCount_->getValue(i, j);

				if (divider == 0)
					divider = 1;

				delayMatrix_->setValue(i, j, oldProb / divider);

				cout << setw(14) << delayMatrix_->getValue(i, j) << " ";
			}
			cout << endl;
		}
	}
};

#endif /* FLOW_HPP_ */
