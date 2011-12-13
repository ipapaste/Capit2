/*
 * FlowManager.hpp
 *
 *  Created on: Dec 13, 2011
 *      Author: issle
 */

#ifndef FLOWMANAGER_HPP_
#define FLOWMANAGER_HPP_

#include "commons/Singleton.hpp"
#include "commons/Lock.hpp"
#include "commons/Container.hpp"
#include "commons/Tools.hpp"
#include "Node.hpp"
#include "Flow.hpp"
#include <boost/lexical_cast.hpp>

typedef Logger<1,NoLock> FlowManagerLogger;


typedef Container<string,Flow*,NoLock> FlowContainer;


typedef Entity3<FlowContainer,AbstractNode,ClientManagerLogger> AbstractFlowManager;


class FlowManager: public AbstractFlowManager
{
private:

public:


	void accept(AbstractClientManager::PacketType& packet)
	{

	}

	string generateFlowIdentifier(string srcIp, string dstIp, int srcPort, int dstPort, string fileName)
	{
		string s;

		string srcPortS = boost::lexical_cast<string>(srcPort);
		string dstPortS = boost::lexical_cast<string>(dstPort);

		s.append(srcIp).append("_").append(dstIp).append("_").append(srcPortS).append("_").append(dstPortS).append("_").append(fileName);

		return s;
	}
};


typedef Singleton<FlowManager> FlowManagerInstance;



#endif /* FLOWMANAGER_HPP_ */
