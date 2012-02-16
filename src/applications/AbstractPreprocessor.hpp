/*
 * AbstractPreproccessor.hpp
 *
 *  Created on: Feb 7, 2012
 *      Author: issle
 */

#ifndef ABSTRACTPREPROCCESSOR_HPP_
#define ABSTRACTPREPROCCESSOR_HPP_

#include <iostream>
#include "Packet.hpp"
#include "../commons/Printable.hpp"
using namespace std;

class AbstractPreprocessor:public Printable
{
public:
	virtual void preprocess(Packet& packet, string oldResponse) = 0;

	virtual ~AbstractPreprocessor()
	{

	}
};

#endif /* ABSTRACTPREPROCCESSOR_HPP_ */
