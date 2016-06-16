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
// File Name:      scStateMachine.cpp
// Created By:     Christopher A. Snyder
// Creation Date:  01/25/1998
// $Id: scStateMachine.cpp $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================

#include <stdarg.h>

#include "scStateMachine.h"
#ifdef SUPPORT_STREAMIO 
#include <fstream>
#include <iostream>
#endif

using namespace SharedCore;
//lint --e{1566}    // constructor init is getting confused.
//lint -lib

#ifdef _WIN32
#   ifdef _DEBUG
#       ifdef DEBUG_NEW
#           define new DEBUG_NEW
#       endif
#   endif
#   include "assert.h"
#endif

#ifndef ASSERT
#   ifdef _WIN32
#       define ASSERT(A) assert(A)
#   else
#       define ASSERT(A)
#   endif
#endif

//////////////////////////////////////////////////////////////////////////////
//	Function	:	scStateMachine::setTransitionMatrix
//
//	Arguments	:	int ni	
//					int ns	
//					...		
//	Purpose		:	Initializes the scStateMachine matrix based on the input parameters
//	Returns		:	scStateMachine &
//----------------------------------------------------------------------------
//	Author		:	Christopher A. Snyder
//	Date		:	10-28-95
//----------------------------------------------------------------------------
scStateMachine & scStateMachine::setTransitionMatrix(int ni, int ns, ...)
{
	int i, j;
	int state;
	va_list ap;

	numInputs = ni;
	numStates = ns;
	setSize(ni, ns);
	va_start(ap, ns);
	for (i=0; i < int(ns); i++)
	{
		for(j=0; j < int(ni); j++) 
		{
			state = va_arg(ap,int);
			ASSERT( state < numStates);
			transMatrix[i*numInputs+j] = state;
		}
	}
	return *this;
}

/*******************************************************************************
//	Function	:	scStateMachine::scStateMachine
//
//	Purpose		:	Configures the transition matrix from a matrix pointer
//	Arguments	:	int ni	
//					int ns	
//					int is	
//					const int* pMatrix	
//	Returns		:		
//------------------------------------------------------------------------------
//	Date		:	06/08/05
/+******************************************************************************/
scStateMachine::scStateMachine(int ni, int ns, int is, const int* pMatrix )
	: numInputs(ni)
	, numStates(ns)
	, currentState(is)
	, initialState(is)
	, transMatrix()
#ifdef _DEBUG
	//	, m_nLastState(-1)
	//	, m_nLastInput(-1)
	, m_nHistoryIndex(0)
#endif
{
	int i, j, state;
	setSize(ni, ns);
	for (i=0; i<ns; i++)
	{
		for(j=0; j<ni; j++) 
		{
			state = pMatrix[i*numInputs+j];
			ASSERT( state < numStates);
			transMatrix[i*numInputs+j] = state;
		}
	}
#ifdef _DEBUG
	InitHistory();
#endif
	//lint -sem( scStateMachine::InitHistory, initializer )
	//lint -sem( scStateMachine::setSize, initializer)
}

#ifdef SUPPORT_STREAMIO 
//////////////////////////////////////////////////////////////////////////////
//	Function	:	istream & operator>> (istream & file, scStateMachine & sm )
//
//	Arguments	:	istream & file		
//					scStateMachine & sm	
//	Purpose		:	Reads a state matrix from a file.
//	Returns		:	istream &
//----------------------------------------------------------------------------
//	Author		:	Christopher A. Snyder
//	Date		:	11-06-97
//----------------------------------------------------------------------------

istream & operator>> (istream & file, scStateMachine & sm )
{
	int i, j;
	int state;
	char	str[100];

#ifdef _DEBUG
	sm.InitHistory();
#endif

	file >> str >> sm.numInputs;
	file >> str >> sm.numStates;
	file >> str >> sm.initialState;
	sm.currentState = sm.initialState;
	sm.setSize( sm.numInputs, sm.numStates );
	file >> str;
	for (i=0; i<sm.numStates; i++)
		for(j=0; j<sm.numInputs; j++) {
			file >> state;
			ASSERT( state < sm.numStates);
			sm.transMatrix[i*sm.numInputs+j] = state;
		}
		return file;
}

//////////////////////////////////////////////////////////////////////////////
//	Function	:	ostream & operator<< (ostream & file, const scStateMachine & sm)
//
//	Arguments	:	ostream & file			
//					const scStateMachine & sm	
//	Purpose		:	Writes a state matrix to a file stream
//	Returns		:	ostream &
//----------------------------------------------------------------------------
//	Author		:	Christopher A. Snyder
//	Date		:	11-06-97
//----------------------------------------------------------------------------

