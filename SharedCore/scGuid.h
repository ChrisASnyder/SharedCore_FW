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
// File Name:		scGuid.h
// Created By:		Christopher Snyder
// Creation Date:	13-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCGUID_H__INCLUDED_)
#define __SCGUID_H__INCLUDED_

#include "scDeviceDescriptor.h"

namespace SharedCore
{
	/// <summary>
	/// This is a template class designed two wrap a user enum set for Guids to provide
	/// the interface to the descriptor objects.
	/// </summary>
	template<class T>
	class scGuid : public scDeviceDescriptor
	{

	public:
		/// <summary>
		/// Default Destructor
		/// </summary>		
		virtual ~scGuid() {}

		/// <summary>
		/// Default Constructor
		/// </summary>
		scGuid( T nGuid, uint32_t nParam = 0 )
			: scDeviceDescriptor( (int)nGuid, nParam )
		{
		}

		/// <summary>
		/// The constructor creating the object for this Guid.
		/// </summary>
		/// <param name="source">Source object</param>
		scGuid(const scGuid<T>& source)
		{
			if ( this != &source )
			{
				Guid( source.Guid() );
				Param( source.Param() );
			}
		}
	
		/// <summary>
		/// Overloaded assignment operator. Allows the Guid to be changed.
		/// </summary>
		/// <param name="rhs">Source item being compared.</param>
		scGuid<T>& operator=(T rhs)
		{
			Guid( (int)rhs );
		}

		/// <summary>
		/// Overloaded equality operator
		/// </summary>
		/// <param name="rhs">Source item being compared.</param>
		bool operator==(const scGuid<T>& rhs) const
		{
			return Guid() == rhs.Guid();
		}

		/// <summary>
		/// Overloaded equality operator
		/// </summary>
		/// <param name="rhs">Source item being compared.</param>
		bool operator==(T rhs) const
		{
			return Guid() == (int)rhs;
		}

		/// <summary>
		/// Overloaded inequality operator
		/// </summary>
		/// <param name="rhs">Source item being compared.</param>
		bool operator!=(const scGuid<T>& rhs) const
		{
			return Guid() != rhs.Guid();
		}

		/// <summary>
		/// Overloaded inequality operator
		/// </summary>
		/// <param name="rhs">Source item being compared.</param>
		bool operator!=(T rhs) const
		{
			return Guid() != (int)rhs;
		}
	};

}
#endif // !defined(__SCGUID_H__INCLUDED_)
