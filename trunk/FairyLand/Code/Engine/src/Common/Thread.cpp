//============================================================================
// Thread.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Thread Implementation
//----------------------------------------------------------------------------

Thread::Thread( )
{
	mInterval			= 100;
	mThreadID			= 0;
	mSelfDelete			= _false;

	mCloseThreadFunc	= _null;
	mCloseParameter1	= _null;
	mCloseParameter2	= _null;

	mFairyLandTls		= &GetFairyLandGlobal( );
}

Thread::~Thread( )
{
	Close( );
}

_long Thread::ThreadProc( _void* parameter )
{
	Thread* thread = (Thread*) parameter;

	SetFairyLandGlobal( thread->mFairyLandTls );

	thread->mReadyEvent.Set( );

	while ( _true )
	{
		thread->mThreadLock.Enter( );
		_dword functionnumber = thread->mUpdateFuncQueue.Number( );
		thread->mThreadLock.Leave( );

		// Call all registered update function.
		for ( _dword i = 0; i < functionnumber; i ++ )
		{
			UpdateFuncStruct updatefunc;

			thread->mThreadLock.Enter( );

			_bool breakprocess = thread->mUpdateFuncQueue.Number( ) == 0;

			// Get the function to process.
			if ( breakprocess == _false )
			{
				updatefunc = thread->mUpdateFuncQueue.Front( );
				thread->mUpdateFuncQueue.Remove( );
			}

			thread->mThreadLock.Leave( );

			// There is no need to process.
			if ( breakprocess )
				break;

			_bool funcresult = _false;

			// Call the function by pointer and parameters.
			if ( updatefunc.mUpdateThreadFunc != _null )
				funcresult = (*updatefunc.mUpdateThreadFunc)( updatefunc.mUpdateParameter1, updatefunc.mUpdateParameter2 );

			// Function returns true, means we need call it again next time.
			if ( funcresult )
			{
				thread->mThreadLock.Enter( );
				thread->mUpdateFuncQueue.Append( updatefunc );
				thread->mThreadLock.Leave( );
			}

			if ( thread->mInterval == -1 || thread->mSelfDelete )
				break;
		}

		if ( thread->mInterval == -1 || thread->mSelfDelete )
			break;

		_dword sleeptime = thread->mInterval;

		if ( thread->mUpdateFuncQueue.Number( ) == 0 )
			sleeptime *= 10;

		// Use SleepEx to make sure can be woken up.
		::SleepEx( sleeptime, _true );
	}

	OnCloseThread closefunc = thread->mCloseThreadFunc;
	_void* parameter1 = thread->mCloseParameter1;
	_void* parameter2 = thread->mCloseParameter2;
	_bool  selfdelete = thread->mSelfDelete;

	if ( closefunc != _null )
		(*closefunc)( parameter1, parameter2 );

	thread->mFinishedEvent.Set( );

	if ( selfdelete )
		delete thread;

	return 0;
}

_void Thread::APCProc( _void* parameter )
{
	APCCallStruct* apccall = (APCCallStruct*) parameter;
	if ( parameter == _null )
		return;

	FL_ASSERT( apccall->mUpdateThreadFunc != _null )
	FL_ASSERT( apccall->mFinishedEvent != _null )

	// Call the function.
	(*apccall->mUpdateThreadFunc)( apccall->mUpdateParameter1, apccall->mUpdateParameter2 );

	// Set the event, tell the caller function it finished.
	apccall->mFinishedEvent->Set( );
}

_bool Thread::CallFunctionUsingBackgroundThread( OnUpdateThread updatefuncpointer, OnCloseThread closefuncpointer, _void* parameter1, _void* parameter2 )
{
	Thread* thread = new Thread;

	thread->EnableSelfDelete( _true );
	thread->RegisterUpdateFunc( updatefuncpointer, parameter1, parameter2 );
	thread->RegisterCloseFunc( closefuncpointer, parameter1, parameter2 );
	
	if ( thread->Create( 100, _true ) == _false )
	{
		delete thread;
		return _false;
	}

	if ( thread->Resume( ) == _false )
	{
		delete thread;
		return _false;
	}

	return _true;
}

