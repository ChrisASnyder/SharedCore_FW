//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          chris.snyder@commongroundss.com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>
//
//==============================================================================

//==============================================================================
//
// File Name:       scFSM.h
// Created By:      Christopher A. Snyder
// Creation Date:   6/1/2014
// $Id: scFSM.h $
//
//==============================================================================

#if !defined(__FINITE_STATE_MACHINE_H_)
#define __FINITE_STATE_MACHINE_H_

#include <vector>
#include "scTypes.h"
#include "scStateMachine.h"
#include "scFSMState.h"
#include "scIQueue.h"

namespace SharedCore
{

	// This macro defines how deep the advance queue can be before any OnInput callers will
	// be forced to wait. In general, most state machines will only need a couple of items
	// to manage the auto advancing where an action during the processing of a state activity
	// will cause the state machine to advance.
#ifndef FSM_MAX_QUEUE_DEPTH
#define FSM_MAX_QUEUE_DEPTH  ((int)20)
#endif

	// no default constructor. This class requires a state machine be provide during construction
	// and as a result a default constructor doesn't make apply. However, if additional initialization
	// member methods were added to allow the matrix to be defined at a later time then this can
	// be removed.
	//lint -esym(1712,scFSM)



	/// <summary>
	/// This class implements a Finite State Machine design pattern.
	/// For this pattern, every state in a state machine has an implementation object
	/// that will perform three possible actions. OnEnter, OnIn, and OnLeave. A state
	/// implementation may choose to implement any or or none of those actions.
	/// When a state is entered, the OnEnter method will be called followed immediately
	/// by the OnIn action. If a state advance keeps the state machine in the current
	/// state then the OnIn method is called. When the state input causes a transition to
	/// another state then the OnLeave method is called.
	/// </summary>
	class scFSM
	{
	public:
		/// <summary>
		/// Construct the state finite state machine using the provided matrix
		/// </summary>
		/// <param name="ni">Number of inputs.</param>
		/// <param name="ns">Number of states</param>
		/// <param name="is">Initial State</param>
		/// <param name="pMatrix">Pointer to the transition matrix. </param>
		/// <param name="pQueue">This is a pointer to the queue that will that the task will place
		/// the next transaction onto.
		/// </param>
		scFSM(int ni, int ns, int is, const int* pMatrix, scIQueue* pQueue );

		/// <summary>
		/// The destructor
		/// </summary>
		virtual ~scFSM();

		/// <summary>
		/// This will assign a state object to handle the Enter, In, and Leaving actions. If a pointer already exists for
		/// the specified state it will be deleted before the next one is added.
		/// </summary>
		/// <param name="pAction">An action object pointer.</param>
		virtual void Set( scFSMState* pAction );

		/// <summary>
		/// This will clear and delete the object for the specified state.
		/// </summary>
		/// <param name="nState"></param>
		/// <param name="bDeleteMemory">when true the memory is deleted and the return is NULL</param>
		virtual scFSMState* Clear( int nState, bool bDeleteMemory );

		/// <summary>
		/// This method will handle the asynchronous inputs that are used to advance the state machine. Every state
		/// transition will cause the Enter, In and leave actions to occur before anther input will be processed. All
		/// inputs are processed in the order in which they are received.
		/// </summary>
		/// <param name="nInput"></param>
		virtual void OnInput( int nInput );

		/// <summary>
		/// This method will handle the asynchronous inputs that are used to advance the state machine. Every state
		/// transition will cause the Enter, In and leave actions to occur before anther input will be processed. All
		/// inputs are processed in the order in which they are received.
		/// </summary>
		/// <param name="nInput"></param>
		virtual void OnInputISR( int nInput );

		/// <summary>
		/// This is a blocking call that will wait for a input to receive or timeout. If an input is received then the
		/// transition actions for the states involved will be called and processed on this task before this method
		/// returns.
		/// </summary>
		/// <param name="nTimeout"></param>
		virtual bool HandleInput( uint32_t nTimeout );

		/// <summary>
		/// Returns to the caller the currently active state.
		/// </summary>
		virtual int CurrentState( void ) const;

		/// <summary>
		/// Configure a function pointer to be used as the yield callback activity, by default
		/// the function is a no-op.
		/// </summary>
		virtual void SetYieldFunction( void (*Yield_Callback)(void) );

	protected:
		/// <summary>
		/// This is a helper method for the constructor to setup the variables.
		/// </summary>
		/// <param name="ns">Number of states that will be added</param>
		void Init( int ns );

		/// <summary>
		/// Process the OnEnter action for the current state.
		/// </summary>
		/// <param name="nState"></param>
		virtual void OnEnter( int nState );

		/// <summary>
		/// Process the OnLeave action for the specified state.
		/// </summary>
		/// <param name="nState"></param>
		virtual void OnLeave( int nState );

		/// <summary>
		/// Process the OnIn action for the specified state.
		/// </summary>
		/// <param name="nState"></param>
		virtual void OnIn( int nState );

	private:
		/// <summary>
		/// The state machine engine.
		/// </summary>
		scStateMachine			_StateMachine;

		/// <summary>
		/// This is the vector of pointers that are objects for each state. A state is permitted to have a NULL object in
		/// which no actions will occur.
		/// </summary>
		vector<scFSMState*>		_StateActions;

		/// <summary>
		/// A list to manage task context switching as the state machine is advanced.
		/// </summary>
		scIQueue*				_pQueue;

		void (*fsmYield_Callback)( void );
	};

}	// namespace
#endif // !defined(__FINITE_STATE_MACHINE_H_)
