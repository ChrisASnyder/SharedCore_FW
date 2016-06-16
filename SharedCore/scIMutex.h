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



#ifndef __scIMutex_H
#define __scIMutex_H

#include "scTypes.h"

#ifdef _WIN32
#	pragma once
#endif

namespace SharedCore
{
	/// <summary>
	/// This class provides an interface to a basic mutex implementation. Specific implementations will then
	/// be constructed. This is a pure virtual class.
	/// </summary>
	class scIMutex
	{
	public:

		/// <summary>
		///	The interface version of this implementation doesn't do anything. It is a placeholder
		/// for the derived classes.
		/// </summary>
		scIMutex( void ) {}

		/// <summary>
		/// Virtual destructor
		/// </summary>
		virtual ~scIMutex() {}

		/// <summary>
		/// This method will attempt to acquire the mutex using the timeout value. This is primarily
		/// dependent on the specific mutex implementation since some platforms do not support
		/// a timeout wait on mutex acquisition.
		/// </summary>
		virtual int TryAcquire( uint32_t timeout ) = 0;

		/// <summary>
		/// This will acquire the mutex and prevent other implementations from using it.
		/// </summary>
		virtual void Acquire( void ) = 0;

		/// <summary>
		/// This will release the mutex. Other tasks waiting will get released.
		/// </summary>
		virtual void Release(void) = 0;

		/// <summary>
		/// For the FreeRTOS there is a difference between a non ISR and and ISR acquisition
		/// of a mutex.
		/// </summary>
		virtual void ReleaseISR(void) = 0;
	};

}	// Namespace SharedCore
#endif // __scIMutex_H