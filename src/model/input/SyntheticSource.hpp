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
//#include "ActiveFlow.hpp"
#include "commons/math/Rnd.hpp"
#include <boost/lexical_cast.hpp>
#include "../IMatrix.hpp"

using namespace std;

class SyntheticSource:public AbstractSource
{
	IMatrix& markovMatrix_;
	IMatrix& delayMatrix_;
	int port_;
	int count_;

public:
	SyntheticSource(IMatrix& markovMatrix, IMatrix& delay, int port, int count):markovMatrix_(markovMatrix),delayMatrix_(delay)
	{
		port_ = port;
		count_ = count;
	}

	int getPort()
	{
		return port_;
	}

	IMatrix& getDelayMatrix()
	{
		return delayMatrix_;
	}

	int getCount()
	{
		return count_;
	}

	void print()
	{
		cout << "SyntheticSource info:" << endl;
		cout << "Port: " << port_ << " Clients: " << count_ << endl;
		cout << "Delay matrix:" << endl;
		delayMatrix_.print();
		cout <<"Probability matrix:" << endl;
		markovMatrix_.print();
	}

	void replay()
	{
		/**
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
			string targetIp = ClientManagerInstance::getInstance().getTargetIp();

			int targetPort = port_;


			MarkovMatrix* group = new MarkovMatrix(markovMatrix_);
			DelayMatrix* delaymat = new DelayMatrix(delayMatrix_);

			bool result = group->validate();

			if(result == 0)
				continue;
			ActiveFlow* aflow = new ActiveFlow(sourceIp, targetIp, sourcePort, targetPort, group, delaymat);

			cout << "Scheduling a flow" << endl;
			aflow->execute(1000);
		}
		*/
	}

	void extract()
	{

	}
};


#endif /* SYNTHETICSOURCE_HPP_ */
