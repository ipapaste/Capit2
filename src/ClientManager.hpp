/*
 * ClientManager.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef CLIENTMANAGER_HPP_
#define CLIENTMANAGER_HPP_

#include "commons/Singleton.hpp"
#include "commons/Lock.hpp"
#include "commons/Container.hpp"
#include "commons/Tools.hpp"
#include "Client.hpp"
#include "Node.hpp"


/**
 * A ClientManager can be accessed by multiple
 * PacketSources that act as independent threads
 * thus it needs a Locking Logger.
 */
typedef Logger<1,MutexLock> ClientManagerLogger;


/**
 * A ClientManager contains a map of <IP , Client >.
 */
typedef Container<string,Client*,MutexLock> ClientManagerContainer;


/**
 * A ClientManager is an Entity that receives packets
 * from multiple sources, generates Clients that based
 * on those Packets and forwards the packets to the Clients.
 */
typedef Entity3<ClientManagerContainer, AbstractNode,ClientManagerLogger> AbstractClientManager;


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
	string* targetIp;

public:



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
		string* dstIp = packet.getDestinationIp();
		string* srcIp = packet.getSourceIp();

		int dstPort = packet.getDestinationPort();


		//Filter out.
		if(dstIp->compare(*targetIp) !=0)
		{
			delete &packet;
			return;
		}

		Client* client;

		if(!hasItem(srcIp))
		{
			client = new Client();
			log("Generating a new client.",4);
			addItem(srcIp,client);
		}
		else
		{
			client = getItem(srcIp);
		}

		client->accept(packet);

	}

	void setTargetIp(string* ip)
	{
		targetIp = ip;
	}

	string& getTargetIp()
	{
		return *targetIp;
	}
};

/**
 * We have only one ClientManager in the application so we
 * define it as a Singleton for universal access.
 */
typedef Singleton<ClientManager> ClientManagerInstance;

#endif /* CLIENTMANAGER_HPP_ */
