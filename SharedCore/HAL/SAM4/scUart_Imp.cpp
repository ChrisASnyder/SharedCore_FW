//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          All Rights Reserved
//
// File Name:		Uart_Imp.cpp
// Created By:		Christopher Snyder
// Creation Date:	28 June 2015
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include "scUart_Imp.h"

extern "C"
{
	#include "HAL/SAM4/asf/sysclk.h"
};


#ifndef assert_param
	#define assert_param(X)	if ( !(X) ) { for(;;) ; }
#endif

using namespace SharedCore::HAL;
using namespace SharedCore::HAL::SAM4;

// This macro should be defined in a board config, however, since the Atmel toolchain forces
// the use of libraries, it's difficult to chain macros through a config header. Later I will fix this
// using a scConf.h file
#ifndef SC_MAX_UARTS_SUPPORTED
	#define SC_MAX_UARTS_SUPPORTED	4
#endif

const int scUart_Imp::NumberOfUarts = SC_MAX_UARTS_SUPPORTED;

// Since this board has 5 UARTS there could be as many as 5 drivers.
scUart_Imp* scUart_Imp::_Drivers[NumberOfUarts] =
{
	NULL,
	#if (SC_MAX_UARTS_SUPPORTED > 1 )
	NULL,
	#endif
	#if (SC_MAX_UARTS_SUPPORTED > 2 )
	NULL,
	#endif
	#if (SC_MAX_UARTS_SUPPORTED > 3 )
	NULL,
	#endif
	#if (SC_MAX_UARTS_SUPPORTED > 4 )
	NULL,
	#endif
	#if (SC_MAX_UARTS_SUPPORTED > 5 )
	NULL,
	#endif
	#if (SC_MAX_UARTS_SUPPORTED > 6 )
	NULL,
	#endif
};

// To avoid stack depth callback and delays in the ISR, the interrupt vectors
// can be redirected to these functions directly.
extern "C"
{
	/// <summary>
	/// This is the ISR called by the vector. This should be references
	/// </summary>
	void UART0_Handler(void) { scUart_Imp::Global_Vector( scUART_1 ); }
	void UART1_Handler(void) { scUart_Imp::Global_Vector( scUART_2 ); }
	void USART0_Handler(void) { scUart_Imp::Global_Vector( scUART_3 ); }
	void USART1_Handler(void) { scUart_Imp::Global_Vector( scUART_4 ); }
}

/// <summary>
/// This is the ISR called by the STM library to indicate that an ISR event has happened
/// for the specified USART port. This will use the static array to route the handler
/// to the proper object that was constructed.
/// </summary>
void scUart_Imp::Global_Vector( scUartNum_t port )
{
	if ( port < NumberOfUarts && _Drivers[port] != NULL )
	{
		_Drivers[port]->ISR_Handler();
	}
}

/// <summary>
/// This is used to cross reference the UART enum with the port.
/// </summary>
Uart* scUart_Imp::GetPort(void) const
{
	switch( PortNumber() )
	{
		case scUART_1:
			return reinterpret_cast<Uart*>(UART0);
		case scUART_2:
			return reinterpret_cast<Uart*>(UART1);
		default:
			assert_param(FALSE);
			break;
	}
	return reinterpret_cast<Uart*>(UART0);
}


scUart_Imp::scUart_Imp( const scDeviceDescriptor& dd )
	: scUartIF( dd )
	, _pUsartCache(NULL)
{
	
}

scUart_Imp::scUart_Imp( const scDeviceDescriptor& dd, const scDeviceDescriptor& dependant )
	: scUartIF( dd, dependant )
	, _pUsartCache(NULL)
{
	
}

scUart_Imp::~scUart_Imp()
{
	
}

IRQn_Type scUart_Imp::GetIRQ(void) const
{
	IRQn_Type nIRQ = UART0_IRQn;
	switch( PortNumber() )
	{
		case scUART_1:
			nIRQ = UART0_IRQn;
			break;
		case scUART_2:
			nIRQ = UART1_IRQn;
			break;
		default:
			assert_param(FALSE);
			break;
	}
	return nIRQ;
}

/// <summary>
/// Setup the pointer to the UART memory address
/// </summary>
void scUart_Imp::Initialize( scDeviceManager* pDm )
{
	_pUsartCache = GetPort();
	
    if ( Receive_ByteHandler == NULL )
    {
	    // this must happen at least once.
		SetReceiveCB( DefaultRxHandler, this );
    }

    if ( Transmit_ByteHandler == NULL )
    {
		// this must happen at least once.
		SetTransmitCB( DefaultTxHandler, this );
    }

    // This registers this object as the handler for the specified UART
    scUartNum_t nUartPort = this->PortNumber();
    if ( nUartPort < NumberOfUarts && _Drivers[nUartPort] == NULL )
    {
	    _Drivers[nUartPort] = this;
    }
    scUartIF::Initialize(pDm);	
	
	uart_disable( _pUsartCache);
	
	Baud(115200);	// setup a starting rate.
	
	NVIC_EnableIRQ(GetIRQ());
	
	uart_enable(_pUsartCache);							// Enable UART1

	//uart_enable_interrupt( _pUsartCache, UART_IER_RXBUFF | UART_IER_TXBUFE);   //Interrupt reading ready
	//uart_enable_interrupt( _pUsartCache, UART_IER_TXBUFE);   //Interrupt reading ready
	//uart_enable_interrupt( _pUsartCache, UART_IER_RXRDY);
}

