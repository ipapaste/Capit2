/*
 * SessionState.hpp
 *
 *  Created on: Jan 6, 2012
 *      Author: issle
 */

#ifndef SESSIONSTATE_HPP_
#define SESSIONSTATE_HPP_

/**
 * A SessionState object represents the authentication status
 * of an Application. Since most common protocols use a username
 * and password combination this class ensures the correct tran-
 * sition between states. Whenever an application is created it's
 * state is set to pending username, meaning that the application
 * needs a username to be provided to start accepting packets.
 *
 * When a username is provided the state changes to password pending
 * meaning that we are waiting for a password. After that is done
 * the application can start receiving and forwarding packets.
 */
class SessionState
{
private:
	int sessionState;

public:
	static const int STATE_USERNAME_PENDING = 0;
	static const int STATE_PASSWORD_PENDING = 1;
	static const int STATE_READY = 3;

	SessionState()
	{
		sessionState = STATE_USERNAME_PENDING;
	}

	int getSessionState()
	{
		return sessionState;
	}

	bool setReady()
	{
		if(sessionState == STATE_PASSWORD_PENDING)
		{
			sessionState = STATE_READY;
			return true;
		}

		return false;
	}

	bool setPasswordPending()
	{
		if(sessionState == STATE_USERNAME_PENDING)
		{
			sessionState = STATE_PASSWORD_PENDING;
			return true;
		}

		return false;
	}
};



#endif /* SESSIONSTATE_HPP_ */
