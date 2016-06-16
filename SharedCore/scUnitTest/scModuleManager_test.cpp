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

#include "scModuleManager_test.h"
#include "gmock/gmock.h"
#include "scErrorCodes.h"

using namespace ::SharedCore;
using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::_;

scModuleManager_test::scModuleManager_test(void)
{
}

scModuleManager_test::~scModuleManager_test()
{
}
	
scModuleManager_test::MyManager::MyManager(void)
{
}

void scModuleManager_test::MyManager::Initialize(void)
{
	TestModule* pModule1 = new TestModule();
	TestModule* pModule2 = new TestModule();
	TestModule* pModule3 = new TestModule();

	// check new item auto number starts from 1
	uint16_t nid1 = Add( pModule1 );
	EXPECT_EQ( 1, nid1 );

	/// check setting a specific ID
	nid1 = Add( pModule2, 5 );
	EXPECT_EQ( 5, nid1 );

	// Check the already exists case
	nid1 = Add( pModule2, 10 );
	EXPECT_EQ( 5, nid1 );

	// check auto advance with multiple items using already existing id
	nid1 = Add( pModule3, 1 );
	EXPECT_EQ( 6, nid1 );

	EXPECT_EQ( pModule1, Find(1) );
	EXPECT_EQ( pModule2, Find(5) );
	EXPECT_EQ( pModule3, Find(6) );

}


// for this test we are simply going to create a few modules
// verify setting of a specific ID and the auto ID.
// check the failure cases of asking for invalid values
// and calling the notify
void scModuleManager_test::BasicManagerTest()
{
	MyManager	manager;

	manager.Initialize();

	// hold onto the pointers for testing.
	TestModule* pModule1 = reinterpret_cast<TestModule*>( manager.Find(1) );
	TestModule* pModule2 = reinterpret_cast<TestModule*>( manager.Find(5) );
	TestModule* pModule3 = reinterpret_cast<TestModule*>( manager.Find(6) );

	EXPECT_CALL( *pModule1, Notify(1, NULL)).Times(Exactly(3));
	EXPECT_CALL( *pModule2, Notify(1, NULL)).Times(Exactly(2));
	EXPECT_CALL( *pModule3, Notify(1, NULL)).Times(Exactly(2));

	int nResult = manager.Notify( 1 );

	EXPECT_EQ( ERROR_SUCCESS, nResult );

	nResult = manager.Notify( 1, 1, NULL );

	EXPECT_EQ( ERROR_SUCCESS, nResult );

	nResult = manager.Notify( 2, 1, NULL );
	EXPECT_EQ( ERROR_SC_MODULE_NOT_FOUND, nResult );

	nResult = manager.Notify( 1 );

	EXPECT_EQ( ERROR_SUCCESS, nResult );


	// Since the module manager is designed to manage singleton objects the test
	// framework must perform it's own cleanup.
	delete pModule1;
	delete pModule2;
	delete pModule3;

}
