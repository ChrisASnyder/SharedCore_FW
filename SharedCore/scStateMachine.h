//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          chris.snyder@commongroundss.com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  any later version.
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
// File Name:      scStateMachine.h
// Created By:     Christopher A. Snyder
// Creation Date:  01/25/1998
// $Id: scStateMachine.h $
//
//==============================================================================

#ifndef __STATEMACHINE_H
#define __STATEMACHINE_H

#include "scTypes.h"

#	pragma once

	//#define SUPPORT_STREAMIO

#ifdef SUPPORT_STREAMIO 
	#include <iostream>
#endif

#include <vector>

using namespace std;

#define STATE_MACHINE_DEBUG_HISTORY	10

namespace SharedCore
{
	class scStateMachine
	{
#ifdef SUPPORT_STREAMIO 
		friend ostream & operator<< (ostream &, const scStateMachine &);
		friend istream & operator>> (istream &, scStateMachine &);
#endif
	public:
		scStateMachine(int ni=0, int ns=0, int is=0);
		scStateMachine(int ni, int ns, int is, const int* pMatrix );
#ifdef SUPPORT_STREAMIO 
		scStateMachine(const char* file);
#endif

		scStateMachine(const scStateMachine & sm)
		{ *this = sm; }
		scStateMachine & setSize(int ni, int ns);
		scStateMachine & setInitialState(int s)
		{ initialState = s; return *this; }
		int getNumInputs(void) const
		{ return numInputs; }
		int getNumStates(void) const
		{ return numStates; }
		int getInitialState(void) const
		{ return initialState; }
		scStateMachine & setSingleTransition(int, int, int);
		scStateMachine & setTransitionMatrix(int, int, ...);
		scStateMachine & operator= (const scStateMachine &);
		int operator() (void) const
		{ return currentState; }
		int CurrentState (void) const
		{ return currentState; }
		scStateMachine & init(void)
		{ currentState = initialState; return *this; }
		int advance(int);

	protected:
		int              numInputs;
		int              numStates;
		int              currentState;
		int              initialState;
		vector<int>      transMatrix;

#ifdef _DEBUG
		void InitHistory(void);
		int              m_nLastState[STATE_MACHINE_DEBUG_HISTORY];
		int              m_nLastInput[STATE_MACHINE_DEBUG_HISTORY];
		int                 m_nHistoryIndex;
#endif

	};
}	// Namespace SharedCore

#endif

