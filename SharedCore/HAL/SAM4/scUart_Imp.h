//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          All Rights Reserved
//
// File Name:		Uart_Imp.h
// Created By:		Christopher Snyder
// Creation Date:	28 June 2015
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCUART_IMP_H_)
#define __SCUART_IMP_H_

#include "scTypes.h"
#include "scDeviceDescriptor.h"
#include "HAL/scUartIF.h"

extern "C"
{
	#include "asf/uart.h"
};

namespace SharedCore
{
	namespace HAL
	{
		namespace SAM4
		{
			class scUart_Imp : public scUartIF
			{
			public:
				scUart_Imp( const scDeviceDescriptor& dd );
				scUart_Imp( const scDeviceDescriptor& dd, const scDeviceDescriptor& dependant );

				virtual ~scUart_Imp();

				virtual void Initialize( scDeviceManager* pDm );

				/// <summary>
				/// Disable the UART. must be implemented in the platform imp.
				/// </summary>
				virtual void Disable();

				/// <summary>
				/// Disable the UART. must be implemented in the platform imp.
				/// </summary>
				virtual void Enable();

				/// <summary>
				/// Determines if the implementation has a receive task. Not all drivers require it.
				/// </summary>
				virtual const bool HasReceiveTask();

				/// <summary>
				/// Change the state of the RTS line manually. This must be implemented in the port.
				/// </summary>
				/// <param name="bEnable">true for the line to be set, false to not be set.</param>
				virtual void RTS(bool bEnable);
			
				/// <summary>
				/// Returns the current RTS state.
				/// </summary>
				virtual const bool RTS();

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
				/// Set the current baud rate.
				/// </summary>
				virtual uint32_t Baud( uint32_t nRate );
				
				/// <summary>
				/// Gets the overflow flag
				/// </summary>
				virtual bool Overflow(void) const;

				/// <summary>
				/// Clears the overflow flag.
				/// </summary>
				virtual void OverflowClear();

				/// <summary>
				/// This is the ISR called by the vector. This should be references
				/// </summary>
				static void Global_Vector( scUartNum_t port );

			protected:
				static const int					NumberOfUarts;

				void ISR_Handler(void);

				/// <summary>
				/// This will obtain the enum that is used to cross reference the port number. this
				/// is needed because the there are many cross references that are needed to initialize
				/// and identify the USART for the micro
				/// </summary>
				/// <returns>This is the enum describing which UART is used.</returns>
				virtual scUartNum_t	PortNumber(void) const = 0;
				
				void TriggerSend(void);
			private:

				static void DefaultTxHandler( void* pPort, void* pContext );
				static void DefaultRxHandler( uint8_t nByte, void* pContext );
				
				/// <summary>
				/// This is used to cross reference the UART enum with the port.
				/// </summary>
				Uart* GetPort(void) const;
				IRQn_Type GetIRQ(void) const;
				/// <summary>
				/// Since there is only one ISR for all the different USARTs in the system this static
				/// pointer list is used by the ISR handler to route the ISR routine to the proper
				/// object to handle the event.
				/// </summary>
				static scUart_Imp*			_Drivers[];
				
				Uart*						_pUsartCache;
			};
		}
	}
}


#endif /* __UART_IMP_H_ */