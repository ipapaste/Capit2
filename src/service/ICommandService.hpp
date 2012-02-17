/*
 * ICommandService.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef ICOMMANDSERVICE_HPP_
#define ICOMMANDSERVICE_HPP_
#include "../model/ICommand.hpp"
#include "../commons/Printable.hpp"
class ICommandService:public Printable
{
public:
	virtual ICommand& getNewCommand() = 0;

	virtual ~ICommandService()
	{

	}
};


#endif /* ICOMMANDSERVICE_HPP_ */
