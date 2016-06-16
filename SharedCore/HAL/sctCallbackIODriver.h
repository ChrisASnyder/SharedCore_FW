//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          All Rights Reserved
//
// File Name:		sctCallbackIODriver.h
// Created By:		Christopher Snyder
// Creation Date:	25-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCTCALLBACKIODRIVER_H__INCLUDED_)
#define __SCTCALLBACKIODRIVER_H__INCLUDED_

namespace SharedCore
{
	namespace HAL
	{
		/// <summary>
		/// A driver implementation that will provide the callbacks for a driver
		/// </summary>
		template<class Base_T>
		class sctCallbackIODriver : public Base_T
		{
		public:
			virtual ~sctCallbackIODriver();

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
			/// This method will set the callback method used for the transmit data
			/// functionality.
			/// </summary>
			/// <param name="void (*pTransmit_ByteHandler)( uint8_t* nByte, void*
			/// pContext)">Receive callback handler</param>
			/// <param name="pContext">Context pointer.</param>
			virtual void SetTransmitCB(void (*pTransmit_ByteHandler)( void* pPort, void* pContext), void* pContext);

			/// <summary>
			/// This method will set the callback method used for the receive data
			/// functionality.
			/// </summary>
			/// <param name="void (*pReceive_ByteHandler)( uint8_t* nByte, void*
			/// pContext)">Receive callback handler</param>
			/// <param name="pContext">Context pointer.</param>
			virtual void SetReceiveCB(void (*pReceive_ByteHandler)( uint8_t nByte, void* pContext), void* pContext);

		protected:
			/// <summary>
			/// Construct the class using the Guid
			/// </summary>
			/// <param name="descriptor">Describes the descriptor for this device.</param>
			sctCallbackIODriver(const scDeviceDescriptor& descriptor);

			/// <summary>
			/// Construct the class using the Guid and multiple dependants
			/// </summary>
			/// <param name="descriptor">Describes the descriptor for this device.</param>
			/// <param name="nCount">Number of dependants listed.</param>
			sctCallbackIODriver(const scDeviceDescriptor& descriptor, int nCount, ... );

			/// <summary>
			/// Construct the class using the Guid and one dependant
			/// </summary>
			/// <param name="descriptor">Describes the descriptor for this device.</param>
			/// <param name="dependant">the sole dependant of the device.</param>
			sctCallbackIODriver(const scDeviceDescriptor& descriptor, const scDeviceDescriptor& dependant);

			void*   _pRxCallbackContext;
			void*   _pTxCallbackContext;

			void (*Receive_ByteHandler)( uint8_t nByte, void* pContext );
			void (*Transmit_ByteHandler)( void* pPort, void* pContext );

		};


//////////////////////////////////////////////////////////////////////////////////////
/// Public Methods
//////////////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Destructor. This will use the internal allocator to release all the memory used.
		/// </summary>
		template<class Base_T>
		sctCallbackIODriver<Base_T>::~sctCallbackIODriver()
		{
		}

		/// <summary>
		/// This will configure the callback method and the context pointer
		/// </summary>
		template<class Base_T>
		void sctCallbackIODriver<Base_T>::SetReceiveCB( void (*pReceive_ByteHandler)( uint8_t nByte, void* pContext ), void* pContext )
		{
			_pRxCallbackContext = pContext;
			Receive_ByteHandler = pReceive_ByteHandler;
		}

		/// <summary>
		/// This will configure the callback method and the context pointer
		/// </summary>
		template<class Base_T>
		void sctCallbackIODriver<Base_T>::SetTransmitCB( void (*pTransmit_ByteHandler)( void* pPort, void* pContext ), void* pContext )
		{
			_pTxCallbackContext = pContext;
			Transmit_ByteHandler = pTransmit_ByteHandler;
		}

//////////////////////////////////////////////////////////////////////////////////////
/// Protected Methods
//////////////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Construct the class using the Guid
		/// </summary>
		/// <param name="descriptor">Describes the descriptor for this device.</param>
		template<class Base_T>
		sctCallbackIODriver<Base_T>::sctCallbackIODriver(const scDeviceDescriptor& descriptor)
			: Base_T( descriptor )
			, _pRxCallbackContext(NULL)
			, _pTxCallbackContext(NULL)
			, Receive_ByteHandler(NULL)
			, Transmit_ByteHandler(NULL)
		{
		}

		/// <summary>
		/// Construct the class using the Guid and multiple dependants
		/// </summary>
		/// <param name="descriptor">Describes the descriptor for this device.</param>
		/// <param name="nCount">Number of dependants listed.</param>
		/// <param name="prm1"></param>
		template<class Base_T>
		sctCallbackIODriver<Base_T>::sctCallbackIODriver(const scDeviceDescriptor& descriptor, int nCount, ... )
			: Base_T( descriptor )
			, _pRxCallbackContext(NULL)
			, _pTxCallbackContext(NULL)
			, Receive_ByteHandler(NULL)
			, Transmit_ByteHandler(NULL)
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
		sctCallbackIODriver<Base_T>::sctCallbackIODriver(const scDeviceDescriptor& descriptor, const scDeviceDescriptor& dependant)
			: Base_T( descriptor, dependant )
			, _pRxCallbackContext(NULL)
			, _pTxCallbackContext(NULL)
			, Receive_ByteHandler(NULL)
			, Transmit_ByteHandler(NULL)
		{
		}
	}

}
#endif // !defined(__SCTCALLBACKIODRIVER_H__INCLUDED_)
