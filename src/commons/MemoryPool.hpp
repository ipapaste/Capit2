/*
 * MemoryPool.hpp
 *
 *  Created on: Dec 24, 2011
 *      Author: issle
 */

#ifndef MEMORYPOOL_HPP_
#define MEMORYPOOL_HPP_

#include <iostream>
using namespace std;


/**
 * Credits to : Scott Meyers.
 *
 * A Memory Pool is a preallocated block of memory
 * that contains a list of "objects" that we intend
 * to use in our application. New and Delete operators
 * have been overloaded to provide memory acquiring
 * through resource request and release.
 */
template<class T> class MemoryPool
{
private:

	/**
	 * The chunk of memory that is allocated
	 * whenever the pool gets full. Optimal
	 * solution would need increment and decrement
	 * of the size according to the rate at which
	 * the application requests or releases resources.
	 */
	static const int BLOCK_SIZE = 1000;

	/*
	 * Points to the head of the free objects block.
	 */
	static T* headOfFreeList;

	/*
	 * Specifies the next object in the block.
	 */
	T* next;


public:

	/*
	 * Acquires a preallocated space from the memory pool.
	 */
	static void* operator new(size_t size)
	{
		if (size != sizeof(T))
			return ::operator new(size);

		T* p = headOfFreeList;

		if (p)
		{
			headOfFreeList = p->next;
		}
		else
		{
			T* newBlock = static_cast<T*>(::operator new(
					BLOCK_SIZE * sizeof(T)));

			for (int i = 0; i < BLOCK_SIZE - 1; ++i)
			{
				newBlock[i].next = &newBlock[i + 1];
			}

			newBlock[BLOCK_SIZE].next = 0;

			p = newBlock;

			headOfFreeList = &newBlock[1];
		}

		return p;
	}

	/*
	 * Releases a space to the preallocated memory pool.
	 */
	static void operator delete(void *deadObject, size_t size)
	{
		if (deadObject == 0)
			return;

		if (size != sizeof(T))
		{
			::operator delete(deadObject);
			return;
		}

		T* carcass = static_cast<T*>(deadObject);

		carcass->next = headOfFreeList;

		headOfFreeList = carcass;
	}
};

template< class T > T* MemoryPool<T>::headOfFreeList = 0;

#endif /* MEMORYPOOL_HPP_ */
