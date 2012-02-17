/*
 * FlowTypeService.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef FLOWTYPESERVICE_HPP_
#define FLOWTYPESERVICE_HPP_

#include "../../model/impl/FlowType.hpp"
#include "../../model/IFlowType.hpp"
#include "../IFlowTypeService.hpp"

class FlowTypeService:public IFlowTypeService
{
private:
	map<int,IFlowType*> flowTypes;
public:

	void addType(int id, IFlowType& type)
	{
		flowTypes[id] = &type;
	}

	IFlowType& getType(int port)
	{
		IFlowType* type = NULL;

		type = flowTypes[port];

		if(type == NULL)
		{
			type = &getNewFlowType();
			type->setIsUnknown();
		}

		return *type;
	}

	IFlowType& getNewFlowType()
	{
		FlowType* type = new FlowType();

		return *type;
	}


	virtual ~FlowTypeService()
	{

	}

	void print()
	{
		map<int,IFlowType*>::iterator it;

		for(it = flowTypes.begin(); it != flowTypes.end(); it++)
		{
			IFlowType* type = it->second;

			type->print();
		}
	}
};


#endif /* FLOWTYPESERVICE_HPP_ */
