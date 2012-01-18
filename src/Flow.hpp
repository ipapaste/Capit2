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
#include "LoginUState.hpp"
#include "CommandState.hpp"
#include "FlowGroup.hpp"
using namespace std;

/**
 * A Flow is an Object that consists of all the packets
 * that have the same srcip/dstip/srcport/dstport combination.
 */
class Flow: public AbstractNode
{
private:
	string flowId;

	int sourcePort_;
	int destinationPort_;

	map<int,Packet*> packets;

	FlowGroup* group;

	vector<int> types;

	list<FlowState*> flowStates;

	FlowState* activeState;
public:

	Flow(string s, int sourcePort, int destinationPort)
	{
		flowId = s;
		sourcePort_ = sourcePort;
		destinationPort_ = destinationPort;
		group = new FlowGroup(2);
		types.resize(2);
	}

	static string getFlowId(const string& srcIp, const string& dstIp, int srcPort, int dstPort)
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
		FlowState* state = getFlowState(packet);

		if(activeState != NULL)
			types[activeState->getId()]++;

		if(activeState == NULL)
		{
			activeState = state;

			flowStates.push_back(activeState);
		}
		else if(activeState->getId() != state->getId())
		{
			int temp = group->getProbability(activeState->getId(), state->getId());
			temp++;
			group->setProbability(activeState->getId(), state->getId(),temp);

			activeState = state;
			flowStates.push_back(activeState);
			cout << "Changing state." << endl;
		}
		else if(activeState->getId() == state->getId())
		{
			int temp = group->getProbability(activeState->getId(), state->getId());
			temp++;

			cout << "Found " << activeState->getId() << " "<< state->getId() << endl;
			group->setProbability(activeState->getId(), state->getId(),temp);
		}

		activeState->accept(packet);
	}

	FlowState* getFlowState(Packet& packet)
	{
		FlowState* state = NULL;

		if(packet.getPayload()->find("PASS") != string::npos || packet.getPayload()->find("USER") != string::npos)
		{
			cout << "Found logins tate." << endl;
			state = new LoginUState();
		}
		else
			state = new CommandState();

		return state;
	}

	void calc()
	{
		cout << "Calculating "<< getId()<< endl;
		BOOST_FOREACH(FlowState* st , flowStates)
		{
			st->calc();
		}

		cout << "Transition matrix: " << endl;
		for(int i = 0; i<2; i++ )
		{
			for(int j = 0; j < 2; j++)
			{
				float oldProb = group->getProbability(i,j);

				int divider = types[i];

				if(divider == 0)
					divider = 1;

				group->setProbability(i,j,oldProb/divider);

				cout << group->getProbability(i,j) << " ";
			}

			cout << endl;
		}


	}
};

#endif /* FLOW_HPP_ */
