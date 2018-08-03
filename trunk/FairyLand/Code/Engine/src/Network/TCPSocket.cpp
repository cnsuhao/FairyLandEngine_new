//============================================================================
// TCPSocket.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// TCPSocket Implementation
//----------------------------------------------------------------------------

TCPSocket::TCPSocket( BufferManager& buffermanager ) : Socket( buffermanager )
{
}

_bool TCPSocket::Listen( _dword queue )
{
	return ::listen( mSocket, queue ) == 0;
}

_bool TCPSocket::Accept( _dword socket, SocketOperation* operation )
{
	if ( operation == _null )
		return _false;

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	networkmanager.StatisticIncreaseSocketOperation( SocketOperation::_TCP_ACCEPT );

	// Set a socket operation to accept.
	operation->mSocketID	= mSocket;
	operation->mType		= SocketOperation::_TCP_ACCEPT;

	// Old code, will result TCP accept operation never finish if client not send any bytes.
	//_dword receivedatalength = operation->mRawSocket == _null ? sizeof( Packet ) : 0;

	_dword receivedatalength = 0;

	// Submit accept operation for listening socket, keep trying until overlapped IO operation sumbit successfully.
	while ( ::AcceptEx( socket, mSocket, operation->mBuffer, receivedatalength, 32, 32, _null, (OVERLAPPED*) operation ) == _false )
	{
		// Error occors, return false.
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

	return _true;
}

_bool TCPSocket::Connect( const Address& address, SocketOperation* operation )
{
	if ( operation == _null )
		return _false;

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	networkmanager.StatisticIncreaseSocketOperation( SocketOperation::_TCP_CONNECT );

	// Set a socket operation to connect.
	operation->mSocketID	= mSocket;
	operation->mType		= SocketOperation::_TCP_CONNECT;
	operation->mAddress		= SocketAddress( address );

	#ifndef WSAID_CONNECTEX

	typedef
	BOOL (PASCAL FAR * LPFN_CONNECTEX) (
		IN SOCKET s,
		IN const struct sockaddr FAR *name,
		IN int namelen,
		IN PVOID lpSendBuffer OPTIONAL,
		IN DWORD dwSendDataLength,
		OUT LPDWORD lpdwBytesSent,
		IN LPOVERLAPPED lpOverlapped
		);

	#define WSAID_CONNECTEX \
		{0x25a207b9,0xddf3,0x4660,{0x8e,0xe9,0x76,0xe5,0x8c,0x74,0x06,0x3e}}

	#endif

	LPFN_CONNECTEX connectexfunc = _null; GUID guidconnectex = WSAID_CONNECTEX; _dword bytes = 0;

	// Use WSAConnect API.
	if ( ::WSAIoctl( mSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidconnectex, sizeof( GUID ), &connectexfunc, sizeof( _void* ), &bytes, _null, _null ) != 0 || connectexfunc == _null )
	{
		operation->mEvent = ::WSACreateEvent( );
		::WSAEventSelect( mSocket, operation->mEvent, FD_CONNECT );

		if ( ::WSAConnect( mSocket, (SOCKADDR*) &( operation->mAddress ), sizeof( SocketAddress ), _null, _null, _null, _null ) != 0 )
		{
			if ( ::GetLastError( ) != WSAEWOULDBLOCK && ::GetLastError( ) != 0 )
			{
				mBufferManager.ReleaseOperation( operation );
				return _false;
			}
		}

		NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

		networkmanager.DeferTCPConnect( operation );
	}
	// Use ConnectEx API.
	else
	{
		// Connect to remote address, keep trying until overlapped IO operation sumbit successfully.
		while ( (*connectexfunc)( mSocket, (SOCKADDR*) &( operation->mAddress ), sizeof( SocketAddress ), _null, 0, &bytes, (OVERLAPPED*) operation ) == _false )
		{
			if ( ::GetLastError( ) == WSAEWOULDBLOCK )
			{
				System::Sleep( 0 );

				continue;
			}

			if ( ::GetLastError( ) == WSA_IO_PENDING )
				break;

			mBufferManager.ReleaseOperation( operation );

			return _false;
		}
	}

	return _true;
}

_bool TCPSocket::Send( SocketOperation* operation, _bool forcesend )
{
	if ( operation == _null )
		return _false;

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	if ( forcesend == _false )
	{
		// Set a socket operation to send.
		operation->mSocketID	= mSocket;
		operation->mType		= SocketOperation::_TCP_SEND;

		if ( HasDeferOperation( ) || IsSendBusy( ) )
		{
			DeferOperation( operation );

			return _true;
		}
	}

	networkmanager.StatisticIncreaseSocketOperation( SocketOperation::_TCP_SEND );

	// Create a socket buffer, ready to send.
	WSABUF databuffer = { operation->mLength, (_chara*) operation->mBuffer };

	_dword length = operation->mLength, flag = 0;

	// Send packet, keep trying until overlapped IO operation sumbit successfully.
	while ( ::WSASend( mSocket, &databuffer, 1, &length, flag, (OVERLAPPED*) operation, _null ) != 0 )
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

_bool TCPSocket::Recv( SocketOperation* operation )
{
	if ( operation == _null )
		return _false;

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	networkmanager.StatisticIncreaseSocketOperation( SocketOperation::_TCP_RECV );

	// Set a socket operation to receive.
	operation->mSocketID	= mSocket;
	operation->mType		= SocketOperation::_TCP_RECV;

	// Create a socket buffer, ready to receive.
	WSABUF databuffer = { operation->mLength, (_chara*) operation->mBuffer };

	_dword length = operation->mLength, flag = 0;

	// Receive packet, keep trying until overlapped IO operation sumbit successfully.
	while ( ::WSARecv( mSocket, &databuffer, 1, &length, &flag, (OVERLAPPED*) operation, _null ) != 0 )
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