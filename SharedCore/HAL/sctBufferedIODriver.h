//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          All Rights Reserved
//
// File Name:		sctBufferedIODriver.h
// Created By:		Christopher Snyder
// Creation Date:	25-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCTBUFFEREDIODRIVER_H__INCLUDED_)
#define __SCTBUFFEREDIODRIVER_H__INCLUDED_

#include <string.h>
#include "scRingBuffer.h"
#include "scErrorCodes.h"

#ifndef assert_param
#define assert_param(X)	if ( !(X) ) { for(;;) ; }
#endif

namespace SharedCore
{
	namespace HAL
	{
		/// <summary>
		/// Provides the driver implementation for the use of a ring buffer. Allows the
		/// setting and assignment of the buffer to use. Access to these buffers are
		/// provided through the send_n and recv_n functions.
		/// </summary>
		template<class Base_T >
		class sctBufferedIODriver : public Base_T
		{
		public:
			virtual ~sctBufferedIODriver();

			/// <summary>
			/// Perform one time initialization activity. This is called by the Device Manager
			/// during the application startup and this method is only ever called once. The
			/// Configure/Unconfigure methods are used to change the resource usage.
			/// </summary>
			/// <param name="pDm"></param>
			virtual void Initialize(scDeviceManager* pDm)
			{
				Base_T::Initialize( pDm );
			}

			/// <summary>
			/// Used to initialize the data queues used by the system
			/// </summary>
			/// <param name="pQueueIn">This is the pointer to DataQueue used to manage the
			/// incoming data.</param>
			/// <param name="pQueueOut">Used to manage the buffered outgoing data. This may be
			/// NULL</param>
			/// <param name="pMutex">This is an optional parameter that may be used for buffer
			/// protection for the situations where it is required.</param>
			void SetQueue( scRingBuffer* pQueueIn, scRingBuffer* pQueueOut );

			uint32_t Send_n( const uint8_t* pBuffer, uint32_t nLength );

			uint32_t Recv_n( uint8_t* pBuffer, uint32_t nLength );

			/// <summary>
			/// Returns true if the driver entered an overflow condition, false if it is not in overflow.
			/// </summary>
			virtual bool GetOverflow(void) const 
			{ return _bOverflow; }

			/// <summary>
			/// When a send is attempted and the buffer does not have enough room to hold the
			/// data to be sent this method will be called to indicate an overflow condition.
			/// The send buffer will get reset.
			/// </summary>
			/// <param name="bSetOverflow"></param>
			virtual void SetOverflow( bool bSetOverflow )
			{ _bOverflow = bSetOverflow; }

		protected:
			/// <summary>
			/// Construct the class using the Guid
			/// </summary>
			/// <param name="descriptor">Describes the descriptor for this device.</param>
			sctBufferedIODriver(const scDeviceDescriptor& descriptor);

			/// <summary>
			/// Construct the class using the Guid and multiple dependants
			/// </summary>
			/// <param name="descriptor">Describes the descriptor for this device.</param>
			/// <param name="nCount">Number of dependants listed.</param>
			sctBufferedIODriver(const scDeviceDescriptor& descriptor, int nCount, ... );

			/// <summary>
			/// Construct the class using the Guid and one dependant
			/// </summary>
			/// <param name="descriptor">Describes the descriptor for this device.</param>
			/// <param name="dependant">the sole dependant of the device.</param>
			sctBufferedIODriver(const scDeviceDescriptor& descriptor, const scDeviceDescriptor& dependant);

			/// <summary>
			/// Signal to the child class to start transmitting data in the send buffer
			/// </summary>
			virtual void TriggerSend(void) = 0;

		protected:
			/// <summary>
			/// The queue for the incoming data.
			/// </summary>
			scRingBuffer*		_pQueueIn;

			/// <summary>
			/// The queue for the outgoing data.
			/// </summary>
			scRingBuffer*		_pQueueOut;

			/// <summary>
			/// Flag that is set when the transmit buffer is too full. the send method will hold
			/// until the device frees some data.
			/// </summary>
			bool				_bOverflow;
		};

//////////////////////////////////////////////////////////////////////////////////////
/// Public Methods
//////////////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Destructor. This will use the internal allocator to release all the memory used.
		/// </summary>
		template<class Base_T>
		sctBufferedIODriver<Base_T>::~sctBufferedIODriver()
		{
			delete _pQueueIn;
			delete _pQueueOut;
		}

