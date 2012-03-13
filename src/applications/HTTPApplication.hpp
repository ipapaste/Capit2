/*
 * HTTPApplication.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef HTTPAPPLICATION_HPP_
#define HTTPAPPLICATION_HPP_

#include "../AbstractApplication.hpp"
#include "HttpCookiePreprocessor.hpp"

class HTTPApplication: public AbstractApplication
{
public:
	HTTPApplication():AbstractApplication(80)
	{
		AbstractPreprocessor* pr = new HttpCookiePreprocessor();
		addPreprocessor(pr);
	}
};

#endif /* HTTPAPPLICATION_HPP_ */
