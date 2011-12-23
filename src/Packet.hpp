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


	Packet *next;

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

	static const int BLOCK_SIZE = 512;
	static Packet* headOfFreeList;

	static void* operator new(size_t size)
	{
		if(size != sizeof(Packet))
			return ::operator new(size);

		Packet* p = headOfFreeList;

		if(p)
		{
			headOfFreeList = p->next;
		}
		else
		{
			Packet* newBlock = static_cast<Packet*>(::operator new(BLOCK_SIZE * sizeof(Packet)));

			for(int i = 0; i < BLOCK_SIZE-1; ++i)
			{
				newBlock[i].next = &newBlock[i+1];
			}

			newBlock[BLOCK_SIZE].next = 0;

			p = newBlock;

			//This will leak memory if the else{} was entered because
			//the queue got full ( headoffreelist went to the tail ).
			//A new list will be allocated while the old one will still
			//exist since the packets need it. Once the old packets wont
			//need the memory, they wont release on the old one butmay even
			//try to access memory outside the scope of the freelist.
			headOfFreeList = &newBlock[1];
		}

		return p;
	}

	static void operator delete(void *deadObject, size_t size)
	{
		if(deadObject == 0)
			return;

		if(size != sizeof(Packet))
		{
			::operator delete(deadObject);
			return;
		}

		Packet* carcass = static_cast<Packet*>(deadObject);

		carcass->next = headOfFreeList;

		headOfFreeList = carcass;
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
Packet* Packet::headOfFreeList = 0;

string *Packet::getFileName() const
{
    return fileName;
}

void Packet::setFileName(string *fileName)
{
    this->fileName = fileName;
}



#endif
