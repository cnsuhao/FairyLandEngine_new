//============================================================================
// TCPSocket.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// TCPSocket
//----------------------------------------------------------------------------

class TCPSocket : public Socket
{
public:
	TCPSocket( BufferManager& buffermanager );

	_bool Listen( _dword queue = 5 );

	_bool Accept( _dword socket, SocketOperation* operation );
	_bool Connect( const Address& address, SocketOperation* operation );

	_bool Send( SocketOperation* operation, _bool forcesend = _false );
	_bool Recv( SocketOperation* operation );
};

};