/*
 * ValueGroupManager.hpp
 *
 *  Created on: Feb 2, 2012
 *      Author: issle
 */

#ifndef VALUEGROUPMANAGER_HPP_
#define VALUEGROUPMANAGER_HPP_

#include <deque>
#include <map>
#include <iostream>
#include "commons/math/Rnd.hpp"
#include "ValueGroup.hpp"

using namespace std;

class ValueGroupManager
{
private:
	static map<int,deque<ValueGroup> > groups;
public:
	static ValueGroup getRandomGroup(int port)
	{
		int dice = Rnd::getInt(0,groups[port].size()-1);

		return groups[port][dice];
	}

	static void addGroup(ValueGroup g, int port)
	{
		groups[port].push_back(g);
	}
};

map<int, deque<ValueGroup> >ValueGroupManager::groups;

#endif /* VALUEGROUPMANAGER_HPP_ */
