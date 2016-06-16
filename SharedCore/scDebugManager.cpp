//==============================================================================
//          UNCLASSIFIED
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
// File Name:		scDebugManager.h
// Created By:		Christopher Snyder
// Creation Date:	14-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================

#include <string.h>
#include <stdio.h>
#include "scDebugManager.h"
#include "scSingletonPtr.h"

#ifndef assert_param
#define assert_param(X)	if ( !(X) ) { for(;;) ; }
#endif


#ifdef DEBUG_MANAGER_BUFFER_SIZE
	#define MAX_STRING_LEN DEBUG_MANAGER_BUFFER_SIZE
#else
	#define MAX_STRING_LEN 256
#endif

using namespace SharedCore;

static scSingletonPtr<scDebugManager> g_Instance;

/// <summary>
/// Access to the singleton object
/// </summary>
scDebugManager* scDebugManager::Instance(void)
{
	return g_Instance.Ptr();
}

/// <summary>
/// Simple Destructor
/// </summary>
scDebugManager::~scDebugManager()
{
	Shutdown();
}

/// <summary>
/// The default constructor. Must be called from the child.
/// </summary>
scDebugManager::scDebugManager(void)
	: _nState(scDisabled)
	, _Paths(0)
	, _pLabelManager(NULL)
	, _Allocator( NULL )
{
}

/// <summary>
/// This method will change the enabled state of the provided path ID. When enabled,
/// debug messages will use that path, when disabled no message will use that path.
/// 
/// </summary>
/// <param name="nPathId">The ID of the path to change state.</param>
/// <param name="nState ">True to enable the path, false to disable the
/// path</param>
void scDebugManager::SetPathEnable(uint8_t nPathId, scEnableState_t nState )
{
	vector<scDebugPath*>::iterator itr = _Paths.begin();
	for( ; itr != _Paths.end(); itr++ )
	{
		if ( (*itr)->PathId() == nPathId )
		{
			(*itr)->State( nState );
		}
	}
}

/// <summary>
/// Get the current enable state of the specified path.
/// </summary>
/// <param name="nPathId">the path of the state to check</param>
scEnableState_t scDebugManager::GetPathEnable(uint8_t nPathId)
{
	scEnableState_t nResult = scDisabled;
	vector<scDebugPath*>::iterator itr = _Paths.begin();
	for( ; itr != _Paths.end(); itr++ )
	{
		if ( (*itr)->PathId() == nPathId )
		{
			nResult = (*itr)->State();
			break;
		}
	}
	return nResult;
}

/// <summary>
/// This will enable the debug manage, the current state of the paths are now in
/// effect.
/// </summary>
void scDebugManager::Enable(void)
{
	_nState = scEnabled;
}

/// <summary>
/// This will disable the debug manage from logging any messages. The path states
/// do not change and are not called.
/// </summary>
void scDebugManager::Disable(void)
{
	_nState = scDisabled;
}

/// <summary>
/// Get the current state of the manager.
/// </summary>
scEnableState_t scDebugManager::State(void) const 
{
	return _nState;
}

/// <summary>
/// This method will add a new path to the debug manger. Once added the debug
/// manager will be responsible for deleting the memory associated with this
/// pointer.
/// </summary>
/// <param name="pPath">A pointer to the new path.</param>
size_t scDebugManager::Add(scDebugPath* pPath)
{
	_Paths.push_back( pPath );
	return  _Paths.size();
}

/// <summary>
/// This path will remove the path from the list and return the pointer.
/// </summary>
/// <param name="nPathId">The path id</param>
scDebugPath* scDebugManager::Remove( uint8_t nPathId )
{
	scDebugPath* pResult = NULL;
	vector<scDebugPath*>::iterator itr = _Paths.begin();
	for( ; pResult == NULL && itr != _Paths.end(); itr++ )
	{
		if ( (*itr)->PathId() == nPathId )
		{
			pResult = *itr;
			itr = _Paths.erase( itr );
			break;
		}
	}
	return pResult;
}

/// <summary>
/// Change the enable state of all paths known to the debugger.
/// </summary>
/// <param name="nState">the new state of all paths.</param>
void scDebugManager::SetPaths( scEnableState_t nState )
{
	vector<scDebugPath*>::iterator itr = _Paths.begin();
	for( ; itr != _Paths.end(); itr++ )
	{
		(*itr)->State(nState);
	}
}

/// <summary>
/// Get the current state of the debug label.
/// </summary>
/// <param name="nLabel">the id of the debug label.</param>
scEnableState_t scDebugManager::LabelState(uint16_t nLabel) const 
{
	scEnableState_t result = scDisabled;
	if ( _pLabelManager != NULL )
	{
		result = _pLabelManager->LabelState( nLabel );
	}
	return result;
}

