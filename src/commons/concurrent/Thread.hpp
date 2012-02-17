/*
 * Thread.hpp
 *
 *  Created on: Feb 16, 2012
 *      Author: issle
 */

#ifndef THREAD_HPP_
#define THREAD_HPP_

#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
using namespace std;

class Thread
{
private:
	long delay_;

	boost::shared_ptr<boost::thread> m_thread;

	void doRun()
	{
		boost::posix_time::milliseconds workTime(delay_);

		boost::this_thread::sleep(workTime);
		run();
	}

public:
	/*
	 * Creates an thread with zero delay.
	 */
	Thread()
	{
		delay_ = 0;
	}

	virtual ~Thread()
	{
	}

	/*
	 * Sets the execution delay of this thread in ms.
	 */
	void setDelay(long delay)
	{
		delay_ = delay;
	}


	/*
	 * Executes the thread.
	 */
	void execute()
	{
		m_thread = boost::shared_ptr < boost::thread
				> (new boost::thread(boost::bind(&Thread::doRun, this)));
	}

	/*
	 * Executes the thread after <delay> ms.
	 */
	void execute(long delay)
	{
		setDelay(delay);
		m_thread = boost::shared_ptr < boost::thread
				> (new boost::thread(boost::bind(&Thread::doRun, this)));
	}

	/*
	 * Joins runtime with this thread.
	 */
	void join()
	{
		assert(m_thread);
		m_thread->join();
	}

	/*
	 * Implement this for business logic execution.
	 */
	virtual void run() = 0;
};


#endif /* THREAD_HPP_ */
