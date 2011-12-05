/*
 * Queue.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef QUEUE_HPP_
#define QUEUE_HPP_

#include <queue>

template<class Type, class Lock> class Queue: public Lock
{
private:
	queue<Type> items;
public:
	void add(Type item)
	{
		Lock::lock();
		items.push(item);
		Lock::unlock();
	}

	Type getNext()
	{
		Lock::lock();
		if (items.empty())
		{
			Lock::unlock();
			return 0;
		}
		Type item = items.front();
		items.pop();
		Lock::unlock();
		return item;
	}

	Type peekNext()
	{
		Lock::lock();
		if (items.empty())
		{
			Lock::unlock();
			return 0;
		}
		Type item = items.front();
		Lock::unlock();
		return item;
	}

	bool isEmpty()
	{
		Lock::lock();
		if (items.empty())
		{
			Lock::unlock();
			return true;
		}
		Lock::unlock();
		return false;
	}
};

#endif /* QUEUE_HPP_ */