/// <summary>
/// Get the current state of the debug label.
/// </summary>
/// <param name="nLabel">the id of the debug label.</param>
/// <param name="nState">The new state for the label.</param>
void scDebugManager::LabelState(uint16_t nLabel, scEnableState_t nState)
{
	if ( _pLabelManager != NULL )
	{
		_pLabelManager->LabelState( nLabel, nState );
	}
}

/// <summary>
/// Changes the state of all labels to be the provided state.
/// </summary>
/// <param name="nState">The new state for the label.</param>
void scDebugManager::LabelState( scEnableState_t nState)
{
	if ( _pLabelManager != NULL )
	{
		_pLabelManager->LabelState( nState );
	}
}

/// <summary>
/// Will get a text string for the enable state enum.
/// </summary>
/// <param name="nState">the state</param>
const char* scDebugManager::StateText(scEnableState_t nState)
{
	return ( nState == scEnabled ? "Enabled" : "Disabled" );
}

/// <summary>
/// Convert a byte value to a hex value.
/// </summary>
/// <param name="pOut"></param>
/// <param name="nByte"></param>
char* scDebugManager::ByteToHex(char* pOut, uint8_t nByte)
{
	(*pOut) = nByte >> 4;
	(*pOut)+= ((*pOut) < 0xA ? 0x30 : 0x57 );		// Use 0x37 for upper case
	pOut++;											// Use 0x57 for lower case
	(*pOut) = nByte & 0x0F;
	(*pOut)+= ((*pOut) < 0xA ? 0x30 : 0x57 );
	return ++pOut;
}

/// <summary>
/// This method will convert a memory array to a hex dump.
/// </summary>
/// <param name="nLabel">the label.</param>
/// <param name="pBuffer">Pointer to the memory to be printed.</param>
/// <param name="nLength">The number of bytes in the memory to print.</param>
void scDebugManager::PrintArray( uint16_t nLabel, const uint8_t* pBuffer, uint32_t nLength )
{
	assert_param( _pLabelManager != NULL );

	if ( _pLabelManager->LabelState( nLabel ) == scEnabled )
	{
		char* sLine = WorkingBuffer_Lock();

		int nCol = 0;
		int nRow = 0;
		uint32_t i;
		int p = 0;
		const char* pText = reinterpret_cast<const char*> (pBuffer);

		i=0;

		while( i < nLength )
		{
			p=0;
			sLine[p++] = '0';
			sLine[p++] = 'x';
			ByteToHex( sLine+p, (uint8_t)nRow );
			p+=2;
			sLine[p++] = ' ';

			for( nCol=0; nCol < 16; ++nCol )
			{
				if ( i < nLength )
				{
					ByteToHex( sLine+p, pBuffer[i++] );
					p+=2;
					sLine[p++] = ' ';
				}
				else
				{
					sLine[p++] = ' ';
					sLine[p++] = ' ';
					sLine[p++] = ' ';
				}
			}

			sLine[p++] = '[';
			for( nCol = 0; nCol < 16; nCol++ )
			{
				if ( pText < reinterpret_cast<const char*>(pBuffer + nLength) )
				{
					if ( *pText < 0x20 || *pText > 0x7E )
					{
						sLine[p++] = '.';
					}
					else if ( *pText == '%' )
					{
						// Since this string goes into a printf function you need to escape the % symbol
						sLine[p++] = '%'; //sLine[p++] = '%';
					}
					else
					{
						sLine[p++] = *pText;
					}
					pText++;
				}
				else
				{
					sLine[p++] = ' ';
				}
			}
			sLine[p++] = ']';
			sLine[p++] = '\r'; sLine[p++] = '\n'; sLine[p++] = 0;
			Trace_Info( nLabel, sLine );
			++nRow;
			if ( nRow > 8 )
			{
				// force a delay so the uart can send
				for( int j=0; j < 100000; ++j ) ;
			}
		}
		WorkingBuffer_Release(sLine);
	}
}

/// <summary>
/// This is the logging method that will behave like a sprintf and format the
/// string.
/// </summary>
/// <param name="nLabel">The debug label.</param>
/// <param name="pFormat">Format of the output.</param>
/// <param name="..."></param>
void scDebugManager::Trace(uint16_t nLabel, const char* pFormat, ...)
{
	va_list ap;
	//lint -e{ 64 }
	va_start( ap, pFormat );
	vTrace( nLabel, pFormat, ap );
	va_end( ap );
}

