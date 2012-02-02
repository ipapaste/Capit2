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
private:
	ValueGroup valueGroup_;
public:
	ActiveFlow(string sourceIp, string destinationIp, int sourcePort, int destinationPort, MarkovMatrix* group, DelayMatrix* delayMatrix):Flow(sourceIp, destinationIp, sourcePort, destinationPort,group, delayMatrix)
	{
		BOOST_FOREACH(FlowState* state, flowStates)
		{
			state->setTransitions(group);
			if(state->getId() == 0)
			{
				activeState = state;
			}
		}
		valueGroup_ = ValueGroupManager::getRandomGroup(destinationPort);
		ClientManagerInstance::getInstance()->registerSource();
	}

	void run()
	{
		if(activeState == NULL)
		{
			cout << "NULL activeState found." << endl;
			return;
		}

		//FlowType* type = FlowTypeManager::getInstance()->getType(destinationPort_);

		deque<Command*> commands = activeState->getCommands();
		Packet* packet = new Packet();

		string* src = new string(sourceIp_);
		string* dst = new string(destinationIp_);
		packet->setDestinationIp(dst);
		packet->setSourceIp(src);
		packet->setDestinationPort(destinationPort_);
		packet->setSourcePort(sourcePort_);

		if(commands.size()>=1)
		{
			int size = commands.size();
			string test = commands[Rnd::getInt(size-1)]->getVariableCommand(valueGroup_);
			string* s = new string(test);
		packet->setPayload(s);
		}
		else
		{
			string* s = new string("TATATA");
			packet->setPayload(s);
		}

		ClientManagerInstance::getInstance()->accept(*packet);
		//TODO: Send the packet.

		int dice = Rnd::getInt(1000);

		int stateId = activeState->getTransitionId(dice);

		//cout << "Dice: " << dice << " from id: " << activeState->getId()<<" to id: " << stateId << endl;



		if(stateId == -1)
		{
			cout << "No more states to move to." << endl;
			ClientManagerInstance::getInstance()->removeSource();
			return;
		}

		int delay = 100;
		BOOST_FOREACH(FlowState* state, flowStates)
		{
			if(state->getId() == stateId)
			{
				delay = Rnd::getNormalCutoff(delayMatrix_->getValue(activeState->getId(), state->getId()),100,50);
				activeState = state;
				break;
			}
		}

		ThreadShell::schedule(*this,delay);
	}
};

#endif /* ACTIVEFLOW_HPP_ */
