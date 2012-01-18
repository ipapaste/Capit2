/*
 * LoginUState.hpp
 *
 *  Created on: Jan 18, 2012
 *      Author: issle
 */

#ifndef LOGINUSTATE_HPP_
#define LOGINUSTATE_HPP_

#include <iostream>
#include <string>
#include "FlowState.hpp"

using namespace std;

class LoginUState:public FlowState
{
public:

	LoginUState():FlowState(0)
	{
	}

};

#endif /* LOGINUSTATE_HPP_ */
