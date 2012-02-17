/*
 * IFlowStateService.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef IFLOWSTATESERVICE_HPP_
#define IFLOWSTATESERVICE_HPP_

#include "../model/IFlowState.hpp"
#include "../commons/Printable.hpp"
class IFlowStateService:public Printable
{
public:
	virtual IFlowState& getNewFlowState() = 0;

	virtual ~IFlowStateService()
	{

	}
};


#endif /* IFLOWSTATESERVICE_HPP_ */
