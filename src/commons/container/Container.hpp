/*
 * Container.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef CONTAINER_HPP_
#define CONTAINER_HPP_

#include <iostream>
#include <map>

using namespace std;

/**
 * A container is an STL map template guarded by a lock.
 */
template < class ContainerIndex, class ContainerValue , class Lock> class Container: public Lock
{
private:
	map<ContainerIndex, ContainerValue> content;

public:

	map<ContainerIndex,ContainerValue>* getMap()
	{
		return &content;
	}

	void addItem(ContainerIndex index, ContainerValue value)
	{
		Lock::lock();
		content[index] = value;
		Lock::unlock();
	}

	ContainerValue getItem(ContainerIndex index)
	{
		return content[index];
	}

	void removeItem(ContainerIndex index)
	{
		Lock::lock();
		if(hasItem(index))
			content[index]= 0;

		Lock::unlock();
	}

	bool hasItem(ContainerIndex index)
	{
		return content[index] == 0 ? false : true;
	}
};


#endif /* CONTAINER_HPP_ */
