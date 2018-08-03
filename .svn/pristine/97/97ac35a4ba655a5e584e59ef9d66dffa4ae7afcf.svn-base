//============================================================================
// IUDPSocket.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IUDPSocket
//----------------------------------------------------------------------------

class IUDPSocket
{
public:
	virtual _void	EnablePollingMode( _bool enable ) = 0;
	virtual _bool	IsPollingModeEnabled( ) const = 0;

	virtual _void	EnablePacketHeader( _bool enable ) = 0;
	virtual _bool	IsPacketHeaderEnabled( ) const = 0;

	virtual _void	SetUserData( _dword userdata ) = 0;
	virtual _dword	GetUserData( ) const = 0;

	virtual _bool	Listen( ) = 0;
	virtual _bool	Send( const Address& address, const _void* buffer, _dword length ) = 0;

	virtual Address	GetLocalAddress( ) = 0;
};

};