//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          chris.snyder@commongroundss.com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  any later version.
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
// File Name:		scRingBuffer.h
// Created By:		Christopher Snyder
// Creation Date:	10-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCRINGBUFFER_H__INCLUDED_)
#define __SCRINGBUFFER_H__INCLUDED_

#include "scTypes.h"


namespace SharedCore
{
	// forward reference
	class scIMutex;

	/// <summary>
	/// This class will server as a wrapper to a block of memory and to manage the
	/// circular insert and extract operations. This is typically used with device
	/// drivers to provide block operations with incoming data and allow the exchange
	/// of the data with other objects in the system that must pull the data before
	/// overwrite occurs.
	/// </summary>
	class scRingBuffer
	{
	public:
		/// <summary>
		/// Construct the ring buffer using the provided length, and pointer. If the
		/// pointer is not provided then the buffer is internally allocated using the new
		/// command.
		/// </summary>
		/// <param name="nBufferSize">Size of the ring buffer in bytes</param>
		/// <param name="pBuffer">pointer to the ring buffer if pre-allocated. NULL will allocate the buffer</param>
		/// <param name="pLockContext">Pointer to the locking mechanism. NULL doesn't use one.</param>
		scRingBuffer( uint32_t nBufferSize, uint8_t* pBuffer, scIMutex* pLockContext );

		/// <summary>
		/// destructor
		/// </summary>
		virtual ~scRingBuffer();

		/// <summary>
		/// Returns the number of bytes that are available in the ring buffer.
		/// </summary>
		uint32_t Available(void) const;

		/// <summary>
		/// This is a utility method to shift the used portion of the ring buffer to the
		/// beginning to ensure that the largest block of memory is available for writing.
		/// </summary>
		void Contract(void);

		/// <summary>
		/// Returns the total number of bytes current in use in the ring buffer.
		/// </summary>
		uint32_t InUse(void) const;

		/// <summary>
		/// This method will return the total size of the ring buffer include the available
		/// and in use bytes.
		/// </summary>
		uint32_t MaxSize(void) const;

		/// <summary>
		/// Obtains the pointer to the extraction location in the ring buffer.
		/// </summary>
		const uint8_t* ReadBlock(void) const;

		/// <summary>
		/// This method will complete the read operation by removing the specified number
		/// of bytes from the buffer.
		/// </summary>
		/// <param name="nLength">The number of bytes that were read from the buffer.
		/// </param>
		uint32_t ReadEnd( uint32_t nLength );

		/// <summary>
		/// This method will prepare the ring buffer for removal of data. The return is the
		/// number of bytes that can be copied in a single operation.
		/// </summary>
		uint32_t ReadStart(void);

		/// <summary>
		/// This method will act as a clear and reinitialize the ring buffer to the initial
		/// state.
		/// </summary>
		void Reset(void);

		/// <summary>
		/// This method will return the pointer to the memory block that was prepared
		/// during a WriteStart operation. This pointer may be passed of to a trusted
		/// operation to copy or fill the memory. ISR routines will use this pointer for
		/// their copy operations.
		/// </summary>
		uint8_t* WriteBlock(void);

		/// <summary>
		/// This method is a safe version to copy data from one memory location into the
		/// ring buffer. This copy will automatically wrap.
		/// </summary>
		/// <param name="nLength">The number of bytes to copy from the source pointer.
		/// </param>
		/// <param name="pStart">The source pointer for the data to be copied into the ring
		/// buffer.</param>
		uint32_t WriteBlock(const uint8_t* pStart, uint32_t nLength );

		/// <summary>
		/// This method will complete a write operation and adjust the bookkeeping pointers
		/// in the ring buffer. The return will indicate the number of bytes still
		/// available in the buffer.
		/// </summary>
		/// <param name="nLength">The number of bytes that were copied into the ring buffer.
		/// </param>
		uint32_t WriteEnd(uint32_t nLength);

		/// <summary>
		/// This method will prepare the ring buffer for writing and will return the
		/// largest continuous block of memory that is available in the ring buffer. More
		/// memory may be available once the number of bytes specified here is consumed.
		/// </summary>
		uint32_t WriteStart(void);

		/// <summary>
		/// This method will lock the ring buffer to allow memory operations to happen.
		/// Unlock must be called when complete.
		/// </summary>
		void Lock(void);

		/// <summary>
		/// This method will release the mutex when the memory operations are complete.
		/// </summary>
		void Unlock(void);


	protected:
		/// <summary>
		/// This method will shift the byte array down by one byte.
		/// </summary>
		uint32_t Shift(void);

	private:
		/// <summary>
		/// Specifies the total number of bytes allocated in the ring buffer
		/// </summary>
		uint32_t					_nBufferSize;
		/// <summary>
		/// Pointer to the memory that is the ring buffer.
		/// </summary>
		uint8_t*					_pBuffer;

		/// <summary>
		/// This is the number of bytes that have been used in the ring buffer.
		/// </summary>
		uint32_t _nUsedSize;
		/// <summary>
		/// This is a special counter that is used to track the start and stop operations.
		/// </summary>
		int							_nWriteInUse;
		/// <summary>
		/// This is the pointer to the location where data will be extracted from the ring
		/// buffer.
		/// </summary>
		uint8_t*					 _pExtract;
		/// <summary>
		/// This is a pointer to the location in the ring buffer where data will be added
		/// into the memory.
		/// </summary>
		uint8_t*					_pInsert;
		/// <summary>
		/// This pointer is used for cases when a mutex is required to protected the
		/// control variables that access the memory. Failure to protect can cause memory
		/// collisions and unusual behavior.
		/// </summary>
		scIMutex*					_pProtectContext;


		// prevent copy constructor.
		scRingBuffer(const scRingBuffer& T ) {};

	};

}
#endif // !defined(__SCRINGBUFFER_H__INCLUDED_)
