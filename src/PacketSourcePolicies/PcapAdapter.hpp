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

class PcapAdapterShell
{
private:
	/**
	 * Descriptor pointing at the .cap file that this
	 * parser is opening for offline capturing.
	 */
	pcap_t *descr;

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
public:

	void openSource(const char* source)
	{
		descr = pcap_open_offline(source, errbuf);
		if (descr == NULL)
		{
			cout << "pcap_open_live() failed: " << errbuf << endl;
		}
	}

	Packet* getNextPacket()
	{
		const u_char* packet = pcap_next(descr, &header);

		if(packet == 0)
			return NULL;

		return createPacket(&header, packet);

	}

	Packet* createPacket(const struct pcap_pkthdr* pkthdr, const u_char* packet)
	{
		//Packet* packet_ = new Packet();
		const struct ether_header* ethernetHeader;
		const struct ip* ipHeader;
		const struct tcphdr* tcpHeader;
		const struct udphdr* udpHeader;

		char sourceIp[INET_ADDRSTRLEN];
		char destIp[INET_ADDRSTRLEN];

		u_int sourcePort;
		u_int destPort;
		u_char* data;

		int dataLength = 0;
		string dataStr = "";

		ethernetHeader = (struct ether_header*)packet;
		if (ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP)
		{
			ipHeader = (struct ip*)(packet + sizeof(struct ether_header));
			inet_ntop(AF_INET, &(ipHeader->ip_src), sourceIp, INET_ADDRSTRLEN);
			inet_ntop(AF_INET, &(ipHeader->ip_dst), destIp, INET_ADDRSTRLEN);

			if (ipHeader->ip_p == IPPROTO_TCP)
			{
				tcpHeader = (tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
				udpHeader = (udphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
				sourcePort = ntohs(tcpHeader->source);
				destPort = ntohs(tcpHeader->dest);
				data = (u_char*)(packet + sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));
				dataLength = pkthdr->len - (sizeof(struct ether_header) + sizeof(struct ip) + sizeof(struct tcphdr));

				//for(int i =0; i < dataLength; i++)
				//	cout << (char)data[i];

				//cout << endl;

				// convert non-printable characters, other than carriage return, line feed,
				// or tab into periods when displayed.
				for (int i = 0; i < dataLength; i++)
				{
					if (((data[i] >= 32 && data[i] <= 126) || data[i] == 10 || data[i] == 11 || data[i] == 13) && i > 11) {
						dataStr += (char)data[i];
					} //else {
						//dataStr += ".";
					//}

				}

			}
		}


		string* sourceIp_ = new string(sourceIp);
		string* destinationIp_ = new string(destIp);
		string* payload = new std::string(reinterpret_cast<const char*>((u_char*)dataStr.c_str()));
		//cout << *payload << endl;
		Packet* packet1= new Packet(pkthdr->ts);
		packet1->setSourceIp(sourceIp_);
		packet1->setDestinationIp(destinationIp_);
		packet1->setSourcePort(sourcePort);
		packet1->setDestinationPort(destPort);
		packet1->setPayload(payload);
		packet1->setPacketId(Packet::getNextId());
		//packet1->setTimestamp(pkthdr->ts);

		return  packet1;
	}

};

typedef PcapAdapterShell PcapAdapter;

#endif /* PCAPADAPTER_HPP_ */
