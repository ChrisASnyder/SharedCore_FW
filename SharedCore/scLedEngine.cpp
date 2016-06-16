//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//          � Copyright Common Ground Software Solutions 2014
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
// File Name:		scLedEngine.h
// Created By:		Christopher Snyder
// Creation Date:	10-Nov-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include "scLedEngine.h"
#include "scScopeLock.h"

#ifndef assert_param
#define assert_param(X)	if ( !(X) ) { for(;;) ; }
#endif


using namespace SharedCore;

#define _P				(1)
#define P_SHORT			(5 * _P)
#define P_LONG			(10 * _P)
#define P_NONE			(0 * _P)
#define P_QUICK			(2 * _P)

// Alternating on and off delay times is scaled by the SLEEP delay const
// this matrix is scaled, adjust multiplier if the sleep delay is different.
static const uint8_t g_pDefaulPatterns[] = {
	//	ON			OFF			ON			OFF			ON			OFF			ON			OFF
	P_SHORT,	P_LONG,		P_SHORT,	P_LONG,		P_SHORT,	P_LONG,		P_SHORT,	P_LONG,			// heartbeat, typical
	P_QUICK,	P_QUICK,	P_QUICK,	P_QUICK,	P_QUICK,	P_QUICK,	P_QUICK,	P_QUICK,		// fast flicker
	P_LONG,		P_LONG,		P_LONG,		P_LONG,		P_LONG,		P_LONG,		P_LONG,		P_LONG,			// slow blink
	P_LONG,		P_LONG,		P_LONG,		P_LONG,		P_LONG,		P_LONG,		P_LONG,		0,				// slow then off
};

#define PATTERN_COLS		8
#define PATTERN_ROWS		3
// for some reason the intellisense has trouble with this #undef.
//#undef _P

//lint -e751    // ignore the enums that are not referenced info
//lint -e747    // it's a result of FreeRTOS
//
// Begin State Machine Definition Block
//
// State Machine Nodes
typedef enum
{
	State_Standby_Off       =   0,
	State_AlwaysOn          =   1,
	State_AlwaysOff         =   2,
	State_FlashOn           =   3,
	State_FlashOff          =   4,
	State_FlashOnToStandby  =   5,
	State_Disabled          =   6
} StateNodes_t;

// State Machine Inputs
typedef enum
{
	Input_Standby   =   0,
	Input_TurnOn    =   1,
	Input_TurnOff   =   2,
	Input_Flash     =   3,
	Input_FlashOff  =   4,
	Input_Disable   =   5,
	Input_Enable    =   6
} StateInputs_t;
//
// End State Machine Definition Block

//
// State Machine Definition Block
//    Generated by SMBuilder
//
static const int g_NumberInputs = 7;
static const int g_NumberStates = 7;
static const int g_InitialState = 0;
static const int g_TransitionMatrix[] = {
	//     I n p u t s ->
	// 0  1  2  3  4  5  6
	0, 1, 2, 3, 0, 6, 0, // State_Standby_Off
	0, 1, 2, 1, 1, 6, 1, // State_AlwaysOn
	0, 1, 2, 2, 2, 6, 2, // State_AlwaysOff
	5, 1, 2, 3, 4, 6, 3, // State_FlashOn
	0, 1, 2, 3, 4, 6, 4, // State_FlashOff
	5, 1, 2, 5, 0, 6, 5, // State_FlashOnToStandby
	6, 6, 6, 6, 6, 6, 0  // State_Disabled`
};

/// <summary>
/// Default constructor. Sets up the table.
/// </summary>
scLedEngine::scLedEngine( scIMutex* pMutex )
	: _hLock(pMutex)
	, _pLedImp(NULL)
	, _pPatterns(g_pDefaulPatterns)
	, _nPatternRows(PATTERN_ROWS)
	, _nPatternCols(PATTERN_COLS)
	, _LedList()
{
}

/// <summary>
/// Destructor for the class. Clean up allocated resources.
/// </summary>
scLedEngine::~scLedEngine()
{
	_pLedImp = NULL;	// don't own memory
	_hLock = NULL;
	_pPatterns = NULL;

	vector<scLedEngineDescriptor_t*>::iterator	itr = _LedList.begin();
	for( ; itr != _LedList.end(); itr++ )
	{
		assert_param( (*itr) != NULL );
		delete (*itr)->_pSM;
		delete (*itr);
		(*itr) = NULL;
	}
}

/// <summary>
/// Initializes the engine.
/// </summary>
void scLedEngine::Initialize( scLedIF* pLedIF )
{
	_pLedImp = pLedIF;

	assert_param( _pLedImp != NULL );
	// Now add all the known indicators to the manage list.
	for( uint32_t i=0; i < _pLedImp->Count(); ++i )
	{
		Add( _pLedImp->GetId( i ), 0, scState_Standby );
	}
}


