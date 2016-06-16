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
// File Name:		scDeviceGeneric.h
// Created By:		Christopher Snyder
// Creation Date:	13-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCDEVICEGENERIC_H__INCLUDED_)
#define __SCDEVICEGENERIC_H__INCLUDED_

#include <vector>
#include <stdarg.h>
#include "scTypes.h"
#include "scDeviceDescriptor.h"
#include "scConfigureDevice.h"

namespace SharedCore
{
	using namespace std;

	// Forward Reference.
	class scDeviceManager;


	/// <summary>
	/// The base class for all drivers in the system. This class will provide the
	/// default interface for all drivers and the methods needed to create and
	/// configure a driver. All children of this class should implement the methods
	/// that are of importance to them. The default behavior of all functions are
	/// considered no-operation methods.
	/// </summary>
	class scDeviceGeneric
	{
	public:

		typedef enum
		{
			Uninitialized,
			Initialized,
			Configured,
			Unconfigured,
		} DeviceSetupState_t;

		virtual ~scDeviceGeneric();

		/// <summary>
		/// The descriptor for the device.
		/// </summary>
		const scDeviceDescriptor& Guid(void) const;

		/// <summary>
		/// The list of devices that this device requires to be functional
		/// </summary>
		const vector<scDeviceDescriptor>& Dependants(void) const;

		/// <summary>
		/// Perform one time initialization activity. This is called by the Device Manager
		/// during the application startup and this method is only ever called once. The
		/// Configure/Unconfigure methods are used to change the resource usage.
		/// </summary>
		virtual void Initialize( scDeviceManager* pDm );

		/// <summary>
		/// Reset the driver state back to a state where it no longer is using any
		/// resources.
		/// </summary>
		virtual void Unconfigure( const scConfigureDevice& config );

		/// <summary>
		/// This method is used to configure the device. Configuration is similar to the
		/// initialization route but contains the actions that can be repeated multiple
		/// times and is required to be performed following an unconfigure call.
		/// </summary>
		virtual void Configure( const scConfigureDevice& config );

		/// <summary>
		/// Obtain the error code last reported by this driver.
		/// </summary>
		uint32_t GetLastError(void) const;

		/// <summary>
		/// This method will cause the driver to become active. This method is not intended
		/// to setup or change the properties of the driver or the resources it used beyond
		/// what is necessary to make to start and stop. For example the enable of an
		/// Interrupt through a register.
		/// </summary>
		virtual void Enable(void);

		/// <summary>
		/// This method will look at the dependants list and return true if other drivers
		/// are required to be functional before this driver can function.
		/// </summary>
		bool HaveDependants(void) const;

		/// <summary>
		/// For drivers that support a connection based functionality this method will
		/// return true and false. Other drivers will return true by default.
		/// </summary>
		virtual bool IsConnected(void) const;

		/// <summary>
		/// This will cause the driver to full reset its state back to that of a newly
		/// initialized device.
		/// </summary>
		virtual void Reset(void);

		/// <summary>
		/// Stop the driver from functioning by turning off features that cause it to do
		/// things.
		/// </summary>
		virtual void Disable(void);

		/// <summary>
		/// Determine if the descriptor matches this drivers Guid
		/// </summary>
		/// <param name="rhs"></param>
		bool operator==(const scDeviceDescriptor& rhs) const;

		/// <summary>
		/// Determine if the descriptor does not matches this drivers Guid
		/// </summary>
		/// <param name="rhs"></param>
		bool operator!=(const scDeviceDescriptor& rhs) const;

		/// <summary>
		/// Get the current enable/disable state of the device.
		/// </summary>
		scEnableState_t State(void) const;

		/// <summary>
		/// Get the current setup state of the device. This driver must manage it's state
		/// during the initialization process.
		/// </summary>
		SharedCore::scDeviceGeneric::DeviceSetupState_t SetupState(void) const;

	protected:
		/// <summary>
		/// Set the state of the device
		/// </summary>
		/// <param name="next"></param>
		void State( scEnableState_t next);

		/// <summary>
		/// Configure the initialization state of the device.
		/// </summary>
		/// <param name="next"></param>
		void SetupState(SharedCore::scDeviceGeneric::DeviceSetupState_t next);

		/// <summary>
		/// Construct the class using the Guid
		/// </summary>
		/// <param name="descriptor">Describes the descriptor for this device.</param>
		scDeviceGeneric( const scDeviceDescriptor& descriptor );

		/// <summary>
		/// Construct the class using the Guid and one dependant
		/// </summary>
		/// <param name="descriptor">Describes the descriptor for this device.</param>
		/// <param name="dependant">the sole dependant of the device.</param>
		scDeviceGeneric( const scDeviceDescriptor& descriptor, const scDeviceDescriptor& dependant );

		/// <summary>
		/// Construct the class using the Guid and multiple dependants
		/// </summary>
		/// <param name="descriptor">Describes the descriptor for this device.</param>
		/// <param name="nCount">Number of dependants listed.</param>
		scDeviceGeneric( const scDeviceDescriptor& descriptor, int nCount, ... );

		/// <summary>
		/// This method is need to allow subclasses to implement the variable length
		/// argument. 
		/// </summary>
		void AddDependants( va_list vl, int nCount );

		/// <summary>
		/// Set the last error code.
		/// </summary>
		void SetLastError( uint32_t nLastError )
		{
			_nLastError = nLastError; 
		}

	private:

		/// <summary>
		/// The identity of this driver.
		/// </summary>
		SharedCore::scDeviceDescriptor							_Guid;

		/// <summary>
		/// A list of the drivers that must exist prior to this driver becoming active.
		/// This list is used to protect the startup integrity of the initialization
		/// sequence.
		/// </summary>
		vector<scDeviceDescriptor>								_dependants;

		/// <summary>
		/// This represents the last error the driver has encountered. Child class driver
		/// implementations will use this as necessary.
		/// </summary>
		uint32_t												_nLastError;

		/// <summary>
		/// Tracks the current setup state of the device.
		/// </summary>
		SharedCore::scDeviceGeneric::DeviceSetupState_t			_nSetupState;

		/// <summary>
		/// Tracks the current activity state of the device.
		/// </summary>
		scEnableState_t											_nState;

		/// <summary>
		/// Prohibit the copy functions
		/// </summary>
		/// <param name="source">Describes the descriptor for this device.</param>
		scDeviceGeneric( const scDeviceGeneric& source )
		{
		}

		/// <summary>
		/// Prohibit the copy functions
		/// </summary>
		/// <param name="source">Describes the descriptor for this device.</param>
		scDeviceGeneric& operator=(const scDeviceGeneric& source )
		{
			return *this;
		}

	};

}
#endif // !defined(__SCDEVICEGENERIC_H__INCLUDED_)
