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
#include "../../commons/math/Rnd.hpp"
using namespace std;

class Command
{
private:
	string command_;
	string name_;
public:
	Command(string name, string command):name_(name),command_(command)
	{

	}

	string getVariableCommand(string var)
	{

		string dup(command_);
		String::replace(dup, "&var", var);
		return dup;
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

	string getRandom()
	{
		return getVariableCommand("sdfkjosdf");
	}
};

#endif /* COMMAND_HPP_ */
