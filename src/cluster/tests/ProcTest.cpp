/*
 * ProcTest.cpp
 *
 *  Created on: Dec 27, 2011
 *      Author: issle
 */
#include <iostream>
#include "../../PacketSourcePolicies/PcapAdapter.hpp"
#include "../Preproccessor.hpp"
#include "../../Packet.hpp"
#include "../../commons/IdFactory.hpp"
#include "boost/lexical_cast.hpp"

using namespace std;

typedef OutputObject<Packet> OutputObjectT;

class Flow: public OutputObjectT, public IdFactory<Flow, int>
{
private:
	string flowId;
public:

	Flow(string s)
	{
		flowId = s;
	}

	static string getFlowId(const string& srcIp, const string& dstIp, int srcPort, int dstPort)
	{
		string s;

		string srcPortS = boost::lexical_cast<string>(srcPort);
		string dstPortS = boost::lexical_cast<string>(dstPort);

		s.append(srcIp).append("_").append(dstIp).append("_").append(srcPortS).append(
				"_").append(dstPortS);

		return s;
	}

	string getFlowId()
	{
		return flowId;
	}
};

typedef Preproccessor<string,Flow> Preproc;

class FTPFlowPreproc: public Preproc
{
public:
	void readInputObject(Preproc::InputObject* obj)
	{

		string s = Flow::getFlowId(*obj->getSourceIp(), *obj->getDestinationIp(), obj->getSourcePort(), obj->getDestinationPort());

		Flow* f = NULL;

		if(getOutputObjects()->count(s) == 0 )
		{
			f = new Flow(s);
			(*getOutputObjects())[s] = f;
		}
		else
		{
			f = (*getOutputObjects())[s];

		}

		(*f->getInputObjects())[obj->getPacketId()] = obj;
	}
};

int main()
{
	PcapAdapterShell shell;
	FTPFlowPreproc proc;

	shell.openSource("../../../data/ftp.pcap");

	Packet* p = shell.getNextPacket();

	while (p != NULL)
	{
		proc.readInputObject(p);
		cout << *p->getPayload() << endl;
		p = shell.getNextPacket();
	}

}

