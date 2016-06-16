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
// File Name:		scMessageFactory.h
// Created By:		Christopher Snyder
// Creation Date:	15-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCMESSAGEFACTORY_H__INCLUDED_)
#define __SCMESSAGEFACTORY_H__INCLUDED_

#include "scTypes.h"
#include "scIAllocator.h"
#include "scIMutex.h"
#include "scDebugManager.h"
#include "scRingBuffer.h"
#include "scScopeLock.h"
#include "scDebugLabelCodes.h"
#include "scErrorCodes.h"
#include <vector>


#ifndef assert_param
#define assert_param(X)	if ( !(X) ) { for(;;) ; }
#endif


namespace SharedCore
{
	/// <summary>
	/// The message factory is used to provide two services. The first is to create
	/// messages for each message ID in the system. This is handled by application
	/// subclasses. The second feature is to manage the memory usage of all messages in
	/// the system by maintaining a list of message slots to use for messages that move
	/// through the system. The factory also maintains a small internal memory buffer
	/// that can serve as the message  buffer for small messages, larger messages will
	/// need to use the allocator to get memory allocated for them. There is still a
	/// fixed number of message slots available. The factory will also support using
	/// external ring buffers to overlay a message slot on. This can be through
	/// internal copy or direct overlay.
	/// </summary>
	template<class IMessage>
	class scMessageFactory
	{
	public:
		/// <summary>
		/// Destructor. This will use the internal allocator to release all the memory used.
		/// </summary>
		virtual ~scMessageFactory();

		/// <summary>
		/// This will perform initialize the internal memory usage.
		/// </summary>
		/// <param name="overflowAllocator">allocator used to create internal memory.
		/// </param>
		/// <param name="internalAllocator">The allocator that will be used to allocate the
		/// required memory. This is for the internal memory allocations that are static
		/// and one time use.</param>
		/// <param name="pProtect">mutex to protect the memory</param>
		virtual uint32_t Initialize( scAllocator internalAllocator, scAllocator overflowAllocator, scIMutex* pProtect );

		/// <summary>
		/// Get the last error.
		/// </summary>
		uint32_t GetLastError()
		{
			return _nLastError;
		}

		/// <summary>
		/// Create a new message
		/// </summary>
		/// <param name="pBuffer">pointer to the buffer that contains data that needs to be
		/// wrapped by the message class.</param>
		/// <param name="nSize">Number of bytes allocated for the message.</param>
		virtual IMessage* Create( uint8_t* pBuffer, uint32_t nSize );

		/// <summary>
		/// Create a new message
		/// </summary>
		/// <param name="pBuffer">pointer to the buffer that contains data that needs to be
		/// wrapped by the message class.</param>
		/// <param name="nSize">Number of bytes allocated for the message.</param>
		virtual IMessage* Create( uint32_t nSize );

		/// <summary>
		/// Create a new message
		/// </summary>
		/// <param name="pBuffer">pointer to the buffer that contains data that needs to be
		/// wrapped by the message class.</param>
		/// <param name="nSize">Number of bytes allocated for the message.</param>
		virtual IMessage* Create( scRingBuffer* pRing );

		/// <summary>
		/// Create a copy of the message provided and use the internal/overflow storage to
		/// make the copy.
		/// </summary>
		/// <param name="pMessage">Message object to be copied.</param>
		virtual IMessage* Copy( const IMessage* pMessage );

		/// <summary>
		/// Adds an additional owner to the message so when one object releases the message
		/// it continues to be valid until all users release it.
		/// </summary>
		virtual void Acquire( const IMessage* pMessage );

		/// <summary>
		/// This will release the resources used by this message. The factory is tracking
		/// how this message has been allocated.
		/// </summary>
		virtual bool Release( const IMessage* pMessage );

		/// <summary>
		/// Utility method to dump the contents of the internal message list.
		/// </summary>
		void DebugDump(void);

		/// <summary>
		/// Will obtain the number of message slots available
		/// </summary>
		uint16_t MessagesAvailable(void) const;

