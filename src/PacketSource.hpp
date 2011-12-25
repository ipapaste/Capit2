/*
 * PacketSource.hpp
 *
 *  Created on: Nov 25, 2011
 *      Author: issle
 */

#ifndef PACKETSOURCE_HPP_
#define PACKETSOURCE_HPP_
#include "Packet.hpp"
#include "PacketSourcePolicies/PcapAdapter.hpp"
#include "ClientManager.hpp"
#include "Thread.hpp"
#include "commons/Tools.hpp"

template <class PacketSourceAdapter> class PacketSourceShell: public PacketSourceAdapter
{
public:

};

typedef PacketSourceShell<PcapAdapter> PacketSourceType;

typedef Entity2<PacketSourceType,ThreadShell> PacketSource1;

class PacketSource:public PacketSource1
{
public:
	void run(){
		int packetCount = 0;
		int initialDelay = 0;
		int lastDelay = 0;
		while(packetCount < 5)
		{

			Packet* packet1 = getNextPacket();
			if(packet1 == NULL)
			{
				ClientManagerInstance::getInstance()->removeSource();
				return;
			}

			ClientManagerInstance::getInstance()->accept(*packet1);
			if(packetCount == 0)
				initialDelay = Tools::getTimeInMillis(packet1->getTimestamp());

			if(packetCount == 5 -1)
				lastDelay = Tools::getTimeInMillis(packet1->getTimestamp());

			packetCount++;

		}
		//cout << "Delay is: " << lastDelay - initialDelay<< endl;
		this->setDelay(lastDelay - initialDelay);
		ThreadShell::schedule(this);
	}
};

#endif /* PACKETSOURCE_HPP_ */
