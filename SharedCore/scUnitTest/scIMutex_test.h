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
#include "scIMutex.h"
#include "scISemaphore.h"
using namespace ::SharedCore;

class MockscIMutex : public scIMutex 
{
public:
	MockscIMutex(void) : scIMutex() {};
	virtual ~MockscIMutex() {}

	MOCK_METHOD1(TryAcquire, int(uint32_t timeout) );
	MOCK_METHOD0(Acquire, void() );
	MOCK_METHOD0(Release, void() );
	MOCK_METHOD0(ReleaseISR, void() );
};

class MockscISemaphore : public scISemaphore
{
public:
	MockscISemaphore(void) : scISemaphore() {};
	virtual ~MockscISemaphore() {}

	MOCK_METHOD1(Wait, bool(uint32_t timeout) );
	MOCK_METHOD0(Wait, bool() );
	MOCK_METHOD0(Release, void() );
	MOCK_METHOD0(ReleaseISR, void() );
};

bool MutexTest( MockscIMutex& mutex );
bool SemaphoreTest( MockscISemaphore& semp );
