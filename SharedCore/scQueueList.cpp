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
// File Name:		scQueueList.h
// Created By:		Christopher Snyder
// Creation Date:	15-Oct-2014
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================

#include <string.h>
#include "scQueueList.h"

namespace SharedCore
{
	/// <summary>
	/// This will construct the empty queue. If this constructor is used then the create
	/// method must be called before the object can be used. In general most implementations
	/// will use the parameteratized constructor.
	/// </summary>
	scQueueList::scQueueList( void )
		:	_items(0)
		,	_nItemSize(sizeof(uint8_t*))
		,	_nMaxListSize(1)
	{
	}

	/// <summary>
	/// Construct the queue with specifying the queue size and the size of the items being
	/// stored in the queue.
	/// </summary>
	scQueueList::scQueueList( uint32_t nQueueLength, uint32_t nItemSize )
		:	_items(nQueueLength)
		,	_nItemSize(nItemSize)
		,	_nMaxListSize(nQueueLength)
	{
		// The initial size just sets the max memory it needs to hold.
		// other list classes support the initial memory limit set.
		_items.clear();	// we clear the list.
	}

	/// <summary>
	/// Creates a new queue instance.  This allocates the storage required by the
	/// new queue and returns a handle for the queue.
	/// </summary>
	uint32_t scQueueList::Create( uint32_t nQueueLength, uint32_t nItemSize )
	{
		_nMaxListSize = nQueueLength;
		_nItemSize = nItemSize;
		return _nMaxListSize;
	}

	/// <summary>
	/// This method will destroy the queue and cleanup any memory associated with it.
	/// </summary>
	scQueueList::~scQueueList()
	{
		while( _items.size() > 0 )
		{
			delete _items.front();
			_items.pop_front();
		}
		_items.empty();
	}

	/// <summary>
	/// This will attempt to send the an item to the Front of the queue. the call will fail if
	/// it cannot complete within the time specified.
	/// </summary>
	uint32_t scQueueList::SendToFront( const void* pvItemToQueue, uint32_t xTicksToWait )
	{
		uint32_t nResult = 0;
		if ( SpacesAvailable() > 0 )
		{
			uint8_t* pNextItem = new uint8_t[_nItemSize];
#ifdef SC_SUPPORTS_CRT_SECURITY
			memcpy_s( pNextItem, _nItemSize, pvItemToQueue, _nItemSize );
#else
			memcpy( pNextItem, pvItemToQueue, _nItemSize );
#endif
			_items.push_front( pNextItem );
			nResult = 1;
		}
		return nResult;
	}

	uint32_t scQueueList::SendToFrontISR( const void* pvItemToQueue )
	{
		return SendToFront( pvItemToQueue, 0 );
	}

	/// <summary>
	/// This will attempt to send the an item to the back of the queue. the call will fail if
	/// it cannot complete within the time specified.
	/// </summary>
	uint32_t scQueueList::SendToBack( const void* pvItemToQueue, uint32_t xTicksToWait )
	{
		uint32_t nResult = 0;
		if ( SpacesAvailable() > 0 )
		{
			uint8_t* pNextItem = new uint8_t[_nItemSize];
#ifdef SC_SUPPORTS_CRT_SECURITY
			memcpy_s( pNextItem, _nItemSize, pvItemToQueue, _nItemSize );
#else
			memcpy( pNextItem, pvItemToQueue, _nItemSize );
#endif
			_items.push_back( pNextItem );
			nResult = 1;
		}
		return nResult;
	}

	uint32_t scQueueList::SendToBackISR( const void* pvItemToQueue )
	{
		return SendToBack( pvItemToQueue, 0 );
	}

	/// <summary>
	/// Receive an item from a queue without removing the item from the queue.
	/// The item is received by copy so a buffer of adequate size must be
	/// provided.  The number of bytes copied into the buffer was defined when
	/// the queue was created.
	/// Successfully received items remain on the queue so will be returned again
	/// by the next call,
	/// </summary>
	uint32_t scQueueList::Peek( void* pBuffer, uint32_t xTicksToWait )
	{
		uint32_t nResult = 0;

		if ( _items.size() > 0 )
		{
#ifdef SC_SUPPORTS_CRT_SECURITY
			memcpy_s( pBuffer, _nItemSize, _items.front(), _nItemSize );
#else
			memcpy( pBuffer, _items.front(), _nItemSize );
#endif
			nResult = 1;
		}
		return nResult;
	}

	/// <summary>
	/// Receive an item from a queue.  The item is received by copy so a buffer of
	/// adequate size must be provided.  The number of bytes copied into the buffer
	/// was defined when the queue was created.
	/// </summary>
	uint32_t scQueueList::Receive( void* pBuffer, uint32_t xTicksToWait )
	{
		uint32_t nResult = 0;

		if ( _items.size() > 0 )
		{
			uint8_t* pItem = reinterpret_cast<uint8_t*>(_items.front() );
			_items.pop_front();
#ifdef SC_SUPPORTS_CRT_SECURITY
			memcpy_s( pBuffer, _nItemSize, pItem, _nItemSize );
#else
			memcpy( pBuffer, pItem, _nItemSize );
#endif
			delete pItem;
			nResult = 1;
		}
		return nResult;
	}

	/// <summary>
	/// Return the number of free spaces available in a queue.  This is equal to the
	/// number of items that can be sent to the queue before the queue becomes full
	/// if no items are removed.
	/// </summary>
	uint32_t scQueueList::SpacesAvailable(void)
	{
		return _nMaxListSize - _items.size();
	}

}	// Namespace SharedCore