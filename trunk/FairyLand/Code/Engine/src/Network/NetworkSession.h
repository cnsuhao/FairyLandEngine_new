//============================================================================
// NetworkSession.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// NetworkSession
//----------------------------------------------------------------------------

class NetworkSession : public INetworkSession
{
public:
	typedef Triple< _dword, _dword, _dword >		RefTickcount;
	typedef Array< RefTickcount, _dword >			RefTickcountArray;
	typedef Pool< Connection* >						ConnectionPool;
	typedef Triple< Connection*, _dword, _dword >	GroupTriple;
	typedef Array< GroupTriple >					GroupArray;

	Lock&								mNetworkLock;
	BufferManager&						mBufferManager;

	INetworkManager::OnCreateConnection	mCreateConnectionFunc;
	INetworkManager::OnDeleteConnection	mDeleteConnectionFunc;
	INetworkManager::OnReceivePacket	mReceivePacketFunc;
	INetworkManager::OnUpdateLatency	mUpdateLatencyFunc;
	INetworkManager::OnJoinFailed		mJoinFailedFunc;
	INetworkManager::OnUpdateSession	mUpdateSessionFunc;

	_bool								mPollingMode;
	_bool								mWaitForUpdate;
	_bool								mReleased;
	_bool								mClosed;
	_dword								mRefCount;

	_dword								mFlushInterval;
	_dword								mFlushTickcount;
	_dword								mLastUpdateTickcount;
	_dword								mInitSessionTickcount;
	_bool								mEnableSessionTickcount;

	_dword								mSessionID;
	_dword								mProperty;
	_dword								mPriority;
	_dword								mNewConnectionID;
	_dword								mAliveConnectionNumber;
	_dword								mUserData;
	_dword								mGlobalToken;

	NetworkStatistics*					mSessionStatistics;

	ConnectionPool						mConnectionPool;
	GroupArray							mGroupArray;
	RefTickcountArray					mRefTickcountArray;

private:
	_dword			GetGroupIndex( _dword groupid ) const;
	_bool			JoinSessionHelper( const Address& localaddress, const Address& remoteaddress, _dword groupid, _dword userdata, _dword sessionid, _bool ping );
	_bool			SendPacketHelper( Connection* connection, const _void* buffer, _dword length );
	_bool			SendDeferPacketHelper( Connection* connection, const _void* buffer, _dword length );
	_bool			SendConnectionPropertyHelper( _dword connectionid, _dword mask, _bool enable );

	Socket*			CreateJoinSocket( const Address& localaddress, const Address& remoteaddress );
	Connection*		Reconnect( Connection* connection );

public:
	NetworkSession( _dword sessionid, _dword maxconnection, Lock& lock, BufferManager& buffermanager );
	~NetworkSession( );

	inline _bool	IsSessionFull( ) const
		{ return mConnectionPool.Number( ) == mConnectionPool.Size( ); }

	Connection*		CreateConnection( _dword socketid, const Address& localaddress, const Address& remoteaddress );
	Connection*		SearchConnection( _dword socketid, const Address& address ) const;
	Connection*		SearchConnection( _dword connectionid ) const;
	Connection*		GetGroupFirstConnection( _dword groupid ) const;

	_void			UpdateSession( _dword tickcount );
	_void			CheckDeadConnection( );

	virtual _bool		OpenPort( const Address& localaddress, _dword groupid );
	virtual _void		ClosePort( const Address& localaddress );
	virtual _void		ClosePort( _dword groupid );

	virtual _bool		IsPortOpened( const Address& localaddress, _dword& groupid );
	virtual _bool		IsPortOpened( _dword groupid, Address& localaddress );

	virtual _bool		PingSession( const Address& remoteaddress, _dword groupid, _dword userdata, _dword sessionid );
	virtual _bool		PingSession( const Address& localaddress, const Address& remoteaddress, _dword groupid, _dword userdata, _dword sessionid );

	virtual _bool		JoinSession( const Address& remoteaddress, _dword groupid, _dword userdata, _dword sessionid );
	virtual _bool		JoinSession( const Address& localaddress, const Address& remoteaddress, _dword groupid, _dword userdata, _dword sessionid );

	virtual _dword		GetSessionID( ) const;
	virtual _dword		GetSessionGlobalToken( ) const;
	virtual _dword		GetNewConnectionID( ) const;

	virtual _void		EnablePollingMode( _bool enable );
	virtual _bool		IsPollingModeEnabled( ) const;

	virtual _void		SetPriority( _dword priority );
	virtual _dword		GetPriority( ) const;

	virtual _void		SetUserData( _dword userdata );
	virtual _dword		GetUserData( ) const;

	virtual _void		SetFlushInterval( _dword interval );
	virtual _dword		GetFlushInterval( ) const;

	virtual _dword		GetAliveConnectionNumber( ) const;
	virtual _dword		GetSessionConnectionNumber( ) const;
	virtual _dword		GetSessionFirstConnectionID( ) const;
	virtual _dword		GetSessionNextConnectionID( _dword previousid ) const;
	virtual _dword		GetGroupConnectionNumber( _dword groupid ) const;
	virtual _dword		GetGroupFirstConnectionID( _dword groupid ) const;
	virtual _dword		GetGroupNextConnectionID( _dword groupid, _dword previousid ) const;

	virtual _void		SetGroupUserData( _dword groupid, _dword userdata );
	virtual _dword  	GetGroupUserData( _dword groupid ) const;

