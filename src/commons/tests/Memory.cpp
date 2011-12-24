/*
 * Memory.cpp
 *
 *  Created on: Dec 24, 2011
 *      Author: issle
 */

#include "../MemoryPool.hpp"
#include "../TimeBench.hpp"
#include <sys/resource.h>
#include <string>


class SampleNP
{
private:
	int x;
public:
	SampleNP()
	{
		x = 10;
	}
};

#define MAX 20000000

int main()
{
	//TimeBench* t = new TimeBench();

	//SampleP* s = new SampleP();
	//SampleNP* ns = new SampleNP();

	//cout << sizeof(s) << endl;
	//cout << sizeof(ns) << endl;

	//t->openProbe("Pool");
	for (int i = 0; i < MAX; i++)
	{
		SampleNP* s = new SampleNP();

	}
	//t->closeProbe("Pool");

	//t->openProbe("No Pool");
	//for (int i = 0; i < MAX; i++)
	//{
	//	SampleNP* s = new SampleNP();

	//}
	//t->closeProbe("No Pool");

	//t->printProbe("Pool");
	//t->printProbe("No Pool");

	int who = RUSAGE_SELF;
	struct rusage usage;
	int ret;

	ret = getrusage(who, &usage);

	cout << usage.ru_maxrss << endl;
	return 0;
}

