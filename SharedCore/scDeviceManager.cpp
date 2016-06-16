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
// File Name:		scDeviceManager.h
// Created By:		Christopher Snyder
// Creation Date:	14-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================

#include <stddef.h>
#include "scDeviceManager.h"

using SharedCore::scDeviceManager;
using SharedCore::scDeviceDescriptor;
using SharedCore::scDeviceGeneric;
using SharedCore::scConfigureDevice;

#ifndef assert_param
#define assert_param(X)	if ( !(X) ) { for(;;) ; }
#endif


/// <summary>
/// The class destructor 
/// </summary>
scDeviceManager::~scDeviceManager()
{
	for( size_t i=0; i < _devices.size(); ++i )
	{
		delete _devices[i];
		_devices[i] = NULL;
	}
}

/// <summary>
/// The constructor is protected since this class is never to be used alone. The
/// children will create and manage the drivers.
/// </summary>
scDeviceManager::scDeviceManager(void)
	: _devices(0)
{
}

/// <summary>
/// This method is called only once during the power up sequence of the system.
/// When called all drivers stored in the driver list will be initialized in the
/// proper order.
/// </summary>
void scDeviceManager::Initialize(void)
{
	vector<scDeviceGeneric*>::iterator itr = _devices.begin();
	for( ; itr != _devices.end(); ++itr )
	{
		assert_param( (*itr) != NULL );
		if ( (*itr)->SetupState() == scDeviceGeneric::Uninitialized )
		{
			DeviceInitialize( (*itr)->Guid(), 0 );
		}
	}
}

/// <summary>
/// This method will call the configure method for the specified driver. This is a
/// recursive method that will iterate through all the drivers calling their
/// initialize method. If a driver has dependencies on other drivers, then the
/// dependent drivers will get Initialized first.
/// </summary>
/// <param name="dd">The device to be reset.</param>
/// <param name="bConfigured">This is a flag to indicate that the specified driver
/// has already been configured.</param>
/// <param name="nCallDepth">Used to cap the call depth of this recursive function.
/// </param>
void scDeviceManager::DeviceInitialize( const scDeviceDescriptor& dd, uint8_t nCallDepth )
{
	// This is to prevent the recursive call from running away. This would happen when there is an unresolved
	// dependency or possibly a circular dependency
	assert_param( nCallDepth <= 10 );

	if ( nCallDepth <= 10 )
	{
		scDeviceGeneric* pToConfigure = NULL;
		if ( GetDevice( dd, &pToConfigure ) )
		{
			if ( pToConfigure->HaveDependants() )
			{
				vector<scDeviceDescriptor>::const_iterator itr = pToConfigure->Dependants().begin();
				for( ; itr != pToConfigure->Dependants().end(); ++itr )
				{
					if ( *itr != dd )	// ignore self which shouldn't be there.
					{
						scDeviceGeneric* pGoFirst = NULL;
						if ( GetDevice( *itr, &pGoFirst ) )
						{
							if ( pGoFirst->SetupState() == scDeviceGeneric::Uninitialized )
							{
								// The recursive call
								DeviceInitialize( *itr, nCallDepth+1 );
							}
							else
							{
								// Nothing to do since it is already initialized
							}
						}
						else
						{
							// Didn't find a dependant driver. This could be bad. So lets assert
							assert_param( pGoFirst != NULL );
						}
					}
				}
			}
			if ( pToConfigure->SetupState() == scDeviceGeneric::Uninitialized )
			{
				pToConfigure->Initialize( this );
			}
			else
			{
				// already configured don't need to initialize
			}
		}
		else
		{
			// did not find the driver to initialize. This is bad
			assert_param( pToConfigure != NULL );
		}
	}

}

/// <summary>
/// This method will add a new device to the device list. This is usually called by
/// the device manage, but may also be called from the Exec or a factory that is
/// creating all the initial drivers.
/// </summary>
/// <param name="pDevice">Pointer to the new driver. Once this pointer is passed
/// into the manager the manager will own and delete it. In general, this class
/// never terminates, however, for test reasons, it will perform deletion on all
/// drivers in the device list to prevent memory leaks in test applications.</param>
size_t scDeviceManager::Add( scDeviceGeneric* pDevice )
{
	scDeviceGeneric*	pExistDriver = NULL;

	assert_param( pDevice != NULL );
	if ( !GetDevice( pDevice->Guid(), &pExistDriver ) )
	{
		// Driver doesn't exist. Good to add
		_devices.push_back( pDevice );
	}
	else
	{
		// Driver already exists in the driver list
		assert_param( pExistDriver == NULL );
	}
	return _devices.size();
}

