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

	list<FlowState*> getFlowStates()
	{
		return states;
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


#endif /* FLOWTYPE_HPP_ */
