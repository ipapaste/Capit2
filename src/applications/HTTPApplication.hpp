/*
 * HTTPApplication.hpp
 *
 *  Created on: Nov 29, 2011
 *      Author: issle
 */

#ifndef HTTPAPPLICATION_HPP_
#define HTTPAPPLICATION_HPP_

#include "../AbstractApplication.hpp"

class HTTPApplication: public AbstractApplication
{
public:
	HTTPApplication():AbstractApplication(80)
	{

	}
};

#endif /* HTTPAPPLICATION_HPP_ */
