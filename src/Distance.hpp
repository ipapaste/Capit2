/*
 * Distance.hpp
 *
 *  Created on: Nov 30, 2011
 *      Author: issle
 */

#ifndef DISTANCE_HPP_
#define DISTANCE_HPP_

template < class ReturnType, class Packet> class IDistance
{
public:
	virtual ReturnType getDistance(Packet packet1, Packet packet2) = 0;
};

#endif /* DISTANCE_HPP_ */
