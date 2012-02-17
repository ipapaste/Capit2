/*
 * CommandService.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef COMMANDSERVICE_HPP_
#define COMMANDSERVICE_HPP_
#include "../../model/impl/Command.hpp"
#include "../ICommandService.hpp"

class CommandService: public ICommandService
{
public:
	ICommand& getNewCommand()
	{
		ICommand* com = new Command();
		return *com;
	}

	void print()
	{

	}
};


#endif /* COMMANDSERVICE_HPP_ */
