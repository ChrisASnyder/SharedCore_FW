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
// File Name:		scDebugPathDevice.h
// Created By:		Christopher Snyder
// Creation Date:	28-Jun-2015 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include "scDebugPathDevice.h"

using namespace SharedCore;
using namespace SharedCore::HAL;

/// <summary>
/// Class Destructor for scDebugPathDevice
/// </summary>
scDebugPathDevice::scDebugPathDevice( uint8_t nPathID, scBufferIODriver* pPipe )
	: scDebugPath( nPathID )
	, _pPipe(pPipe)
{

}

/// <summary>
/// Class constructor for scDebugPathDevice
/// </summary>
scDebugPathDevice::~scDebugPathDevice()
{

}

void scDebugPathDevice::Initialize( scBufferIODriver* pPipe )
{
	assert_param( pPipe != NULL );
	_pPipe = pPipe;
}

/// <summary>
/// This method is the command that will take the string provided and route to the
/// method in which it will get displayed, stored or routed. This method must be
/// overridden by the child class to provide the required implementation.
/// </summary>
void scDebugPathDevice::Capture( const uint8_t* pData, uint32_t nLength )
{
	assert_param( _pPipe != NULL );
	_pPipe->Send_n( pData, nLength );
}
