/*
 * FTPApplication.hpp
 *
 *  Created on: Dec 1, 2011
 *      Author: issle
 */

#ifndef FTPAPPLICATION_HPP_
#define FTPAPPLICATION_HPP_

#include "../AbstractApplication.hpp"

class FTPApplication: public AbstractApplication
{
private:
	int appState;
public:
	static const int STATE_USERNAME_PENDING = 0;
	static const int STATE_PASSWORD_PENDING = 1;
	static const int STATE_READY = 3;

	void accept(AbstractApplicationType::PacketType& packet)
	{
		switch (appState)
		{
		case STATE_READY:
		{
			//TODO: Forward the packet to preprocessors (?).
			break;
		}
		case STATE_USERNAME_PENDING:
		{
			//TODO: Drop this packet and send a USER <param> packet.
			appState = STATE_PASSWORD_PENDING;
			break;
		}
		case STATE_PASSWORD_PENDING:
		{
			//TODO: Drop this packet and send a PASS <param> packet.
			appState = STATE_READY;
			break;
		}
		}

		AbstractApplication::accept(packet);
	}
};

#endif /* FTPAPPLICATION_HPP_ */
