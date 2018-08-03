//============================================================================
// NetworkManager.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// NetworkManager
//----------------------------------------------------------------------------

class NetworkManager : public INetworkManager
{
private:
	typedef Pool< Socket* >						SocketPool;
	typedef Pool< INetworkSession* >			SessionPool;
	typedef Triple< _dword, _dword, _dword >	SocketSessionTriple;
	typedef Array< SocketSessionTriple >		SocketSessionArray;

	OnTCPSocketAccept		mTCPSocketAcceptFunc;
	OnTCPSocketConnect		mTCPSocketConnectFunc;
	OnTCPSocketDisconnect	mTCPSocketDisconnectFunc;
	OnTCPSocketReceive		mTCPSocketReceiveFunc;
	OnUDPSocketReceive		mUDPSocketReceiveFunc;

	OnCreateConnection		mCreateConnectionFunc;
	OnDeleteConnection		mDeleteConnectionFunc;
	OnReceivePacket			mReceivePacketFunc;
	OnUpdateLatency			mUpdateLatencyFunc;
	OnJoinFailed			mJoinFailedFunc;
	OnUpdateSession			mUpdateSessionFunc;
	OnNetworkTimer			mNetworkTimerFunc;

	CompletionPort			mSocketIOCP;
	Thread					mNetworkThread;
	Lock					mNetworkLock;

	BufferManager			mBufferManager;

	// Pool of win32 socket.
	SocketPool				mSocketPool;
	// Pool of network session.
	SessionPool				mSessionPool;
	// Relationship array of socket and session.
	SocketSessionArray		mSocketSessionArray;

	// Delaied socket operation array, used for debug.
	DelayOperationArray		mDelayOperationArray;
	// Delaied socket operation queue, used for polling mode.
	PollingOperationQueue	mPollingOperationQueue;
	// Defer TCP connection array.
	SocketOperationArray	mDeferConnectArray;

	NetworkNATHelper		mNATHelper;
	NetworkAgentHelper		mAgentHelper;

	_dword					mLastTickcount;
	_dword					mGlobalToken;
	_dword					mNetworkLatency;
	_byte*					mLastReservedBuffer;

	_bool					mEnablePollingMode;

	NetworkStatistic		mNetworkStatistic;

	static _bool IOCPThread( _void* parameter1, _void* parameter2 );
	static _bool TimerThread( _void* parameter1, _void* parameter2 );

	_void ProcessOperation( Socket* socket, SocketOperation* operation, _dword completionkey, _dword bytestransferred );
	_void ReserveOperation( Socket* socket, SocketOperation* operation, _dword completionkey, _dword bytestransferred );

	_bool OnTCPAccept( _dword socketid, _dword sessionid, _dword listensocketid, _byte* acceptbuffer, _dword length, _byte* addressbuffer );
	_bool OnTCPConnect( _dword socketid, _dword sessionid, _dword connectionid );
	_bool OnTCPSend( _dword socketid, _dword sessionid, _dword connectionid, _byte* buffer, _dword length );
	_bool OnTCPRecv( _dword socketid, _dword sessionid, _dword connectionid, _byte* buffer, _dword length );
	_bool OnUDPSend( _dword socketid, _dword sessionid, _dword connectionid, _byte* buffer, _dword length );
	_bool OnUDPRecv( _dword socketid, const Address& sourceaddress, _byte* buffer, _dword length );
	_bool OnError( _dword socketid, _dword sessionid, _dword connectionid, _dword operationtype, _dword completionkey );

public:
	NetworkManager( );
	~NetworkManager( );

	inline NetworkNATHelper&	GetNATHelper( );
	inline NetworkAgentHelper&	GetAgentHelper( );

	inline _void	StatisticIncreaseSocketOperation( _dword type );
	inline _void	StatisticDecreaseSocketOperation( _dword type );
	inline _void	StatisticCreateConnection( );
	inline _void	StatisticDeleteConnection( );
	inline _void	StatisticResentPacket( _dword bytes );
	inline _void	StatisticPacketsInOrder( );
	inline _void	StatisticPacketsInDisorder( );
	inline _void	StatisticPacketsDroped( );

