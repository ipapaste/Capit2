/*
 * Date.hpp
 *
 *  Created on: Feb 16, 2012
 *      Author: issle
 */

#ifndef DATE_HPP_
#define DATE_HPP_

#include <iostream>
#include <sys/time.h>
#include <string>

using namespace std;
namespace commons
{
namespace util
{
namespace Date
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
}
}
}




#endif /* DATE_HPP_ */
