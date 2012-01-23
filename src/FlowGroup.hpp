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
#include "commons/Math.hpp"

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

	vector<float> getSubGroup(int i)
	{
		return transmitionMatrix[i];
	}
	void setProbability(int i , int j, float value)
	{
		int size = transmitionMatrix.size();
		if(i >= size || j >= size)
		{
			size = Math<int>::max(i+1, j+1);
			transmitionMatrix.resize(size);

			for(int i = 0; i < size; i++)
			{
				transmitionMatrix[i].resize(size);
			}
		}

		transmitionMatrix[i][j] = value;
	}

	float getProbability(int i, int j)
	{
		return transmitionMatrix[i][j];
	}

	void print()
	{
		for(int i = 0; i < transmitionMatrix.size(); i++)
		{
			for ( int j = 0; j < transmitionMatrix.size(); j++)
			{
				cout << transmitionMatrix[i][j] << " ";
			}

			cout << endl;
		}
	}
};


#endif /* FLOWGROUP_HPP_ */
