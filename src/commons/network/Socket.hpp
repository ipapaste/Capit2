/*
 * network/Socket.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using boost::asio::ip::tcp;

class AbstractSocket
{
public:
	virtual void connectImpl(const string& ip, int port) = 0;

	virtual bool isConnected() = 0;

	void connect(const string& ip , int port)
	{
		if (!isConnected())
			connectImpl(ip,port);
	}

	virtual void sendData(u_char* data) = 0;

	virtual ~AbstractSocket()
	{

	}

};

class BoostSocket: public AbstractSocket
{
private:
	boost::asio::io_service* io_service;

	tcp::resolver* resolver;

	tcp::resolver::query* query;

	tcp::socket* socket;

	boost::asio::streambuf* request;

	std::ostream* request_stream;
public:

	void connectImpl(const string& ip, int port)
	{

		io_service = new boost::asio::io_service();

		resolver = new tcp::resolver(*io_service);

		std::string val = boost::lexical_cast<std::string>(port);
		query = new tcp::resolver::query(ip.c_str(), val.c_str());

		tcp::resolver::iterator endpoint_iterator = resolver->resolve(*query);

		socket = new tcp::socket(*io_service);
		boost::asio::connect(*socket, endpoint_iterator);
	}

	bool isConnected()
	{
		return false;
	}

	void sendData(u_char* data)
	{
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << data ;
		// Send the request.
		boost::asio::write(*socket, request);

	}

};

#endif /* SOCKET_HPP_ */
