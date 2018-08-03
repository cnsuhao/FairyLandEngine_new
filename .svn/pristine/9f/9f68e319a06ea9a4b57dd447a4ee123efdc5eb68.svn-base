//============================================================================
// Monitor.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// LowFrequencyTickcounter
//----------------------------------------------------------------------------

class LowFrequencyTickcounter
{
private:
	_dword&	mTickcount;
	_dword	mStartTime;

public:
	LowFrequencyTickcounter( _dword& tickcount );
	~LowFrequencyTickcounter( );
};

//----------------------------------------------------------------------------
// HighFrequencyTickcounter
//----------------------------------------------------------------------------

class HighFrequencyTickcounter
{
private:
	_qword&	mTickcount;
	_qword	mStartTime;

public:
	HighFrequencyTickcounter( _qword& tickcount );
	~HighFrequencyTickcounter( );
};

//----------------------------------------------------------------------------
// SpeedRecorder
//----------------------------------------------------------------------------

class SpeedRecorder
{
private:
	enum { _MAX_RECORD_TIME = 16 };

	_dword	mRecentBytes[ _MAX_RECORD_TIME ];
	_dword	mTotalBytes;
	_dword	mSecond;
	_dword	mDuration;

public:
	SpeedRecorder( );

	_void	AddBytes( _dword bytes );
	_void	Update( _dword elapse );

	_dword	GetRecentBytes( ) const;
	_dword	GetTotalBytes( ) const;

	_float	GetRecentSpeed( ) const;
	_float	GetTotalSpeed( ) const;
};

};