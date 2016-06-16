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
// File Name:		scScopeLock.h
// Created By:		Christopher Snyder
// Creation Date:	16-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCSCOPELOCK_H__INCLUDED_)
#define __SCSCOPELOCK_H__INCLUDED_

#include "scIMutex.h"

namespace SharedCore
{
	/// <summary>
	/// A simple helper class to create a mutex protection within the scope of this
	/// object existing. When this object enters scope it calls Acquire and then calls
	/// Release() on exit.
	/// </summary>
	class scScopeLock
	{
	public:
		/// <summary>
		/// Construct the lock control object.
		/// </summary>
		/// <param name="lockControl">the protection object.</param>
		scScopeLock( scIMutex* lockControl );

		/// <summary>
		/// Release the mutex
		/// </summary>
		virtual ~scScopeLock();

	private:
		/// <summary>
		/// The protection object
		/// </summary>
		scIMutex*			_pMutex;

	};

}
#endif // !defined(__SCSCOPELOCK_H__INCLUDED_)
