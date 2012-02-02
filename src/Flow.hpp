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
#include "FlowState.hpp"
#include "MarkovMatrix.hpp"
#include "commons/container/Singleton.hpp"
#include "Thread.hpp"
#include "commons/math/Rnd.hpp"
#include "Definitions.hpp"
#include "FlowType.hpp"
#include "FlowTypeManager.hpp"
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
	map<int,Packet*> packets;

	/*
	 * MarkovMatrix of this flow. Either
	 * loaded or generated.
	 */
	MarkovMatrix* group;

	DelayMatrix* delayMatrix_;
	DelayMatrix* delayCount_;

	/*
	 * Types of states for fast and
	 * enumerated access.
	 */
	vector<int> types;

	/*
	 * All the past flow states.
	 */
	list<FlowState*> flowStates;

	/*
	 * The current state of the flow.
	 */
	FlowState* activeState;

	int lastTimestamp;
public:

	Flow()
	{
		activeState = NULL;
	}
	Flow(string s, int sourcePort, int destinationPort)
	{
		activeState = NULL;
		flowId = s;
		sourcePort_ = sourcePort;
		destinationPort_ = destinationPort;

		FlowType* type = FlowTypeManager::getInstance()->getType(destinationPort);

		if(type == NULL)
		{
			cout << "There is no such type." << endl;
		}
		group = new MarkovMatrix(type->size());
		delayMatrix_ = new DelayMatrix(type->size());
		delayCount_ = new DelayMatrix(type->size());
		types.resize(type->size());
		lastTimestamp = -1;


	}

	Flow(string sourceIp, string destinationIp, int sourcePort, int destinationPort, MarkovMatrix* _group, DelayMatrix* delayMatrix)
	{
		activeState = NULL;
		sourcePort_ = sourcePort;
		destinationPort_ = destinationPort;
		sourceIp_ = sourceIp;
		destinationIp_ = destinationIp;

		flowId = Flow::getFlowId(sourceIp, destinationIp, sourcePort, destinationPort);

		FlowType* type = FlowTypeManager::getInstance()->getType(destinationPort);

		if(type == NULL)
		{
				cout << "There is no such type." << endl;
		}

		if(type->size()!= _group->size())
		{
			cout <<"The defined size for the flow of type: " << destinationPort << " is " << type->size() << endl;
			cout <<"User specified: " << _group->size() << endl;
		}

		group = _group;
		delayMatrix_ = delayMatrix;
		types.resize(type->size());

		list<FlowState*> states = type->getFlowStates();

		BOOST_FOREACH(FlowState* st, states)
		{
			flowStates.push_back(st);
		}
	}

	static string getFlowId(string srcIp, string dstIp, int srcPort, int dstPort)
	{
		string s;

		string srcPortS = boost::lexical_cast<string>(srcPort);
		string dstPortS = boost::lexical_cast<string>(dstPort);

		s.append(srcIp).append("_").append(dstIp).append("_").append(srcPortS).append(
				"_").append(dstPortS);

		return s;
	}

	string getId()
	{
		return flowId;
	}

	void accept(Packet& packet)
	{
		if(lastTimestamp == -1)
			lastTimestamp = packet.getTimestamp();
		//Get the state that this packet defines.
		FlowState* state = getFlowState(packet);

		//If no state is defined ignore the packet.
		if(state == NULL)
		{
			cout << "No state, returning." << endl;
			return;
		}

		//If there is an activeState, post-increase
		//its occurrence.
		if(activeState != NULL)
		{
			int stateCount = types[activeState->getId()];
			stateCount++;
			types[activeState->getId()] = stateCount;
		}
		cout << "a" << endl;
		//If there is no active state, set this state
		//as the active one and store it in the states
		//of this Flow.
		if(activeState == NULL)
		{
			activeState = state;
			flowStates.push_back(activeState);
		}
		//If we have a state transition, store it and
		//change the state.
		else if(activeState->getId() != state->getId())
		{
			cout << "aaa" << endl;
			int temp = group->getProbability(activeState->getId(), state->getId());
			temp++;
			group->setProbability(activeState->getId(), state->getId(),temp);

			int oldDelay = delayMatrix_->getValue(activeState->getId(), state->getId());
			oldDelay += (packet.getTimestamp()- lastTimestamp);

			delayMatrix_->setValue(activeState->getId(), state->getId(),oldDelay);

			int delayCount = delayCount_->getValue(activeState->getId(), state->getId());
			delayCount++;
			delayCount_->setValue(activeState->getId(), state->getId(),delayCount);

			activeState = state;
			flowStates.push_back(activeState);
		}
		//If we have a state repetition, store it and
		//keep the same test.
		else if(activeState->getId() == state->getId())
		{
			cout << "aaaa" << endl;
			int temp = group->getProbability(activeState->getId(), state->getId());
			temp++;
			group->setProbability(activeState->getId(), state->getId(),temp);



			int oldDelay = delayMatrix_->getValue(activeState->getId(), state->getId());
			oldDelay += (packet.getTimestamp()- lastTimestamp);
			delayMatrix_->setValue(activeState->getId(), state->getId(),oldDelay);

			int delayCount = delayCount_->getValue(activeState->getId(), state->getId());
			delayCount++;

			delayCount_->setValue(activeState->getId(), state->getId(),delayCount);
		}

		//Feed the packet to the valid state.
		activeState->accept(packet);
	}

	FlowState* getFlowState(Packet& packet)
	{
		FlowState* state = NULL;

		FlowType* type =  FlowTypeManager::getInstance()->getType(packet.getDestinationPort());

		if(type == NULL)
			return state;

		state = type->getFlowStateForPacket(packet);

		return state;
	}

	void calc()
	{
		cout << "Calculating "<< getId()<< endl;
		BOOST_FOREACH(FlowState* st , flowStates)
		{
			st->calc();
		}

		cout << "Markov matrix: " << endl;
		int size = FlowTypeManager::getInstance()->size();
		for(int i = 0; i<types.size(); i++ )
		{
			for(int j = 0; j < types.size(); j++)
			{
				float oldProb = group->getProbability(i,j);

				int divider = types[i];

				if(divider == 0)
					divider = 1;

				group->setProbability(i,j,oldProb/divider);

				cout << setw(5) << group->getProbability(i,j) << " ";
			}
			cout << endl;
		}

		cout << "Delay matrix: " << endl;
				for(int i = 0; i<types.size(); i++ )
				{
					for(int j = 0; j < types.size(); j++)
					{
						float oldProb = delayMatrix_->getValue(i,j);

						int divider = delayCount_->getValue(i,j);

						if(divider == 0)
							divider = 1;

						delayMatrix_->setValue(i,j,oldProb/divider);

						cout << setw(5) << delayMatrix_->getValue(i,j) << " ";
					}
					cout << endl;
				}
	}
};



#endif /* FLOW_HPP_ */
