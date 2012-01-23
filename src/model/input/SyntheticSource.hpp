/*
 * SyntheticSource.hpp
 *
 *  Created on: Jan 23, 2012
 *      Author: issle
 */

#ifndef SYNTHETICSOURCE_HPP_
#define SYNTHETICSOURCE_HPP_

#include <iostream>
#include <vector>

using namespace std;

class SyntheticSource
{
	FlowGroup flowGroup_;
	int port_;
	int delay_;
	int count_;

public:
	SyntheticSource(FlowGroup flowGroup, int delay, int port, int count):flowGroup_(flowGroup)
	{
		port_ = port;
		delay_ = delay;
		count_ = count;
	}

	int getPort()
	{
		return port_;
	}

	int getDelay()
	{
		return delay_;
	}

	int getCount()
	{
		return count_;
	}

	void print()
	{
		cout << "SyntheticSource info:" << endl;
		cout << "Port: " << port_ << " Delay: " << delay_ << " Clients: " << count_ << endl;
		flowGroup_.print();
	}
};


#endif /* SYNTHETICSOURCE_HPP_ */
