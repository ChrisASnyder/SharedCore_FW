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

#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"  // Brings in Google Mock.
#include "scQueueList.h"
#include "scFSM.h"

using namespace ::SharedCore;

#define NAME_SIZE 10

class scFSM_test : public ::testing::Test
{
public:
	void FiniteStateMachineTest();


	class MockState1 : public scFSMState
	{
	public:
		MockState1(void) : scFSMState( 0 ) {}
		virtual ~MockState1(void) {}

		MOCK_METHOD0(OnEnter, void( void ) );
		MOCK_METHOD0(OnLeave, void( void ) );
		MOCK_METHOD0(OnIn, void( void ) );
	};

	class MockState2 : public scFSMState
	{
	public:
		MockState2(void) : scFSMState( 1 ) {}
		virtual ~MockState2(void){}

		MOCK_METHOD0(OnEnter, void( void ) );
		MOCK_METHOD0(OnLeave, void( void ) );
		MOCK_METHOD0(OnIn, void( void ) );
	};

	class MockState3 : public scFSMState
	{
	public:
		MockState3(void) : scFSMState( 2 ) {}
		virtual ~MockState3(void){}

		MOCK_METHOD0(OnEnter, void( void ) );
		MOCK_METHOD0(OnLeave, void( void ) );
		MOCK_METHOD0(OnIn, void( void ) );
	};

protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	scFSM_test();

	virtual ~scFSM_test();

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	// virtual void SetUp() will be called before each test is run.  You
	// should define it if you need to initialize the variables.
	// Otherwise, this can be skipped.

	virtual void SetUp();

	virtual void TearDown();

	scFSM*					_pFSM;
	scQueueList*			_pList;

	MockState1*				_pState1;
	MockState2*				_pState2;
	MockState3*				_pState3;

};


