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

// File Name:       scFSMState.h
// Created By:      Christopher A. Snyder
// Creation Date:   6/1/2014
// $Id: scFSMState.h $
//
//==============================================================================

#if !defined(__SCFSMSTATE_H)
#define __SCFSMSTATE_H

#ifdef _WIN32
#	pragma once
#endif

#include <stddef.h>
#include "scTypes.h"

namespace SharedCore
{

#ifndef assert_param
#define assert_param(X)
#endif

	/// <summary>
	/// This class is the base class interface used by the FiniteStateMachine to
	/// perform the state actions. Users will create derived objects from this class
	/// and implement the activates that will be performed as a state transitions.
	///
	/// The general principal of a FSM is to create enter and exit actions that and an
	/// action to perform while in the state. Most FSM will receive the input transactions
	/// from an external source. However, for auto advancing states they OnInput call
	/// should occur while processing the OnIn method call.
	/// </summary>
	class scFSMState
	{
	public:
		/// <summary>
		/// This method will construct the state object. The state ID is required and
		/// cannot be changed. That value is used by the FiniteStateMahcine to manage
		/// the states. The context pointer is provided to aid the transaction objects
		/// and is optional.
		/// </summary>
		scFSMState( size_t nStateID, void* pContext = NULL )
			: _pContext(pContext)
			, _nStateID(nStateID)
		{
		}

		/// <summary>
		/// Simple class destructor
		/// </summary>
		virtual ~scFSMState() {}

		/// <summary>
		/// Optional:
		/// These actions are performed every time a state is entered.
		/// any setup conditions.
		/// No actions are should be performed to change the current
		/// state or generate new input actions while in this method
		/// </summary>
		virtual void OnEnter(void) {};

		/// <summary>
		/// Optional:
		/// These actions are performed every time a state is exited.
		/// These represent any cleanup for this state
		/// No actions are should be performed to change the current
		/// state or generate new input actions while in this method
		/// </summary>
		virtual void OnLeave(void) {};

		/// <summary>
		/// Optional:
		/// This action is performed while in the state. This will be called
		/// following the OnEnter command and if any input transaction keeps
		/// the state machine in this state. If a action is taken that requires
		/// the state machine to advance, the new input action is returned.
		/// </summary>
		virtual void OnIn(void) {};

		/// <summary>
		/// This method will return the state ID for this object.
		/// </summary>
		inline size_t State(void) const { return _nStateID; }

		/// <summary>
		/// This is used to change the context pointer. The usage is up to the
		/// user of the state machine. It is provided to aid in the Transaction
		/// implementations.
		/// </summary>
		void Context( void* pContext )
		{
			_pContext = pContext;
		}

		/// <summary>
		/// This is the largest delay possible for the queue receiving state inputs
		/// This value is in milliseconds.
		/// </summary>
		static const uint32_t MaxTimeDelay = 0xFFFFFFFF;

	protected:
		/// <summary>
		/// Pointer to the context
		/// </summary>
		void* _pContext;

	private:
		/// <summary>
		/// The state identification for the object.
		/// </summary>
		const size_t _nStateID;

	};

	/// <summary>
	/// This template class is used to provide a context pointer that the transaction
	/// methods may use to access some master object, such as a manager class.  The
	/// template version allows the type to be specified and will provide a casting
	/// Context() method for the actions.
	/// </summary>
	template <class T>
	class scFSMStateContext : public scFSMState
	{
	public:
		/// <summary>
		/// Construct the state object and provide a context parameter.
		/// </summary>
		scFSMStateContext( size_t nStateID, T* pContext = NULL )
			: scFSMState( nStateID, (void*)pContext )
		{
		}

		/// <summary>
		/// Simple class destructor
		/// </summary>
		virtual ~scFSMStateContext() {}

		/// <summary>
		/// Method to cast the context pointer from the void* type to something
		/// more useful to the state action handlers. Usage is determined by the user.
		/// </summary>
		inline T* Context(void)
		{
			assert_param( _pContext != NULL );
			return reinterpret_cast<T*>(_pContext);
		}
	};
}	// Namespace SharedCore

#endif // !defined(__SCFSMSTATE_H)

