//============================================================================
// NetworkStatistics.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// NetworkStatistics
//----------------------------------------------------------------------------

class NetworkStatistics : public INetworkStatistics
{
private:
	_dword	mInterval;
	_dword	mStatNumber;

	_dword*	mStatSendTime;
	_dword*	mStatSendBytes;
	_dword*	mStatReceiveTime;
	_dword*	mStatReceiveBytes;
	_dword*	mStatLatency;

	_dword	mLastUpdateTickcount;

public:
	NetworkStatistics( _dword interval, _dword statnumber );
	~NetworkStatistics( );

	_void			Update( );
	_void			StatisticSend( _dword bytes );
	_void			StatisticReceive( _dword bytes );
	_void			StatisticLatency( _dword latency );

	virtual _void	SetStatisticsConfig( _dword interval, _dword statnumber );
	virtual _dword	GetStatisticsInterval( ) const;
	virtual _dword	GetStatisticsNumber( ) const;
	virtual _void	ResetStatistics( );

	virtual _dword	GetRecentSendTime( _dword seconds ) const;
	virtual _dword	GetRecentSendBytes( _dword seconds ) const;
	virtual _dword	GetRecentReceiveTime( _dword seconds ) const;
	virtual _dword	GetRecentReceiveBytes( _dword seconds ) const;

	virtual _void	GetTotalSendTime( _dword* statbuffer, _dword statnumber ) const;
	virtual _void	GetTotalSendBytes( _dword* statbuffer, _dword statnumber ) const;
	virtual _void	GetTotalReceiveTime( _dword* statbuffer, _dword statnumber ) const;
	virtual _void	GetTotalReceiveBytes( _dword* statbuffer, _dword statnumber ) const;
	virtual _void	GetTotalLatency( _dword* statbuffer, _dword statnumber ) const;
};

};