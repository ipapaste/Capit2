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

template<class T,class OutputObject> class Preproccessor
{
public:
	typedef typename OutputObject::InputObjectType InputObject;
private:
	map<T, OutputObject*> objects;

public:

	virtual void readInputObject(InputObject* obj) = 0;

	map<T, OutputObject*>* getOutputObjects()
	{
		return &objects;
	}

	virtual ~Preproccessor()
	{

	}
};

template <class InputObject> class OutputObject
{
public:
	typedef InputObject InputObjectType;
private:
	map<int,InputObject*> inputObjects;
public:
	map<int,InputObject*>* getInputObjects()
	{
		return &inputObjects;
	}
};

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

	string getId()
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

		map<int,Packet*>& input = *f->getInputObjects();
		cout << "Packet id: " << obj->getPacketId() << endl;
		input[obj->getPacketId()] = obj;

		//cout <<this->getOutputObjects()->begin()->first << endl;
	}
};

#endif /* PREPROCCESSOR_HPP_ */
