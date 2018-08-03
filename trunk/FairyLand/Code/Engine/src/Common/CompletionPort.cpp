//============================================================================
// CompletionPort.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// CompletionPort Implementation
//----------------------------------------------------------------------------

_bool CompletionPort::Create( _dword threadnumber )
{
	mObjectHandle = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, _null, 0, threadnumber );

	return mObjectHandle != _null;
}

_bool CompletionPort::Attach( _handle object, _dword completionkey )
{
	return ::CreateIoCompletionPort( object, mObjectHandle, completionkey, 0 ) != _null;
}

_bool CompletionPort::GetQueuedStatus( _dword& bytestransferred, _dword& completionkey, Overlapped*& overlapped, _dword milliseconds )
{
	return ::GetQueuedCompletionStatus( mObjectHandle, &bytestransferred, &completionkey, (OVERLAPPED**) &overlapped, milliseconds ) != 0;
}

_bool CompletionPort::PostQueuedStatus( _dword bytestransferred, _dword completionkey, Overlapped* overlapped )
{
	return ::PostQueuedCompletionStatus( mObjectHandle, bytestransferred, completionkey, (OVERLAPPED*) overlapped ) != 0;
}