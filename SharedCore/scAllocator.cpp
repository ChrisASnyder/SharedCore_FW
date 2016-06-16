//==============================================================================
//          UNCLASSIFIED
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
// File Name:		scAllocator.h
// Created By:		Christopher Snyder
// Creation Date:	15-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#include "scAllocator.h"

using SharedCore::scAllocator;
using SharedCore::scIAllocator;

#ifndef assert_param
#define assert_param(X)	if ( !(X) ) { for(;;) ; }
#endif

/// <summary>
/// Construct the class using a provided implementation.
/// </summary>
/// <param name="pImp"></param>
scAllocator::scAllocator( scIAllocator* pImp )
	: _pImp(NULL)
{
	_pImp = pImp;
}

/// <summary>
/// simple destructor. Does not delete the imp pointer
/// </summary>
scAllocator::~scAllocator()
{
}

/// <summary>
/// Obtain a pointer to a memory block with the indicated size. NULL is returned if
/// sufficient memory is not available.
/// </summary>
/// <param name="nSize">The number of bytes to be allocated.</param>
/// <param name="bIsStatic">Flag to indicate that this allocation is intended to be
/// statically allocated and not ever to be released.</param>
/// <param name="nType">This is a optional field that can be used to allow
/// switching of where memory will come from. Not used by default.</param>
uint8_t* scAllocator::Allocate(uint32_t nSize, bool bIsStatic, size_t nType )
{
	assert_param( _pImp != NULL );
	return _pImp->Allocate( nSize, bIsStatic, nType );
}

/// <summary>
/// This method will release a memory pointer that was allocated using this
/// allocator.
/// </summary>
/// <param name="pBuffer">pointer to the memory to be released. NULL is allowed.
/// </param>
void scAllocator::Destroy( void* pBuffer)
{
	assert_param( _pImp != NULL );
	_pImp->Destroy( pBuffer );
}

/// <summary>
/// Copy the pointer
/// </summary>
/// <param name="source"></param>
scAllocator& scAllocator::operator=( const scAllocator& source )
{
	if ( this != &source )
	{
		_pImp = source._pImp;
	}
	return *this;
}

/// <summary>
/// copy constructor. copies the pointer.
/// </summary>
/// <param name="source"></param>
scAllocator::scAllocator( const scAllocator& source )
	: _pImp( source._pImp )
{
}

/// <summary>
/// Equal operator, same if the pointers are equal.
/// </summary>
/// <param name="rhs"></param>
bool scAllocator::operator==( const scAllocator& rhs ) const
{
	return _pImp == rhs._pImp;
}

/// <summary>
/// True of the pointers are not the same.
/// </summary>
/// <param name="rhs"></param>
bool scAllocator::operator!=( const scAllocator& rhs ) const
{
	return _pImp != rhs._pImp;
}

