/*
 * IFlowState.hpp
 *
 *  Created on: Feb 16, 2012
 *      Author: issle
 */

#ifndef IFLOWSTATE_HPP_
#define IFLOWSTATE_HPP_
#include "../commons/Printable.hpp"
#include <deque>
#include "ICommand.hpp"
#include "IMatrix.hpp"
#include "../Packet.hpp"
using namespace std;

class IFlowState:public Printable
{


public:
	/*
	 * Sets the matrix of transitions for this state.
	 */
	virtual void setTransitions(IMatrix& group) = 0;

	/*
	 * Gets a random transition based on the transition matrix.
	 */
	virtual int getRandomTransition() = 0;

	/*
	 * Gets all the commands that correspond to this FlowState.
	 */
	virtual deque<ICommand*> getCopyCommands() =0;

	virtual void setId(int id) = 0;

	virtual void setName(string name) = 0;

	virtual int getId() = 0;

	virtual string getName() = 0;

	virtual void addCommand(ICommand& command) = 0;

	virtual bool isUnknown() = 0;

	virtual void setIsUnknown() = 0;

	virtual void accept(Packet& p) = 0;

	virtual void calc() = 0;

	virtual ~IFlowState()
	{

	}
	virtual IFlowState& belongsToState(Packet& packet) = 0;
};


#endif /* IFLOWSTATE_HPP_ */
