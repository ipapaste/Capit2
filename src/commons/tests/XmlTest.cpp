/*
 * XmlTest.cpp
 *
 *  Created on: Jan 6, 2012
 *      Author: issle
 */

#include "../XMLParser.hpp"

#include <iostream>
using namespace std;

class CapitXMLParser: public XMLParser
{
public:
	CapitXMLParser(string filename): XMLParser(filename)
	{

	}

	void readImpl(ptree& tree)
	{
		cout << "went here." << endl;
		BOOST_FOREACH( ptree::value_type const& application, tree.get_child("capit") )
		{
			string appName = application.second.get("<xmlattr>.name","");

			if(appName == "ftp")
			{
				BOOST_FOREACH( ptree::value_type const& component, application.second )
				{
					if(component.first == "credential")
					{
						string username = component.second.get("<xmlattr>.username","");
						string password = component.second.get("<xmlattr>.password","");
						cout << username << " " << password << endl;
					}
					else if(component.first == "command")
					{
						string name = component.second.get("<xmlattr>.name","");
						string value = component.second.get("<xmlattr>.value","");
						cout << name << " " << value << endl;
					}
				}
			}

		}
	}
};

int main()
{
	CapitXMLParser* p = new CapitXMLParser("build.xml");
	p->read();
	return 0;
}

