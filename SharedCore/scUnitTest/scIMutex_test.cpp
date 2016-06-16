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
#include "scUnitTest/scIMutex_test.h"



bool MutexTest( MockscIMutex& mutex )
{
	mutex.Acquire();
	mutex.TryAcquire( 1000 );
	mutex.Release();
	mutex.ReleaseISR();
	return true;
}


bool SemaphoreTest( MockscISemaphore& semp )
{
	semp.Wait();
	semp.Wait(1000);
	semp.Release();
	semp.ReleaseISR();
	return true;
}