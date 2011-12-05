/*
 * Node.hpp
 *
 *  Created on: Nov 28, 2011
 *      Author: issle
 */

#ifndef NODE_HPP_
#define NODE_HPP_

#include "Packet.hpp"

/**
 * The Capit Application's goal is to read packets
 * from multiple packet source and replay them towards
 * a target machine IP. To do that, Capit generates a
 * virtual environment that tries to emulate a real
 * network environment.
 *
 * To do so, our software is designed based upon a
 * chain of responsibility. A packet travels through
 * the virtual environment from Node to Node, from
 * its generation towards its end. Each node
 * pre - processes the packet and delegates further
 * responsibility to another Node that follows it
 * hierarchically.
 *
 * In other words the application is structured as
 * a tree of Nodes where Packets travel from the
 * Head to the Root with each Node having a different
 * functionality.
 *
 */
template  < class Packet> class Node
{
public:
	/**
	 * All Nodes share the same packet structure.
	 */
	typedef Packet PacketType;

	/**
	 * Should be implemented by each Node type.
	 * Does not necessarily define polymorphism.
	 */
	virtual void accept(Packet& p) = 0;
};


/**
 * Specialize the template with a Packet implementation.
 * To enforce code integrity when changing the Packet we
 * should make sure that its "Interface" remainst he same.
 */
typedef Node<Packet> AbstractNode;

#endif /* NODE_HPP_ */
