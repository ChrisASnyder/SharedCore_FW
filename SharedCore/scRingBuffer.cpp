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
// File Name:		scRingBuffer.h
// Created By:		Christopher Snyder
// Creation Date:	10-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include <string.h>
#include "scMutexNoOp.h"
#include "scRingBuffer.h"

#ifdef _DEBUG
#	define	DEBUG_RING_BUFFER
#endif

using SharedCore::scRingBuffer;
using SharedCore::scMutexNoOp;

#ifndef assert_param
#define assert_param(X)	if ( !(X) ) { for(;;) ; }
#endif

/// <summary>
/// Construct the ring buffer using the provided length, and pointer. If the
/// pointer is not provided then the buffer is internally allocated using the new
/// command.
/// </summary>
/// <param name="nBufferSize">Size of the ring buffer in bytes</param>
/// <param name="pBuffer">pointer to the ring buffer if pre-allocated. NULL will
/// allocate the buffer</param>
/// <param name="pLockContext">Pointer to the locking mechanism. NULL doesn't use
/// one.</param>
scRingBuffer::scRingBuffer( uint32_t nBufferSize, uint8_t* pBuffer, SharedCore::scIMutex* pLockContext )
	:	_nBufferSize(nBufferSize)
		, _pBuffer(pBuffer)
		, _nUsedSize(0)
		, _nWriteInUse(0)
		, _pExtract(pBuffer)
		, _pInsert(pBuffer)
		, _pProtectContext(NULL)
{
	if ( _pBuffer == NULL )
	{
		_pBuffer =  new uint8_t[_nBufferSize];	// should maybe use an allocator
	}
	_pExtract = _pInsert = _pBuffer;
#ifdef DEBUG_RING_BUFFER
	memset( _pBuffer, 0xFF, _nBufferSize );
#endif
	if ( _pProtectContext == NULL )
	{
		_pProtectContext = new scMutexNoOp();
	}
}

/// <summary>
/// destructor
/// </summary>
scRingBuffer::~scRingBuffer()
{
	_pBuffer = NULL;
	_pExtract = NULL;
	_pInsert = NULL;
	delete _pProtectContext;
	delete []_pBuffer;
}

/// <summary>
/// Returns the number of bytes that are available in the ring buffer.
/// </summary>
uint32_t scRingBuffer::Available(void) const
{
	return _nBufferSize - _nUsedSize;
}

/// <summary>
/// This is a utility method to shift the used portion of the ring buffer to the
/// beginning to ensure that the largest block of memory is available for writing.
/// </summary>
void scRingBuffer::Contract(void)
{
	// Only permit the contraction of the buffer if there are currently no
	// outstanding write operations. Write operations would cause problems.
	if ( _nWriteInUse == 0 )
	{
		if ( _nUsedSize > 0 && _pExtract != (_pBuffer+_nUsedSize) )
		{
			memmove( _pBuffer, _pExtract, _nUsedSize );
			_pExtract = _pBuffer;
			_pInsert = _pBuffer + _nUsedSize;
	#ifdef DEBUG_RING_BUFFER
			memset( _pInsert, 0xFF, _nBufferSize - _nUsedSize );
	#endif
		}

		if ( _nUsedSize == 0 )
		{
			_pInsert = _pExtract = _pBuffer;
		}
	}
}

/// <summary>
/// Returns the total number of bytes current in use in the ring buffer.
/// </summary>
uint32_t scRingBuffer::InUse(void) const
{
	return _nUsedSize;
}

/// <summary>
/// This method will return the total size of the ring buffer include the available
/// and in use bytes.
/// </summary>
uint32_t scRingBuffer::MaxSize(void) const
{
	return _nBufferSize;
}

/// <summary>
/// Obtains the pointer to the extraction location in the ring buffer.
/// </summary>
const uint8_t* scRingBuffer::ReadBlock() const
{
	return _pExtract;
}

/// <summary>
/// This method will complete the read operation by removing the specified number
/// of bytes from the buffer.
/// </summary>
/// <param name="nLength">The number of bytes that were read from the buffer.
/// </param>
uint32_t scRingBuffer::ReadEnd(uint32_t nLength)
{
	assert_param( _pProtectContext != NULL );
	uint32_t nBlockLen = ReadStart();
	_pProtectContext->Acquire();
	uint32_t nUsedTemp = _nUsedSize;    // only stop ISRs once
	_pProtectContext->Release();

	// Don't remove more data than we have in the buffer
	if ( nLength > nUsedTemp )
	{
		nLength = nUsedTemp;
	}

	if ( nLength > nBlockLen )
	{
		// Wrap around the ring buffer
		nLength = nLength - nBlockLen;
		ReadEnd( nBlockLen );
		ReadEnd( nLength );
	}
	else
	{
		_pProtectContext->Acquire();
		_nUsedSize-= nLength;										// Subtract the bytes removed
		_pProtectContext->Release();
#ifdef DEBUG_RING_BUFFER
		memset( _pExtract, 0xFF, nLength );							// Zero out the buffer
#endif

		if ( _pExtract + nLength >= _pBuffer + _nBufferSize )		// Wrap around buffer
		{
			_pExtract = _pBuffer;
		}
		else
		{
			_pExtract+= nLength;
		}
	}
	return _nUsedSize;
}

