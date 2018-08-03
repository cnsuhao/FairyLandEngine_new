//============================================================================
// Lock.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Lock
//----------------------------------------------------------------------------

//! This class represents a critical section object, used for threads synchronization in a process.
class Lock
{
private:
	//! Reserved data used by win32 api.
	_byte	mReserved[24];
	_bool	mEnable;
	_dword	mEnterNumber;

public:
	//! Constructor, create a critical section and initialize it.
	//! @param		none
	Lock( );
	//! Destructor, delete the critical section.
	//! @param		none
	~Lock( );

	//! Initialize critical section, only used if constructor not called.
	//! @param		none
	_void Initialize( );

	//! Initialize critical section, only used if destructor not called.
	//! @param		none
	_void Finalize( );

	//! Enable or disable this lock.
	//! @param		enable		True to enable lock, false to disable.
	_void Enable( _bool enable );

	//! Waits for ownership of the critical section, returns when the calling thread is granted ownership.
	//! @param		none
	_void Enter( ) const;
	//! Releases ownership of the critical section.
	//! @param		none
	_void Leave( ) const;
	//! Attempts to enter the critical section without blocking.
	//! @param		testtime	How long we can try, in millisecond.
	//! @return		True if the calling thread takes ownership of the critical section, false otherwise.
	_bool TryEnter( _dword testtime = 0 ) const;

	static _void* SetPointer( _void** pointer, _void* value );
	static _long  SetValue( _long* value, _long target );
	static _long  AddValue( _long* value, _long increment );
	static _long  IncValue( _long* value );
	static _long  DecValue( _long* value );
};

//----------------------------------------------------------------------------
// LockOwner
//----------------------------------------------------------------------------

//! This class maintain the ownership of a critical section object, it will obtain & release ownership in constructor & destructor.
class LockOwner
{
private:
	//! The critical section object that maintains.
	const Lock&	mLock;

public:
	//! Constructor, obtain ownership of the critical section.
	//! @param		lock		The lock to be owned.
	LockOwner( const Lock& lock );
	//! Destructor, release ownership of the critical section.
	//! @param		none
	~LockOwner( );
};

};