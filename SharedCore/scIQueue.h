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



#ifndef __scIQueue_H
#define __scIQueue_H

#include "scTypes.h"

#ifdef _WIN32
#	pragma once
#endif
namespace SharedCore
{

	/// <summary>
	/// This class provides a generic wrapper for the Queue functions used by a OS. Specifically this is a
	/// wrapper for the FreeRTOS queue construct and will prevent the base objects from needing the FreeRTOS
	/// or could be removed from FreeRTOS for another platform.
	/// </summary>
	class scIQueue
	{
	public:

		/// <summary>
		/// This will construct the empty queue. If this constructor is used then the create
		/// method must be called before the object can be used. In general most implementations
		/// will use the parameteratized constructor.
		/// </summary>
		scIQueue( void ) {}

		/// <summary>
		/// Construct the queue with specifying the queue size and the size of the items being
		/// stored in the queue.
		/// </summary>
		scIQueue( uint32_t nQueueLength, uint32_t nItemSize ) {}

		/// <summary>
		/// Creates a new queue instance.  This allocates the storage required by the
		/// new queue and returns a handle for the queue.
		/// </summary>
		virtual uint32_t Create( uint32_t nQueueLength, uint32_t nItemSize ) = 0;

		/// <summary>
		/// This method will destroy the queue and cleanup any memory associated with it.
		/// </summary>
		virtual ~scIQueue() {}

		/// <summary>
		/// This will attempt to send the an item to the Front of the queue. the call will fail if
		/// it cannot complete within the time specified.
		/// </summary>
		virtual uint32_t SendToFront( const void* pvItemToQueue, uint32_t xTicksToWait ) = 0;

		virtual uint32_t SendToFrontISR( const void* pvItemToQueue ) = 0;

		/// <summary>
		/// This will attempt to send the an item to the back of the queue. the call will fail if
		/// it cannot complete within the time specified.
		/// </summary>
		virtual uint32_t SendToBack( const void* pvItemToQueue, uint32_t xTicksToWait ) = 0;

		virtual uint32_t SendToBackISR( const void* pvItemToQueue ) = 0;

		/// <summary>
		/// Receive an item from a queue without removing the item from the queue.
		/// The item is received by copy so a buffer of adequate size must be
		/// provided.  The number of bytes copied into the buffer was defined when
		/// the queue was created.
		/// Successfully received items remain on the queue so will be returned again
		/// by the next call,
		/// </summary>
		virtual uint32_t Peek( void* pBuffer, uint32_t xTicksToWait ) = 0;

		/// <summary>
		/// Receive an item from a queue.  The item is received by copy so a buffer of
		/// adequate size must be provided.  The number of bytes copied into the buffer
		/// was defined when the queue was created.
		/// </summary>
		virtual uint32_t Receive( void* pBuffer, uint32_t xTicksToWait ) = 0;

		/// <summary>
		/// Return the number of free spaces available in a queue.  This is equal to the
		/// number of items that can be sent to the queue before the queue becomes full
		/// if no items are removed.
		/// </summary>
		virtual uint32_t SpacesAvailable(void) = 0;

	};
}	// Namespace SharedCore
#endif // __scIQueue_H

