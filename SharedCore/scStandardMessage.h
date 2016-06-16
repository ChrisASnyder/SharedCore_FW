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
// File Name:		scStandardMessage.h
// Created By:		Christopher Snyder
// Creation Date:	15-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCSTANDARDMESSAGE_H__INCLUDED_)
#define __SCSTANDARDMESSAGE_H__INCLUDED_

#include "scTypes.h"
#include "scIMessage.h"
#include "scStandardHeader.h"

namespace SharedCore
{
	/// <summary>
	/// This is the typical message implementation that for the SharedCore. Other
	/// implementations can be derived using the IMessage base class.
	/// </summary>
	template<class MsgType>
	class scStandardMessage : public scIMessage< MsgType, scStandardHeader<MsgType> >
	{
	public:
		virtual ~scStandardMessage() {};

		/// <summary>
		/// Examine the block of memory and the length value to determine if this message
		/// is valid. In most cases there will be a CRC embedded in the message block
		/// someplace that can be used to accept or reject the message.
		/// </summary>
		/// <param name="void"></param>
		virtual bool Validate(void) const
		{
			bool bResult = _Header.Valid();

			if ( bResult )
			{
				// Compute the 32-bit CRC found at the end of the message.
			}
			return bResult;
		}

		/// <summary>
		/// Get the message ID from the message data.
		/// </summary>
		virtual MsgType GetID(void) const
		{
			return _Header.Id();
		}

		/// <summary>
		/// Obtain the number of bytes needed for the header portion of this message.
		/// </summary>
		virtual uint32_t LengthOfHeader(void) const
		{
			return sizeof(scStandardHeader_t);
		}

		/// <summary>
		/// Obtain the number of bytes needed for this entire message, which is both the
		/// header and the payload. Not this may be less than the length of the buffer.
		/// </summary>
		virtual uint32_t LengthOfMessage(void) const
		{
			return sizeof(scStandardHeader_t) + _Header.Length();
		}

		/// <summary>
		/// Obtain the number of bytes that follow the payload.
		/// </summary>
		virtual uint32_t LengthOfPayload(void) const
		{
			return _Header.Length();
		}

		/// <summary>
		/// Get the information about the message header.
		/// </summary>
		virtual const scStandardHeader<MsgType>& Header(void) const
		{
			return _Header;
		}

		/// <summary>
		/// Access to the payload data of the message.
		/// </summary>
		virtual const uint8_t* Payload(void) const
		{
			return _pBuffer + LengthOfHeader();
		}

	protected:
		scStandardMessage( const uint8_t* pBuffer = NULL, uint32_t nLength = 0 )
			: scIMessage< MsgType, scStandardHeader<MsgType> >( pBuffer, nLength )
			, _Header( pBuffer, nLength )
		{
		}

		scStandardHeader<MsgType>	_Header;
	};

}
#endif // !defined(__SCSTANDARDMESSAGE_H__INCLUDED_)
