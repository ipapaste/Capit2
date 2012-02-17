#ifndef CAPIT_XMLMARKOV_PARSER_
#define CAPIT_XMLMARKOV_PARSER_

#include "commons/XMLParser.hpp"
//#include "FlowState.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>
//#include "Flow.hpp"
//#include "ValueGroup.hpp"
//#include "ValueGroupManager.hpp"
#include "service/CapitService.hpp"
#include "model/IFlowState.hpp"
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

			IFlowType& app = CapitService::getInstance().getFlowTypeService().getNewFlowType();
			app.setName(appName);
			app.setPort(appPort);


			BOOST_FOREACH( ptree::value_type const& component, application.second )
			{

				if(component.first == "state")
				{
					string stateName = component.second.get("<xmlattr>.name","");
					if(stateName.size()<1)
						continue;
					int stateId = boost::lexical_cast<int, std::string>(component.second.get("<xmlattr>.id",""));

					IFlowState& state = CapitService::getInstance().getFlowStateService().getNewFlowState();
					state.setId(stateId);
					state.setName(stateName);




					BOOST_FOREACH( ptree::value_type const& packet, component.second )
					{
						string packetName = packet.second.get("<xmlattr>.regex","");
						string packetValue = packet.second.get("<xmlattr>.value","");
						if(packetName.size()<1)
							continue;

						ICommand& command = CapitService::getInstance().getCommandService().getNewCommand();
						command.setName(packetName);
						command.setCommand(packetValue);
						state.addCommand(command);

					}
					app.addState(state);
				}
				else if(component.first == "groups")
				{
					BOOST_FOREACH( ptree::value_type const&  group, component.second )
					{
						IValueGroup& vgroup = CapitService::getInstance().getValueGroupService().getNewValueGroup();
						BOOST_FOREACH( ptree::value_type const& value, group.second )
						{
							string key(value.first);
							BOOST_FOREACH( ptree::value_type const& valuesingle, value.second )
							{
								string data(valuesingle.second.data());
								vgroup.addValue(key, data);
							}
						}
						CapitService::getInstance().getValueGroupService().addGroup(vgroup,appPort);
					}

				}
			}
			CapitService::getInstance().getFlowTypeService().addType(appPort , app);
		}

		CapitService::getInstance().print();
	}
};

#endif
