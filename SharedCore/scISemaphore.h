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



#ifndef __scISemaphore_H
#define __scISemaphore_H

#ifdef _WIN32
#	pragma once
#endif

namespace SharedCore
{
	/// <summary>
	/// This class provides an interface to a basic semaphore implementation. Specific implementations will then
	/// be constructed based on the core library in use.
	/// </summary>
	class scISemaphore
	{
	public:

		/// <summary>
		///	The interface version of this implementation doesn't do anything. It is a placeholder
		/// for the derived classes.
		/// </summary>
		scISemaphore( void ) {}

		/// <summary>
		/// Virtual destructor
		/// </summary>
		virtual ~scISemaphore() {}

		/// <summary>
		/// This method will wait indefinitely until the semaphore is released.
		/// The return is true when the semaphore is acquired. False if it is released
		/// due to an error of some sort.
		/// </summary>
		virtual bool Wait( void ) = 0;

		/// <summary>
		/// Try to acquire the semaphore within the timeout. The value is usually milliseconds
		/// The return is True if acquired, false if not acquired.
		/// </summary>
		virtual bool Wait( uint32_t timeout ) = 0;

		/// <summary>
		/// This will release the semaphore causing any waiting tasks to be released.
		/// </summary>
		virtual void Release(void) = 0;

		/// <summary>
		/// For FreeRTOS, this is the call needed when inside an ISR routine.
		/// </summary>
		virtual void ReleaseISR(void) = 0;
	};

}	// Namespace SharedCore
#endif // __scISemaphore_H