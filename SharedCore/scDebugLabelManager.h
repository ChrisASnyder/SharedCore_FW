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
// File Name:		scDebugLabelManager.h
// Created By:		Christopher Snyder
// Creation Date:	14-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCDEBUGLABELMANAGER_H__INCLUDED_)
#define __SCDEBUGLABELMANAGER_H__INCLUDED_

#include "scTypes.h"
#include "scIDebugLabelManager.h"
#include "scDebugLabelCodes.h"
#include <vector>

namespace SharedCore
{
	using namespace std;

	/// <summary>
	/// This class is used to move the label tracking and customization out of the
	/// DebugManager and place it in a stand alone class. This then is the class that
	/// will be subclassed to customize the labels actually used in the debug manager.
	/// </summary>
	class scDebugLabelManager : public scIDebugLabelManager
	{
	public:
		/// <summary>
		/// Simple Destructor
		/// </summary>
		virtual ~scDebugLabelManager();

		/// <summary>
		/// Default Constructor
		/// </summary>
		/// <param name="nMaxLabels">The number of labels that will exist in this system.
		/// </param>
		scDebugLabelManager( uint16_t nMaxLabels = scDEBUGLABEL_LAST, scEnableState_t nDefault = scDisabled );

		/// <summary>
		/// Get the current state of the label.
		/// </summary>
		/// <param name="nLabel">The label id to lookup.</param>
		virtual scEnableState_t LabelState(uint16_t nLabel) const;

		/// <summary>
		/// Get the current state of the label.
		/// </summary>
		/// <param name="nLabel">The label ID to set</param>
		/// <param name="nState">The new state of the label.</param>
		virtual void  LabelState( uint16_t nLabel, scEnableState_t nState );

		/// <summary>
		/// Changes the state for all the labels to match the provided state.
		/// </summary>
		/// <param name="nState">The new state of the label.</param>
		virtual void  LabelState( scEnableState_t nState );

		/// <summary>
		/// Get a string description of the specified label.
		/// </summary>
		/// <param name="nLabel">The label to look up</param>
		virtual const char* LabelText(uint16_t nLabel);

	private:
		/// <summary>
		/// Vector of all the labels and their enable state.
		/// </summary>
		vector<scEnableState_t>		_nLabelState;

		/// <summary>
		/// Prevent copy operators
		/// </summary>
		scDebugLabelManager( const scDebugLabelManager& source ) {}
		/// <summary>
		/// Prevent copy operators
		/// </summary>
		scDebugLabelManager& operator=(const scDebugLabelManager& source ){ return *this; }

	};

}
#endif // !defined(__SCDEBUGLABELMANAGER_H__INCLUDED_)
