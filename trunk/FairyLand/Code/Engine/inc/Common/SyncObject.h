//============================================================================
// SyncObject.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// SyncObject
//----------------------------------------------------------------------------

//! This class is base class of all synchronization objects.
class SyncObject : public KernelObject
{
public:
	//! Wait for the object when it is signaled, and obtains the ownership.
	//! @param		milliseconds	Time-out interval in milliseconds, -1 indicates wait forever.
	//! @return		True if the object is signaled before time out, false otherwise.
	_bool Wait( _dword milliseconds = -1 );
};

//----------------------------------------------------------------------------
// Event
//----------------------------------------------------------------------------

//! This class represents a event object, used for threads synchronization in system.

//! An event object is a synchronization object whose state can be explicitly set to signaled
//! by use of the SetEvent function. Following are the two types of event object.
class Event : public SyncObject
{
public:
	//! Create a event object with or without a name, if it is alread existed then copy the object handle.
	//! @param		manualreset		Whether the event object is manual-reset or auto-reset.
	//! @param		initialstate	The initial state of the event object, true means signaled.
	//! @param		objectname		The name of event object, can be null indicates no name needed.
	//! @return		True indicates success, false indicates failure.
	_bool Create( _bool manualreset, _bool initialstate, StringPtr objectname = L"" );
	//! Open an existing named event object.
	//! @param		objectname		The name of event object, case-sensitive.
	//! @return		True indicates success, false indicates failure.
	_bool Open( StringPtr objectname );

	//! Test if the event object is signaled.
	//! @param		none
	//! @return		True indicates signaled, false indicates not signaled.
	_bool IsSignaled( );

	//! Set the state of the event object to signaled.
	//! @param		none
	//! @return		True indicates success, false indicates failure.
	_bool Set( );
	//! Set the state of the event object to nonsignaled.
	//! @param		none
	//! @return		True indicates success, false indicates failure.
	_bool Reset( );
	//! Sets the event to signaled then resets, can wake up one waiting thread.
	//! @param		none
	//! @return		True indicates success, false indicates failure.
	_bool Pulse( );
};

//----------------------------------------------------------------------------
// Mutex
//----------------------------------------------------------------------------

//! This class represents a mutex object, used for threads synchronization in system.

//! A mutex object is a synchronization object whose state is set to signaled when it is not
//! owned by any thread, and nonsignaled when it is owned.
class Mutex : public SyncObject
{
public:
	//! Create a mutex object with or without a name, if it is alread existed then copy the object handle.
	//! @param		initialowner	Whether the calling thread obtains the initial ownership of the mutex.
	//! @param		objectname		The name of mutex object, can be null indicates no name needed.
	//! @return		True indicates success, false indicates failure.
	_bool Create( _bool initialowner, StringPtr objectname = L"" );
	//! Open an existing named mutex object.
	//! @param		objectname		The name of mutex object, case-sensitive.
	//! @return		True indicates success, false indicates failure.
	_bool Open( StringPtr objectname );

	//! Releases ownership of themutex object, then it can be obtained by other threads.
	//! @param		none
	//! @return		True indicates success, false indicates failure.
	_bool Release( );
};

//----------------------------------------------------------------------------
// Semaphore
//----------------------------------------------------------------------------

//! This class represents a semaphore object, used for threads synchronization in system.

//! A semaphore object is a synchronization object that maintains a count between zero and a
//! specified maximum value. The count is decremented each time a thread completes a wait for
//! the semaphore object and incremented each time a thread releases the semaphore. When the
//! count reaches zero, no more threads can successfully wait for the semaphore object state
//! to become signaled. The state of a semaphore is set to signaled when its count is greater
//! than zero, and nonsignaled when its count is zero.
class Semaphore : public SyncObject
{
public:
	//! Create a semaphore object with or without a name, if it is alread existed then copy the object handle.
	//! @param		initialcount	The initial count for the semaphore object
	//! @param		maximumcount	The maximum count for the semaphore object.
	//! @param		objectname		The name of semaphore object, can be null indicates no name needed.
	//! @return		True indicates success, false indicates failure.
	_long Create( _dword initialcount, _dword maximumcount, StringPtr objectname = L"" );
	//! Open an existing named semaphore object.
	//! @param		objectname		The name of semaphore object, case-sensitive.
	//! @return		True indicates success, false indicates failure.
	_bool Open( StringPtr objectname );

	//! Increases usable count for the semaphore object, then it can be obtained by other threads.
	//! @param		releasecount	Amount by which the semaphore object's current count is to be increased.
	//! @return		True indicates success, false indicates failure.
	_long Release( _dword releasecount = 1 );
};

//----------------------------------------------------------------------------
// Timer
//----------------------------------------------------------------------------

//! This class represents a timer object, used for threads synchronization in system.

//! A timer object is a synchronization object whose state is set to signaled when the specified
//! due time arrives. There are two types of timers that can be created: manual-reset and synchronization.
//! A timer of either type can also be a periodic timer.
class Timer : public SyncObject
{
public:
	//! Create a timer object with or without a name, if it is alread existed then copy the object handle.
	//! @param		manualreset		Whether the timer object is manual-reset or synchronization.
	//! @param		objectname		The name of timer object, can be null indicates no name needed.
	//! @return		True indicates success, false indicates failure.
	_bool Create( _bool manualreset, StringPtr objectname = L"" );
	//! Open an existing named timer object.
	//! @param		objectname		The name of timer object, case-sensitive.
	//! @return		True indicates success, false indicates failure.
	_bool Open( StringPtr objectname );

	//! Activates the timer, sets the interval and period time of it.
	//! @param		interval		The time-out interval of the timer in milliseconds.
	//!	@param		period			The period of the timer, in milliseconds.
	//! @return		True indicates success, false indicates failure.
	//! @remarks	If period parameter is zero, the timer is signaled once, otherwise it is periodic.
	//!				A periodic timer automatically reactivates each time the period elapses, until the timer
	//!				is canceled using the Cancel function.
	_bool SetInterval( _dword interval, _dword period );
	//! Sets the timer to inactive state.
	//! @param		none
	//! @return		True indicates success, false indicates failure.
	_bool Cancel( );
};

};