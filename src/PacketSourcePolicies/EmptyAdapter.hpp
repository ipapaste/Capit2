/*
 * EmptyAdapter.hpp
 *
 *  Created on: Nov 25, 2011
 *      Author: issle
 */

#ifndef EMPTYADAPTER_HPP_
#define EMPTYADAPTER_HPP_
#include "../Packet.hpp"

class EmptyAdapter
{
public:

	void openSource(const char* source)
	{

	}

	Packet& getNextPacket()
	{
		return NULL;
	}
};

#endif /* EMPTYADAPTER_HPP_ */