/// <summary>
/// This method will find the specified driver in the list and return it. False is
/// return if the driver is not found.
/// </summary>
/// <param name="dd">The driver to be located</param>
/// <param name="pDevice">Pointer to the driver. This is set when the device is
/// found, otherwise it is NULL.</param>
bool scDeviceManager::GetDevice( const scDeviceDescriptor& dd, scDeviceGeneric** pDevice )
{
	bool bResult = false;
	
	vector<scDeviceGeneric*>::iterator itr = _devices.begin();
	for( ; itr != _devices.end(); ++itr )
	{
		if ( (*itr) != NULL && dd == (*itr)->Guid() )
		{
			*pDevice = *itr;
			bResult = true;
		}
	}

	return bResult;
}

/// <summary>
/// This method will perform a reset on the specified driver.
/// </summary>
/// <param name="dd">The device to be reset.</param>
/// <param name="bIncludeDependants">When this is true all dependant devices will also get reset.</param>
void scDeviceManager::Reset( const scDeviceDescriptor& dd, bool bIncludeDependants  )
{
	scDeviceGeneric* pDevice = NULL;
	if ( GetDevice( dd, &pDevice ) )
	{
		assert_param( pDevice != NULL );
		if ( bIncludeDependants )
		{
			vector<scDeviceDescriptor>::const_iterator itrd = pDevice->Dependants().begin();
			for( ; itrd != pDevice->Dependants().end(); ++itrd )
			{
				if ( *itrd != dd )	// ignore self incase of bad dependency list
				{
					Reset( (*itrd), bIncludeDependants );
				}
			}
		}
		pDevice->Reset();
	}
}

/// <summary>
/// This method will call Enable on the specified driver
/// </summary>
/// <param name="dd">The device to locate</param>
void scDeviceManager::Enable( const scDeviceDescriptor& dd, bool bIncludeDependants  )
{
	scDeviceGeneric* pDevice = NULL;
	if ( GetDevice( dd, &pDevice ) )
	{
		assert_param( pDevice != NULL );
		if ( bIncludeDependants )
		{
			vector<scDeviceDescriptor>::const_iterator itrd = pDevice->Dependants().begin();
			for( ; itrd != pDevice->Dependants().end(); ++itrd )
			{
				if ( *itrd != dd )	// ignore self incase of bad dependency list
				{
					Enable( (*itrd), bIncludeDependants );
				}
			}
		}
		if ( pDevice->State() != scEnabled )
		{
			pDevice->Enable();
		}
	}
}

/// <summary>
/// This method will call disable on the specified driver
/// </summary>
/// <param name="dd">The device to locate</param>
void scDeviceManager::Disable( const scDeviceDescriptor& dd, bool bIncludeDependants  )
{
	scDeviceGeneric* pDevice = NULL;
	if ( GetDevice( dd, &pDevice ) )
	{
		assert_param( pDevice != NULL );
		if ( bIncludeDependants )
		{
			vector<scDeviceDescriptor>::const_iterator itrd = pDevice->Dependants().begin();
			for( ; itrd != pDevice->Dependants().end(); ++itrd )
			{
				if ( *itrd != dd )	// ignore self incase of bad dependency list
				{
					Disable( (*itrd), bIncludeDependants );
				}
			}
		}
		if ( pDevice->State() != scDisabled )
		{
			pDevice->Disable();
		}
	}
}

/// <summary>
/// This method will call the configure method for the specified driver.
/// </summary>
/// <param name="dd">The device to be reset.</param>
void scDeviceManager::Configure( const scDeviceDescriptor& dd, const scConfigureDevice& config )
{
	vector<scDeviceGeneric*>::iterator itr = _devices.begin();
	for( ; itr != _devices.end(); ++itr )
	{
		if ( (*itr) != NULL && dd == (*itr)->Guid() )
		{
			(*itr)->Configure(config);
		}
	}
}


/// <summary>
/// This method will call the unconfigure method for the specified driver.
/// </summary>
/// <param name="dd">The device to be reset.</param>
void scDeviceManager::Unconfigure( const scDeviceDescriptor& dd, const scConfigureDevice& config )
{
	vector<scDeviceGeneric*>::iterator itr = _devices.begin();
	for( ; itr != _devices.end(); ++itr )
	{
		if ( (*itr) != NULL && dd == (*itr)->Guid() )
		{
			(*itr)->Unconfigure(config);
		}
	}
}

