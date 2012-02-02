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
#include "../../ValueGroup.hpp"
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

	string getVariableCommand(ValueGroup g)
	{
		string dup(command_);
		map<string,deque<string> > myMap = g.getAllValues();
		map<string,deque<string> >::iterator  iter;
		for(iter = myMap.begin(); iter != myMap.end(); ++iter)
		{
			string key =  iter->first;
			string regex("&"+key+"&");
			String::replace(dup,regex,g.getValue(iter->first));
		}

		String::replace(dup,"&var", getRandom());
		dup.append("\r\n");
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
		return String::getRandom();
	}
};

#endif /* COMMAND_HPP_ */