		template<class Base_T>
		uint32_t sctBufferedIODriver<Base_T>::Send_n( const uint8_t* pBuffer, uint32_t nLength )
		{
			uint32_t overflow = 10000;
			while( nLength > 0 && overflow > 0 && _pQueueOut != NULL )
			{
				_pQueueOut->Lock();
				uint32_t nLimit = _pQueueOut->WriteStart();
				if ( nLimit == 0 )
				{
					// this will let the ISR trigger to clear data from the send buffer provided
					// we are not getting called from an ISR. IF we are in an ISR this will not
					// work and the program will get stuck here.
					_pQueueOut->Unlock();
					overflow--;
					_pQueueOut->Lock();
				}
				else if ( nLimit >= nLength )
				{
					nLimit = _pQueueOut->WriteBlock( pBuffer, nLength );
					nLength = 0;
				}
				else
				{
					nLimit = _pQueueOut->WriteBlock( pBuffer, nLimit );
					nLength -= nLimit;
					pBuffer += nLimit;
				}
				_pQueueOut->Unlock();
			}

			if ( overflow == 0 )
			{
				Base_T::SetLastError( ERROR_SC_BUFFER_OVERFLOW );
				_bOverflow = true;
			}

			TriggerSend();
			return Base_T::GetLastError();
		}

		template<class Base_T>
		uint32_t sctBufferedIODriver<Base_T>::Recv_n( uint8_t* pBuffer, uint32_t nLength )
		{
			uint32_t nResult = 0;

			if ( _pQueueIn != NULL )
			{
				_pQueueIn->Lock();
				while( _pQueueIn->InUse() > 0 && nLength > 0 )
				{
					uint32_t nFirstCopy = _pQueueIn->ReadStart();
					if ( nFirstCopy > nLength )
					{
						nFirstCopy = nLength;
					}
					memcpy( pBuffer, _pQueueIn->ReadBlock(), nFirstCopy );
					nResult+= nFirstCopy;
					nLength-= nFirstCopy;
					_pQueueIn->ReadEnd( nFirstCopy );
				}
				_pQueueIn->Unlock();
			}
			return nResult;
		}

		
		/// <summary>
		/// Used to initialize the data queues used by the system
		/// </summary>
		/// <param name="pQueueIn">This is the pointer to DataQueue used to manage the
		/// incoming data.</param>
		/// <param name="pQueueOut">Used to manage the buffered outgoing data. This may be
		/// NULL</param>
		/// <param name="pMutex">This is an optional parameter that may be used for buffer
		/// protection for the situations where it is required.</param>
		template<class Base_T>
		void sctBufferedIODriver<Base_T>::SetQueue( scRingBuffer* pQueueIn, scRingBuffer* pQueueOut )
		{
			_pQueueIn = pQueueIn;
			_pQueueOut = pQueueOut;
		}

//////////////////////////////////////////////////////////////////////////////////////
/// Protected Methods
//////////////////////////////////////////////////////////////////////////////////////


		/// <summary>
		/// Construct the class using the Guid
		/// </summary>
		/// <param name="descriptor">Describes the descriptor for this device.</param>
		template<class Base_T>
		sctBufferedIODriver<Base_T>::sctBufferedIODriver(const scDeviceDescriptor& descriptor)
			: Base_T( descriptor )
			, _pQueueIn(NULL)
			, _pQueueOut(NULL)
			, _bOverflow(false)
		{
		}

		/// <summary>
		/// Construct the class using the Guid and multiple dependants
		/// </summary>
		/// <param name="descriptor">Describes the descriptor for this device.</param>
		/// <param name="nCount">Number of dependants listed.</param>
		/// <param name="prm1"></param>
		template<class Base_T>
		sctBufferedIODriver<Base_T>::sctBufferedIODriver(const scDeviceDescriptor& descriptor, int nCount, ... )
			: Base_T( descriptor )
			, _pQueueIn(NULL)
			, _pQueueOut(NULL)
			, _bOverflow(false)
		{
			va_list vl;
			va_start( vl, nCount );
			Base_T::AddDependants( vl, nCount );
			va_end(vl);
		}

		/// <summary>
		/// Construct the class using the Guid and one dependant
		/// </summary>
		/// <param name="descriptor">Describes the descriptor for this device.</param>
		/// <param name="dependant">the sole dependant of the device.</param>
		template<class Base_T>
		sctBufferedIODriver<Base_T>::sctBufferedIODriver(const scDeviceDescriptor& descriptor, const scDeviceDescriptor& dependant)
			: Base_T( descriptor, dependant )
			, _pQueueIn(NULL)
			, _pQueueOut(NULL)
			, _bOverflow(false)
		{
		}


	}
}
#endif // !defined(__SCTBUFFEREDIODRIVER_H__INCLUDED_)
