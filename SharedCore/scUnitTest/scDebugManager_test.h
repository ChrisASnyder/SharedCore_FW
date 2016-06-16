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

#pragma once

#include <stdarg.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"  // Brings in Google Mock.
#include "scDebugPath.h"
#include "scDebugLabelManager.h"
#include "scDebugManager.h"

using namespace ::SharedCore;

#define NAME_SIZE 10

// Going to use this class to hold all the tests related to the device descriptor classes. 
// just to keep things a little cleaner.
class scDebugManager_test : public ::testing::Test
{
public:
	void SimplePathTest();

	class MyPath : public scDebugPath
	{
	public:
		MyPath( uint8_t id ) : scDebugPath(id) {}
		MOCK_METHOD2( Capture, void(const uint8_t*, uint32_t) );

	};

	class MyPathSystem : public scDebugPath
	{
	public:
		MyPathSystem( uint8_t id ) : scDebugPath(id) {}
		void Capture( const uint8_t*, uint32_t )
		{
		}

	};

	class MyLabels : public scDebugLabelManager
	{
	public:
		MyLabels() : scDebugLabelManager( 2, scEnabled )
		{
		}

		virtual const char* LabelText( uint16_t nLabel )
		{
			const char* pResult;
			switch( nLabel )
			{
			default:
				pResult = "Unknown";
				break;
			case 1:
				pResult = "Label 1";
				break;
			case 2:
				pResult = "Label 2";
				break;
			}
			return pResult;
		}
	};

protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	scDebugManager_test();

	virtual ~scDebugManager_test();

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	// virtual void SetUp() will be called before each test is run.  You
	// should define it if you need to initialize the variables.
	// Otherwise, this can be skipped.

	virtual void SetUp();

	virtual void TearDown();

};


