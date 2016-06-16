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
#include "scDebugManager_test.h"
#include "scAllocator_Imp.h"

using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::_;
using namespace SharedCore;

static scAllocator_Imp* pAllocatorImp = new scAllocator_Imp();

scDebugManager_test::scDebugManager_test()
{
}

scDebugManager_test::~scDebugManager_test()
{
}

void scDebugManager_test::SetUp()
{
}

void scDebugManager_test::TearDown()
{
	// Do this to avoid tripping tests and to allow debug manager to work 
	// in the rest of the testing.
	scDebugManager::Instance()->Add( new MyPathSystem(1) );

	scDebugLabelManager*		pmgr = new scDebugLabelManager();
	scIDebugLabelManager* pme = scDebugManager::Instance()->SetLabelManager( pmgr );
	delete pme;
}


void scDebugManager_test::SimplePathTest()
{
	MyPath	path(2);

	EXPECT_EQ(2, path.PathId() );
	EXPECT_EQ( scEnabled, path.State() );
	path.Disable();
	EXPECT_EQ( scDisabled, path.State() );

	path.Enable();
	EXPECT_EQ( scEnabled, path.State() );
	path.Disable();
	EXPECT_EQ( scDisabled, path.State() );
	path.Enable();
	EXPECT_EQ( scEnabled, path.State() );

	/* Debug Manager setup routines */
	MyPath*		pPath = new MyPath(0);

	scDebugManager* pDm = scDebugManager::Instance();

	pDm->Allocator( pAllocatorImp );
	pDm->Add( pPath );

	EXPECT_EQ( scEnabled, pDm->GetPathEnable(0) );
	pDm->SetPathEnable( 0, scDisabled );
	EXPECT_EQ( scDisabled, pDm->GetPathEnable(0) );
	pDm->SetPaths( scEnabled );
	EXPECT_EQ( scEnabled, pDm->GetPathEnable(0) );

	scDebugLabelManager*		pMyLabels = new MyLabels();
	
	EXPECT_EQ( scEnabled, pMyLabels->LabelState(0) );
	EXPECT_EQ( scEnabled, pMyLabels->LabelState(1) );
	EXPECT_EQ( scDisabled, pMyLabels->LabelState(2) );
	pMyLabels->LabelState(0, scDisabled );
	EXPECT_EQ( scDisabled, pMyLabels->LabelState(0) );
	pMyLabels->LabelState(0, scEnabled );
	EXPECT_EQ( scEnabled, pMyLabels->LabelState(0) );

	pMyLabels->LabelState(2, scEnabled );	// this is out of range
	EXPECT_EQ( scDisabled, pMyLabels->LabelState(2) );

	pMyLabels->LabelState( scDisabled );
	EXPECT_EQ( scDisabled, pMyLabels->LabelState(0) );
	EXPECT_EQ( scDisabled, pMyLabels->LabelState(1) );
	pMyLabels->LabelState( scEnabled );
	EXPECT_EQ( scEnabled, pMyLabels->LabelState(0) );
	EXPECT_EQ( scEnabled, pMyLabels->LabelState(1) );

	EXPECT_EQ( NULL, scDebugManager::Instance()->SetLabelManager( pMyLabels ) );
	EXPECT_EQ( scEnabled, pDm->LabelState(0) );
	EXPECT_EQ( scEnabled, pDm->LabelState(1) );

	pDm->LabelState(1, scDisabled );
	EXPECT_EQ( scDisabled, pDm->LabelState(1) );
	pDm->LabelState( scEnabled );
	EXPECT_EQ( scEnabled, pDm->LabelState(1) );

	pDm->Disable();
	EXPECT_EQ( scDisabled, pDm->State() );
	pDm->Enable();
	EXPECT_EQ( scEnabled, pDm->State() );

	EXPECT_CALL( *pPath, Capture( _, _ )).Times(AtLeast(4));


	pDm->Enable();
	pDm->LabelState(0, scEnabled );

	pDm->Trace( 0, "%s", "This is a test." );
	pDm->Trace_Info( 0, "This is a test of a static buffer." );

	int				nMemorySize = 25;
	uint8_t*		pMemory = new uint8_t[nMemorySize];

	for( int i=0; i < nMemorySize; ++i )
	{
		pMemory[i] = i * 2;
	}

	pDm->PrintArray( 0, pMemory, nMemorySize );

	// note after this no more test can be executed.
	// Even though the static objects destroy calls this method
	// If I don't call it here then the Mock allocations complain
	// about memory leaks even though there technically isn't one.
	// perhaps the manager could be designed a little better.
	delete scDebugManager::Instance()->Remove( pPath->PathId() );

	delete []pMemory;
}
