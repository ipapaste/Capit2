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

using namespace std;

class Packet
{
private:

	string* sourceIp;
	string* destinationIp;
	string* payload;
	string* fileName;

	int sourcePort;
	int destinationPort;
	int packetId;

	static int idCounter;

	/**
	 * Represents the delay in which this packet arrived
	 * with zero reference the start of the pcap file reading.
	 */
	const timeval* timestamp;
public:
	Packet(const timeval& ts)
	{
		sourceIp = NULL;
		destinationIp = NULL;
		payload = NULL;
		timestamp = &ts;
	}

	~Packet()
	{
		delete sourceIp;
		delete destinationIp;
		delete payload;
	}

	static int getNextId()
	{
		return idCounter++;
	}

    string *getDestinationIp() const
    {
        return destinationIp;
    }

    int getDestinationPort() const
    {
        return destinationPort;
    }

    string *getPayload() const
    {
        return payload;
    }

    string *getSourceIp() const
    {
        return sourceIp;
    }

    int getSourcePort() const
    {
        return sourcePort;
    }

    const timeval *getTimestamp() const
    {
        return timestamp;
    }

    void setDestinationIp(string *destinationIp)
    {
        this->destinationIp = destinationIp;
    }

    void setDestinationPort(int destinationPort)
    {
        this->destinationPort = destinationPort;
    }

    void setPayload(string *payload)
    {
        this->payload = payload;
    }

    void setSourceIp(string *sourceIp)
    {
        this->sourceIp = sourceIp;
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

    string *getFileName() const;
    void setFileName(string *fileName);

};


int Packet::idCounter=0;

string *Packet::getFileName() const
{
    return fileName;
}

void Packet::setFileName(string *fileName)
{
    this->fileName = fileName;
}



#endif
