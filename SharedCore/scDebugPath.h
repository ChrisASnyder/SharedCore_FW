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
// File Name:		scDebugPath.h
// Created By:		Christopher Snyder
// Creation Date:	14-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCDEBUGPATH_H__INCLUDED_)
#define __SCDEBUGPATH_H__INCLUDED_

#include "scTypes.h"

namespace SharedCore
{
	/// <summary>
	/// This is the base class of an object that is used to route debug messages. The
	/// debug manager will contain a list of this objects that have been constructed as
	/// a path for debug messages. Since debug activity observed through many different
	/// methods this provides the flexibility to define the path most useful for any
	/// given system. The framework will contain several of the more commonly used
	/// paths.
	/// </summary>
	class scDebugPath
	{

	public:
		/// <summary>
		/// Simple destructor
		/// </summary>
		virtual ~scDebugPath();

		/// <summary>
		/// The constructor of the path. By default only the path id is assigned.
		/// </summary>
		/// <param name="nPathID">The path Id</param>
		scDebugPath( uint8_t nPathID = 0 );
		
		/// <summary>
		/// This method is the command that will take the string provided and route to the
		/// method in which it will get displayed, stored or routed. This method must be
		/// overridden by the child class to provide the required implementation.
		/// </summary>
		virtual void Capture( const uint8_t* pData, uint32_t nLength ) = 0;

		/// <summary>
		/// This method will change the flag that permits the use of this debug path.
		/// </summary>
		/// <param name="bValue">The new state of the enable flag.</param>
		void Enable( void );
		
		/// <summary>
		/// Disable this path.
		/// </summary>
		void Disable();

		/// <summary>
		/// Access to the path ID for this object.
		/// </summary>
		uint8_t PathId(void) const;

		/// <summary>
		/// Get the state of the Enable flag.
		/// </summary>
		scEnableState_t State(void) const;

		/// <summary>
		/// Set the state of the Enable flag.
		/// </summary>
		void State(scEnableState_t nState );

	private:
		/// <summary>
		/// This variable is used to identify this path from other paths available in a
		/// given project. The actual value is not to important other than it is different
		/// from the other paths used in a system.
		/// </summary>
		uint8_t					_nPathId;

		/// <summary>
		/// Flag to indicate if this debug path is usable.
		/// </summary>
		scEnableState_t			_nState;

		/// <summary>
		/// Copy constructors are not allowed.
		/// </summary>
		/// <param name="source">no copy constructors allowed.</param>
		scDebugPath(const scDebugPath& source)
		{}

		/// <summary>
		/// Copy constructors are not allowed.
		/// </summary>
		scDebugPath& operator=( const scDebugPath& source ) 
		{ return *this; }
	};

}
#endif // !defined(__SCDEBUGPATH_H__INCLUDED_)
