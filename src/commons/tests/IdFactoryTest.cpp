/*
 * IdFactoryTest.cpp
 *
 *  Created on: Dec 27, 2011
 *      Author: issle
 */


#include <iostream>
#include "../IdFactory.hpp"

using namespace std;

class Sample1: public IdFactory<Sample1, int>
{

};

class Sample2: public IdFactory<Sample2,int>
{

};

int main()
{
	Sample1 s1,s11;
	Sample2 s2;

	cout << "Sample1 1: " << s1.getNextId()<<endl;
	cout << "Sample1 2: " << s11.getNextId()<<endl;
	cout << "Sample2 1: " << s2.getNextId()<<endl;
	cout << "Sample1 1 Again: " << s1.getNextId()<<endl;
}