		/// <summary>
		/// Will obtain the number of message slots in use.
		/// </summary>
		uint16_t MessagesInUse(void) const;

	protected:
		/// <summary>
		/// If an error occurs this value will reflect the last one.
		/// </summary>
		uint32_t		_nLastError;

		/// <summary>
		/// The constructor that will specify the elements of the message factory.
		/// </summary>
		/// <param name="nMessageSlots">The number of message slots available to the
		/// factory for use. This value is system dependant and is determined by the amount
		/// of memory the system has to spare and how many messages need to exist at one
		/// time in the system. Generally, this can probably be a small number.</param>
		/// <param name="nMessageBufferSize">Specifies the amount of memory the factory
		/// should allocate as a statically reserved area for placing message contents.
		/// This should be sized based on the typical size of the message structures in
		/// this system. 0 is an allowed value.</param>
		scMessageFactory( uint16_t nMessageSlots, uint32_t nMessageBufferSize );

		/// <summary>
		/// This is called following the creating of a message to allow the subclass the
		/// option to perform additional modifications to a newly created message. Such as
		/// insert a sequence number.
		/// </summary>
		/// <param name="pMessage">pointer to the new message allocated.</param>
		virtual IMessage* PostCreate( IMessage* pMessage )
		{
			return pMessage;
		}

	private:
		typedef enum
		{
			/// <summary>
			/// The pointer is not defined, probably a NULL value.
			/// </summary>
			mem_None,

			/// <summary>
			/// The pointer stored is from the scMessageFactory internal storage buffer.
			/// </summary>
			mem_LocalBuffer,

			/// <summary>
			/// This message is attached to a ring buffer and must be released. The pointer is
			/// a ring buffer pointer and can be cast to make the ReadEnd Call.
			/// </summary>
			mem_RingBuffer,

			/// <summary>
			/// The pointer was allocated using the overflow allocator and should be released
			/// using it.
			/// </summary>
			mem_Allocator,

			/// <summary>
			/// The pointer is something external to the factory and will be managed on it's
			/// own. The factory will not perform any action on the memory.
			/// </summary>
			mem_External
		} MemoryType_t;

		typedef struct
		{
			/// <summary>
			/// Pointer to the message object these are all pre-allocated.
			/// </summary>
			IMessage*		_pMessage;

			/// <summary>
			/// This is a counter flag indicating how many users have the message in use. If
			/// desired other owners may acquire a lock on a message to prevent it from being
			/// released by other processes. This value will increase for each user.
			/// </summary>
			int				_nInUse;

			/// <summary>
			/// This is the pointer to the buffer in which the message object wraps. The buffer
			/// Type describes the pointer type of this object.
			/// </summary>
			void*			_pBuffer;

			/// <summary>
			/// Defined by the enum on how to handle the cleanup of the message pointer.
			/// </summary>
			MemoryType_t	_BufferType;

			/// <summary>
			/// The number of bytes allocated to the message object.
			/// </summary>
			uint32_t		_nSize;

			/// <summary>
			/// An increasing Id for each message in the system. This number will simply roll
			/// over when the max is reached.
			/// </summary>
			uint32_t		_nId;
		} InternalRecord_t;

		typedef vector<InternalRecord_t*>	SlotVector_t;

		/// <summary>
		/// A mutex to protect the memory allocation and the list usage.
		/// </summary>
		scIMutex*					_pProtoect;

		/// <summary>
		/// This counter will track how many items have been allocated. It's mostly for debugging to
		/// allow knowledge of how many slots have been used without having to iterate through the list.
		/// </summary>
		int							_nInUseCounter;

		/// <summary>
		/// This is a counter to track the number of messages allocated by the factory.
		/// This can be used as a sequence number for the message system.
		/// </summary>
		uint32_t					_nMessageCounter;

		/// <summary>
		/// The number of message slots in the factory.
		/// </summary>
		uint16_t					_nMessageSlots;

		/// <summary>
		/// The number of bytes made available for the internal message buffer.
		/// </summary>
		uint32_t					_nMessageBufferSize;

