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
// File Name:       scTriState.h
// Created By:      Christopher A. Snyder
// Creation Date:   6/1/2014
// $Id: scTriState.h  $
//
//==============================================================================

#if !defined(__scTriState_h__INLCUDE)
#define __scTriState_h__INLCUDE

#include "scTypes.h"

namespace SharedCore
{
	/// <summary>
	/// This is an implementation of the fuzzy logic concept of a tri-state boolean expression where 
	/// it's possible for the value to be in an unknown state. This is useful for boolean expressions
	/// that initialize in an unknown state and will remain there until some event occurs to set them
	/// to a specific value.
	/// </summary>
	class scTriState
	{
	public:
		typedef enum
		{
			scFalse,
			scTrue,
			scUnknown
		} scTriState_t;

		/// <summary>
		/// Construct the object in the default unknown state.
		/// </summary>
		scTriState( void )
			: m_value( scUnknown )
		{}

		/// <summary>
		/// Construct in a boolean true or false state.
		/// </summary>
		/// <param name="bTrue"></param>
		scTriState( bool bTrue )
			: m_value( (bTrue ? scTrue : scFalse) )
		{}

		/// <summary>
		/// Construct from the enum explicitly.
		/// </summary>
		/// <param name="value"></param>
		scTriState(scTriState::scTriState_t value)
			: m_value( value )
		{}

		/// <summary>
		/// Destructor.
		/// </summary>
		virtual ~scTriState() 
		{}

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="source"></param>
		scTriState( const scTriState& source )
			: m_value(source.m_value)
		{}

		/// <summary>
		/// Assignment operator.
		/// </summary>
		/// <param name="source"></param>
		scTriState& operator=( const scTriState& source )
		{ if ( this != &source ) m_value = source.m_value; return *this; }

		/// <summary>
		/// Assignment operator.
		/// </summary>
		/// <param name="right"></param>
		scTriState& operator=( const bool& right )
		{ m_value = (right ? scTrue : scFalse); return *this; }

		/// <summary>
		/// Assignment operator.
		/// </summary>
		/// <param name="right"></param>
		scTriState& operator=( const scTriState_t& right )
		{ m_value = right; return *this; }

		/// <summary>
		/// Get the value of the state
		/// </summary>
		scTriState::scTriState_t operator()() const
		{ return m_value; }

		/// <summary>
		/// Comparison operator.
		/// </summary>
		/// <param name="right"></param>
		bool operator==( const scTriState& right ) const
		{ return m_value == right.m_value; }

		/// <summary>
		/// Comparison operator.
		/// </summary>
		/// <param name="right"></param>
		bool operator!=( const scTriState& right ) const
		{ return m_value != right.m_value; }

		/// <summary>
		/// Comparison operator.
		/// </summary>
		/// <param name="right"></param>
		bool operator==( bool right ) const
		{ return True() == right; }

		/// <summary>
		/// Comparison operator.
		/// </summary>
		/// <param name="right"></param>
		bool operator!=( bool right ) const
		{ return True() != right; }

		/// <summary>
		/// Comparison operator.
		/// </summary>
		/// <param name="right"></param>
		bool operator==( const scTriState_t& right ) const
		{ return m_value == right; }

		/// <summary>
		/// Comparison operator.
		/// </summary>
		/// <param name="right"></param>
		bool operator!=( const scTriState_t& right ) const
		{ return m_value != right; }

		/// <summary>
		/// Returns true if the current state is a boolean true.
		/// </summary>
		bool True(void) const
		{ return (m_value == scTrue ? true : false); }

		/// <summary>
		/// Returns a logical true if the current state is a boolean false.
		/// </summary>
		bool False(void) const
		{ return (m_value == scFalse ? true : false); }

		/// <summary>
		/// Returns a logical true if the current state is unknown.
		/// </summary>
		bool Unknown(void) const
		{ return (m_value == scUnknown ? true : false); }

	private:
		/// <summary>
		/// This is the tri-state value used to track True False or unknown.
		/// </summary>
		scTriState::scTriState_t m_value;
	};
}	// Namespace SharedCore
#endif // !defined(__scTriState_h__INLCUDE)
