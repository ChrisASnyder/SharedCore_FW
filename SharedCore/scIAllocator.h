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
// File Name:		scIAllocator.h
// Created By:		Christopher Snyder
// Creation Date:	15-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCIALLOCATOR_H__INCLUDED_)
#define __SCIALLOCATOR_H__INCLUDED_

#include <stddef.h>
#include "scTypes.h"

namespace SharedCore
{
	/// <summary>
	/// The scIAllocator provides an interface for allocating memory within the shared
	/// core framework. Subclasses of the allocator can perform custom memory
	/// management as needed. The default memory management is to use the new/delete
	/// functions directly.  Since embedded system may have memory in different
	/// locations and some objects may desire to have memory in specific locations and
	/// there may also be distinction made between memory allocated statically, or
	/// memory for short term use, this class will provide the hooks to create those
	/// distinctions. Usage of them is not strictly required.
	/// </summary>
	class scIAllocator
	{

	public:
		/// <summary>
		/// Constructor of the allocator. The paradigm of the allocator is not to contain
		/// and track the memory model, but to provide the interface to secondary
		/// implementations that perform specific implementations. This is essentially a
		/// facade pattern.
		/// </summary>
		scIAllocator(void) {}

		/// <summary>
		/// Destroy the allocator object.
		/// </summary>
		virtual ~scIAllocator() {}

		/// <summary>
		/// Obtain a pointer to a memory block with the indicated size. NULL is returned if
		/// sufficient memory is not available.
		/// </summary>
		/// <param name="nSize">The number of bytes to be allocated.</param>
		/// <param name="bIsStatic">Flag to indicate that this allocation is intended to be
		/// statically allocated and not ever to be released.</param>
		/// <param name="nType">This is a optional field that can be used to allow
		/// switching of where memory will come from. Not used by default.</param>
		virtual uint8_t* Allocate(uint32_t nSize, bool bIsStatic = false, size_t nType = 0) = 0;

		/// <summary>
		/// This method will release a memory pointer that was allocated using this
		/// allocator.
		/// </summary>
		/// <param name="pBuffer">pointer to the memory to be released. NULL is allowed.
		/// </param>
		virtual void Destroy( void* pBuffer) = 0;

	};

}
#endif // !defined(__SCIALLOCATOR_H__INCLUDED_)
