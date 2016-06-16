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
#include "scDeviceGuid_test.h"
#include "scDeviceDescriptor.h"
#include "scGuid.h"
#include "scErrorCodes.h"

using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::_;
using SharedCore::scEnableState_t;

scDeviceGuid_test::scDeviceGuid_test()
{
}

scDeviceGuid_test::~scDeviceGuid_test()
{
}

scDeviceGuid_test::Device1::Device1()
	: TestDevice( scDeviceDescriptor( Device_1 ) )
{

}

scDeviceGuid_test::Device2::Device2()
	: TestDevice( scDeviceDescriptor( Device_2 ), scDeviceDescriptor( Device_1 ) )
{

}

scDeviceGuid_test::Device3::Device3()
	: TestDevice( scDeviceDescriptor( Device_3 ), 2, &scDeviceDescriptor( Device_1 ), &scDeviceDescriptor( Device_2 ) )
{

}

scDeviceGuid_test::Device4::Device4()
	: TestDevice( scDeviceDescriptor( Device_4 ), 3, &scDeviceDescriptor( Device_1 ), &scDeviceDescriptor( Device_3 ), &scDeviceDescriptor( Device_5 ) )
{

}

scDeviceGuid_test::DeviceHasSelf::DeviceHasSelf()
	: TestDevice( scDeviceDescriptor( Device_6 ), 4, 
	&scDeviceDescriptor( Device_1 ), 
	&scDeviceDescriptor( Device_3 ), 
	&scDeviceDescriptor( Device_6 ),		// this is bad a recursive dependency and should just be ignored
	&scDeviceDescriptor( Device_5 )
	)
{

}

scDeviceGuid_test::Device5::Device5()
	: TestDevice( scDeviceDescriptor( Device_5 ) )
{

}

scDeviceGuid_test::DeviceSpecialConstructor::DeviceSpecialConstructor()
	: scDeviceGeneric( scDeviceDescriptor( Device_7 ), 3, &scDeviceDescriptor( Device_1 ), &scDeviceDescriptor( Device_3 ), &scDeviceDescriptor( Device_5 ) )
{
}

scDeviceGuid_test::testBlocks::testBlocks()
	: scBlockMemoryIF( scDeviceDescriptor( Device_8 )  )
{
}


#define TEST_GUID_1		(2)
#define TEST_GUID_2		(11)

void scDeviceGuid_test::DeviceDescriptorTest()
{
	scDeviceDescriptor	test1;
	scDeviceDescriptor	test2(TEST_GUID_1);
	scDeviceDescriptor	test3(TEST_GUID_2, 50 );

	EXPECT_EQ( INVALID_DRIVER_GUID, test1.Guid() );
	EXPECT_EQ( 0, test1.Param() );

	EXPECT_EQ( TEST_GUID_1, test2.Guid() );
	EXPECT_EQ( 0, test2.Param() );

	EXPECT_EQ( TEST_GUID_2, test3.Guid() );
	EXPECT_EQ( 50, test3.Param() );

	test1 = test3;

	EXPECT_EQ( TEST_GUID_2, test1.Guid() );
	EXPECT_EQ( 50, test1.Param() );

	scDeviceDescriptor	test4( test2 );

	EXPECT_EQ( test2.Guid(), test4.Guid() );
	EXPECT_EQ( test2.Param(), test4.Param() );

	test1.Param( 100 );
	EXPECT_EQ( 100, test1 .Param() );

	EXPECT_TRUE( test1() == test1.Guid() );
	EXPECT_TRUE( test1 == TEST_GUID_2 );
	EXPECT_FALSE( test1 != TEST_GUID_2 );
	EXPECT_TRUE( test1 != 70 );
	EXPECT_TRUE( test1 == test3 );
	EXPECT_FALSE( test1 != test3 );


}

void scDeviceGuid_test::scGuidTest()
{
	scGuid<MyGuids_t>	guid1( Device_1 );
	scGuid<MyGuids_t>	guid2( Device_2, 1 );
	scGuid<MyGuids_t>	guid3( Device_5, 2 );
	scGuid<MyGuids_t>	guid4( guid1 );

	EXPECT_EQ( Device_1, guid1.Guid() );
	EXPECT_EQ( 0, guid1.Param() );

	EXPECT_EQ( Device_2, guid2.Guid() );
	EXPECT_EQ( 1, guid2.Param() );

	EXPECT_EQ( Device_5, guid3.Guid() );
	EXPECT_EQ( 2, guid3.Param() );

	EXPECT_EQ( guid1(), guid4.Guid() );
	EXPECT_EQ( guid1.Param(), guid4.Param() );
	
	guid4 = guid2;

	EXPECT_EQ( guid2(), guid4.Guid() );
	EXPECT_EQ( guid2.Param(), guid4.Param() );

	EXPECT_TRUE( guid3 == Device_5 );
	EXPECT_FALSE( guid3 != Device_5 );
	EXPECT_TRUE( guid3 != Device_3 );
	EXPECT_FALSE( guid3 == Device_3 );
	EXPECT_TRUE( guid4 == guid2 );
	EXPECT_FALSE( guid4 != guid2 );
	EXPECT_FALSE( guid4 == guid3 );
	EXPECT_TRUE( guid4 != guid3);

}

