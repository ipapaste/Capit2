/*
 * FTPApplication.hpp
 *
 *  Created on: Dec 1, 2011
 *      Author: issle
 */

#ifndef FTPAPPLICATION_HPP_
#define FTPAPPLICATION_HPP_

#include "../AbstractApplication.hpp"
#include "model/Command.hpp"
#include "model/Credential.hpp"
#include "model/SessionState.hpp"
#include "commons/tools/Rnd.hpp"

class FTPApplication: public AbstractApplication, public SessionState
{
private:

	static vector<Credential*> credentials;
	static queue<Command*> commands;

	Credential* credential;

public:

	FTPApplication()
	{
		credential = new Credential("issle1", "kaiap4981");
	}

	void accept(AbstractApplicationType::PacketType& packet)
	{
		switch (SessionState::getSessionState())
		{
		case STATE_READY:
		{
			AbstractApplication::accept(packet);
			break;
		}
		case STATE_USERNAME_PENDING:
		{
			if(!packet.getPayload()->find("USER"))
			{
				string* payload = new string("USER");
				payload->append(" ");
				payload->append(credential->getUsername());
				payload->append(" \n");
				packet.setPayload(payload);
			}

			AbstractApplication::accept(packet);
			SessionState::setPasswordPending();
			break;
		}
		case STATE_PASSWORD_PENDING:
		{
			if(!packet.getPayload()->find("PASS"))
			{
				string* payload = new string("USER");
				payload->append(" ");
				payload->append(credential->getUsername());
				payload->append(" \n");
				packet.setPayload(payload);
			}

			AbstractApplication::accept(packet);
			SessionState::setReady();
			break;
		}
		}

	}
};

#endif /* FTPAPPLICATION_HPP_ */
