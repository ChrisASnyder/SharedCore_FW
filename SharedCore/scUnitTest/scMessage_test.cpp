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
#include "scMessage_test.h"
#include "scRingBuffer.h"
#include "scAllocator_Imp.h"
#include "scErrorCodes.h"

using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::_;
using namespace SharedCore;


scMessage_tests::MessageFactory::MessageFactory(uint8_t slots, uint32_t nBytes )
	: scMessageFactory( slots, nBytes)
{
}

scMessage_tests::MessageFactory::~MessageFactory()
{
}


scMessage_tests::MyMessage& scMessage_tests::MyMessage::operator=( const scMessage_tests::MyMessage& source )
{
	if ( this != &source )
	{
		this->_pBuffer = source._pBuffer;
		this->_nLength = source._nLength;
		this->_Header = source.Header();
	}
	return *this;
}

scMessage_tests::scMessage_tests()
{
}

scMessage_tests::~scMessage_tests()
{
}

void scMessage_tests::SetUp()
{
}

void scMessage_tests::StandardHeader()
{
	scStandardHeader<MyMessages_t> header1;
	
	// test all the sets/gets
	header1.SetFlags( 0x80);
	header1.SetSequence( 0x01 );
	header1.SetDestination( 10 );
	header1.SetSource( 1 );
	header1.SetLength( 20 );
	header1.SetId( msg_Start );
	header1.UpdateChecksum();

	EXPECT_EQ( STANDARD_HEADER_PREFIX,	header1.Prefix() );
	EXPECT_EQ( msg_Start,				header1.Id() );
	EXPECT_EQ( 10,						header1.Destination() );
	EXPECT_EQ( 1,						header1.Source() );
	EXPECT_EQ( 20,						header1.Length() );
	EXPECT_EQ( 0x80,					header1.Flags() );
	EXPECT_EQ( 1,						header1.Sequence() );
	EXPECT_EQ( 0x4A,					header1.Checksum() );

	scStandardHeader_t		_local;
	_local._prefix			= STANDARD_HEADER_PREFIX;
	_local._nFlags			= 0xFF;
	_local._nDestination	= 5;
	_local._nSource			= 2;
	_local._nMessageID		= msg_Stop;
	_local._nLength			= 2;
	_local._nCheckSum		= 44;
	_local._nSequence		= 100;

	scStandardHeader<MyMessages_t> header2(_local);

	EXPECT_EQ( STANDARD_HEADER_PREFIX,	header2.Prefix() );
	EXPECT_EQ( msg_Stop,				header2.Id() );
	EXPECT_EQ( 5,						header2.Destination() );
	EXPECT_EQ( 2,						header2.Source() );
	EXPECT_EQ( 2,						header2.Length() );
	EXPECT_EQ( 0xFF,					header2.Flags() );
	EXPECT_EQ( 44,						header2.Checksum() );
	EXPECT_EQ( 100,						header2.Sequence() );

	scStandardHeader<MyMessages_t> header3( &_local );
	EXPECT_EQ( STANDARD_HEADER_PREFIX,	header3.Prefix() );
	EXPECT_EQ( msg_Stop,				header3.Id() );
	EXPECT_EQ( 5,						header3.Destination() );
	EXPECT_EQ( 2,						header3.Source() );
	EXPECT_EQ( 2,						header3.Length() );
	EXPECT_EQ( 0xFF,					header3.Flags() );
	EXPECT_EQ( 44,						header3.Checksum() );
	EXPECT_EQ( 100,						header3.Sequence() );

	scStandardHeader<MyMessages_t> header4( reinterpret_cast<const uint8_t*>(&_local), sizeof(scStandardHeader_t) );
	EXPECT_EQ( STANDARD_HEADER_PREFIX,	header4.Prefix() );
	EXPECT_EQ( msg_Stop,				header4.Id() );
	EXPECT_EQ( 5,						header4.Destination() );
	EXPECT_EQ( 2,						header4.Source() );
	EXPECT_EQ( 2,						header4.Length() );
	EXPECT_EQ( 0xFF,					header4.Flags() );
	EXPECT_EQ( 44,						header4.Checksum() );
	EXPECT_EQ( 100,						header4.Sequence() );

	EXPECT_FALSE( header2.Valid() );

	header2.UpdateChecksum();
	EXPECT_TRUE( header2.Valid() );

	header2.SetPrefix( 1 );
	EXPECT_FALSE( header2.Valid() );
	header2.UpdateChecksum();
	EXPECT_FALSE( header2.Valid() );
}

