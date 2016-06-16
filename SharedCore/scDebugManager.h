//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
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
//==============================================================================

//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//
// File Name:		scDebugManager.h
// Created By:		Christopher Snyder
// Creation Date:	14-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCDEBUGMANAGER_H__INCLUDED_)
#define __SCDEBUGMANAGER_H__INCLUDED_

#include <stdarg.h>
#include <vector>
#include "scTypes.h"
#include "scDebugPath.h"
#include "scIDebugLabelManager.h"
#include "scSingletonPtr.h"
#include "scAllocator.h"

using namespace std;

namespace SharedCore
{
	/// <summary>
	/// A singleton class, the child must implement the static Instance() method. This
	/// class will manage the debug paths to allow the system to enable and disable
	/// debug messages and the routes they use. This class will support multiple
	/// possibilities for debug messages to be used. For example they can go out a UART,
	/// or saved to memory as a log, a disk, or a message system. These can be any path
	/// that can handle writing a block of memory.
	/// </summary>
	class scDebugManager
	{
	public:

		/// <summary>
		/// Access to the singleton object
		/// </summary>
		static scDebugManager* Instance(void);

		/// <summary>
		/// Simple Destructor
		/// </summary>
		virtual ~scDebugManager();

		/// <summary>
		/// This method will change the enabled state of the provided path ID. When enabled,
		/// debug messages will use that path, when disabled no message will use that path.
		/// </summary>
		/// <param name="nPathId">The ID of the path to change state.</param>
		/// <param name="nState ">True to enable the path, false to disable the
		/// path</param>
		void SetPathEnable(uint8_t nPathId, scEnableState_t nState );

		/// <summary>
		/// Get the current enable state of the specified path.
		/// </summary>
		/// <param name="nPathId">the path of the state to check</param>
		scEnableState_t GetPathEnable(uint8_t nPathId);

		/// <summary>
		/// This will enable the debug manage, the current state of the paths are now in
		/// effect.
		/// </summary>
		void Enable(void);
		
		/// <summary>
		/// This will disable the debug manage from logging any messages. The path states
		/// do not change and are not called.
		/// </summary>
		void Disable(void);

		/// <summary>
		/// This method will add a new path to the debug manger. Once added the debug
		/// manager will be responsible for deleting the memory associated with this
		/// pointer.
		/// </summary>
		/// <param name="pPath">A pointer to the new path.</param>
		size_t Add(scDebugPath* pPath);

		/// <summary>
		/// This path will remove the path from the list and return the pointer.
		/// </summary>
		/// <param name="nPathId">The path id</param>
		scDebugPath* Remove( uint8_t nPathId );

		/// <summary>
		/// Change the enable state of all paths known to the debugger.
		/// </summary>
		/// <param name="nState">the new state of all paths.</param>
		void SetPaths( scEnableState_t nState = scEnabled );

		/// <summary>
		/// Get the current state of the manager.
		/// </summary>
		scEnableState_t State(void) const;

		/// <summary>
		/// Get the current state of the debug label.
		/// </summary>
		/// <param name="nLabel">the id of the debug label.</param>
		scEnableState_t LabelState(uint16_t nLabel) const;

		/// <summary>
		/// Set the current state of the debug label.
		/// </summary>
		/// <param name="nLabel">the id of the debug label.</param>
		/// <param name="nState">The new state for the label.</param>
		void LabelState(uint16_t nLabel, scEnableState_t nState);

		/// <summary>
		/// Changes the state of all labels to be the provided state.
		/// </summary>
		/// <param name="nState">The new state for the label.</param>
		void LabelState( scEnableState_t nState);

		/// <summary>
		/// Will get a text string for the enable state enum.
		/// </summary>
		/// <param name="nState">the state</param>
		static const char* StateText(scEnableState_t nState);

		/// <summary>
		/// Convert a byte value to a hex value.
		/// </summary>
		/// <param name="pOut"></param>
		/// <param name="nByte"></param>
		static char* ByteToHex(char* pOut, uint8_t nByte);

		/// <summary>
		/// This method will convert a memory array to a hex dump.
		/// </summary>
		/// <param name="nLabel">the label.</param>
		/// <param name="pBuffer">Pointer to the memory to be printed.</param>
		/// <param name="nLength">The number of bytes in the memory to print.</param>
		void PrintArray( uint16_t nLabel, const uint8_t* pBuffer, uint32_t nLength );

		/// <summary>
		/// This is the logging method that will behave like a sprintf and format the
		/// string.
		/// </summary>
		/// <param name="nLabel">The debug label.</param>
		/// <param name="pFormat">Format of the output.</param>
		/// <param name="..."></param>
		virtual void Trace(uint16_t nLabel, const char* pFormat, ...);

		/// <summary>
		/// This is a direct output function that does not perform any formatting of the
		/// text prior to sending to the paths. This can be faster than the trace method
		/// since sprintf is not used. This also does not use the working buffer to swap
		/// the memory.
		/// </summary>
		/// <param name="nLabel">The debug label.</param>
		/// <param name="pBuffer">The message, must be null terminated.</param>
		virtual void Trace_Info(uint16_t nLabel, const char* pBuffer);

		/// <summary>
		/// set the label manager.
		/// By design the there should only be one label manager ever assigned.
		/// Any additional manager assignments are rejected because possible memory
		/// leaks.
		/// </summary>
		virtual scIDebugLabelManager* SetLabelManager( scIDebugLabelManager* pLabelManager );

		/// <summary>
		/// For applications where the debug manager needs to exist and have it's resources
		/// cleaned up, this method will disable the logger and delete the paths and manager
		/// </summary>
		virtual void Shutdown(void);

		/// <summary>
		/// Assign the allocator
		/// </summary>
		virtual void Allocator( scAllocator pAllocator );
		
		/// <summary>
		/// get the allocator
		/// </summary>
		virtual const scAllocator& Allocator( void ) const;

	protected:
		/// <summary>
		/// The default constructor. Must be called from the child.
		/// </summary>
		/// <param name="nMaxLabels">The maximum number of debug labels that will be
		/// controlled by the debug manager.</param>
		scDebugManager(void);

		/// <summary>
		/// This is the implementation that will convert the debug formatted string to the
		/// output.
		/// </summary>
		/// <param name="nLabel">The debug label.</param>
		/// <param name="pFormat">The format string.</param>
		/// <param name="ap">the variable argument list.</param>
		virtual void vTrace(uint16_t nLabel, const char* pFormat, va_list ap);

		/// <summary>
		/// get access to the buffer used by the DebugManager.
		/// to change the buffer management subclass this manager and use a different memory allocator
		/// </summary>
		virtual char* WorkingBuffer_Lock(void);

		/// <summary>
		/// get access to the buffer used by the DebugManager.
		/// to change the buffer management subclass this manager and use a different memory allocator
		/// </summary>
		virtual void  WorkingBuffer_Release(char* pBuffer);

	private:

		friend class scSingletonPtr< scDebugManager >;

		/// <summary>
		/// Enables or disables the entire debug manager behavior. By default the debug
		/// manager is disabled.
		/// </summary>
		scEnableState_t					_nState;

		/// <summary>
		/// The list of all the paths managed by the debug manager.
		/// </summary>
		vector<scDebugPath*>			_Paths;

		/// <summary>
		/// The list of all the labels.
		/// </summary>
		scIDebugLabelManager*			_pLabelManager;


		/// <summary>
		/// This is the class that will allocate the temporary buffers
		/// </summary>
		scAllocator						_Allocator;

	};

}
#endif // !defined(__SCDEBUGMANAGER_H__INCLUDED_)
