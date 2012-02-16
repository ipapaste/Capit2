/*
 * LexicalCast.hpp
 *
 *  Created on: Feb 8, 2012
 *      Author: issle
 */

#ifndef LEXICALCAST_HPP_
#define LEXICALCAST_HPP_

#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;

class LexicalCast
{
public:
	template<typename T> static T get(string s)
	{
		return boost::lexical_cast<T>(s);
	}
};


#endif /* LEXICALCAST_HPP_ */
