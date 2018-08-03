//============================================================================
// Memory.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Memory Implementation
//----------------------------------------------------------------------------

_char*	Memory::sOwnerFileName		= _null;
_dword	Memory::sOwnerLineNumber	= 0;
_void*	Memory::sAllocater			= _null;

_void Memory::OnFreshMemoryMonitor( _dword monitorid )
{
	Memory* globalmemory = GetFairyLandGlobal( ).mMemory;

	if ( globalmemory == _null )
		return;

	globalmemory->mMemoryMonitorWindow->AddGraphRecordSample( monitorid, 0, System::GetCurrentMemoryUsage( ) / 1024 / 1024 );
	globalmemory->mMemoryMonitorWindow->AddGraphRecordSample( monitorid, 1, globalmemory->mCurAllocTimes );
	globalmemory->mMemoryMonitorWindow->AddGraphRecordSample( monitorid, 2, globalmemory->mHappenAllocTimes );
	globalmemory->mMemoryMonitorWindow->FreshMonitor( monitorid );

	globalmemory->mHappenAllocTimes = 0;
}

_void Memory::Initialize( _dword debugmode )
{
	// Create global memory system.
	Memory* globalmemory = (Memory*) HeapAlloc( sizeof( Memory ) );

	globalmemory->mMemoryLock.Initialize( );
	globalmemory->mHead					= _null;
	globalmemory->mTail					= _null;
	globalmemory->mNumber				= 0;
	globalmemory->mHappenAllocTimes		= 0;
	globalmemory->mCurAllocTimes		= 0;
	globalmemory->mCurAllocSize			= 0;
	globalmemory->mMaxAllocTimes		= 0;
	globalmemory->mMaxAllocSize			= 0;
	globalmemory->mDebugMode			= debugmode;
	globalmemory->mMemoryMonitorWindow	= _null;
	globalmemory->mMemoryMonitorID		= 0;

	GetFairyLandGlobal( ).mMemory = globalmemory;
}

_void Memory::Finalize( )
{
	Memory* globalmemory = GetFairyLandGlobal( ).mMemory;

	if ( globalmemory == _null )
		return;

	if ( Trace::GetTraceTarget( ) == Trace::_TARGET_TRACEFILE )
	{
		_char procname[1024];
		::GetModuleFileName( _null, procname, 1024 );
		_char pathname[1024];
		StringFormatter::GetPathName( pathname, 1024, procname );
		_char filename[1024];
		StringFormatter::GetFileName( filename, 1024, procname, _false );

		Time curtime = System::GetCurrentLocalTime( );

		// Build log file name from the current time and exe file name.
		_char logfilename[1024];
		StringFormatter::FormatBuffer( logfilename, 1024, L"%s\\Log\\[%.4d-%.2d-%.2d %.2d-%.2d-%.2d(%.4d)] %s [MEM].log",
			(const _char*) pathname, curtime.mYear, curtime.mMonth, curtime.mDay, curtime.mHour, curtime.mMinute, curtime.mSecond, curtime.mMilliseconds, (const _char*) filename );

		// Create log file to dump memory leak.
		File file;
		if ( file.Open( logfilename, File::_CREATE_ALWAYS, File::_OPERATION_WRITE ) )
		{
			// Write unicode flag.
			file.WriteUnicodeFlag( );

			Trace::SetTraceTarget( Trace::_TARGET_TRACEFILE, (_handle) file );
		}
	}

	// No memory leak.
	if ( globalmemory->mCurAllocTimes == 0 )
	{
		PS_TRACE( L"[Memory] Congratulations! No Memory Leaks Found!\r\n" );
	}
	else
	{
		PS_TRACE( L"[Memory] Caution! Memory Leaks Found!\r\n\r\n" );

		_dword totalsize = 0;

		// Dump each memory leak.
		for ( Iterator memit = globalmemory->GetHeadIterator( ); memit.Valid( ); memit ++ )
		{
			const MemoryAllocation& allocation = memit;

			totalsize += allocation.mSize;

			PS_TRACE( L"================================================================================\r\n" );
			PS_TRACE2( L"Memory Leak : 0x%p\t%u\t\r\n", allocation.mAddress, allocation.mSize );

			// Dump call stack.
			for ( CallStack::Iterator stackit = allocation.mCallStack.GetHeadIterator( ); stackit.Valid( ); stackit ++ )
			{
				const CallStackFrame& stackframe = stackit;

				if ( StringPtr( L"operator new" ) == stackframe.mFuncName )
					continue;

				if ( StringPtr( L"operator new[]" ) == stackframe.mFuncName )
					continue;

				if ( StringPtr( L"FL::Memory::OperatorNew" ) == stackframe.mFuncName )
					continue;

				if ( stackframe.mFuncName[0] != 0 )
					PS_TRACE3( L"    %s => %s:%u\r\n", stackframe.mFuncName, stackframe.mFileName, stackframe.mLineNumber )
				else
					PS_TRACE2( L"    %s:%u\r\n", stackframe.mFileName, stackframe.mLineNumber )
			}

			_char buffer[1024]; buffer[0] = 0;
			_char tempbuffer[1024];

			for ( _dword i = 0; i < allocation.mSize && i < 32; i ++ )
			{
				_char data = ( (_byte*) allocation.mAddress )[i];

				if ( data == '\r' || data == '\n' || data == '\t' || data == '\b' )
					data = '?';

				StringFormatter::FormatBuffer( tempbuffer, 1024, L"%c", data );

				if ( tempbuffer[0] == 0 )
				{
					tempbuffer[0] = '.';
					tempbuffer[1] = 0;
				}

				StringFormatter::AppendString( buffer, tempbuffer );
			}

			PS_TRACE1( L"    %s\r\n", buffer );
		}

		// Dump the total memory leak size.
		PS_TRACE2( L"\r\nTotal %u Bytes(s) in %u Time(s)\r\n", totalsize, globalmemory->mCurAllocTimes );

		globalmemory->Clear( );
	}

	GetFairyLandGlobal( ).mMemory = _null;

	globalmemory->mMemoryLock.Finalize( );

	// Free global memory system.
	HeapFree( globalmemory );
}