		/// <summary>
		/// The internal message buffer.
		/// </summary>
		uint8_t*					_pMessageBuffer;

		/// <summary>
		/// The internal record of the messages in use.
		/// </summary>
		SlotVector_t				_Records;

		/// <summary>
		/// Used to quickly find the next index in the system that is available to hold a
		/// new message.
		/// </summary>
		size_t						_nNextAvailable;

		/// <summary>
		/// The allocator that will be used to allocate and free memory used internally.
		/// </summary>
		scAllocator					_InternalAllocator;

		/// <summary>
		/// The allocator that will be used to allocate and free memory when the size of
		/// the message is larger than the internal buffer.
		/// </summary>
		scAllocator					_OverFlowAllocator;

		/// <summary>
		/// This will loop through the vector table looking for an available message slot,
		/// and will return the index in the table that contains the record to use. If a
		/// record is available the return is true, if a record is not available the result
		/// is false and the index is invalid.
		/// </summary>
		bool NextAvailableRecord( size_t& nIndex );

		/// <summary>
		/// This method will configure the specified record as being in use.
		/// </summary>
		/// <param name="pRecord">Pointe to the record.</param>
		/// <param name="pBuffer">Pointer to the memory that contains the message.</param>
		/// <param name="nSize">Number of bytes allocated to the pBuffer.</param>
		/// <param name="nType">How this memory pointer is managed.</param>
		void SetInUse( InternalRecord_t* pRecord, void* pBuffer, uint32_t nSize, MemoryType_t nType );

		/// <summary>
		/// This is a utility method to clear the field from the record.
		/// </summary>
		/// <param name="pRecord">Pointer to the record.</param>
		void ClearInUse( InternalRecord_t* pRecord );

		/// <summary>
		/// This method will create the message pointer and provide the allocated buffer.
		/// If the data being requested is too large for the internal buffer space a new
		/// buffer will be allocated.
		/// </summary>
		/// <param name="nSize">Size being requested.</param>
		/// <param name="pRecord">Pointer to the record to be used for the entry.</param>
		bool AllocateRecord(uint32_t nSize, InternalRecord_t** pRecord );

		/// <summary>
		/// Scans through the allocation table and finds the larges block of memory available and
		/// returns the pointer to the start only if the block is large enough to hold the size
		/// of the message requested.
		/// </summary>
		uint8_t* GetLocalPointer( uint32_t nSize );

		/// <summary>
		/// Difficult to manage the allocation of the template cleanly. The allocator doesn't handle
		/// the new operator well, so for now I will create this virtual allocator that can be 
		/// modified by applications if they want the object put someplace else.
		/// </summary>
		virtual IMessage* MessageObject_Allocate(void)
		{ return new IMessage(); }

		virtual void MessageObject_Destroy( IMessage* pMessage )
		{
			delete pMessage; 
		}
	};
	
//////////////////////////////////////////////////////////////////////////////////////
/// Protected Methods
//////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// The constructor that will specify the elements of the message factory.
	/// </summary>
	/// <param name="nMessageSlots">The number of message slots available to the
	/// factory for use. This value is system dependant and is determined by the amount
	/// of memory the system has to spare and how many messages need to exist at one
	/// time in the system. Generally, this can probably be a small number.</param>
	/// <param name="nMessageBufferSize">Specifies the amount of memory the factory
	/// should allocate as a statically reserved area for placing message contents.
	/// This should be sized based on the typical size of the message structures in
	/// this system. 0 is an allowed value.</param>
	template<class IMessage>
	scMessageFactory<IMessage>::scMessageFactory( uint16_t nMessageSlots, uint32_t nMessageBufferSize )
		:	_pProtoect(NULL)
		,	_nInUseCounter(0)
		,	_nMessageCounter(0)
		,	_nMessageSlots(nMessageSlots)
		,	_nMessageBufferSize(nMessageBufferSize)
		,	_pMessageBuffer(NULL)
		,	_Records(_nMessageSlots)
		,	_nNextAvailable(0)
		,	_InternalAllocator()
		,	_OverFlowAllocator()
		,	_nLastError(ERROR_SUCCESS)
	{
	}

//////////////////////////////////////////////////////////////////////////////////////
/// Public Methods
//////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// Destructor. This will use the internal allocator to release all the memory used.
	/// </summary>
	template<class IMessage>
	scMessageFactory<IMessage>::~scMessageFactory()
	{
		SlotVector_t::iterator itr = _Records.begin();
		for( ; itr != _Records.end(); ++itr )
		{
			//_InternalAllocator.Destroy( (*itr)->_pMessage );
			if ( (*itr) != NULL )
			{
				MessageObject_Destroy( (*itr)->_pMessage );
				(*itr)->_pMessage = NULL;
			}
			_InternalAllocator.Destroy( (*itr) );
		}
		_InternalAllocator.Destroy(_pMessageBuffer);
		_pMessageBuffer = NULL;
	}

