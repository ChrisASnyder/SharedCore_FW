//==============================================================================
//          UNCLASSIFIED
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
// File Name:		scDeviceDescriptor.h
// Created By:		Christopher Snyder
// Creation Date:	13-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include "scDeviceDescriptor.h"

using SharedCore::scDeviceDescriptor;

/// <summary>
/// The constructor creating the object for this Guid.
/// </summary>
scDeviceDescriptor::~scDeviceDescriptor()
{

}

/// <summary>
/// The constructor creating the object for this Guid.
/// </summary>
/// <param name="nGuid">The Guid to be assigned for this class. Default is INVALID_DRIVER_GUID</param>
/// <param name="nParam">Parameter optional.</param>
scDeviceDescriptor::scDeviceDescriptor( int nGuid, uint32_t nParam )
	: _deviceGuid( nGuid )
	, _nParameter( nParam )
{
}

/// <summary>
/// The constructor creating the object for this Guid.
/// </summary>
/// <param name="source">Source object</param>
scDeviceDescriptor::scDeviceDescriptor(const scDeviceDescriptor& source)
	: _deviceGuid( source._deviceGuid )
	, _nParameter( source._nParameter )
{
}

/// <summary>
/// Overloaded assignment operator. Allows the Guid to be changed.
/// </summary>
/// <param name="rhs">Source item being compared.</param>
scDeviceDescriptor& scDeviceDescriptor::operator=(int rhs)
{
	_deviceGuid = rhs;
	return *this;
}

/// <summary>
/// Get method for obtaining the Guid.
/// </summary>
int scDeviceDescriptor::Guid(void) const
{
	return _deviceGuid;
}

/// <summary>
/// Provide access to the Guid
/// </summary>
int scDeviceDescriptor::operator()() const
{
	return _deviceGuid;
}

/// <summary>
/// Overloaded inequality operator
/// </summary>
/// <param name="rhs">Source item being compared.</param>
bool scDeviceDescriptor::operator!=(const scDeviceDescriptor& rhs) const
{
	return this->Guid() != rhs.Guid();
}

/// <summary>
/// Overloaded inequality operator
/// </summary>
/// <param name="rhs">Source item being compared.</param>
bool scDeviceDescriptor::operator!=(int rhs) const
{
	return this->Guid() != rhs;
}


/// <summary>
/// Overloaded equality operator
/// </summary>
/// <param name="rhs">Source item being compared.</param>
bool scDeviceDescriptor::operator==(const scDeviceDescriptor& rhs) const
{
	return Guid() == rhs.Guid();
}

/// <summary>
/// Overloaded equality operator
/// </summary>
/// <param name="rhs">Source item being compared.</param>
bool scDeviceDescriptor::operator==(int rhs) const
{
	return Guid() == rhs;
}

/// <summary>
/// Get method for setting the Guid.
/// </summary>
scDeviceDescriptor& scDeviceDescriptor::Guid(int rhs )
{
	_deviceGuid = rhs;
	return *this;
}

/// <summary>
/// Get method for the additional parameter
/// </summary>
uint32_t scDeviceDescriptor::Param(void) const
{
	return _nParameter;
}

/// <summary>
/// Set method for the parameter value.
/// </summary>
/// <param name="value">This is the value to be assigned to the parameter member.
/// </param>
scDeviceDescriptor& scDeviceDescriptor::Param(uint32_t value)
{
	_nParameter = value;
	return *this;
}

/// <summary>
/// Overloaded equal operator
/// </summary>
/// <param name="rhs">Source item being compared.</param>
scDeviceDescriptor& scDeviceDescriptor::operator=(const scDeviceDescriptor& rhs)
{
	if ( this != &rhs )
	{
		_deviceGuid = rhs._deviceGuid;
		_nParameter = rhs._nParameter;
	}
	return *this;
}

