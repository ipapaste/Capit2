/*
 * ValueGroup.hpp
 *
 *  Created on: Feb 2, 2012
 *      Author: issle
 */

#ifndef VALUEGROUP_HPP_
#define VALUEGROUP_HPP_

#include <iostream>
#include <deque>
#include <map>
#include <iostream>
#include "commons/tools/String.hpp"

using namespace std;

class ValueGroup
{
private:
	map<string,deque<string> > values;

public:
	string getValue(string var)
	{
		if(values.count(var) != 0 && values[var].size() !=0)
		{
			if(values[var].size() == 1)
				return values[var].front();
			else
				return values[var][Rnd::getInt(0,values[var].size()-1)];
		}
		else
		{
			return String::getRandom();
		}
	}

	void setValue(string var, string value)
	{
		if(values.count(var) == 0)
		{
			deque<string> s;
			s.push_back(value);
			values[var] = s;
		}
		else
			values[var].push_back(value);
	}

	map<string,deque<string> > getAllValues()
	{
		return values;
	}
};


#endif /* VALUEGROUP_HPP_ */