_void Memory::SetDebugMode( _dword debugmode )
{
	Memory* globalmemory = GetFairyLandGlobal( ).mMemory;

	if ( globalmemory != _null )
		globalmemory->mDebugMode = debugmode;
}

_dword Memory::GetDebugMode( )
{
	Memory* globalmemory = GetFairyLandGlobal( ).mMemory;

	if ( globalmemory != _null )
		return globalmemory->mDebugMode;

	return 0;
}

_void Memory::SetAllocater( _void* allocater )
{
	sAllocater = allocater;
}

_void* Memory::GetAllocater( )
{
	return sAllocater;
}

_void Memory::MemSet( _void* desbuffer, _byte value, _dword length )
{
	//mmx_memset( desbuffer, value, (_long) length );

	::memset( desbuffer, value, length );
}

_bool Memory::MemCmp( const _void* buffer1, const _void* buffer2, _dword length )
{
	return ::memcmp( buffer1, buffer2, length ) == 0;
}

_void Memory::MemCpy( _void* desbuffer, const _void* srcbuffer, _dword length )
{
	mmx_memcpy( desbuffer, (_void*) srcbuffer, (_long) length );

	//::memcpy( desbuffer, srcbuffer, length );
}

_void Memory::MemMove( _void* desbuffer, const _void* srcbuffer, _dword length )
{
	::memmove( desbuffer, srcbuffer, length );
}

_void* Memory::HeapAlloc( _dword size )
{
	FL_ASSERT( size > 0 )

	_void* pointer = _null;

	// Alloc from allocater.
	if ( sAllocater != _null )
	{
		pointer = ( (Allocater*) sAllocater )->Allocate( size );
	}
	// Alloc from OS by win32 api.
	else
	{
		pointer = ::HeapAlloc( ::GetProcessHeap( ), 0, size );
	}

	if ( pointer == _null )
		PS_ERROR1( L"Out of memory, using %d bytes\r\n", System::GetCurrentMemoryUsage( ) )

	FL_ASSERT( pointer != _null )

	return pointer;
}

_void Memory::HeapFree( _void* pointer )
{
	if ( pointer == _null )
		return;

	// Free to allocater.
	if ( sAllocater != _null )
	{
		( (Allocater*) sAllocater )->Free( pointer );
	}
	// Free to OS.
	else
	{
		::HeapFree( ::GetProcessHeap( ), 0, pointer );
	}
}

