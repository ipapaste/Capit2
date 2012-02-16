/*
 * ClientManager.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef CLIENTMANAGER_HPP_
#define CLIENTMANAGER_HPP_

#include "commons/container/Singleton.hpp"
#include "commons/container/Entity.hpp"
#include "commons/Lock.hpp"
#include "commons//container/Container.hpp"
#include "commons/Tools.hpp"
#include "Client.hpp"
#include "Node.hpp"

/**
 * A ClientManager can be accessed by multiple
 * PacketSources that act as independent threads
 * thus it needs a Locking Logger.
 */
typedef Logger<1, MutexLock> ClientManagerLogger;

/**
 * A ClientManager contains a map of <IP , Client >.
 */
typedef Container<const string, Client*, MutexLock> ClientManagerContainer;

/**
 * A ClientManager is an Entity that receives packets
 * from multiple sources, generates Clients that based
 * on those Packets and forwards the packets to the Clients.
 */
typedef Entity3<ClientManagerContainer, AbstractNode, ClientManagerLogger> AbstractClientManager;

/**
 * The ClientManager contains Clients that communicate
 * with a single Server called the Target. The ClientManager
 * needs to know the targets ip so as to filter out the
 * packets that come from that specific source.
 *
 * Whenever the manager receives a packet it checks
 * its container to find the appropriate Client to
 * forward the packet. If no client is found it generates
 * a Client and then forwards the packet.
 */
class ClientManager: public AbstractClientManager
{
private:

	/*
	 * The target server Ip.
	 */
	string targetIp;

	/*
	 * Applications in running mode;
	 */

	int runningApps_;

	/*
	 * Counts the sources registered to this
	 * client manager singleton that are authorized
	 * to send packets. Used in order to wake
	 * applications when the sources are done.
	 */
	int sourceCount;

public:

	ClientManager()
	{
		sourceCount = 0;
		runningApps_=0;
		targetIp = string("127.0.0.1");
	}

	void registerSource()
	{
		sourceCount++;
	}

	void registerApp()
	{
		runningApps_++;
	}

	void unregisterApp()
	{
		runningApps_--;
	}

	void removeSource()
	{
		sourceCount--;

		if (sourceCount == 0)
		{
			print();
			if(runningApps_ <=0)
				exit(0);
		}
	}

	/**
	 * Before forwarding the packets over to the Clients
	 * this method filters out any packet that does not
	 * have as destination ip, the target ip set to this
	 * manager.
	 *
	 * A pcap log, contains packets that are identical
	 * when captured on the source ( client ) and on
	 * the destination. A packet with source S and destination
	 * D will have the same source and same destination
	 * when it reach the server.
	 */
	void accept(AbstractClientManager::PacketType& packet)
	{
		const string* dstIp = packet.getDestinationIp();
		const string* srcIp = packet.getSourceIp();

		int dstPort = packet.getDestinationPort();

		//Filter out.
		if (dstIp->compare(targetIp) != 0)
		{

			delete &packet;
			return;
		}

		Client* client;

		if (!hasItem(*srcIp))
		{
			client = new Client(*srcIp);
			log("Generating a new client.", 4);
			addItem(*srcIp, client);
		}
		else
		{
			client = getItem(*srcIp);
		}

		client->accept(packet);

	}

	void setTargetIp(string ip)
	{
		targetIp = ip;
	}

	string getTargetIp()
	{
		return targetIp;
	}

	void print()
	{
		cout << "1" << endl;
		cout << "-------------------- Client Manager statistics -----------------" << endl;
		map<string,Client*>::iterator it;
		cout << "2" << endl;
		for(it = getMap()->begin(); it != getMap()->end(); it++)
		{
			cout << "3" << endl;
			it->second->print();
		}
	}
}
;

/**
 * We have only one ClientManager in the application so we
 * define it as a Singleton for universal access.
 */
typedef Singleton<ClientManager> ClientManagerInstance;

#endif /* CLIENTMANAGER_HPP_ */
