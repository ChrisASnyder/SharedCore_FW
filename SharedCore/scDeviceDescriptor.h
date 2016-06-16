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
// File Name:		scDeviceDescriptor.h
// Created By:		Christopher Snyder
// Creation Date:	13-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCDEVICEDESCRIPTOR_H__INCLUDED_)
#define __SCDEVICEDESCRIPTOR_H__INCLUDED_

#include "scTypes.h"

#define INVALID_DRIVER_GUID		(0xFFFFFFFF)

namespace SharedCore
{
	/// <summary>
	/// This class is a wrapper over the enumeration type that will be used to identify
	/// drivers in any given system. Since the actual list of drivers is not known to
	/// the SharedCore, this class is used as a substitute. The application will define
	/// the enumerated list of drives it uses and will wrap that item in this class for
	/// use within the SharedCore framework. The scDeviceDescriptorFactory will be
	/// responsible for creating the descriptors.
	/// </summary>
	class scDeviceDescriptor
	{
	public:
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~scDeviceDescriptor();

		/// <summary>
		/// The constructor creating the object for this Guid.
		/// </summary>
		/// <param name="nGuid">The Guid to be assigned for this class. Default is INVALID_DRIVER_GUID</param>
		/// <param name="nParam">Parameter optional.</param>
		scDeviceDescriptor( int nGuid = INVALID_DRIVER_GUID, uint32_t nParam = 0 );

		/// <summary>
		/// The constructor creating the object for this Guid.
		/// </summary>
		/// <param name="source">Source object</param>
		scDeviceDescriptor(const scDeviceDescriptor& source);

		/// <summary>
		/// Overloaded equal operator
		/// </summary>
		/// <param name="rhs">Source item being compared.</param>
		scDeviceDescriptor& operator=(const scDeviceDescriptor& rhs);

		/// <summary>
		/// Overloaded equality operator
		/// </summary>
		/// <param name="rhs">Source item being compared.</param>
		bool operator==(const scDeviceDescriptor& rhs) const;

		/// <summary>
		/// Overloaded equality operator
		/// </summary>
		/// <param name="rhs">Source item being compared.</param>
		bool operator==(int rhs) const;

		/// <summary>
		/// Overloaded inequality operator
		/// </summary>
		/// <param name="rhs">Source item being compared.</param>
		bool operator!=(const scDeviceDescriptor& rhs) const;

		/// <summary>
		/// Overloaded inequality operator
		/// </summary>
		/// <param name="rhs">Source item being compared.</param>
		bool operator!=(int rhs) const;

		/// <summary>
		/// Provide access to the Guid
		/// </summary>
		int operator()() const;

		/// <summary>
		/// Get method for obtaining the Guid.
		/// </summary>
		int Guid(void) const;

		/// <summary>
		/// Get method for the additional parameter
		/// </summary>
		uint32_t Param(void) const;

		/// <summary>
		/// Set method for the parameter value.
		/// </summary>
		/// <param name="value">This is the value to be assigned to the parameter member.
		/// </param>
		scDeviceDescriptor& Param( uint32_t value );

	protected:
		/// <summary>
		/// Get method for setting the Guid.
		/// </summary>
		scDeviceDescriptor& Guid( int rhs );

		/// <summary>
		/// Overloaded assignment operator. Allows the Guid to be changed.
		/// </summary>
		/// <param name="rhs">Source item being compared.</param>
		scDeviceDescriptor& operator=(int rhs);

	private:
		/// <summary>
		/// A unique number used to identify a device within a system. In most cases this
		/// is a enum and is defined by the application.
		/// </summary>
		int				_deviceGuid;

		/// <summary>
		/// Some devices may require an additional parameter to identify them beyond the
		/// simple driver ID. In those cases this value is used.
		/// </summary>
		uint32_t		_nParameter;

	};

}
#endif // !defined(__SCDEVICEDESCRIPTOR_H__INCLUDED_)
