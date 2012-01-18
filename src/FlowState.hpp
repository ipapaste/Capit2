/*
 * FlowState.hpp
 *
 *  Created on: Jan 18, 2012
 *      Author: issle
 */

#ifndef FLOWSTATE_HPP_
#define FLOWSTATE_HPP_

#include <iostream>
#include <string>
#include "Node.hpp"
#include "Packet.hpp"
#include <boost/foreach.hpp>
using namespace std;

class FlowState: public AbstractNode
{
private:
	int id_;
	float meanDelay;
	float stdDelay;
	int count;
	list<Packet*> packets;
public:

	FlowState(int id)
	{
		id_ = id;
	}
	void accept(Packet& packet)
	{
		packets.push_front(&packet);
		count++;
	}

	void calc()
	{
		cout << "Calculating a flow state." << endl;

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
};


#endif /* FLOWSTATE_HPP_ */
