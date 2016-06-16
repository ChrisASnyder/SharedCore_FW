//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          All Rights Reserved
//
// File Name:		scLedIF.h
// Created By:		Christopher Snyder
// Creation Date:	25-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include "scLedIF.h"
#include "scErrorCodes.h"

using SharedCore::HAL::scLedIF;
using namespace SharedCore;

/// <summary>
/// Default constructor.
/// </summary>
scLedIF::~scLedIF()
{
	vector<const LedDescriptor_t*>::iterator itr = _Leds.begin();
	for( ; itr != _Leds.end(); ++itr )
	{
		delete (*itr);
	}
}

/// <summary>
/// Constructor as protected since this class must be sub classed.
/// </summary>
/// <param name="dd">The descriptor for the driver.</param>
scLedIF::scLedIF( const scDeviceDescriptor& dd )
	: scDeviceGeneric( dd )
	, _Leds()
{
}

/// <summary>
/// Turn the Led on or off.
/// </summary>
/// <param name="nLedID">The common name identifier for the LED being controlled.
/// </param>
/// <param name="nState">The desired state of the LED</param>
void scLedIF::Illuminate(uint32_t nLedID, LedState_t nState)
{
	vector<const LedDescriptor_t*>::const_iterator itr = _Leds.begin();
	for( ; itr != _Leds.end(); ++itr )
	{
		if ( (*itr)->_nLedId == nLedID )
		{
			ChangeState( (*itr), nState );
			break;
		}
	}
}

/// <summary>
/// Adds a new LED definition to the list of known LED's. Sub classed drivers
/// should call this method during the initialize call to add all the known ports
/// to the driver.
/// </summary>
/// <param name="nLed">The identifier for the led being added.</param>
/// <param name="pPort">generic pointer for the port identifier</param>
/// <param name="nPin">the pin being used for the led on the specified port.</param>
uint32_t scLedIF::Add( uint32_t nLed, void* pPort, uint16_t nPin )
{
	LedDescriptor_t* pItem = new LedDescriptor_t;
	pItem->_nLedId = nLed;
	pItem->_pPort = pPort;
	pItem->_nPin = nPin;
	pItem->_bInvert = false;
	return Add( pItem );
}

/// <summary>
/// Adds a new LED definition to the list of known LED's. Sub classed drivers
/// should call this method during the initialize call to add all the known ports
/// to the driver.
/// </summary>
/// <param name="nLed">The identifier for the led being added.</param>
/// <param name="pPort">generic pointer for the port identifier</param>
/// <param name="nPin">the pin being used for the led on the specified port.
/// </param>
/// <param name="bInverted">Depending on the hardware configuration, the logic for
/// an illuminated pin may be inverted.</param>
uint32_t scLedIF::Add(uint32_t nLed, void* pPort, uint16_t nPin, bool bInverted)
{
	LedDescriptor_t* pItem = new LedDescriptor_t;
	pItem->_nLedId = nLed;
	pItem->_pPort = pPort;
	pItem->_nPin = nPin;
	pItem->_bInvert = bInverted;
	return Add( pItem );
}

/// <summary>
/// This is a vector of all the LED's defined in the system.
/// </summary>
/// <param name="pItem"></param>
uint32_t scLedIF::Add( const LedDescriptor_t* pItem )
{
	uint32_t nResult = ERROR_SUCCESS;
	bool bFound = false;
	vector<const LedDescriptor_t*>::iterator itr = _Leds.begin();
	for( ; !bFound && itr != _Leds.end(); ++itr )
	{
		bFound = (*itr)->_nLedId == pItem->_nLedId;
	}

	if ( !bFound )
	{
		_Leds.push_back( pItem );
	}
	else
	{
		nResult = ERROR_SC_ITEM_ALREADY_EXISTS;
	}
	return nResult;
}




/// <summary>
/// Returns the number of  LED's known to the driver.
/// </summary>
uint32_t scLedIF::Count(void) const
{
	return _Leds.size();
}

/// <summary>
/// Gets the LED descriptor assigned to the index.
/// </summary>
/// <param name="nIndex">The array index.</param>
uint32_t scLedIF::GetId( uint32_t nIndex ) const
{
	uint32_t nResult = 0;
	if ( nIndex < _Leds.size() )
	{
		nResult = _Leds[nIndex]->_nLedId;
	}

	return nResult;
}
