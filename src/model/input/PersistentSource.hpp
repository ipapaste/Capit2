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

using namespace std;

class PersistentSource
{
	string filename_;
	string filter_;
public:
	PersistentSource(string filename, string filter):filename_(filename),filter_(filter)
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

	void init()
	{
		PacketSource* source = new PacketSource();
		source->openSource(filename_.c_str());

		if(filter_.compare("NONE") != 0)
		{
			source->setFilter(filter_);
		}

		ClientManagerInstance::getInstance()->registerSource();
		ThreadShell::schedule(source);
	}
};


#endif /* PERSISTENTSOURCE_HPP_ */
