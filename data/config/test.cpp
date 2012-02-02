/*
 * test.cpp
 *
 *  Created on: Jan 28, 2012
 *      Author: issle
 */
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include "../../src/commons/math/Rnd.hpp"
#include <map>

#include <deque>

using boost::property_tree::ptree;
using namespace std;



class Generator
{
public:
	static string getRandom()
	{
		return "hi";
	}
};

template <class RandomGenerator> class ValueGroup_
{
private:
	map<string,deque<string> > values;

public:
	string getValue(string var)
	{
		if(values.count(var) != 0 && values[var].size() !=0)
		{
			if(values[var].size() == 1)
				return values[var].front();
			else
				return values[var][Rnd::getInt(0,values[var].size()-1)];
		}
		else
		{
			return RandomGenerator::getRandom();
		}
	}

	void setValue(string var, string value)
	{
		if(values.count(var) == 0)
		{
			deque<string> s;
			s.push_back(value);
			values[var] = s;
		}
		else
			values[var].push_back(value);
	}
};

typedef ValueGroup_<Generator> ValueGroup;

int main()
{
	ptree pt;

	read_xml("test.xml", pt);

	BOOST_FOREACH( ptree::value_type& source, pt.get_child("capit") )
	{
		BOOST_FOREACH( ptree::value_type& item, source.second )
		{
			if(item.first == "states")
			{

			}
			else if(item.first == "groups")
			{
				BOOST_FOREACH( ptree::value_type& group, item.second )
				{
					ValueGroup vgroup;
					BOOST_FOREACH( ptree::value_type& value, group.second )
					{
						string key(value.first);
						BOOST_FOREACH( ptree::value_type& valuesingle, value.second )
						{
							string data(valuesingle.second.data());
							vgroup.setValue(key, data);
						}
					}

					cout << vgroup.getValue("name") << endl;
					cout << vgroup.getValue("file") << endl;
				}
			}
		}
	}

	return 0;
}



