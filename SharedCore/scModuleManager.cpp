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
// File Name:		scModuleManager.h
// Created By:		Christopher Snyder
// Creation Date:	03-Jun-2015 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include "scModuleManager.h"
#include "scErrorCodes.h"

using namespace SharedCore;

/// <summary>
/// Class constructor for scModuleManager
/// </summary>
scModuleManager::scModuleManager(void)
	: _Modules(0)
{

}

/// <summary>
/// Class Destructor for scModuleManager
/// </summary>
scModuleManager::~scModuleManager()
{

}



/// <summary>
/// This method will add a new singleton module to the list of items and assign the unique id for that module.
/// This is called by the child during initialization.
/// </summary>
/// <param name="pModule">Pointer to the module to be added</param>
/// <param name="nModuleID">The new ID to be assigned to this module.</param>
uint16_t scModuleManager::Add( SharedCore::scIModule* pModule, uint16_t nModuleID )
{
	// Make sure the pointer is valid.
	assert_param( pModule != NULL );
	// first locate the provided ID checking if it already exists
	if ( Find( nModuleID ) != NULL )
	{
		// Node already exists
		// create find a new ID
		nModuleID = GetNextAutoID();
	}

	// make sure the pointer isn't already in the list

	scIModule* pExists = NULL;
	for( size_t i=0; pExists == NULL && i < _Modules.size(); ++i )
	{
		if ( _Modules[i] == pModule )
		{
			pExists = _Modules[i];
		}
	}
	
	if ( pExists == NULL )
	{
		// doesn't exist add the module
		pModule->SetID( nModuleID );
		_Modules.push_back( pModule );
	}
	else
	{
		// exists, just return the ID already assigned.
		nModuleID = pExists->GetID();
	}
	return nModuleID;
}

/// <summary>
/// This method will add a new singleton module to the list of items and assign the unique id for that module.
/// This is called by the child during initialization. This add method is assuming the use of an auto ID.
/// The return value in this instance is the ID assigned to the module
/// </summary>
/// <param name="pModule">Pointer to the module to be added</param>
uint16_t scModuleManager::Add(SharedCore::scIModule* pModule )
{
	return Add( pModule, GetNextAutoID() );
}


/// <summary>
/// Permits the sending of commands to all modules. Returns a result code of 0 on success or an ERROR code on
/// failure. The last error code encountered is returned All modules will be called even if one of them returns
/// an error.
/// </summary>
/// <param name="nCommand">Command ID to send to all modules</param>
/// <param name="pParameter">Optional parameter to the command</param>
int scModuleManager::Notify(uint32_t nCommand, void* pParameter)
{
	int nResult = ERROR_SUCCESS;

	for(size_t i=0; i < _Modules.size(); ++i )
	{
		assert_param( _Modules[i] != NULL );
		int nLocalError = _Modules[i]->Notify( nCommand, pParameter );
		if ( nResult == ERROR_SUCCESS )
		{
			nResult = nLocalError;
		}
	}
	return nResult;
}

/// <summary>
/// Permits the sending of commands to a specific modules by the ID. Returns a result code of 0 on success or an
/// ERROR code on failure. The code is directly from the referenced module.
/// </summary>
/// <param name="nId">ID of the module to receive the command</param>
/// <param name="nCommand">Command to send</param>
/// <param name="pParameter">Optional parameter for the command.</param>
int scModuleManager::Notify(uint16_t nId, uint32_t nCommand, void* pParameter )
{
	int nResult = ERROR_SC_MODULE_NOT_FOUND;

	scIModule* pModule = Find( nId );
	if ( pModule != NULL )
	{
		nResult = pModule->Notify( nCommand, pParameter );
	}
	return nResult;
}

/// <summary>
/// This method will search the known list of modules and find the last ID used and return a value of +1 to that
/// value. Note that 0 is never a valid ID for module with the assumption that the Module manager is always ID 0.
/// </summary>
uint16_t scModuleManager::GetNextAutoID()
{
	uint16_t nLargest = 0;
	for(size_t i=0; i < _Modules.size(); ++i )
	{
		assert_param( _Modules[i] != NULL );
		if ( _Modules[i]->GetID() > nLargest )
		{
			nLargest = _Modules[i]->GetID();
		}
	}

	return nLargest + 1;
}

scIModule* scModuleManager::Find( uint16_t nId ) const
{
	scIModule* pResult = NULL;

	for(size_t i=0; pResult== NULL && i < _Modules.size(); ++i )
	{
		assert_param( _Modules[i] != NULL );
		if ( _Modules[i]->GetID() == nId )
		{
			pResult = _Modules[i];
		}
	}
	return pResult;
}

