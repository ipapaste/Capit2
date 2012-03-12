/*
 * HttpPreprocessor.hpp
 *
 *  Created on: Feb 26, 2012
 *      Author: issle
 */

#ifndef HTTPPREPROCESSOR_HPP_
#define HTTPPREPROCESSOR_HPP_
#include <iostream>
using namespace std;

class HttpPreprocessor: public AbstractPreprocessor
{
private:
	string cookieId_;
	bool gotCookie;
public:
	void HttpPreprocessor()
	{
		gotCookie = false;
	}

	void preprocess(Packet& packet, string lastResponce)
	{
		//If lastResponse sets cookie
		//set and true


		//If set replace every other.
	}
};


#endif /* HTTPPREPROCESSOR_HPP_ */
