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
// File Name:		scScopeLock.h
// Created By:		Christopher Snyder
// Creation Date:	16-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include "scScopeLock.h"

using SharedCore::scScopeLock;
using SharedCore::scIMutex;

/// <summary>
/// Construct the lock control object.
/// </summary>
/// <param name="lockControl">the protection object.</param>
scScopeLock::scScopeLock( scIMutex* lockControl )
	: _pMutex(lockControl)
{
	if ( _pMutex != NULL )
	{
		_pMutex->Acquire();
	}
}

/// <summary>
/// Release the mutex
/// </summary>
scScopeLock::~scScopeLock()
{
	if ( _pMutex != NULL )
	{
		_pMutex->Release();
		_pMutex = NULL;
	}
}