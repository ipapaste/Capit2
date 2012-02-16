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
#include "MarkovMatrix.hpp"
#include "commons/tools/String.hpp"
#include <queue>
using namespace std;

/**
 * This class represents a continuous sequence of packets
 * inside a Flow that are grouped together by some well
 * defined property and time restriction.
 *
 * A Flow state starts with the first packet that falls
 * into the accepted property range and ends with the first
 * packet that does not fall into this flow state. The last
 * packet is excluded from the flow state.
 */
class FlowState: public AbstractNode
{
private:

	/*
	 * Each FlowState has an id that represents
	 * the type of the FlowState and not the Flow
	 * State itself. Two FlowStates can have the
	 * same id_ when they belong in the same type
	 * of FlowState
	 */
	int id_;

	/*
	 * The mean inter-packet delay in milliseconds.
	 */
	float meanDelay;

	/*
	 * The std inter-packet delay in milliseconds.
	 */
	float stdDelay;

	/*
	 * The number of packets contained in this
	 * FlowState.
	 */
	int count;

	list<Packet*> packets;
public:
	vector<int> transitions;

	int getTransitionId(int index)
	{
		return transitions[index];
	}

	void setTransitions(MarkovMatrix* group)
	{
		cout << "For: " << id_ << endl;
		vector<float> trans = group->getSubGroup(id_);

		transitions.resize(1100);

		for(int i =0; i< 1100; i++)
		{
			transitions[i] = -1;
		}

		int index = 0;
		for(int i = 0; i < trans.size(); i++)
		{
			float prob = trans[i];



			int count = 0;

			count = index + 1100*prob;

			if(count == 0)
				continue;

			for(int j = index; j < count; j++ )
			{
				transitions[j] = i;
				cout << i ;
			}

			index = count+1;
		}
	}

	deque<Command*> getCommands()
	{
	return commands;
	}
private:
	/*
	 * A template packet command list that define
	 * the FlowState type.
	 */
	deque<Command*> commands;

	string name_;

	/*
	 * Should be used to generate new flow state
	 * objects.
	 */
	FlowState* getNewState()
	{
		return new FlowState(*this);
	}
public:

	/*
	 * Should be used when reading the FlowState
	 * types from the persistent data.
	 */
	FlowState(int id,string name):name_(name)
	{
		id_ = id;
		count = 0;
	}

	/*
	 * Generates a FlowState object if the input
	 * packet belongs in that state. Returns NULL
	 * if the packet does not belong to this state.
	 */
	FlowState* getState(Packet& packet)
	{
		BOOST_FOREACH(Command* command, commands)
		{
			string text = packet.getPayload()->c_str();
			string regex = command->getName();

			if(String::regexMatch(text,regex))
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
			string text = packet.getPayload()->c_str();
			string regex = command->getName();
			if(String::regexMatch(text,regex))
			{
				type = true;
			}
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