	/// <summary>
	/// This will perform initialize the internal memory usage.
	/// </summary>
	/// <param name="overflowAllocator">allocator used to create internal memory.
	/// </param>
	/// <param name="internalAllocator">The allocator that will be used to allocate the
	/// required memory. This is for the internal memory allocations that are static
	/// and one time use.</param>
	/// <param name="pProtect">mutex to protect the memory</param>
	template<class IMessage>
	uint32_t scMessageFactory<IMessage>::Initialize( scAllocator internalAllocator, scAllocator overflowAllocator, scIMutex* pProtect )
	{
		assert_param( pProtect != NULL );	// need a valid protect object

		_pProtoect = pProtect;
		_InternalAllocator = internalAllocator;
		_OverFlowAllocator = overflowAllocator;

		// zero is allowed to force all messages into the overflow allocator
		if ( _nMessageBufferSize > 0 )
		{
			// Allocate the internal message buffer
			_pMessageBuffer = _InternalAllocator.Allocate( _nMessageBufferSize );

			if ( _pMessageBuffer == NULL )
			{
				_nMessageBufferSize = 0;
				scDebugManager::Instance()->Trace( 
					scDEBUGLABEL_WARNING_MESSAGE, 
					"scMessageFactory: No overflow buffer.\n\r" );
			}
		}
		else
		{
			_pMessageBuffer = NULL;
		}

		// Create the message slots vector
		for( size_t i=0 ; i < _Records.size(); i++ )
		{
			InternalRecord_t* pRecord = reinterpret_cast<InternalRecord_t*>(_InternalAllocator.Allocate( sizeof(InternalRecord_t ) ));

			if ( pRecord != NULL )
			{
				memset( pRecord, 0, sizeof(InternalRecord_t ) );
				
				// Not sure how to wrap this in an allocator in a clean way
				//pRecord->_pMessage = _InternalAllocator.Allocate( sizeof( IMessage ) );
				pRecord->_pMessage = MessageObject_Allocate();
				if ( pRecord->_pMessage != NULL )
				{
					ClearInUse( pRecord );
					_Records[i] = pRecord;
				}
				else
				{
					// memory allocation failure.
					_InternalAllocator.Destroy( pRecord );
					_Records[i] = NULL;
					_nLastError = ERROR_SC_MEMORY_ALLOCATION_FAILURE;
				}
			}
			else
			{
				scDebugManager::Instance()->Trace( 
					scDEBUGLABEL_WARNING_MESSAGE, 
					"scMessageFactory: Unable to allocate memory message slot memory.\n\r" );
				_nLastError = ERROR_SC_MEMORY_ALLOCATION_FAILURE;
			}
		}

		return _nLastError;
	}

