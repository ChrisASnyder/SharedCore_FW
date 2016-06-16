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
// File Name:		scModuleManager.h
// Created By:		Christopher Snyder
// Creation Date:	03-Jun-2015 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCMODULEMANAGER_H__INCLUDED_)
#define __SCMODULEMANAGER_H__INCLUDED_

#include <vector>
#include "scTypes.h"
#include "scIModule.h"

namespace SharedCore
{

	using namespace std;

	/// <summary>
	/// Maintains a collection of scIModule objects. All of the scIModule objects are singleton and are mostly used
	/// directly by accessing the singleton Instance call. This however provides a single collection point for all
	/// the objects in the system.  When subclassed in an application the derived child will in the Initialize method
	/// acquire the each of the singleton objects and assign their identifier.
	/// </summary>
	class scModuleManager
	{

	public:
		/// <summary>
		/// Class constructor for scModuleManager
		/// </summary>
		scModuleManager(void);

		/// <summary>
		/// Class Destructor for scModuleManager
		/// </summary>
		virtual ~scModuleManager();

		/// <summary>
		/// The Initialize method is a pure virtual that is implemented in the derived child. This purpose of this method
		/// is to gather all the singleton objects in an order construction sequence and store a reference to their
		/// interface in the list.
		/// </summary>
		virtual void Initialize(void) = 0;

		/// <summary>
		/// Permits the sending of commands to all modules.
		/// Returns a result code of 0 on success or an ERROR code on failure. The last error code encountered is
		/// returned All modules will be called even if one of them returns an error.
		/// </summary>
		/// <param name="nCommand">Command ID to send to all modules</param>
		/// <param name="pParameter">Optional parameter to the command. NULL if not used.</param>
		int Notify(uint32_t nCommand, void* pParameter = NULL );

		/// <summary>
		/// Permits the sending of commands to a specific modules by the ID.
		/// Returns a result code of 0 on success or an ERROR code on failure. The code is directly from the referenced
		/// module.
		/// </summary>
		/// <param name="nId">ID of the module to receive the command</param>
		/// <param name="nCommand">Command to send</param>
		/// <param name="pParam">Optional parameter for the command. NULL if not used</param>
		int Notify( uint16_t nId, uint32_t nCommand, void* pParameter );

		scIModule* Find( uint16_t nId ) const;

	protected:
		/// <summary>
		/// This is the list of managed singleton objects in the system.
		/// </summary>
		vector<scIModule*>							_Modules;

		/// <summary>
		/// This method will add a new singleton module to the list of items and assign the unique id for that module.
		/// This is called by the child during initialization.
		/// Returns the ID of the module added. 
		/// if the module ID already exists the method will assert if in debug otherwise it will return the next largest.
		/// </summary>
		/// <param name="pModule">Pointer to the module to be added</param>
		/// <param name="nModuleID">The new ID to be assigned to this module.</param>
		uint16_t Add(SharedCore::scIModule* pModule, uint16_t nModuleID);

		/// <summary>
		/// This method will add a new singleton module to the list of items and assign the unique id for that module.
		/// This is called by the child during initialization. This add method is assuming the use of an auto ID.
		/// The return value in this instance is the ID assigned to the module
		/// </summary>
		/// <param name="pModule">Pointer to the module to be added</param>
		uint16_t Add(SharedCore::scIModule* pModule );

		/// <summary>
		/// This method will search the known list of modules and find the last ID used and return a value of +1 to that
		/// value. Note that 0 is never a valid ID for module with the assumption that the Module manager is always ID 0.
		/// </summary>
		uint16_t GetNextAutoID();

	};

}
#endif // !defined(__SCMODULEMANAGER_H__INCLUDED_)
