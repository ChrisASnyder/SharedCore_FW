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
// File Name:		scAllocator_Imp.h
// Created By:		Christopher Snyder
// Creation Date:	15-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCALLOCATOR_IMP_H__INCLUDED_)
#define __SCALLOCATOR_IMP_H__INCLUDED_

#include "scIAllocator.h"

namespace SharedCore
{
	/// <summary>
	/// This is a simple allocator implementation that uses new and delete.
	/// </summary>
	class scAllocator_Imp : public SharedCore::scIAllocator
	{

	public:
		/// <summary>
		/// Construct the implementation object.
		/// </summary>
		scAllocator_Imp();
		virtual ~scAllocator_Imp();

		/// <summary>
		/// Obtain a pointer to a memory block with the indicated size. NULL is returned if
		/// sufficient memory is not available. Uses the new operator
		/// </summary>
		/// <param name="nSize">The number of bytes to be allocated.</param>
		/// <param name="bIsStatic">Flag to indicate that this allocation is intended to be
		/// statically allocated and not ever to be released.</param>
		/// <param name="nType">This is a optional field that can be used to allow
		/// switching of where memory will come from. Not used by default.</param>
		virtual uint8_t* Allocate(uint32_t nSize, bool bIsStatic = false, size_t nType = 0);

		/// <summary>
		/// This method will release a memory pointer that was allocated using this
		/// allocator. Uses the Delete operator.
		/// </summary>
		/// <param name="pBuffer">pointer to the memory to be released. NULL is allowed.
		/// </param>
		virtual void Destroy( void* pBuffer);
	};

}
#endif // !defined(__SCALLOCATOR_IMP_H__INCLUDED_)
