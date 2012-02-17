/*
 * FlowState.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef FLOWSTATE_HPP_
#define FLOWSTATE_HPP_
#include "../IFlowState.hpp"
#include "../IMatrix.hpp"
#include "../ICommand.hpp"
#include <deque>
#include <iostream>
#include <list>
#include <string>
#include "Packet.hpp"
#include <boost/foreach.hpp>
#include "commons/tools/String.hpp"
#include <queue>
using namespace std;

class FlowState: public IFlowState
{
private:
	static const int TABLE_SIZE = 1100;
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

	bool unknown;

	/*
	 * A template packet command list that define
	 * the FlowState type.
	 */
	deque<ICommand*> commands;

	string name_;

	vector<int> transitions;
public:

	FlowState()
	{
		unknown = false;
	}
	void setTransitions(IMatrix& group)
	{
		vector<double> trans = group.getLineCopy(id_);

		transitions.resize(TABLE_SIZE);

		for (int i = 0; i < TABLE_SIZE; i++)
		{
			transitions[i] = -1;
		}

		int index = 0;
		for (int i = 0; i < trans.size(); i++)
		{
			float prob = trans[i];

			int count = 0;

			count = index + TABLE_SIZE * prob;

			if (count == 0)
				continue;

			for (int j = index; j < count; j++)
			{
				transitions[j] = i;
				cout << i;
			}

			index = count + 1;
		}
	}

	int getRandomTransition()
	{
		int dice = Rnd::getInt(0, transitions.size() - 1);

		return transitions[dice];
	}

	virtual ~FlowState()
	{

	}

	/*
	 * Gets all the commands that correspond to this FlowState.
	 */
	deque<ICommand*> getCopyCommands()
	{
		deque<ICommand*> commands;
		return commands;
	}

	void setId(int id)
	{
		id_ = id;
	}

	void setName(string name)
	{
		name_ = name;
	}

	int getId()
	{
		return id_;
	}

	string getName()
	{
		return name_;
	}

	void setIsUnknown()
	{
		unknown = true;
	}

	bool isUnknown()
	{
		return unknown;
	}

	void addCommand(ICommand& command)
	{
		commands.push_back(&command);
	}

	void print()
	{
		cout << "State: " << id_ << endl;

		for (int i = 0; i < commands.size(); i++)
		{
			commands[i]->print();
		}
	}

	void accept(Packet& packet)
	{
		bool type = false;

		for (int i = 0; i < commands.size(); i++)
		{
			string text = packet.getPayload()->c_str();
			string regex = commands[i]->getName();
			if (String::regexMatch(text, regex))
			{
				type = true;
			}
		}
		packets.push_front(&packet);
		count++;
	}

	void calc()
	{
		if (packets.size() <= 1)
		{
			meanDelay = 0;
			return;
		}

		list<Packet*>::iterator it;

		Packet* old = NULL;

		BOOST_FOREACH(Packet* it, packets)
				{
					if (old == NULL)
					{
						old = it;
						continue;
					}
					else
					{
						meanDelay += (old->getTimestamp() - it->getTimestamp())
								/ count;
					}
					old = it;
				}
	}

	IFlowState& belongsToState(Packet& packet)
	{
		for (int i = 0; i < commands.size(); i++)
		{
			string text = packet.getPayload()->c_str();
			string regex = commands[i]->getName();

			if (String::regexMatch(text, regex))
			{
				return *(new FlowState(*this));
			}
		}

		IFlowState* st = new FlowState();
		st->setIsUnknown();

		return *st;
	}


};

#endif /* FLOWSTATE_HPP_ */
