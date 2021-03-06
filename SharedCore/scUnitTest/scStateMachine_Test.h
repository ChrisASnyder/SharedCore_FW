//==============================================================================
//          � Copyright Common Ground Software Solutions 2014
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
#include "scStateMachine.h"

using namespace ::SharedCore;

class scStateMachine_Test : public ::testing::Test
{
public :
	typedef enum 
	{
		Advance,
		Hit,
		Error,
	} smInputs_t;

	void TransitionTest(void);

protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	scStateMachine_Test();

	virtual ~scStateMachine_Test();

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	// virtual void SetUp() will be called before each test is run.  You
	// should define it if you need to initialize the variables.
	// Otherwise, this can be skipped.

	virtual void SetUp() 
	{
		// Code here will be called immediately after the constructor (right
		// before each test).
	}

	virtual void TearDown()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	scStateMachine*	m_pSM;

};


