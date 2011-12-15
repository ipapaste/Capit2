/*
 * Client.hpp
 *
 *  Created on: Nov 25, 2011
 *      Author: issle
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include "AbstractApplication.hpp"
#include "Node.hpp"
#include "commons/Lock.hpp"
#include "commons/Container.hpp"
#include "ApplicationFactory.hpp"

/**
 * A client is accessed by a ClientManager thus
 * a Locking Logger is not needed.
 */
typedef Logger<1, NoLock> ClientLogger;

/**
 * A client contains a map of <ports/applications>.
 */
typedef Container<int, AbstractApplication*, NoLock> ClientContainer;

/**
 * A Client is an Entity that receives packets
 * from the ClientManager, is responsible for
 * the creation of the corresponding applications
 * and the forwarding of the packets in them.
 *
 * The client can be decomposed to simpler
 * Policies as follows:
 *
 * 1)A Client is a Container of Applications.
 *
 * 2)A Client is a Logger.
 *
 * 3)A Client is a Node in our program right
 * after the ClientManager and before the Applications.
 */
typedef Entity3<ClientContainer, ClientLogger, AbstractNode> AbstractClient;

class Client: public AbstractClient
{
public:
	/**
	 * Before this method forwards packets
	 * to the appropriate application it will
	 * filter out those packets that do not
	 * correspond to implemented applications
	 * that this program is made to simulate.
	 */
	void accept(Packet& p)
	{
		if(p.getPayload()->size()<1)
		{
			delete &p;
			return;
		}

		int serverPort = p.getDestinationPort();

		AbstractApplication* app;

		if (!hasItem(&serverPort))
		{
			ApplicationAccessor* accessor = ApplicationFactoryInstance::getInstance()->getItem(&serverPort);

			if(accessor == NULL)
			{
				delete &p;
				return;
			}

			app = accessor->getNewObject();

			if(app == NULL)
			{
				delete &p;
				return;
			}

			addItem(&serverPort, app);

			log("Generating a new application", 4);
		}
		else
		{
			app = getItem(&serverPort);
		}
		app->accept(p);
	}

};

#endif /* CLIENT_HPP_ */

