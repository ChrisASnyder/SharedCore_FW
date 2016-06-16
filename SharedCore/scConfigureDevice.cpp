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
// File Name:		scConfigureDevice.h
// Created By:		Christopher Snyder
// Creation Date:	14-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include "scConfigureDevice.h"

using SharedCore::scConfigureDevice;

	/// <summary>
	/// Simple Destructor
	/// </summary>
scConfigureDevice::~scConfigureDevice()
{
}

/// <summary>
/// Copy Constructor
/// </summary>
/// <param name="source">copy source</param>
scConfigureDevice::scConfigureDevice(const scConfigureDevice& source)
	: _nParameter(source._nParameter)
{
}

/// <summary>
/// Copy Constructor
/// </summary>
/// <param name="nValue">The default value of the parameter</param>
scConfigureDevice::scConfigureDevice(uint32_t nValue)
	: _nParameter(nValue)
{
}

/// <summary>
/// Get method for the parameter
/// </summary>
uint32_t scConfigureDevice::Parameter() const 
{
	return _nParameter;
}

/// <summary>
/// Set method for the parameter
/// </summary>
/// <param name="nValue">32-bit generic parameter.</param>
scConfigureDevice& scConfigureDevice::Parameter(uint32_t nValue)
{
	_nParameter = nValue;
	return *this;
}

/// <summary>
/// Assignment operator.
/// </summary>
/// <param name="nValue">Parameter value</param>
scConfigureDevice& scConfigureDevice::operator=(uint32_t nValue)
{
	_nParameter = nValue;
	return *this;
}

/// <summary>
/// Assignment operator.
/// </summary>
/// <param name="source">Parameter value</param>
scConfigureDevice& scConfigureDevice::operator=(const scConfigureDevice& source)
{
	if ( this != &source )
	{
		_nParameter = source._nParameter;
	}
	return *this;
}