/*
 * FlowType.hpp
 *
 *  Created on: Jan 24, 2012
 *      Author: issle
 */

#ifndef FLOWTYPE_HPP_
#define FLOWTYPE_HPP_

#include <iostream>
#include <string>
#include <boost/foreach.hpp>

using namespace std;

/**
 * Each flow contains a specific set of States that
 * it can transit to thus a FlowType is defined based
 * on the protocol that a Flow is based on.
 */
class FlowType
{
private:
	string name_;
	int port_;

	/*
	 * A list of all the possible states that the flow
	 * can access. Should be used for new FlowState
	 * generation.
	 */
	deque<FlowState> states;

	bool unknown;
public:

	FlowType()
	{
		unknown = true;
	}
	FlowType(string name, int port):name_(name)
	{
		port_ = port;
		unknown = false;
	}

	bool isUnknown()
	{
		return unknown;
	}

	void setIsUnknown()
	{
		unknown = true;
	}

	int size()
	{
		return states.size();
	}

	deque<FlowState> getFlowStates()
	{
		return states;
	}

	/*
	 * Returns the state in which the input packet belongs.
	 * Returns NULL if no state exists.
	 */
	FlowState getFlowStateForPacket(Packet& packet)
	{
		cout << "Came there " << endl;
		FlowState state;
		for(int i = 0; i < states.size(); i++)
		{
			state = states[i].getState(packet);
			if(!states[i].isUnknown())
			{
				return state;
			}
		}
		return state;
	}

	void addState(IFlowState& state)
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
		for(int i = 0; i < states.size(); i++)
		{
			states[i].print();
		}
	}

};


#endif /* FLOWTYPE_HPP_ */
