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

#include "gtest/gtest.h"
#include "gmock/gmock.h"  // Brings in Google Mock.
#include "scRingBuffer.h"

using namespace ::SharedCore;

#define NAME_SIZE 10

class scRingBuffer_Test : public ::testing::Test
{
public:
	void SingleByteFill();

protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	scRingBuffer_Test();

	virtual ~scRingBuffer_Test();

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	// virtual void SetUp() will be called before each test is run.  You
	// should define it if you need to initialize the variables.
	// Otherwise, this can be skipped.

	virtual void SetUp();

	virtual void TearDown();

	scRingBuffer*			_pBuffer;
	uint32_t				_nBufferSize;


};


