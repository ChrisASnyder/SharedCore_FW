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


#if !defined(__SHARED_CORE_ERROR_CODES_H)
#define __SHARED_CORE_ERROR_CODES_H

#ifdef WIN32
#	pragma once
#endif

/// <summary>
/// This is the list of device identifiers that exist in this system.
/// </summary>

#ifndef ERROR_SUCCESS
    #define ERROR_SUCCESS												(0)
#endif

// The SharedCore framework reserves error codes between values of 1000-2000
// the application may user error codes in other ranges.
#define ERROR_SC_GENERIC_ERROR											(1000)

#define ERROR_SC_MEMORY_ALLOCATION_FAILURE								(ERROR_SC_GENERIC_ERROR + 1)
#define ERROR_SC_MEMORY_INVALID_POINTER									(ERROR_SC_GENERIC_ERROR + 2)

#define ERROR_SC_SDCARD_FAILURE											(ERROR_SC_GENERIC_ERROR + 3)
#define ERROR_SC_EXTERNAL_MEMORY_FAILURE								(ERROR_SC_GENERIC_ERROR + 4)
#define ERROR_SC_RTC_INIT_FAILURE										(ERROR_SC_GENERIC_ERROR + 5)
#define ERROR_SC_BUFFER_OVERFLOW										(ERROR_SC_GENERIC_ERROR + 6)
#define ERROR_SC_MESSAGEFACTORY_FULL									(ERROR_SC_GENERIC_ERROR + 7)
#define ERROR_SC_DEVICE_FEATURE_UNSUPPORTED								(ERROR_SC_GENERIC_ERROR + 8)
#define ERROR_SC_ITEM_ALREADY_EXISTS									(ERROR_SC_GENERIC_ERROR + 9)

#define ERROR_SC_MODULE_NOT_FOUND										(ERROR_SC_GENERIC_ERROR + 10)

#endif // !defined(__SHARED_CORE_ERROR_CODES_H)

