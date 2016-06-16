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

#include "gmock/gmock.h"  // Brings in Google Mock.
#include "scIQueue.h"

using namespace ::SharedCore;

class MockscIQueue : public scIQueue
{
public:
	MockscIQueue(void) : scIQueue() {};
	MockscIQueue(uint32_t nSize, uint32_t nItemSize ) 
		: scIQueue(nSize, nItemSize ) {};

	virtual ~MockscIQueue() {}

	MOCK_METHOD2(Create, uint32_t( uint32_t size, uint32_t nItemSize ) );
	MOCK_METHOD2(SendToFront, uint32_t( const void* pItem, uint32_t nTicks ) );
	MOCK_METHOD2(SendToBack, uint32_t( const void* pItem, uint32_t nTicks ) );
	MOCK_METHOD1(SendToFrontISR, uint32_t( const void* pItem ) );
	MOCK_METHOD1(SendToBackISR, uint32_t( const void* pItem ) );
	MOCK_METHOD2(Peek, uint32_t( void* pItem, uint32_t nTicks ) );
	MOCK_METHOD2(Receive, uint32_t( void* pItem, uint32_t nTicks ) );
	MOCK_METHOD0(SpacesAvailable, uint32_t() );
};


bool MockscIQueue_Test( MockscIQueue& q );
