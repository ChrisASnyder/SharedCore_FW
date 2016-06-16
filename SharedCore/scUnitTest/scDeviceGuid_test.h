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

#include <stdarg.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"  // Brings in Google Mock.
#include "scDeviceDescriptor.h"
#include "scDeviceGeneric.h"
#include "scDeviceManager.h"
#include "HAL/scBlockMemoryIF.h"

using namespace ::SharedCore;
using namespace ::SharedCore::HAL;

#define NAME_SIZE 10

// Going to use this class to hold all the tests related to the device descriptor classes. 
// just to keep things a little cleaner.
class scDeviceGuid_test : public ::testing::Test
{
public:
	void DeviceDescriptorTest();
	void scGuidTest();
	void scDeviceGenericTest();

	void DeviceManagerTest();
	void BlockMemoryIFTest();

	typedef enum
	{
		Device_1 = 101,
		Device_2,
		Device_3,
		Device_4,
		Device_5 = 205,
		Device_6,
		Device_7,
		Device_8,
	} MyGuids_t;

	// This to make sure the right states are controlled since the Mock classes
	// don't call the parent functions.
	class TestDevice : public scDeviceGeneric
	{
	public:
		TestDevice( const scDeviceDescriptor& descriptor )
			: scDeviceGeneric( descriptor ) {}
		TestDevice( const scDeviceDescriptor& descriptor, const scDeviceDescriptor& dependant )
			: scDeviceGeneric( descriptor, dependant ) {}
		TestDevice( const scDeviceDescriptor& descriptor, int nCount, ... )
			: scDeviceGeneric( descriptor ) 
		{
			va_list vl;
			va_start( vl, nCount );
			AddDependants( vl, nCount );
			va_end(vl);
		}
		
		virtual void Initialize( scDeviceManager* pDm ) { scDeviceGeneric::Initialize( pDm ); }
		virtual void Enable() { scDeviceGeneric::Enable(); }
		virtual void Disable() { scDeviceGeneric::Disable(); }
		virtual void Configure(const scConfigureDevice& config ) { scDeviceGeneric::Configure(config); }
		virtual void Unconfigure(const scConfigureDevice& config) { scDeviceGeneric::Unconfigure(config); }
	};

	class Device1 : public TestDevice
	{
	public:
		Device1();
		MOCK_METHOD0( Reset, void() );
	};

	class Device2 : public TestDevice
	{
	public:
		Device2();
		MOCK_METHOD0( Reset, void() );
	};

	class Device3 : public TestDevice
	{
	public:
		Device3();
		MOCK_METHOD0( Reset, void() );
	};
	class Device4 : public TestDevice
	{
	public:
		Device4();
		MOCK_METHOD0( Reset, void() );
	};

	class Device5 : public TestDevice
	{
	public:
		Device5();
		MOCK_METHOD0( Reset, void() );
	};

	class DeviceHasSelf : public TestDevice
	{
	public:
		DeviceHasSelf();
		MOCK_METHOD0( Reset, void() );
	};

	class DeviceSpecialConstructor : public scDeviceGeneric
	{
	public:
		DeviceSpecialConstructor();
		MOCK_METHOD0( Reset, void() );
		MOCK_METHOD1( Initialize, void( scDeviceManager* pDm ) );
		MOCK_METHOD0( Enable, void() );
		MOCK_METHOD0( Disable, void() );
		MOCK_METHOD1( Configure, void (const scConfigureDevice& config ) );
		MOCK_METHOD1( Unconfigure, void (const scConfigureDevice& config) );
	};

	class testDM : public scDeviceManager
	{
	public:
		testDM() : scDeviceManager() {}
	};

	class testBlocks : public scBlockMemoryIF
	{
	public:
		testBlocks();

		MOCK_METHOD0( Reset, void() );
		void Initialize( scDeviceManager* pDm ) { };
		MOCK_METHOD0( Enable, void() );
		MOCK_METHOD0( Disable, void() );
		MOCK_METHOD1( Configure, void (const scConfigureDevice& config ) );
		MOCK_METHOD1( Unconfigure, void (const scConfigureDevice& config) );

		MOCK_METHOD4( Read, uint32_t (uint8_t* pData, uint32_t nDataLength, uint32_t nStartAddress, uint32_t nBlockCount) );
		MOCK_METHOD3( Write, uint32_t (const uint8_t* pData, uint32_t nStartAddress, uint32_t nBlockCount) );
		uint32_t DefaultBlockSize(void) const { return 0; }
		const uint64_t TotalSizeBytes(void) const { return 0; }
		uint32_t TotalSizeBlocks(void) const { return 0; }
		MOCK_METHOD3( BulkErase, uint32_t( uint32_t nStartSector, uint32_t nCount, bool bWait) );
		MOCK_METHOD1( Select, uint32_t(uint32_t nItem) );
		MOCK_METHOD0( Selected, uint32_t() );
	
	};

protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	scDeviceGuid_test();

	virtual ~scDeviceGuid_test();

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	// virtual void SetUp() will be called before each test is run.  You
	// should define it if you need to initialize the variables.
	// Otherwise, this can be skipped.

	//virtual void SetUp();

	//virtual void TearDown();

};


