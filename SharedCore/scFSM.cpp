//==============================================================================
//          UNCLASSIFIED
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
// File Name:       scFSM.cpp
// Created By:      Christopher A. Snyder
// Creation Date:   6/1/2014
// $Id: scFSM.cpp $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================

#include "scFSM.h"

using namespace SharedCore;

#ifndef LONG_TIME
#define LONG_TIME ((uint16_t)0xFFFF)
#endif

#define MAX_UINT16    ((uint16_t)0xFFFF)

#ifndef MIN
	// Simple math macro used to select the smallest of 2 values.
#define  MIN(a, b)      (((a) < (b)) ? (a) : (b))
#endif


/// <summary>
/// On this callback can be used to allow the task to let other activities progress
/// while this action has been performed.
/// </summary>
static void TaskYield_NoOp(void);


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
scFSM::scFSM(int ni, int ns, int is, const int* pMatrix, scIQueue* pQueue )
	: _StateMachine( ni, ns, is, pMatrix )
	, _StateActions(ns)
	, _pQueue(pQueue)
	, fsmYield_Callback(NULL)
{
	Init( ns );
	fsmYield_Callback = TaskYield_NoOp;
}

/// <summary>
/// This is a helper method for the constructor to setup the variables.
/// </summary>
/// <param name="ns">Number of states to be in the state machine</param>
void scFSM::Init( int ns )
{
	_StateActions.resize(ns);
	for( int i=0; i < ns; ++i )
	{
		_StateActions[i] = NULL;
	}
}


/// <summary>
/// The destructor
/// </summary>
scFSM::~scFSM()
{
	// Should memory cleanup of the queue happen here or by the creator?
	_pQueue = NULL;
}

/// <summary>
/// This will assign a state object to handle the Enter, In, and Leaving actions. If a pointer already exists for
/// the specified state it will be deleted before the next one is added.
/// </summary>
/// <param name="pAction">An action object pointer.</param>
void scFSM::Set( scFSMState* pAction )
{
	assert_param( pAction != NULL );

	_StateActions[pAction->State()] = pAction;
}

/// <summary>
/// This will clear and delete the object for the specified state.
/// </summary>
/// <param name="nState"></param>
/// <param name="bDeleteMemory">when true the memory is deleted and the return is NULL</param>
scFSMState* scFSM::Clear( int nState, bool bDeleteMemory  )
{
	scFSMState* pAction = NULL;
	for( size_t i=0; i < _StateActions.size(); ++i )
	{
		if ( _StateActions[i] != NULL
			&& _StateActions[i]->State() == nState )
		{
			pAction = _StateActions[i];
			_StateActions[i] = NULL;
		}
	}
	if ( bDeleteMemory )
	{
		delete pAction;
		pAction = NULL;
	}
	return pAction;
}

/// <summary>
/// This method will handle the asynchronous inputs that are used to advance the state machine. Every state
/// transition will cause the Enter, In and leave actions to occur before anther input will be processed. All
/// inputs are processed in the order in which they are received.
/// </summary>
/// <param name="nInput"></param>
void scFSM::OnInput( int nInput )
{
	assert_param( _pQueue != NULL );

	if ( !_pQueue->SendToBack( &nInput, LONG_TIME ) )
	{
		// the queue is full and we cannot add any more events
		// this is probably a bug in the state transition logic.
		assert_param( false );
	}
}

/// <summary>
/// This method will handle the asynchronous inputs that are used to advance the state machine. Every state
/// transition will cause the Enter, In and leave actions to occur before anther input will be processed. All
/// inputs are processed in the order in which they are received.
/// </summary>
/// <param name="nInput"></param>
void scFSM::OnInputISR( int nInput )
{
	assert_param( _pQueue != NULL );

	if ( !_pQueue->SendToBackISR( &nInput ) )
	{
		// the queue is full and we cannot add any more events
		// this is probably a bug in the state transition logic.
		assert_param( false );
	}
}

/// <summary>
/// This is a blocking call that will wait for a input to receive or timeout. If an input is received then the
/// transition actions for the states involved will be called and processed on this task before this method
/// returns.
/// </summary>
/// <param name="nTimeout">a new timeout value.</param>
bool scFSM::HandleInput( uint32_t nTimeout )
{
	bool bResult = false;
	int nInput;
	uint32_t nTimeRemaining = nTimeout;
	uint16_t nMaxWait = MAX_UINT16;
	uint16_t nDelay = 0;

	assert_param( _pQueue != NULL );

	do
	{
		nDelay = MIN( nMaxWait, nTimeRemaining );
		if ( _pQueue->Receive( &nInput, nDelay ) )
		{
			int nEnterState = _StateMachine();
			int current = _StateMachine.advance(nInput);

			if ( current != nEnterState )
			{
				OnLeave( nEnterState );
				OnEnter( current );
			}
			OnIn( current );
			assert_param(fsmYield_Callback != NULL );
			fsmYield_Callback();
			bResult = true;
			// because we just triggered  we do not want to wait
			// but we do want to immediately process any inputs that
			// auto advanced the state machine. However, before any
			// delay happens the current queue will be checked again.
			// then we exit. The caller can the readjust the timeout
			// for the delay for the current condition.
			nMaxWait = 0;
		}
		else if ( !bResult )
		{
			// Time expired in the queue, is there more time to wait
			nTimeRemaining-= nDelay;
		}
		else
		{
			// force the exit of the loop if nothing was in the queue
			// after checking it once already.
			nTimeRemaining = 0;
		}
	} while( nTimeRemaining > 0 );

	return bResult;
}

/// <summary>
/// Returns to the caller the currently active state.
/// </summary>
int scFSM::CurrentState( void ) const
{
	return _StateMachine.CurrentState();
}

/// <summary>
/// Process the OnEnter action for the current state.
/// </summary>
/// <param name="nState"></param>
void scFSM::OnEnter( int nState )
{
	if ( _StateActions[nState] != NULL )
	{
		_StateActions[nState]->OnEnter();
	}
}

/// <summary>
/// Process the OnLeave action for the specified state.
/// </summary>
/// <param name="nState"></param>
void scFSM::OnLeave( int nState )
{
	if ( _StateActions[nState] != NULL )
	{
		_StateActions[nState]->OnLeave();
	}
}

/// <summary>
/// Process the OnIn action for the specified state.
/// </summary>
/// <param name="nState"></param>
void scFSM::OnIn( int nState )
{
	if ( _StateActions[nState] != NULL )
	{
		_StateActions[nState]->OnIn();
	}
}

/// <summary>
/// Configure a function pointer to be used as the yield callback activity, by default
/// the function is a no-op.
/// </summary>
void scFSM::SetYieldFunction( void (*Yield_Callback)(void) )
{
	fsmYield_Callback = Yield_Callback;
}

/// <summary>
/// On this callback can be used to allow the task to let other activities progress
/// while this action has been performed.
/// </summary>
void TaskYield_NoOp(void)
{
}
