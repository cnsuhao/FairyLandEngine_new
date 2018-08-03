//============================================================================
// UDPSocket.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// UDPSocket
//----------------------------------------------------------------------------

class UDPSocket : public Socket
{
public:
	UDPSocket( BufferManager& buffermanager );

	_bool Send( const Address& address, SocketOperation* operation, _bool forcesend = _false );
	_bool Recv( SocketOperation* operation );
};

};