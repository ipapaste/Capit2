/*
 * IFlowTypeService.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef IFLOWTYPESERVICE_HPP_
#define IFLOWTYPESERVICE_HPP_

#include "../model/IFlowType.hpp"
#include "../commons/Printable.hpp"
class IFlowTypeService:public Printable
{
public:
	virtual IFlowType& getNewFlowType() = 0;


	virtual void addType(int id, IFlowType& type) = 0;

	virtual IFlowType& getType(int port) = 0;

	virtual ~IFlowTypeService()
	{

	}
};


#endif /* IFLOWTYPESERVICE_HPP_ */
