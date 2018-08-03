//============================================================================
// NetworkHelper.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// NetworkHelper Implementation
//----------------------------------------------------------------------------

NetworkHelper::NetworkHelper( BufferManager& buffermanager ) : mBufferManager( buffermanager )
{
	mHelperSocketID		= 0;
	mKeepAliveTickcount	= 0;
}

_bool NetworkHelper::NeedKeepAlive( _dword tickcount )
{
	if ( tickcount - mKeepAliveTickcount < _KEEPALIVE_INTERVAL )
		return _false;

	mKeepAliveTickcount = tickcount;

	return _true;
}

_void NetworkHelper::KeepHelperAlive( _dword socketid )
{
	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	// Get socket from socket pool by id.
	Socket* socket = networkmanager.SearchSocket( socketid );
	FL_ASSERT( socket != _null )

	// Get local address of the socket.
	Address localaddress = socket->GetLocalAddress( );

	// If the socket is not UDP socket, just return.
	if ( localaddress.mType != Address::_TYPE_UDP )
		return;

	// Build keep alive packet using local address.
	KeepAlivePacket keepalive;

	PacketBuffer sendbuffer;
	sendbuffer.mBuffer[0] = &keepalive;
	sendbuffer.mLength[0] = keepalive.mPacketSize;

	// Post overlapped send operation.
	( (UDPSocket*) socket )->Send( mHelperServer, mBufferManager.CreateOperation( sendbuffer ) );
}

_void NetworkHelper::SetHelperServer( const Address& address )
{
	mHelperServer = address;
}

_bool NetworkHelper::OpenHelperPort( const Address& address )
{
	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	// Close previous socket used by helper.
	if ( mHelperSocketID != 0 )
		CloseHelperPort( );

	// Search helper socket by address.
	Socket* socket = networkmanager.SearchSocket( address, _false );

	_bool newsocket = _false;

	// If there is no existing socket with same address, create a new socket.
	if ( socket == _null )
	{
		socket		= networkmanager.CreateSocket( address );
		newsocket	= _true;
	}

	// Create socket failed, return false.
	if ( socket == _null )
		return _false;

	socket->IncreaseRefCount( );

	// Post many receive operations.
	if ( newsocket )
	{
		for ( _dword i = 0; i < SocketOperation::_MAX_RECVOPT_NUMBER; i ++ )
			( (UDPSocket*) socket )->Recv( mBufferManager.CreateOperation( ) );
	}

	mHelperPort		= address;
	mHelperSocketID	= (*socket);

	return _true;
}

_void NetworkHelper::CloseHelperPort( )
{
	if ( mHelperSocketID == 0 )
		return;

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	networkmanager.ReleaseSocket( mHelperSocketID );

	mHelperPort		= Address( );
	mHelperSocketID	= 0;
}

_bool NetworkHelper::ProcessPacket( _dword socketid, Packet* packet, const Address& address )
{
	return _false;
}