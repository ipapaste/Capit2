/*
 * IFlowType.hpp
 *
 *  Created on: Feb 16, 2012
 *      Author: issle
 */

#ifndef IFLOWTYPE_HPP_
#define IFLOWTYPE_HPP_
#include <iostream>
using namespace std;
#include "../commons/Printable.hpp"
#include "IFlowState.hpp"
class IFlowType:public Printable
{
public:
	virtual bool isUnknown() = 0;

	virtual void setIsUnknown() = 0;

	virtual ~IFlowType()
	{

	}

	virtual void setName(string name) = 0;

	virtual void setPort(int port) = 0;

	virtual void addState(IFlowState& state) = 0;

	virtual int size() = 0;

	virtual deque<IFlowState*> getFlowStates() = 0;

	virtual IFlowState& getFlowStateForPacket(Packet& packet) = 0;
};


#endif /* IFLOWTYPE_HPP_ */