static uint8_t		manual[] = {
	0xAA,						// prefix
	0x80,						// flags
	0x01,						// Sequence number
	0x01, 0x00,					// destination
	0x02, 0x00,					// source
	0x01, 0x00,					// message id
	0x06, 0x00, 0x00, 0x00,		// length
	0x35,						// checksum
	0xCC, 0xCC,					// payload
	0x00, 0x00, 0x00, 0x00,		// CRC32	TODO
};

void scMessage_tests::StandardMessage()
{
	MyMessage	blank;


	EXPECT_FALSE( blank.Validate() );


	MyMessage	wrapper( &manual[0], sizeof( manual ) );

	EXPECT_EQ( STANDARD_HEADER_PREFIX,	wrapper.Header().Prefix() );
	EXPECT_EQ( 0x01,					wrapper.Header().Sequence() );
	EXPECT_EQ( 0x80,					wrapper.Header().Flags() );
	EXPECT_EQ( msg_Stop,				wrapper.Header().Id() );
	EXPECT_EQ( 1,						wrapper.Header().Destination() );
	EXPECT_EQ( 2,						wrapper.Header().Source() );
	EXPECT_EQ( 6,						wrapper.Header().Length() );
	EXPECT_EQ( 0x35,					wrapper.Header().Checksum() );

	EXPECT_TRUE( wrapper.Validate() );
}

// TODO Test factory using only external allocator.
// TODO test factory that uses all slots.
void scMessage_tests::FactoryTest()
{
	uint8_t*			pRingMemory = new uint8_t[100];
	MyMessage*			pMsg1;
	MyMessage*			pMsg2;
	MyMessage*			pMsg3;
	MyMessage*			pMsg4;
	scMutexNoOp			lock;
	scMutexNoOp			lock2;
	scAllocator_Imp*	pNewOp = new scAllocator_Imp();
	scAllocator			memManager( pNewOp );
	scRingBuffer*		pRingBuffer = new scRingBuffer( 100, pRingMemory, &lock2 );

	MessageFactory*		pFactory = new MessageFactory(10, 100);

	EXPECT_CALL( *pFactory, PostCreateP(_)).Times(AtLeast(2));

	EXPECT_TRUE( pRingBuffer->WriteStart() > sizeof( manual ) );
	memcpy( pRingBuffer->WriteBlock() , &manual[0], sizeof( manual ) );
	pRingBuffer->WriteEnd( sizeof( manual ) );

	uint32_t nError = pFactory->Initialize( memManager, memManager, &lock );
	EXPECT_EQ( ERROR_SUCCESS, nError );
	EXPECT_EQ( ERROR_SUCCESS, pFactory->GetLastError() );

	pMsg1 = pFactory->Create( NULL, 0 );

	pMsg2 = pFactory->Create( 20 );

	pMsg3 = pFactory->Create( pRingBuffer );

	pMsg4 = pFactory->Copy( pMsg3 );

	EXPECT_EQ( 4, pFactory->MessagesInUse() );
	EXPECT_EQ( 6, pFactory->MessagesAvailable() );

	pFactory->Acquire( pMsg4 );
	pFactory->Acquire( NULL );

	pFactory->DebugDump();

	EXPECT_TRUE( pFactory->Release( pMsg2 ) );
	EXPECT_TRUE( pFactory->Release( pMsg1 ) );

	EXPECT_EQ( sizeof( manual ), pRingBuffer->InUse() );
	EXPECT_TRUE( pFactory->Release( pMsg3 ) );
	EXPECT_EQ( 0, pRingBuffer->InUse() );

	EXPECT_FALSE( pFactory->Release( NULL ) );
	EXPECT_FALSE( pFactory->Release( pMsg4 ) );
	EXPECT_TRUE( pFactory->Release( pMsg4 ) );

	pFactory->DebugDump();

	// Todo create the memory full cases

	pMsg1 = pFactory->Create( 90 );
	EXPECT_TRUE( pMsg1 != NULL );
	EXPECT_EQ( 90, pMsg1->LengthOfBuffer() );

	// this will rollover to the external allocator
	pMsg2 = pFactory->Create( &manual[0], sizeof( manual ) );

	//EXPECT_TRUE( pMsg2 == NULL );
	EXPECT_TRUE( pFactory->Release( pMsg2 ) );
	EXPECT_TRUE( pFactory->Release( pMsg1 ) );


	delete pFactory;
	delete pNewOp;
	delete pRingBuffer;
	delete pRingMemory;
}

