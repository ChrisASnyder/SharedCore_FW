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
//          UNCLASSIFIED
//==============================================================================
//
// File Name:		scStandardHeader.h
// Created By:		Christopher Snyder
// Creation Date:	15-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCSTANDARDHEADER_H__INCLUDED_)
#define __SCSTANDARDHEADER_H__INCLUDED_

#include "scStandardHeader_t.h"

namespace SharedCore
{
	/// <summary>
	/// This class provides a utility interface to the standard header structure. This
	/// method can both parse from and write into a memory location the data for a
	/// message header.
	/// </summary>
	template<class MsgType>
	class scStandardHeader
	{
	public:
		/// <summary>
		/// The basic constructor. Makes the header data empty.
		/// </summary>
		scStandardHeader( void )
			: _local()
		{
			memset(&_local, 0, sizeof(scStandardHeader_t) );
			_local._prefix = STANDARD_HEADER_PREFIX;
		}

		/// <summary>
		/// Construct the class from the structure pointer
		/// </summary>
		/// <param name="pHeader"></param>
		scStandardHeader( const scStandardHeader_t* pHeader )
			: _local()
		{
			if ( pHeader != NULL )
			{
				memcpy( &_local, pHeader, sizeof(scStandardHeader_t) );
			}
		}

		/// <summary>
		/// Construct from a reference to the structure.
		/// </summary>
		/// <param name="header"></param>
		scStandardHeader( const scStandardHeader_t& header )
			: _local()
		{
			memcpy( &_local, &header, sizeof(scStandardHeader_t) );
		}

		/// <summary>
		/// Construct from a memory buffer
		/// </summary>
		/// <param name="pBuffer"></param>
		/// <param name="nLength"></param>
		scStandardHeader( const uint8_t* pBuffer, uint32_t nLength )
			: _local()
		{
			if ( pBuffer != NULL )
			{
				if ( nLength >= sizeof(scStandardHeader_t) )
				{
					memcpy( &_local, pBuffer, sizeof(scStandardHeader_t) );
				}
				else
				{
					memcpy( &_local, pBuffer, nLength );
				}
			}
		}

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="source"></param>
		scStandardHeader( const scStandardHeader& source )
			: _local()
		{
			memcpy( &_local, &source._local, sizeof(scStandardHeader_t) );
		}

		/// <summary>
		/// equal operator.
		/// </summary>
		/// <param name="source"></param>
		scStandardHeader& operator=( const scStandardHeader& source )
		{
			if ( this != &source )
			{
				memcpy( &_local, &source._local, sizeof(scStandardHeader_t) );
			}
			return *this;
		}

		/// <summary>
		/// Check for equality
		/// </summary>
		/// <param name="rhs"></param>
		bool operator==( const scStandardHeader& rhs ) const
		{

		}

		/// <summary>
		/// Check for inequality
		/// </summary>
		/// <param name="rhs"></param>
		bool operator!=( const scStandardHeader& rhs ) const
		{
			return !( (*this) == rhs );
		}

		/// <summary>
		/// Simple destructor.
		/// </summary>
		virtual ~scStandardHeader() 
		{}

		/// <summary>
		/// Get the prefix from the header
		/// </summary>
		uint8_t		Prefix(void) const
		{
			return _local._prefix;
		}

		/// <summary>
		/// The flags
		/// </summary>
		uint8_t	Flags(void) const
		{
			return _local._nFlags;
		}

		/// <summary>
		/// The sequence number
		/// </summary>
		uint8_t	Sequence(void) const
		{
			return _local._nSequence;
		}

		/// <summary>
		/// The destination address
		/// </summary>
		uint16_t	Destination(void) const
		{
			return _local._nDestination;
		}

		/// <summary>
		/// The Source address
		/// </summary>
		uint16_t	Source(void) const
		{
			return _local._nSource;
		}

		/// <summary>
		/// The Message ID
		/// </summary>
		MsgType		Id(void) const
		{
			return (MsgType)_local._nMessageID;
		}

		/// <summary>
		/// The length of the payload. Does not include this header.
		/// </summary>
		uint32_t	Length(void) const
		{
			return _local._nLength;
		}

		/// <summary>
		/// The checksum.
		/// </summary>
		uint8_t		Checksum(void) const
		{
			return _local._nCheckSum;
		}

		/// <summary>
		/// Is the current structure valid.
		/// </summary>
		bool Valid(void) const
		{
			uint8_t nChecksum = ComputeChecksum();
			return Prefix() == STANDARD_HEADER_PREFIX && nChecksum == Checksum();
		}

		/// <summary>
		/// Set the prefix
		/// </summary>
		/// <param name="nValue"></param>
		void		SetPrefix( uint8_t nValue )
		{
			_local._prefix = nValue;
		}

		/// <summary>
		/// set the flags.
		/// </summary>
		/// <param name="nValue"></param>
		void		SetFlags( uint8_t nValue )
		{
			_local._nFlags = nValue;
		}

		/// <summary>
		/// The sequence number
		/// </summary>
		void		SetSequence( uint8_t nValue )
		{
			_local._nSequence = nValue;
		}

		/// <summary>
		/// set the destination.
		/// </summary>
		/// <param name="nValue"></param>
		void		SetDestination( uint16_t nValue )
		{
			_local._nDestination = nValue;
		}

		/// <summary>
		/// Set the message source.
		/// </summary>
		/// <param name="nValue">Address</param>
		void SetSource(uint16_t nValue)
		{
			_local._nSource = nValue;
		}

		/// <summary>
		/// Set the message ID
		/// </summary>
		/// <param name="nValue"></param>
		void		SetId( MsgType nValue )
		{
			_local._nMessageID = (uint16_t)nValue;
		}

		/// <summary>
		/// Set the payload length
		/// </summary>
		/// <param name="nValue"></param>
		void		SetLength( uint32_t nValue )
		{
			_local._nLength = nValue;
		}

		/// <summary>
		/// Set the checksum.
		/// </summary>
		/// <param name="nValue"></param>
		void		SetChecksum( uint8_t nValue )
		{
			_local._nCheckSum = nValue;
		}

		/// <summary>
		/// This method will compute a new checksum for the current values in the local
		/// structure and will overwrite any value currently there.
		/// </summary>
		void UpdateChecksum(void)
		{
			SetChecksum( ComputeChecksum() );
		}

		/// <summary>
		/// Compute the 8bit checksum for the structure.
		/// </summary>
		uint8_t ComputeChecksum(void) const
		{
			uint8_t nResult = 0;
			const uint8_t* pCounter = reinterpret_cast<const uint8_t*>(&_local);
			for( size_t i=0; i < sizeof(scStandardHeader_t) -1; ++i )
			{
				nResult+= *(pCounter+i);
			}
			return nResult;
		}


	private:
		/// <summary>
		/// This is the structure for the local copy of the header object.
		/// </summary>
		scStandardHeader_t _local;
	};

}
#endif // !defined(__SCSTANDARDHEADER_H__INCLUDED_)

