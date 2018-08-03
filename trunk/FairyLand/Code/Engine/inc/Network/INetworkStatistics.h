//============================================================================
// INetworkStatistics.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// INetworkStatistics
//----------------------------------------------------------------------------

class INetworkStatistics
{
public:
	virtual _void	SetStatisticsConfig( _dword interval = 1000, _dword statnumber = 10 ) = 0;
	virtual _dword	GetStatisticsInterval( ) const = 0;
	virtual _dword	GetStatisticsNumber( ) const = 0;
	virtual _void	ResetStatistics( ) = 0;

	virtual _dword	GetRecentSendTime( _dword seconds = 10 ) const = 0;
	virtual _dword	GetRecentSendBytes( _dword seconds = 10 ) const = 0;
	virtual _dword	GetRecentReceiveTime( _dword seconds = 10 ) const = 0;
	virtual _dword	GetRecentReceiveBytes( _dword seconds = 10 ) const = 0;

	virtual _void	GetTotalSendTime( _dword* statbuffer, _dword statnumber ) const = 0;
	virtual _void	GetTotalSendBytes( _dword* statbuffer, _dword statnumber ) const = 0;
	virtual _void	GetTotalReceiveTime( _dword* statbuffer, _dword statnumber ) const = 0;
	virtual _void	GetTotalReceiveBytes( _dword* statbuffer, _dword statnumber ) const = 0;
	virtual _void	GetTotalLatency( _dword* statbuffer, _dword statnumber ) const = 0;
};

};