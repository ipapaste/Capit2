/*
 * IValueGroup.hpp
 *
 *  Created on: Feb 16, 2012
 *      Author: issle
 */

#ifndef IVALUEGROUP_HPP_
#define IVALUEGROUP_HPP_

#include <iostream>
#include "../commons/Printable.hpp"
using namespace std;

/*
 * This class holds a list of values for each key string it holds.
 */
class IValueGroup: public Printable
{
public:
	/*
	 * Return one of the random values for the given string. Returns
	 * a random string if no value can be found.
	 */
	virtual string getRandomValue(string var) = 0;

	/*
	 * Adds a value for the given string var key.
	 */
	virtual void addValue(string var, string val) = 0;

	/*
	 * Not needed.
	 */
	virtual ~IValueGroup()
	{

	}

	virtual map<string,deque<string> > getAllValuesCopy() = 0;
};


#endif /* IVALUEGROUP_HPP_ */
