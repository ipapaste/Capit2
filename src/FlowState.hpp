/*
 * FlowState.hpp
 *
 *  Created on: Jan 18, 2012
 *      Author: issle
 */

#ifndef FLOWSTATE_HPP_
#define FLOWSTATE_HPP_

#include <iostream>
#include <list>
#include <string>
#include "Node.hpp"
#include "Packet.hpp"
#include <boost/foreach.hpp>
#include "applications/model/Command.hpp"
using namespace std;

class FlowState: public AbstractNode
{
private:
	int id_;
	float meanDelay;
	float stdDelay;
	int count;
	list<Packet*> packets;
	list<Command*> commands;
	string name_;

	FlowState* getNewState()
	{
		return new FlowState(*this);
	}
public:

	FlowState(int id,string name):name_(name)
	{
		id_ = id;
		count = 0;
	}

	FlowState* getState(Packet& packet)
	{
		BOOST_FOREACH(Command* command, commands)
		{
			if(packet.getPayload()->find(command->getName()) != string::npos)
			{
				return getNewState();
			}
		}

		return NULL;
	}

	void accept(Packet& packet)
	{
		bool type = false;

		BOOST_FOREACH(Command* command, commands)
		{
			if(packet.getPayload()->find(command->getCommand()) != string::npos)
				type = true;
		}

		packets.push_front(&packet);
		count++;
	}

	void addCommand(Command* command)
	{
		commands.push_front(command);
	}

	void calc()
	{
		cout << "Calculating a flow state." << endl;

		if(packets.size()<=1)
		{
			meanDelay = 0;
			return;
		}

		list<Packet*>::iterator it;

		Packet* old = NULL;

		BOOST_FOREACH(Packet* it, packets)
		{
			if(old == NULL)
			{
				old = it;
				continue;
			}
			else
			{
				meanDelay += (old->getTimestamp() - it->getTimestamp())/count;
			}
			old = it;
		}
		cout <<"Delay Mean: "<< meanDelay << endl;
		cout << "Packet size:" << count << endl;
	}

	int getId()
	{
		return id_;
	}

	void print()
	{
		cout << "State: " << name_ << " Id: " << id_ << endl;
		BOOST_FOREACH(Command* command, commands)
		{
			command->print();
		}
	}
};


#endif /* FLOWSTATE_HPP_ */
