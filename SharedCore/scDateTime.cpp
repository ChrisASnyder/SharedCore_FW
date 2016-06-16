//==============================================================================
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
// File Name:       scDateTime.cpp
// Created By:      Christopher A. Snyder
// Creation Date:   6/1/2014
// $Id: scDateTime.cpp $
//
//==============================================================================

#include "scDateTime.h"
#include <stdio.h>

using SharedCore::scDateTime;


/// <summary>
/// Constructs scDateTime objects in various ways
/// </summary>
/// <param name="nYear"></param>
/// <param name="nMonth"></param>
/// <param name="nDay"></param>
/// <param name="nHour"></param>
/// <param name="nMin"></param>
/// <param name="nSec"></param>
scDateTime::scDateTime(
	int nYear,
	int nMonth,
	int nDay,
	int nHour,
	int nMin,
	int nSec )
	: m_time()
{
	struct tm now;
	now.tm_sec = nSec;
	now.tm_min = nMin;
	now.tm_hour = nHour;
	now.tm_mday = nDay;
	now.tm_mon = nMonth - 1;
	now.tm_year = nYear - 1900;
	now.tm_wday = 0;
	now.tm_yday = 0;
	now.tm_isdst = 0;
	m_time = mktime( &now );
}

/// <summary>
/// Constructs scDateTime objects in various ways
/// </summary>
/// <param name="source"></param>
scDateTime::scDateTime( const scDateTime& source )
	: m_time(source.m_time)
{
	if ( this != &source )
	{
		m_time = source.m_time;
	}
}


/// <summary>
/// The assignment operator.
/// </summary>
/// <param name="source"></param>
scDateTime& scDateTime::operator=( const scDateTime& source )
{
	if ( this != &source )
	{
		m_time = source.m_time;
	}
	return *this;
}

/// <summary>
/// Converts the time to a simple string in the HH:MM:SS time format
/// the length must be at least 9 characters in order for the sprintf
/// function to fill in the string.
/// </summary>
/// <param name="pString">buffer to receive the formatted string</param>
/// <param name="nLength">length of the provided buffer must be at least 9</param>
void scDateTime::TimeString( char* pString, uint32_t nLength )
{
#ifdef USE_SECURE_CALLS
	tm ltime;
	if ( localtime_s( &ltime, &m_time ) == 0 )
	{
		sprintf_s( pString, nLength, "%02u:%02u:%02u", ltime.tm_hour, ltime.tm_min, ltime.tm_sec);
	}
#else
	tm* pltime =  localtime( &m_time );
	if ( nLength >= 9 )
	{
		sprintf( pString, "%02u:%02u:%02u",
			pltime->tm_hour, pltime->tm_min, pltime->tm_sec);
	}
#endif
}

/// <summary>
/// Converts the date to a simple string in the YYYY-MM-DD date format
/// the length must be at least 11 characters in order for the sprintf
/// function to fill in the string.
/// </summary>
/// <param name="pString">buffer to receive the formatted string</param>
/// <param name="nLength">length of the provided buffer must be at least 11</param>
void scDateTime::DateString( char* pString, uint32_t nLength )
{
	if ( nLength >= 11 )
	{
#ifdef USE_SECURE_CALLS
		sprintf_s( pString, nLength, "%04u-%02u-%02u",
			GetYear(), GetMonth(), GetDay() );
#else
		sprintf( pString, "%04u-%02u-%02u",
			GetYear(), GetMonth(), GetDay() );
#endif
	}
}

/// <summary>
/// The day of the week using the STM definition for the RTC
/// Gets the day of the week 1-7, 1-Monday, 2-Tuesday, ... 7-Sunday
/// </summary>
int scDateTime::GetDayOfWeek_STM(void) const
{
	int nResult = 0;
#ifdef USE_SECURE_CALLS
	tm ltime;
	if ( localtime_s( &ltime, &m_time ) == 0 )
	{
		nResult = ltime.tm_wday;
	}
#else
	nResult = localtime( &m_time )->tm_wday;
#endif
	return ( nResult == 0 ? 7 : nResult );
}


#ifdef STM32F4XX
void scDateTime::GetTime( RTC_AlarmTypeDef* pAlarm ) const
{
	struct tm* pTime = gmtime( &m_time );
	struct tm* ltime =  localtime( &m_time );

	RTC_AlarmStructInit( pAlarm );

	/*!< Specifies the RTC Time Hour.
	This parameter must be set to a value in the 0-12 range
	if the RTC_HourFormat_12 is selected or 0-23 range if
	the RTC_HourFormat_24 is selected. */
	pAlarm->RTC_AlarmTime.RTC_Hours = ltime->tm_hour;
	/*!< Specifies the RTC Time Minutes. This parameter must be set to a value in the 0-59 range. */
	pAlarm->RTC_AlarmTime.RTC_Minutes = ltime->tm_min;
	/*!< Specifies the RTC Time Seconds. This parameter must be set to a value in the 0-59 range. */
	pAlarm->RTC_AlarmTime.RTC_Seconds = ltime->tm_sec;
	/*!< Specifies the RTC AM/PM Time. This parameter can be a value of @ref RTC_AM_PM_Definitions */
	pAlarm->RTC_AlarmTime.RTC_H12 = RTC_H12_AM;   // Does the alarm work with a 24 hour clock?

	// By setting the alarm mask for the date, the specific time is what will
	// trigger the alarm.
	pAlarm->RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
	pAlarm->RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	pAlarm->RTC_AlarmDateWeekDay = ltime->tm_mday;
}

#endif
