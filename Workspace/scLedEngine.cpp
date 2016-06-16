//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          All Rights Reserved
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

using SharedCore::scLedEngine;

/// <summary>/// Class Destructor for scLedEngine
/// </summary>
scLedEngine::scLedEngine()
{

}

/// <summary>/// Class constructor for scLedEngine
/// </summary>
scLedEngine::~scLedEngine()
{

}



/// <summary>
/// Initializes the engine.
/// </summary>
void scLedEngine::Initialize()
{
}

/// <summary>
/// This method will assign the pattern table to the engine. The default patter
/// contains some simple on off sequences for heartbeat and normal blinking.
/// </summary>
/// <param name="pTable">Pointer to the pattern table.</param>
/// <param name="nRows">Number of rows in teh table</param>
/// <param name="nCols">Number of columns in the table.</param>
void scLedEngine::SetPatterns(const uint8_t* pTable, uint8_t nRows, uint8_t nCols)
{
}

/// <summary>
/// This is how long the polling thread will delay between updating the LED
/// flashing. This delay will be the fastest speed any single LED can blink.
/// </summary>
void scLedEngine::Sleep()
{
}

/// <summary>
/// Destructor for the class. Clean up allocated resources.
/// </summary>
scLedEngine::~LedEngine()
{
}

/// <summary>
/// Default constructor. Sets up the table.
/// </summary>
/// <param name="pTable">Default blink pattern table</param>
/// <param name="nRows">Number of rows in the table.</param>
/// <param name="nCols">Number of columns in the table.</param>
scLedEngine::LedEngine(const uint8_t* pTable, uint8_t nRows, uint8_t nCols)
{
}

/// <summary>
/// Enable the engine. This will start the transitions and blinking. By default the
/// engine is enabled.
/// </summary>
void scLedEngine::Enable()
{
}

/// <summary>
/// Stop the engine from changing any of the LED states. Current states are frozen
/// as they were left.
/// </summary>
void scLedEngine::Disable()
{
}

/// <summary>
/// This method will perform an LED test turning each LED on/off and blinking them.
/// 
/// </summary>
/// <param name="bSuspendTasks">Flag to suspend tasks if the environment supports a
/// task based implementation.</param>
void scLedEngine::Test(bool bSuspendTasks)
{
}

/// <summary>
/// Put all the LED task into a suspended state.
/// </summary>
void scLedEngine::TaskSuspend()
{
}

/// <summary>
/// Resume a suspended LED task.
/// </summary>
void scLedEngine::TaskResume()
{
}

/// <summary>
/// Change the pattern index that is currently being used for the specified LED
/// index.
/// </summary>
/// <param name="nLedId">The Id of the Led.</param>
/// <param name="nPattern">The new index this LED will use in the pattern table.
/// </param>
bool scLedEngine::PatternChange(uint32_t nLedId, uint8_t nPattern)
{
	return false;
}