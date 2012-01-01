/*
 * ClusterManager.hpp
 *
 *  Created on: Dec 28, 2011
 *      Author: issle
 */

#ifndef CLUSTERMANAGER_HPP_
#define CLUSTERMANAGER_HPP_

#include <iostream>
#include <map>
#include "Preprocessor.hpp"
#include "Accessor.hpp"
#include "Cluster.hpp"


using namespace std;

typedef Cluster<typename FlowPreproc::OutputObject, AccessorT> ClusterType;

template < class Cluster, class Preprocessor> class ObjectWrapper
{
private:
	Cluster* cluster_;
	typename Preprocessor::OutputObject* object_;
public:
	ObjectWrapper(Cluster* cluster, typename Preprocessor::OutputObject* object)
	{
		cluster_ = cluster;
		object_ = object;
	}

	void setCluster(Cluster* c)
	{
		cluster_ = c;
	}

	Cluster* getCluster()
	{
		return cluster_;
	}

	typename Preprocessor::OutputObject* getObject()
	{
		return object_;
	}

};

typedef ObjectWrapper<ClusterType, FlowPreproc> ObjectWr;

template< class Accessor, class Cluster, class Preprocessor> class ClusterManager
{
public:
	typedef std::pair<typename Accessor::IdType,Accessor*> mypair;
private:

	vector<Accessor*> accessors_;
	vector<ObjectWr*> objects_;
	Preprocessor* preproc_;

	/**
	 * The number of clusters to use.
	 */
	int clusterNumber_;

	/*
	 * Every algorithm has clusters so there
	 * is a need to help clusters somewhere.
	 */
	vector<Cluster*> clusters_;

public:

	typedef map<typename Preprocessor::OutputObject::IdType, typename Preprocessor::OutputObject*> ObjectsMapType;

	ClusterManager(int clusterNumber, Preprocessor* preproc)
	{
		clusterNumber_ = clusterNumber;
		preproc_ = preproc;
	}

	void addAccessor(Accessor* accessor)
	{
		accessors_.push_back(accessor);
	}

	void cluster()
	{
		bool didMove = false;
		int i = 0;
		for(i = 0; i < clusterNumber_; i++)
		{
			Cluster* cluster = new Cluster();
			clusters_.push_back(cluster);

			typename Preprocessor::OutputObject* item = objects_[i]->getObject();

			cluster->getObjects()[item->getId()] = item;

			objects_[i]->setCluster(cluster);

			for(int i =0; i < accessors_.size(); i++)
			{
				Accessor* acc = accessors_[i];
				cluster->getCenter()[acc->getName()] = acc->getValue(objects_[i]->getObject());

			}
		}

		didMove = true;

		while(didMove == true)
		{
			didMove = false;
			while(i < objects_.size())
			{
				i++;
				typename Preprocessor::OutputObject* item = objects_[i]->getObject();

				Cluster* bestCluster = NULL;
				typename Accessor::ValueType bestDistance = 0;

				for(int j = 0; j < clusters_.size(); j++)
				{
					Cluster* c = clusters_[j];

					if(bestCluster == NULL)
					{
						bestCluster = c;
						bestDistance = getDistance(c,item);
						continue;
					}

					typename Accessor::ValueType candidateDistance = getDistance(c,item);

					if(candidateDistance < bestDistance)
					{
						bestDistance = candidateDistance;
						bestCluster = c;
					}
				}



				//Remove the item from its old cluster.
				Cluster* old = objects_[i]->getCluster();

				if(bestCluster == old)
					continue;

				if(old != NULL)
					old->getObjects()[item->getId()] = NULL;


				//Add item to the new cluster.
				objects_[i]->setCluster(bestCluster);
				bestCluster->getObjects()[item->getId()] = item;

				didMove = true;
			}

			i = 0;

		}



	}

	typename Accessor::ValueType getDistance(Cluster* c, typename Preprocessor::OutputObject* item)
	{
		int count = accessors_.size();

		typename Accessor::ValueType value = 0;


		//BOOST_FOREACH(mypair acc , accessors_)
		//{
		//	value += (typename Accessor::ValueType)(acc.second->getValue(item)/count);
		//}

		return value;
	}

	void calculateNewCenter(Cluster* c)
	{
		BOOST_FOREACH(Accessor* proc, accessors_)
		{
			ObjectsMapType* objectsMap = c->getObjects();

			int objSize = objectsMap->size();

			typename Accessor::ValueType value = 0;

			BOOST_FOREACH(typename Preprocessor::OutputObject* obj, objectsMap)
			{
				value += proc->getValue(obj)/objSize;
			}

			c->getCenter()[proc->getId()] = value;
		}
	}

};


typedef ClusterManager<AccessorT,ClusterType,FlowPreproc> KMeansManager;

#endif /* CLUSTERMANAGER_HPP_ */
