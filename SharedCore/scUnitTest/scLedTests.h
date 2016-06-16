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
#include "HAL/scLedIF.h"
#include "scDeviceManager.h"
#include "scLedEngine.h"

using namespace ::SharedCore;
using namespace SharedCore::HAL;

#define NAME_SIZE 10

class scLedTests : public ::testing::Test
{
public:
	void LedIF_Test();
	void LedEngine_Test();

	class MyLed : public scLedIF
	{
	public:
		MyLed();
		virtual ~MyLed();

		virtual void Initialize( scDeviceManager* pDm );
		bool Validate(uint32_t nId ) { return _bState[nId]; }
	protected:
		virtual void ChangeState( const LedDescriptor_t* pLed, LedState_t nState ) 
		{ 
			_bState[pLed->_nLedId] = (nState == scLedIF::scOn ? true : false );
		}

		bool _bState[10];
	};

	class testDM : public scDeviceManager
	{
	public:
		testDM() : scDeviceManager() {}
	};

	class TestEngine : public scLedEngine
	{
	public:
		TestEngine(void);
		virtual ~TestEngine();

		void Run( int count );
	};

protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	scLedTests();

	virtual ~scLedTests();

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	// virtual void SetUp() will be called before each test is run.  You
	// should define it if you need to initialize the variables.
	// Otherwise, this can be skipped.

	virtual void SetUp();

	virtual void TearDown();

};


