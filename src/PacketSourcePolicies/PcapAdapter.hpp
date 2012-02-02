/*
 * PcapAdapter.hpp
 *
 *  Created on: Nov 25, 2011
 *      Author: issle
 */

#ifndef PCAPADAPTER_HPP_
#define PCAPADAPTER_HPP_

#include <iostream>
#include <pcap.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include "../Packet.hpp"
#include <string>

using namespace std;

class PcapAdapterShell
{
private:
	/**
	 * Descriptor pointing at the .cap file that this
	 * parser is opening for offline capturing.
	 */
	pcap_t* descr;

	bpf_program filter;

	/**
	 * Each pcap file has a header that contains file
	 * specific information and a list of packets in
	 * sequence following this header.
	 */
	struct pcap_pkthdr header;

	/**
	 * Buffer holding the error output.
	 */
	char errbuf[PCAP_ERRBUF_SIZE];

	string* fileName;
public:

	void openSource(const char* source)
	{
		fileName = new string(source);
		descr = pcap_open_offline(source, errbuf);
		if (descr == NULL)
		{
			cout << "pcap_open_live() failed: " << errbuf << endl;
		}
	}

	void setFilter(string& s)
	{
		if(pcap_compile(descr, &filter, s.c_str(), 0, 0) == -1)
		{
			cout << "Incorrect filter expression." << endl;
			exit(0);
		}
		pcap_setfilter(descr, &filter);
	}

	Packet* getNextPacket()
	{
		const u_char* packet = pcap_next(descr, &header);

		if (packet == 0)
			return NULL;

		return createPacket(&header, packet);

	}

	Packet* createPacket(const struct pcap_pkthdr* pkthdr, const u_char* packet)
	{

		/**
		 * Ethernet layer.
		 */
		const struct ether_header* ethernetHeader;

		/**
		 * IP layer.
		 */
		const struct ip* ipHeader;

		/**
		 * TCP/UDP layer.
		 */
		const struct tcphdr* tcpHeader;
		const struct udphdr* udpHeader;

		/**
		 * Source and destination address.
		 */
		char sourceIp[INET_ADDRSTRLEN];
		char destIp[INET_ADDRSTRLEN];

		/**
		 * Source and destination ports.
		 */
		u_int sourcePort;
		u_int destPort;

		/**
		 * Raw payload.
		 */
		u_char* data;

		/**
		 * Payload length.
		 */
		int dataLength = 0;


		/**
		 * Printable payload.
		 */
		string dataStr = "";

		/**
		 * Read Ethernet layer.
		 */
		ethernetHeader = (struct ether_header*)(packet);
        if (ntohs(ethernetHeader->ether_type) != ETHERTYPE_IP)
        {
        	Packet* p = new Packet(pkthdr->ts);
        	p->setIsUnknown();
			return p;
        }
        /**
		 * Read IP layer.
		 */
        ipHeader = (struct ip*)((packet + sizeof (struct ether_header)));
        /**
		 * Read IP addresses.
		 */
        inet_ntop(AF_INET, &(ipHeader->ip_src), sourceIp, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(ipHeader->ip_dst), destIp, INET_ADDRSTRLEN);
        /**
		 * The layer under IP is TCP.
		 */
        if (ipHeader->ip_p == IPPROTO_TCP)
		{

			tcpHeader = (tcphdr*) (packet + sizeof(struct ether_header) + sizeof(struct ip));

			/**
			 * Read ports.
			 */
			sourcePort = ntohs(tcpHeader->source);
			destPort = ntohs(tcpHeader->dest);

			/**
			 * The total header size is the sum of the sub-headers aka each layter.
			 * The tcp layer size should get calculated from the data offset byte
			 * couple cause it is of a variable length thus size of wont work with
			 * it.
			 */
			int size_h = sizeof(struct ether_header) + sizeof(struct ip) + tcpHeader->doff*4;

			data = (u_char*) (packet + size_h);

			dataLength = pkthdr->len - size_h;

		}
		else if (ipHeader->ip_p == IPPROTO_UDP)
		{
			udpHeader = (udphdr*) (packet + sizeof(struct ether_header)
					+ sizeof(struct ip));
			sourcePort = ntohs(udpHeader->source);
			destPort = ntohs(udpHeader->dest);
			data = (u_char*) (packet + sizeof(struct ether_header)
					+ sizeof(struct ip) + sizeof(struct udphdr));
			dataLength = pkthdr->len - (sizeof(struct ether_header)
					+ sizeof(struct ip) + sizeof(struct udphdr));
		}
        for(int i = 0;i < dataLength;i++){
            if(((data[i] >= 32 && data[i] <= 126) || data[i] == 10 || data[i] == 11 || data[i] == 13)){
                dataStr += (char)(data[i]);
            }
        }

        string *sourceIp_ = new string(sourceIp);
        string *destinationIp_ = new string(destIp);
        string *payload = new std::string(reinterpret_cast<const char*>((u_char*)(dataStr.c_str())));

        Packet *packet1 = new Packet(pkthdr->ts);
        packet1->setSourceIp(sourceIp_);
        packet1->setDestinationIp(destinationIp_);
        packet1->setSourcePort(sourcePort);
        packet1->setDestinationPort(destPort);
        packet1->setPayload(payload);
        packet1->setFileName(fileName);
        return packet1;
    }

    string *getFileName() const
    {
        return fileName;
    }

};

typedef PcapAdapterShell PcapAdapter;

#endif /* PCAPADAPTER_HPP_ */
