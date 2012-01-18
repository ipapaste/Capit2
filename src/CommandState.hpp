/*
 * CommandState.hpp
 *
 *  Created on: Jan 18, 2012
 *      Author: issle
 */

#ifndef COMMANDSTATE_HPP_
#define COMMANDSTATE_HPP_

#include <iostream>
#include <string>
#include "FlowState.hpp"

using namespace std;

class CommandState:public FlowState
{
public:

	CommandState():FlowState(1)
	{
	}

};

#endif /* COMMANDSTATE_HPP_ */
