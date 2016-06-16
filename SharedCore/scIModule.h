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
// File Name:		scIModule.h
// Created By:		Christopher Snyder
// Creation Date:	03-Jun-2015 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCIMODULE_H__INCLUDED_)
#define __SCIMODULE_H__INCLUDED_
#include "scTypes.h"

namespace SharedCore
{

	// forward reference for the friend designation
	class scModuleManager;

	/// <summary>
	/// This class is an interface for the singleton objects in the system. All singletons will utilize this base
	/// class and will become registered in the module manager. The main point of this interface is to provide a
	/// unified method of communicating with all singleton objects in the system with system wide commands and events.
	///   Actions power up, power down activities, initialization and other global events can be signaled through
	/// this interface.
	/// </summary>
	class scIModule
	{
	public:
		scIModule();
		virtual ~scIModule();

		/// <summary>
		/// Optionally overridden by the subclassed objects to handle to command notification.
		/// </summary>
		/// <param name="nCommand">Command to execute.</param>
		/// <param name="pParameter">Optional parameter for the command being provided.</param>
		virtual int Notify(uint32_t nCommand, void* pParameter);

		/// <summary>
		/// Return the ID of this module
		/// </summary>
		uint16_t GetID(void) const;

	
	private:
		/// <summary>
		/// This is a friend class used to manage the ID.
		/// </summary>
		friend class scModuleManager;

		/// <summary>
		/// This call is restricted to the module manager and is typically a one time call to setup the ID of the module.
		/// </summary>
		/// <param name="nID">The new ID for the object.</param>
		void SetID(uint16_t nID);

		/// <summary>
		/// A unique ID for the module. This gets assigned by the module manager during startup.
		/// </summary>
		uint16_t											_nID;

};

}
#endif // !defined(__SCIMODULE_H__INCLUDED_)
