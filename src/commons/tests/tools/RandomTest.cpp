/*
 * RandomTest.cpp
 *
 *  Created on: Jan 1, 2012
 *      Author: issle
 */


#include <iostream>
#include "../../tools//Rnd.hpp"

using namespace std;

int main()
{
	Rnd rnd;
	for(int i = 0; i < 10; i++)
		cout << rnd.getInt(10) << endl;
	return 0;
}