void scDeviceGuid_test::scDeviceGenericTest()
{
	//scGuid<MyGuids_t>	guid1(Device_1);
	scDeviceDescriptor guid1(Device_1);

	Device1 device1;
	Device2 device2;
	Device3 device3;
	Device4 device4;
	Device5 device5;

	EXPECT_FALSE( device1.HaveDependants() );
	EXPECT_EQ( 0, device1.Dependants().size() );

	EXPECT_FALSE( device5.HaveDependants() );
	EXPECT_EQ( 0, device5.Dependants().size() );

	EXPECT_TRUE( device2.HaveDependants() );
	EXPECT_EQ( 1, device2.Dependants().size() );

	EXPECT_TRUE( device3.HaveDependants() );
	EXPECT_EQ( 2, device3.Dependants().size() );

	EXPECT_TRUE( device4.HaveDependants() );
	EXPECT_EQ( 3, device4.Dependants().size() );

	EXPECT_EQ( guid1, device1.Guid() );
	EXPECT_TRUE( device1 == scDeviceDescriptor(Device_1) );
	EXPECT_FALSE( device1 != scDeviceDescriptor(Device_1) );
	EXPECT_TRUE( device1 != scDeviceDescriptor(Device_2) );
	EXPECT_FALSE( device1 == scDeviceDescriptor(Device_2) );

	EXPECT_TRUE( guid1 == device1.Guid() );
	EXPECT_FALSE( device1.Guid() != guid1 );

}

void scDeviceGuid_test::DeviceManagerTest()
{
	// The more interesting setup case is to verify the dependant walk
	// so add device 4 second.
	Device1* pDevice1 = new Device1();
	Device2* pDevice2 = new Device2();
	Device3* pDevice3 = new Device3();
	Device4* pDevice4 = new Device4();
	Device5* pDevice5 = new Device5();
	scConfigureDevice	config1(50);
	scConfigureDevice	config2;

	testDM	dm;
	scGuid<MyGuids_t>	guid4(Device_4);

	EXPECT_EQ( 1, dm.Add( pDevice1 ) );
	EXPECT_EQ( 2, dm.Add( pDevice4 ) );
	EXPECT_EQ( 3, dm.Add( pDevice2 ) );
	EXPECT_EQ( 4, dm.Add( pDevice3 ) );
	EXPECT_EQ( 5, dm.Add( pDevice5 ) );
	EXPECT_EQ( 6, dm.Add( new DeviceHasSelf() ) );

	dm.Initialize();
	EXPECT_EQ( scDeviceGeneric::Initialized, pDevice1->SetupState() );
	EXPECT_EQ( scDeviceGeneric::Initialized, pDevice2->SetupState() );
	EXPECT_EQ( scDeviceGeneric::Initialized, pDevice3->SetupState() );
	EXPECT_EQ( scDeviceGeneric::Initialized, pDevice4->SetupState() );
	EXPECT_EQ( scDeviceGeneric::Initialized, pDevice5->SetupState() );

	EXPECT_EQ( 50, config1.Parameter() );
	config2 = config1;
	config1.Parameter( 100 );
	EXPECT_EQ( 100, config1.Parameter() );
	EXPECT_EQ( 50, config2.Parameter() );

	dm.Configure(guid4, config1 );
	EXPECT_EQ( scDeviceGeneric::Configured, pDevice4->SetupState() );

	dm.Enable(guid4, true);
	EXPECT_EQ( scEnabled, pDevice4->State() );

	dm.Disable(guid4, true);
	EXPECT_EQ( scDisabled, pDevice4->State() );

	EXPECT_CALL( *pDevice1, Reset()).Times(Exactly(3));
	EXPECT_CALL( *pDevice2, Reset()).Times(Exactly(1));
	EXPECT_CALL( *pDevice3, Reset()).Times(Exactly(1));
	EXPECT_CALL( *pDevice5, Reset()).Times(Exactly(1));
	EXPECT_CALL( *pDevice4, Reset()).Times(Exactly(1));
	dm.Reset(guid4, true);

	dm.Unconfigure(guid4, config1 );
	EXPECT_EQ( scDeviceGeneric::Unconfigured, pDevice4->SetupState() );

	dm.Configure(guid4, config1 );
	EXPECT_EQ( scDeviceGeneric::Configured, pDevice4->SetupState() );

	EXPECT_TRUE( pDevice1->IsConnected() );
	EXPECT_EQ( ERROR_SUCCESS, pDevice1->GetLastError() );

	EXPECT_EQ( 7, dm.Add( new DeviceSpecialConstructor() ) );
}

void scDeviceGuid_test::BlockMemoryIFTest()
{
	testDM			dm;
	uint8_t			input[10];
	testBlocks*		pTestBlocks = new testBlocks();

	dm.Add( pTestBlocks );
	dm.Initialize();

	EXPECT_CALL( *pTestBlocks, Read(_,_,_,_)).Times(AtLeast(1));
	EXPECT_CALL( *pTestBlocks, Write(_,_,_)).Times(AtLeast(1));
	EXPECT_CALL( *pTestBlocks, BulkErase(_,_,_)).Times(AtLeast(1));
	EXPECT_CALL( *pTestBlocks, Select(_)).Times(AtLeast(1));
	EXPECT_CALL( *pTestBlocks, Selected()).Times(AtLeast(1));

	EXPECT_EQ( 0, pTestBlocks->Read( &input[0], 10, 0, 1 ) );
	EXPECT_EQ( 0, pTestBlocks->Write( &input[0], 0, 1 ) );
	EXPECT_EQ( 0, pTestBlocks->DefaultBlockSize() );
	EXPECT_EQ( 0, pTestBlocks->TotalSizeBytes() );
	EXPECT_EQ( 0, pTestBlocks->TotalSizeBlocks() );
	EXPECT_EQ( 0, pTestBlocks->BulkErase( 0,10, false ) );
	EXPECT_EQ( 0, pTestBlocks->Select(0) );
	EXPECT_EQ( 0, pTestBlocks->Selected() );

}