/// <summary>
/// This method will prepare the ring buffer for removal of data. The return is the
/// number of bytes that can be copied in a single operation.
/// </summary>
uint32_t scRingBuffer::ReadStart(void)
{
	assert_param( _pProtectContext != NULL );
	_pProtectContext->Acquire();
	uint32_t nUsedTemp = _nUsedSize;    // only stop ISRs once
	_pProtectContext->Release();

	uint32_t nLenToBufEnd = _nBufferSize - (_pExtract - _pBuffer);
	if ( nLenToBufEnd < nUsedTemp )
	{
		return nLenToBufEnd;
	}
	return nUsedTemp;
}

/// <summary>
/// This method will act as a clear and reinitialize the ring buffer to the initial
/// state.
/// </summary>
void scRingBuffer::Reset(void)
{
#ifdef DEBUG_RING_BUFFER
	memset( _pBuffer, 0xFF, _nBufferSize );
#endif
	_pExtract = _pInsert = _pBuffer;
	_nUsedSize = 0;

	assert_param( _nWriteInUse != 0 );

	// What if this isn't zero when this happens. This will probably
	// mean that a write operation is pending but the queue is getting
	// reset. Probably something bad is about to happing in this situation.
	_nWriteInUse = 0;
}

/// <summary>
/// This method will return the pointer to the memory block that was prepared
/// during a WriteStart operation. This pointer may be passed of to a trusted
/// operation to copy or fill the memory. ISR routines will use this pointer for
/// their copy operations.
/// </summary>
uint8_t* scRingBuffer::WriteBlock(void)
{
	return _pInsert;
}

/// <summary>
/// This method is a safe version to copy data from one memory location into the
/// ring buffer. This copy will automatically wrap.
/// </summary>
/// <param name="pStart">The source pointer for the data to be copied into the ring
/// buffer.</param>
/// <param name="nLength">The number of bytes to copy from the source pointer.
/// </param>
uint32_t scRingBuffer::WriteBlock(const uint8_t* pStart, uint32_t nLength )
{
	uint32_t nWritten = 0;
	// Will this length fit from the insertion point to the end of
	// the buffer
	if ( _pInsert + nLength <= _pBuffer + _nBufferSize )
	{
		// Yes just copy the whole buffer
		memcpy( _pInsert, pStart, nLength );
		WriteEnd( nLength );
		nWritten = nLength;
	}
	else
	{
		// No, break it into two write block
		uint32_t nDiff = (uint32_t)( (_pBuffer + _nBufferSize) - _pInsert );
		nWritten = WriteBlock( pStart, nDiff );
		if ( WriteStart() > 0 )
		{
			nWritten+= WriteBlock( pStart+nWritten, nLength - nWritten );
		}
		else
		{
			// This is an error.
			WriteEnd(0);
		}
	}
	return nWritten;
}

/// <summary>
/// This method will complete a write operation and adjust the bookkeeping pointers
/// in the ring buffer. The return will indicate the number of bytes still
/// available in the buffer.
/// </summary>
/// <param name="nLength">The number of bytes that were copied into the ring buffer.
/// </param>
uint32_t scRingBuffer::WriteEnd(uint32_t nLength)
{
	_nUsedSize+= nLength;
	_pInsert+=nLength;

	// Wrap around insert pointer
	if ( _pInsert >= (_pBuffer + _nBufferSize ) )
	{
		_pInsert = _pBuffer;
	}

	assert_param( _nUsedSize <= _nBufferSize );

	--_nWriteInUse;

	return _nBufferSize - _nUsedSize;
}

/// <summary>
/// This method will prepare the ring buffer for writing and will return the
/// largest continuous block of memory that is available in the ring buffer. More
/// memory may be available once the number of bytes specified here is consumed.
/// </summary>
uint32_t scRingBuffer::WriteStart(void)
{
	uint32_t dif = (uint32_t)(_pInsert - _pBuffer);
	uint32_t nResult = 0;

	++_nWriteInUse;

	if ( dif < _nUsedSize )		// wrap around
	{
		nResult = _nBufferSize - _nUsedSize;
	}
	else
	{
		nResult = _nBufferSize - dif;
	}
	return nResult;
}

/// <summary>
/// This method will shift the byte array down by one byte.
/// </summary>
uint32_t scRingBuffer::Shift(void)
{
	for( uint32_t i=0; i < _nUsedSize-1; i++ )
	{
		*(_pExtract+i) = *(_pExtract+i+1);
	}
	_pInsert--;
	return --_nUsedSize;
}

/// <summary>
/// This method will lock the ring buffer to allow memory operations to happen.
/// Unlock must be called when complete.
/// </summary>
void scRingBuffer::Lock(void)
{
	assert_param( _pProtectContext != NULL );
	_pProtectContext->Acquire();
}

/// <summary>
/// This method will release the mutex when the memory operations are complete.
/// </summary>
void scRingBuffer::Unlock(void)
{
	assert_param( _pProtectContext != NULL );
	_pProtectContext->Release();
}