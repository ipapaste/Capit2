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
#include "commons/math/Matrix.hpp"

using namespace std;

/**
 * This class defines a Matrix representing a Markov
 * chain of first order.
 */
class MarkovMatrix: public Matrix<float>
{

public:
	MarkovMatrix(int size):Matrix(size)
	{

	}

	vector<float> getSubGroup(int i)
	{
		return getLine(i);
	}

	void setProbability(int i , int j, float value)
	{
		setValue(i,j,value);
	}

	float getProbability(int i, int j)
	{
		return getValue(i,j);
	}

	/*
	 * Each row must have a sum of one. Call only when using
	 * a user specified matrix since the extraction precision
	 * may lead to an output different than 1 but close enough
	 * to it.
	 */
	bool validate()
	{
		for(int i = 0; i < size(); i++)
		{
			float sum = 0;
			for ( int j = 0; j < size(); j++)
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
