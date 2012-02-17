/*
 * IValueGroupService.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef IVALUEGROUPSERVICE_HPP_
#define IVALUEGROUPSERVICE_HPP_

#include "../model/IValueGroup.hpp"
#include "../commons/Printable.hpp"
class IValueGroupService:public Printable
{
public:
	virtual IValueGroup& getNewValueGroup() = 0;

	virtual void addGroup(IValueGroup& g, int port) = 0;

	virtual ~IValueGroupService()
	{

	}
};


#endif /* IVALUEGROUPSERVICE_HPP_ */