/// <summary>
/// This is a direct output function that does not perform any formatting of the
/// text prior to sending to the paths. This can be faster than the trace method
/// since sprintf is not used. This also does not use the working buffer to swap
/// the memory.
/// </summary>
/// <param name="nLabel">The debug label.</param>
/// <param name="pBuffer">The message, must be null terminated.</param>
void scDebugManager::Trace_Info(uint16_t nLabel, const char* pBuffer)
{
	assert_param( _pLabelManager != NULL );
	if ( _pLabelManager->LabelState( nLabel ) == scEnabled )
	{
		int nLogLength = strlen( pBuffer );
		vector<scDebugPath*>::iterator itr = _Paths.begin();
		for( ; itr != _Paths.end(); itr++ )
		{
			if ( (*itr)->State() == scEnabled )
			{
				(*itr)->Capture( reinterpret_cast<const uint8_t*>(pBuffer), nLogLength );
			}
		}
	}
}

/// <summary>
/// This is the implementation that will convert the debug formatted string to the
/// output.
/// </summary>
/// <param name="nLabel">The debug label.</param>
/// <param name="pFormat">The format string.</param>
/// <param name="ap">the variable argument list.</param>
void scDebugManager::vTrace(uint16_t nLabel, const char* pFormat, va_list ap)
{
	int nLogLength = 0;
	assert_param( _pLabelManager != NULL );

	if ( _nState == scEnabled &&  _pLabelManager->LabelState(nLabel) == scEnabled )
	{
		// This buffer may need to be obtained from a memory manager of some sort.
		// Also this length may be made configurable. Also what to do about multithreaded
		// apps where there may be collisions on the debug output.
		char* pBuffer = WorkingBuffer_Lock();

//lint --e(774)      // depends of the compiler options if true
		if ( pBuffer != NULL )
		{
#ifdef _DEBUG
			if ( strlen( pFormat ) > MAX_STRING_LEN )
			{
				// BAD just overran the buffer. Can't do this. Make debug statements much shorter.
				assert_param( strlen( pFormat ) > MAX_STRING_LEN );
				return;
			}
#endif
#ifdef _WIN32
			nLogLength = vsprintf_s( pBuffer, MAX_STRING_LEN, pFormat, ap );
#else
			nLogLength = vsprintf( pBuffer, pFormat, ap );
#endif
#ifdef _DEBUG
			if ( nLogLength >= MAX_STRING_LEN )
			{
				// BAD just overran the buffer. Can't do this. Make debug statements much shorter.
				assert_param( nLogLength >= MAX_STRING_LEN );
				return;
			}
#endif
			vector<scDebugPath*>::iterator itr = _Paths.begin();
			for( ; itr != _Paths.end(); itr++ )
			{
				if ( (*itr)->State() == scEnabled )
				{
					(*itr)->Capture( reinterpret_cast<const uint8_t*>(pBuffer), nLogLength );
				}
			}
			WorkingBuffer_Release( pBuffer );
		}
		else
		{
			assert_param( pBuffer != NULL );
		}
	}
}

/// <summary>
/// set the label manager.
/// By design the there should only be one label manager ever assigned.
/// Any additional manager assignments are rejected because possible memory
/// leaks.
/// </summary>
scIDebugLabelManager* scDebugManager::SetLabelManager( scIDebugLabelManager* pLabelManager )
{
	assert_param( pLabelManager != NULL );
	scIDebugLabelManager* pOld = _pLabelManager;
	_pLabelManager = pLabelManager;
	return pOld;
}

/// <summary>
/// Assign the allocator
/// </summary>
void scDebugManager::Allocator( scAllocator pAllocator )
{
	_Allocator = pAllocator;
}
		
/// <summary>
/// get the allocator
/// </summary>
const scAllocator& scDebugManager::Allocator( void ) const
{
	return _Allocator;
}

/// <summary>
/// get access to the buffer used by the DebugManager.
/// to change the buffer management subclass this manager and use a different memory allocator
/// </summary>
char* scDebugManager::WorkingBuffer_Lock(void)
{
	return reinterpret_cast<char*>(_Allocator.Allocate( MAX_STRING_LEN ) );
}

/// <summary>
/// get access to the buffer used by the DebugManager.
/// to change the buffer management subclass this manager and use a different memory allocator
/// </summary>
void  scDebugManager::WorkingBuffer_Release(char* pBuffer)
{
	_Allocator.Destroy( pBuffer );
}

/// <summary>
/// For applications where the debug manager needs to exist and have it's resources
/// cleaned up, this method will disable the logger and delete the paths and manager
/// </summary>
void scDebugManager::Shutdown(void)
{
	Disable();
	delete []_pLabelManager;
	_pLabelManager = NULL;
	vector<scDebugPath*>::iterator itr = _Paths.begin();
	for( ; itr != _Paths.end(); itr++ )
	{
		delete (*itr);
		(*itr) = NULL;
	}
}