/// <summary>
/// Adds a single LED id to the list of managed indicators.
/// </summary>
/// <param name="nLedId">The indicator for the LED</param>
/// <param name="nPatternIndex">The pattern index assigned to the LED.</param>
/// <param name="nInitialState">The initial state for the indicator.</param>
void scLedEngine::Add(uint32_t nLedId, uint8_t nPatternIndex, scLedStates_t nInitialState)
{
	scLedEngineDescriptor_t* pDesc = new scLedEngineDescriptor_t;
	pDesc->_nLedId          = nLedId;
	pDesc->_pSM             = new scStateMachine( g_NumberInputs, g_NumberStates, g_InitialState, g_TransitionMatrix );
	pDesc->_nTimeCouter     = 0;
	pDesc->_bRunning        = false;
	pDesc->_nPatternActive  = 0;
	pDesc->_nPatternIndex   = 0;
	_LedList.push_back( pDesc );
	//lint -e{ 429, 423, 438 }  // not a memory leak.
}


/// <summary>
/// This method will assign the pattern table to the engine. The default patter
/// contains some simple on off sequences for heartbeat and normal blinking.
/// </summary>
/// <param name="pTable">Pointer to the pattern table.</param>
/// <param name="nRows">Number of rows in the table</param>
/// <param name="nCols">Number of columns in the table.</param>
void scLedEngine::SetPatterns( const uint8_t* pTable, uint8_t nRows, uint8_t nCols )
{
	scScopeLock	lock( _hLock );

	_pPatterns = pTable;
	_nPatternRows = nRows;
	_nPatternCols = nCols;
}

/// <summary>
/// This is how long the polling thread will delay between updating the LED
/// flashing. This delay will be the fastest speed any single LED can blink.
/// </summary>
void scLedEngine::Sleep(void)
{
	// derived class implements the sleep functions.
}

void scLedEngine::Update( uint32_t nLedId, scLedStates_t nextState )
{
	scScopeLock	lock( _hLock );
	vector<scLedEngineDescriptor_t*>::iterator	itr = _LedList.begin();
	for( ; itr != _LedList.end(); itr++ )
	{
		assert_param( (*itr) != NULL );
		if ( (*itr)->_nLedId == nLedId && (*itr)->_pSM != NULL )
		{
			switch( nextState )
			{
			default:
				break;
			case scState_Standby:
				(*itr)->_pSM->advance( Input_Standby );
				break;
			case scState_On:
				(*itr)->_pSM->advance( Input_TurnOn );
				break;
			case scState_Off:
				(*itr)->_pSM->advance( Input_TurnOff );
				break;
			case scState_Flash:
				(*itr)->_pSM->advance( Input_Flash );
				break;
			case scState_Disabled:
				(*itr)->_pSM->advance( Input_Enable );
				break;
			case scState_Enabled:
				(*itr)->_pSM->advance( Input_Disable );
				break;
			}
			ChangeState( (*itr) );
		}
	}
}

void scLedEngine::ChangeState( scLedEngine::scLedEngineDescriptor_t* pLed )
{
	if ( pLed != NULL && _pLedImp != NULL )
	{
		scLedIF::LedState_t	nextState = scLedIF::scOff;
		switch( (*pLed->_pSM)() )
		{
		case State_FlashOn:
			nextState = scLedIF::scOn;
			break;
		case State_FlashOff:
			nextState = scLedIF::scOff;
			break;
		case State_FlashOnToStandby:
			nextState = scLedIF::scOn;
			break;
		case State_Standby_Off:
			nextState = scLedIF::scOff;
			break;
		case State_AlwaysOff:
			nextState = scLedIF::scOff;
			break;
		case State_AlwaysOn:
			nextState = scLedIF::scOn;
			break;
		case State_Disabled:
			nextState = scLedIF::scOff;
			break;
		default:
			nextState = scLedIF::scOff;
			break;
		}
		_pLedImp->Illuminate( pLed->_nLedId, nextState );
	}
}


void scLedEngine::UpdateAll( scLedIF::LedState_t state )
{
	assert_param( _pLedImp != NULL );
	vector<scLedEngineDescriptor_t*>::iterator	itr = _LedList.begin();
	for( ; itr != _LedList.end(); itr++ )
	{
		_pLedImp->Illuminate( (*itr)->_bRunning, state );
	}
}

/// <summary>
/// Enable the engine. This will start the transitions and blinking. By default the
/// engine is enabled.
/// </summary>
void scLedEngine::Enable(void)
{
	scScopeLock	lock( _hLock );
	vector<scLedEngineDescriptor_t*>::iterator	itr = _LedList.begin();
	for( ; itr != _LedList.end(); itr++ )
	{
		Update( (*itr)->_nLedId, scState_Enabled );
	}
}

/// <summary>
/// Stop the engine from changing any of the LED states. Current states are frozen
/// as they were left.
/// </summary>
void scLedEngine::Disable(void)
{
	scScopeLock	lock( _hLock );
	vector<scLedEngineDescriptor_t*>::iterator	itr = _LedList.begin();
	for( ; itr != _LedList.end(); itr++ )
	{
		Update( (*itr)->_nLedId, scState_Disabled );
	}
}

