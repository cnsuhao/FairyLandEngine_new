//============================================================================
// TCPSocketWrapper.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// TCPSocketWrapper Implementation
//----------------------------------------------------------------------------

TCPSocketWrapper::TCPSocketWrapper( TCPSocket* socket, Lock& lock, BufferManager& buffermanager )
	: mTCPSocket( socket ), mNetworkLock( lock ), mBufferManager( buffermanager ), mPollingMode( _false ), mPacketHeader( _false ), mReleased( _false ), mUserData( 0 )
{
	socket->mSocketWrapper = this;
	socket->IncreaseRefCount( );

	IncreaseRefCount( );
}

_bool TCPSocketWrapper::Recv( )
{
	// Post overlapped received operation.
	return mTCPSocket->Recv( mBufferManager.CreateOperation( ) );
}

_void TCPSocketWrapper::EnablePollingMode( _bool enable )
{
	mPollingMode = enable;
}

_bool TCPSocketWrapper::IsPollingModeEnabled( ) const
{
	return mPollingMode;
}

_void TCPSocketWrapper::EnablePacketHeader( _bool enable )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	mPacketHeader = enable;
}

_bool TCPSocketWrapper::IsPacketHeaderEnabled( ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	return mPacketHeader;
}

_void TCPSocketWrapper::SetUserData( _dword userdata )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	mUserData = userdata;
}

_dword TCPSocketWrapper::GetUserData( ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	return mUserData;
}

_bool TCPSocketWrapper::Listen( _dword queue )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// Start listen.
	if ( mTCPSocket->Listen( queue ) == _false )
		return _false;

	Address localaddress = mTCPSocket->GetLocalAddress( );

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	// Create many sockets for accepting connection.
	for ( _dword i = 0; i < SocketOperation::_MAX_ACCEPTOPT_NUMBER; i ++ )
	{
		// Create socekt to accept new connection.
		TCPSocket* acceptsocket = (TCPSocket*) networkmanager.CreateSocket( Address::_TYPE_TCP );

		if ( acceptsocket != _null )
		{
			acceptsocket->SetLocalAddress( localaddress );

			// Post accept operation.
			acceptsocket->Accept( mTCPSocket->mSocket, mBufferManager.CreateOperation( ) );
		}
	}

	return _true;
}

_bool TCPSocketWrapper::Connect( const Address& address )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// Start connecting.
	if ( mTCPSocket->Connect( address, mBufferManager.CreateOperation( ) ) == _false )
		return _false;

	mTCPSocket->SetRemoteAddress( address );

	return _true;
}

_bool TCPSocketWrapper::Send( const _void* buffer, _dword length )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	PacketBuffer sendbuffer;

	// Build packet header.
	RawPacket header( (_word) ( length + sizeof( RawPacket ) ) );

	if ( mPacketHeader )
	{
		sendbuffer.mBuffer[0] = &header;
		sendbuffer.mLength[0] = sizeof( RawPacket );
		sendbuffer.mBuffer[1] = (_void*) buffer;
		sendbuffer.mLength[1] = length;
	}
	else
	{
		sendbuffer.mBuffer[0] = (_void*) buffer;
		sendbuffer.mLength[0] = length;
	}

	// Post overlapped send operation.
	return mTCPSocket->Send( mBufferManager.CreateOperation( sendbuffer ) );
}

Address TCPSocketWrapper::GetLocalAddress( )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	return mTCPSocket->GetLocalAddress( );
}

Address TCPSocketWrapper::GetRemoteAddress( )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	return mTCPSocket->GetRemoteAddress( );
}