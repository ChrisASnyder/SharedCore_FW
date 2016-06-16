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


#include "gmock/gmock.h"  // Brings in Google Mock.
#include "scTypes.h"
#include "scUnitTest/scIQueue_test.h"


bool MockscIQueue_Test( MockscIQueue& q )
{
	q.Create( 10, sizeof(uint32_t) );

	for( uint32_t i = 0; i < 10; i++ )
	{
		q.SendToFront( (void*)i, 0 );
	}

	for( uint32_t i = 0; i < 10; ++i )
	{
		uint32_t nvalue;
		q.Receive( &nvalue, 0 );
	}

	for( uint32_t i = 0; i < 10; i++ )
	{
		q.SendToBack( (void*)i, 0 );
	}

	uint32_t oneEntry;
	q.Peek( &oneEntry, 0 );

	for( uint32_t i = 0; i < 10; ++i )
	{
		uint32_t nvalue;
		q.Receive( &nvalue, 0 );
	}

	uint32_t nSpace = q.SpacesAvailable();
	return true;
}
