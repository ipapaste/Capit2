/*
 * Credential.hpp
 *
 *  Created on: Jan 6, 2012
 *      Author: issle
 */

#ifndef CREDENTIAL_HPP_
#define CREDENTIAL_HPP_

#include <iostream>

using namespace std;

class Credential
{
private:
	string username_;
	string password_;
public:
	Credential(string username, string password)
	{
		username_ = username;
		password_ = password;
	}

	string getUsername() const
	{
		return username_;
	}

	string getPassword() const
	{
		return password_;
	}
};


#endif /* CREDENTIAL_HPP_ */
