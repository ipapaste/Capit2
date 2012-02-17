/*
 * Command.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include "../ICommand.hpp"
#include "../IValueGroup.hpp"
#include "../../commons/tools/String.hpp"
#include <iostream>

using namespace std;
class Command: public ICommand
{
private:
	string name_;
	string command_;
public:
	/*
	 * Returns a payload-ready string to be passed in a packet
	 * that will travel the network as part of synthetic traffic.
	 */
	string getVariableCommand(IValueGroup& g)
	{
		string dup(command_);
		map<string,deque<string> > myMap = g.getAllValuesCopy();
		map<string,deque<string> >::iterator  iter;
		for(iter = myMap.begin(); iter != myMap.end(); ++iter)
		{
			string key =  iter->first;
			string regex("&"+key+"&");
			String::replace(dup,regex,g.getRandomValue(iter->first));
		}

		String::replace(dup,"&var", String::getRandom());
		dup.append("\r\n");
		return dup;
	}

	/*
	 * Gets the command structure as a string.
	 */
	string getCommand()
	{
		return command_;
	}

	/*
	 * Gets the name of this command.
	 */
	string getName()
	{
		return name_;
	}

	void setName(string name)
	{
		name_ = name;
	}

	void setCommand(string command)
	{
		command_ = command;
	}

	void print()
	{
		cout << "Command: " << command_ << " Name: " << name_ << endl;
	}

	~Command()
	{

	}
};



#endif /* COMMAND_HPP_ */
