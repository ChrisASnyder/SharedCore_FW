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
// File Name:		scDebugPath.h
// Created By:		Christopher Snyder
// Creation Date:	14-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include "scDebugPath.h"

using SharedCore::scDebugPath;
using SharedCore::scEnableState_t;

/// <summary>
/// Simple destructor
/// </summary>
scDebugPath::~scDebugPath()
{
}

/// <summary>
/// The constructor of the path. By default only the path id is assigned.
/// </summary>
/// <param name="nPathID">The path Id</param>
scDebugPath::scDebugPath( uint8_t nPathID )
	: _nPathId( nPathID )
	, _nState(scEnabled)
{
}

/// <summary>
/// This method will change the flag that permits the use of this debug path.
/// </summary>
void scDebugPath::Enable( void )
{
	_nState = scEnabled;
}

/// <summary>
/// Disable this path.
/// </summary>
void scDebugPath::Disable()
{
	_nState = scDisabled;
}

/// <summary>
/// Set the state of the Enable flag.
/// </summary>
void scDebugPath::State(scEnableState_t nState )
{
	_nState = nState;
}

/// <summary>
/// Access to the path ID for this object.
/// </summary>
uint8_t scDebugPath::PathId(void) const
{
	return _nPathId;
}

/// <summary>
/// Get the state of the Enable flag.
/// </summary>
scEnableState_t scDebugPath::State(void) const 
{
	return _nState;
}
