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
#include "FlowGroup.hpp"
#include "commons/Singleton.hpp"
using namespace std;



class FlowType
{
	string name_;
	int port_;
	list<FlowState*> states;
public:
	FlowType(string name, int port):name_(name)
	{
		port_ = port;
	}

	int size()
	{
		return states.size();
	}

	FlowState* getFlowStateForPacket(Packet& packet)
	{
		FlowState* state = NULL;
		BOOST_FOREACH(FlowState* st, states)
		{
			state = st->getState(packet);
			if(state != NULL)
				return state;
		}

		return state;
	}

	void addState(FlowState* state)
	{
		states.push_front(state);
	}

	int getPort()
	{
		return port_;
	}

	string getName()
	{
		return name_;
	}

	void print()
	{
		BOOST_FOREACH(FlowState* state, states)
		{
			state->print();
		}
	}

};

class Flows
{
private:
	map<int,FlowType*> flowTypes;
public:

	void addType(int id, FlowType* type)
	{
		flowTypes[id] = type;
	}

	FlowType* getType(int id)
	{
		FlowType* type = NULL;

		type = flowTypes[id];

		return type;
	}

	int size()
	{
		return flowTypes.size();
	}
};

typedef Singleton<Flows> FlowsInstance;

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

		FlowType* type = FlowsInstance::getInstance()->getType(destinationPort);
		group = new FlowGroup(type->size());
		types.resize(type->size());
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
		if(state == NULL)
			return;

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
		}
		else if(activeState->getId() == state->getId())
		{
			int temp = group->getProbability(activeState->getId(), state->getId());
			temp++;
			group->setProbability(activeState->getId(), state->getId(),temp);
		}

		activeState->accept(packet);
	}

	FlowState* getFlowState(Packet& packet)
	{
		FlowState* state = NULL;

		FlowType* type =  FlowsInstance::getInstance()->getType(packet.getDestinationPort());

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

		cout << "Transition matrix: " << endl;
		int size = FlowsInstance::getInstance()->size();
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


	}
};

#endif /* FLOW_HPP_ */
