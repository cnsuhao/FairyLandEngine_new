//============================================================================
// AsyncManager.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// AsyncManager Implementation
//----------------------------------------------------------------------------

AsyncManager::AsyncManager( ) : mManagerThreadID( 0 ), mForceAsyncMode( _false )
{
}

_void AsyncManager::SetManagerThreadID( _dword threadid )
{
	mManagerThreadID = threadid;
}

_void AsyncManager::SetForceAsyncMode( _bool asyncmode )
{
	mForceAsyncMode = asyncmode;
}

_bool AsyncManager::GetForceAsyncMode( ) const
{
	return mForceAsyncMode;
}

_bool AsyncManager::NeedAsyncOperation( ) const
{
	if ( mForceAsyncMode )
		return _true;

	if ( mManagerThreadID == 0 )
		return _false;

	return System::GetCurrentThreadID( ) != mManagerThreadID;
}