	/// <summary>
	/// Create a new message
	/// </summary>
	/// <param name="pBuffer">pointer to the buffer that contains data that needs to be
	/// wrapped by the message class.</param>
	/// <param name="nSize">Number of bytes allocated for the message.</param>
	template<class IMessage>
	IMessage* scMessageFactory<IMessage>::Create( uint8_t* pBuffer, uint32_t nSize )
	{
		assert_param( _pProtoect != NULL );

		IMessage*		pResult = NULL;
		scScopeLock		protect( _pProtoect );
		size_t			nIndex = 0;

		if ( NextAvailableRecord( nIndex ) )
		{
			InternalRecord_t* pRecord = _Records[nIndex];
				
			*(pRecord->_pMessage) = IMessage( pBuffer, nSize);
			SetInUse( pRecord, pBuffer, nSize, mem_External );

			pResult = PostCreate( pRecord->_pMessage );
		}
		else
		{
			scDebugManager::Instance()->Trace( 
				scDEBUGLABEL_WARNING_MESSAGE, 
				"scMessageFactory: No record slots open.\n\r" );
			_nLastError = ERROR_SC_MESSAGEFACTORY_FULL;
		}

		return pResult;
	}

	/// <summary>
	/// Create a new message
	/// </summary>
	/// <param name="pBuffer">pointer to the buffer that contains data that needs to be
	/// wrapped by the message class.</param>
	/// <param name="nSize">Number of bytes allocated for the message.</param>
	template<class IMessage>
	IMessage* scMessageFactory<IMessage>::Create( uint32_t nSize )
	{
		assert_param( _pProtoect != NULL );

		IMessage*		pResult = NULL;
		scScopeLock		protect( _pProtoect );

		InternalRecord_t* pRecord = NULL;
		if ( AllocateRecord( nSize, &pRecord ) )
		{
			pResult = PostCreate( pRecord->_pMessage );
		}
		else
		{
			scDebugManager::Instance()->Trace( scDEBUGLABEL_ERROR_MESSAGE, 
				"scMessageFactory: Create failure memory full. %u bytes\n\r", nSize );
			_nLastError = ERROR_SC_MESSAGEFACTORY_FULL;
		}

		return pResult;
	}

	/// <summary>
	/// Create a new message
	/// </summary>
	/// <param name="pBuffer">pointer to the buffer that contains data that needs to be
	/// wrapped by the message class.</param>
	/// <param name="nSize">Number of bytes allocated for the message.</param>
	template<class IMessage>
	IMessage* scMessageFactory<IMessage>::Create( scRingBuffer* pRing )
	{
		assert_param( _pProtoect != NULL );

		IMessage*		pResult = NULL;
		scScopeLock		protect( _pProtoect );
		size_t			nIndex = 0;

		if ( NextAvailableRecord( nIndex ) )
		{
			InternalRecord_t* pRecord = _Records[nIndex];
				
			*(pRecord->_pMessage) = IMessage( pRing->ReadBlock(), pRing->ReadStart() );
			SetInUse( pRecord, pRing, (pRecord->_pMessage)->LengthOfMessage(), mem_RingBuffer );

			pResult = PostCreate( pRecord->_pMessage );
		}
		else
		{
			scDebugManager::Instance()->Trace( 
				scDEBUGLABEL_WARNING_MESSAGE, 
				"scMessageFactory: No record slots open.\n\r" );
			_nLastError = ERROR_SC_MESSAGEFACTORY_FULL;
		}

		return pResult;
	}

	/// <summary>
	/// Create a copy of the message provided and use the internal/overflow storage to
	/// make the copy.
	/// </summary>
	/// <param name="pMessage">Message object to be copied.</param>
	template<class IMessage>
	IMessage* scMessageFactory<IMessage>::Copy( const IMessage* pMessage )
	{
		assert_param( _pProtoect != NULL );

		IMessage*		pResult = NULL;
		scScopeLock		protect( _pProtoect );

		if ( pMessage != NULL )
		{
			InternalRecord_t* pRecord = NULL;
			uint32_t nSize = pMessage->LengthOfMessage();

			if ( AllocateRecord( nSize, &pRecord ) )
			{
				assert_param( pRecord->_pBuffer != NULL );
				memcpy( pRecord->_pBuffer, pMessage->Buffer(), nSize );
				*(pRecord->_pMessage) = IMessage( 
												reinterpret_cast<const uint8_t*>(pRecord->_pBuffer), 
												nSize );

				pResult = PostCreate( pRecord->_pMessage );
			}
			else
			{
				scDebugManager::Instance()->Trace( scDEBUGLABEL_ERROR_MESSAGE, 
					"scMessageFactory: Copy failure memory full. %u bytes\n\r", nSize );
				_nLastError = ERROR_SC_MESSAGEFACTORY_FULL;
			}
		}

		return pResult;
	}

