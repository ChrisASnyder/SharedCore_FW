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


#include <stdint.h>
#include <conio.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "scDateTime.h"
#include "scTriState.h"
#include <time.h>
#include "scStateMachine_Test.h"
#include "scIMutex_test.h"
#include "scIQueue_test.h"
#include "scQueueList_test.h"
#include "scFSM_test.h"
#include "scRingBuffer_test.h"
#include "scDeviceGuid_test.h"
#include "scDebugManager_test.h"
#include "scMessage_test.h"
#include "scLedTests.h"
#include "scIODriverTests.h"
#include "scModuleManager_test.h"

using namespace ::SharedCore;

namespace {
	using ::testing::AtLeast;					// #1
	using ::testing::_;

TEST(scMutexTest, baseClassTest) 
{
	MockscIMutex turtle;							// #2
	EXPECT_CALL(turtle, Acquire()).Times(AtLeast(1));
	EXPECT_CALL(turtle, Release()).Times(AtLeast(1));
	EXPECT_CALL(turtle, ReleaseISR()).Times(AtLeast(1));
	EXPECT_CALL(turtle, TryAcquire(1000)).Times(AtLeast(1));

	EXPECT_TRUE( MutexTest(turtle) );
}

TEST(scSempahoreTest, baseClassTest) 
{
	MockscISemaphore turtle;
	EXPECT_CALL(turtle, Wait(1000)).Times(AtLeast(1));
	EXPECT_CALL(turtle, Wait()).Times(AtLeast(1));
	EXPECT_CALL(turtle, Release()).Times(AtLeast(1));
	EXPECT_CALL(turtle, ReleaseISR()).Times(AtLeast(1));

	EXPECT_TRUE( SemaphoreTest(turtle) );
}

TEST(scIQueue, baseClassTest) 
{
	MockscIQueue testQueue;

	EXPECT_CALL(testQueue, Create( 10, _ )).Times(AtLeast(1));
	EXPECT_CALL(testQueue, SendToFront( _, _ )).Times(AtLeast(10));
	EXPECT_CALL(testQueue, SendToBack( _, _ )).Times(AtLeast(10));
	EXPECT_CALL(testQueue, Peek( _, _ )).Times(AtLeast(1));
	EXPECT_CALL(testQueue, Receive( _, _ )).Times(AtLeast(20));
	EXPECT_CALL(testQueue, SpacesAvailable()).Times(AtLeast(1));
		
	EXPECT_TRUE( MockscIQueue_Test(testQueue) );
}

TEST_F(scQueueList_test, baseClassTest) 
{
	SimpleTest();
}

TEST(DateTimeTest, Construt1 )
{
	scDateTime when( 2010, 1, 4, 1, 10, 20 );

	EXPECT_EQ( 2010, when.GetYear() );
	EXPECT_EQ( 1, when.GetMonth() );
	EXPECT_EQ( 4, when.GetDay() );
	EXPECT_EQ( 1, when.GetHour() );
	EXPECT_EQ( 10, when.GetMinute() );
	EXPECT_EQ( 20, when.GetSecond() );
}

TEST(DateTimeTest, Construt2 )
{
	__time64_t	now;
	time(&now);
	struct tm when;
	_localtime64_s( &when, &now );
	time_t numTime = mktime(&when);
	scDateTime ttest( mktime(&when) );

	EXPECT_EQ( 1900+ when.tm_year, ttest.GetYear() );
	EXPECT_EQ( when.tm_mon+1, ttest.GetMonth() );
	EXPECT_EQ( when.tm_mday, ttest.GetDay() );
	EXPECT_EQ( when.tm_hour, ttest.GetHour() );
	EXPECT_EQ( when.tm_min, ttest.GetMinute() );
	EXPECT_EQ( when.tm_sec, ttest.GetSecond() );
	EXPECT_EQ( when.tm_wday, ttest.GetDayOfWeek() );
	EXPECT_EQ( numTime, ttest() );

}

TEST(DateTimeTest, Comparing )
{
	scDateTime tA( 2014, 5, 10, 5, 20, 0 );
	scDateTime tB( 2013, 5, 10, 5, 20, 0 );

	EXPECT_EQ( false, tA == tB );
	EXPECT_EQ( true, tA == tA );
	EXPECT_EQ( true, tA != tB );

	EXPECT_EQ( true, tA > tB );
	EXPECT_EQ( true, tA >= tB );
	EXPECT_EQ( false, tA < tB );
	EXPECT_EQ( false, tA <= tB );
}

TEST(TimeSpanTest, Construction )
{
	scTimeSpan tA( 1, 5, 22, 40 );
	scTimeSpan tB( 1, 44, 1 );
	scTimeSpan tC( 5, 20 );

	EXPECT_EQ( 1, tA.GetDays() );
	EXPECT_EQ( 5, tA.GetHours() );
	EXPECT_EQ( 22, tA.GetMinutes() );
	EXPECT_EQ( 40, tA.GetSeconds() );

	EXPECT_EQ( 0, tB.GetDays() );
	EXPECT_EQ( 1, tB.GetHours() );
	EXPECT_EQ( 44, tB.GetMinutes() );
	EXPECT_EQ( 1, tB.GetSeconds() );

	EXPECT_EQ( 0, tC.GetDays() );
	EXPECT_EQ( 0, tC.GetHours() );
	EXPECT_EQ( 5, tC.GetMinutes() );
	EXPECT_EQ( 20, tC.GetSeconds() );
}

TEST(TimeSpanTest, Operators )
{
	scTimeSpan tA( 1, 5, 22, 40 );
	scTimeSpan tB( 1, 44, 1 );
	scTimeSpan tC( 5, 20 );

	EXPECT_EQ( false, tA == tB );
	EXPECT_EQ( true, tA != tB );
	EXPECT_EQ( true, tA == tA );

	EXPECT_EQ( true, tA > tB );
	EXPECT_EQ( true, tA >= tB );
	EXPECT_EQ( true, tA >= tA );
	EXPECT_EQ( false, tA < tB );
	EXPECT_EQ( false, tA <= tB );

	scTimeSpan result = tA + tC;

	EXPECT_EQ( 1, result.GetDays() );
	EXPECT_EQ( 5, result.GetHours() );
	EXPECT_EQ( 28, result.GetMinutes() );
	EXPECT_EQ( 0, result.GetSeconds() );

	result = tA - tB;
	EXPECT_EQ( 1, result.GetDays() );
	EXPECT_EQ( 3, result.GetHours() );
	EXPECT_EQ( 38, result.GetMinutes() );
	EXPECT_EQ( 39, result.GetSeconds() );

}

TEST( TriState, AllOperations )
{
	scTriState tA;
	scTriState tB(scTriState::scTrue);
	scTriState tC(false);

	EXPECT_TRUE( tA.Unknown() );
	EXPECT_FALSE( tA.True() );
	EXPECT_FALSE( tA.False() );

	EXPECT_FALSE( tB.Unknown() );
	EXPECT_TRUE( tB.True() );
	EXPECT_FALSE( tB.False() );

	EXPECT_FALSE( tC.Unknown() );
	EXPECT_FALSE( tC.True() );
	EXPECT_TRUE( tC.False() );

	tC = true;
	EXPECT_FALSE( tC.Unknown() );
	EXPECT_TRUE( tC.True() );
	EXPECT_FALSE( tC.False() );

	tC = scTriState::scUnknown;
	EXPECT_TRUE( tC.Unknown() );
	EXPECT_FALSE( tC.True() );
	EXPECT_FALSE( tC.False() );
	EXPECT_EQ( scTriState::scUnknown, tC() );

	tC = tB;
	EXPECT_FALSE( tC.Unknown() );
	EXPECT_TRUE( tC.True() );
	EXPECT_FALSE( tC.False() );

	EXPECT_TRUE( tC == tB );
	EXPECT_FALSE( tC != tB );
	EXPECT_FALSE( tC == tA );
	EXPECT_TRUE( tC != tA );
	EXPECT_TRUE( tC == true );
	EXPECT_FALSE( tC != true );
}

TEST_F(scStateMachine_Test, TransitionTest )
{
	TransitionTest();
}

TEST_F(scFSM_test, FiniteStateMachineTest)
{
	FiniteStateMachineTest();
}

TEST_F(scRingBuffer_Test, SingleByteFill )
{
	SingleByteFill();
}

TEST_F(scDeviceGuid_test, DeviceDescriptorTest )
{
	DeviceDescriptorTest();
}

TEST_F(scDeviceGuid_test, GuidTest )
{
	scGuidTest();
}

TEST_F(scDeviceGuid_test, DeviceGenericTest )
{
	scDeviceGenericTest();
}

TEST_F(scDeviceGuid_test, DeviceManagerTest )
{
	DeviceManagerTest();
}

TEST_F(scDeviceGuid_test, BlockMemoryIFTest )
{
	BlockMemoryIFTest();
}

TEST_F(scDebugManager_test, SimplePathTest )
{
	SimplePathTest();
}

TEST_F(scMessage_tests, StandardHeader )
{
	StandardHeader();
}

TEST_F(scMessage_tests, StandardMessage )
{
	StandardMessage();
}

TEST_F(scMessage_tests, FactoryTest )
{
	FactoryTest();
}

TEST_F(scMessage_tests, FactoryTestUsedAllSlots )
{
	FactoryTestUsedAllSlots();
}

TEST_F(scMessage_tests, FactoryTestUsedNoOverflow )
{
	FactoryTestUsedNoOverflow();
}


TEST_F(scLedTests, LedIF_Test )
{
	LedIF_Test();
}

TEST_F(scIODriverTests, EventIODriver_Test )
{
	EventIODriver_Test();
}

TEST_F(scIODriverTests, BufferIODriver_Test )
{
	BufferIODriver_Test();
}

TEST_F(scIODriverTests, CallbackIODriver_Test )
{
	CallbackIODriver_Test();
}

TEST_F(scLedTests, LedEngine_Test )
{
	LedEngine_Test();
}

TEST_F(scModuleManager_test, BasicManagerTest )
{
	BasicManagerTest();
}

//////////////////////////////////////////////////////
// Long tests
//////////////////////////////////////////////////////

TEST_F(scMessage_tests, FactoryStressTest )
{
	FactoryStressTest();
}

//////////////////////////////////////////////////////
// End of all tests
//////////////////////////////////////////////////////
} // namespace

void MySpecialTestCode(void);

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  int nResult = RUN_ALL_TESTS();

  printf("Hit SPACE to end");
  while( !_kbhit() )
  {
  }
  _getch();

  return nResult;
}

void MySpecialTestCode(void)
{
	int *i = new int[10];
	delete i;
}