/// <summary>
/// Disable the UART. must be implemented in the platform imp.
/// </summary>
void scUart_Imp::Disable()
{
	assert_param(_pUsartCache != NULL );
	uart_disable( _pUsartCache );
}

/// <summary>
/// Disable the UART. must be implemented in the platform imp.
/// </summary>
void scUart_Imp::Enable()
{
	assert_param(_pUsartCache != NULL );
	//NVIC_EnableIRQ(GetIRQ());
	uart_enable( _pUsartCache );
}

/// <summary>
/// Determines if the implementation has a receive task. Not all drivers require it.
/// </summary>
const bool scUart_Imp::HasReceiveTask()
{
	return false;
}

/// <summary>
/// Change the state of the RTS line manually. This must be implemented in the port.
/// </summary>
/// <param name="bEnable">true for the line to be set, false to not be set.</param>
void scUart_Imp::RTS(bool bEnable)
{
	assert_param(_pUsartCache != NULL );
	// TODO
	// probably unimplemented in the subclass
}

/// <summary>
/// Returns the current RTS state.
/// </summary>
const bool scUart_Imp::RTS()
{
	// TODO
	return false;
}

/// <summary>
/// Will validate the provided baud rate. Allows specific implementations to reject
/// allowed values.
/// </summary>
/// <param name="nBaud">A baud rate to validate.</param>
bool scUart_Imp::ValidBaudRate(uint32_t nBaud)
{
	// call the parent class 
	// override any values that differ from the base class implementation.
	return scUartIF::ValidBaudRate(nBaud);
}

/// <summary>
/// Set the current baud rate.
/// </summary>
uint32_t scUart_Imp::Baud( uint32_t nRate )
{
	uint32_t nResult = -1;
	
	assert_param(_pUsartCache != NULL );

	if ( ValidBaudRate( nRate ) )
	{
		sam_uart_opt_t	initStruct;
		initStruct.ul_baudrate = nRate;
		bool bUseMe = true;
		if ( bUseMe )
		{
			initStruct.ul_mck = sysclk_get_cpu_hz();			
		}
		else
		{
			//240000000UL  UART_SERIAL_MCK
			initStruct.ul_mck = CHIP_FREQ_CPU_MAX;
		}
		initStruct.ul_mode = UART_MR_CHMODE_NORMAL | UART_MR_PAR_NO;	
		//initStruct.ul_mode = UART_MR_CHMODE_LOCAL_LOOPBACK;
		
		// Init UART1
		if ( uart_init( _pUsartCache, &initStruct ) == 0 )
		{
			nResult = Baud();
		}
		// what to do if failure?
	}
	return nResult;
}

/// <summary>
/// Get the current baud rate.
/// </summary>
uint32_t scUart_Imp::Baud(void) const
{
	uint32_t cd = _pUsartCache->UART_BRGR;
	uint32_t nBaud = 0; /* _pUsartCache->ul_mck * cd * UART_MCK_DIV */
	return nBaud;
}

/// <summary>
/// Gets the overflow flag
/// </summary>
bool scUart_Imp::Overflow(void) const
{
	return false;
}

/// <summary>
/// Clears the overflow flag.
/// </summary>
void scUart_Imp::OverflowClear()
{
	
}

void scUart_Imp::ISR_Handler(void)
{
	assert_param( _pUsartCache != NULL );
	uint32_t dw_imr = uart_get_interrupt_mask(_pUsartCache);
	uint32_t dw_status = uart_get_status(_pUsartCache);
	
	//NVIC_ClearPendingIRQ(GetIRQ());
	
	// First check for uart overrun error
	if ( dw_imr & UART_IMR_OVRE )
	{
		
	}
	// Next check for the framing error
	if ( dw_imr & UART_IMR_FRAME )
	{
		
	}
	// next check for the parity error
	if ( dw_imr & UART_IMR_PARE )
	{
		
	}
	// RX ready interrupt
	if ( dw_imr & UART_IMR_RXRDY )
	{
	}

	// RXBUFF interrupt
	if ( dw_imr & UART_IMR_RXBUFF && dw_status & UART_SR_RXBUFF )
	{
		Receive_ByteHandler( _pUsartCache->UART_RHR, _pRxCallbackContext );
	}

	// end of receiver transfer interrupt
	if ( dw_imr & UART_IMR_ENDRX )
	{
		
	}
	
	// of of transmit interrupt
	if ( dw_imr & UART_IMR_ENDTX )
	{
		
	}

	
	// TX ready interrupt
	if ( dw_imr & UART_IMR_TXRDY && dw_status & UART_SR_TXRDY )
	{
		// Used to get the transmit running.
		Transmit_ByteHandler( _pUsartCache, _pTxCallbackContext );
	}

	// TX Empty intrrupt
	if ( dw_imr & UART_IMR_TXEMPTY && dw_status & UART_SR_TXEMPTY )
	{
		//Transmit_ByteHandler( _pUsartCache, _pTxCallbackContext );
	}
	
	// TXBUFE interrupt
	if ( dw_imr & UART_IMR_TXBUFE && dw_status & UART_SR_TXBUFE )
	{

	}
}

