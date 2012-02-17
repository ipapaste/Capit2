/*
 * container/Singleton.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

/**
 * This template represents a wrapper around
 * a specific class that transforms it into
 * a singleton object.
 *
 *
 * A singleton object should never be initialized
 * since it only exposes static functionality. Each
 * singleton definition as a class is the Singleton
 * Holder of a different class.
 *
 * How to use :
 * Singleton<MyClass> MyClassSingleton;
 * MyClassSingleton::getInstance()....
 */
template <class T> class Singleton
{
public:
	static T& getInstance()
	{
		if (!instance)
			instance = new T;

		return *instance;
	}
private:
	/*
	 * Hide constructor.
	 */
	Singleton();

	/*
	 * Hide destructor (needed ? )
	 */
	~Singleton();

	/*
	 * Hide copy constructor.
	 */
	Singleton(Singleton const&);

	/*
	 * Hide assignment.
	 */
	Singleton& operator=(Singleton const&);

	static T* instance;
};

template<class T> T* Singleton<T>::instance = NULL;

#endif /* SINGLETON_HPP_ */
