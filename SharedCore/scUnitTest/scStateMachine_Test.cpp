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


#include "scStateMachine_Test.h"


static const int g_table[] = 
{
// Input	Adv		Hit		Error
			1,		0,		0,		// State 0
			2,		2,		0,		// State 1
			0,		1,		0		// State 2
};


scStateMachine_Test::scStateMachine_Test(void)
{
	// You can do set-up work for each test here.
	m_pSM = new scStateMachine( 3, 3, 0, g_table );
}


scStateMachine_Test::~scStateMachine_Test(void)
{
	// You can do clean-up work that doesn't throw exceptions here.
	delete m_pSM;
}

void scStateMachine_Test::TransitionTest(void)
{
	EXPECT_EQ( 0, (*m_pSM)() );
	m_pSM->advance(scStateMachine_Test::Advance);
	EXPECT_EQ( 1, (*m_pSM)() );
	EXPECT_EQ( 2, m_pSM->advance(scStateMachine_Test::Advance) );
	EXPECT_EQ( 1, m_pSM->advance(scStateMachine_Test::Hit) );
	EXPECT_EQ( 2, m_pSM->advance(scStateMachine_Test::Hit) );
	EXPECT_EQ( 1, m_pSM->advance(scStateMachine_Test::Hit) );
	EXPECT_EQ( 0, m_pSM->advance(scStateMachine_Test::Error) );
	EXPECT_EQ( 0, m_pSM->advance(scStateMachine_Test::Hit) );
	EXPECT_EQ( 1, m_pSM->advance(scStateMachine_Test::Advance) );
	EXPECT_EQ( 2, m_pSM->advance(scStateMachine_Test::Advance) );
	EXPECT_EQ( 0, m_pSM->advance(scStateMachine_Test::Error) );
}