void scUart_Imp::TriggerSend(void)
{
	// TODO	
	_pQueueOut->Lock();
	uint32_t dw_imr = uart_get_interrupt_mask(_pUsartCache);
	if ( uart_is_tx_buf_end( _pUsartCache ) && ( (dw_imr & UART_IMR_TXRDY ) == 0 ) )
	{
		if ( _pQueueOut->ReadStart() > 0 )
		{
			uart_enable_interrupt( _pUsartCache, UART_IER_TXRDY);
			//uart_write( _pUsartCache , *_pQueueOut->ReadBlock() );
			uart_enable_tx( _pUsartCache );
			_pQueueOut->ReadEnd(0);
		}
	}

	_pQueueOut->Unlock();
}

void scUart_Imp::DefaultTxHandler( void* pPort, void* pContext )
{
	if ( pContext == NULL ) return;

	scUart_Imp* pSelf = reinterpret_cast<scUart_Imp*>(pContext);

	// There is no mutex call here because the mutex is simply
	// disabling this ISR from triggering. The only mutex needed
	// is to prevent the ISR from going off when the other tasks
	// are accessing the _pQueue objects.
	if ( pSelf->_pQueueOut->ReadStart() > 0 )
	{
		uart_write( reinterpret_cast<Uart*>(pPort), *pSelf->_pQueueOut->ReadBlock() );
		pSelf->_pQueueOut->ReadEnd(1);
	}
	else
	{
		// Stop the TX interrupt from triggering since all bytes have
		// been sent.
		uart_disable_tx(reinterpret_cast<Uart*>(pPort) );
		uart_disable_interrupt( pSelf->_pUsartCache, UART_IER_TXRDY);
		//uart_disable_interrupt( pSelf->_pUsartCache, UART_IER_TXBUFE );   //Interrupt reading ready
	}
}

void scUart_Imp::DefaultRxHandler( uint8_t nByte, void* pContext )
{
	if ( pContext == NULL ) return;

	scUart_Imp* pSelf = reinterpret_cast<scUart_Imp*>(pContext);

#if(0)
{
	if ( pSelf->_pQueueIN != NULL )
	{
		// This method occurs in an ISR so we do not need to disable the
		// ISR to protect the buffer since the protection is only to disable
		// this ISR routine from triggering while access to this memory is
		// occurring.
		uint32_t limit = pSelf->_pQueueIN->WriteStart();
		// The limit value is the number of continuous bytes that can be written
		// when it reaches 1 we are at the end of the FIFO and will wrap around
		if ( limit > 0 )
		{
			*(pSelf->_pQueueIN->WriteBlock()) = (uint8_t)nByte;
			pSelf->_pQueueIN->WriteEnd(1);

			if ( pSelf->_bUseTriggerChar )
			{
				if ( nByte == pSelf->_nTriggerOff )
				{
					pSelf->_nTriggerCurrent = 0;
				}
				else if ( nByte == pSelf->_nTriggerOn )
				{
					pSelf->_nTriggerCurrent = 1;
				}

				if ( pSelf->_nTriggerCurrent > 0 )
				{
					pSelf->SignalFromISR(BufferedIODriverISR::ReceiveData);
				}
			}
			else
			{
				// Notify the incoming monitor task that data
				// has arrived.
				++pSelf->_nTriggerCurrent;
				if ( pSelf->_nTriggerCurrent >= pSelf->_nTriggerMark )
				{
					pSelf->_nTriggerCurrent = 0;
					pSelf->SignalFromISR(BufferedIODriverISR::ReceiveData);
				}
			}

			// When the limit is 1, that means we just used the last byte in the
			// incoming buffer and now we must do something to stop the incoming
			// flow of data.
			if ( pSelf->_pQueueIN->Available() <= pSelf->_nRTSThreashold )
			{
				// Set the RTS to high
				pSelf->RTS(true);
				pSelf->SignalFromISR(BufferedIODriverISR::ReceiveData);
			}
		}
		else
		{
			// Set the RTS to high
			// the FIFO is full
			pSelf->RTS(true);
			pSelf->SignalFromISR(BufferedIODriverISR::Overflow);
		}
	}
}
#endif
}

