/*
 * Daemon.hpp
 *
 *  Created on: Feb 16, 2012
 *      Author: issle
 */

#ifndef DAEMON_HPP_
#define DAEMON_HPP_
#include "Thread.hpp"

class Daemon:public Thread
{
public:
	void run()
	{
		//Do Nothing.
	}

	virtual ~Daemon()
	{

	}
};


#endif /* DAEMON_HPP_ */
