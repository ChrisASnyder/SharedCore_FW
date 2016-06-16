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

#include "scQueueList_test.h"


scQueueList_test::scQueueList_test(void)
{
	// You can do set-up work for each test here.
	_myList.Create( 5, sizeof(TestContainer_t) );

	TestContainer_t next;

	next._nId = 1;
	strcpy_s( next._nName, "Test 1" );
	_myList.SendToBack( &next, 0 );

	next._nId = 2;
	strcpy_s( next._nName, "Test 2" );
	_myList.SendToBack( &next, 0 );

	next._nId = 3;
	strcpy_s( next._nName, "Test 3" );
	_myList.SendToBack( &next, 0 );

	next._nId = 4;
	strcpy_s( next._nName, "Test 4" );
	_myList.SendToBack( &next, 0 );

	next._nId = 5;
	strcpy_s( next._nName, "Test 5" );
	_myList.SendToFront( &next, 0 );

}


scQueueList_test::~scQueueList_test(void)
{
	// You can do clean-up work that doesn't throw exceptions here.
}


void scQueueList_test::SimpleTest(void)
{
	TestContainer_t item;

	EXPECT_EQ( 0, _myList.SpacesAvailable() );
	EXPECT_EQ( 1, _myList.Peek( &item, 0 ) );
	EXPECT_EQ( 0, _myList.SpacesAvailable() );

	ASSERT_TRUE( item._nId == 5 );
	memset( &item, 0, sizeof( TestContainer_t ) );
	EXPECT_EQ( 1, _myList.Receive( &item, 0 ) );
	ASSERT_TRUE( item._nId == 5 );
	ASSERT_TRUE( strcmp( item._nName, "Test 5") == 0 );
	EXPECT_EQ( 1, _myList.SpacesAvailable() );

	memset( &item, 0, sizeof( TestContainer_t ) );
	EXPECT_EQ( 1, _myList.Receive( &item, 0 ) );
	ASSERT_TRUE( item._nId == 1 );
	ASSERT_TRUE( strcmp( item._nName, "Test 1") == 0 );
	EXPECT_EQ( 2, _myList.SpacesAvailable() );

	memset( &item, 0, sizeof( TestContainer_t ) );
	EXPECT_EQ( 1, _myList.Receive( &item, 0 ) );
	ASSERT_TRUE( item._nId == 2 );
	ASSERT_TRUE( strcmp( item._nName, "Test 2") == 0 );
	EXPECT_EQ( 3, _myList.SpacesAvailable() );

	memset( &item, 0, sizeof( TestContainer_t ) );
	EXPECT_EQ( 1, _myList.Receive( &item, 0 ) );
	ASSERT_TRUE( item._nId == 3 );
	ASSERT_TRUE( strcmp( item._nName, "Test 3") == 0 );
	EXPECT_EQ( 4, _myList.SpacesAvailable() );

	memset( &item, 0, sizeof( TestContainer_t ) );
	EXPECT_EQ( 1, _myList.Receive( &item, 0 ) );
	ASSERT_TRUE( item._nId == 4 );
	ASSERT_TRUE( strcmp( item._nName, "Test 4") == 0 );
	EXPECT_EQ( 5, _myList.SpacesAvailable() );

	memset( &item, 0, sizeof( TestContainer_t ) );
	EXPECT_EQ( 0, _myList.Receive( &item, 0 ) );
	EXPECT_EQ( 0, _myList.Peek( &item, 0 ) );
}
