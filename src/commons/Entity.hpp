/*
 * Entity.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

/**
 * An Entity<?> is a combination of ? Policies
 * into one single unity.
 */

template <class T1, class T2> class Entity2:
public T1,
public T2
{

};

template <class T1, class T2, class T3> class Entity3:
public T1,
public T2,
public T3
{

};

template <class T1, class T2, class T3, class T4> class Entity4:
public T1,
public T2,
public T3,
public T4
{

};

template <class T1, class T2, class T3, class T4, class T5> class Entity5:
public T1,
public T2,
public T3,
public T4,
public T5
{

};

template <class T1, class T2, class T3, class T4, class T5, class T6> class Entity6:
public T1,
public T2,
public T3,
public T4,
public T5,
public T6
{

};


#endif /* ENTITY_HPP_ */
