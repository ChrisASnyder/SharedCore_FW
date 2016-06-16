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

#include "scIODriverTests.h"
#include "scErrorCodes.h"
#include "scScopeLock.h"

using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::_;

using namespace SharedCore;

scIODriverTests::TestDriver1::TestDriver1()
	: scBufferIODriver( scDeviceDescriptor(Driver_1) )
{
}

void scIODriverTests::TestDriver1::TriggerSend(void)
{
	uint32_t nNumber = _pQueueOut->ReadStart();
	_pQueueOut->ReadEnd( nNumber );
}

void scIODriverTests::TestDriver1::Initialize( scDeviceManager* pDm )
{
	scRingBuffer* pIn = new scRingBuffer( 50, new uint8_t[50], NULL );
	scRingBuffer* pOut = new scRingBuffer( 50, new uint8_t[50], NULL );

	SetQueue( pIn, pOut );

	scBufferIODriver::Initialize( pDm );
}

void scIODriverTests::TestDriver1::Test_Receive(void)
{
	if ( _pQueueIn != NULL )
	{
		_pQueueIn->Lock();

		if ( _pQueueIn->WriteStart() > 0 )
		{
			uint8_t* pBuffer = _pQueueIn->WriteBlock();
			for( int i=0; i < 10; i++ )
			{
				*(pBuffer+i) = i;
			}
			_pQueueIn->WriteEnd(10);
		}
		_pQueueIn->Unlock();
	}
}

scIODriverTests::scIODriverTests(void)
{
	// the ring buffer delete the buffer.
}

scIODriverTests::TestDriver2::TestDriver2()
	: scEventIODriver( scDeviceDescriptor(Driver_2) )
{
}

void scIODriverTests::TestDriver2::Initialize( scDeviceManager* pDm )
{
	scEventIODriver::Initialize( pDm );
}

void scIODriverTests::TestDriver2::Test_Events(void)
{
	_event.FireEvent( scError );
	_event.FireEvent( scStartup );
	_event.FireEvent( scIdle );
	_event.FireEvent( scReceiveData );
	_event.FireEvent( scTransmitComplete );
	_event.FireEvent( scOverflow );
	_event.FireEvent( scUnderflow );
	_event.FireEvent( scRejected );
}


scIODriverTests::TestDriver3::TestDriver3()
	: scCallbackIODriver( scDeviceDescriptor(Driver_3) )
{
}

void scIODriverTests::TestDriver3::Initialize( scDeviceManager* pDm )
{
	scCallbackIODriver::Initialize( pDm );
}

scIODriverTests::~scIODriverTests(void)
{
}

void scIODriverTests::SetUp()
{
}

void scIODriverTests::TearDown()
{
}


void scIODriverTests::EventIODriver_Test()
{
	testDM	dm;

	uint8_t			TestBuffer[100];
	TestDriver1*	pDriver = new TestDriver1();

	memset( &TestBuffer[0], 0xAD, 100 );

	dm.Add( pDriver );
	dm.Initialize();
	pDriver->Enable();

	pDriver->SetOverflow( false );
	EXPECT_FALSE( pDriver->GetOverflow() );

	EXPECT_EQ( ERROR_SUCCESS, pDriver->Send_n( &TestBuffer[0], 20 ) );

	pDriver->Test_Receive();

	EXPECT_EQ( 10, pDriver->Recv_n( &TestBuffer[0], 10 ) );

}

static void Handler_Event( scEventIODriver *source, scDataEvents_t eventArg, void* context )
{
	switch(eventArg)
	{
	case SharedCore::HAL::scError:
		break;
	case SharedCore::HAL::scStartup:
		break;
	case SharedCore::HAL::scIdle:
		break;
	case SharedCore::HAL::scReceiveData:
		break;
	case SharedCore::HAL::scTransmitComplete:
		break;
	case SharedCore::HAL::scOverflow:
		break;
	case SharedCore::HAL::scUnderflow:
		break;
	case SharedCore::HAL::scRejected:
		break;
	default:
		break;
	}
}

static void Handler_Event2( scBufferedEventCBIODriver *source, scDataEvents_t eventArg, void* context )
{
	switch(eventArg)
	{
	case SharedCore::HAL::scError:
		break;
	case SharedCore::HAL::scStartup:
		break;
	case SharedCore::HAL::scIdle:
		break;
	case SharedCore::HAL::scReceiveData:
		break;
	case SharedCore::HAL::scTransmitComplete:
		break;
	case SharedCore::HAL::scOverflow:
		break;
	case SharedCore::HAL::scUnderflow:
		break;
	case SharedCore::HAL::scRejected:
		break;
	default:
		break;
	}
}

