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
//
// File Name:       scDateTime.h
// Created By:      Christopher A. Snyder
// Creation Date:   6/1/2014
// $Id: scDateTime.h $
//
//==============================================================================

#if !defined(__scDateTime_H__INCLUDED_)
#define __scDateTime_H__INCLUDED_

#include "scTypes.h"
#include <time.h>
#include "scTimeSpan.h"

#if defined( STM32F4XX )
extern "C"
{
#include "stm32f4xx_rtc.h"
}
#endif

#ifdef _WIN32
	#define USE_SECURE_CALLS
#endif

namespace SharedCore
{

	/// <summary>
	/// scDateTime does not have a base class.
	/// scDateTime values are based on coordinated universal time (UTC), which is equivalent to Coordinated Universal
	/// time (Greenwich Mean Time, GMT).
	/// When you create a scDateTime object, set the nDST parameter to 0 to indicate that standard time is in effect,
	/// or to a value larger than 0 to indicate that daylight saving time is in effect, or to a value less than zero
	/// to have the C run-time library code compute whether standard time or daylight saving time is in effect.
	/// tm_isdst is a required field. If not set, its value is undefined and the return value from mktime is
	/// unpredictable. If timeptr points to a tm structure returned by a previous call to asctime_s, _gmtime_s, or
	/// localtime_s, the tm_isdst field contains the correct value.
	/// A companion class, scDateTime, represents a time interval.
	/// The scDateTime and scTimeSpan classes are not designed for derivation. Because there are no virtual functions,
	/// the size of scDateTime and scTimeSpan objects is exactly 8 bytes. Most member functions are inline.
	/// </summary>
	class scDateTime
	{
	public:
		/// <summary>
		/// Constructs scDateTime objects in various ways
		/// </summary>
		/// <param name="source"></param>
		scDateTime( const scDateTime& source );

		/// <summary>
		/// Constructs scDateTime objects in various ways
		/// </summary>
		/// <param name="time"></param>
		inline scDateTime( time_t time = 0 )
			: m_time(time) { }

		/// <summary>
		/// Constructs scDateTime objects in various ways
		/// </summary>
		/// <param name="nYear"></param>
		/// <param name="nMonth"></param>
		/// <param name="nDay"></param>
		/// <param name="nHour"></param>
		/// <param name="nMin"></param>
		/// <param name="nSec"></param>
		scDateTime(
			int nYear,
			int nMonth,
			int nDay,
			int nHour,
			int nMin,
			int nSec );

		/// <summary>
		/// The assignment operator.
		/// </summary>
		/// <param name="source"></param>
		scDateTime& operator=( const scDateTime& source );

		/// <summary>
		/// The assignment operator.
		/// </summary>
		/// <param name="time"></param>
		inline scDateTime& operator=( time_t time)
		{ m_time = time; return *this; }

		/// <summary>
		/// These operators add and subtract a scDateTime object to and from this scDateTime  object
		/// </summary>
		/// <param name="span"></param>
		inline scDateTime& operator+=( const scTimeSpan& span)
		{ m_time += span.GetTimeSpan(); return( *this ); }

		/// <summary>
		/// These operators add and subtract a scDateTime object to and from this scDateTime  object
		/// </summary>
		/// <param name="span"></param>
		inline scDateTime& operator-=( const scTimeSpan& span)
		{ m_time -= span.GetTimeSpan();  return( *this ); }

		/// <summary>
		/// These operators add and subtract a scDateTime object to and from this scDateTime  object
		/// </summary>
		/// <param name="time"></param>
		inline scTimeSpan operator-( const scDateTime& right) const
		{ return( scTimeSpan( m_time-right.m_time ) ); }

		/// <summary>
		/// These operators add and subtract a scTimeSpan object to and from this scDateTime  object
		/// </summary>
		/// <param name="span"></param>
		inline scDateTime operator-( const scTimeSpan& span) const
		{ return( scDateTime( m_time-span.GetTimeSpan() ) ); }

		/// <summary>
		/// These operators add and subtract a scDateTime object to and from this scDateTime  object
		/// </summary>
		/// <param name="span"></param>
		inline scDateTime operator+( const scTimeSpan& span) const
		{ return( scDateTime( m_time+span.GetTimeSpan() ) ); }

		/// <summary>
		/// Comparison operators
		/// </summary>
		/// <param name="right"></param>
		inline bool operator==( const scDateTime& right) const
		{ return m_time == right.m_time; }

		/// <summary>
		/// Comparison operators
		/// </summary>
		/// <param name="right"></param>
		inline bool operator!=( const scDateTime& right) const
		{ return m_time != right.m_time; }

		/// <summary>
		/// Comparison operators
		/// </summary>
		/// <param name="right"></param>
		inline bool operator<( const scDateTime& right) const
		{ return m_time < right.m_time; }