	virtual _void		SetConnectionGroup( _dword connectionid, _dword groupid );
	virtual _dword  	GetConnectionGroup( _dword connectionid ) const;
	virtual _void		SetConnectionUserData( _dword connectionid, _dword userdata );
	virtual _dword  	GetConnectionUserData( _dword connectionid ) const;
	virtual _void		SetConnectionTimer( _dword connectionid, _dword timer );
	virtual _dword  	GetConnectionTimer( _dword connectionid ) const;
	virtual _void		SetConnectionTimeout( _dword connectionid, _dword timeout );
	virtual _dword  	GetConnectionTimeout( _dword connectionid ) const;
	virtual _void		SetConnectionConnectInterval( _dword connectionid, _dword interval );
	virtual _dword  	GetConnectionConnectInterval( _dword connectionid ) const;
	virtual _void		SetConnectionPriority( _dword connectionid, _dword priority );
	virtual _dword  	GetConnectionPriority( _dword connectionid ) const;
	virtual _dword  	GetConnectionGlobalToken( _dword connectionid ) const;
	virtual _dword  	GetConnectionLatency( _dword connectionid ) const;
	virtual _dword  	GetConnectionDuration( _dword connectionid ) const;
	virtual _dword  	GetConnectionPhase( _dword connectionid ) const;
	virtual _dword  	GetConnectionSentBytes( _dword connectionid ) const;
	virtual _dword		GetConnectionRecentSentBytes( _dword connectionid ) const;
	virtual _float  	GetConnectionSendSpeed( _dword connectionid ) const;
	virtual _dword  	GetConnectionReceivedBytes( _dword connectionid ) const;
	virtual _dword		GetConnectionRecentReceivedBytes( _dword connectionid ) const;
	virtual _float  	GetConnectionReceiveSpeed( _dword connectionid ) const;
	virtual _dword		GetConnectionCloseReason( _dword connectionid ) const;
	virtual StringPtr	GetConnectionCloseReasonString( _dword connectionid ) const;
	virtual Address		GetConnectionLocalAddress( _dword connectionid ) const;
	virtual Address		GetConnectionRemoteAddress( _dword connectionid ) const;
	virtual _dword		GetConnectionRemoteSessionID( _dword connectionid ) const;
	virtual _bool		IsLANConnection( _dword connectionid ) const;

	virtual _bool		SetConnectionBackupAddress( _dword connectionid, const Address& remoteaddress );
	virtual _dword		SearchConnection( const Address& remoteaddress, _dword groupid ) const;

	virtual _void		EnableSessionTickcount( _bool enable );
	virtual _bool		IsSessionTickcountEnabled( ) const;
	virtual _void		InitSessionTickcount( _dword tickcount );
	virtual _dword		GetSessionTickcount( ) const;
	virtual _dword		GetPacketTickcount( _dword connectionid ) const;

	virtual _void		SetPacketReference( _dword connectionid, _dword referenceid );
	virtual _dword		GetPacketReference( _dword connectionid ) const;

	virtual _void		SetReferenceTickcount( _dword referenceid, _dword sessiontickcount, _dword duration );
	virtual _dword		GetReferenceTickcount( _dword referenceid ) const;

	virtual _bool		SetDelayTickcount( _dword connectionid, _dword channel, _dword sessiontickcount, _dword referenceid );
	virtual _bool		SetGroupDelayTickcount( _dword groupid, _dword excludeconnection, _dword channel, _dword sessiontickcount, _dword referenceid );
	virtual _bool		SetSessionDelayTickcount( _dword excludegroup, _dword excludeconnection, _dword channel, _dword sessiontickcount, _dword referenceid );

	virtual _bool		SendPacket( _dword connectionid, const _void* buffer, _dword length );
	virtual _bool		SendDeferPacket( _dword connectionid, const _void* buffer, _dword length );
	virtual _bool		FlushDeferPacket( _dword connectionid );

	virtual _bool		SendGroupPacket( _dword groupid, _dword excludeconnection, const _void* buffer, _dword length );
	virtual _bool		SendGroupDeferPacket( _dword groupid, _dword excludeconnection, const _void* buffer, _dword length );
	virtual _bool		FlushGroupDeferPacket( _dword groupid, _dword excludeconnection );

	virtual _bool		SendSessionPacket( _dword excludegroup, _dword excludeconnection, const _void* buffer, _dword length );
	virtual _bool		SendSessionDeferPacket( _dword excludegroup, _dword excludeconnection, const _void* buffer, _dword length );
	virtual _bool		FlushSessionDeferPacket( _dword excludegroup, _dword excludeconnection );

	virtual _bool		EnableAutoReconnect( _dword connectionid, _bool enable );
	virtual _bool		EnableCompressPacket( _dword connectionid, _bool enable );
	virtual _bool		EnableResendPacketList( _dword connectionid, _bool enable );
	virtual _bool		EnableNoNeedConfirmPacket( _dword connectionid, _bool enable );
	virtual _bool		SyncConnectionProperty( _dword connectionid );

	virtual _void		CloseConnection( _dword connectionid, _bool needcallback = _true );
	virtual _void		CloseGroup( _dword groupid, _bool needcallback = _true );
	virtual _void		CloseSession( _bool needcallback );

	virtual _dword		GetRecentSendTime( _dword seconds ) const;
	virtual _dword		GetRecentSendBytes( _dword seconds ) const;
	virtual _dword		GetRecentReceiveTime( _dword seconds ) const;
	virtual _dword		GetRecentReceiveBytes( _dword seconds ) const;

	virtual INetworkStatistics*	GetStatistics( ) const;
};

};