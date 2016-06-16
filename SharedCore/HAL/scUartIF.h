//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          All Rights Reserved
//
// File Name:		scUartIF.h
// Created By:		Christopher Snyder
// Creation Date:	11-Nov-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCUARTIF_H__INCLUDED_)
#define __SCUARTIF_H__INCLUDED_

#include "scTypes.h"
#include "scDeviceDescriptor.h"
#include "HAL/scBufferedIODriver.h"
#include "scIQueue.h"

namespace SharedCore
{
	namespace HAL
	{
		// Define UARTs enums that are used to cross reference the UART port number
		// to the device driver that will control this UART. Not all devices will support all
		// the UARTS. In some cases UARTs may actually share driver, but use different pin
		// configurations this is the reason for needing the cross reference to permit this UART
		// switching concept.
		typedef enum
		{
			scUART_1 = 0,
			scUART_2,
			scUART_3,
			scUART_4,
			scUART_5,
			scUART_6
		} scUartNum_t;
		
		
		/// <summary>
		/// This class defines the interface and some of the core functionality for the
		/// UART device driver. The main implementation will be a function pointer
		/// callbacks for the transmit and receive byte handlers which are called from the
		/// ISR routines. The ring buffers are used to hold the user data. The events are
		/// triggered when appropriate.
		/// </summary>
		class scUartIF : public scBufferedEventCBIODriver
		{
		public:
			scUartIF( const scDeviceDescriptor& dd );
			scUartIF( const scDeviceDescriptor& dd, const scDeviceDescriptor& dependant );

			virtual ~scUartIF();

			virtual void Initialize( scDeviceManager* pDm );

			/// <summary>
			/// Disable the UART. must be implemented in the platform imp.
			/// </summary>
			virtual void Disable() = 0;

			/// <summary>
			/// Disable the UART. must be implemented in the platform imp.
			/// </summary>
			virtual void Enable() = 0;

			/// <summary>
			/// Determines if the implementation has a receive task. Not all drivers require it.
			/// </summary>
			virtual const bool HasReceiveTask();

			/// <summary>
			/// Change the state of the RTS line manually. This must be implemented in the port.
			/// </summary>
			/// <param name="bEnable">true for the line to be set, false to not be set.</param>
			virtual void RTS(bool bEnable) = 0;
			
			/// <summary>
			/// Returns the current RTS state.
			/// </summary>
			virtual const bool RTS() = 0;

			/// <summary>
			/// Will validate the provided baud rate. Allows specific implementations to reject
			/// allowed values.
			/// </summary>
			/// <param name="nBaud">A baud rate to validate.</param>
			virtual bool ValidBaudRate(uint32_t nBaud);

			/// <summary>
			/// Get the current baud rate.
			/// </summary>
			virtual uint32_t Baud(void) const;

			/// <summary>
			/// Gets the overflow flag
			/// </summary>
			virtual bool Overflow(void) const;

			/// <summary>
			/// Clears the overflow flag.
			/// </summary>
			virtual void OverflowClear();
			
		protected:
			/// <summary>
			/// This is called by the receive monitor when the task begins.
			/// </summary>
			virtual void MonitorStartup(void);
			
			/// <summary>
			/// A static method that is the callback for the task implementations.
			/// </summary>
			/// <param name="pParams">This value should be the pointer to the UartIF
			/// implementation.</param>
			static void MonitorTask(void* pParams);

			scIQueue*		_pQueue;
		private:
			uint32_t		_nBaudRate;
		};

	}

}
#endif // !defined(__SCUARTIF_H__INCLUDED_)
