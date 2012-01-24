/*
 * MarkovMatrix.hpp
 *
 *  Created on: Jan 18, 2012
 *      Author: issle
 */

#ifndef MarkovMatrix_HPP_
#define MarkovMatrix_HPP_

#include <iostream>
#include <vector>
#include "commons/math/Math.hpp"

using namespace std;


/**
 * This class defines a Matrix representing a Markov
 * chain of first order.
 */
class MarkovMatrix
{
private:
	vector<vector<float> > matrix;
public:
	MarkovMatrix(int size)
	{
		matrix.resize(size);

		for(int i = 0; i < size; i++)
		{
			matrix[i].resize(size);
		}

		for(int i = 0; i < size; i++)
		{
			for(int j = 0;j < size; j++)
			{
				matrix[i][j] = 0;
			}
		}
	}

	vector<float> getSubGroup(int i)
	{
		return matrix[i];
	}

	void setProbability(int i , int j, float value)
	{
		int size = matrix.size();
		if(i >= size || j >= size)
		{
			size = Math<int>::max(i+1, j+1);
			matrix.resize(size);

			for(int i = 0; i < size; i++)
			{
				matrix[i].resize(size);
			}
		}

		matrix[i][j] = value;
	}

	float getProbability(int i, int j)
	{
		return matrix[i][j];
	}

	void print()
	{
		for(int i = 0; i < matrix.size(); i++)
		{
			for ( int j = 0; j < matrix.size(); j++)
			{
				cout << matrix[i][j] << " ";
			}

			cout << endl;
		}
	}

	int size()
	{
		return matrix.size();
	}

	/*
	 * Each row must have a sum of one. Call only when using
	 * a user specified matrix since the extraction precision
	 * may lead to an output different than 1 but close enough
	 * to it.
	 */
	bool validate()
	{
		for(int i = 0; i < matrix.size(); i++)
		{
			float sum = 0;
			for ( int j = 0; j < matrix.size(); j++)
			{
				sum+= getProbability(i,j);
			}

			if(sum != 0 && sum != 1)
			{
				cout << "This Markov matrix does not have a row sum of probabilities equal to zero or one.";
				return false;
			}

			sum = 0;
		}

		return true;
	}
};


#endif /* MarkovMatrix_HPP_ */
