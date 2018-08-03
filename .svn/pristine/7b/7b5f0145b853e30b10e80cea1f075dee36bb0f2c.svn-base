//============================================================================
// UDPSocket.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// UDPSocket Implementation
//----------------------------------------------------------------------------

UDPSocket::UDPSocket( BufferManager& buffermanager ) : Socket( buffermanager )
{
}

_bool UDPSocket::Send( const Address& address, SocketOperation* operation, _bool forcesend )
{
	if ( operation == _null )
		return _false;

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	if ( forcesend == _false )
	{
		// Set a socket operation to send.
		operation->mSocketID	= mSocket;
		operation->mType		= SocketOperation::_UDP_SEND;
		operation->mAddress		= SocketAddress( address );

		if ( HasDeferOperation( ) || IsSendBusy( ) )
		{
			DeferOperation( operation );

			return _true;
		}
	}

	networkmanager.StatisticIncreaseSocketOperation( SocketOperation::_UDP_SEND );

	if ( address.mType != 0 )
		operation->mAddress	= SocketAddress( address );

	// Create a socket buffer, ready to send.
	WSABUF databuffer = { operation->mLength, (_chara*) operation->mBuffer };

	_dword length = operation->mLength, flag = 0;

	// Send packet, keep trying until overlapped IO operation sumbit successfully.
	while ( ::WSASendTo( mSocket, &databuffer, 1, &length, flag, (SOCKADDR*) &( operation->mAddress ), operation->mAddressSize, (OVERLAPPED*) operation, _null ) != 0 )
	{
		if ( ::GetLastError( ) == WSAEWOULDBLOCK )
		{
			DeferOperation( operation );

			return _true;
		}

		if ( ::GetLastError( ) == WSA_IO_PENDING )
			break;

		mBufferManager.ReleaseOperation( operation );

		return _false;
	}

	PostSendOperation( );

	return _true;
}

_bool UDPSocket::Recv( SocketOperation* operation )
{
	if ( operation == _null )
		return _false;

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	networkmanager.StatisticIncreaseSocketOperation( SocketOperation::_UDP_RECV );

	// Set a socket operation to receive.
	operation->mSocketID	= mSocket;
	operation->mType		= SocketOperation::_UDP_RECV;

	// Create a socket buffer, ready to receive.
	WSABUF databuffer = { operation->mLength, (_chara*) operation->mBuffer };

	_dword length = operation->mLength, flag = 0;

	// Receive packet, keep trying until overlapped IO operation sumbit successfully.
	while ( ::WSARecvFrom( mSocket, &databuffer, 1, &length, &flag, (SOCKADDR*) &( operation->mAddress ), (_long*) &operation->mAddressSize, (OVERLAPPED*) operation, _null ) != 0 )
	{
		if ( ::GetLastError( ) == WSAEWOULDBLOCK )
		{
			System::Sleep( 1 );

			continue;
		}

		if ( ::GetLastError( ) == WSA_IO_PENDING )
			break;

		mBufferManager.ReleaseOperation( operation );

		return _false;
	}

	PostRecvOperation( );

	return _true;
}