	inline _byte*	GetLastReservedBuffer( ) const;

	static _dword	GetLocalIP( _dword adapter );
	static _void	GetConnectionAddress( _byte* addressbuffer, Address& localaddress, Address& remoteaddress );

	_dword		CreateSecretCode( );
	_dword		CreateGlobalToken( );

	_void		CallTCPSocketAccept( ITCPSocket* socket );
	_void		CallTCPSocketConnect( ITCPSocket* socket, _dword errorcode );
	_void		CallTCPSocketDisconnect( ITCPSocket* socket, _dword errorcode );
	_void		CallTCPSocketReceive( ITCPSocket* socket, const _void* buffer, _dword length, _byte* reservedbuffer );
	_void		CallUDPSocketReceive( IUDPSocket* socket, const Address& address, const _void* buffer, _dword length, _byte* reservedbuffer );

	_void		CallCreateConnectionCallback( _dword sessionid, _dword connectionid );
	_void		CallDeleteConnectionCallback( _dword sessionid, _dword connectionid );
	_void		CallReceivePacketCallback( _dword sessionid, _dword connectionid, _byte* buffer, _dword size, _byte* reservedbuffer, _bool flush = _false );
	_void		CallUpdateLatencyCallback( _dword sessionid, _dword connectionid, _dword latency );
	_void		CallJoinFailedCallback( _dword sessionid, _dword connectionid );
	_void		CallUpdateSessionCallback( _dword sessionid );
	_void		CallNetworkTimerCallback( _dword tickcount );

	Socket*		CreateSocket( _dword type );
	Socket*		CreateSocket( const Address& address );
	Socket*		SearchSocket( const Address& address, _bool hostsocket ) const;
	Socket*		SearchSocket( _dword socketid ) const;
	_void		ReleaseSocket( _dword socketid );

	_void		SocketAttachSession( _dword socketid, _dword sessionid, _dword groupid );
	_void		SocketDetachSession( _dword sessionid, const Address& address );
	_void		SocketDetachSession( _dword sessionid, _dword groupid );
	_bool		IsSocketHosting( _dword socketid, _dword sessionid, _dword& groupid ) const;
	_bool		IsGroupHosting( _dword sessionid, _dword groupid, _dword& socketid ) const;

	_void		ConnectUseHelper( _dword socketid, _dword sessionid, const Address& address );
	_void		DeferTCPConnect( SocketOperation* operation );
	_void		FinishTCPConnect( _dword sessionid, _dword connectionid );

	_bool		ProcessNATRequest( Packet* packet, _dword socketid, const Address& address );

	virtual _void				Release( );

	virtual _void				Update( _dword elapse );

	virtual _void				SetTCPSocketAcceptCallback( OnTCPSocketAccept funcpointer );
	virtual _void				SetTCPSocketConnectCallback( OnTCPSocketConnect funcpointer );
	virtual _void				SetTCPSocketDisconnectCallback( OnTCPSocketDisconnect funcpointer );
	virtual _void				SetTCPSocketReceiveCallback( OnTCPSocketReceive funcpointer );
	virtual _void				SetUDPSocketReceiveCallback( OnUDPSocketReceive funcpointer );

	virtual _void				SetCreateConnectionCallback( OnCreateConnection funcpointer, _dword sessionid );
	virtual _void				SetDeleteConnectionCallback( OnDeleteConnection funcpointer, _dword sessionid );
	virtual _void				SetReceivePacketCallback( OnReceivePacket funcpointer, _dword sessionid );
	virtual _void				SetUpdateLatencyCallback( OnUpdateLatency funcpointer, _dword sessionid );
	virtual _void				SetJoinFailedCallback( OnJoinFailed funcpointer, _dword sessionid );
	virtual _void				SetUpdateSessionCallback( OnUpdateSession funcpointer, _dword sessionid );
	virtual _void				SetNetworkTimerCallback( OnNetworkTimer funcpointer );

	virtual _void				SetBufferLimit( _dword memorylimit );