_bool Thread::Create( _dword interval, _bool suspend )
{
	// Make sure dont create multiple threads.
	if ( mObjectHandle != _null )
		return _false;

	// Create the thread ready events.
	if ( mReadyEvent.Create( _false, _false ) == _false )
		{ Close( ); return _false; }

	// Create the thread finished events.
	if ( mFinishedEvent.Create( _true, _false ) == _false )
		{ Close( ); return _false; }

	// Create the thread.
	mObjectHandle = ::CreateThread( _null, 0, (LPTHREAD_START_ROUTINE) ThreadProc, this, suspend ? CREATE_SUSPENDED : 0, &mThreadID );

	if ( mObjectHandle == _null )
		{ Close( ); return _false; }

	mInterval = interval;

	// Wait for thread to finish making itself.
	if ( suspend == _false )
		mReadyEvent.Wait( );

	PS_TRACE1( L"[KL] Create Thread %d\r\n", mThreadID )

	return _true;
}

_void Thread::Close( )
{
	if ( mObjectHandle == _null || mThreadID == 0 )
		return;

	PS_TRACE1( L"[KL] Close Thread %d\r\n", mThreadID )

	mThreadID = 0;
	mInterval = -1;

	// Wait for thread to finish exiting itself.
	mFinishedEvent.Wait( );

	mThreadLock.Enter( );
	mUpdateFuncQueue.Clear( );
	mThreadLock.Leave( );

	mReadyEvent.Close( );
	mFinishedEvent.Close( );

	KernelObject::Close( );
}

_bool Thread::Suspend( )
{
	return ::SuspendThread( mObjectHandle ) != -1;
}

_bool Thread::Resume( )
{
	return ::ResumeThread( mObjectHandle ) != -1;
}

_bool Thread::Wakeup( )
{
	return ::QueueUserAPC( (PAPCFUNC) APCProc, mObjectHandle, _null ) != 0;
}

_bool Thread::CallFunction( OnUpdateThread funcpointer, _void* parameter1, _void* parameter2 )
{
	// If already in the current thread, just call it directly.
	if ( ::GetCurrentThreadId( ) == mThreadID )
	{
		(*funcpointer)( parameter1, parameter2 );

		return _true;
	}

	// Set up the call struct.
	Event finishevent;

	if ( finishevent.Create( _false, _false ) == _false )
		return _false;

	APCCallStruct apccall;
	apccall.mUpdateThreadFunc = funcpointer;
	apccall.mUpdateParameter1 = parameter1;
	apccall.mUpdateParameter2 = parameter2;
	apccall.mFinishedEvent	  = &finishevent;

	// Call the function with APC
	if ( ::QueueUserAPC( (PAPCFUNC) APCProc, mObjectHandle, (_dword) &apccall ) == 0 )
		return _false;

	// Wait for completion.
	finishevent.Wait( );

	return _true;
}

_void Thread::SetInterval( _dword interval )
{
	mInterval = interval;
}

_void Thread::WaitForThreadEnd( )
{
	mFinishedEvent.Wait( );
}

_void Thread::EnableSelfDelete( _bool enable )
{
	mSelfDelete = enable;
}

_bool Thread::SetPriority( _dword priority )
{
	if ( priority == _THREAD_PRIORITY_HIGH )
		priority = THREAD_PRIORITY_ABOVE_NORMAL;
	else if ( priority == _THREAD_PRIORITY_NORMAL )
		priority = THREAD_PRIORITY_NORMAL;
	else if ( priority == _THREAD_PRIORITY_LOW )
		priority = THREAD_PRIORITY_BELOW_NORMAL;
	else
		return _false;

	return ::SetThreadPriority( mObjectHandle, priority );
}

_bool Thread::IsThreadBusy( ) const
{
	LockOwner lockowner( mThreadLock );

	return mUpdateFuncQueue.Number( ) > 0;
}

_void Thread::RegisterUpdateFunc( OnUpdateThread funcpointer, _void* parameter1, _void* parameter2 )
{
	LockOwner lockowner( mThreadLock );

	UpdateFuncStruct updatefunc;
	updatefunc.mUpdateThreadFunc = funcpointer;
	updatefunc.mUpdateParameter1 = parameter1;
	updatefunc.mUpdateParameter2 = parameter2;

	mUpdateFuncQueue.Append( updatefunc );

	Wakeup( );
}

_void Thread::RegisterCloseFunc( OnCloseThread funcpointer, _void* parameter1, _void* parameter2 )
{
	LockOwner lockowner( mThreadLock );

	mCloseThreadFunc = funcpointer;
	mCloseParameter1 = parameter1;
	mCloseParameter2 = parameter2;
}

_void Thread::ClearAllFunc( )
{
	LockOwner lockowner( mThreadLock );

	mUpdateFuncQueue.Clear( );

	mCloseThreadFunc = _null;
	mCloseParameter1 = _null;
	mCloseParameter2 = _null;
}