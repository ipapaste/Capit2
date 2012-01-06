/*
 * Command.hpp
 *
 *  Created on: Jan 6, 2012
 *      Author: issle
 */

#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include <iostream>
#include "commons/tools/String.hpp"

using namespace std;

class Command
{
private:
	string command_;
	vector<string> variables;
public:
	Command(string command)
	{
		command_ = command;
	}

	string& getVariableCommand(string var)
	{
		string* command = new string(command_);

		String::replace(*command, "<var>", var);

		return *command;
	}
};


#endif /* COMMAND_HPP_ */
