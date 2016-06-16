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
// File Name:		scDeviceGeneric.h
// Created By:		Christopher Snyder
// Creation Date:	13-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include "scDeviceGeneric.h"
#include "scErrorCodes.h"
#include "scDeviceManager.h"

using SharedCore::scDeviceGeneric;
using SharedCore::scDeviceDescriptor;
using SharedCore::scDeviceManager;
using SharedCore::scConfigureDevice;
using SharedCore::scEnableState_t;
using std::vector;

/// <summary>
/// Destructor
/// </summary>
scDeviceGeneric::~scDeviceGeneric()
{
}

/// <summary>
/// Construct the class using the Guid
/// </summary>
/// <param name="descriptor">Describes the descriptor for this device.</param>
scDeviceGeneric::scDeviceGeneric( const scDeviceDescriptor& descriptor)
	: _Guid(descriptor)
	, _dependants(0)
	, _nLastError(ERROR_SUCCESS)
	, _nSetupState(Uninitialized)
	, _nState(scDisabled)
{
}

/// <summary>
/// Construct the class using the Guid and one dependant
/// </summary>
/// <param name="descriptor">Describes the descriptor for this device.</param>
/// <param name="dependant">the sole dependant of the device.</param>
scDeviceGeneric::scDeviceGeneric( const scDeviceDescriptor& descriptor, const scDeviceDescriptor& dependant)
	: _Guid(descriptor)
	, _dependants(0)
	, _nLastError(ERROR_SUCCESS)
	, _nSetupState(Uninitialized)
	, _nState(scDisabled)
{
	_dependants.push_back( dependant );
}

/// <summary>
/// Construct the class using the Guid and multiple dependants
/// </summary>
/// <param name="descriptor">Describes the descriptor for this device.</param>
/// <param name="nCount">Number of dependants listed.</param>
/// <param name="prm1"></param>
scDeviceGeneric::scDeviceGeneric( const scDeviceDescriptor& descriptor, int nCount, ... )
	: _Guid(descriptor)
	, _dependants(0)
	, _nLastError(ERROR_SUCCESS)
	, _nSetupState(Uninitialized)
	, _nState(scDisabled)
{
	va_list vl;
	va_start( vl, nCount );
	AddDependants( vl, nCount );
	va_end(vl);
}

/// <summary>
/// This method is need to allow subclasses to implement the variable length
/// argument. 
/// </summary>
void scDeviceGeneric::AddDependants( va_list vl, int nCount )
{
	for (int i=0; i < nCount;i++)
	{
		const scDeviceDescriptor* next = va_arg(vl, const scDeviceDescriptor* );
		_dependants.push_back( *next );
	}
}

/// <summary>
/// The descriptor for the device.
/// </summary>
const scDeviceDescriptor& scDeviceGeneric::Guid(void) const 
{
	return  _Guid;
}

/// <summary>
/// Perform one time initialization activity. This is called by the Device Manager
/// during the application startup and this method is only ever called once. The
/// Configure/Unconfigure methods are used to change the resource usage.
/// </summary>
void scDeviceGeneric::Initialize( scDeviceManager* pDm )
{
	SetupState( Initialized );
}

/// <summary>
/// Reset the driver state back to a state where it no longer is using any
/// resources.
/// </summary>
void scDeviceGeneric::Unconfigure(const scConfigureDevice& config)
{
	SetupState( Unconfigured );
}

/// <summary>
/// This method is used to configure the device. Configuration is similar to the
/// initialization route but contains the actions that can be repeated multiple
/// times and is required to be performed following an unconfigure call.
/// </summary>
void scDeviceGeneric::Configure(const scConfigureDevice& config)
{
	SetupState( Configured );
}

/// <summary>
/// Obtain the error code last reported by this driver.
/// </summary>
uint32_t scDeviceGeneric::GetLastError(void) const 
{
	return _nLastError;
}

/// <summary>
/// This method will cause the driver to become active. This method is not intended
/// to setup or change the properties of the driver or the resources it used beyond
/// what is necessary to make to start and stop. For example the enable of an
/// Interrupt through a register.
/// </summary>
void scDeviceGeneric::Enable(void)
{
	State( scEnabled );
}

/// <summary>
/// The list of devices that this device requires to be functional
/// </summary>
const vector<scDeviceDescriptor>& scDeviceGeneric::Dependants(void) const 
{
	return _dependants;
}

/// <summary>
/// This method will look at the dependants list and return true if other drivers
/// are required to be functional before this driver can function.
/// </summary>
bool scDeviceGeneric::HaveDependants(void) const 
{
	return _dependants.size() > 0;
}

/// <summary>
/// For drivers that support a connection based functionality this method will
/// return true and false. Other drivers will return true by default.
/// </summary>
bool scDeviceGeneric::IsConnected(void) const 
{
	return true;
}

/// <summary>
/// This will cause the driver to full reset its state back to that of a newly
/// initialized device.
/// </summary>
void scDeviceGeneric::Reset(void)
{
}

/// <summary>
/// Stop the driver from functioning by turning off features that cause it to do
/// things.
/// </summary>
void scDeviceGeneric::Disable(void)
{
	State( scDisabled );
}


/// <summary>
/// Determine if the descriptor matches this drivers Guid
/// </summary>
/// <param name="rhs"></param>
bool scDeviceGeneric::operator==( const scDeviceDescriptor& rhs) const 
{
	return _Guid == rhs;
}

/// <summary>
/// Determine if the descriptor does not matches this drivers Guid
/// </summary>
/// <param name="rhs"></param>
bool scDeviceGeneric::operator!=(const scDeviceDescriptor& rhs) const 
{
	return _Guid != rhs;
}


/// <summary>
/// Get the current enable/disable state of the device.
/// </summary>
scEnableState_t scDeviceGeneric::State(void) const 
{
	return _nState;
}

/// <summary>
/// Set the state of the device
/// </summary>
/// <param name="next"></param>
void scDeviceGeneric::State( scEnableState_t next)
{
	_nState = next;
}

/// <summary>
/// Get the current setup state of the device. This driver must manage it's state
/// during the initialization process.
/// </summary>
SharedCore::scDeviceGeneric::DeviceSetupState_t scDeviceGeneric::SetupState(void) const 
{
	return _nSetupState;
}

/// <summary>
/// Set the current setup state of the device. This driver must manage it's state
/// during the initialization process.
/// </summary>
void scDeviceGeneric::SetupState( scDeviceGeneric::DeviceSetupState_t next )
{
	_nSetupState = next;
}
