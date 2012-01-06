/*
 * String.hpp
 *
 *  Created on: Jan 6, 2012
 *      Author: issle
 */

#ifndef STRING_HPP_
#define STRING_HPP_

#include <boost/algorithm/string.hpp>
#include <iostream>

using namespace std;


/**
 * This class allows implementation independent
 * std::string manipulation.
 */
class String
{
public:

	/**
	 * Replaces the first occurrence of pattern in the source reference
	 * with the data std::string. Subsequent calls to this will progressively
	 * replace the remaining occurrences of the pattern.
	 */
	static string& replace(string& source, string pattern, string data)
	{
		boost::replace_first(source, pattern, data);

		return source;
	}

	/**
	 * Replaces all the occurrences of the pattern in the source reference
	 * with the data std::string.
	 */
	static string& replaceAll(string&source, string pattern, string data)
	{
		boost::replace_all(source,pattern,data);

		return source;
	}
};


#endif /* STRING_HPP_ */
