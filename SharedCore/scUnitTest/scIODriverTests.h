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

#include "gtest/gtest.h"
#include "gmock/gmock.h"  // Brings in Google Mock.
#include "HAL/scBufferedIODriver.h"
#include "scDeviceManager.h"

using namespace ::SharedCore;
using namespace SharedCore::HAL;

#define NAME_SIZE 10

class scIODriverTests : public ::testing::Test
{
public:
	typedef enum
	{
		Driver_1,
		Driver_2,
		Driver_3,
		Driver_4
	} Driver_t;

	void EventIODriver_Test();
	void BufferIODriver_Test();
	void CallbackIODriver_Test();

	class testDM : public scDeviceManager
	{
	public:
		testDM() : scDeviceManager() {}
	};

	class TestDriver1 : public scBufferIODriver
	{
	public:
		TestDriver1();
		virtual ~TestDriver1() {}

		virtual void Initialize( scDeviceManager* pDm );

		virtual void TriggerSend(void);

		void Test_Receive(void);
	};

	class TestDriver2 : public scEventIODriver
	{
	public:
		TestDriver2();
		virtual ~TestDriver2() {}

		virtual void Initialize( scDeviceManager* pDm );

		void Test_Events(void);
	};

	class TestDriver3 : public scCallbackIODriver
	{
	public:
		TestDriver3();
		virtual ~TestDriver3() {}

		virtual void Initialize( scDeviceManager* pDm );
	};

	class TestDriver4 : public scBufferedEventCBIODriver
	{
	public:
		TestDriver4();
		virtual ~TestDriver4() {}

		virtual void Initialize( scDeviceManager* pDm );
		virtual void TriggerSend(void);

		void Test_Receive(void);
		void Test_Events(void);
	};

protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	scIODriverTests();

	virtual ~scIODriverTests();

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	// virtual void SetUp() will be called before each test is run.  You
	// should define it if you need to initialize the variables.
	// Otherwise, this can be skipped.

	virtual void SetUp();

	virtual void TearDown();

};


