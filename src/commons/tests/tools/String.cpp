/*
 * String.cpp
 *
 *  Created on: Jan 6, 2012
 *      Author: issle
 */
#include <iostream>
#include "../../tools/String.hpp"

using namespace std;

int main()
{
	string s1("Test1: double replace <var> <var>");

	String::replace(s1,"<var>", "hi");

	cout << s1 << endl;

	String::replace(s1,"<var>", "hi");

	cout << s1 << endl;

	string s2("Test2: replace all <var> <var>");

	String::replaceAll(s2, "<var>", "hi");

	cout << s2 << endl;
	return 0;
}



