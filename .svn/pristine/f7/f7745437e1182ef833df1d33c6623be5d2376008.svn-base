//============================================================================
// NetworkStatistics.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// NetworkStatistics Implementation
//----------------------------------------------------------------------------

NetworkStatistics::NetworkStatistics( _dword interval, _dword statnumber )
{
	mInterval			= 0;
	mStatNumber			= 0;

	mStatSendTime		= _null;
	mStatSendBytes		= _null;
	mStatReceiveTime	= _null;
	mStatReceiveBytes	= _null;
	mStatLatency		= _null;

	SetStatisticsConfig( interval, statnumber );

	mLastUpdateTickcount = System::GetCurrentTickcount( );
}

NetworkStatistics::~NetworkStatistics( )
{
	delete[] mStatSendTime;
	delete[] mStatSendBytes;
	delete[] mStatReceiveTime;
	delete[] mStatReceiveBytes;
	delete[] mStatLatency;
}

_void NetworkStatistics::Update( )
{
	_dword tickcount = System::GetCurrentTickcount( );

	if ( tickcount - mLastUpdateTickcount < mInterval )
		return;

	_dword number = mStatNumber - 2;

	// Update statistic.
	for ( _dword i = 0; i <= number; i ++ )
	{
		mStatSendTime[ number - i + 1 ]		= mStatSendTime[ number - i ];
		mStatSendBytes[ number - i + 1 ]	= mStatSendBytes[ number - i ];
		mStatReceiveTime[ number - i + 1 ]	= mStatReceiveTime[ number - i ];
		mStatReceiveBytes[ number - i + 1 ]	= mStatReceiveBytes[ number - i ];
	}

	mStatSendTime[0]		= 0;
	mStatSendBytes[0]		= 0;
	mStatReceiveTime[0]		= 0;
	mStatReceiveBytes[0]	= 0;

	mLastUpdateTickcount	= tickcount;
}

_void NetworkStatistics::StatisticSend( _dword bytes )
{
	mStatSendTime[0]		+= 1;
	mStatSendBytes[0]		+= bytes;
}

_void NetworkStatistics::StatisticReceive( _dword bytes )
{
	mStatReceiveTime[0]		+= 1;
	mStatReceiveBytes[0]	+= bytes;
}

_void NetworkStatistics::StatisticLatency( _dword latency )
{
	_dword number = mStatNumber - 2;

	// Update statistic.
	for ( _dword i = 0; i <= number; i ++ )
		mStatLatency[ number - i + 1 ] = mStatLatency[ number - i ];

	mStatLatency[0] = latency;
}

_void NetworkStatistics::SetStatisticsConfig( _dword interval, _dword statnumber )
{
	delete[] mStatSendTime;
	delete[] mStatSendBytes;
	delete[] mStatReceiveTime;
	delete[] mStatReceiveBytes;
	delete[] mStatLatency;

	mInterval	= interval;
	mStatNumber	= statnumber;

	if ( mStatNumber == 0 )
		return;

	mStatSendTime		= new _dword[ statnumber ];
	mStatSendBytes		= new _dword[ statnumber ];
	mStatReceiveTime	= new _dword[ statnumber ];
	mStatReceiveBytes	= new _dword[ statnumber ];
	mStatLatency		= new _dword[ statnumber ];

	ResetStatistics( );
}

_dword NetworkStatistics::GetStatisticsInterval( ) const
{
	return mInterval;
}

_dword NetworkStatistics::GetStatisticsNumber( ) const
{
	return mStatNumber;
}

_void NetworkStatistics::ResetStatistics( )
{
	if ( mStatNumber == 0 )
		return;

	Memory::MemSet( mStatSendTime, 0, sizeof( _dword ) * mStatNumber );
	Memory::MemSet( mStatSendBytes, 0, sizeof( _dword ) * mStatNumber );
	Memory::MemSet( mStatReceiveTime, 0, sizeof( _dword ) * mStatNumber );
	Memory::MemSet( mStatReceiveBytes, 0, sizeof( _dword ) * mStatNumber );
	Memory::MemSet( mStatLatency, 0, sizeof( _dword ) * mStatNumber );
}

_dword NetworkStatistics::GetRecentSendTime( _dword seconds ) const
{
	seconds *= 1000;

	_dword sendtime = 0;

	for ( _dword i = 0; i < mStatNumber && seconds >= mInterval; i ++ )
	{
		seconds  -= mInterval;
		sendtime += mStatSendTime[i];
	}

	return sendtime;
}

_dword NetworkStatistics::GetRecentSendBytes( _dword seconds ) const
{
	seconds *= 1000;

	_dword sendbytes = 0;

	for ( _dword i = 0; i < mStatNumber && seconds >= mInterval; i ++ )
	{
		seconds   -= mInterval;
		sendbytes += mStatSendBytes[i];
	}

	return sendbytes;
}

_dword NetworkStatistics::GetRecentReceiveTime( _dword seconds ) const
{
	seconds *= 1000;

	_dword receivetime = 0;

	for ( _dword i = 0; i < mStatNumber && seconds >= mInterval; i ++ )
	{
		seconds     -= mInterval;
		receivetime += mStatReceiveTime[i];
	}

	return receivetime;
}

_dword NetworkStatistics::GetRecentReceiveBytes( _dword seconds ) const
{
	seconds *= 1000;

	_dword receivebytes = 0;

	for ( _dword i = 0; i < mStatNumber && seconds >= mInterval; i ++ )
	{
		seconds      -= mInterval;
		receivebytes += mStatReceiveBytes[i];
	}

	return receivebytes;
}

_void NetworkStatistics::GetTotalSendTime( _dword* statbuffer, _dword statnumber ) const
{
	Memory::MemCpy( statbuffer, mStatSendTime, Math::Min( statnumber, mStatNumber ) * sizeof( _dword ) );
}

_void NetworkStatistics::GetTotalSendBytes( _dword* statbuffer, _dword statnumber ) const
{
	Memory::MemCpy( statbuffer, mStatSendBytes, Math::Min( statnumber, mStatNumber ) * sizeof( _dword ) );
}

_void NetworkStatistics::GetTotalReceiveTime( _dword* statbuffer, _dword statnumber ) const
{
	Memory::MemCpy( statbuffer, mStatReceiveTime, Math::Min( statnumber, mStatNumber ) * sizeof( _dword ) );
}

_void NetworkStatistics::GetTotalReceiveBytes( _dword* statbuffer, _dword statnumber ) const
{
	Memory::MemCpy( statbuffer, mStatReceiveBytes, Math::Min( statnumber, mStatNumber ) * sizeof( _dword ) );
}

_void NetworkStatistics::GetTotalLatency( _dword* statbuffer, _dword statnumber ) const
{
	Memory::MemCpy( statbuffer, mStatLatency, Math::Min( statnumber, mStatNumber ) * sizeof( _dword ) );	
}