	/// <summary>
	/// Adds an additional owner to the message so when one object releases the message
	/// it continues to be valid until all users release it.
	/// </summary>
	template<class IMessage>
	void scMessageFactory<IMessage>::Acquire( const IMessage* pMessage )
	{
		assert_param( _pProtoect != NULL );

		IMessage*		pResult = NULL;
		scScopeLock		protect( _pProtoect );

		if ( pMessage != NULL )
		{
			SlotVector_t::iterator itr = _Records.begin();
			for( ; itr != _Records.end(); ++itr )
			{
				if ( (*itr)->_pMessage == pMessage )
				{
					if ( (*itr)->_nInUse == 0 )
					{
						scDebugManager::Instance()->Trace( scDEBUGLABEL_WARNING_MESSAGE, 
							"scMessageFactory: Acquired message not in use\n\r" );
					}
					(*itr)->_nInUse++;
				}
			}
		}
	}

	/// <summary>
	/// This will release the resources used by this message. The factory is tracking
	/// how this message has been allocated.
	/// </summary>
	template<class IMessage>
	bool scMessageFactory<IMessage>::Release( const IMessage* pMessage )
	{
		bool bResult = false;

		if ( pMessage != NULL )
		{
			assert_param( _pProtoect != NULL );

			scScopeLock			protect( _pProtoect );
			InternalRecord_t*	pRecord = NULL;

			// First locate the record for this message
			SlotVector_t::iterator itr = _Records.begin();
			for( ; pRecord == NULL && itr != _Records.end(); ++itr )
			{
				if ( (*itr)->_pMessage == pMessage )
				{
					pRecord = (*itr);
				}
			}

			if ( pRecord != NULL && pRecord->_nInUse > 0 )
			{
				pRecord->_nInUse--;		// decrement the in use record
				if ( pRecord->_nInUse == 0 )
				{
					switch( pRecord->_BufferType )
					{
					default:
					case mem_None:
						{
							scDebugManager::Instance()->Trace( scDEBUGLABEL_WARNING_MESSAGE, 
								"scMessageFactory: Releasing unknown memory type.\n\r" );
							break;
						}
					case mem_LocalBuffer:
						{
							if ( (pRecord->_pBuffer >= _pMessageBuffer) && (pRecord->_pBuffer < (_pMessageBuffer + _nMessageBufferSize ) ) )
							{
#ifdef _DEBUG
								// Fill with debug 
								memset( pRecord->_pBuffer, 0xEA, pRecord->_nSize );
#endif
							}
							else
							{
								// this is a difficult path to test because this is a case that should
								// never happen in practice. If it does usually memory corruption has
								// happened. The classes will not create this state.
								scDebugManager::Instance()->Trace( scDEBUGLABEL_WARNING_MESSAGE, 
									"scMessageFactory: Message pointer not in local buffer.\n\r" );
							}
							break;
						}
					case mem_RingBuffer:
						{
							scRingBuffer* pRing = reinterpret_cast<scRingBuffer*>(pRecord->_pBuffer);
							pRing->ReadEnd( pRecord->_pMessage->LengthOfMessage() );
							break;
						}
					case mem_Allocator:
						{
							_OverFlowAllocator.Destroy( pRecord->_pBuffer );
							break;
						}
					case mem_External:
						{
							// Do nothing
							break;
						}
					}
					ClearInUse( pRecord );
					bResult = true;
				}

				// This is a point where a check for old messages could be performed.
				// leaving out for now.
			}
			else
			{
				scDebugManager::Instance()->Trace( scDEBUGLABEL_WARNING_MESSAGE, 
					"scMessageFactory: Releasing message not in use.\n\r" );
			}

		}

		return bResult;
	}

