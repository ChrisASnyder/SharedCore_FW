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

#include "scRingBuffer_Test.h"
#include "scFSMState.h"


static const size_t g_pMatix[] = 
{
	// States	Step1	Step2,	Step 3
				1,		2,		1,			// Input Advance
				0,		0,		0,			// Input Reset
};


scRingBuffer_Test::scRingBuffer_Test(void)
	: _pBuffer(NULL)
	, _nBufferSize(20)
{
	// the ring buffer delete the buffer.
	_pBuffer = new scRingBuffer( _nBufferSize, new uint8_t[_nBufferSize], NULL );
}


scRingBuffer_Test::~scRingBuffer_Test(void)
{
	// You can do clean-up work that doesn't throw exceptions here.
	delete _pBuffer;
}

void scRingBuffer_Test::SetUp()
{
}

void scRingBuffer_Test::TearDown()
{
}

void scRingBuffer_Test::SingleByteFill()
{
	uint32_t nSize = 0;
	uint32_t nUsed = 0;

	EXPECT_EQ( _nBufferSize, _pBuffer->WriteStart() );
	memset( _pBuffer->WriteBlock(), 0, 5 );
	EXPECT_EQ( _nBufferSize-5, _pBuffer->WriteEnd(5) );
	EXPECT_EQ( 5, _pBuffer->ReadStart() );
	EXPECT_EQ( 0x00, *(_pBuffer->ReadBlock()) );
	EXPECT_EQ( 0, _pBuffer->ReadEnd( 5 ) );
	nUsed+=5;

	for( uint32_t i=0; i < _nBufferSize; ++i )
	{
		nSize = _pBuffer->WriteStart();
		EXPECT_EQ( _nBufferSize-i, _pBuffer->Available() );
		*(_pBuffer->WriteBlock()) = 0xAA;
		nUsed++;
		_pBuffer->WriteEnd( 1 );
	}

	EXPECT_EQ( 0, _pBuffer->Available() );

	for( uint32_t i=0; i < _nBufferSize; ++i )
	{
		nSize = _pBuffer->ReadStart();
		EXPECT_EQ( i, _pBuffer->Available() );
		EXPECT_EQ( 0xAA, *(_pBuffer->ReadBlock()) );
		nUsed--;
		_pBuffer->ReadEnd( 1 );
	}

	EXPECT_EQ( _nBufferSize, _pBuffer->Available() );
}

