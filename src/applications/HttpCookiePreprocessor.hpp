/*
 * HttpCookiePreprocessor.hpp
 *
 *  Created on: Mar 13, 2012
 *      Author: issle
 */

#ifndef HTTPCOOKIEPREPROCESSOR_HPP_
#define HTTPCOOKIEPREPROCESSOR_HPP_

#include "AbstractPreprocessor.hpp"
#include <iostream>
#include <map>
#include "commons/tools/LexicalCast.hpp"
using namespace std;

class HttpCookiePreprocessor: public AbstractPreprocessor
{
private:
	map<int,int>responces;
	string cookie;

public:

	HttpCookiePreprocessor():cookie("empty")
	{

	}

	void preprocess(Packet& packet, string lastResponce)
	{
		cout << "Responce is :" << endl;

		if(lastResponce.length() < 3)
			return;

		cout << "It is: " << lastResponce << endl;

		string regex("Set-Cookie");

		if(String::regexMatch(lastResponce,regex))
		{
			int index = String::findPosition(lastResponce,regex);
			index += 12;

			string rawCookie = lastResponce.substr(index, lastResponce.size()- index +1);

			int endIndex = String::findPosition(lastResponce,";");

			string cleanCookie = rawCookie.substr(0,endIndex - index);


			cookie = cleanCookie;
		}

		if(String::areEqual(cookie,"empty"))
			return;
		else
		{
			string& payload = *packet.getPayloadN();

			string cookieRegex("Cookie:");

			if(String::regexMatch(payload,cookieRegex))
			{
				int index = String::findPosition(payload,cookieRegex);
				String::replaceAt(index+8, cookie.size(),payload,cookie);
			}
		}
	}

	void print()
	{

	}
};



#endif /* HTTPCOOKIEPREPROCESSOR_HPP_ */
