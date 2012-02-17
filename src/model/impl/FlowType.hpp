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
#include "../IFlowType.hpp"
#include "../IFlowState.hpp"
using namespace std;

/**
 * Each flow contains a specific set of States that
 * it can transit to thus a FlowType is defined based
 * on the protocol that a Flow is based on.
 */
class FlowType: public IFlowType
{
private:
	string name_;
		int port_;

		/*
		 * A list of all the possible states that the flow
		 * can access. Should be used for new FlowState
		 * generation.
		 */
		deque<IFlowState*> states;

		bool unknown;
public:

	FlowType()
	{
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


	void setName(string name)
	{
		name_ = name;
	}

	void setPort(int port)
	{
		port_ = port;
	}

	void addState(IFlowState& state)
	{
		states.push_back(&state);
	}

	void print()
	{
		cout << "Flow type: " << port_ << endl;
		for(int i = 0; i < states.size(); i++)
		{
			states[i]->print();
		}
	}

	int size()
	{
		return states.size();
	}

	deque<IFlowState*> getFlowStates()
	{
		return states;
	}

	IFlowState& getFlowStateForPacket(Packet& packet)
	{
		IFlowState& st = states[0]->belongsToState(packet);
		for(int i = 0; i < states.size(); i++)
		{
			if(!states[i]->belongsToState(packet).isUnknown())
				return *states[i];
		}

		return st;

	}
};


#endif /* FLOWTYPE_HPP_ */
