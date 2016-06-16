//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          chris.snyder@commongroundss.com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  any later version.
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
// File Name:		scLedEngine.h
// Created By:		Christopher Snyder
// Creation Date:	10-Nov-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCLEDENGINE_H__INCLUDED_)
#define __SCLEDENGINE_H__INCLUDED_

#include <vector>
#include "scTypes.h"
#include "scIMutex.h"
#include "scStateMachine.h"
#include "HAL/scLedIF.h"

using namespace SharedCore::HAL;

namespace SharedCore
{
	/// <summary>
	/// Many systems have LED indicators to provide operational feedback using blinking
	/// patterns. This class provides an engine to drive the indicators using a
	/// predefined pattern. Each indicator will contain it's own state machine that
	/// will ensure that the on indication always occurs at least once. This lets a
	/// rapid transitioning activity always get seen. In most cases this class should
	/// have a derived child that is a singleton implementation that creates a task to
	/// provide the polling operation.
	/// </summary>
	class scLedEngine
	{
	public:

		/// <summary>
		/// These are the public labels used for changing the state of an individual LED.
		/// </summary>
		typedef enum
		{
			scState_Standby,
			scState_On,
			scState_Off,
			scState_Flash,
			scState_Disabled,
			scState_Enabled,
		} scLedStates_t;

		/// <summary>
		/// Initializes the engine.
		/// </summary>
		virtual void Initialize( scLedIF* pLedIF );

		/// <summary>
		/// Destructor for the class. Clean up allocated resources.
		/// </summary>
		virtual ~scLedEngine();

		/// <summary>
		/// Enable the engine. This will start the transitions and blinking. By default the
		/// engine is enabled.
		/// </summary>
		virtual void Enable(void);

		/// <summary>
		/// Stop the engine from changing any of the LED states. Current states are frozen
		/// as they were left.
		/// </summary>
		virtual void Disable(void);

		/// <summary>
		/// This method will perform an LED test turning each LED on/off and blinking them.
		/// </summary>
		/// <param name="bSuspendTasks">Flag to suspend tasks if the environment supports a
		/// task based implementation.</param>
		virtual void Test(bool bSuspendTasks = true);

		/// <summary>
		/// Put all the LED task into a suspended state.
		/// </summary>
		virtual void TaskSuspend(void);

		/// <summary>
		/// Resume a suspended LED task.
		/// </summary>
		virtual void TaskResume(void);

		/// <summary>
		/// Perform a single transaction check of the led states. Normally called from the
		/// task that managed the time delay. The trigger can be from a sleep timer, normal
		/// timer, or some other interval trigger.
		/// </summary>
		void Poll(void);

		/// <summary>
		/// Change the pattern index that is currently being used for the specified LED
		/// index.
		/// </summary>
		/// <param name="nLedId">The Id of the Led.</param>
		/// <param name="nPattern">The new index this LED will use in the pattern table.
		/// </param>
		virtual bool UpdatePattern(uint32_t nLedId, uint8_t nPattern);

		/// <summary>
		/// Change the pattern index that is currently being used for the specified LED
		/// index.
		/// </summary>
		/// <param name="nLedId">The Id of the Led.</param>
		/// <param name="nPattern">The new index this LED will use in the pattern table.
		/// </param>
		virtual bool UpdatePatternUnsafe(uint32_t nLedId, uint8_t nPattern);

		virtual void Update( uint32_t nLedId, scLedStates_t nextState );

		virtual void UpdateAll( scLedIF::LedState_t state );

	protected:
		/// <summary>
		/// Default constructor.
		/// </summary>
		scLedEngine( scIMutex* pMutex );

		/// <summary>
		/// This method will assign the pattern table to the engine. The default patter
		/// contains some simple on off sequences for heartbeat and normal blinking.
		/// </summary>
		/// <param name="pTable">Pointer to the pattern table.</param>
		/// <param name="nRows">Number of rows in the table</param>
		/// <param name="nCols">Number of columns in the table.</param>
		void SetPatterns(const uint8_t* pTable, uint8_t nRows, uint8_t nCols);

		/// <summary>
		/// This is how long the polling thread will delay between updating the LED
		/// flashing. This delay will be the fastest speed any single LED can blink.
		/// </summary>
		virtual void Sleep(void);

		/// <summary>
		/// Adds a single LED id to the list of managed indicators.
		/// </summary>
		/// <param name="nLedId">The indicator for the LED</param>
		/// <param name="nPatternIndex">The pattern index assigned to the LED.</param>
		/// <param name="nInitialState">The initial state for the indicator.</param>
		void Add(uint32_t nLedId, uint8_t nPatternIndex, scLedStates_t nInitialState);

		/// <summary>
		/// Mutex used to protect the database from context access.
		/// </summary>
		scIMutex*						_hLock;

	private:

		/// <summary>
		/// This is an internal structure for managing the blink state of the LEDs
		/// </summary>
		typedef struct
		{
			uint32_t					_nLedId;
			uint32_t					_nTimeCouter;
			uint32_t					_nFlashRate;
			uint8_t						_nPatternActive;
			uint8_t						_nPatternIndex;
			scStateMachine*				_pSM;
			bool						_bRunning;
		} scLedEngineDescriptor_t;

		void ChangeState( scLedEngineDescriptor_t* pLed );

		/// <summary>
		/// This is the implementation for the LED indicators that are present in this
		/// system.
		/// </summary>
		scLedIF*							_pLedImp;

		/// <summary>
		/// The patterns used to drive the blink indicators.
		/// </summary>
		const uint8_t*						_pPatterns;

		/// <summary>
		/// Number of rows in the patterns.
		/// </summary>
		uint8_t								_nPatternRows;

		/// <summary>
		/// NUmber of columns in the patterns.
		/// </summary>
		uint8_t								_nPatternCols;


		/// <summary>
		/// This is the list of the LEDs known to the engine that can be transitioned.
		/// </summary>
		vector<scLedEngineDescriptor_t*>	_LedList;
	};

}
#endif // !defined(__SCLEDENGINE_H__INCLUDED_)
