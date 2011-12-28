/*
 * Cluster.hpp
 *
 *  Created on: Dec 27, 2011
 *      Author: issle
 */

#ifndef CLUSTER_HPP_
#define CLUSTER_HPP_

#include "../commons/Lock.hpp"

#include <vector>

using namespace std;

template <class OutputObject, class Accessor> class Cluster
{
	typedef typename Accessor::ValueType ValueType;

private:
	 map<int,ValueType>* accessors_;
	 map<int, OutputObject*> items_;

public:


	Cluster(vector<Accessor*>& accessors, OutputObject* obj)
	{
		vector<Accessor*>::iterator it;

		for(it = accessors.begin(); it < accessors.end(); ++it)
		{
			Accessor* accessor = it;
			ValueType value = accessor->getValue(obj);
			accessors[accessor->getId()] = value;
			items_[obj->getId()] = obj;
		}
		//TODO: For each accessor get the obj value to match.
	}

	void regenerateCenter()
	{

	}

};


#endif /* CLUSTER_HPP_ */