	virtual Address				CreateAddress( _word type, _word port, _dword adapter );
	virtual Address				CreateAddress( _word type, _dword ip, _word port );
	virtual Address				CreateAddress( _word type, StringPtr ipaddress, _word port );
	virtual Address				CreateAddress( _word type, StringPtr ipaddress );

	virtual ITCPSocket*			CreateTCPSocket( const Address& bindaddress );
	virtual ITCPSocket*			SearchTCPSocket( const Address& bindaddress );
	virtual _void				ReleaseTCPSocket( ITCPSocket*& socket );

	virtual IUDPSocket*			CreateUDPSocket( const Address& bindaddress );
	virtual IUDPSocket*			SearchUDPSocket( const Address& bindaddress );
	virtual _void				ReleaseUDPSocket( IUDPSocket*& socket );

	virtual _dword				GetAvailableSessionID( ) const;
	virtual _dword				GetTotalSessionNumber( ) const;

	virtual INetworkSession*	CreateSession( _dword sessionid, _dword maxconnection );
	virtual INetworkSession*	SearchSession( _dword sessionid ) const;
	virtual _void				ReleaseSession( INetworkSession*& networksession );

	virtual IFileDownloader*	CreateHTTPDownloader( );

	virtual _void				SetNATHelperServer( const Address& address );
	virtual _bool				OpenNATHelperPort( const Address& address );
	virtual _void				CloseNATHelperPort( );

	virtual _void				SetAgentHelperServer( const Address& address );
	virtual _bool				OpenAgentHelperPort( const Address& address );
	virtual _void				CloseAgentHelperPort( );

	virtual Thread&				GetNetworkThread( );
	virtual Lock&				GetNetworkLock( );

	virtual _void				SetNetworkLatency( _dword latency );
	virtual _dword				GetNetworkLatency( ) const;

	virtual const NetworkStatistic& GetNetworkStatistic( ) const;
	virtual _void				DumpNetworkStatistic( ) const;
	virtual _void				ClearNetworkStatistic( );
};

//----------------------------------------------------------------------------
// NetworkManager Implementation
//----------------------------------------------------------------------------

NetworkNATHelper& NetworkManager::GetNATHelper( )
{
	return mNATHelper;
}

NetworkAgentHelper& NetworkManager::GetAgentHelper( )
{
	return mAgentHelper;
}

_void NetworkManager::StatisticIncreaseSocketOperation( _dword type )
{
	mNetworkStatistic.mCurrentSocketOpt ++;
	if ( mNetworkStatistic.mMaximalSocketOpt < mNetworkStatistic.mCurrentSocketOpt )
		mNetworkStatistic.mMaximalSocketOpt = mNetworkStatistic.mCurrentSocketOpt;

	switch ( type )
	{
		case SocketOperation::_TCP_ACCEPT:
		{
			mNetworkStatistic.mCurrentTCPAcceptOpt ++;
			if ( mNetworkStatistic.mMaximalTCPAcceptOpt < mNetworkStatistic.mCurrentTCPAcceptOpt )
				mNetworkStatistic.mMaximalTCPAcceptOpt = mNetworkStatistic.mCurrentTCPAcceptOpt;

			break;
		}

		case SocketOperation::_TCP_CONNECT:
		{
			mNetworkStatistic.mCurrentTCPConnectOpt ++;
			if ( mNetworkStatistic.mMaximalTCPConnectOpt < mNetworkStatistic.mCurrentTCPConnectOpt )
				mNetworkStatistic.mMaximalTCPConnectOpt = mNetworkStatistic.mCurrentTCPConnectOpt;

			break;
		}

		case SocketOperation::_TCP_SEND:
		{
			mNetworkStatistic.mCurrentTCPSendOpt ++;
			if ( mNetworkStatistic.mMaximalTCPSendOpt < mNetworkStatistic.mCurrentTCPSendOpt )
				mNetworkStatistic.mMaximalTCPSendOpt = mNetworkStatistic.mCurrentTCPSendOpt;

			break;
		}

		case SocketOperation::_TCP_RECV:
		{
			mNetworkStatistic.mCurrentTCPRecvOpt ++;
			if ( mNetworkStatistic.mMaximalTCPRecvOpt < mNetworkStatistic.mCurrentTCPRecvOpt )
				mNetworkStatistic.mMaximalTCPRecvOpt = mNetworkStatistic.mCurrentTCPRecvOpt;

			break;
		}

		case SocketOperation::_UDP_SEND:
		{
			mNetworkStatistic.mCurrentUDPSendOpt ++;
			if ( mNetworkStatistic.mMaximalUDPSendOpt < mNetworkStatistic.mCurrentUDPSendOpt )
				mNetworkStatistic.mMaximalUDPSendOpt = mNetworkStatistic.mCurrentUDPSendOpt;

			break;
		}

		case SocketOperation::_UDP_RECV:
		{
			mNetworkStatistic.mCurrentUDPRecvOpt ++;
			if ( mNetworkStatistic.mMaximalUDPRecvOpt < mNetworkStatistic.mCurrentUDPRecvOpt )
				mNetworkStatistic.mMaximalUDPRecvOpt = mNetworkStatistic.mCurrentUDPRecvOpt;

			break;
		}
	}
}

