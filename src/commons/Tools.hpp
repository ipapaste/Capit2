/*
 * Tools.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef TOOLS_HPP_
#define TOOLS_HPP_

#include <iostream>
#include <sys/time.h>
#include <string>

using namespace std;

namespace Tools
{
	long getTimeInMillis(const timeval* time)
	{
		//cout << (int)(time->tv_usec/1000) + time->tv_sec*1000 << endl;
		return (long)(time->tv_usec/1000) + time->tv_sec*1000;
	}

	long currentTimeInMillis()
	{
		struct timeval now;

		gettimeofday(&now, NULL);
		return getTimeInMillis(&now);
	}

	bool areEqual(char* seq1, char* seq2)
	{
		string s1 = string(seq1);
		string s2 = string(seq2);
		if(s1.compare(s2) == 0)
			return true;
		else
			return false;
	}
}




#endif /* TOOLS_HPP_ */
