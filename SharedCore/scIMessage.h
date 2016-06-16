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
// File Name:		scIMessage.h
// Created By:		Christopher Snyder
// Creation Date:	15-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCIMESSAGE_H__INCLUDED_)
#define __SCIMESSAGE_H__INCLUDED_

#include "scTypes.h"

namespace SharedCore
{
	/// <summary>
	/// This provides the lowest level fundamental for the message structure. This
	/// class will wrap a single pointer and provide basic message functional access to
	/// the typical parts of a message object. This class is a template and should have
	/// a subclass created for specific features.
	/// </summary>
	template< class MsgType, class HeaderType >
	class scIMessage
	{

	public:
		/// <summary>
		/// Simple destructor. Does not delete the memory pointer.
		/// </summary>
		virtual ~scIMessage() { }

		/// <summary>
		/// Examine the block of memory and the length value to determine if this message
		/// is valid. In most cases there will be a CRC embedded in the message block
		/// someplace that can be used to accept or reject the message.
		/// </summary>
		/// <param name="void"></param>
		virtual bool Validate(void) const = 0;

		/// <summary>
		/// Get the message ID from the message data.
		/// </summary>
		virtual MsgType GetID(void) const = 0;

		/// <summary>
		/// Obtain the number of bytes needed for the header portion of this message.
		/// </summary>
		virtual uint32_t LengthOfHeader(void) const = 0;

		/// <summary>
		/// Obtain the number of bytes needed for this entire message, which is both the
		/// header and the payload. Not this may be less than the length of the buffer.
		/// </summary>
		virtual uint32_t LengthOfMessage(void) const = 0;

		/// <summary>
		/// Obtain the number of bytes that follow the payload.
		/// </summary>
		virtual uint32_t LengthOfPayload(void) const = 0;

		/// <summary>
		/// Get a pointer to the underlying buffer pointer.
		/// </summary>
		/// <param name="void"></param>
		virtual const uint8_t* Buffer(void) const
		{
			return _pBuffer;
		}

		/// <summary>
		/// Get a pointer to the underlying buffer pointer.
		/// </summary>
		/// <param name="void"></param>
		virtual uint32_t LengthOfBuffer(void) const
		{
			return _nLength;
		}

		/// <summary>
		/// Get the information about the message header.
		/// </summary>
		/// <param name="void"></param>
		virtual const HeaderType& Header(void) const = 0;

	protected:
		/// <summary>
		/// Construct the message object providing a pointer to a memory location and the
		/// number of bytes for the message.
		/// </summary>
		/// <param name="pBuffer">Pointer to the buffer for the message.</param>
		/// <param name="nLength">Number of bytes in the buffer.</param>
		scIMessage( const uint8_t* pBuffer = NULL, uint32_t nLength = 0 )
			: _pBuffer( pBuffer )
			, _nLength( nLength )
		{
		}

		/// <summary>
		/// Pointer to the memory that contains the message.
		/// </summary>
		const uint8_t*					_pBuffer;

		/// <summary>
		/// The number of bytes associated with the message buffer.
		/// </summary>
		uint32_t						_nLength;


	};

}
#endif // !defined(__SCIMESSAGE_H__INCLUDED_)
