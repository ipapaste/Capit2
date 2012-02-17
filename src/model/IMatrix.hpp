/*
 * IMatrix.hpp
 *
 *  Created on: Feb 16, 2012
 *      Author: issle
 */

#ifndef IMATRIX_HPP_
#define IMATRIX_HPP_
#include <vector>
using namespace std;
#include "../commons/Printable.hpp"
class IMatrix: public Printable
{
public:
	virtual bool validate() = 0;

	virtual ~IMatrix()
	{

	}

	virtual void setValue(int i , int j, double value) = 0;

	virtual double getValue(int i , int j) = 0;

	virtual int getSize() = 0;

	virtual vector<double> getLineCopy(int i) = 0;

	virtual void resize(int size) = 0;
};

#endif /* IMATRIX_HPP_ */
