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


#include "scUartIF.h"

#ifndef assert_param
#define assert_param(X)	if ( !(X) ) { for(;;) ; }
#endif

using namespace SharedCore;
using namespace SharedCore::HAL;

/// <summary>
/// Class Destructor for scUartIF
/// </summary>
scUartIF::scUartIF( const scDeviceDescriptor& dd )
	: scBufferedEventCBIODriver(dd)
{

}

scUartIF::scUartIF( const scDeviceDescriptor& dd, const scDeviceDescriptor& dependant )
	: scBufferedEventCBIODriver(dd, dependant)
{
}

/// <summary>
/// Class constructor for scUartIF
/// </summary>
scUartIF::~scUartIF()
{

}

void scUartIF::Initialize( scDeviceManager* pDm )
{
}

/// <summary>
/// Determines if the implementation has a receive task. Not all drivers require it.
/// </summary>
const bool scUartIF::HasReceiveTask()
{
	return true;
}

/// <summary>
/// Get the current baud rate.
/// </summary>
uint32_t scUartIF::Baud(void) const
{
	return _nBaudRate;
}

/// <summary>
/// Gets the overflow flag
/// </summary>
bool scUartIF::Overflow(void) const
{
	return false;
}

/// <summary>
/// Clears the overflow flag.
/// </summary>
void scUartIF::OverflowClear(void)
{
}

/// <summary>
/// This is called by the receive monitor when the task begins.
/// </summary>
void scUartIF::MonitorStartup()
{
}

/// <summary>
/// A static method that is the callback for the task implementations.
/// </summary>
/// <param name="pParams">This value should be the pointer to the UartIF
/// implementation.</param>
void scUartIF::MonitorTask(void* pParams)
{
	// This method runs forever
	scUartIF* pSelf = reinterpret_cast<scUartIF*>(pParams);
	scDataEvents_t reason;

	pSelf->MonitorStartup();
	assert_param( pSelf->_pQueue != NULL );

	for( ;; )
	{
		if ( pSelf->_pQueue->Receive( &reason, pSelf->_nIdleTimeout ) > 0 )
		{
			pSelf->_event.FireEvent( reason );
		}
		else
		{
			pSelf->_event.FireEvent( scIdle );
		}
	}
}


/// <summary>
/// Will validate the provided baud rate. Allows specific implementations to reject
/// allowed values. These are the typical baud rates that are supported. Subclassed 
/// should use chain of responsibility to override values being added or rejected.
/// </summary>
/// <param name="nBaud">A baud rate to validate.</param>
bool scUartIF::ValidBaudRate(uint32_t nBaud)
{
	bool bValid = false;

	switch( nBaud )
	{
	case    2400:
	case    9600:
	case   19200:
	case   28800:
	case   57600:
	case  115200:
	case  230400:
	case  460800:
	case  921600:
	case 2250000:
	case 2304000:
		bValid = true;
		break;
	default:
		bValid = false;
	}
	return bValid;
}

