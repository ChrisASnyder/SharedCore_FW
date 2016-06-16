//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//          © Copyright Common Ground Software Solutions 2015
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
// File Name:		scIModule.h
// Created By:		Christopher Snyder
// Creation Date:	03-Jun-2015 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include "scIModule.h"
#include "scErrorCodes.h"

using SharedCore::scIModule;

/// <summary>
/// /// Class constructor for scIModule
/// </summary>
scIModule::scIModule()
	: _nID(0)
{

}

/// <summary>
/// Class Destructor for scIModule
/// </summary>
scIModule::~scIModule()
{
}

/// <summary>
/// Optionally overridden by the subclassed objects to handle to command notification.
/// The default implementation simply returns ERROR_SUCCESS
/// </summary>
/// <param name="nCommand">Command to execute.</param>
/// <param name="pParameter">Optional parameter for the command being provided.</param>
int scIModule::Notify(uint32_t nCommand, void* pParameter)
{
	return ERROR_SUCCESS;
}

/// <summary>
/// Return the ID of this module
/// </summary>
uint16_t scIModule::GetID(void) const
{
	return _nID;
}

/// <summary>
/// This call is restricted to the module manager and is typically a one time call to setup the ID of the module.
/// 
/// </summary>
/// <param name="nID">The new ID for the object.</param>
void scIModule::SetID(uint16_t nID)
{
	_nID = nID;
}