/// <summary>
/// This method will perform an LED test turning each LED on/off and blinking them.
/// </summary>
/// <param name="bSuspendTasks">Flag to suspend tasks if the environment supports a
/// task based implementation.</param>
void scLedEngine::Test(bool bSuspendTasks)
{
	// Stop the blinking task
	if ( bSuspendTasks )
	{
		TaskSuspend();
	}

	// Turn the LEDs on and off and then back on.
	UpdateAll( scLedIF::scOn );
	Sleep();
	UpdateAll( scLedIF::scOff);
	Sleep();
	UpdateAll( scLedIF::scOn );

	// Flash one by one and turn off
	vector<scLedEngineDescriptor_t*>::iterator	itr = _LedList.begin();
	for( ; itr != _LedList.end(); itr++ )
	{
		for (int x = 0; x < 5; x++)
		{
			_pLedImp->Illuminate( (*itr)->_nLedId, scLedIF::scOff );

			Sleep();

			_pLedImp->Illuminate((*itr)->_nLedId, scLedIF::scOn);

			Sleep();

			_pLedImp->Illuminate((*itr)->_nLedId, scLedIF::scOn);
		}
	}

	// Restart the blinking task
	if ( bSuspendTasks )
	{
		TaskResume();
	}
}

/// <summary>
/// Put all the LED task into a suspended state.
/// </summary>
void scLedEngine::TaskSuspend(void)
{
	// no-op
}

/// <summary>
/// Resume a suspended LED task.
/// </summary>
void scLedEngine::TaskResume(void)
{
	// no-op
}

/// <summary>
/// Change the pattern index that is currently being used for the specified LED
/// index. Locks the mutex
/// </summary>
/// <param name="nLedId">The Id of the Led.</param>
/// <param name="nPattern">The new index this LED will use in the pattern table.
/// </param>
bool scLedEngine::UpdatePattern(uint32_t nLedId, uint8_t nPattern)
{
	assert_param( _pPatterns != NULL );
	bool bResult = false;
	if ( nPattern < _nPatternRows )
	{
		scScopeLock lock( _hLock );
		bResult = UpdatePatternUnsafe( nLedId, nPattern );
	}
	return bResult;
}

/// <summary>
/// Change the pattern index that is currently being used for the specified LED
/// index. Skips the mutex locking
/// </summary>
/// <param name="nLedId">The Id of the Led.</param>
/// <param name="nPattern">The new index this LED will use in the pattern table.
/// </param>
bool scLedEngine::UpdatePatternUnsafe(uint32_t nLedId, uint8_t nPattern)
{
	assert_param( _pPatterns != NULL );
	bool bResult = false;
	if ( nPattern < _nPatternRows )
	{
		vector<scLedEngineDescriptor_t*>::iterator	itr = _LedList.begin();
		for( ; !bResult && itr != _LedList.end(); itr++ )
		{
			if ( (*itr)->_nLedId == nLedId )
			{
				(*itr)->_nTimeCouter		= 0;
				(*itr)->_nPatternActive		= nPattern;
				(*itr)->_nFlashRate			= _pPatterns[(*itr)->_nPatternActive * _nPatternCols];
				(*itr)->_nPatternIndex		= 0;
				bResult = true;
			}
		}
	}
	return bResult;
}

/// <summary>
/// Perform a single transaction check of the led states. Normally called from the
/// task that managed the time delay. The trigger can be from a sleep timer, normal
/// timer, or some other interval trigger.
/// </summary>
void scLedEngine::Poll(void)
{
	scScopeLock	lock( _hLock );
	vector<scLedEngineDescriptor_t*>::iterator	itr = _LedList.begin();
	for( ; itr != _LedList.end(); itr++ )
	{
		if ( (*itr) != NULL && (*itr)->_pSM != NULL )
		{
			if (	(*(*itr)->_pSM)() == State_FlashOn 
				||	(*(*itr)->_pSM)() == State_FlashOff
				||	(*(*itr)->_pSM)() == State_FlashOnToStandby )
			{
				// advance the single tick counter
				(*itr)->_nTimeCouter++;
				// check if the time counter has crossed the change rate threshold.
				if ( (*itr)->_nTimeCouter >= (*itr)->_nFlashRate )
				{
					// Reset the counter and move to the next blink rate index
					(*itr)->_nTimeCouter = 0;
					(*itr)->_nPatternIndex++;

					// handle the wrap around
					if ( (*itr)->_nPatternIndex >= _nPatternCols )
					{
						(*itr)->_nPatternIndex = 0;
					}
					// set the flash rate using the next pattern lookup.
					(*itr)->_nFlashRate = _pPatterns[(*itr)->_nPatternIndex + ((*itr)->_nPatternActive * _nPatternCols)];
					if ( (*itr)->_nFlashRate == 0 )
					{
						(*itr)->_pSM->advance( Input_Standby );
					}

					if ( (*(*itr)->_pSM)() == State_FlashOff )
					{
						(*itr)->_pSM->advance( Input_Flash );
					}
					else
					{
						(*itr)->_pSM->advance( Input_FlashOff );
					}
					ChangeState( *itr );
				}
			}
		}
		else
		{
			// The else is to trap the debugging error.
			assert_param( (*itr) != NULL );
			assert_param( (*itr)->_pSM != NULL );
		}
	}
}