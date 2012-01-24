/*
 * math/Math.hpp
 *
 *  Created on: Jan 23, 2012
 *      Author: issle
 */

#ifndef MATH_HPP_
#define MATH_HPP_

template <typename T>  class Math
{
public:
	static T max(T t1, T t2)
	{
		if(t1 > t2)
			return t1;
		else
			return t2;
	}
};


#endif /* MATH_HPP_ */
