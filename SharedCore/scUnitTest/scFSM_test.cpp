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


#include "gmock/gmock.h"
#include "scFSM_test.h"
#include "scFSMState.h"


using ::testing::AtLeast;					// #1

static const int g_pMatix[] = 
{
	// Inputs	Advance	reset
				1,		0,		// Step 1
				2,		0,		// Step 2
				1,		0,		// Step 3

};


scFSM_test::scFSM_test(void)
	: _pFSM(NULL)
	, _pList(NULL)
	, _pState1(NULL)
	, _pState2(NULL)
	, _pState3(NULL)
{
	uint16_t nInputs = 2;
	uint16_t nStates = 3;
	_pList = new scQueueList( 5, sizeof( int ) );
	_pFSM = new scFSM( nInputs, nStates, 0, g_pMatix, _pList );

	_pState1 = new MockState1();
	_pState2 = new MockState2();
	_pState3 = new MockState3();
}


scFSM_test::~scFSM_test(void)
{
	// You can do clean-up work that doesn't throw exceptions here.
	delete _pFSM;
	delete _pList;
	delete _pState1;
	delete _pState2;
	delete _pState3;
}

void scFSM_test::SetUp()
{
	_pFSM->Set( _pState1 );
	_pFSM->Set( _pState2 );
	_pFSM->Set( _pState3 );
}

void scFSM_test::TearDown()
{
	// doing it this way to show the states can be stored in the FSM and deleted
	// during the cleanup.
	_pFSM->Clear(2, false);
	_pFSM->Clear(1, true);
	_pFSM->Clear(0, true );
	_pState1 = NULL;
	_pState2 = NULL;
}

void scFSM_test::FiniteStateMachineTest()
{
	// Most uses of the FSM will have a task running that will be pending in the
	// handle input call. Since this is single threaded we will just push a state change then call handle input
	// to work with the action.

	EXPECT_CALL(*_pState1, OnEnter()).Times(AtLeast(1));
	EXPECT_CALL(*_pState1, OnLeave()).Times(AtLeast(1));
	EXPECT_CALL(*_pState1, OnIn()).Times(AtLeast(1));

	EXPECT_CALL(*_pState2, OnEnter()).Times(AtLeast(1));
	EXPECT_CALL(*_pState2, OnLeave()).Times(AtLeast(1));
	EXPECT_CALL(*_pState2, OnIn()).Times(AtLeast(1));

	EXPECT_CALL(*_pState3, OnEnter()).Times(AtLeast(1));
	EXPECT_CALL(*_pState3, OnLeave()).Times(AtLeast(1));
	EXPECT_CALL(*_pState3, OnIn()).Times(AtLeast(1));

	EXPECT_FALSE( _pFSM->HandleInput( 1000 ) );	// no inputs so timeout result

	_pFSM->OnInput( 0 );	// advance to state 2
	EXPECT_TRUE( _pFSM->HandleInput( 1000 ) );

	_pFSM->OnInput( 0 );	// advance to state 3
	EXPECT_TRUE( _pFSM->HandleInput( 1000 ) );

	_pFSM->OnInput( 0 );	// advance to state 2
	EXPECT_TRUE( _pFSM->HandleInput( 1000 ) );

	_pFSM->OnInput( 1 );	// advance to state 1
	EXPECT_TRUE( _pFSM->HandleInput( 1000 ) );

	_pFSM->OnInput( 0 );	// advance to state 2
	EXPECT_TRUE( _pFSM->HandleInput( 1000 ) );

}