/*
 * CapitInputParser.hpp
 *
 *  Created on: Jan 23, 2012
 *      Author: issle
 */

#ifndef CAPITINPUTPARSER_HPP_
#define CAPITINPUTPARSER_HPP_

#include <iostream>
#include "commons/XMLParser.hpp"
#include "FlowState.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include "model/input/PersistentSource.hpp"
#include "model/input/SyntheticSource.hpp"
#include "Flow.hpp"
#include "model/input/SourceManager.hpp"
using namespace std;
using namespace boost;

class CapitInputParser: public XMLParser
{
public:
	CapitInputParser(string filename): XMLParser(filename)
	{

	}

	void readImpl(ptree& tree)
	{

		BOOST_FOREACH( ptree::value_type const& source, tree.get_child("capit") )
		{
			string sourceType = source.second.get("<xmlattr>.type","");

			if(sourceType.size() <2)
				continue;

			if(String::areEqual(sourceType, "synthetic"))
			{
				string matrix = source.second.get("<xmlattr>.matrix","");

				int clients = boost::lexical_cast<int, std::string>(source.second.get("<xmlattr>.clients",""));
				int port = boost::lexical_cast<int, std::string>(source.second.get("<xmlattr>.port",""));
				int delay = boost::lexical_cast<int, std::string>(source.second.get("<xmlattr>.delay",""));

				MarkovMatrix group(1);

				char_separator<char> sep(";");

				tokenizer<char_separator<char> > tokens(matrix, sep);

				int i = 0;
				int j = 0;

				BOOST_FOREACH(string line, tokens)
				{
					char_separator<char> sep(" ");

					tokenizer<char_separator<char> > colTokens(line, sep);

					BOOST_FOREACH(string value, colTokens)
					{
						float val = boost::lexical_cast<float, std::string>(value);
						group.setProbability(i,j,val);
						j++;
					}
					i++;
					j = 0;
				}

				SyntheticSource* source = new SyntheticSource(group,delay, port, clients);
				SourceManager::getInstance()->addSource(source);

			}
			else if(String::areEqual(sourceType, "persistent"))
			{
				string filename= source.second.get("<xmlattr>.filename","");
				string filter = source.second.get("<xmlattr>.filter","");

				PersistentSource* source = new PersistentSource(filename,filter);
				SourceManager::getInstance()->addSource(source);
			}
			else
			{
				cout << "Type: " << sourceType << " is not a valid source type. It can be either synthetic or persistent." << endl;
			}

		}
	}
};


#endif /* CAPITINPUTPARSER_HPP_ */
