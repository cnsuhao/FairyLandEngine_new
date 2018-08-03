//============================================================================
// UDPConnection.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// UDPConnection
//----------------------------------------------------------------------------

class UDPConnection : public Connection
{
public:
	enum
	{
		_MIN_CONGESTION			=  1280,
		_MAX_CONGESTION			= 81920,

		_MIN_THRESHOLD			=  2560,
		_MAX_THRESHOLD			= 81920,

		_MIN_CONFIRM_INTERVAL	=    0,
		_AVG_CONFIRM_INTERVAL	=   50,
		_MAX_CONFIRM_INTERVAL	=  100,

		_MIN_RESEND_INTERVAL	=  600,
		_AVG_RESEND_INTERVAL	=  800,
		_MAX_RESEND_INTERVAL	= 1500,
	};

	struct UnconfirmedPacket
	{
		Packet*	mPacket;
		_dword	mPostTickcount;
	};

	typedef Queue< UnconfirmedPacket >	PacketQueue;
	typedef Array< Packet* >			PacketArray;

	UDPSocket*	mUDPSocket;

	_dword		mCongestionWindow;		// cwnd from TCP
	_dword		mSlowStartThreshold;	// ssthresh from TCP
	_dword		mUnconfirmPacketSize;
	_dword		mSendPacketIndex;

	_word		mBadConfirmNumber;
	_word		mLastConfirmSN;
	_word		mNextSendPacketSN;
	_word		mNextRecvPacketSN;

	_bool		mStartFastResend;
	_bool		mStartFastConfirm;
	_dword		mConfirmTickcount;
	_dword		mResendTickcount;
	_dword		mResendNumber;

	PacketQueue	mSendPacketQueue;
	PacketArray	mRecvPacketArray;

	inline _bool PacketNeedConfirm( _byte magiccode, _word packetproperty ) const;
	inline _bool PacketUseful( _word waitingsn, _word packetsn ) const;

	_bool ResendHelper( Packet* packet );
	_bool CheckPacketHelper( Packet* packet, _dword index );
	_bool NeedConfirmBack( ) const;

	_void ConfirmPacketSN( _word packetsn, _bool confirmflag );
	_void CongestionHappen( _bool timeout );
	_void FlushPacketList( _byte*& packetlist, _dword& bufferindex, _dword& packetnumber );
	_void SendConfirmPacket( );
	_void ResendPacketQueue( _dword sendnumber );
	_void CheckResendPacket( );
	_void ClearPacketQueue( );

public:
	UDPConnection( _dword socketid, const Address& localaddress, const Address& remoteaddress, BufferManager& buffermanager );
	virtual ~UDPConnection( );

	virtual _dword GetConnectionType( );

	virtual _bool Connect( );
	virtual _void Close( );

	virtual _bool Send( SocketOperation* operation );
	virtual _bool Recv( );
	virtual _bool ProcessData( _byte* buffer, _dword length );

	virtual _bool ProcessPacket( Packet* packet );
	virtual _bool KeepAlive( _dword tickcount );
	virtual _void FlushFinish( );
};

_bool UDPConnection::PacketNeedConfirm( _byte magiccode, _word packetproperty ) const
{
	if ( magiccode == JoinHintPacket::_MAGIC_CODE || magiccode == CloseReqPacket::_MAGIC_CODE || magiccode == ConfirmSNPacket::_MAGIC_CODE )
		return _false;

	if ( ( mProperty & _PROPERTY_RELIABLEPING ) == 0 )
	{
		if ( magiccode == PingReqPacket::_MAGIC_CODE || magiccode == PingAckPacket::_MAGIC_CODE )
			return _false;
	}

	// This packet no need confirmed.
	if ( packetproperty & _PROPERTY_NONEEDCONFIRM )
		return _false;

	return _true;
}

_bool UDPConnection::PacketUseful( _word waitingsn, _word packetsn ) const
{
	if ( packetsn > waitingsn && (_dword) packetsn - (_dword) waitingsn < 0x0FFF )
		return _true;

	if ( packetsn < waitingsn && 0x10000 + (_dword) packetsn - (_dword) waitingsn < 0x0FFF )
		return _true;

	return _false;
}

};