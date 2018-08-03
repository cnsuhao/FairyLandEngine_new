//============================================================================
// Monitor.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// LowFrequencyTickcounter Implementation
//----------------------------------------------------------------------------

LowFrequencyTickcounter::LowFrequencyTickcounter( _dword& tickcount ) : mTickcount( tickcount )
{
	mStartTime = System::GetCurrentTickcount( );
}

LowFrequencyTickcounter::~LowFrequencyTickcounter( )
{
	_dword currenttime = System::GetCurrentTickcount( );

	if ( currenttime > mStartTime )
		mTickcount += currenttime - mStartTime;
}

//----------------------------------------------------------------------------
// HighFrequencyTickcounter Implementation
//----------------------------------------------------------------------------

HighFrequencyTickcounter::HighFrequencyTickcounter( _qword& tickcount ) : mTickcount( tickcount )
{
	mStartTime = System::GetCurrentCycleCount( );
}

HighFrequencyTickcounter::~HighFrequencyTickcounter( )
{
	_qword currenttime = System::GetCurrentCycleCount( );

	if ( currenttime > mStartTime )
		mTickcount += currenttime - mStartTime;
}

//----------------------------------------------------------------------------
// SpeedRecorder Implementation
//----------------------------------------------------------------------------

SpeedRecorder::SpeedRecorder( )
{
	for ( _dword i = 0; i < _MAX_RECORD_TIME; i ++ )
		mRecentBytes[i] = 0;

	mTotalBytes	= 0;
	mSecond		= 0;
	mDuration	= 1;
}

_void SpeedRecorder::AddBytes( _dword bytes )
{
	mRecentBytes[0] += bytes;
	mTotalBytes		+= bytes;
}

_void SpeedRecorder::Update( _dword elapse )
{
	mSecond		+= elapse;
	mDuration	+= elapse;

	while ( mSecond >= 1000 )
	{
		_dword number = _MAX_RECORD_TIME - 2;

		for ( _dword i = 0; i <= number; i ++ )
			mRecentBytes[ number - i + 1 ] = mRecentBytes[ number - i ];
		mRecentBytes[0] = 0;

		mSecond -= 1000;
	}
}

_dword SpeedRecorder::GetRecentBytes( ) const
{
	_dword bytes = 0;

	for ( _dword i = 0; i < _MAX_RECORD_TIME; i ++ )
		bytes += mRecentBytes[i];

	return bytes;
}

_dword SpeedRecorder::GetTotalBytes( ) const
{
	return mTotalBytes;
}

_float SpeedRecorder::GetRecentSpeed( ) const
{
	_dword bytes = 0;

	for ( _dword i = 0; i < _MAX_RECORD_TIME; i ++ )
		bytes += mRecentBytes[i];

	return bytes / 10240.0f;
}

_float SpeedRecorder::GetTotalSpeed( ) const
{
	return mTotalBytes / 10240.0f * 1000.0f / mDuration;
}