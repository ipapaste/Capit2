/*
 * AbstractApplication.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef ABSTRACTAPPLICATION_HPP_
#define ABSTRACTAPPLICATION_HPP_

#include "commons/logging/Logger.hpp"
#include "commons/container/Entity.hpp"
#include "commons/concurrent/Mutex.hpp"
#include "commons/container/Queue.hpp"
#include "commons/network/Socket.hpp"
#include "commons/Printable.hpp"

#include "Node.hpp"
#include "commons/concurrent/Thread.hpp"
#include "applications/AbstractPreprocessor.hpp"
#include <deque>

/**
 * Should have a lock since two threads will
 * access it to print data. The first is the
 * thread that represents a packet source
 * that is sending packets to this application.
 * The second is the application itself since
 * it is sending the packets to its output
 * channel asynchronously.
 */
typedef Logger<1, Mutex> ApplicationLogger;

/**
 * This represents the type of a Queue that
 * contains pointers to packets and the write
 * operation is guarded by a POSIX mutex.
 */
typedef Queue<AbstractNode::PacketType*, Mutex> STLQueue;

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
typedef Entity5<Thread, AbstractNode, ApplicationLogger, STLQueue, BoostSocket> AbstractApplicationType;

class AbstractApplication: public AbstractApplicationType, public Printable
{
private:

	static const long SLEEP_TIME_ON_EMPTY = 3000;

	int port_;
	/*
	 *  The application status.
	 */
	int status;

	/*
	 * Used for guarded access to the status.
	 */
	Mutex lock;

	deque<AbstractPreprocessor*> preprocs;

	string previousResponce;

	long lastTimestamp_;

public:

	void addPreprocessor(AbstractPreprocessor* pr)
	{
		preprocs.push_back(pr);
	}

	/**
	 * Each application should be initialized as inactive.
	 */
	AbstractApplication(int port)
	{
		port_ = port;
		lastTimestamp_ = -1;
	}

	/**
	 * A PacketSource that acts as a different thread calls
	 * this method to send packets to this Application. The
	 * packets are stored in the Queue policy.
	 */
	virtual void accept(AbstractApplicationType::PacketType& packet)
	{
		add(&packet);

		lock.lock();

		long packetTimestamp = packet.getTimestamp();

		string destIp = *packet.getDestinationIp();
		int destPort = packet.getDestinationPort();

		if (lastTimestamp_ == -1)
		{
			cout << "Connecting to host " << destIp << " at port " << destPort
					<< " ..." << endl;
			connect(destIp, destPort);
			cout << "Successfully connected to host " << destIp << endl;
			lastTimestamp_ = packetTimestamp;

			int delay = packetTimestamp - lastTimestamp_;

			execute();
		}



		lock.unlock();

	}

	/*
	 * This method runs on the applications thread asynchronously
	 * in regards to the rest of the process. Its role is to read
	 * packets from the Queue, pre - process them and send them
	 * over the output channel.
	 */
	void run()
	{
		/**
		 * Should never be null since run() is called
		 * after a PacketSource adds the first packet
		 * to the Queue or after the application is
		 * signaled of the existence of packets inside
		 * the Queue.
		 */
		Packet* packet = getNext();

		if(packet == NULL)
		{
			execute(SLEEP_TIME_ON_EMPTY);
			return;
		}

		cout << "Sending:" << *packet->getPayload() << endl;

		for (int i = 0; i < preprocs.size(); i++)
		{
			preprocs[i]->preprocess(*packet, previousResponce);
		}

		if (packet->getPayload() != 0)
		{
			previousResponce = sendData(
					(u_char*) packet->getPayload()->c_str());
		}

		delete packet;

		Packet* next = peekNext();

		if(next == NULL)
			execute(SLEEP_TIME_ON_EMPTY);
		else
		{
			int delay = next->getTimestamp() - lastTimestamp_;
			execute(delay);
		}
	}

	void print()
	{
		cout << "[Application:" << port_ << "] Statistics:" << endl;
		for (int i = 0; i < preprocs.size(); i++)
		{
			preprocs[i]->print();
		}
	}
};

#endif /* ABSTRACTAPPLICATION_HPP_ */
