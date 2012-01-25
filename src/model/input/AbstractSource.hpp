/*
 * Source.hpp
 *
 *  Created on: Jan 23, 2012
 *      Author: issle
 */

#ifndef SOURCE_HPP_
#define SOURCE_HPP_

/*
 * An AbstractSource, is a source of data used
 * by the Capit 2 system to either extract information
 * or use the provided information in the replay mode
 * so as to generate or replay traffic.
 */
class AbstractSource
{
public:
	/*
	 * Uses this Source in replay mode.
	 */
	virtual void replay() = 0;

	/*
	 * Uses this Source in extract mode.
	 */
	virtual void extract() = 0;

	/*
	 * Prints the details of this source.
	 */
	virtual void print() = 0;

	virtual ~AbstractSource()
	{

	}
};


#endif /* SOURCE_HPP_ */
