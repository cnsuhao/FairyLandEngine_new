//============================================================================
// INetworkSession.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// INetworkSession
//----------------------------------------------------------------------------

class INetworkSession
{
public:
	enum _CONNECTION_PHASE
	{
		_PHASE_UNKNOWN			= 0,	// TCP/UDP, bad connection.
		_PHASE_CONNECTING		= 1,	// TCP,     when sent join req  -> _PHASE_SENTJOINREQ, need inform app if failed.
		_PHASE_WAITJOINREQ		= 2,	// TCP/UDP, when recv join req  -> _PHASE_RECVJOINREQ
		_PHASE_SENTJOINREQ		= 3,	// TCP/UDP, when recv join ack  -> _PHASE_CONNECTED, need inform app if failed.
		_PHASE_RECVJOINREQ		= 4,	// TCP/UDP, when recv join ack  -> _PHASE_CONNECTED
		_PHASE_CONNECTED		= 5,	// TCP/UDP, when sent close req -> _PHASE_INITCLOSE, when recv close req - > _PHASE_PASVCLOSE.
		_PHASE_INIVCLOSE		= 6,	// TCP/UDP, the connection will be deleted
		_PHASE_PASVCLOSE		= 7,	// TCP/UDP, the connection will be deleted
		_PHASE_CLOSED			= 8,	// TCP/UDP, the connection is already close in logic, but resource is not freed.
	};

	enum _CLOSE_REASON
	{
		_REASON_UNKNOWN			= 0,
		_REASON_TIMEOUT			= 1,
		_REASON_LOCALCLOSE		= 2,
		_REASON_REMOTECLOSE		= 3,
		_REASON_INVALIDPACKET	= 4,
		_REASON_SENDERROR		= 5,
		_REASON_RECVERROR		= 6,
	};

public:
	virtual _bool		OpenPort( const Address& localaddress, _dword groupid = 0 ) = 0;
	virtual _void		ClosePort( const Address& localaddress ) = 0;
	virtual _void		ClosePort( _dword groupid ) = 0;

	virtual _bool		IsPortOpened( const Address& localaddress, _dword& groupid ) = 0;
	virtual _bool		IsPortOpened( _dword groupid, Address& localaddress ) = 0;

	virtual _bool		PingSession( const Address& remoteaddress, _dword groupid = 0, _dword userdata = 0, _dword sessionid = 0 ) = 0;
	virtual _bool		PingSession( const Address& localaddress, const Address& remoteaddress, _dword groupid = 0, _dword userdata = 0, _dword sessionid = 0 ) = 0;

	virtual _bool		JoinSession( const Address& remoteaddress, _dword groupid = 0, _dword userdata = 0, _dword sessionid = 0 ) = 0;
	virtual _bool		JoinSession( const Address& localaddress, const Address& remoteaddress, _dword groupid = 0, _dword userdata = 0, _dword sessionid = 0 ) = 0;

	virtual _dword		GetSessionID( ) const = 0;
	virtual _dword		GetSessionGlobalToken( ) const = 0;
	virtual _dword		GetNewConnectionID( ) const = 0;

	virtual _void		EnablePollingMode( _bool enable ) = 0;
	virtual _bool		IsPollingModeEnabled( ) const = 0;

	virtual _void		SetPriority( _dword priority ) = 0;
	virtual _dword		GetPriority( ) const = 0;

	virtual _void		SetUserData( _dword userdata ) = 0;
	virtual _dword		GetUserData( ) const = 0;

	virtual _void		SetFlushInterval( _dword interval ) = 0;
	virtual _dword		GetFlushInterval( ) const = 0;

	virtual _dword		GetAliveConnectionNumber( ) const = 0;
	virtual _dword		GetSessionConnectionNumber( ) const = 0;
	virtual _dword		GetSessionFirstConnectionID( ) const = 0;
	virtual _dword		GetSessionNextConnectionID( _dword previousid ) const = 0;
	virtual _dword		GetGroupConnectionNumber( _dword groupid ) const = 0;
	virtual _dword		GetGroupFirstConnectionID( _dword groupid ) const = 0;
	virtual _dword		GetGroupNextConnectionID( _dword groupid, _dword previousid ) const = 0;

	virtual _void		SetGroupUserData( _dword groupid, _dword userdata ) = 0;
	virtual _dword		GetGroupUserData( _dword groupid ) const = 0;

