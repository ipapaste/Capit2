/*
 * SourceManager.hpp
 *
 *  Created on: Jan 24, 2012
 *      Author: issle
 */

#ifndef SOURCEMANAGER_HPP_
#define SOURCEMANAGER_HPP_

#include <iostream>
#include <queue>
#include "commons/container/Singleton.hpp"

using namespace std;

class SourceM
{
private:
	vector<AbstractSource*> sources_;
public:
	void addSource(AbstractSource* source)
	{
		sources_.push_back(source);
	}


	void print()
	{
		for(int i = 0; i < sources_.size(); i++)
		{
			sources_[i]->print();
		}
	}

	void replay()
	{
		for(int i = 0; i < sources_.size(); i++)
		{
			sources_[i]->replay();
		}
	}

	void extract()
	{
		for(int i = 0; i < sources_.size(); i++)
		{
			sources_[i]->extract();
		}
	}
};

typedef Singleton<SourceM> SourceManager;


#endif /* SOURCEMANAGER_HPP_ */
