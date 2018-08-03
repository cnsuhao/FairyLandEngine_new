//============================================================================
// SyncObject.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// SyncObject Implementation
//----------------------------------------------------------------------------

_bool SyncObject::Wait( _dword milliseconds )
{
	return ::WaitForSingleObject( mObjectHandle, milliseconds ) == WAIT_OBJECT_0;
}

//----------------------------------------------------------------------------
// Event Implementation
//----------------------------------------------------------------------------

_bool Event::Create( _bool manualreset, _bool initialstate, StringPtr objectname )
{
	Close( );

	mObjectHandle = ::CreateEvent( _null, manualreset, initialstate, objectname == L"" ? _null : objectname );

	return mObjectHandle != _null;
}

_bool Event::Open( StringPtr objectname )
{
	Close( );

	mObjectHandle = ::OpenEvent( EVENT_ALL_ACCESS, 0, objectname );

	return mObjectHandle != _null;
}

_bool Event::IsSignaled( )
{
	return ::WaitForSingleObject( mObjectHandle, 0 ) == WAIT_OBJECT_0;
}

_bool Event::Set( )
{
	return ::SetEvent( mObjectHandle );
}

_bool Event::Reset( )
{
	return ::ResetEvent( mObjectHandle );
}

_bool Event::Pulse( )
{
	return ::PulseEvent( mObjectHandle );
}

//----------------------------------------------------------------------------
// Mutex Implementation
//----------------------------------------------------------------------------

_bool Mutex::Create( _bool initialowner, StringPtr objectname )
{
	Close( );

	mObjectHandle = ::CreateMutex( _null, initialowner, objectname == L"" ? _null : objectname );

	return mObjectHandle != _null;
}

_bool Mutex::Open( StringPtr objectname )
{
	Close( );

	mObjectHandle = ::OpenMutex( MUTEX_ALL_ACCESS, 0, objectname );

	return mObjectHandle != _null;
}

_bool Mutex::Release( )
{
	return ::ReleaseMutex( mObjectHandle );
}

//----------------------------------------------------------------------------
// Semaphore Implementation
//----------------------------------------------------------------------------

_bool Semaphore::Create( _dword initialcount, _dword maximumcount, StringPtr objectname )
{
	Close( );

	mObjectHandle = ::CreateSemaphore( _null, initialcount, maximumcount, objectname == L"" ? _null : objectname );

	return mObjectHandle != _null;
}

_bool Semaphore::Open( StringPtr objectname )
{
	Close( );

	mObjectHandle = ::OpenSemaphore( SEMAPHORE_ALL_ACCESS, 0, objectname );

	return mObjectHandle != _null;
}

_bool Semaphore::Release( _dword releasecount )
{
	return ::ReleaseSemaphore( mObjectHandle, releasecount, _null );
}

//----------------------------------------------------------------------------
// Timer Implementation
//----------------------------------------------------------------------------

_bool Timer::Create( _bool manualreset, StringPtr objectname )
{
	Close( );

	mObjectHandle = ::CreateWaitableTimer( _null, manualreset, objectname == L"" ? _null : objectname );

	return mObjectHandle != _null;
}

_bool Timer::Open( StringPtr objectname )
{
	Close( );

	mObjectHandle = ::OpenWaitableTimer( TIMER_ALL_ACCESS, 0, objectname );

	return mObjectHandle != _null;
}

_bool Timer::SetInterval( _dword interval, _dword period )
{
	LARGE_INTEGER nsinterval;

	nsinterval.QuadPart = interval;
	// Convert ms to 100 ns
	nsinterval.QuadPart = 1000 * nsinterval.QuadPart;
	// Negative values indicate relative time.
	nsinterval.QuadPart = - nsinterval.QuadPart;

	return ::SetWaitableTimer( mObjectHandle, &nsinterval, period, _null, _null, 0 );
}

_bool Timer::Cancel( )
{
	return ::CancelWaitableTimer( mObjectHandle );
}