/*
 * CapitService.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef CAPITSERVICE_HPP_
#define CAPITSERVICE_HPP_

#include "IFlowTypeService.hpp"
#include "impl/FlowTypeService.hpp"
#include "IFlowStateService.hpp"
#include "impl/FlowStateService.hpp"
#include "ICommandService.hpp"
#include "impl/CommandService.hpp"
#include "impl/ValueGroupService.hpp"
#include "IValueGroupService.hpp"
#include "../commons/container/Singleton.hpp"

class CapitService_
{
private:
	IFlowTypeService& flowType_;
	IFlowStateService& flowState_;
	ICommandService& commandService_;
	IValueGroupService& valueGroupService_;

public:

	CapitService_() :
			flowType_(*(new FlowTypeService())), flowState_(
					*(new FlowStateService())),commandService_(*(new CommandService())),valueGroupService_(
					*(new ValueGroupService()))
	{

	}

	IFlowTypeService& getFlowTypeService()
	{
		return flowType_;
	}

	IFlowStateService& getFlowStateService()
	{
		return flowState_;
	}
	ICommandService& getCommandService()
	{
		return commandService_;
	}
	IValueGroupService& getValueGroupService()
	{
		return valueGroupService_;
	}

	void print()
	{
		flowType_.print();
		flowState_.print();
		commandService_.print();
		valueGroupService_.print();
	}
};

typedef Singleton<CapitService_> CapitService;

#endif /* CAPITSERVICE_HPP_ */
