#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"  // Brings in Google Mock.
#include "scModuleManager.h"

using namespace ::SharedCore;

#define NAME_SIZE 10

class scModuleManager_test : public ::testing::Test
{
public:
	void BasicManagerTest();


protected:

	scModuleManager_test(void);

	virtual ~scModuleManager_test();
	
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

	class TestModule : public scIModule
	{
	public:
		TestModule(void) : scIModule() {};
		MOCK_METHOD2( Notify, int( uint32_t nCommand, void* pParameter ) );
	};

	class MyManager : public scModuleManager
	{
	public:
		MyManager(void);
		virtual void Initialize(void);

	};

};

