/*
 * FlowManager.hpp
 *
 *  Created on: Jan 18, 2012
 *      Author: issle
 */

#ifndef FLOWMANAGER_HPP_
#define FLOWMANAGER_HPP_

#include <iostream>
#include "Packet.hpp"
#include "Node.hpp"
#include "Flow.hpp"

class FlowManager: public AbstractNode
{
private:
	map<string,Flow*> flows;
public:
	void accept(Packet& obj)
	{
		if(obj.getPayload() == NULL)
			return;
		if(obj.getPayload()->size()<1)
			return;

		if(FlowsInstance::getInstance()->getType(obj.getDestinationPort()) ==NULL)
			return;

		string id = Flow::getFlowId(*obj.getSourceIp(), *obj.getDestinationIp(), obj.getSourcePort(), obj.getDestinationPort());

		Flow* f = NULL;

		if(flows.count(id) == 0 )
		{
			f = new Flow(id, obj.getSourcePort(), obj.getDestinationPort());
			flows[id] = f;
		}
		else
		{
			f = flows[id];

		}
		f->accept(obj);
	}

	void calc()
	{
		map<string,Flow*>::iterator it;

		for(it = flows.begin(); it != flows.end(); it++)
			it->second->calc();
	}
};


#endif /* FLOWMANAGER_HPP_ */
