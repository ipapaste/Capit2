/*
 * Source.hpp
 *
 *  Created on: Jan 23, 2012
 *      Author: issle
 */

#ifndef SOURCE_HPP_
#define SOURCE_HPP_

class AbstractSource
{
public:
	virtual void replay() = 0;
	virtual void extract() = 0;
	virtual void print() = 0;
};


#endif /* SOURCE_HPP_ */
