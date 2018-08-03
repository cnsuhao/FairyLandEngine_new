//============================================================================
// Socket.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// Socket Implementation
//----------------------------------------------------------------------------

Socket::Socket( BufferManager& buffermanager ) : mBufferManager( buffermanager )
{
	mSocket			= 0;
	mSubnetMask		= -1;
	mSendOptNumber	= 0;
	mRecvOptNumber	= 0;
	mTotalOptNumber	= 0;

	mSocketWrapper	= _null;

	// Used for TCP accept operation.
	mAddressBuffer	= _null;
	mAcceptBuffer	= _null;
	mAcceptLength	= 0;
	mAcceptSocket	= 0;
}

Socket::~Socket( )
{
	Close( );
}

_bool Socket::Create( _dword type )
{
	_dword socktype = 0;

	switch ( type )
	{
		case Address::_TYPE_TCP: socktype = SOCK_STREAM; break;
		case Address::_TYPE_UDP: socktype = SOCK_DGRAM;  break;
	}

	// Invalid socket type.
	if ( socktype == 0 )
		return _false;

	// Create udp socket.
	_dword socket = (_dword) ::WSASocket( AF_INET, socktype, 0, _null, 0, WSA_FLAG_OVERLAPPED );

	if ( socket == INVALID_SOCKET )
		return _false;

	mSocket	= socket;

	// Set socket option, set send buffer size to 0K.
	_dword sendbuffersize = 0;
	if ( ::setsockopt( mSocket, SOL_SOCKET, SO_SNDBUF, (_chara*) &sendbuffersize, sizeof( _dword ) ) != 0 )
		return _false;

	// Set socket option, set receive buffer size to 80K ( TCP ) or 800K ( UDP ).
	_dword recvbuffersize = type == Address::_TYPE_UDP ? 800 * 1024 : 80 * 1024;
	if ( ::setsockopt( mSocket, SOL_SOCKET, SO_RCVBUF, (_chara*) &recvbuffersize, sizeof( _dword ) ) != 0 )
		return _false;

	// Set SIO_UDP_CONNRESET option, to disable WSAECONNRESET problem when Recvfrom failed. 
	if ( type == Address::_TYPE_UDP )
	{
		_dword disable = _false, bytesreturned = 0;
		::WSAIoctl( mSocket, SIO_UDP_CONNRESET, &disable, sizeof( _bool ), _null, 0, &bytesreturned, _null, _null );
	}

	// Get subnet mask.
	_byte buffer[1024]; _dword bytes = 0;
	if ( ::WSAIoctl( mSocket, SIO_GET_INTERFACE_LIST, _null, 0, buffer, 1024, &bytes, _null, _null ) == 0 )
	{
		INTERFACE_INFO* infolist = (INTERFACE_INFO*) buffer;

		for ( _dword i = 0; i < bytes / sizeof( INTERFACE_INFO ); i ++ )
		{
			// Skip interfaces that have been disabled.
			if ( ( infolist[i].iiFlags & IFF_UP ) == 0 )
				continue;

			// Skip the loop back interface.
			if ( ( infolist[i].iiFlags & IFF_LOOPBACK ) != 0 )
				continue;

			// Skip other than ipv4.
			if ( infolist[i].iiAddress.AddressIn.sin_family != AF_INET )
				continue;

			// Skip empty subnet address.
			if ( infolist[i].iiNetmask.AddressIn.sin_addr.s_addr == 0 )
				continue;

			// Get subnet mask.
			mSubnetMask = Dword( infolist[i].iiNetmask.AddressIn.sin_addr.s_addr ).InverseByteOrder( );

			break;
		}
	}

	return _true;
}

_bool Socket::Bind( const Address& address )
{
	// Set socket option, reuse address.
	_dword enable = 1;
	if ( ::setsockopt( mSocket, SOL_SOCKET, SO_REUSEADDR, (_chara*) &enable, sizeof( _dword ) ) != 0 )
		return _false;

	// Bind socket with address.
	if ( ::bind( mSocket, (SOCKADDR*) &SocketAddress( address ), sizeof( SocketAddress ) ) != 0 )
		return _false;

	return _true;
}

_void Socket::Close( )
{
	if ( mSocket == 0 )
		return;

	::shutdown( mSocket, SD_BOTH );
	::closesocket( mSocket );

	mSocket = 0;
}

_word Socket::GetSocketType( ) const
{
	// Get socket type ( TCP or UDP )
	_long socktype, length = sizeof( _dword );
	if ( ::getsockopt( mSocket, SOL_SOCKET, SO_TYPE, (_chara*) &socktype, &length ) != 0 )
		return 0;

	_word type = 0;

	switch ( socktype )
	{
		case SOCK_STREAM: type = Address::_TYPE_TCP; break;
		case SOCK_DGRAM:  type = Address::_TYPE_UDP; break;
	}

	return type;
}

_void Socket::SetLocalAddress( const Address& address )
{
	mLocalAddress = address;
}

_void Socket::SetRemoteAddress( const Address& address )
{
	mRemoteAddress = address;
}

const Address& Socket::GetLocalAddress( )
{
	if ( mLocalAddress != Address( ) )
		return mLocalAddress;

	// Get host socket address.
	SocketAddress sockaddr; _long length = sizeof( SocketAddress );

	// Maybe failed because IPv6, try again.
	if ( ::getsockname( mSocket, (SOCKADDR*) &sockaddr, &length ) != 0 )
		return mLocalAddress;

	mLocalAddress.mIP	= Dword( sockaddr.mIP ).InverseByteOrder( );
	mLocalAddress.mType	= GetSocketType( );
	mLocalAddress.mPort	= Word( sockaddr.mPort ).InverseByteOrder( );

	return mLocalAddress;
}

const Address& Socket::GetRemoteAddress( )
{
	if ( mRemoteAddress != Address( ) )
		return mRemoteAddress;

	// Get peer socket address.
	SocketAddress sockaddr; _long length = sizeof( SocketAddress );
	if ( ::getpeername( mSocket, (SOCKADDR*) &sockaddr, &length ) != 0 )
		return mRemoteAddress;

	mRemoteAddress.mIP		= Dword( sockaddr.mIP ).InverseByteOrder( );
	mRemoteAddress.mType	= GetSocketType( );
	mRemoteAddress.mPort	= Word( sockaddr.mPort ).InverseByteOrder( );

	return mRemoteAddress;
}

_void Socket::DeferOperation( SocketOperation* operation )
{
	mDeferQueue.Append( operation );
}

_void Socket::FlushQueue( )
{
	if ( mDeferQueue.Number( ) == 0 )
		return;

	// We can send defered operation.
	while ( IsSendBusy( ) == _false )
	{
		SocketOperation* deferoperation = mDeferQueue.Front( );

		// TCP send operation.
		if ( deferoperation->mType == SocketOperation::_TCP_SEND )
		{
			( (TCPSocket*) this )->Send( deferoperation, _true );
		}
		// UDP send operation.
		else if ( deferoperation->mType == SocketOperation::_UDP_SEND )
		{
			( (UDPSocket*) this )->Send( Address( ), deferoperation, _true );
		}
		// Unknown operation.
		else
		{
			FL_ASSERT( 0 )
		}

		mDeferQueue.Remove( );

		if ( mDeferQueue.Number( ) == 0 )
			break;
	}
}