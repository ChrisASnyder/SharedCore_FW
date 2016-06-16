//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          chris.snyder@commongroundss.com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  any later version.
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
// File Name:		scStandardHeader_t.h
// Created By:		Christopher Snyder
// Creation Date:	15-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCSTANDARDHEADER_T_H__INCLUDED_)
#define __SCSTANDARDHEADER_T_H__INCLUDED_

#include "scTypes.h"

namespace SharedCore
{
	#define STANDARD_HEADER_PREFIX 0xAA

	#pragma pack(1)

	typedef struct
	{
		uint8_t				_prefix;
		uint8_t				_nFlags;
		uint8_t				_nSequence;
		uint16_t			_nDestination;
		uint16_t			_nSource;
		uint16_t			_nMessageID;
		uint32_t			_nLength;
		uint8_t				_nCheckSum;
	} scStandardHeader_t;
	#pragma pack()
}	// namespace SharedCore

#endif // __SCSTANDARDHEADER_T_H__INCLUDED_
