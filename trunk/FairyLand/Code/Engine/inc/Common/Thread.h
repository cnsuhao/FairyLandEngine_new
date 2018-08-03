//============================================================================
// Thread.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Thread
//----------------------------------------------------------------------------

//! This class represents a thread in a process.
class Thread : public KernelObject
{
public:
	enum _THREAD_PRIORITY
	{
		_THREAD_PRIORITY_LOW	= 1,
		_THREAD_PRIORITY_NORMAL	= 2,
		_THREAD_PRIORITY_HIGH	= 3,
	};

	//! Define thread update callback function prototype.
	//! @param		parameter1		User defined parameter.
	//! @param		parameter2		User defined parameter.
	//! @return		True indicates success, false indicates this function is useless, must be remove from update functions list.
	typedef _bool (*OnUpdateThread)( _void* parameter1, _void* parameter2 );
	//! Define thread close callback function prototype.
	//! @param		parameter1		User defined parameter.
	//! @param		parameter2		User defined parameter.
	typedef _void (*OnCloseThread)( _void* parameter1, _void* parameter2 );

private:
	//! Update function structure.
	struct UpdateFuncStruct
	{
		//! Update function pointer.
		OnUpdateThread	mUpdateThreadFunc;
		//! Update function parameter1.
		_void*			mUpdateParameter1;
		//! Update function parameter2.
		_void*			mUpdateParameter2;

		UpdateFuncStruct( )
			: mUpdateThreadFunc( _null ), mUpdateParameter1( _null ), mUpdateParameter2( _null ) { }
	};

	//! APC function call structure.
	struct APCCallStruct
	{
		//! Update function pointer.
		OnUpdateThread	mUpdateThreadFunc;
		//! Update function parameter1.
		_void*			mUpdateParameter1;
		//! Update function parameter2.
		_void*			mUpdateParameter2;
		//! Finished event.
		Event*			mFinishedEvent;

		APCCallStruct( )
			: mUpdateThreadFunc( _null ), mUpdateParameter1( _null ), mUpdateParameter2( _null ), mFinishedEvent( _null ) { }
	};

	//! Define update function array.
	typedef Queue< UpdateFuncStruct > UpdateFuncQueue;

	//! Update interval
	_dword				mInterval;
	//! Thread ID
	_dword				mThreadID;

	//! Wether self delete the thread object after close callback function called.
	_bool				mSelfDelete;

	//! The critical section used internal.
	Lock				mThreadLock;
	//! Thread ready event
	Event				mReadyEvent;
	//! Thread finished event.
	Event				mFinishedEvent;

	//! Update functions list.
	UpdateFuncQueue		mUpdateFuncQueue;

	//! Close function pointer.
	OnCloseThread		mCloseThreadFunc;
	//! Close function parameter1.
	_void*				mCloseParameter1;
	//! Close function parameter2.
	_void*				mCloseParameter2;

	//! FairyLand Tls
	_void*				mFairyLandTls;

	//! The thread work function, called by win32 api.
	//! @param		parameter		User defined parameter.
	//! @return		The exit code of the thread.
	static _long __stdcall ThreadProc( _void* parameter );
	//! The APC function, used to execute a function in the thread or wake a sleeping thread up.
	//! @param		parameter		User defined parameter.
	static _void __stdcall APCProc( _void* parameter );

public:
	//! Constructor, create a empty thread, that is not running.
	//! @param		none
	Thread( );
	//! Destructor, close thread and delete its kernel object.
	//! @param		none
	~Thread( );

	//! Execute a function in a background thread.
	//! @param		updatefuncpointer	Update function pointer.
	//! @param		closefuncpointer	Close function pointer.
	//! @param		parameter1			User defined parameter.
	//! @param		parameter2			User defined parameter.
	static _bool CallFunctionUsingBackgroundThread( OnUpdateThread updatefuncpointer, OnCloseThread closefuncpointer, _void* parameter1, _void* parameter2 );

	//! Get thread ID.
	//! @param		none
	//! @return		The thread ID.
	inline _dword GetThreadID( ) const;

	//! Creates a thread to execute update functions.
	//! @param		interval		The sleep interval of thread in milliseconds
	//! @param		suspend			If it is true, thread is created in a suspended state and does not run until Resume is called.
	//! @return		True indicates success, false indicates failure.
	_bool Create( _dword interval, _bool suspend = _false );
	//! Close the thread in a safe way.
	//! @param		none
	_void Close( );
	//! Suspend the thread, ( suspend count is incremented ).
	//! @param		none
	//! @return		True indicates success, false indicates failure.
	_bool Suspend( );
	//! Decrements suspend count. When it is decremented to zero, the thread is resumed.
	//! @param		none
	//! @return		True indicates success, false indicates failure.
	_bool Resume( );
	//! Wake up the thread if it is sleeping.
	//! @param		none
	//! @return		True indicates success, false indicates failure.
	_bool Wakeup( );
	//! Execute a function in the thread.
	//! @param		funcpointer		Update function pointer.
	//! @param		parameter1		User defined parameter.
	//! @param		parameter2		User defined parameter.
	_bool CallFunction( OnUpdateThread funcpointer, _void* parameter1, _void* parameter2 );

	//! Set the sleep interval of thread in milliseconds, the thread will execute update functions every interval.
	//! @param		interval		The sleep interval of thread in milliseconds, the default interval is 100ms.
	_void SetInterval( _dword interval );
	//! Wait for thread end.
	//! @param		none
	_void WaitForThreadEnd( );

	//! Enable or disable thread object self delete after close callback function called.
	//! @param		enable			Wether self delete the thread object.
	_void EnableSelfDelete( _bool enable );

	//! Set the priority level of thread.
	//! @param		priority		The priority to set.
	//! @return		True indicates success, false indicates failure.
	_bool SetPriority( _dword priority );

	//! Get if this thread is busy ( has some update function to call ) or not.
	//! @param		none.
	//! @return		True if thread is busy.
	_bool IsThreadBusy( ) const;

	//! Register an update function to thread, the function will be called every interval in the thread.
	//! @param		funcpointer		Update function pointer.
	//! @param		parameter1		User defined parameter.
	//! @param		parameter2		User defined parameter.
	_void RegisterUpdateFunc( OnUpdateThread funcpointer, _void* parameter1, _void* parameter2 );
	//! Register a close function to thread, the function will be called when thread is being finished.
	//! @param		funcpointer		Close function pointer.
	//! @param		parameter1		User defined parameter.
	//! @param		parameter2		User defined parameter.
	_void RegisterCloseFunc( OnCloseThread funcpointer, _void* parameter1, _void* parameter2 );

	//! Clear all registered function.
	//! @param		none.
	_void ClearAllFunc( );
};

//----------------------------------------------------------------------------
// Thread Implementation
//----------------------------------------------------------------------------

_dword Thread::GetThreadID( ) const
{
	return mThreadID;
}

};