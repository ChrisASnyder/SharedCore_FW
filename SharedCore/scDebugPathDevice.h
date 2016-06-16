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
//          UNCLASSIFIED
//==============================================================================
//
// File Name:		scDebugPathDevice.h
// Created By:		Christopher Snyder
// Creation Date:	28-Jun-2015 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCDEBUGPATHDEVICE_H__INCLUDED_)
#define __SCDEBUGPATHDEVICE_H__INCLUDED_

#include "scDebugPath.h"
#include "HAL\scBufferedIODriver.h"

namespace SharedCore
{
	/// <summary>
	/// This class provides the implementation to send debug data to a device configured in the system. In most cases
	/// this device is UART, but it could be some other streaming component.
	/// </summary>
	class scDebugPathDevice : public SharedCore::scDebugPath
	{
	public:

		scDebugPathDevice( uint8_t nPathID, HAL::scBufferIODriver* pPipe );

		virtual ~scDebugPathDevice();

		void Initialize( HAL::scBufferIODriver* pPipe );

		/// <summary>
		/// This method is the command that will take the string provided and route to the
		/// method in which it will get displayed, stored or routed. This method must be
		/// overridden by the child class to provide the required implementation.
		/// </summary>
		virtual void Capture( const uint8_t* pData, uint32_t nLength );

	private:
		HAL::scBufferIODriver*				_pPipe;

	};
}

#endif // !defined(__SCDEBUGPATHDEVICE_H__INCLUDED_)
