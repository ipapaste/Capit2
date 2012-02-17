/*
 * Matrix.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include "../IMatrix.hpp"
#include "../../commons/math/Matrix.hpp"
#include <vector>
using namespace std;

class Matrix: public IMatrix
{
private:
	commons::math::Matrix<double> matrix;
public:

	virtual ~Matrix()
	{

	}

	void setValue(int i , int j, double value)
	{
		matrix.setValue(i,j,value);
	}

	double getValue(int i , int j)
	{
		return matrix.getValue(i,j);
	}

	int getSize()
	{
		return matrix.size();
	}

	vector<double> getLineCopy(int i)
	{
		return matrix.getLine(i);
	}

	void print()
	{
		matrix.print();
	}

	void resize(int size)
	{
		matrix = commons::math::Matrix<double>(size);
	}
};


#endif /* MATRIX_HPP_ */
