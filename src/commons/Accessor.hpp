/*
 * Accessor.hpp
 *
 *  Created on: Dec 1, 2011
 *      Author: issle
 */

#ifndef ACCESSOR_HPP_
#define ACCESSOR_HPP_

/**
 * An AbstractAccessor is a class that provides an
 * interface to an Abstract class and can be used
 * to implement polymorphism with Template programming.
 */
template < class AbstractClass> class AbstractAccessor
{
public:
	typedef AbstractClass ReturnType;
	virtual ReturnType getNewObject() = 0;
};

/**
 * An ObjectAccessor is a class that represents a wrapper
 * around a specific type of class that serves as an Object
 * generator.
 */
template < class Object, class Accessor> class ObjectAccessor : public Accessor
{
public:
	typename Accessor::ReturnType getNewObject()
	{
		return new Object;
	}
};


#endif /* ACCESSOR_HPP_ */
