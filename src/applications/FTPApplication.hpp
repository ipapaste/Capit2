/*
 * FTPApplication.hpp
 *
 *  Created on: Dec 1, 2011
 *      Author: issle
 */

#ifndef FTPAPPLICATION_HPP_
#define FTPAPPLICATION_HPP_

#include "../AbstractApplication.hpp"
#include "commons/math/Rnd.hpp"
#include "commons/XMLParser.hpp"
#include "FtpPreprocessor.hpp"
#include <iostream>

using namespace std;

class FTPApplication: public AbstractApplication
{
public:
	FTPApplication():AbstractApplication(21)
	{
		AbstractPreprocessor* pr = new FtpPreprocessor();
		addPreprocessor(pr);
	}
};

#endif /* FTPAPPLICATION_HPP_ */
