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
//
// File Name:      scTypes.h
// Created By:     Christopher A. Snyder
// $Id: $
//
//==============================================================================

#ifndef __FTYPES_H__
#define __FTYPES_H__

#include "scConf.h"

#ifdef __cplusplus
// These are enums that are defined within the entire shared core framework
namespace SharedCore
{
	typedef enum
	{
		scDisabled,
		scEnabled,
	} scEnableState_t;
};
#endif

#ifndef FALSE
	#define FALSE 0
	#define TRUE !FALSE
#endif

#ifndef NULL
	#define NULL 0
#endif

#define BIT0   (1 << 0)
#define BIT1   (1 << 1)
#define BIT2   (1 << 2)
#define BIT3   (1 << 3)
#define BIT4   (1 << 4)
#define BIT5   (1 << 5)
#define BIT6   (1 << 6)
#define BIT7   (1 << 7)
#define BIT8   (1 << 8)
#define BIT9   (1 << 9)
#define BIT10  (1 << 10)
#define BIT11  (1 << 11)
#define BIT12  (1 << 12)
#define BIT13  (1 << 13)
#define BIT14  (1 << 14)
#define BIT15  (1 << 15)

// = Types =====================================================================

/*  typedefs are here  */
typedef unsigned char bool_t;

//#ifdef _WIN32
//	typedef unsigned char uint8_t;
//	typedef signed char int8_t;
//	typedef unsigned short uint16_t;
//	typedef signed short int16_t;
//#ifndef uint32_t
//	typedef unsigned long uint32_t;
//#endif
//#ifndef int32_t
//	typedef signed long int32_t;
//#endif
//#ifndef uint64_t
//	typedef unsigned long long uint64_t;
//#endif
//	typedef signed long long int64_t;
//#else
	#include <stdint.h>
//#endif

#ifndef BYTE
	typedef unsigned char BYTE;
#endif

#ifndef WIN32
	// WORD type definition
	#ifndef _WORD_DEF_                     // Compiler Specific, for Little Endian
		#define _WORD_DEF_
		typedef union {uint16_t i; uint8_t c[2];} WORD;
		// All words sent to and received from the host are little endian, this is
		// switched by software when necessary.  These sections of code have been
		// marked with "Compiler Specific" as above for easier modification
	#endif

#ifndef LSB
	#define LSB 0
#endif
#ifndef MSB 
	#define MSB 1
#endif

#endif   // _WORD_DEF_

#ifndef DWORD
	typedef unsigned long DWORD;
#endif

#ifndef UINT
	typedef unsigned int UINT;
#endif

#endif
