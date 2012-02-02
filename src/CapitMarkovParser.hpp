#ifndef CAPIT_XMLMARKOV_PARSER_
#define CAPIT_XMLMARKOV_PARSER_

#include "commons/XMLParser.hpp"
#include "FlowState.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "Flow.hpp"
#include "ValueGroup.hpp"
#include "ValueGroupManager.hpp"
using namespace std;

/**
 * This class is an XML Parser that reads and validates
 * a file that represents the Markov chain definition
 * of a set of known Application Protocols.
 */
class CapitMarkovParser: public XMLParser
{
public:
	CapitMarkovParser(string filename): XMLParser(filename)
	{

	}

	void readImpl(ptree& tree)
	{

		BOOST_FOREACH( ptree::value_type const& application, tree.get_child("capit") )
		{
			string appName = application.second.get("<xmlattr>.name","");
			int appPort = boost::lexical_cast<int, std::string>(application.second.get("<xmlattr>.port",""));

			FlowType* app = new FlowType(appName, appPort);
			FlowTypeManager::getInstance()->addType(appPort, app);

			BOOST_FOREACH( ptree::value_type const& component, application.second )
			{

				if(component.first == "state")
				{
					string stateName = component.second.get("<xmlattr>.name","");
					if(stateName.size()<1)
						continue;
					int stateId = boost::lexical_cast<int, std::string>(component.second.get("<xmlattr>.id",""));
					FlowState* state = new FlowState(stateId, stateName);

					app->addState(state);


					BOOST_FOREACH( ptree::value_type const& packet, component.second )
					{
						string packetName = packet.second.get("<xmlattr>.regex","");
						string packetValue = packet.second.get("<xmlattr>.value","");
						if(packetName.size()<1)
							continue;

						Command* command = new Command(packetName,packetValue);
						state->addCommand(command);

					}
				}
				else if(component.first == "groups")
				{
					BOOST_FOREACH( ptree::value_type const&  group, component.second )
					{
						ValueGroup vgroup;
						BOOST_FOREACH( ptree::value_type const& value, group.second )
						{
							string key(value.first);
							BOOST_FOREACH( ptree::value_type const& valuesingle, value.second )
							{
								string data(valuesingle.second.data());
								vgroup.setValue(key, data);
							}
						}
						ValueGroupManager::addGroup(vgroup,appPort);
					}

				}
			}
		}
	}
};

#endif
