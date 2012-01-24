/*
 * FlowTypeManager.hpp
 *
 *  Created on: Jan 24, 2012
 *      Author: issle
 */

#ifndef FLOWTYPEMANAGER_HPP_
#define FLOWTYPEMANAGER_HPP_

#include <iostream>
#include <string>

using namespace std;

/**
 * Class that defines the list of possible FlowTypes
 * that exist in Capit2. Gets populated from persistent
 * data.
 */
class Flows
{
private:
	map<int,FlowType*> flowTypes;
public:

	void addType(int id, FlowType* type)
	{
		flowTypes[id] = type;
	}

	FlowType* getType(int id)
	{
		FlowType* type = NULL;

		type = flowTypes[id];

		return type;
	}

	int size()
	{
		return flowTypes.size();
	}
};

typedef Singleton<Flows> FlowTypeManager;


#endif /* FLOWTYPEMANAGER_HPP_ */