_void NetworkManager::StatisticDecreaseSocketOperation( _dword type )
{
	if ( mNetworkStatistic.mCurrentSocketOpt > 0 )
		mNetworkStatistic.mCurrentSocketOpt --;

	switch ( type )
	{
		case SocketOperation::_TCP_ACCEPT:
		{
			if ( mNetworkStatistic.mCurrentTCPAcceptOpt > 0 )
				mNetworkStatistic.mCurrentTCPAcceptOpt --;

			break;
		}

		case SocketOperation::_TCP_CONNECT:
		{
			if ( mNetworkStatistic.mCurrentTCPConnectOpt > 0 )
				mNetworkStatistic.mCurrentTCPConnectOpt --;

			break;
		}

		case SocketOperation::_TCP_SEND:
		{
			if ( mNetworkStatistic.mCurrentTCPSendOpt > 0 )
				mNetworkStatistic.mCurrentTCPSendOpt --;

			break;
		}

		case SocketOperation::_TCP_RECV:
		{
			if ( mNetworkStatistic.mCurrentTCPRecvOpt > 0 )
				mNetworkStatistic.mCurrentTCPRecvOpt --;

			break;
		}

		case SocketOperation::_UDP_SEND:
		{
			if ( mNetworkStatistic.mCurrentUDPSendOpt > 0 )
				mNetworkStatistic.mCurrentUDPSendOpt --;

			break;
		}

		case SocketOperation::_UDP_RECV:
		{
			if ( mNetworkStatistic.mCurrentUDPRecvOpt > 0 )
				mNetworkStatistic.mCurrentUDPRecvOpt --;

			break;
		}
	}
}

_void NetworkManager::StatisticCreateConnection( )
{
	mNetworkStatistic.mTotalConnectionUsed ++;
	mNetworkStatistic.mCurrentConnectionUsed ++;

	if ( mNetworkStatistic.mCurrentConnectionUsed > mNetworkStatistic.mMaximalConnectionUsed )
		mNetworkStatistic.mMaximalConnectionUsed = mNetworkStatistic.mCurrentConnectionUsed;
}

_void NetworkManager::StatisticDeleteConnection( )
{
	if ( mNetworkStatistic.mCurrentConnectionUsed > 0 )
		mNetworkStatistic.mCurrentConnectionUsed --;
}

_void NetworkManager::StatisticResentPacket( _dword bytes )
{
	mNetworkStatistic.mTotalUDPResendTime	+= 1;
	mNetworkStatistic.mTotalUDPResendBytes	+= bytes;
}

_void NetworkManager::StatisticPacketsInOrder( )
{
	mNetworkStatistic.mUDPPacketsInOrder ++;
}

_void NetworkManager::StatisticPacketsInDisorder( )
{
	mNetworkStatistic.mUDPPacketsInDisorder ++;
}

_void NetworkManager::StatisticPacketsDroped( )
{
	mNetworkStatistic.mUDPPacketsDroped ++;
}

_byte* NetworkManager::GetLastReservedBuffer( ) const
{
	return mLastReservedBuffer;
}

};