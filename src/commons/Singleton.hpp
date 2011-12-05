/*
 * Singleton.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

/**
 * This class represents a wrapper around
 * a specific class that transforms it into
 * a singleton object.
 *
 * Object instantiation is blocked through
 * protected constructor.
 */
template <class T> class Singleton
{
public:
	static T* getInstance()
	{
		if (!instance)
			instance = new T;


		return instance;
	}
protected:
	Singleton();
	~Singleton();
private:
	Singleton(Singleton const&);
	Singleton& operator=(Singleton const&);
	static T* instance;
};

template<class T> T* Singleton<T>::instance = NULL;

#endif /* SINGLETON_HPP_ */
