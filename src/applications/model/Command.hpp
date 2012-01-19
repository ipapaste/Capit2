/*
 * Command.hpp
 *
 *  Created on: Jan 6, 2012
 *      Author: issle
 */

#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include <iostream>
#include "../../commons/tools/String.hpp"

using namespace std;

class Command
{
private:
	string command_;
	string name_;
	vector<string> variables;
public:
	Command(string name, string command):name_(name),command_(command)
	{

	}

	string& getVariableCommand(string var)
	{
		string* command = new string(command_);

		String::replace(*command, "<var>", var);

		return *command;
	}
	string getCommand()
	{
		return command_;
	}

	string getName()
	{
		return name_;
	}
	void print()
	{
		cout << name_ << " " << command_ << endl;
	}
};


#endif /* COMMAND_HPP_ */