	virtual _void		SetConnectionGroup( _dword connectionid, _dword groupid ) = 0;
	virtual _dword  	GetConnectionGroup( _dword connectionid ) const = 0;
	virtual _void		SetConnectionUserData( _dword connectionid, _dword userdata ) = 0;
	virtual _dword  	GetConnectionUserData( _dword connectionid ) const = 0;
	virtual _void		SetConnectionTimer( _dword connectionid, _dword timer ) = 0;
	virtual _dword  	GetConnectionTimer( _dword connectionid ) const = 0;
	virtual _void		SetConnectionTimeout( _dword connectionid, _dword timeout ) = 0;
	virtual _dword  	GetConnectionTimeout( _dword connectionid ) const = 0;
	virtual _void		SetConnectionConnectInterval( _dword connectionid, _dword interval ) = 0;
	virtual _dword  	GetConnectionConnectInterval( _dword connectionid ) const = 0;
	virtual _void		SetConnectionPriority( _dword connectionid, _dword priority ) = 0;
	virtual _dword  	GetConnectionPriority( _dword connectionid ) const = 0;
	virtual _dword  	GetConnectionGlobalToken( _dword connectionid ) const = 0;
	virtual _dword  	GetConnectionLatency( _dword connectionid ) const = 0;
	virtual _dword  	GetConnectionDuration( _dword connectionid ) const = 0;
	virtual _dword  	GetConnectionPhase( _dword connectionid ) const = 0;
	virtual _dword  	GetConnectionSentBytes( _dword connectionid ) const = 0;
	virtual _dword		GetConnectionRecentSentBytes( _dword connectionid ) const = 0;
	virtual _float  	GetConnectionSendSpeed( _dword connectionid ) const = 0;
	virtual _dword  	GetConnectionReceivedBytes( _dword connectionid ) const = 0;
	virtual _dword		GetConnectionRecentReceivedBytes( _dword connectionid ) const = 0;
	virtual _float  	GetConnectionReceiveSpeed( _dword connectionid ) const = 0;
	virtual _dword  	GetConnectionCloseReason( _dword connectionid ) const = 0;
	virtual StringPtr	GetConnectionCloseReasonString( _dword connectionid ) const = 0;
	virtual Address 	GetConnectionLocalAddress( _dword connectionid ) const = 0;
	virtual Address 	GetConnectionRemoteAddress( _dword connectionid ) const = 0;
	virtual _dword		GetConnectionRemoteSessionID( _dword connectionid ) const = 0;
	virtual _bool		IsLANConnection( _dword connectionid ) const = 0;

	virtual _bool		SetConnectionBackupAddress( _dword connectionid, const Address& remoteaddress ) = 0;
	virtual _dword		SearchConnection( const Address& remoteaddress, _dword groupid = 0 ) const = 0;

	virtual _void		EnableSessionTickcount( _bool enable ) = 0;
	virtual _bool		IsSessionTickcountEnabled( ) const = 0;
	virtual _void		InitSessionTickcount( _dword tickcount ) = 0;
	virtual _dword		GetSessionTickcount( ) const = 0;
	virtual _dword		GetPacketTickcount( _dword connectionid ) const = 0;

	virtual _void		SetPacketReference( _dword connectionid, _dword referenceid ) = 0;
	virtual _dword		GetPacketReference( _dword connectionid ) const = 0;

	virtual _void		SetReferenceTickcount( _dword referenceid, _dword sessiontickcount, _dword duration = 10 * 1000 ) = 0;
	virtual _dword		GetReferenceTickcount( _dword referenceid ) const = 0;

	virtual _bool		SetDelayTickcount( _dword connectionid, _dword channel, _dword tickcount, _dword referenceid = 0 ) = 0;
	virtual _bool		SetGroupDelayTickcount( _dword groupid, _dword excludeconnection, _dword channel, _dword tickcount, _dword referenceid = 0 ) = 0;
	virtual _bool		SetSessionDelayTickcount( _dword excludegroup, _dword excludeconnection, _dword channel, _dword tickcount, _dword referenceid = 0 ) = 0;

	virtual _bool		SendPacket( _dword connectionid, const _void* buffer, _dword length ) = 0;
	virtual _bool		SendDeferPacket( _dword connectionid, const _void* buffer, _dword length ) = 0;
	virtual _bool		FlushDeferPacket( _dword connectionid ) = 0;

	virtual _bool		SendGroupPacket( _dword groupid, _dword excludeconnection, const _void* buffer, _dword length ) = 0;
	virtual _bool		SendGroupDeferPacket( _dword groupid, _dword excludeconnection, const _void* buffer, _dword length ) = 0;
	virtual _bool		FlushGroupDeferPacket( _dword groupid, _dword excludeconnection ) = 0;

	virtual _bool		SendSessionPacket( _dword excludegroup, _dword excludeconnection, const _void* buffer, _dword length ) = 0;
	virtual _bool		SendSessionDeferPacket( _dword excludegroup, _dword excludeconnection, const _void* buffer, _dword length ) = 0;
	virtual _bool		FlushSessionDeferPacket( _dword excludegroup, _dword excludeconnection ) = 0;

	virtual _bool		EnableAutoReconnect( _dword connectionid, _bool enable ) = 0;
	virtual _bool		EnableCompressPacket( _dword connectionid, _bool enable ) = 0;
	virtual _bool		EnableResendPacketList( _dword connectionid, _bool enable ) = 0;
	virtual _bool		EnableNoNeedConfirmPacket( _dword connectionid, _bool enable ) = 0;
	virtual _bool		SyncConnectionProperty( _dword connectionid ) = 0;

	virtual _void		CloseConnection( _dword connectionid, _bool needcallback = _true ) = 0;
	virtual _void		CloseGroup( _dword groupid, _bool needcallback = _true ) = 0;
	virtual _void		CloseSession( _bool needcallback = _true ) = 0;

	virtual _dword		GetRecentSendTime( _dword seconds = 10 ) const = 0;
	virtual _dword		GetRecentSendBytes( _dword seconds = 10 ) const = 0;
	virtual _dword		GetRecentReceiveTime( _dword seconds = 10 ) const = 0;
	virtual _dword		GetRecentReceiveBytes( _dword seconds = 10 ) const = 0;

	virtual INetworkStatistics*	GetStatistics( ) const = 0;
};

};