ostream & operator<< (ostream & file, const scStateMachine & sm)
{
	int i, j;

	file << "NumInputs   " << int(sm.numInputs) << endl;
	file << "NumState    " << int(sm.numStates) << endl;
	file << "InitalState " << int(sm.initialState) << endl;
	file << "TransitionMatrix" << endl;
	for (i=0; i<sm.numStates; i++) {
		for(j=0; j<sm.numInputs; j++)
			file << "\t" << int(sm.transMatrix[i*sm.numInputs+j]);
		file << endl;
	}
	return file;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//	Function	:	scStateMachine::scStateMachine
//
//	Arguments	:	int ni	
//					int ns	
//					int is	
//	Purpose		:	Constructor that initializes the scStateMachine
//	Returns		:	
//----------------------------------------------------------------------------
//	Author		:	Christopher A. Snyder
//	Date		:	11-06-97
//----------------------------------------------------------------------------

scStateMachine::scStateMachine(int ni, int ns, int is)
	: transMatrix( ni*ns )
#ifdef _DEBUG
	//	, m_nLastState(0)
	//	, m_nLastInput(0)
	, m_nHistoryIndex(0)
#endif
{
#ifdef _DEBUG
	InitHistory();
#endif
	setSize( ni, ns );
	currentState = is;
	initialState = is;
}

//////////////////////////////////////////////////////////////////////////////
//	Function	:	scStateMachine::setSize
//
//	Arguments	:	int ni	
//					int ns	
//	Purpose		:	Sets the size of the matrix 
//	Returns		:	scStateMachine &
//----------------------------------------------------------------------------
//	Author		:	Christopher A. Snyder
//	Date		:	11-06-97
//----------------------------------------------------------------------------

scStateMachine & scStateMachine::setSize(int ni, int ns)
{
	numInputs = ni;
	numStates = ns;
	transMatrix.resize( ni * ns );
	return *this;
}

//////////////////////////////////////////////////////////////////////////////
//	Function	:	scStateMachine::setSingleTransition
//
//	Arguments	:	int ni		
//					int ns		
//					int state	
//	Purpose		:	Sets the state of a single transition matrix element
//	Returns		:	scStateMachine &
//----------------------------------------------------------------------------
//	Author		:	Christopher A. Snyder
//	Date		:	11-06-97
//----------------------------------------------------------------------------

scStateMachine & scStateMachine::setSingleTransition(int ni, int ns, int state)
{
	ASSERT( ni < numInputs && ns < numStates && state < numStates );
	//	ASSERT( ni <= 0 && ns <= 0 && state <= 0 );
	transMatrix[ns * numInputs + ni] = state;
	return *this;
}

//////////////////////////////////////////////////////////////////////////////
//	Function	:	scStateMachine::operator= 
//
//	Arguments	:	const scStateMachine & sm	
//	Purpose		:	sets this equal to the scStateMachine sm
//	Returns		:	scStateMachine &
//----------------------------------------------------------------------------
//	Author		:	Christopher A. Snyder
//	Date		:	11-06-97
//----------------------------------------------------------------------------

scStateMachine & scStateMachine::operator= (const scStateMachine & sm)
{
	if( &sm != this )
	{
		numInputs = sm.numInputs;
		numStates = sm.numStates;
		currentState = sm.currentState;
		initialState = sm.initialState;
		transMatrix = sm.transMatrix;
#ifdef _DEBUG
		for ( m_nHistoryIndex = 0; m_nHistoryIndex < STATE_MACHINE_DEBUG_HISTORY
			&& m_nHistoryIndex < sm.m_nHistoryIndex; ++m_nHistoryIndex )
		{
			m_nLastState[m_nHistoryIndex] = sm.m_nLastState[m_nHistoryIndex];
			m_nLastInput[m_nHistoryIndex] = sm.m_nLastInput[m_nHistoryIndex];
		}
#endif
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////////////
//	Function	:	scStateMachine::advance
//
//	Arguments	:	int input	
//	Purpose		:	checks for valid input and moves to the next state based on the
//					transition matrix
//	Returns		:	int
//----------------------------------------------------------------------------
//	Author		:	Christopher A. Snyder
//	Date		:	11-06-97
//----------------------------------------------------------------------------

int scStateMachine::advance(int input)
{
#ifdef _DEBUG
	if ( m_nHistoryIndex < STATE_MACHINE_DEBUG_HISTORY )
	{
		m_nLastState[m_nHistoryIndex] = currentState;
		m_nLastInput[m_nHistoryIndex] = input;
	}
	if ( ++m_nHistoryIndex >= STATE_MACHINE_DEBUG_HISTORY )
	{
		m_nHistoryIndex = 0;
	}
#endif
	ASSERT( input < numInputs );
	return currentState = transMatrix[ currentState * numInputs + input ];
}

#ifdef SUPPORT_STREAMIO 
//////////////////////////////////////////////////////////////////////////////
//	Function	:	scStateMachine::scStateMachine
//
//	Arguments	:	char* file	
//	Purpose		:	Constructor. Opens a file that contains the initialization
//					data for the parser. See header for format.
//	Returns		:	
//----------------------------------------------------------------------------
//	Author		:	Christopher A. Snyder
//	Date		:	11-06-97
//----------------------------------------------------------------------------

scStateMachine::scStateMachine( const char* file)
	: numInputs(0)
	, numStates(0)
	, currentState(0)
	, initialState(0)
	, transMatrix()
#ifdef _DEBUG
	//	, m_nLastState(-1)
	//	, m_nLastInput(-1)
	, m_nHistoryIndex(0)
#endif
{
	ifstream	lpfile( file );
	if ( lpfile.bad() )
		cout << "Could not open the file " << file << " GenericParser Not Initialized." << endl;
	else
		lpfile >> *this;
#ifdef _DEBUG
	InitHistory();
#endif
}
#endif

#ifdef _DEBUG 
void scStateMachine::InitHistory(void)
{
	m_nHistoryIndex = 0;
	memset( m_nLastState, 0, STATE_MACHINE_DEBUG_HISTORY * sizeof(int) );
	memset( m_nLastInput, 0, STATE_MACHINE_DEBUG_HISTORY * sizeof(int) );
}
#endif
