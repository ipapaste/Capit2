/*
 * AheadReplayer.hpp
 *
 *  Created on: Feb 16, 2012
 *      Author: issle
 */

#ifndef AHEADREPLAYER_HPP_
#define AHEADREPLAYER_HPP_

#include "Packet.hpp"
#include "commons/concurrent/Thread.hpp"
#include "commons/Tools.hpp"
#include "commons/util/Date.hpp"
#include "commons/math/Math.hpp"

using namespace commons::util;

class AheadReplayer: public Thread
{
private:
	static const long AHEAD_THRESHOLD = 50000;

protected:
	long startingTime;

	long firstPacketTimestamp;

	long lastPacketTimestamp;

	long currentTime;

	long ahead;

public:

	AheadReplayer()
	{
		startingTime= Date::currentTimeInMillis();

		firstPacketTimestamp = -1;

		lastPacketTimestamp = 0;

		currentTime = 0;

		ahead = 0;
	}
	virtual void doWork(Packet* packet) = 0;

	virtual Packet* generate() = 0;

	virtual void calculate(Packet* packet)
	{
		if(firstPacketTimestamp ==-1)
			firstPacketTimestamp = packet->getTimestamp();

		lastPacketTimestamp = packet->getTimestamp();

		currentTime = Date::currentTimeInMillis();

		long packetCoveredTime = lastPacketTimestamp - firstPacketTimestamp;

		long applicationDelay = currentTime - startingTime;

		ahead = Math<long>::max(0,packetCoveredTime - applicationDelay);
	}

	void run()
	{
		startingTime= Date::currentTimeInMillis();

		firstPacketTimestamp = -1;

		lastPacketTimestamp = 0;

		currentTime = 0;

		ahead = 0;

		while(ahead < AHEAD_THRESHOLD)
		{
			Packet* packet = generate();
			calculate(packet);
			doWork(packet);

		}

		setDelay(ahead);

		execute();
	}
};


#endif /* AHEADREPLAYER_HPP_ */
