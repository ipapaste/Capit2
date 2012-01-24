/*
 * Rnd.hpp
 *
 *  Created on: Dec 15, 2011
 *      Author: issle
 */

#ifndef RND_HPP_
#define RND_HPP_

#include <iostream>
#include <boost/random.hpp>
#include <time.h>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

#include "../container/Singleton.hpp"

double gen_normal(void)
{
  boost::variate_generator<boost::mt19937, boost::normal_distribution<> >
    generator(boost::mt19937(time(0)),
           boost::normal_distribution<>());

  double r = generator();
  return r;
}

using namespace std;

class Rnd
{
	boost::random::mt19937 rng;
public:

	Rnd():rng(time(0))
	{

	}
	int getInt(int start, int end)
	{
		boost::random::uniform_int_distribution<> disto(start,end);
		return disto(rng);
	}

	int getInt(int end)
	{
		return getInt(0,end);
	}
};

typedef Singleton<Rnd> Rand;


#endif /* RND_HPP_ */
