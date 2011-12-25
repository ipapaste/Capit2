/*
 * AbstractApplication.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef ABSTRACTAPPLICATION_HPP_
#define ABSTRACTAPPLICATION_HPP_

#include "commons/Logger.hpp"
#include "commons/Entity.hpp"
#include "commons/Lock.hpp"
#include "commons/Queue.hpp"
#include "commons/Socket.hpp"

#include "Node.hpp"
#include "Thread.hpp"

/**
 * Should have a lock since two threads will
 * access it to print data. The first is the
 * thread that represents a packet source
 * that is sending packets to this application.
 * The second is the application itself since
 * it is sending the packets to its output
 * channel asynchronously.
 */
typedef Logger<1, MutexLock> ApplicationLogger;

/**
 * This represents the type of a Queue that
 * contains pointers to packets and the write
 * operation is guarded by a POSIX mutex.
 */
typedef Queue<AbstractNode::PacketType*, MutexLock> STLQueue;

/**
 * This type represents a hierarchy of classes
 * that define how an application is decomposed
 * into smaller orthogonal policies.
 *
 * An Application is:
 *
 * 1)An independent thread.
 *
 * 2)A the last Node of our system that accepts
 * packets.
 *
 * 3)A logger that can print out information
 * about its execution.
 *
 * 4)A Queue of packets stored and waiting to
 * be sent over the output channel.
 *
 * 5)A Socket that is used to send packets to
 * a server IP.
 *
 * The application receives packets from a packet
 * source and sends them over the output channel
 * asynchronously. As a thread it is waiting on
 * a packet arrived condition and once its signaled
 * it keeps sending packets till its Queue gets
 * empty. And this continues till the Source
 * informs the application that there are no more
 * packets to be sent, which is when the application
 * terminates its thread.
 */
typedef Entity5<ThreadShell, AbstractNode, ApplicationLogger, STLQueue,
		BoostSocket> AbstractApplicationType;

class AbstractApplication: public AbstractApplicationType
{
private:

	/*
	 *  The application status.
	 */
	int status;

	/**
	 * No packets received yet.
	 */
	static const int INACTIVE = 0;

	/**
	 * Currently sending packets.
	 */
	static const int ACTIVE = 1;

	/**
	 * Waiting on a packet arrived condition.
	 */
	static const int SLEEPING = 2;

	/*
	 * Used for guarded access to the status.
	 */
	MutexLock lock;

public:

	/**
	 * Each application should be initialized as inactive.
	 */
	AbstractApplication()
	{
		status = INACTIVE;
	}

	/**
	 * A PacketSource that acts as a different thread calls
	 * this method to send packets to this Application. The
	 * packets are stored in the Queue policy.
	 */
	void accept(AbstractApplicationType::PacketType& packet)
	{
		add(&packet);

		lock.lock();

		if (status == SLEEPING)
		{
			status = ACTIVE;
			lock.unlock();

			ThreadShell::condSignal(fakeCond);
		}
		else if (status == INACTIVE)
		{
			connect(*packet.getDestinationIp(), packet.getDestinationPort());

			status = ACTIVE;

			lock.unlock();

			ThreadShell::schedule(this);
		}
		else
		{
			lock.unlock();
		}

	}

	/*
	 * This method runs on the applications thread asynchronously
	 * in regards to the rest of the process. Its role is to read
	 * packets from the Queue, pre - process them and send them
	 * over the output channel.
	 */
	void run()
	{
		while (1==1)
		{
			executeCode();
		}

	}

	void executeCode()
	{
		/**
		 * Should never be null since run() is called
		 * after a PacketSource adds the first packet
		 * to the Queue or after the application is
		 * signaled of the existence of packets inside
		 * the Queue.
		 */
		Packet* packet = getNext();

		cout << "Sending:" << *packet->getPayload() << endl;

		if (packet->getPayload() != 0)
			sendData((u_char*) packet->getPayload()->c_str());

		delete packet;

		if (isEmpty())
		{
			lock.lock();
			status = SLEEPING;
			lock.unlock();
			ThreadShell::condWait(fakeCond, fakeMutex);
		}

	}
};

#endif /* ABSTRACTAPPLICATION_HPP_ */
