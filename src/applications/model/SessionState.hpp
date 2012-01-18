/*
 * SessionState.hpp
 *
 *  Created on: Jan 6, 2012
 *      Author: issle
 */

#ifndef SESSIONSTATE_HPP_
#define SESSIONSTATE_HPP_

#include "../../Node.hpp"

class SessionState: public AbstractNode
{
private:
	float delayMean;
	float delayStd;
	int count;
	queue<AbstractNode::PacketType*> packets;

public:
	void accept(AbstractNode::PacketType& packet)
	{
		count++;
		packets.push(&packet);
	}

	void calc()
	{
		Packet* previousPacket = NULL;
		for(int i = 0; i < packets.size(); i++)
		{
			if(previousPacket == NULL)
				previousPacket = packets[i];
			else
			{
				Packet* currentPacket = packets[i];
				delayMean += (currentPacket->getTimestamp() - previousPacket->getTimestamp())/count;
			}
		}
	}
};

class Flow: public AbstractNode
{
private:
	int sessionType_;
	vector<>
	map<int,SessionState*> states;
public:
	void accept(AbstractNode::PacketType& packet)
	{
		int sessionType = getSessionType(packet);

		SessionState* state = states[sessionType];

		state->accept(packet);

		if(sessionType == sessionType_)
		{

		}
	}

	int getSessionType(AbstractNode::PacketType& packet)
	{
		if(packet.getPayload() == "USER")
		{

		}
	}
};

#endif /* SESSIONSTATE_HPP_ */
