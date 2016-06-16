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
// File Name:		scDebugLabelManager.h
// Created By:		Christopher Snyder
// Creation Date:	14-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include "scDebugLabelManager.h"

using namespace SharedCore;
using namespace std;

/// <summary>
/// Simple Destructor
/// </summary>
scDebugLabelManager::~scDebugLabelManager()
{
}

/// <summary>
/// Default Constructor
/// </summary>
/// <param name="nMaxLabels">The number of labels that will exist in this system.
/// </param>
scDebugLabelManager::scDebugLabelManager( uint16_t nMaxLabels, scEnableState_t nDefault )
	: _nLabelState(nMaxLabels)
{
	LabelState( nDefault );
}

/// <summary>
/// Get the current state of the label.
/// </summary>
/// <param name="nLabel">The label id to lookup.</param>
scEnableState_t scDebugLabelManager::LabelState(uint16_t nLabel) const 
{
	scEnableState_t nResult = scDisabled;
	if ( nLabel < _nLabelState.size() )
	{
		nResult = _nLabelState[nLabel];
	}
	return nResult;
}

/// <summary>
/// Get the current state of the label.
/// </summary>
/// <param name="nLabel">The label ID to set</param>
/// <param name="nState">The new state of the label.</param>
void scDebugLabelManager::LabelState( uint16_t nLabel, scEnableState_t nState) 
{
	if ( nLabel < _nLabelState.size() )
	{
		_nLabelState[nLabel] = nState;
	}
}

/// <summary>
/// Changes the state for all the labels to match the provided state.
/// </summary>
/// <param name="nState">The new state of the label.</param>
void scDebugLabelManager::LabelState( scEnableState_t nState )
{
	vector<scEnableState_t>::iterator itr = _nLabelState.begin();
	for( ; itr != _nLabelState.end(); ++itr )
	{
		(*itr) = nState;
	}
}

/// <summary>
/// Get a string description of the specified label.
/// </summary>
/// <param name="nLabel">The label to look up</param>
const char* scDebugLabelManager::LabelText(uint16_t nLabel)
{
	/* a typical implementation would be a simple switch statement that return
	static strings */
	const char* pLabel;
	switch( nLabel )
	{
	case scDEBUGLABEL_ERROR_MESSAGE:
		pLabel = "scError";
		break;
	case scDEBUGLABEL_WARNING_MESSAGE:
		pLabel = "scWarning";
		break;
	case scDEBUGLABEL_INFO_MESSAGE:
		pLabel = "scInfo";
		break;
	case scDEBUGLABEL_DEBUG_MESSAGE:
		pLabel = "scDebug";
		break;
	default:
		pLabel = "scError";
		break;
	}	
	return pLabel;
}

