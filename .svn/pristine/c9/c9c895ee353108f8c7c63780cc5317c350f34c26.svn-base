//============================================================================
// CompletionPort.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Overlapped
//----------------------------------------------------------------------------

struct Overlapped
{
	_dword		mInternal;
	_dword		mInternalHigh;
	_dword		mOffset;
	_dword		mOffsetHigh;
	_handle		mEvent;

	Overlapped( ) : mEvent( _null ) { }
};

//----------------------------------------------------------------------------
// CompletionPort
//----------------------------------------------------------------------------

class CompletionPort : public KernelObject
{
public:
	_bool Create( _dword threadnumber = 0 );
	_bool Attach( _handle object, _dword completionkey = 0 );

	_bool GetQueuedStatus( _dword& bytestransferred, _dword& completionkey, Overlapped*& overlapped, _dword milliseconds );
	_bool PostQueuedStatus( _dword bytestransferred, _dword completionkey, Overlapped* overlapped );
};

};