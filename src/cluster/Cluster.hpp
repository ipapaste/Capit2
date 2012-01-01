/*
 * Cluster.hpp
 *
 *  Created on: Dec 27, 2011
 *      Author: issle
 */

#ifndef CLUSTER_HPP_
#define CLUSTER_HPP_

#include "../commons/Lock.hpp"

#include <map>

using namespace std;

template <class OutputObject, class Accessor> class Cluster
{
private:
	map<typename OutputObject::IdType, OutputObject*> objects;
	map<typename Accessor::IdType,typename Accessor::ValueType> center;
public:
	map<typename OutputObject::IdType, OutputObject*>& getObjects()
	{
		return *&objects;
	}

	map<typename Accessor::IdType, typename Accessor::ValueType>& getCenter()
	{
		return *&center;
	}
};


#endif /* CLUSTER_HPP_ */
