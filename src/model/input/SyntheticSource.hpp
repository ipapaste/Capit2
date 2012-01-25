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
#include "AbstractSource.hpp"
#include "ActiveFlow.hpp"
#include "commons/math/Rnd.hpp"
#include <boost/lexical_cast.hpp>

using namespace std;

class SyntheticSource:public AbstractSource
{
	MarkovMatrix MarkovMatrix_;
	int port_;
	int delay_;
	int count_;
	int std_;

public:
	SyntheticSource(MarkovMatrix MarkovMatrix, int delay, int std, int port, int count):MarkovMatrix_(MarkovMatrix)
	{
		port_ = port;
		delay_ = delay;
		count_ = count;
		std_ = std;
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
		MarkovMatrix_.print();
	}

	void replay()
	{
		for(int i = 0; i < count_; i++)
		{
			int sourcePort = Rnd::getInt(1024 , 12000);

			string sourceIp("");
			for(int i =0; i< 4; i++)
			{
				int chunk = Rnd::getInt(10,255);
				sourceIp.append(boost::lexical_cast<string>(chunk));
				if( i != 3)
				sourceIp.append(".");
			}

			cout << sourceIp << endl;
			string targetIp = ClientManagerInstance::getInstance()->getTargetIp();

			int targetPort = port_;


			MarkovMatrix* group = new MarkovMatrix(MarkovMatrix_);

			if(!group->validate())
				return;
			ActiveFlow* aflow = new ActiveFlow(sourceIp, targetIp, sourcePort, targetPort, group, delay_, std_);

			ThreadShell::schedule(*aflow,1000);
		}
	}

	void extract()
	{

	}
};


#endif /* SYNTHETICSOURCE_HPP_ */
