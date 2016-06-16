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

#pragma once

#include <stdarg.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"  // Brings in Google Mock.

#include "scStandardHeader.h"
#include "scStandardMessage.h"
#include "scMessageFactory.h"
#include "scMutexNoOp.h"
#include "scAllocator_Imp.h"

using namespace ::SharedCore;

// Going to use this class to hold all the tests related to the device descriptor classes. 
// just to keep things a little cleaner.
class scMessage_tests: public ::testing::Test
{
public:
	void StandardHeader();
	void StandardMessage();
	void FactoryTest();
	void FactoryTestUsedAllSlots();
	void FactoryTestUsedNoOverflow();
	void FactoryStressTest();

	typedef enum
	{
		msg_Start,
		msg_Stop,
		msg_Test
	} MyMessages_t;

	class MyMessage : public scStandardMessage<MyMessages_t>
	{
	public:
		MyMessage(const uint8_t* pBuffer = NULL, uint32_t nLength = 0)
			: scStandardMessage<MyMessages_t>( pBuffer, nLength )
		{
		}

		/// <summary>
		/// Implement the equal operator
		/// </summary>
		MyMessage& operator=( const MyMessage& source );
	};

	class MessageFactory : public scMessageFactory<MyMessage>
	{
	public:
		MessageFactory(uint8_t slots, uint32_t nBytes );
		virtual ~MessageFactory();
		MOCK_METHOD1(PostCreateP, MyMessage*( MyMessage* pMessage ) );
	protected:
		virtual MyMessage* PostCreate( MyMessage* pMessage ) 
		{
			PostCreateP( pMessage );
			return pMessage;
		}
	};

	class failMessageFactory : public scMessageFactory<MyMessage>
	{
	public:
		failMessageFactory(uint8_t slots, uint32_t nBytes )
			: scMessageFactory<MyMessage>( slots, nBytes )
		{
		}
		virtual ~failMessageFactory()
		{}


	protected:
		virtual MyMessage* PostCreate( MyMessage* pMessage ) 
		{
			return pMessage;
		}

		virtual MyMessage* MessageObject_Allocate(void)
		{
			return NULL;
		}

	};

	class emtpyAllocator : public scAllocator_Imp
	{
	public:
		emtpyAllocator(void) : scAllocator_Imp()
		{
		}

		virtual ~emtpyAllocator() {} 
		virtual uint8_t* Allocate(uint32_t nSize, bool bIsStatic = false, size_t nType = 0)
		{
			return NULL;
		}

		virtual void Destroy( void* pBuffer)
		{
		}

	};

protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	scMessage_tests();

	virtual ~scMessage_tests();

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	// virtual void SetUp() will be called before each test is run.  You
	// should define it if you need to initialize the variables.
	// Otherwise, this can be skipped.

	virtual void SetUp();

	//virtual void TearDown();

};