scIODriverTests::TestDriver4::TestDriver4()
	: scBufferedEventCBIODriver( scDeviceDescriptor(Driver_4) )
{
}

void scIODriverTests::TestDriver4::Test_Events(void)
{
	_event.FireEvent( scError );
	_event.FireEvent( scStartup );
	_event.FireEvent( scIdle );
	_event.FireEvent( scReceiveData );
	_event.FireEvent( scTransmitComplete );
	_event.FireEvent( scOverflow );
	_event.FireEvent( scUnderflow );
	_event.FireEvent( scRejected );
}

void scIODriverTests::TestDriver4::TriggerSend(void)
{
	uint32_t nNumber = _pQueueOut->ReadStart();
	_pQueueOut->ReadEnd( nNumber );
}

void scIODriverTests::TestDriver4::Initialize( scDeviceManager* pDm )
{
	scRingBuffer* pIn = new scRingBuffer( 50, new uint8_t[50], NULL );
	scRingBuffer* pOut = new scRingBuffer( 50, new uint8_t[50], NULL );

	SetQueue( pIn, pOut );

	scBufferIODriver::Initialize( pDm );
}

void scIODriverTests::TestDriver4::Test_Receive(void)
{
	if ( _pQueueIn != NULL )
	{
		_pQueueIn->Lock();

		if ( _pQueueIn->WriteStart() > 0 )
		{
			uint8_t* pBuffer = _pQueueIn->WriteBlock();
			for( int i=0; i < 10; i++ )
			{
				*(pBuffer+i) = i;
			}
			_pQueueIn->WriteEnd(10);
		}
		_pQueueIn->Unlock();
	}
}


static void Rx_ByteHandler( uint8_t nByte, void* pContext )
{
}

static void Tx_ByteHandler( void* pPort, void* pContext )
{
}

void scIODriverTests::BufferIODriver_Test()
{
	testDM	dm;

	uint8_t			TestBuffer[100];
	TestDriver2*	pDriver = new TestDriver2();

	memset( &TestBuffer[0], 0xAD, 100 );

	dm.Add( pDriver );
	dm.Initialize();
	pDriver->Enable();

	pDriver->Subscribe( Handler_Event, this );

	pDriver->Test_Events();

	EXPECT_EQ( IDLE_TIMEOUT_DEFAULT, pDriver->SetIdleNotifyTimeout(1000) );
	EXPECT_EQ( 1000, pDriver->GetIdleNotifyTimeout() );
	pDriver->Unsubscribe( Handler_Event );
}

void scIODriverTests::CallbackIODriver_Test()
{
	testDM	dm;
	uint8_t			TestBuffer[100];
	TestDriver3*	pDriver = new TestDriver3();
	TestDriver4*	pDriver2 = new TestDriver4();

	memset( &TestBuffer[0], 0xAD, 100 );

	dm.Add( pDriver );
	dm.Add( pDriver2 );
	dm.Initialize();
	pDriver->Enable();
	pDriver2->Enable();

	pDriver->SetTransmitCB( Tx_ByteHandler, this );
	pDriver->SetReceiveCB( Rx_ByteHandler, this );

	pDriver2->SetTransmitCB( Tx_ByteHandler, this );
	pDriver2->SetReceiveCB( Rx_ByteHandler, this );

	pDriver2->Subscribe( Handler_Event2, this );

	pDriver2->Test_Events();

	pDriver2->SetOverflow( false );
	EXPECT_FALSE( pDriver2->GetOverflow() );

	EXPECT_EQ( ERROR_SUCCESS, pDriver2->Send_n( &TestBuffer[0], 20 ) );

	pDriver2->Test_Receive();

	EXPECT_EQ( 10, pDriver2->Recv_n( &TestBuffer[0], 10 ) );

	EXPECT_EQ( IDLE_TIMEOUT_DEFAULT, pDriver2->SetIdleNotifyTimeout(1000) );
	EXPECT_EQ( 1000, pDriver2->GetIdleNotifyTimeout() );
	pDriver2->Unsubscribe( Handler_Event2 );

}