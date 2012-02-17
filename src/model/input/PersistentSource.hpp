/*
 * PersistentSource.hpp
 *
 *  Created on: Jan 23, 2012
 *      Author: issle
 */

#ifndef PERSISTENTSOURCE_HPP_
#define PERSISTENTSOURCE_HPP_

#include <iostream>
#include <string>
#include "AbstractSource.hpp"
#include "FlowManager.hpp"
using namespace std;

class PersistentSource: public AbstractSource
{
	string filename_;
	string filter_;
public:
	PersistentSource(string filename, string filter) :
			filename_(filename), filter_(filter)
	{

	}

	string getFilname()
	{
		return filename_;
	}

	void print()
	{
		cout << "PersistentSource info:" << endl;
		cout << "Filename : " << filename_ << endl;
	}

	void replay()
	{
		PacketSource* source = new PacketSource();
		source->openSource(filename_.c_str());

		if (filter_.compare("NONE") != 0)
		{
			source->setFilter(filter_);
		}

		ClientManagerInstance::getInstance().registerSource();
		source->execute();
	}


	void extract()
	{

		PacketSource* source = new PacketSource();
		source->openSource(filename_.c_str());

		if (filter_.compare("NONE") != 0)
		{
			source->setFilter(filter_);
		}
		else
		{
			cout << "No filter specified." << endl;
		}
		ClientManagerInstance::getInstance().registerSource();
		FlowManager* manager = new FlowManager();
		Packet* packet = NULL;
		while ((packet = source->getNextPacket()) != NULL)
		{
			manager->accept(*packet);
		}
		manager->calc();

	}

};

#endif /* PERSISTENTSOURCE_HPP_ */
