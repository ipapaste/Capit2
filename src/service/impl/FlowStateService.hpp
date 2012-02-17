/*
 * FlowStateService.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef FLOWSTATESERVICE_HPP_
#define FLOWSTATESERVICE_HPP_

#include "../../model/impl/FlowState.hpp"
#include "../../model/IFlowState.hpp"
#include "../IFlowStateService.hpp"

class FlowStateService:public IFlowStateService
{
	IFlowState& getNewFlowState()
	{
		IFlowState* state= new FlowState();

		return *state;
	}

	virtual ~FlowStateService()
	{

	}

	void print()
	{

	}
};


#endif /* FLOWSTATESERVICE_HPP_ */
