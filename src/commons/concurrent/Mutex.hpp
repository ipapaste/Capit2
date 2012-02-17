/*
 * Mutex.hpp
 *
 *  Created on: Feb 16, 2012
 *      Author: issle
 */

#ifndef MUTEX_HPP_
#define MUTEX_HPP_

#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
using namespace std;

class Mutex
{
private:
	boost::mutex mutex;
public:
	void lock()
	{
		mutex.lock();
	}

	void unlock()
	{
		mutex.unlock();
	}
};


#endif /* MUTEX_HPP_ */