	/// <summary>
	/// Utility method to dump the contents of the internal message list.
	/// </summary>
	template<class IMessage>
	void scMessageFactory<IMessage>::DebugDump(void)
	{
		assert_param( _pProtoect != NULL );

		scScopeLock		protect( _pProtoect );
		scDebugManager* pDm = scDebugManager::Instance();
		pDm->Trace( scDEBUGLABEL_INFO_MESSAGE, "scMessageFactory: Records Dump\n\r" );

		size_t i = 0;
		SlotVector_t::iterator itr = _Records.begin();
		for( ; itr != _Records.end(); itr++ )
		{
			pDm->Trace( scDEBUGLABEL_INFO_MESSAGE, 
				"MessageFactory: [%u] %s, %u, #%u, %u\n\r",
				i++,
				((*itr)->_nInUse == 0 ? "Open" : "Used" ),
				(*itr)->_pMessage->GetID(),	// this could cause problems later
				(*itr)->_nSize,
				(*itr)->_BufferType );
		}
	}

//////////////////////////////////////////////////////////////////////////////////////
/// Private Methods
//////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// This will loop through the vector table looking for an available message slot,
	/// and will return the index in the table that contains the record to use. If a
	/// record is available the return is true, if a record is not available the result
	/// is false and the index is invalid.
	/// </summary>
	template<class IMessage>
	bool scMessageFactory<IMessage>::NextAvailableRecord( size_t& nIndex )
	{
		size_t nMax = _Records.size();
		do
		{
			nIndex = _nNextAvailable++;
			if ( _nNextAvailable >= _Records.size() )
			{
				_nNextAvailable = 0;
			}
		} while( _Records[nIndex]->_nInUse > 0 && --nMax >  0 );

		return nMax > 0;
	}

	/// <summary>
	/// This method will configure the specified record as being in use.
	/// </summary>
	/// <param name="pRecord">Pointe to the record.</param>
	/// <param name="pBuffer">Pointer to the memory that contains the message.</param>
	/// <param name="nSize">Number of bytes allocated to the pBuffer.</param>
	/// <param name="nType">How this memory pointer is managed.</param>
	template<class IMessage>
	void scMessageFactory<IMessage>::SetInUse( InternalRecord_t* pRecord, void* pBuffer, uint32_t nSize, MemoryType_t nType  )
	{
		pRecord->_nInUse			= 1;
		pRecord->_pBuffer			= pBuffer;
		pRecord->_nSize				= nSize;
		pRecord->_BufferType		= nType;
		pRecord->_nId				= _nMessageCounter++;

		++_nInUseCounter;
	}

	/// <summary>
	/// This is a utility method to clear the field from the record.
	/// </summary>
	/// <param name="pRecord">Pointer to the record.</param>
	template<class IMessage>
	void scMessageFactory<IMessage>::ClearInUse( InternalRecord_t* pRecord )
	{
		pRecord->_nInUse			= 0;
		pRecord->_pBuffer			= NULL;
		pRecord->_nSize				= 0;
		pRecord->_BufferType		= mem_None;
		pRecord->_nId				= 0;
	}

	/// <summary>
	/// This method will create the message pointer and provide the allocated buffer.
	/// If the data being requested is too large for the internal buffer space a new
	/// buffer will be allocated.
	/// </summary>
	/// <param name="nSize">Size being requested.</param>
	template<class IMessage>
	bool scMessageFactory<IMessage>::AllocateRecord( uint32_t nSize, InternalRecord_t** pRecord )
	{
		size_t				nIndex = 0;
		MemoryType_t		nMemType = mem_None;
		uint8_t*			pBuffer = NULL;

		*pRecord = NULL;

		do
		{
			if ( !NextAvailableRecord( nIndex ) )
			{
				scDebugManager::Instance()->Trace( 
				scDEBUGLABEL_WARNING_MESSAGE, 
				"scMessageFactory: No record slots open.\n\r" );
				_nLastError = ERROR_SC_MESSAGEFACTORY_FULL;
				break;
			}

			*pRecord = _Records[nIndex];

			pBuffer = GetLocalPointer( nSize );
			nMemType = mem_LocalBuffer;

			if ( pBuffer == NULL )
			{
				pBuffer = _OverFlowAllocator.Allocate( nSize );
				nMemType = mem_Allocator;
			}

			if ( pBuffer == NULL )
			{
				scDebugManager::Instance()->Trace( 
								scDEBUGLABEL_ERROR_MESSAGE, 
								"scMessageFactory: Message too Large %u bytes.\n\r", nSize );
				_nLastError = ERROR_SC_MEMORY_ALLOCATION_FAILURE;
				*pRecord = NULL;
				break;
			}

			(*(*pRecord)->_pMessage) = IMessage( pBuffer, nSize );
			SetInUse( *pRecord, pBuffer, nSize, nMemType );
			// Success
		} while( false );	// error loop

		return *pRecord != NULL;
	}

