//============================================================================
// TCPConnection.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// TCPConnection
//----------------------------------------------------------------------------

class TCPConnection : public Connection
{
private:
	TCPSocket*	mTCPSocket;

	_byte*		mUnprocessedBuffer;
	_dword		mUnprocessedSize;

public:
	TCPConnection( _dword socketid, const Address& localaddress, const Address& remoteaddress, BufferManager& buffermanager );
	virtual ~TCPConnection( );

	virtual _dword GetConnectionType( );

	virtual _bool Connect( );
	virtual _void Close( );

	virtual _bool Send( SocketOperation* operation );
	virtual _bool Recv( );
	virtual _bool ProcessData( _byte* buffer, _dword length );
	virtual _bool ProcessPacket( Packet* packet );
};

};