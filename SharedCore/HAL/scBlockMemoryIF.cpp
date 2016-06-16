//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          All Rights Reserved
//
// File Name:		scBlockMemoryIF.h
// Created By:		Christopher Snyder
// Creation Date:	23-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include "scBlockMemoryIF.h"
#include "scErrorCodes.h"

using namespace SharedCore;
using SharedCore::HAL::scBlockMemoryIF;

/// <summary>
/// Destructor
/// </summary>
scBlockMemoryIF::~scBlockMemoryIF()
{

}

/// <summary>
/// Construct the class using the Guid
/// </summary>
/// <param name="descriptor">Describes the descriptor for this device.</param>
scBlockMemoryIF::scBlockMemoryIF( const scDeviceDescriptor& descriptor )
	: scDeviceGeneric( descriptor )
{
}

/// <summary>
/// Construct the class using the Guid and one dependent
/// </summary>
/// <param name="descriptor">Describes the descriptor for this device.</param>
/// <param name="dependant">the sole dependent of the device.</param>
scBlockMemoryIF::scBlockMemoryIF( const scDeviceDescriptor& descriptor, const scDeviceDescriptor& dependant )
	: scDeviceGeneric( descriptor, dependant )
{
}

/// <summary>
/// Construct the class using the Guid and multiple dependents
/// </summary>
/// <param name="descriptor">Describes the descriptor for this device.</param>
/// <param name="nCount">Number of dependents listed.</param>
/// <param name="prm1"></param>
scBlockMemoryIF::scBlockMemoryIF( const scDeviceDescriptor& descriptor, int nCount, ... )
	: scDeviceGeneric( descriptor )
{
	va_list vl;
	va_start( vl, nCount );
	AddDependants( vl, nCount );
	va_end(vl);
}



/// <summary>
/// This operation will select internal components if available. Some drivers can
/// support multiple interfaces and a single driver can switch between them, such
/// as a disk drive.
/// </summary>
uint32_t scBlockMemoryIF::Select( uint32_t nItem )
{
	return ERROR_SC_DEVICE_FEATURE_UNSUPPORTED;
}

/// <summary>
/// Obtain the index of the currently selected item.
/// </summary>
uint32_t scBlockMemoryIF::Selected(void) const
{
	return 0;
}