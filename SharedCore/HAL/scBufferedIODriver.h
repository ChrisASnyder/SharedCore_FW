//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          All Rights Reserved
//
// File Name:		scBufferedIODriver.h
// Created By:		Christopher Snyder
// Creation Date:	25-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCBUFFEREDIODRIVER_H__INCLUDED_)
#define __SCBUFFEREDIODRIVER_H__INCLUDED_

#include "scDeviceGeneric.h"
#include "sctBufferedIODriver.h"
#include "sctEventIODriver.h"
#include "sctCallbackIoDriver.h"

namespace SharedCore
{
	namespace HAL
	{
		typedef sctBufferedIODriver<scDeviceGeneric>		scBufferIODriver;
		typedef sctEventIODriver<scDeviceGeneric>			scEventIODriver;
		typedef sctCallbackIODriver<scDeviceGeneric>		scCallbackIODriver;

		typedef sctEventIODriver<sctBufferedIODriver<scDeviceGeneric> >	scBufferedEventIODriver;

		typedef sctEventIODriver<sctCallbackIODriver< sctBufferedIODriver<scDeviceGeneric> > >
			scBufferedEventCBIODriver;

	}
}
#endif // !defined(__SCBUFFEREDIODRIVER_H__INCLUDED_)
