/*
 * XmlTest.cpp
 *
 *  Created on: Jan 6, 2012
 *      Author: issle
 */

#include "../XMLParser.hpp"
#include "../../FlowState.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>
using namespace std;

class Application
{
	string name_;
	int port_;
	list<FlowState*> states;
public:
	Application(string name, int port):name_(name)
	{
		port_ = port;
	}

	void addState(FlowState* state)
	{
		states.push_front(state);
	}

	int getPort()
	{
		return port_;
	}

	string getName()
	{
		return name_;
	}

	void print()
	{
		cout << "Application: " << name_ << " Port: " << port_ << endl;
		BOOST_FOREACH(FlowState* state, states)
		{
			state->print();
		}
	}

};

class CapitXMLParser: public XMLParser
{
public:
	CapitXMLParser(string filename): XMLParser(filename)
	{

	}

	void readImpl(ptree& tree)
	{

		BOOST_FOREACH( ptree::value_type const& application, tree.get_child("capit") )
		{
			string appName = application.second.get("<xmlattr>.name","");
			int appPort = boost::lexical_cast<int, std::string>(application.second.get("<xmlattr>.port",""));

			Application* app = new Application(appName, appPort);

			BOOST_FOREACH( ptree::value_type const& component, application.second )
			{

				string stateName = component.second.get("<xmlattr>.name","");
				if(stateName.size()<1)
					continue;
				int stateId = boost::lexical_cast<int, std::string>(component.second.get("<xmlattr>.id",""));
				FlowState* state = new FlowState(stateId, stateName);
				app->addState(state);


				BOOST_FOREACH( ptree::value_type const& packet, component.second )
				{
					string packetName = packet.second.get("<xmlattr>.name","");
					string packetValue = packet.second.get("<xmlattr>.value","");
					if(packetName.size()<1)
						continue;

					Command* command = new Command(packetName,packetValue);
					state->addCommand(command);

				}
			}

			app->print();

		}
	}
};

int main()
{
	CapitXMLParser* p = new CapitXMLParser("build.xml");
	p->read();
	return 0;
}

