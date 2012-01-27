#ifndef PACKET_H_
#define PACKET_H_

/**
 * This class represents a virtual packet extracted
 * from a pcap packet format. This implementation uses
 * the minimum possible data required for network replay.
 */
#include <netinet/ip.h>
#include <string>
#include <iostream>
#include <sys/time.h>
#include "commons/Tools.hpp"
#include "commons/MemoryPool.hpp"

using namespace std;

class Packet:public MemoryPool<Packet>
{
public:
	typedef int IdType;
private:

	string* sourceIp_;
	string* destinationIp_;
	string* payload_;
	string* fileName_;

	bool unknown;
	int sourcePort;
	int destinationPort;
	int packetId;

	static int idCounter;

	/**
	 * Represents the delay in which this packet arrived
	 * with zero reference the start of the pcap file reading.
	 */
	long timestamp;

public:
	Packet(const timeval& ts)
	{
		sourceIp_ = NULL;
		destinationIp_ = NULL;
		payload_ = NULL;
		fileName_ = NULL;
		timestamp = Tools::getTimeInMillis(&ts);
		setPacketId(getNextId());
		unknown = false;
	}

	Packet()
	{
		sourceIp_ = NULL;
		destinationIp_ = NULL;
		payload_ = NULL;
		fileName_ = NULL;
		timestamp = 0;
		setPacketId(getNextId());
		unknown = false;
	}

	Packet(long ts)
	{
		sourceIp_ = NULL;
		destinationIp_ = NULL;
		payload_ = NULL;
		fileName_ = NULL;
		timestamp = ts;
		setPacketId(getNextId());
		unknown = false;
	}

	~Packet()
	{
		delete sourceIp_;
		delete destinationIp_;
		delete payload_;
	}

	static int getNextId()
	{
		idCounter++;
		return idCounter;
	}

    const string* getDestinationIp() const
    {
        return destinationIp_;
    }

    bool isUnknown()
    {
    	return unknown;
    }

    void setIsUnknown()
    {
    	unknown = true;
    }

    int getDestinationPort() const
    {
        return destinationPort;
    }

    const string* getPayload() const
    {
        return payload_;
    }

    const string* getSourceIp() const
    {
        return sourceIp_;
    }

    int getSourcePort() const
    {
        return sourcePort;
    }

    long getTimestamp() const
    {
        return timestamp;
    }

    void setDestinationIp(string *destinationIp)
    {
        this->destinationIp_ = destinationIp;
    }

    void setDestinationPort(int destinationPort)
    {
        this->destinationPort = destinationPort;
    }

    void setPayload(string* payload)
    {
    	delete payload_;
        this->payload_ = payload;
    }

    void setSourceIp(string* sourceIp)
    {
    	delete sourceIp_;
        this->sourceIp_ = sourceIp;
    }

    void setSourcePort(int sourcePort)
    {
        this->sourcePort = sourcePort;
    }

    int getPacketId() const
    {
        return packetId;
    }

    void setPacketId(int packetId)
    {
        this->packetId = packetId;
    }

    const string* getFileName() const;
    void setFileName(string *fileName);

};


int Packet::idCounter=0;

const string* Packet::getFileName() const
{
    return fileName_;
}

void Packet::setFileName(string* fileName)
{
	delete fileName_;
    this->fileName_ = fileName;
}



#endif
