/*
 * Lock.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef LOCK_HPP_
#define LOCK_HPP_

#include "../Thread.hpp"

/**
 * This class represents a lock interface.
 * Threads can try to acquire the lock by
 * calling lock() and can release the lock
 * by calling unlock().
 *
 * The strategy of the locking system must
 * be defined at subclasses by implementing
 * the two pure virtual methods that this
 * interface exposes.
 */
class AbstractLock
{
	/*
	 * Attempts to lock this lock object.
	 */
	virtual void lock() = 0;

	/*
	 * Releases / unlocks this object.
	 */
	virtual void unlock() = 0;
};

/**
 * This class represents an empty lock.
 * An empty lock does not implement any
 * locking functionality but it does declare
 * both the lock and the unlock methods
 * since it must comply to a specific
 * interface that is catholic for all
 * the locking classes.
 */
class NoLock: public AbstractLock
{
public:
	void lock()
	{

	}

	void unlock()
	{

	}
};

/**
 * This class implements the Lock interface
 * using POSIX Mutex implementation.
 */
class MutexLock: public AbstractLock
{
public:
	Mutex mutex;

	MutexLock()
	{
		ThreadShell::initialize(mutex);
	}

	void lock()
	{
		ThreadShell::lock(mutex);
	}

	void unlock()
	{
		ThreadShell::unlock(mutex);
	}
};



#endif /* LOCK_HPP_ */
