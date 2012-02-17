/*
 * DelayMatrix.hpp
 *
 *  Created on: Feb 17, 2012
 *      Author: issle
 */

#ifndef DELAYMATRIX_HPP_
#define DELAYMATRIX_HPP_
#include "Matrix.hpp"

class DelayMatrix: public Matrix
{
public:
	bool validate()
	{
		return true;
	}
};


#endif /* DELAYMATRIX_HPP_ */
