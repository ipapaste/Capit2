/*
 * ActiveFlow.hpp
 *
 *  Created on: Jan 24, 2012
 *      Author: issle
 */

#ifndef ACTIVEFLOW_HPP_
#define ACTIVEFLOW_HPP_

#include <iostream>
#include <string>
#include <boost/foreach.hpp>
#include "Thread.hpp"

using namespace std;

/**
 * An ActiveFlow, is a flow that can auto-execute itself
 * and feed packets to the ClientManager.
 */
class ActiveFlow: public Flow, public ThreadShell
{
public:
	ActiveFlow(string sourceIp, string destinationIp, int sourcePort, int destinationPort, MarkovMatrix* group):Flow(sourceIp, destinationIp, sourcePort, destinationPort,group)
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

		cout << "0" << endl;
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

#endif /* ACTIVEFLOW_HPP_ */
