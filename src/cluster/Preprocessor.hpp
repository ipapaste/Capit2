/*
 * Preproccessor.hpp
 *
 *  Created on: Dec 27, 2011
 *      Author: issle
 */

#ifndef PREPROCCESSOR_HPP_
#define PREPROCCESSOR_HPP_

#include <map>
#include "../Packet.hpp"
#include "../commons/IdFactory.hpp"
#include "boost/lexical_cast.hpp"

using namespace std;

template<class OutputObjectT> class Preprocessor
{
public:
	typedef OutputObjectT OutputObject;
	typedef typename OutputObjectT::InputObject InputObject;

private:
	map<typename OutputObject::IdType, OutputObject*> objects;

public:

	virtual void readInputObject(InputObject* obj) = 0;

	map<typename OutputObject::IdType, OutputObject*>* getOutputObjects()
	{
		return &objects;
	}

	virtual ~Preprocessor()
	{

	}
};


/**
 * An OutputObject is a complex ( usually ) generation that gets
 * produced from one or more InputObjects. Each OutputObject has
 * an Id Type that distinguishes it from the other OutputObjects
 * so as they can be used in clustering.
 */



template <class InputObjectT, class IdTypeT> class OutputObject
{
	class ClusterType;
public:
	typedef IdTypeT IdType;

	typedef InputObjectT InputObject;
private:
	map<int,InputObject*> inputObjects;
	ClusterType* cluster_;
public:
	map<int,InputObject*>* getInputObjects()
	{
		return &inputObjects;
	}

	ClusterType* getCluster()
	{
		return cluster_;
	}

	void setCluster(ClusterType* c)
	{
		cluster_ = c;
	}
};

typedef OutputObject<Packet, string> FlowOutput;


/**
 * A Flow is an OutputObject that consists of all the packets
 * that have the same srcip/dstip/srcport/dstport combination.
 */
class Flow: public FlowOutput
{
private:
	IdType flowId;
public:

	Flow(IdType s)
	{
		flowId = s;
	}

	static IdType getFlowId(const string& srcIp, const string& dstIp, int srcPort, int dstPort)
	{
		string s;

		string srcPortS = boost::lexical_cast<string>(srcPort);
		string dstPortS = boost::lexical_cast<string>(dstPort);

		s.append(srcIp).append("_").append(dstIp).append("_").append(srcPortS).append(
				"_").append(dstPortS);

		return s;
	}

	IdType getFlowId()
	{
		return flowId;
	}

	IdType getId()
	{
		return flowId;
	}
};

typedef Preprocessor<Flow> FlowPreproc_;

class FlowPreproc: public FlowPreproc_
{
public:
	void readInputObject(InputObject* obj)
	{

		OutputObject::IdType s = Flow::getFlowId(*obj->getSourceIp(), *obj->getDestinationIp(), obj->getSourcePort(), obj->getDestinationPort());

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

		map<OutputObject::InputObject::IdType,OutputObject::InputObject*>& input = *f->getInputObjects();
		input[obj->getPacketId()] = obj;

		//cout <<this->getOutputObjects()->begin()->first << endl;
	}
};

#endif /* PREPROCCESSOR_HPP_ */
