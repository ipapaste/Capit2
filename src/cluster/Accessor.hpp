/*
 * Accessor.hpp
 *
 *  Created on: Dec 27, 2011
 *      Author: issle
 */

#ifndef CLUSTER_ACCESSOR_HPP_
#define CLUSTER_ACCESSOR_HPP_

#include "Preprocessor.hpp"
#include <map>
#include "commons/Tools.hpp"

using namespace std;

/**
 * In clustering, a set of Objects are grouped together
 * via an algorithm based on their similarity. The
 * similarity measure or else the distance between
 * the objects, when measured takes into account a set
 * of characteristics of those objects.
 *
 * An Accessor is an object that extracts one such
 * characteristic. Each Accessor queries every object
 * and returns a value for each one that represents the
 * characteristic.
 *
 * Different Accessors extract different kind of
 * characteristics and can be combined together
 * in a space ( Eucledean ) with contribution that
 * can be either weighted or not.
 */
template < class ValueT , class OutputObject > class Accessor
{
private:
	string name;
public:

	typedef string IdType;

	/*
	 * The value type usually is a number.
	 */
	typedef ValueT ValueType;
	/**
	 * Returns the specific characteristic value that
	 * this Accessor extracts from the object.
	 */
	ValueT getValue(OutputObject t)
	{
		/*
		 * Lazy caching should yield the best results.
		 */
		if(cache.count(t->getId()) == 0)
		{
			ValueT newValue = getValueImpl(t);

			cache[t->getId()] = newValue;

			return newValue;
		}
		else
			return cache[t->getId()];
	}

	/**
	 * Implements the value extraction via polymorphism.
	 */
	virtual ValueT getValueImpl(OutputObject object) = 0;

	virtual ~Accessor()
	{

	}

	IdType getName()
	{
		return name;
	}
private:

	/**
	 * Since each extraction might be a complex operation
	 * a cache should reduce a lot the computation
	 * complexity of the operation by storing the extracted
	 * value for future use, for example when recalculating
	 * the centers of k-means algorithm.
	 */
	map<string,ValueT> cache;
};

typedef Accessor<float,Flow*> AccessorT;

class DelayAccessor:public AccessorT
{
public:

	DelayAccessor()
	{

	}

	AccessorT::ValueType getValueImpl(Flow* flow)
	{
		map<int,Packet*>& packets = *flow->getInputObjects();

		map<int,Packet*>::iterator it;

		Packet* previousPacket = NULL;

		float delay = 0;
		float count = 0;
		for(it = packets.begin(); it != packets.end(); ++it)
		{
			if(previousPacket == NULL)
			{
				previousPacket = it->second;
				continue;
			}
			delay += it->second->getTimestamp() -previousPacket->getTimestamp();

			count++;

			previousPacket = it->second;
		}

		return delay/count;
	}
};

#endif /* ACCESSOR_HPP_ */
