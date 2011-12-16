/*
 * Generator.hpp
 *
 *  Created on: Dec 16, 2011
 *      Author: issle
 */

#ifndef GENERATOR_HPP_
#define GENERATOR_HPP_

#include "../Commons.hpp"
#include "../Thread.hpp"
#include "Parameters.hpp"

typedef Logger<1,NoLock> GeneratorLogger;
typedef Entity2<ThreadShell,GeneratorLogger> GeneratorT;

/**
 * A generator is a class that based on some Parameters
 * generates Packet objects and feeds them to the Client
 * Manager. A generator should implement threading
 * functionality since it should emulate a real source
 * of traffic.
 */
class Generator: GeneratorT
{
public:
	virtual void initialize(Parameters params);

};


#endif /* GENERATOR_HPP_ */
