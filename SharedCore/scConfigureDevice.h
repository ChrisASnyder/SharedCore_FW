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
// File Name:		scConfigureDevice.h
// Created By:		Christopher Snyder
// Creation Date:	14-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCCONFIGUREDEVICE_H__INCLUDED_)
#define __SCCONFIGUREDEVICE_H__INCLUDED_

#include "scTypes.h"

namespace SharedCore
{
	/// <summary>
	/// This is a simple base class that can be used to configure a device. In most
	/// cases a Device will have it's own configure class defined that is derived from
	/// this class that will contain the necessary parameters to fully configure the
	/// device. If the device doesn't require configuration parameters, then simple
	/// pass this class as a placeholder for the Configure Call.
	/// </summary>
	class scConfigureDevice
	{
	public:
		/// <summary>
		/// Simple Destructor
		/// </summary>
		virtual ~scConfigureDevice();

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="source">copy source</param>
		scConfigureDevice(const scConfigureDevice& source);
		
		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="nValue">The default value of the parameter</param>
		scConfigureDevice(uint32_t nValue = 0);
		
		/// <summary>
		/// Get method for the parameter
		/// </summary>
		uint32_t Parameter() const;
		
		/// <summary>
		/// Set method for the parameter
		/// </summary>
		/// <param name="nValue">32-bit generic parameter.</param>
		scConfigureDevice& Parameter(uint32_t nValue);
		
		/// <summary>
		/// Assignment operator.
		/// </summary>
		/// <param name="nValue">Parameter value</param>
		scConfigureDevice& operator=(uint32_t nValue);

		/// <summary>
		/// Assignment operator.
		/// </summary>
		/// <param name="source">Parameter value</param>
		scConfigureDevice& operator=(const scConfigureDevice& source);

	private:
		/// <summary>
		/// This base class simply contains a single 32-bit parameter that can be used as
		/// needed or not used at all.
		/// </summary>
		uint32_t					_nParameter;

	};

}
#endif // !defined(__SCCONFIGUREDEVICE_H__INCLUDED_)
