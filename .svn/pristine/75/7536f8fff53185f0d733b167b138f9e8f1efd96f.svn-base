//============================================================================
// ITCPSocket.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ITCPSocket
//----------------------------------------------------------------------------

class ITCPSocket
{
public:
	virtual _void	EnablePollingMode( _bool enable ) = 0;
	virtual _bool	IsPollingModeEnabled( ) const = 0;

	virtual _void	EnablePacketHeader( _bool enable ) = 0;
	virtual _bool	IsPacketHeaderEnabled( ) const = 0;

	virtual _void	SetUserData( _dword userdata ) = 0;
	virtual _dword	GetUserData( ) const = 0;

	virtual _bool	Listen( _dword queue = 5 ) = 0;
	virtual _bool	Connect( const Address& address ) = 0;
	virtual _bool	Send( const _void* buffer, _dword length ) = 0;

	virtual Address	GetLocalAddress( ) = 0;
	virtual Address	GetRemoteAddress( ) = 0;
};

};