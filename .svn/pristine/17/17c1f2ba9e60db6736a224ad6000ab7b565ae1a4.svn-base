//============================================================================
// Socket.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// SocketAddress
//----------------------------------------------------------------------------

struct SocketAddress
{
	_word			mFamily;
	_word			mPort;
	_dword			mIP;
	_dword			mReserve1;
	_dword			mReserve2;

	SocketAddress( )
		: mFamily( 2 ), mPort( 0 ), mIP( 0 ), mReserve1( 0 ), mReserve2( 0 ) { }

	SocketAddress( const Address& address )
		: mFamily( 2 ), mPort( Word( address.mPort ).InverseByteOrder( ) ), mIP( Dword( address.mIP ).InverseByteOrder( ) ), mReserve1( 0 ), mReserve2( 0 ) { }
};

//----------------------------------------------------------------------------
// SocketOperation
//----------------------------------------------------------------------------

struct SocketOperation : public Overlapped
{
	enum
	{
		_MAX_SENDOPT_NUMBER		= 256,
		_MAX_RECVOPT_NUMBER		= 32,
		_MAX_ACCEPTOPT_NUMBER	= 16,
	};

	enum _OPERATION_TYPE
	{
		_TCP_ACCEPT		= 1,
		_TCP_CONNECT	= 2,
		_TCP_SEND		= 3,
		_TCP_RECV		= 4,
		_UDP_SEND		= 5,
		_UDP_RECV		= 6,
	};

	_dword			mType;
	_dword			mSocketID;
	_dword			mSessionID;
	_dword			mConnectionID;
	_byte*			mBuffer;
	_dword			mLength;
	_dword			mAddressSize;
	SocketAddress	mAddress;

	SocketOperation( )
		: mType( 0 ), mSocketID( 0 ), mSessionID( 0 ), mConnectionID( 0 ), mBuffer( _null ), mLength( 0 ), mAddressSize( 0 ) { }

	SocketOperation( _byte* buffer, _dword length )
		: mType( 0 ), mSocketID( 0 ), mSessionID( 0 ), mConnectionID( 0 ), mBuffer( buffer ), mLength( length ), mAddressSize( sizeof( SocketAddress ) ) { }
};

typedef Array< SocketOperation* > SocketOperationArray;
typedef Queue< SocketOperation* > SocketOperationQueue;

//----------------------------------------------------------------------------
// DelayOperation
//----------------------------------------------------------------------------

struct DelayOperation
{
	_dword			mDelayTime;
	_dword			mSocketID;
	_dword			mCompletionKey;
	_dword			mBytesTransferred;
	SocketOperation	mOperation;

	DelayOperation( )
		: mDelayTime( 0 ), mSocketID( 0 ), mCompletionKey( 0 ), mBytesTransferred( 0 ) { }
};

typedef Array< DelayOperation > DelayOperationArray;

//----------------------------------------------------------------------------
// PollingOperation
//----------------------------------------------------------------------------

struct PollingOperation
{
	enum _SOCKET_OPERATION
	{
		_TCP_ACCEPT			= 1,
		_TCP_CONNECT		= 2,
		_TCP_DISCONNECT		= 3,
		_TCP_RECEIVE		= 4,
		_UDP_RECEIVE		= 5,
	};

	enum _SESSION_OPERATION
	{
		_CREATE_CONNECTION	= 11,
		_DELETE_CONNECTION	= 12,
		_RECEIVE_PACKET		= 13,
		_UPDATE_LATENCY		= 14,
		_JOIN_FAILED		= 15,
		_UPDATE_SESSION		= 16,
	};

	_dword			mOperationType;
	_dword			mSessionID;
	_dword			mConnectionID;
	_dword			mLatency;
	_void*			mSocket;
	_dword			mError;
	Address			mAddress;
	_byte*			mBuffer;
	_dword			mSize;
	_bool			mFlush;

	_byte*			mReservedBuffer;

	PollingOperation( )
		: mOperationType( 0 ), mSessionID( 0 ), mConnectionID( 0 ), mLatency( 0 ), mSocket( _null ), mError( 0 ), mBuffer( _null ), mSize( 0 ), mFlush( _false ), mReservedBuffer( _null ) { }
};

typedef Queue< PollingOperation > PollingOperationQueue;

//----------------------------------------------------------------------------
// DelayPacket
//----------------------------------------------------------------------------

struct DelayPacket
{
	_dword			mChannel;
	_dword			mTickcount;
	_byte*			mBuffer;
	_dword			mSize;

	_byte*			mReservedBuffer;

	DelayPacket( )
		: mChannel( 0 ), mTickcount( 0 ), mBuffer( _null ), mSize( 0 ), mReservedBuffer( _null ) { }
};

typedef Array< DelayPacket > DelayPacketArray;

//----------------------------------------------------------------------------
// Socket
//----------------------------------------------------------------------------

class BufferManager;
class Socket : public ReferencedObject
{
public:
	_dword					mSocket;
	_dword					mSubnetMask;
	_dword					mSendOptNumber;
	_dword					mRecvOptNumber;
	_dword					mTotalOptNumber;

	_void*					mSocketWrapper;

	_byte*					mAddressBuffer;
	_byte*					mAcceptBuffer;
	_dword					mAcceptLength;
	_dword					mAcceptSocket;

	Address					mLocalAddress;
	Address					mRemoteAddress;

	BufferManager&			mBufferManager;
	SocketOperationQueue	mDeferQueue;

public:
	Socket( BufferManager& buffermanager );
	~Socket( );

	inline operator _dword ( ) const;

	_bool Create( _dword type );
	_void Close( );

	_bool Bind( const Address& address );

	_word GetSocketType( ) const;

	inline _bool IsSendBusy( ) const;
	inline _bool HasDeferOperation( ) const;
	inline _void PostSendOperation( );
	inline _void DoneSendOperation( );
	inline _void PostRecvOperation( );
	inline _void DoneRecvOperation( );

	_void SetLocalAddress( const Address& address );
	_void SetRemoteAddress( const Address& address );

	const Address& GetLocalAddress( );
	const Address& GetRemoteAddress( );

	_void DeferOperation( SocketOperation* operation );
	_void FlushQueue( );
};

//----------------------------------------------------------------------------
// Socket Implementation
//----------------------------------------------------------------------------

Socket::operator _dword ( ) const
{
	return mSocket & 0x0000FFFF;
}

_bool Socket::IsSendBusy( ) const
{
	return mSendOptNumber >= SocketOperation::_MAX_SENDOPT_NUMBER;
}

_bool Socket::HasDeferOperation( ) const
{
	return mDeferQueue.Number( ) > 0;
}

_void Socket::PostSendOperation( )
{
	mSendOptNumber ++;
	mTotalOptNumber ++;
}

_void Socket::DoneSendOperation( )
{
	if ( mSendOptNumber > 0 )
		mSendOptNumber --;
}

_void Socket::PostRecvOperation( )
{
	mRecvOptNumber ++;
	mTotalOptNumber ++;
}

_void Socket::DoneRecvOperation( )
{
	if ( mRecvOptNumber > 0 )
		mRecvOptNumber --;
}

};