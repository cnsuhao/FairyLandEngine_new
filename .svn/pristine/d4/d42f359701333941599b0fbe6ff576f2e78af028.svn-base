//============================================================================
// AsyncManager.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// AsyncManager
//----------------------------------------------------------------------------

class AsyncManager
{
protected:
	_dword	mManagerThreadID;
	_bool	mForceAsyncMode;
	Lock	mManagerLock;

protected:
	AsyncManager( );

public:
	_void SetManagerThreadID( _dword threadid );

	_void SetForceAsyncMode( _bool asyncmode );
	_bool GetForceAsyncMode( ) const;

	_bool NeedAsyncOperation( ) const;
};

};