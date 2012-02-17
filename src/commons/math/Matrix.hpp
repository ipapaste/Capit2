/*
 * Matrix.hpp
 *
 *  Created on: Jan 28, 2012
 *      Author: issle
 */

#ifndef MATRIXC_HPP_
#define MATRIXC_HPP_

#include <iostream>
#include <iomanip>
#include "Math.hpp"

using namespace std;

namespace commons
{
namespace math
{
template <typename V> class Matrix
{
private:
	vector<vector<V> > matrix;
public:
	Matrix(int size)
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

	Matrix()
	{
		Matrix(1);
	}

	vector<V> getLine(int i)
	{
		return matrix[i];
	}

	void setValue(int i , int j, V value)
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

	float getValue(int i, int j)
	{
		int size = matrix.size();
		if(i >= size || j >= size)
		{
			cout << i << " " << j << " are out of matrix limmits." << endl;
			exit(0);
		}
		return matrix[i][j];
	}

	void print()
	{
		for(int i = 0; i < matrix.size(); i++)
		{
			for ( int j = 0; j < matrix.size(); j++)
			{
				cout << setw(6)<< matrix[i][j] << " ";
			}

			cout << endl;
		}
	}

	int size()
	{
		return matrix.size();
	}


};
}
}




#endif /* MATRIX_HPP_ */
