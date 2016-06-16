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


#if !defined(__SCLEDENGINE_H__INCLUDED_)
#define __SCLEDENGINE_H__INCLUDED_

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
		scLedEngine();
		virtual ~scLedEngine();

		/// <summary>
		/// Initializes the engine.
		/// </summary>
		virtual void Initialize();
		/// <summary>
		/// Destructor for the class. Clean up allocated resources.
		/// </summary>
		virtual ~LedEngine();
		/// <summary>
		/// Enable the engine. This will start the transitions and blinking. By default the
		/// engine is enabled.
		/// </summary>
		virtual void Enable();
		/// <summary>
		/// Stop the engine from changing any of the LED states. Current states are frozen
		/// as they were left.
		/// </summary>
		virtual void Disable();
		/// <summary>
		/// This method will perform an LED test turning each LED on/off and blinking them.
		/// 
		/// </summary>
		/// <param name="bSuspendTasks">Flag to suspend tasks if the environment supports a
		/// task based implementation.</param>
		virtual void Test(bool bSuspendTasks = true);
		/// <summary>
		/// Put all the LED task into a suspended state.
		/// </summary>
		virtual void TaskSuspend();
		/// <summary>
		/// Resume a suspended LED task.
		/// </summary>
		virtual void TaskResume();
		/// <summary>
		/// Change the pattern index that is currently being used for the specified LED
		/// index.
		/// </summary>
		/// <param name="nLedId">The Id of the Led.</param>
		/// <param name="nPattern">The new index this LED will use in the pattern table.
		/// </param>
		virtual bool PatternChange(uint32_t nLedId, uint8_t nPattern);

	protected:
		/// <summary>
		/// Mutex used to protect the database from context access.
		/// </summary>
		IMutex* _hLock;

		/// <summary>
		/// This method will assign the pattern table to the engine. The default patter
		/// contains some simple on off sequences for heartbeat and normal blinking.
		/// </summary>
		/// <param name="pTable">Pointer to the pattern table.</param>
		/// <param name="nRows">Number of rows in teh table</param>
		/// <param name="nCols">Number of columns in the table.</param>
		void SetPatterns(const uint8_t* pTable, uint8_t nRows, uint8_t nCols);
		/// <summary>
		/// This is how long the polling thread will delay between updating the LED
		/// flashing. This delay will be the fastest speed any single LED can blink.
		/// </summary>
		virtual void Sleep();
		/// <summary>
		/// Default constructor. Sets up the table.
		/// </summary>
		/// <param name="pTable">Default blink pattern table</param>
		/// <param name="nRows">Number of rows in the table.</param>
		/// <param name="nCols">Number of columns in the table.</param>
		LedEngine(const uint8_t* pTable, uint8_t nRows, uint8_t nCols);

	private:
		/// <summary>
		/// This is the implementation for the LED indicators that are present in this
		/// system.
		/// </summary>
		scLedIF* _pLedImp;

		/// <summary>
		/// The patterns used to drive the blink indicators.
		/// </summary>
		const uint8_t* _pPatterns;

		/// <summary>
		/// Number of rows in the patterns.
		/// </summary>
		uint8_t _nPatternRows;

		/// <summary>
		/// NUmber of columns in the patterns.
		/// </summary>
		uint8_t _nPatternCols;

	};

}
#endif // !defined(__SCLEDENGINE_H__INCLUDED_)
