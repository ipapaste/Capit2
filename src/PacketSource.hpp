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
#include "commons/concurrent/Thread.hpp"
#include "commons/Tools.hpp"
#include "commons/util/Date.hpp"
#include "commons/math/Math.hpp"

using namespace commons::util;

typedef Entity2<PcapAdapter,Thread> ActiveSource;

class PacketSource:public ActiveSource
{
private:
	static const long AHEAD_THRESHOLD = 50000;
public:
	void run()
	{
		long startingTime= Date::currentTimeInMillis();

		long firstPacketTimestamp = -1;

		long lastPacketTimestamp;

		long currentTime;

		long ahead = 0;

		while(ahead < AHEAD_THRESHOLD)
		{
			Packet* packet = getNextPacket();

			if(packet == NULL)
			{
				//ClientManagerInstance::getInstance().removeSource();
				return;
			}

			if(packet->isUnknown())
			{
				delete packet;
				continue;
			}

			ClientManagerInstance::getInstance().accept(*packet);

			if(firstPacketTimestamp ==-1)
				firstPacketTimestamp = packet->getTimestamp();

			lastPacketTimestamp = packet->getTimestamp();

			currentTime = Date::currentTimeInMillis();

			long packetCoveredTime = lastPacketTimestamp - firstPacketTimestamp;

			long applicationDelay = currentTime - startingTime;

			ahead = Math<long>::max(0,packetCoveredTime - applicationDelay);
		}

		setDelay(ahead);

		execute();
	}
};

#endif /* PACKETSOURCE_HPP_ */
