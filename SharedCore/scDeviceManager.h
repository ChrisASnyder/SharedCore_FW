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
// File Name:		scDeviceManager.h
// Created By:		Christopher Snyder
// Creation Date:	14-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCDEVICEMANAGER_H__INCLUDED_)
#define __SCDEVICEMANAGER_H__INCLUDED_

#include <vector>
#include "scTypes.h"
#include "scDeviceDescriptor.h"
#include "scDeviceGeneric.h"
#include "scConfigureDevice.h"

namespace SharedCore
{
	/// <summary>
	/// The device manage is a base class that will organize and contain all the device
	/// objects managed through the scDeviceGeneric class. The child classes will
	/// create all the devices and add them to the management list. Then the
	/// Initialize() method will be called which will in turn initialize all the
	/// devices in the required order. The configure and unconfigure methods are also
	/// provided to change the state of a device.
	/// </summary>
	class scDeviceManager
	{
	public:
		/// <summary>
		/// The class destructor 
		/// </summary>
		virtual ~scDeviceManager();

		/// <summary>
		/// This method is called only once during the power up sequence of the system.
		/// When called all drivers stored in the driver list will be initialized in the
		/// proper order.
		/// </summary>
		virtual void Initialize(void);

		/// <summary>
		/// This method will find the specified driver in the list and return it. False is
		/// return if the driver is not found.
		/// </summary>
		/// <param name="dd">The driver to be located</param>
		/// <param name="pDevice">Pointer to the driver. This is set when the device is
		/// found, otherwise it is NULL.</param>
		virtual bool GetDevice( const scDeviceDescriptor& dd, scDeviceGeneric** pDevice );

		/// <summary>
		/// This method will add a new device to the device list. This is usually called by
		/// the device manage, but may also be called from the Exec or a factory that is
		/// creating all the initial drivers.
		/// </summary>
		/// <param name="pDevice">Pointer to the new driver. Once this pointer is passed
		/// into the manager the manager will own and delete it. In general, this class
		/// never terminates, however, for test reasons, it will perform deletion on all
		/// drivers in the device list to prevent memory leaks in test applications.</param>
		size_t Add( scDeviceGeneric* pDevice );

		/// <summary>
		/// This method will call Enable on the specified driver
		/// </summary>
		/// <param name="dd">The device to locate</param>
		/// <param name="bIncludeDependants">When this is true all dependant devices will also get Enabled.</param>
		virtual void Enable( const scDeviceDescriptor& dd, bool bIncludeDependants );

		/// <summary>
		/// This method will call disable on the specified driver
		/// </summary>
		/// <param name="dd">The device to locate</param>
		/// <param name="bIncludeDependants">When this is true all dependant devices will also get disabled.</param>
		virtual void Disable( const scDeviceDescriptor& dd, bool bIncludeDependants );

		/// <summary>
		/// This method will perform a reset on the specified driver.
		/// </summary>
		/// <param name="dd">The device to be reset.</param>
		/// <param name="bIncludeDependants">When this is true all dependant devices will also get reset.</param>
		virtual void Reset( const scDeviceDescriptor& dd, bool bIncludeDependants );

		/// <summary>
		/// This method will call the configure method for the specified driver.
		/// </summary>
		/// <param name="dd">The device to be reset.</param>
		virtual void Configure( const scDeviceDescriptor& dd, const scConfigureDevice& config );

		/// <summary>
		/// This method will call the unconfigure method for the specified driver.
		/// </summary>
		/// <param name="dd">The device to be reset.</param>
		virtual void Unconfigure( const scDeviceDescriptor& dd, const scConfigureDevice& config );

	protected:
		/// <summary>
		/// The constructor is protected since this class is never to be used alone. The
		/// children will create and manage the drivers.
		/// </summary>
		scDeviceManager(void);

		/// <summary>
		/// This method will call the configure method for the specified driver. This is a
		/// recursive method that will iterate through all the drivers calling their
		/// initialize method. If a driver has dependencies on other drivers, then the
		/// dependent drivers will get Initialized first.
		/// </summary>
		/// <param name="dd">The device to be reset.</param>
		/// <param name="bConfigured">This is a flag to indicate that the specified driver
		/// has already been configured.</param>
		/// <param name="nCallDepth">Used to cap the call depth of this recursive function.
		/// </param>
		void DeviceInitialize( const scDeviceDescriptor& dd, uint8_t nCallDepth );

	private:
		/// <summary>
		/// This is the list of devices known and managed by the device manager.
		/// </summary>
		vector<scDeviceGeneric*>			_devices;
	};
}

#endif // !defined(__SCDEVICEMANAGER_H__INCLUDED_)