	/// <summary>
	/// Scans through the allocation table and finds the larges block of memory available and
	/// returns the pointer to the start only if the block is large enough to hold the size
	/// of the message requested.
	/// </summary>
	template<class IMessage>
	uint8_t* scMessageFactory<IMessage>::GetLocalPointer( uint32_t nSize )
	{
		uint8_t*	pRangeStart = NULL;
		uint8_t*	pRangeEnd = NULL;
		uint8_t*	pStart = NULL;
		uint8_t*	pRecordBuffer = NULL;

		// figure out what part of the buffer is currently in use
		SlotVector_t::iterator itr = _Records.begin();
		for( ; itr != _Records.end(); itr++ )
		{
			if ( (*itr)->_nInUse > 0 && (*itr)->_BufferType == mem_LocalBuffer && (*itr)->_nSize > 0 )
			{
				pRecordBuffer = reinterpret_cast<uint8_t*>((*itr)->_pBuffer);
				if ( pRangeEnd == NULL || (pRecordBuffer < pRangeEnd) )
				{
					pRangeStart = pRecordBuffer;
				}
				if ( pRangeEnd == NULL || (pRecordBuffer + (*itr)->_nSize) > pRangeEnd )
				{
					pRangeEnd = (pRecordBuffer + (*itr)->_nSize );
				}
			}
		}

		// local data is available
		if ( pRangeStart == NULL || pRangeEnd == NULL )
		{
			pStart = _pMessageBuffer;
		}
		// check for space in the local message buffer
		else if ( (pRangeEnd + nSize) <= (_pMessageBuffer + _nMessageBufferSize) )
		{
			pStart = pRangeEnd;
		}
		// if the end of buffer can't be used try the start
		else if ( (pRangeStart - nSize) >= _pMessageBuffer )
		{
			// Good use this
			pStart = (pRangeStart - nSize);
		}
		else
		{
			// buffer is full.
			pStart = NULL;
		}

		return pStart;
	}

	/// <summary>
	/// Will obtain the number of message slots available
	/// </summary>
	template<class IMessage>
	uint16_t scMessageFactory<IMessage>::MessagesAvailable(void) const
	{
		assert_param( _pProtoect != NULL );
		scScopeLock						protect( _pProtoect );
		uint16_t						nCount = 0;
		SlotVector_t::const_iterator	itr = _Records.begin();

		for( ; itr != _Records.end(); itr++ )
		{
			if ( (*itr)->_nInUse == 0 )
			{
				nCount++;
			}
		}
		return nCount;
	}

	/// <summary>
	/// Will obtain the number of message slots in use.
	/// </summary>
	template<class IMessage>
	uint16_t scMessageFactory<IMessage>::MessagesInUse(void) const
	{
		assert_param( _pProtoect != NULL );
		scScopeLock							protect( _pProtoect );
		uint16_t							nCount = 0;
		SlotVector_t::const_iterator		itr = _Records.begin();

		for( ; itr != _Records.end(); itr++ )
		{
			if ( (*itr)->_nInUse > 0 )
			{
				nCount++;
			}
		}
		return nCount;
	}


}	// namespace SharedCore
#endif // !defined(__SCMESSAGEFACTORY_H__INCLUDED_)
