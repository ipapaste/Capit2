/*
 * FlowGroup.hpp
 *
 *  Created on: Jan 18, 2012
 *      Author: issle
 */

#ifndef FLOWGROUP_HPP_
#define FLOWGROUP_HPP_

#include <iostream>
#include <vector>

using namespace std;

class FlowGroup
{
private:
	vector<vector<float> > transmitionMatrix;
public:
	FlowGroup(int size)
	{
		transmitionMatrix.resize(size);

		for(int i = 0; i < size; i++)
		{
			transmitionMatrix[i].resize(size);
		}

		for(int i = 0; i < size; i++)
		{
			for(int j = 0;j < size; j++)
			{
				transmitionMatrix[i][j] = 0;
			}
		}
	}

	void setProbability(int i , int j, float value)
	{
		transmitionMatrix[i][j] = value;
	}

	float getProbability(int i, int j)
	{
		return transmitionMatrix[i][j];
	}
};


#endif /* FLOWGROUP_HPP_ */
