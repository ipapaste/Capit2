/*
 * AbstractDistance.hpp
 *
 *  Created on: Nov 30, 2011
 *      Author: issle
 */

#ifndef ABSTRACTDISTANCE_HPP_
#define ABSTRACTDISTANCE_HPP_

template <typename ReturnT, class Packet> class Distance
{
public:
	typedef ReturnT ReturnType;
	typedef Packet PacketType;

	ReturnType getDistance(PacketType& packet1, PacketType& packet2)
	{
		u_char* payload1 = packet1.getPayload();
		u_char* payload2 = packet2.getPayload();

		int length = str_len(payload1)


	}
}


#endif /* ABSTRACTDISTANCE_HPP_ */
