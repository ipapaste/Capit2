/*
 * Flow.hpp
 *
 *  Created on: Dec 13, 2011
 *      Author: issle
 */

#ifndef FLOW_HPP_
#define FLOW_HPP_

#include "commons/Queue.hpp"
#include "commons/Lock.hpp"
#include "Packet.hpp"
#include "Feature.hpp"

typedef Queue<Packet*,NoLock> FlowQueue;

class Flow: public FlowQueue
{
private:
	Feature* feature;
};

#endif /* FLOW_HPP_ */