_void* Memory::OperatorNew( _dword size )
{
	FL_ASSERT( size > 0 )

	Memory* globalmemory = GetFairyLandGlobal( ).mMemory;

	if ( globalmemory != _null )
	{
		LockOwner lockowner( globalmemory->mMemoryLock );

		globalmemory->mHappenAllocTimes ++;

		_dword deltasize = 0;

		// Create additional space to store debug information.
		if ( globalmemory->mDebugMode >= _TRACE_LEAK )
			deltasize = sizeof( Node );

		// Allocate from heap.
		_void* object = HeapAlloc( deltasize + size );

		if ( globalmemory->mDebugMode != _TRACE_NONE )
		{
			globalmemory->mCurAllocTimes ++;

			if ( globalmemory->mMaxAllocTimes < globalmemory->mCurAllocTimes )
				globalmemory->mMaxAllocTimes = globalmemory->mCurAllocTimes;

			// Save current debug mode.
			_dword debugmode = globalmemory->mDebugMode;

			// Clear debug mode for now.
			globalmemory->mDebugMode = 0;

			// Save allocation information into link.
			if ( debugmode >= _TRACE_LEAK )
			{
				globalmemory->mCurAllocSize += size;
				if ( globalmemory->mMaxAllocSize < globalmemory->mCurAllocSize )
					globalmemory->mMaxAllocSize = globalmemory->mCurAllocSize;

				// Create memory allocation record.
				Node* node = (Node*) object;
				node->mElement.mAddress	= (_byte*) object + deltasize;
				node->mElement.mSize	= size;
				node->mPrev				= _null;
				node->mNext				= _null;

				if ( debugmode == _TRACE_STACK )
					node->mElement.mCallStack = CallStack::GetCurrentCallStack( );
				else
					node->mElement.mCallStack = CallStack::CreateCallStack( L"", sOwnerFileName, sOwnerLineNumber );

				globalmemory->InsertTail( node );
			}

			// Set debug mode back.
			globalmemory->mDebugMode = debugmode;
		}

		return (_byte*) object + deltasize;
	}
	else
	{
		// Allocate from heap.
		return HeapAlloc( size );
	}
}

_void Memory::OperatorDelete( _void* pointer )
{
	if ( pointer == _null )
		return;

	Memory* globalmemory = GetFairyLandGlobal( ).mMemory;

	if ( globalmemory != _null && globalmemory->mDebugMode != _TRACE_NONE )
	{
		LockOwner lockowner( globalmemory->mMemoryLock );

		if ( globalmemory->mCurAllocTimes > 0 )
			globalmemory->mCurAllocTimes --;

		// Save current debug mode.
		_dword debugmode = globalmemory->mDebugMode;

		// Clear debug mode for now.
		globalmemory->mDebugMode = 0;

		if ( debugmode >= _TRACE_LEAK )
		{
			pointer = (_byte*) pointer - sizeof( Node );

			Node* node = (Node*) pointer;

			if ( globalmemory->mCurAllocSize >= node->mElement.mSize )
				globalmemory->mCurAllocSize -= node->mElement.mSize;

			node->mElement.mCallStack.Release( );

			globalmemory->Remove( node );
		}

		// Set debug mode back.
		globalmemory->mDebugMode = debugmode;
	}

	// Free to heap.
	HeapFree( pointer );
}

_dword Memory::GetCurAllocTimes( )
{
	Memory* globalmemory = GetFairyLandGlobal( ).mMemory;

	if ( globalmemory == _null )
		return 0;

	return globalmemory->mCurAllocTimes;
}

_dword Memory::GetCurAllocSize( )
{
	Memory* globalmemory = GetFairyLandGlobal( ).mMemory;

	if ( globalmemory == _null )
		return 0;

	return globalmemory->mCurAllocSize;
}

_dword Memory::GetMaxAllocTimes( )
{
	Memory* globalmemory = GetFairyLandGlobal( ).mMemory;

	if ( globalmemory == _null )
		return 0;

	return globalmemory->mMaxAllocTimes;
}

_dword Memory::GetMaxAllocSize( )
{
	Memory* globalmemory = GetFairyLandGlobal( ).mMemory;

	if ( globalmemory == _null )
		return 0;

	return globalmemory->mMaxAllocSize;
}

_void* Memory::CreateReferencedBuffer( _dword size )
{
	_byte* realbuffer = new _byte[ size + 8 ];

	( (_dword*) realbuffer )[0] = 0x4642544E;
	( (_dword*) realbuffer )[1] = 1;

	return realbuffer + 8;
}

_void Memory::ReserveReferencedBuffer( _void* buffer )
{
	_byte* realbuffer = (_byte*) buffer - 8;

	FL_ASSERT( ( (_dword*) realbuffer )[0] == 0x4642544E )
	FL_ASSERT( ( (_dword*) realbuffer )[1] >= 1 )

	( (_dword*) realbuffer )[1] ++;
}

_bool Memory::ReleaseReferencedBuffer( _void* buffer )
{
	_byte* realbuffer = (_byte*) buffer - 8;

	FL_ASSERT( ( (_dword*) realbuffer )[0] == 0x4642544E )
	FL_ASSERT( ( (_dword*) realbuffer )[1] >= 1 )

	( (_dword*) realbuffer )[1] --;

	if ( ( (_dword*) realbuffer )[1] != 0 )
		return _false;

	delete[] realbuffer;
	return _true;
}