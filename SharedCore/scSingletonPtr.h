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

#if !defined(__scSingletonPtr_H_INCLUDE)
#define __scSingletonPtr_H_INCLUDE

#include "scTypes.h"

namespace SharedCore
{
	/// <summary>
	/// </summary>
	template<class T>
	class scSingletonPtr
	{
	public:

		/// <summary>
		/// Construct the object in the default unknown state.
		/// </summary>
		scSingletonPtr( void )
			: m_pInstance( NULL )
		{
			m_pInstance = new T();
		}

		/// <summary>
		/// Destructor.
		/// </summary>
		virtual ~scSingletonPtr() 
		{
			delete m_pInstance;
		}

		/// <summary>
		/// Get the value of the state
		/// </summary>
		const T* operator()() const
		{ return m_pInstance; }

		T* Ptr(void)
		{
			return m_pInstance;
		}

	private:
		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="source"></param>
		scSingletonPtr( const scSingletonPtr& source )
		{}

		/// <summary>
		/// Assignment operator.
		/// </summary>
		/// <param name="source"></param>
		scSingletonPtr& operator=( const scSingletonPtr& source )
		{ if ( this != &source ) ; return *this; }

		/// <summary>
		/// The pointer to the singleton object
		/// </summary>
		T*		m_pInstance;
	};
}	// Namespace SharedCore

#endif // !defined(__scSingletonPtr_H_INCLUDE)
