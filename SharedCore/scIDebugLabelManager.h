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
//          UNCLASSIFIED
//==============================================================================
//
// File Name:		scIDebugLabelManager.h
// Created By:		Christopher Snyder
// Creation Date:	14-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCIDEBUGLABELMANAGER_H__INCLUDED_)
#define __SCIDEBUGLABELMANAGER_H__INCLUDED_

#include "scTypes.h"
#include "scDebugLabelCodes.h"

namespace SharedCore
{
	using namespace std;

	/// <summary>
	/// This class is used to define the interface using a pure virtual base class. This will allow
	/// different types of label manager implementations in case the platform does not support the 
	/// default implementation.
	/// </summary>
	class scIDebugLabelManager
	{
	public:
		/// <summary>
		/// Simple Destructor
		/// </summary>
		virtual ~scIDebugLabelManager() {} ;

		/// <summary>
		/// Default Constructor
		/// </summary>
		/// <param name="nMaxLabels">The number of labels that will exist in this system.
		/// </param>
		scIDebugLabelManager(void) {};

		/// <summary>
		/// Get the current state of the label.
		/// </summary>
		/// <param name="nLabel">The label id to lookup.</param>
		virtual scEnableState_t LabelState(uint16_t nLabel) const = 0;

		/// <summary>
		/// Get the current state of the label.
		/// </summary>
		/// <param name="nLabel">The label ID to set</param>
		/// <param name="nState">The new state of the label.</param>
		virtual void  LabelState( uint16_t nLabel, scEnableState_t nState ) = 0;

		/// <summary>
		/// Changes the state for all the labels to match the provided state.
		/// </summary>
		/// <param name="nState">The new state of the label.</param>
		virtual void  LabelState( scEnableState_t nState ) = 0;

		/// <summary>
		/// Get a string description of the specified label.
		/// </summary>
		/// <param name="nLabel">The label to look up</param>
		virtual const char* LabelText(uint16_t nLabel) = 0;

	private:
		/// <summary>
		/// Prevent copy operators
		/// </summary>
		scIDebugLabelManager( const scIDebugLabelManager& source ) {}
		/// <summary>
		/// Prevent copy operators
		/// </summary>
		scIDebugLabelManager& operator=(const scIDebugLabelManager& source ) { return *this; }

	};

}
#endif // !defined(__SCIDEBUGLABELMANAGER_H__INCLUDED_)
