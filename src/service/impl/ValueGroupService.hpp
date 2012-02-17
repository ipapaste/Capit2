/*
 * ValueGroupService.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef VALUEGROUPSERVICE_HPP_
#define VALUEGROUPSERVICE_HPP_

#include "../IValueGroupService.hpp"
#include "../../model/impl/ValueGroup.hpp"
#include <map>
#include <deque>

using namespace std;
class ValueGroupService: public IValueGroupService
{
private:
	map<int,deque<IValueGroup*> > groups;
public:

	IValueGroup& getNewValueGroup()
	{
		IValueGroup* group = new ValueGroup();
		return *group;
	}

	virtual void addGroup(IValueGroup& g, int port)
	{
		groups[port].push_back(&g);
	}

	virtual ~ValueGroupService()
	{

	}

	void print()
	{
		map<int,deque<IValueGroup*> >::iterator it;

		for(it = groups.begin(); it != groups.end(); it++)
		{
			deque<IValueGroup*> group = it->second;

			for(int i = 0; i < group.size(); i++)
			{
				cout << "----------- Value Group ------------" << endl;
				group[i]->print();
			}
		}
	}
};


#endif /* VALUEGROUPSERVICE_HPP_ */
