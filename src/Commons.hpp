/*
 * Commons.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef COMMONS_HPP_
#define COMMONS_HPP_
#include "commons/Logger.hpp"
#include "commons/Lock.hpp"
#include "commons/Singleton.hpp"
#include "AbstractApplication.hpp"



//----------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------

template <class Container, class Node, class Logger, class Thread> class Entity:
public Container,
public Node,
public Logger,
public Thread
{
public:

};

/**
typedef Container<Packet,Packet,NoGuard> PacketContainer;
typedef Entity<PacketContainer,AbstractNode, Logger1,ThreadShell> AbstractApplication;

typedef Container<int,AbstractApplication, NoGuard> ApplicationContainer;
typedef Entity<ApplicationContainer,AbstractNode,Logger1,NullClass> Client;

typedef Container<string,Client,NoGuard> ClientContainer;
typedef Entity<ClientContainer,AbstractNode,Logger1,NullClass> ClientManager;

*/

#endif /* COMMONS_HPP_ */
