/*
 * MarkovMatrix.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef MARKOVMATRIX_HPP_
#define MARKOVMATRIX_HPP_
#include "Matrix.hpp"
#include <iostream>
using namespace std;
class MarkovMatrix:public Matrix
{
public:
	bool validate()
	{
		for(int i = 0; i < getSize(); i++)
				{
					float sum = 0;
					for ( int j = 0; j < getSize(); j++)
					{
						sum+= getValue(i,j);
					}

					if(sum > 0.99)
						sum = 1;
					if(sum != 0 && sum != 1)
					{
						cout << "Row: " << i <<" Sum: " << sum <<": This Markov matrix does not have a row sum of probabilities equal to zero or one." << endl;
						return false;
					}

					sum = 0;
				}

				cout <<"Markov probability matrix is validated successfully." << endl;
				return true;
	}
};


#endif /* MARKOVMATRIX_HPP_ */
