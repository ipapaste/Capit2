/*
 * ICommand.hpp
 *
 *  Created on: Feb 16, 2012
 *      Author: issle
 */

#ifndef ICOMMAND_HPP_
#define ICOMMAND_HPP_

#include "IValueGroup.hpp"
#include "../commons/Printable.hpp"
class ICommand:public Printable
{
public:
	/*
	 * Returns a payload-ready string to be passed in a packet
	 * that will travel the network as part of synthetic traffic.
	 */
	virtual string getVariableCommand(IValueGroup& g) = 0;

	/*
	 * Gets the command structure as a string.
	 */
	virtual string getCommand() = 0;

	/*
	 * Gets the name of this command.
	 */
	virtual string getName() = 0;

	virtual void setName(string name) = 0;

	virtual void setCommand(string command) = 0;


	virtual ~ICommand()
	{

	}
};


#endif /* ICOMMAND_HPP_ */
