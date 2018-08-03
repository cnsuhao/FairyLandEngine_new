//============================================================================
// UDPSocketWrapper.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// UDPSocketWrapper Implementation
//----------------------------------------------------------------------------

UDPSocketWrapper::UDPSocketWrapper( UDPSocket* socket, Lock& lock, BufferManager& buffermanager )
	: mUDPSocket( socket ), mNetworkLock( lock ), mBufferManager( buffermanager ), mPollingMode( _false ), mPacketHeader( _false ), mReleased( _false ), mUserData( 0 )
{
	socket->mSocketWrapper = this;
	socket->IncreaseRefCount( );

	IncreaseRefCount( );
}

_bool UDPSocketWrapper::Recv( )
{
	// Post overlapped received operation.
	return mUDPSocket->Recv( mBufferManager.CreateOperation( ) );
}

_void UDPSocketWrapper::EnablePollingMode( _bool enable )
{
	mPollingMode = enable;
}

_bool UDPSocketWrapper::IsPollingModeEnabled( ) const
{
	return mPollingMode;
}

_void UDPSocketWrapper::EnablePacketHeader( _bool enable )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	mPacketHeader = enable;
}

_bool UDPSocketWrapper::IsPacketHeaderEnabled( ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	return mPacketHeader;
}

_void UDPSocketWrapper::SetUserData( _dword userdata )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	mUserData = userdata;
}

_dword UDPSocketWrapper::GetUserData( ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	return mUserData;
}

_bool UDPSocketWrapper::Listen( )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( mUDPSocket->mTotalOptNumber > 0 )
		return _true;

	for ( _dword i = 0; i < SocketOperation::_MAX_RECVOPT_NUMBER; i ++ )
		Recv( );

	return _true;
}

_bool UDPSocketWrapper::Send( const Address& address, const _void* buffer, _dword length )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	Listen( );

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
	return mUDPSocket->Send( address, mBufferManager.CreateOperation( sendbuffer ) );
}

Address	UDPSocketWrapper::GetLocalAddress( )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	return mUDPSocket->GetLocalAddress( );
}