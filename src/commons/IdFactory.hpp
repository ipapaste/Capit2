/*
 * IdFactory.hpp
 *
 *  Created on: Dec 27, 2011
 *      Author: issle
 */

#ifndef IDFACTORY_HPP_
#define IDFACTORY_HPP_

template < class T , class ReturnType> class IdFactory
{
private:
	static ReturnType idCounter;
public:
	static ReturnType getNextId()
	{
		idCounter++;
		return idCounter;
	}
};

template <class T, class ReturnType> ReturnType IdFactory<T,ReturnType>::idCounter = 0;

#endif /* IDFACTORY_HPP_ */
