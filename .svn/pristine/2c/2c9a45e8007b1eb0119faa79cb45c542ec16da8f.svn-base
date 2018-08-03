//============================================================================
// Connection.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Connection
//----------------------------------------------------------------------------

class Connection
{
public:
	enum _CONNECTION_PROPERTY
	{
		_PROPERTY_INITIATIVE		= 0x0001,
		_PROPERTY_LAN				= 0x0002,
		_PROPERTY_COMPRESSPACKET	= 0x0004,
		_PROPERTY_RELIABLEPING		= 0x0008,

		_PROPERTY_AUTORECONNECT		= 0x0010,
		_PROPERTY_RESENDWITHSN		= 0x0040,
		_PROPERTY_NONEEDCONFIRM		= 0x0080,

		_PROPERTY_ASKCOMPRESS		= 0x0100,
		_PROPERTY_AGREECOMPRESS		= 0x0200,

		_PROPERTY_PINGONLY			= 0x8000,
	};

	enum _CONNECTION_INTERVAL
	{
		_KEEPALIVE_INTERVAL		= 15 * 1000,
		_CONNECT_INTERVAL		= 40 * 1000,
		_DEFAULT_TIMEOUT		= 60 * 1000,
	};

	BufferManager&		mBufferManager;

	_dword				mGlobalToken;

	_dword				mSocketID;
	_dword				mSecretCode;

	_dword				mLocalSessionID;
	_dword				mLocalConnectionID;
	_dword				mRemoteSessionID;
	_dword				mRemoteConnectionID;

	_dword				mProperty;
	_dword				mPriority;
	_dword				mPhase;
	_dword				mCloseReason;

	_dword				mGroupID;
	_dword				mUserData;
	_dword				mTimer;
	_dword				mTimeout;
	_dword				mConnectInterval;

	_dword				mLastTickcount;
	_dword				mCreateTickcount;
	_dword				mKeepAliveTickcount;
	_dword				mWaitAliveTickcount;
	_dword				mUpdateSpeedTickcount;
	_dword				mLatency;

	_dword				mPacketTickcount;
	_dword				mSendPacketReference;
	_dword				mRecvPacketReference;
	_dword				mDelayChannel;
	_dword				mDelayTickcount;
	_dword				mDelayReference;

	_dword				mSentBytes;
	_dword				mReceivedBytes;
	_dword				mRecentSentBytes[10];
	_dword				mRecentReceivedBytes[10];

	_byte*				mReservedBuffer;

	_byte*				mDeferBuffer;
	_dword				mDeferIndex;
	_dword				mDeferPacketNumber;

	_byte*				mIncompleteBuffer;
	_dword				mIncompleteIndex;
	_dword				mIncompleteSize;
	_dword				mIncompletePacketNumber;

	Address				mLocalAddress;
	Address				mRemoteAddress;
	Address				mInternalAddress;
	Address				mBackupAddress;

	CompressionStream	mInputStream;
	CompressionStream	mOutputStream;

	_byte*				mInputBuffer;
	_dword				mInputIndex;
	_dword				mInputSize;
	_dword				mInputLogicSize;
	_dword				mInputRealSize;

	_byte*				mOutputBuffer;
	_dword				mOutputIndex;
	_dword				mOutputLogicSize;
	_dword				mOutputRealSize;

	Connection*			mNextConnectionInGroup;

	DelayPacketArray	mDelayPacketArray;

	String				mLostReason;

public:
	Connection( _dword socketid, const Address& localaddress, const Address& remoteaddress, BufferManager& buffermanager );
	virtual ~Connection( );

	_bool SendPacket( _void* buffer );
	_bool SendPacket( _void* buffer, _dword length );
	_bool SendPacket( const PacketBuffer& buffer );
	_bool SendBigPacket( const PacketBuffer& buffer, _dword maxpacketsize );
	_bool SendDeferPacket( const PacketBuffer& buffer );
	_bool FlushDeferPacket( );

	_bool SendCompressPacket( PacketBuffer& buffer );
	_bool FlushCompressPacket( );

	_bool ProcessNormalPacket( _byte* remainbuffer, _dword remainbytes, _dword packetflag, _byte* reservedbuffer );
	_bool ProcessPacketList( _byte* remainbuffer, _dword remainbytes, _byte* reservedbuffer );
	_bool ProcessBigPacket( _byte* remainbuffer, _dword remainbytes );

	_void FlushDelayPacket( _dword channel );
	_void CreateDelayPacket( _dword channel, _dword tickcount, _byte* buffer, _dword size, _byte* reservedbuffer );
	_void CheckDelayPacket( _dword sessiontickcount );
	_void UpdateDelayPacket( _dword deltatickcount );
	_void ClearDelayPacket( );

	virtual _dword GetConnectionType( )							= 0;

	virtual _bool Connect( )									= 0;
	virtual _void Close( )										= 0;

	virtual _bool Send( SocketOperation* operation )			= 0;
	virtual _bool Recv( )										= 0;
	virtual _bool ProcessData( _byte* buffer, _dword length )	= 0;

	virtual _bool ProcessPacket( Packet* packet );
	virtual _bool KeepAlive( _dword tickcount );
	virtual _void FlushFinish( );
};

};