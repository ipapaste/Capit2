/*
 * Rnd.hpp
 *
 *  Created on: Dec 15, 2011
 *      Author: issle
 */

#ifndef RND_HPP_
#define RND_HPP_

#include <iostream>
#include <time.h>
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

using namespace std;

/**
 * Library independent interface to random numbers.
 * Current implementation uses boost. This library
 * can be extended for non-int usage but is intended
 * atm for network delays which we count in milli-
 * seconds thus int/long.
 */
class Rnd
{
private:
	static boost::random::mt19937 rng;
public:

	/*
	 * Returns an integer between start and end.
	 */
	static int getInt(int start, int end)
	{
		boost::random::uniform_int_distribution<> disto(start,end);
		return disto(rng);
	}

	/*
	 * Returns an integer between zero and end.
	 */
	static int getInt(int end)
	{
		return getInt(0,end);
	}

	/*
	 * Returns an integer based on Gaussian Dist.
	 */
	static int getNormal(float mean , float std)
	{
		  boost::normal_distribution<> nd(mean, std);

		  boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > var_nor(rng, nd);

		  return static_cast<int>(var_nor());
	}

	/*
	 * Returns a positive int based on Gaussian Dist.
	 * Negative values are set to zero.
	 */
	static int getNormalPositive(float mean, float std)
	{
		return getNormalCutoff(mean,std,0);
	}

	/*
	 * Returns an int based on Gaussian Dist. Values
	 * below cutoff, are set to cutoff automatically.
	 */
	static int getNormalCutoff(float mean , float std, int cutoff)
	{
		int result = getNormal(mean, std);

		if(result < cutoff )
			result = cutoff;

		return result;
	}
};

boost::random::mt19937 Rnd::rng(time(0));

#endif /* RND_HPP_ */
