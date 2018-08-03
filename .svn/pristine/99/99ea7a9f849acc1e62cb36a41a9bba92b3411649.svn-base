//============================================================================
// INetworkManager.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// NetworkStatistic
//----------------------------------------------------------------------------

struct NetworkStatistic
{
	_dword		mTotalDuration;

	_dword		mCurrentSocketOpt;
	_dword		mMaximalSocketOpt;
	_dword		mCurrentTCPAcceptOpt;
	_dword		mMaximalTCPAcceptOpt;
	_dword		mCurrentTCPConnectOpt;
	_dword		mMaximalTCPConnectOpt;
	_dword		mCurrentTCPSendOpt;
	_dword		mMaximalTCPSendOpt;
	_dword		mCurrentTCPRecvOpt;
	_dword		mMaximalTCPRecvOpt;
	_dword		mCurrentUDPSendOpt;
	_dword		mMaximalUDPSendOpt;
	_dword		mCurrentUDPRecvOpt;
	_dword		mMaximalUDPRecvOpt;

	_dword		mTotalSocketUsed;
	_dword		mCurrentSocketUsed;
	_dword		mMaximalSocketUsed;

	_dword		mTotalConnectionUsed;
	_dword		mCurrentConnectionUsed;
	_dword		mMaximalConnectionUsed;

	_dword		mTotalTCPSendTime;
	_dword		mTotalTCPSendBytes;
	_dword		mTotalTCPReceiveTime;
	_dword		mTotalTCPReceiveBytes;

	_dword		mTotalUDPSendTime;
	_dword		mTotalUDPSendBytes;
	_dword		mTotalUDPResendTime;
	_dword		mTotalUDPResendBytes;
	_dword		mTotalUDPReceiveTime;
	_dword		mTotalUDPReceiveBytes;

	_dword		mUDPPacketsInOrder;
	_dword		mUDPPacketsInDisorder;
	_dword		mUDPPacketsDroped;
};

//----------------------------------------------------------------------------
// INetworkManager
//----------------------------------------------------------------------------

class INetworkManager
{
public:
	typedef _void (*OnTCPSocketAccept)( ITCPSocket* socket );
	typedef _void (*OnTCPSocketConnect)( ITCPSocket* socket, _dword errorcode );
	typedef _void (*OnTCPSocketDisconnect)( ITCPSocket* socket, _dword errorcode );
	typedef _void (*OnTCPSocketReceive)( ITCPSocket* socket, const _void* buffer, _dword length );
	typedef _void (*OnUDPSocketReceive)( IUDPSocket* socket, const Address& address, const _void* buffer, _dword length );

	typedef _void (*OnCreateConnection)( INetworkSession* session, _dword connectionid );
	typedef _void (*OnDeleteConnection)( INetworkSession* session, _dword connectionid );
	typedef _void (*OnReceivePacket)( INetworkSession* session, _dword connectionid, _byte* buffer, _dword size );
	typedef _void (*OnUpdateLatency)( INetworkSession* session, _dword connectionid, _dword latency );
	typedef _void (*OnJoinFailed)( INetworkSession* session, _dword connectionid );
	typedef _void (*OnUpdateSession)( INetworkSession* session );
	typedef _void (*OnNetworkTimer)( _dword tickcount );

	enum _NETWORK_PRIORITY
	{
		_NETWORK_PRIORITY_LOW		= 1,
		_NETWORK_PRIORITY_NORMAL	= 2,
		_NETWORK_PRIORITY_HIGH		= 3,
		_NETWORK_PRIORITY_MANAGED	= 4,
	};

public:
	virtual _void				Release( ) = 0;

	virtual _void				Update( _dword elapse ) = 0;

	virtual _void				SetTCPSocketAcceptCallback( OnTCPSocketAccept funcpointer ) = 0;
	virtual _void				SetTCPSocketConnectCallback( OnTCPSocketConnect funcpointer ) = 0;
	virtual _void				SetTCPSocketDisconnectCallback( OnTCPSocketDisconnect funcpointer ) = 0;
	virtual _void				SetTCPSocketReceiveCallback( OnTCPSocketReceive funcpointer ) = 0;
	virtual _void				SetUDPSocketReceiveCallback( OnUDPSocketReceive funcpointer ) = 0;

	virtual _void				SetCreateConnectionCallback( OnCreateConnection funcpointer, _dword sessionid = 0 ) = 0;
	virtual _void				SetDeleteConnectionCallback( OnDeleteConnection funcpointer, _dword sessionid = 0 ) = 0;
	virtual _void				SetReceivePacketCallback( OnReceivePacket funcpointer, _dword sessionid = 0 ) = 0;
	virtual _void				SetUpdateLatencyCallback( OnUpdateLatency funcpointer, _dword sessionid = 0 ) = 0;
	virtual _void				SetJoinFailedCallback( OnJoinFailed funcpointer, _dword sessionid = 0 ) = 0;
	virtual _void				SetUpdateSessionCallback( OnUpdateSession funcpointer, _dword sessionid = 0 ) = 0;
	virtual _void				SetNetworkTimerCallback( OnNetworkTimer funcpointer ) = 0;

	virtual _void				SetBufferLimit( _dword memorylimit ) = 0;

	virtual Address				CreateAddress( _word type, _word port, _dword adapter = 0 ) = 0;
	virtual Address				CreateAddress( _word type, _dword ip, _word port ) = 0;
	virtual Address				CreateAddress( _word type, StringPtr ipaddress, _word port ) = 0;
	virtual Address				CreateAddress( _word type, StringPtr ipaddress ) = 0;

	virtual ITCPSocket*			CreateTCPSocket( const Address& bindaddress ) = 0;
	virtual ITCPSocket*			SearchTCPSocket( const Address& bindaddress ) = 0;
	virtual _void				ReleaseTCPSocket( ITCPSocket*& socket ) = 0;

	virtual IUDPSocket*			CreateUDPSocket( const Address& bindaddress ) = 0;
	virtual IUDPSocket*			SearchUDPSocket( const Address& bindaddress ) = 0;
	virtual _void				ReleaseUDPSocket( IUDPSocket*& socket ) = 0;

	virtual _dword				GetAvailableSessionID( ) const = 0;
	virtual _dword				GetTotalSessionNumber( ) const = 0;

	virtual INetworkSession*	CreateSession( _dword sessionid, _dword maxconnection ) = 0;
	virtual INetworkSession*	SearchSession( _dword sessionid ) const = 0;
	virtual _void				ReleaseSession( INetworkSession*& networksession ) = 0;

	virtual IFileDownloader*	CreateHTTPDownloader( ) = 0;

	virtual _void				SetNATHelperServer( const Address& address ) = 0;
	virtual _bool				OpenNATHelperPort( const Address& address ) = 0;
	virtual _void				CloseNATHelperPort( ) = 0;

	virtual _void				SetAgentHelperServer( const Address& address ) = 0;
	virtual _bool				OpenAgentHelperPort( const Address& address ) = 0;
	virtual _void				CloseAgentHelperPort( ) = 0;

	virtual Thread&				GetNetworkThread( ) = 0;
	virtual Lock&				GetNetworkLock( ) = 0;

	virtual _void				SetNetworkLatency( _dword latency ) = 0;
	virtual _dword				GetNetworkLatency( ) const = 0;

	virtual const NetworkStatistic& GetNetworkStatistic( ) const = 0;
	virtual _void				DumpNetworkStatistic( ) const = 0;
	virtual _void				ClearNetworkStatistic( ) = 0;
};

};