void scMessage_tests::FactoryTestUsedAllSlots()
{
	MyMessage*			pMsg1;
	MyMessage*			pMsg2;
	MyMessage*			pMsg3;
	MyMessage*			pMsg4;
	scMutexNoOp			lock;

	emtpyAllocator*		emptyAlloc = new emtpyAllocator();
	scAllocator_Imp*	pNewOp = new scAllocator_Imp();

	scAllocator			memManager( pNewOp );
	scAllocator			memManager2( emptyAlloc );

	MessageFactory*		pFactory = new MessageFactory(3, 0);

	EXPECT_CALL( *pFactory, PostCreateP(_)).Times(AtLeast(2));

	pFactory->Initialize( memManager, memManager, &lock );

	pMsg1 = pFactory->Create( NULL, 0 );
	EXPECT_TRUE( pMsg1 != NULL );
	pMsg2 = pFactory->Create( 20 );
	EXPECT_TRUE( pMsg2 != NULL );
	pMsg3 = pFactory->Create( 20 );
	EXPECT_TRUE( pMsg3 != NULL );
	pMsg4 = pFactory->Create( 20 );
	EXPECT_FALSE( pMsg4 != NULL );

	pFactory->DebugDump();

	EXPECT_TRUE( pFactory->Release( pMsg2 ) );
	EXPECT_TRUE( pFactory->Release( pMsg1 ) );
	EXPECT_TRUE( pFactory->Release( pMsg3 ) );
	EXPECT_FALSE( pFactory->Release( pMsg4 ) );

	pMsg1 = pFactory->Create( 90 );
	EXPECT_TRUE( pMsg1 != NULL );
	EXPECT_EQ( 90, pMsg1->LengthOfBuffer() );

	// this will rollover to the external allocator
	pMsg2 = pFactory->Create( &manual[0], sizeof( manual ) );

	//EXPECT_TRUE( pMsg2 == NULL );
	EXPECT_TRUE( pFactory->Release( pMsg2 ) );
	EXPECT_TRUE( pFactory->Release( pMsg1 ) );


	delete pFactory;
	delete pNewOp;
	delete emptyAlloc;
}

