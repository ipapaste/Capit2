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
#include "commons/concurrent/Thread.hpp"
#include "AheadReplayer.hpp"

using namespace std;

/**
 * An ActiveFlow, is a flow that can auto-execute itself
 * and feed packets to the ClientManager.
 */
class ActiveFlow:public Flow,  public AheadReplayer
{
private:
	IValueGroup valueGroup_;
	long previousTimestamp_;
public:
	ActiveFlow(string sourceIp, string destinationIp, int sourcePort,
			int destinationPort, MarkovMatrix group, DelayMatrix delayMatrix) :
			Flow(sourceIp, destinationIp, sourcePort, destinationPort, group,
					delayMatrix)
	{
		previousTimestamp_ = 0;
		for(int i = 0; i < flowStates.size(); i++)
				{
					flowStates[i].setTransitions(group);
					if (flowStates[i].getId() == 0)
					{
						activeState = flowStates[i];
					}
				}
		valueGroup_ = ValueGroupManager::getRandomGroup(destinationPort);
		ClientManagerInstance::getInstance().registerSource();
	}
private:
	static const long AHEAD_THRESHOLD = 50000;
public:

	void doWork(Packet* packet)
	{
		ClientManagerInstance::getInstance().accept(*packet);

		int dice = Rnd::getInt(1000);

		cout << "Dice: " << dice << endl;
		int stateId = activeState.getTransitionId(dice);

		cout << "State id: " << stateId << endl;

		//cout << "Dice: " << dice << " from id: " << activeState->getId()<<" to id: " << stateId << endl;

		if (stateId == -1)
		{
			cout << "No more states to move to." << endl;
			ClientManagerInstance::getInstance().removeSource();
			return;
		}

		int delay = 100;
		for(int i =0; i < flowStates.size(); i++)
				{
					if (flowStates[i].getId() == stateId)
					{
						delay = Rnd::getNormalCutoff(
								delayMatrix_.getValue(activeState.getId(),
										flowStates[i].getId()), 100, 50);

						activeState = flowStates[i];
						break;
					}
				}
		previousTimestamp_ += delay;
	}

	Packet* generate()
	{
		if (activeState.isUnknown())
		{
			cout << "NULL activeState found." << endl;
			return NULL;
		}

		//FlowType* type = FlowTypeManager::getInstance().getType(destinationPort_);

		deque<Command> commands = activeState.getCommands();
		Packet* packet = new Packet();

		string* src = new string(sourceIp_);
		string* dst = new string(destinationIp_);
		packet->setDestinationIp(dst);
		packet->setSourceIp(src);
		packet->setDestinationPort(destinationPort_);
		packet->setSourcePort(sourcePort_);
		packet->setTimestamp(previousTimestamp_);

		if (commands.size() >= 1)
		{
			int size = commands.size();
			string test = commands[Rnd::getInt(size - 1)].getVariableCommand(
					valueGroup_);
			cout << test << endl;
			string* s = new string(test);
			packet->setPayload(s);
		}
		else
		{
			string* s = new string("TATATA");
			packet->setPayload(s);
		}
		return packet;

	}



};

#endif /* ACTIVEFLOW_HPP_ */
