/*
 * XMLParser.hpp
 *
 *  Created on: Jan 6, 2012
 *      Author: issle
 */

#ifndef XMLPARSER_HPP_
#define XMLPARSER_HPP_

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/foreach.hpp>

using boost::property_tree::ptree;
using namespace std;

class XMLParser
{
private:
	ptree* tree_;
	string filename_;
public:

	typedef ptree TreeType;

	XMLParser(string filename)
	{
		tree_ = NULL;
		filename_ = (filename);
	}

	virtual void readImpl(ptree& tree) = 0;

	void read()
	{
		delete tree_;

		tree_ = new ptree();

		ptree& tree = *tree_;

		read_xml(filename_, tree);

		readImpl(tree);

	}

	~XMLParser()
	{

	}
};

#endif /* XMLPARSER_HPP_ */
