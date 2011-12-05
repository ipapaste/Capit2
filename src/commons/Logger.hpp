/*
 * Logger.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <iostream>

using namespace std;

/**
 * A logger provides logging functionality
 * as a stand alone object or as a subclass
 * of another object.
 *
 * A Logger class template can be initialized
 * with a specific logging level. Any logging
 * attempt with a level lower than the level
 * of this class initialization will not be
 * logged.
 */
template < int level, class Lock > class Logger: public Lock
{

	enum { value = level };

public:

	void log(const char* message, int log_level)
	{
		Lock::lock();
		if(log_level > value)
			cout << message << endl;
		Lock::unlock();
	}
};


#endif /* LOGGER_HPP_ */
