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

#include "scLedTests.h"
#include "scFSMState.h"
#include "scMutexNoOp.h"

using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::_;

scLedTests::MyLed::MyLed()
	: scLedIF( scDeviceDescriptor(10) )
{
	for( int i=0; i < sizeof( _bState ); i++ )
	{
		_bState[i] = false;
	}
}

scLedTests::MyLed::~MyLed()
{
}

void scLedTests::MyLed::Initialize( scDeviceManager* pDm )
{
	Add( 0, NULL, 1 );
	Add( 1, NULL, 2 );
	Add( 2, NULL, 3, true );
}

scLedTests::TestEngine::TestEngine(void)
	: scLedEngine( new scMutexNoOp() )
{
}

scLedTests::TestEngine::~TestEngine()
{
	delete _hLock;	// since this object allocated it I delete it
	_hLock = NULL;
}

void scLedTests::TestEngine::Run( int count )
{
	for( int i=0; i < count; ++i )
	{
		Poll();
	}
}


scLedTests::scLedTests(void)
{
	// the ring buffer delete the buffer.
}


scLedTests::~scLedTests(void)
{
}

void scLedTests::SetUp()
{
}

void scLedTests::TearDown()
{
}

void scLedTests::LedIF_Test()
{
	testDM		dm;
	MyLed*		pLed = new MyLed();

	dm.Add( pLed );

	dm.Initialize();

	// Not a valid index.
	pLed->Illuminate( 3, scLedIF::scOff );
	// Initial
	EXPECT_FALSE( pLed->Validate( 0 ) );
	EXPECT_FALSE( pLed->Validate( 1 ) );
	EXPECT_FALSE( pLed->Validate( 2 ) );

	pLed->Illuminate( 0, scLedIF::scOff );
	EXPECT_FALSE( pLed->Validate( 0 ) );
	pLed->Illuminate( 1, scLedIF::scOff );
	EXPECT_FALSE( pLed->Validate( 1 ) );
	pLed->Illuminate( 2, scLedIF::scOff );
	EXPECT_FALSE( pLed->Validate( 2 ) );

	pLed->Illuminate( 0, scLedIF::scOn );
	EXPECT_TRUE( pLed->Validate( 0 ) );
	pLed->Illuminate( 1, scLedIF::scOn );
	EXPECT_TRUE( pLed->Validate( 1 ) );
	pLed->Illuminate( 2, scLedIF::scOn );
	EXPECT_TRUE( pLed->Validate( 2 ) );
	// check it stays on.
	pLed->Illuminate( 2, scLedIF::scOn );
	EXPECT_TRUE( pLed->Validate( 2 ) );

	pLed->Illuminate( 0, scLedIF::scOff );
	EXPECT_FALSE( pLed->Validate( 0 ) );
	pLed->Illuminate( 1, scLedIF::scOff );
	EXPECT_FALSE( pLed->Validate( 1 ) );
	pLed->Illuminate( 2, scLedIF::scOff );
	EXPECT_FALSE( pLed->Validate( 2 ) );
	
}

void scLedTests::LedEngine_Test()
{
	testDM		dm;
	MyLed*		pLed = new MyLed();
	TestEngine	engine;

	dm.Add( pLed );
	dm.Initialize();

	engine.Initialize( pLed );
	// all are off at the start
	EXPECT_FALSE( pLed->Validate( 0 ) );
	EXPECT_FALSE( pLed->Validate( 1 ) );
	EXPECT_FALSE( pLed->Validate( 2 ) );

	engine.UpdatePattern( 1, 1 );
	engine.UpdatePattern( 2, 3 );

	// with the complexity of the different patterns. Only going to validate a simple case

	EXPECT_FALSE( pLed->Validate( 1 ) );
	engine.Run(2);
	EXPECT_FALSE( pLed->Validate( 1 ) );
	engine.Run(2);
	EXPECT_FALSE( pLed->Validate( 1 ) );

	engine.Update( 1, scLedEngine::scState_Flash );
	// changes state right away
	EXPECT_TRUE( pLed->Validate( 1 ) );
	engine.Run(1);
	EXPECT_TRUE( pLed->Validate( 1 ) );
	engine.Run(2);
	EXPECT_FALSE( pLed->Validate( 1 ) );
	engine.Run(2);
	EXPECT_TRUE( pLed->Validate( 1 ) );
	engine.Update( 1, scLedEngine::scState_On );
	engine.Run(10);
	EXPECT_TRUE( pLed->Validate( 1 ) );
	engine.Update( 1, scLedEngine::scState_Off );
	engine.Run(10);
	EXPECT_FALSE( pLed->Validate( 1 ) );

	engine.Update( 0, scLedEngine::scState_Flash );
	engine.Update( 1, scLedEngine::scState_Flash );
	engine.Update( 2, scLedEngine::scState_Flash );
	// do a lot of runs. make sure nothing breaks.
	engine.Run(10000);

	engine.Update( 0, scLedEngine::scState_On );
	engine.Update( 1, scLedEngine::scState_On );
	engine.Update( 2, scLedEngine::scState_On );
	EXPECT_TRUE( pLed->Validate( 0 ) );
	EXPECT_TRUE( pLed->Validate( 1 ) );
	EXPECT_TRUE( pLed->Validate( 2 ) );
	engine.Run(10000);
	EXPECT_TRUE( pLed->Validate( 0 ) );
	EXPECT_TRUE( pLed->Validate( 1 ) );
	EXPECT_TRUE( pLed->Validate( 2 ) );
}


