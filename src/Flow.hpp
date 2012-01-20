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
#include "Thread.hpp"
#include "commons/tools/Rnd.hpp"
using namespace std;


/*
 * Each flow contains a specific set of States that
 * it can transit to thus a FlowType is defined based
 * on the protocol that a Flow is based on.
 */
class FlowType
{
	string name_;
	int port_;

	/*
	 * A list of all the possible states that the flow
	 * can access. Should be used for new FlowState
	 * generation.
	 */
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

	/*
	 * Returns the state in which the input packet belongs.
	 * Returns NULL if no state exists.
	 */
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

/**
 * Class that defines the list of possible FlowTypes
 * that exist in Capit2. Gets populated from persistent
 * data.
 */
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
protected:
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
		//Get the state that this packet defines.
		FlowState* state = getFlowState(packet);

		//If no state is defined ignore the packet.
		if(state == NULL)
			return;

		//If there is an activeState, post-increase
		//its occurrence.
		if(activeState != NULL)
			types[activeState->getId()]++;

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
			int temp = group->getProbability(activeState->getId(), state->getId());
			temp++;
			group->setProbability(activeState->getId(), state->getId(),temp);
			activeState = state;
			flowStates.push_back(activeState);
		}
		//If we have a state repetition, store it and
		//keep the same test.
		else if(activeState->getId() == state->getId())
		{
			int temp = group->getProbability(activeState->getId(), state->getId());
			temp++;
			group->setProbability(activeState->getId(), state->getId(),temp);
		}

		//Feed the packet to the valid state.
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

class ActiveFlow: public Flow, public ThreadShell
{
public:
	ActiveFlow(Flow& flow):Flow(flow)
	{
		BOOST_FOREACH(FlowState* state, flowStates)
		{
			state->setTransitions(group);
			if(state->getId() == 0)
			{
				activeState = state;
			}
		}
	}

	void run()
	{
		if(activeState == NULL)
		{
			cout << "NULL activeState found." << endl;
			return;
		}

		//TODO: Send the packet.

		int dice = Rand::getInstance()->getInt(1000);


		int stateId = activeState->getTransitionId(dice);

		cout << "Dice: " << dice << " from id: " << activeState->getId()<<" to id: " << stateId << endl;

		if(stateId == -1)
		{
			cout << "No more states to move to." << endl;
			return;
		}


		BOOST_FOREACH(FlowState* state, flowStates)
		{
			if(state->getId() == stateId)
			{
				activeState = state;
				break;
			}
		}


		ThreadShell::schedule(*this,1000);
	}
};

#endif /* FLOW_HPP_ */
