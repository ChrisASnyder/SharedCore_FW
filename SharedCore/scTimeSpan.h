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
// File Name:       scTimeSpan.h
// Created By:      Christopher A. Snyder
// Creation Date:   6/1/2014
// $Id: scTimeSpan.h $
//
//==============================================================================


#if !defined(__scTimeSpan_H__INCLUDED_)
#define __scTimeSpan_H__INCLUDED_
#include "scTypes.h"
#include <time.h>

namespace SharedCore
{

/// <summary>
/// scTimeSpan does not have a base class.
/// scTimeSpan functions convert seconds to various combinations of days, hours, minutes, and seconds.
/// The scTimeSpan object is stored in a __time64_t structure, which is 8 bytes.
/// A companion class, fwDateTime, represents an absolute time.
/// The fwDateTime and scTimeSpan classes are not designed for derivation. Because there are no virtual functions, the
/// size of both fwDateTime and scTimeSpan objects is exactly 8 bytes. Most member functions are inline.
/// </summary>
class scTimeSpan
{
public:
	/// <summary>
	/// Construct using various parameters
	/// </summary>
	/// <param name="time"></param>
	inline scTimeSpan( time_t time = 0 )
		: m_timeSpan(time) { }

	/// <summary>
	/// Construct using various parameters
	/// </summary>
	/// <param name="lDays"></param>
	/// <param name="nHours"></param>
	/// <param name="nMins"></param>
	/// <param name="nSecs"></param>
	inline scTimeSpan( uint32_t lDays, int nHours, int nMins, int nSecs )
		: m_timeSpan(0)
	{
		m_timeSpan = nSecs + 60* (nMins + 60* (nHours + uint32_t(24) * lDays));
	}

	/// <summary>
	/// Construct using various parameters
	/// </summary>
	/// <param name="nHours"></param>
	/// <param name="nMins"></param>
	/// <param name="nSecs"></param>
	inline scTimeSpan( int nHours, int nMins, int nSecs )
		: m_timeSpan(0)
	{
		m_timeSpan = nSecs + 60* (nMins + 60* (nHours));
	}

	/// <summary>
	/// Construct using various parameters
	/// </summary>
	/// <param name="nMins"></param>
	/// <param name="nSecs"></param>
	inline scTimeSpan( int nMins, int nSecs )
		: m_timeSpan(0)
	{
		m_timeSpan = nSecs + 60* (nMins );
	}



	/// <summary>
	/// Construct using various parameters
	/// </summary>
	/// <param name="source"></param>
	inline scTimeSpan( const scTimeSpan& source )
		: m_timeSpan(source.m_timeSpan) {  }

	/// <summary>
	/// Assignment operator
	/// </summary>
	/// <param name="source"></param>
	scTimeSpan& operator=( const scTimeSpan& source );

	/// <summary>
	/// Returns a value that represents the number of complete days in this scTimeSpan
	/// </summary>
	inline uint64_t GetDays(void) const
	{  return( m_timeSpan/(24*3600) );  }

	/// <summary>
	/// Returns a value that represents the total number of complete hours in this scTimeSpan.
	/// </summary>
	inline uint64_t GetTotalHours(void) const
	{ return( m_timeSpan/3600 ); }

	/// <summary>
	/// Returns a value that represents the number of hours in the current day -23 through 23.
	/// </summary>
	inline uint32_t GetHours(void) const
	{ return( uint32_t( GetTotalHours()-(GetDays()*24) ) ); }

	/// <summary>
	/// Returns a value that represents the total number of complete minutes in this scTimeSpan.
	/// </summary>
	inline uint64_t GetTotalMinutes(void) const
	{ return( m_timeSpan/60 ); }

	/// <summary>
	/// Returns a value that represents the number of minutes in the current hour (–59 through 59).
	/// </summary>
	inline uint32_t GetMinutes(void) const
	{ return( uint32_t( GetTotalMinutes()-(GetTotalHours()*60) ) ); }

	/// <summary>
	/// Returns a value that represents the total number of complete seconds in this scTimeSpan.
	/// </summary>
	inline uint64_t GetTotalSeconds(void) const
	{ return( m_timeSpan ); }

	/// <summary>
	/// Returns a value that represents the number of seconds in the current minute (–59 through 59).
	/// </summary>
	inline uint32_t GetSeconds(void) const
	{ return( uint32_t( GetTotalSeconds()-(GetTotalMinutes()*60) ) ); }

	/// <summary>
	/// Returns the value of the <b>scTimeSpan</b> object
	/// </summary>
	inline time_t GetTimeSpan(void) const
	{ return( m_timeSpan ); }

	/// <summary>
	/// Addition operator
	/// </summary>
	/// <param name="span"></param>
	inline scTimeSpan operator+( const scTimeSpan& span) const
	{ return( scTimeSpan( m_timeSpan+span.m_timeSpan ) ); }

	/// <summary>
	/// Subtraction operator
	/// </summary>
	/// <param name="span"></param>
	inline scTimeSpan operator-( const scTimeSpan& span) const
	{ return( scTimeSpan( m_timeSpan-span.m_timeSpan ) ); }

	/// <summary>
	/// Addition operator
	/// </summary>
	/// <param name="span"></param>
	inline scTimeSpan& operator+=( const scTimeSpan&span)
	{ m_timeSpan += span.m_timeSpan; return( *this ); }

	/// <summary>
	/// Subtraction operator
	/// </summary>
	/// <param name="span"></param>
	inline scTimeSpan& operator-=( const scTimeSpan&span)
	{ m_timeSpan -= span.m_timeSpan; return( *this ); }

	/// <summary>
	/// equality check
	/// </summary>
	/// <param name="span"></param>
	inline bool operator==( const scTimeSpan& span) const
	{ return( m_timeSpan == span.m_timeSpan ); }

	/// <summary>
	/// inequality operator
	/// </summary>
	/// <param name="span"></param>
	inline bool operator!=( const scTimeSpan& span) const
	{ return( m_timeSpan != span.m_timeSpan ); }

	/// <summary>
	/// Comparison operator.
	/// </summary>
	/// <param name="span"></param>
	inline bool operator<( const scTimeSpan& span) const
	{ return( m_timeSpan < span.m_timeSpan ); }

	/// <summary>
	/// Comparison operator.
	/// </summary>
	/// <param name="span"></param>
	inline bool operator>( const scTimeSpan& span) const
	{ return( m_timeSpan > span.m_timeSpan ); }

	/// <summary>
	/// Comparison operator.
	/// </summary>
	/// <param name="span"></param>
	inline bool operator<=( const scTimeSpan& span) const
	{ return( m_timeSpan <= span.m_timeSpan ); }

	/// <summary>
	/// Comparison operator.
	/// </summary>
	/// <param name="span"></param>
	inline bool operator>=( const scTimeSpan& span) const
	{ return( m_timeSpan >= span.m_timeSpan ); }

private:
	/// <summary>
	/// Number of seconds represented by the span
	/// </summary>
	time_t m_timeSpan;
};

}
#endif // !defined(__scTimeSpan_H__INCLUDED_)