		/// <summary>
		/// Comparison operators
		/// </summary>
		/// <param name="right"></param>
		inline bool operator>( const scDateTime& right) const
		{ return m_time > right.m_time; }

		/// <summary>
		/// Comparison operators
		/// </summary>
		/// <param name="right"></param>
		inline bool operator<=( const scDateTime& right) const
		{ return m_time <= right.m_time; }

		/// <summary>
		/// Comparison operators
		/// </summary>
		/// <param name="right"></param>
		inline bool operator>=( const scDateTime& right) const
		{ return m_time >= right.m_time; }

		/// <summary>
		/// Get the current time operator
		/// </summary>
		inline time_t operator()(void) const
		{ return m_time; }

		/// <summary>
		/// Gets the current time in seconds
		/// </summary>
		inline time_t GetTime(void) const
		{ return m_time; }

		/// <summary>
		/// Gets the year in a YYYY format
		/// </summary>
		inline int GetYear(void) const
#ifdef USE_SECURE_CALLS
		{ 
			tm lt;
			localtime_s( &lt, &m_time ); 
			return lt.tm_year + 1900;
		}
#else
		{ return localtime( &m_time )->tm_year + 1900; }
#endif

		/// <summary>
		/// Gets the month, 1-12
		/// </summary>
		inline int GetMonth(void) const
#ifdef USE_SECURE_CALLS
		{ 
			tm lt;
			localtime_s( &lt, &m_time ); 
			return lt.tm_mon + 1;
		}
#else
		{ return localtime( &m_time )->tm_mon + 1; }
#endif

		/// <summary>
		/// Gets the day 1-31.
		/// </summary>
		inline int GetDay(void) const
#ifdef USE_SECURE_CALLS
		{ 
			tm lt;
			localtime_s( &lt, &m_time ); 
			return lt.tm_mday;
		}
#else
		{ return localtime( &m_time )->tm_mday; }
#endif
		/// <summary>
		/// Gets the hour 0-23
		/// </summary>
		inline int GetHour(void) const
#ifdef USE_SECURE_CALLS
		{ 
			tm lt;
			localtime_s( &lt, &m_time ); 
			return lt.tm_hour;
		}
#else
		{ return localtime( &m_time )->tm_hour; }
#endif
		/// <summary>
		/// gets the minute 0-59
		/// </summary>
		inline int GetMinute(void) const
#ifdef USE_SECURE_CALLS
		{ 
			tm lt;
			localtime_s( &lt, &m_time ); 
			return lt.tm_min;
		}
#else
		{ return localtime( &m_time )->tm_min; }
#endif
		/// <summary>
		/// Gets the second field 0-59
		/// </summary>
		inline int GetSecond(void) const
#ifdef USE_SECURE_CALLS
		{ 
			tm lt;
			localtime_s( &lt, &m_time ); 
			return lt.tm_sec;
		}
#else
		{ return localtime( &m_time )->tm_sec; }
#endif

		/// <summary>
		/// Gets the day of the week as per the struct tm definition
		/// where 0=Sunday, 1=Monday...
		/// </summary>
		inline int GetDayOfWeek(void) const
#ifdef USE_SECURE_CALLS
		{ 
			struct tm lt;
			localtime_s( &lt, &m_time ); 
			return lt.tm_wday;
		}
#else
		{ return localtime( &m_time )->tm_wday; }
#endif

		/// <summary>
		/// The day of the week using the STM definition for the RTC
		/// Gets the day of the week 1-7, 1-Monday, 2-Tuesday, ... 7-Sunday
		/// </summary>
		int GetDayOfWeek_STM(void) const;

		/// <summary>
		/// Converts the time to a simple string in the HH:MM:SS time format
		/// the length must be at least 9 characters in order for the sprintf
		/// function to fill in the string.
		/// </summary>
		/// <param name="pString">buffer to receive the formatted string</param>
		/// <param name="nLength">length of the provided buffer must be at least 9</param>
		void TimeString( char* pString, uint32_t nLength );

		/// <summary>
		/// Converts the date to a simple string in the YYYY-MM-DD date format
		/// the length must be at least 11 characters in order for the sprintf
		/// function to fill in the string.
		/// </summary>
		/// <param name="pString">buffer to receive the formatted string</param>
		/// <param name="nLength">length of the provided buffer must be at least 11</param>
		void DateString( char* pString, uint32_t nLength );

		// added support for the STM32F4xx time functions
	#ifdef STM32F4XX
		/// <summary>
		/// Support for converting between the alarm structure.
		/// </summary>
		void GetTime( RTC_AlarmTypeDef* pAlarm ) const;
	#endif

	private:
		/// <summary>
		/// Current time in seconds
		/// </summary>
		time_t m_time;

	};

}
#endif // !defined(__scDateTime_H__INCLUDED__)