void scMessage_tests::FactoryTestUsedNoOverflow()
{
	MyMessage*			pMsg1;
	MyMessage*			pMsg2;
	MyMessage*			pMsg3;
	MyMessage*			pMsg4;
	scMutexNoOp			lock;
	scMutexNoOp			lock2;
	emtpyAllocator*		emptyAlloc = new emtpyAllocator();
	scAllocator_Imp*	pNewOp = new scAllocator_Imp();
	uint8_t*			pRingMemory = new uint8_t[100];

	scAllocator			memManager( pNewOp );
	scAllocator			memManager2( emptyAlloc );

	MessageFactory*		pFactory = new MessageFactory(3, 40);
	scRingBuffer*		pRingBuffer = new scRingBuffer( 100, pRingMemory, &lock2 );

	EXPECT_TRUE( pRingBuffer->WriteStart() > sizeof( manual ) );
	memcpy( pRingBuffer->WriteBlock() , &manual[0], sizeof( manual ) );
	pRingBuffer->WriteEnd( sizeof( manual ) );

	// This test the init failure when the record slots cannot be allocated
	failMessageFactory*	pFailFactory = new failMessageFactory(3, 40);
	pFailFactory->Initialize( memManager2, memManager2, &lock );
	EXPECT_EQ( ERROR_SC_MEMORY_ALLOCATION_FAILURE, pFailFactory->GetLastError() );
	delete pFailFactory;

	// This will test the case where the MessageObject_Allocate() fails
	pFailFactory = new failMessageFactory(3, 40);
	pFailFactory->Initialize( memManager, memManager, &lock );
	EXPECT_EQ( ERROR_SC_MEMORY_ALLOCATION_FAILURE, pFailFactory->GetLastError() );
	delete pFailFactory;


	EXPECT_CALL( *pFactory, PostCreateP(_)).Times(AtLeast(2));

	pFactory->Initialize( memManager, memManager2, &lock );
	EXPECT_EQ( ERROR_SUCCESS, pFactory->GetLastError() );

	pMsg1 = pFactory->Create( 20 );
	EXPECT_TRUE( pMsg1 != NULL );
	pMsg2 = pFactory->Create( 20 );
	EXPECT_TRUE( pMsg2 != NULL );
	// out of memory failure
	pMsg3 = pFactory->Create( 20 );
	EXPECT_FALSE( pMsg3 != NULL );

	EXPECT_TRUE( pFactory->Release( pMsg1 ) );
	EXPECT_TRUE( pFactory->Release( pMsg2 ) );
	pMsg1 = pFactory->Create( 10 );
	EXPECT_TRUE( pMsg1 != NULL );
	pMsg2 = pFactory->Create( 10 );
	EXPECT_TRUE( pMsg2 != NULL );
	pMsg4 = pFactory->Create( 10 );
	EXPECT_TRUE( pMsg4 != NULL );


	// no slot failure with the pointer
	pMsg3 = pFactory->Create( &manual[0], sizeof(manual) );
	EXPECT_FALSE( pMsg3 != NULL );

	// not slot failure with the ring buffer
	pMsg3 = pFactory->Create( pRingBuffer );
	EXPECT_FALSE( pMsg3 != NULL );
	EXPECT_EQ( ERROR_SC_MESSAGEFACTORY_FULL, pFactory->GetLastError() );

	// no slot failure with copy
	pMsg3 = pFactory->Copy( pMsg2 );
	EXPECT_FALSE( pMsg3 != NULL );
	EXPECT_EQ( ERROR_SC_MESSAGEFACTORY_FULL, pFactory->GetLastError() );

	pFactory->DebugDump();

	EXPECT_TRUE( pFactory->Release( pMsg4 ) );
	EXPECT_TRUE( pFactory->Release( pMsg2 ) );
	// this is a very bad usage sequence but the code shouldn't break
	pFactory->Acquire( pMsg2 );	// not really an error but generates a warning.
	// this
	EXPECT_TRUE( pFactory->Release( pMsg2 ) );
	EXPECT_FALSE( pFactory->Release( pMsg2 ) );
	// end bad sequence
	EXPECT_TRUE( pFactory->Release( pMsg1 ) );
	EXPECT_FALSE( pFactory->Release( pMsg3 ) );


	// Create the case for using memory at the start of the buffer
	pMsg1 = pFactory->Create( 10 );
	pMsg2 = pFactory->Create( 10 );						// first 20 bytes in use
	EXPECT_TRUE( pFactory->Release( pMsg1 ) );			// free first 20
	pMsg4 = pFactory->Create( 15 );						// grab the 15 more near buffer end
	pMsg1 = pFactory->Create( 10 );						// this should get from the start again.

	EXPECT_TRUE( pFactory->Release( pMsg1 ) );
	EXPECT_TRUE( pFactory->Release( pMsg2 ) );
	EXPECT_TRUE( pFactory->Release( pMsg4 ) );

	delete pFactory;
	delete pNewOp;
	delete emptyAlloc;
	delete pRingBuffer;
	delete pRingMemory;
}

#define ALLOCATION_COUNT	10000

void scMessage_tests::FactoryStressTest()
{
	vector<MyMessage*>	testMessages;
	scMutexNoOp			lock;
	scAllocator_Imp*	pNewOp = new scAllocator_Imp();
	scAllocator			memManager( pNewOp );
	MessageFactory*		pFactory = new MessageFactory(100, 50);

	EXPECT_CALL( *pFactory, PostCreateP(_)).Times(AtLeast(ALLOCATION_COUNT));

	pFactory->Initialize( memManager, memManager, &lock );

	for( int i=0; i < ALLOCATION_COUNT; i++ )
	{
		if ( pFactory->MessagesAvailable() == 0 )
		{
			bool flipFlop = false;
			// slots are full delete some
			vector<MyMessage*>::iterator itr = testMessages.begin();
			for( ; itr != testMessages.end(); ++itr )
			{
				flipFlop = !flipFlop;
				if ( flipFlop )
				{
					EXPECT_TRUE( pFactory->Release( (*itr) ) );
					itr = testMessages.erase( itr );
					if ( itr == testMessages.end() )
					{
						itr--;
					}
				}
			}
		}

		MyMessage* pNext = pFactory->Create(10);
		EXPECT_TRUE( pNext != NULL );
		testMessages.push_back( pNext );
	}

	vector<MyMessage*>::iterator itr = testMessages.begin();
	for( ; itr != testMessages.end(); ++itr )
	{
		EXPECT_TRUE( pFactory->Release( (*itr) ) );
	}
	testMessages.clear();

	delete pFactory;
	delete pNewOp;

}
