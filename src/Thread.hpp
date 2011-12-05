/*
 * Thread.hpp
 *
 *  Created on: Nov 27, 2011
 *      Author: issle
 */

#ifndef THREAD_HPP_
#define THREAD_HPP_

#include "pthread.h"

typedef pthread_mutex_t Mutex;
typedef pthread_cond_t Condition;
typedef pthread_t Thread;

static Mutex count_mutex;
static Condition count_threshold_cv;
static Thread daemonThread;
static int counter = 0;

class ThreadShell
{
public:

	Thread thread;
	Mutex fakeMutex;
	Condition fakeCond;

	static void lock(Mutex& m)
	{
		pthread_mutex_lock(&m);
	}

	static void unlock(Mutex& m)
	{
		pthread_mutex_unlock(&m);
	}

	static void condWait(Condition& c, Mutex& m)
	{
		pthread_cond_wait(&c, &m);
	}

	static void initialize(Mutex& m)
	{
		pthread_mutex_init(&m, NULL);

	}

	static void initialize(Condition& c)
	{
		pthread_cond_init(&c, NULL);
	}

	static void condSignal(Condition& c)
	{
		pthread_cond_signal(&c);
	}

	static void create(Thread& thread, void *(*start_routine)(void*), void *arg)
	{
		pthread_create(&thread, NULL, start_routine, arg);
	}

	static void initialize()
	{
		initialize(count_mutex);
		initialize(count_threshold_cv);
		create(daemonThread, daemon, (void*) daemonThread);
	}

	static void increaseCounter()
	{
		counter++;
		if (counter == 0)
			condSignal(count_threshold_cv);
		//pthread_cond_signal(&count_threshold_cv);
	}

	static void decreaseCounter()
	{
		counter--;
		if (counter == 0)
			condSignal(count_threshold_cv);
		//pthread_cond_signal(&count_threshold_cv);
	}

	static void schedule(ThreadShell* thread)
	{
		increaseCounter();
		thread->start();
	}

	static void scheduleAtFixedRate(ThreadShell& thread, int initDelay,
			int fixedDelay)
	{
		thread.setIsRate(true);
		thread.setDelay(initDelay);
		thread.setRateDelay(fixedDelay);
		increaseCounter();
		thread.start();
	}

	static void schedule(ThreadShell& thread, int initDelay)
	{
		thread.setIsRate(false);
		thread.setDelay(initDelay);
		increaseCounter();
		thread.start();
	}

	//---------------Implementation ----------------------


	/**
	 * The initial delay before this thread starts
	 * execuding code that belongs in the business
	 * logic.
	 */
	int delay_ms;

	/**
	 * The periodic delay in miliseconds between two
	 * continious thread exection.
	 */
	int rate_ms;

	/**
	 * If true, this thread will reschedule the
	 * execution of its run method after it finishes
	 * executing the previous run.
	 */
	bool fixed_rate;

	bool started;
	int isFirst;

	/**
	 * This pure virtual method should be implemented
	 * by the subclass(es) that inherit from this base
	 * class. This method should not handle any thread
	 * scheduling mechanics, it should only contain
	 * business logic code.
	 */
	virtual void run()=0;

	pthread_mutex_t singleLock;

	ThreadShell()
	{

		initialize(fakeMutex);
		initialize(singleLock);
		initialize(fakeCond);

		this->fixed_rate = false;
		this->delay_ms = 0;
		this->rate_ms = 0;

		started = false;
		isFirst = true;

	}

	void setIsRate(bool rate)
	{
		fixed_rate = rate;
	}

	void setRateDelay(int delay)
	{
		rate_ms = delay;
	}

	~ThreadShell()
	{

	}

	static void* runWrapper(void* obj)
	{
		ThreadShell* thread = (ThreadShell*) obj;
		ThreadShell::lock(thread->singleLock);
		thread->execute();
		ThreadShell::unlock(thread->singleLock);
	}

	void execute()
	{
		if (fixed_rate)
		{
			while (1 == 1)
			{
				wait(isFirst);
				run();
				isFirst = false;
			}
		}
		else
		{
			wait(isFirst);
			run();
			started = false;
		}
		decreaseCounter();
	}

	void start()
	{
		pthread_create(&thread, NULL, ThreadShell::runWrapper, (void*) this);
	}

	void wait(bool isFirst)
	{

		struct timespec timeToWait;

		clock_gettime(CLOCK_REALTIME, &timeToWait);

		int future_ms = rate_ms;
		if (isFirst)
			future_ms = delay_ms;

		int milliseconds = future_ms;
		int seconds = (int) (future_ms / 1000);

		if (seconds > 0)
			milliseconds = future_ms % (seconds * 1000);

		long delay_nanoseconds = timeToWait.tv_nsec + 1000000 * milliseconds;

		int extraSeconds = 0;
		if (delay_nanoseconds > 1000000000)
		{
			extraSeconds = (int) (delay_nanoseconds / 1000000000);
			if (extraSeconds > 0)
				delay_nanoseconds = delay_nanoseconds % (extraSeconds
						* 1000000000);
		}

		timeToWait.tv_sec += (seconds + extraSeconds);
		timeToWait.tv_nsec = delay_nanoseconds;

		pthread_mutex_lock(&fakeMutex);
		pthread_cond_timedwait(&fakeCond, &fakeMutex, &timeToWait);
		pthread_mutex_unlock(&fakeMutex);

	}

	void setDelay(int _delay_ms)
	{
		delay_ms = _delay_ms;
	}

	static void* daemon(void* args)
	{
		lock(count_mutex);
		condWait(count_threshold_cv, count_mutex);

		unlock(count_mutex);
	}

	static void join()
	{
		pthread_join(daemonThread, NULL);
	}

};

#endif /* THREAD_HPP_ */
