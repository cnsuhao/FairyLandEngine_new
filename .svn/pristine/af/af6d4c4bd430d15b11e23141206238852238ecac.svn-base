//============================================================================
// Lock.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Lock Implementation
//----------------------------------------------------------------------------

Lock::Lock( ) : mEnable( _true ), mEnterNumber( 0 )
{
	::InitializeCriticalSection( (CRITICAL_SECTION*) this );
}

Lock::~Lock( )
{
	::DeleteCriticalSection( (CRITICAL_SECTION*) this );
}

_void Lock::Initialize( )
{
	::InitializeCriticalSection( (CRITICAL_SECTION*) this );
}

_void Lock::Finalize( )
{
	::InitializeCriticalSection( (CRITICAL_SECTION*) this );
}

_void Lock::Enable( _bool enable )
{
	mEnable = enable;
}

_void Lock::Enter( ) const
{
	if ( mEnable == _false )
		return;

	::EnterCriticalSection( (CRITICAL_SECTION*) this );

	_dword* number = (_dword*) ( (_byte*) this + sizeof( Lock ) - sizeof( _dword ) );

	*number = *number + 1;
}

_void Lock::Leave( ) const
{
	if ( mEnable == _false )
		return;

	_dword* number = (_dword*) ( (_byte*) this + sizeof( Lock ) - sizeof( _dword ) );

	if ( *number > 0 )
		*number = *number - 1;

	::LeaveCriticalSection( (CRITICAL_SECTION*) this );
}

_bool Lock::TryEnter( _dword testtime ) const
{
	if ( mEnable == _false )
		return _true;

	_dword* number = (_dword*) ( (_byte*) this + sizeof( Lock ) - sizeof( _dword ) );

	if ( testtime == 0 )
	{
		if ( ::TryEnterCriticalSection( (CRITICAL_SECTION*) this ) )
		{
			*number = *number + 1;
			return _true;
		}
		else
		{
			return _false;
		}
	}

	_dword tickcount = System::GetCurrentTickcount( );

	while ( tickcount + testtime > System::GetCurrentTickcount( ) )
	{
		if ( ::TryEnterCriticalSection( (CRITICAL_SECTION*) this ) )
		{
			*number = *number + 1;
			return _true;
		}

		System::Sleep( 0 );
	}

	return _false;
}

_void* Lock::SetPointer( _void** pointer, _void* value )
{
	return (_void*) ::InterlockedExchange( (LONG volatile*) pointer, (_long) value );
}

_long Lock::SetValue( _long* value, _long target )
{
	return ::InterlockedExchange( (LONG volatile*) value, target );
}

_long Lock::AddValue( _long* value, _long increment )
{
	return ::InterlockedExchangeAdd( (LONG volatile*) value, increment );
}

_long Lock::IncValue( _long* value )
{
	return ::InterlockedIncrement( (LONG volatile*) value );
}

_long Lock::DecValue( _long* value )
{
	return ::InterlockedDecrement( (LONG volatile*) value );
}

//----------------------------------------------------------------------------
// LockOwner Implementation
//----------------------------------------------------------------------------

LockOwner::LockOwner( const Lock& lock ) : mLock( lock )
{
	mLock.Enter( );
}

LockOwner::~LockOwner( )
{
	mLock.Leave( );
}