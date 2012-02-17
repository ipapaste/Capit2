/*
 * ValueGroup.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef VALUEGROUP_HPP_
#define VALUEGROUP_HPP_

#include "../IValueGroup.hpp"
#include <map>
using namespace std;

class ValueGroup: public IValueGroup
{
private:
	map<string,deque<string> > values;

public:
	map<string,deque<string> > getAllValuesCopy()
	{
		return values;
	}

	string getRandomValue(string var)
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

	void addValue(string var, string value)
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

	virtual ~ValueGroup()
	{

	}

	void print()
	{
		map<string,deque<string> >::iterator it;

		for(it = values.begin(); it != values.end(); it++)
		{
			cout<< "Key: " << it->first << " ";

			deque<string> seq = it->second;

			for(int i = 0; i < seq.size(); i++)
			{
				cout << seq[i] << " " ;
			}

			cout << endl;
		}
	}
};


#endif /* VALUEGROUP_HPP_ */
