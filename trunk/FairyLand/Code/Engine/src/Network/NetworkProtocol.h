//============================================================================
// Protocol.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

#pragma pack( push, 1 )

//----------------------------------------------------------------------------
// Packet
//----------------------------------------------------------------------------

struct Packet
{
	enum { _PACKET_VERSION = 0x02 };
	enum { _MAGIC_CODE = 0xAD };

	enum _PACKET_FLAG
	{
		_PACKET_NORMAL			= 0,
		_PACKET_LIST			= 1,

		_PACKET_RESERVED1		= 2,
		_PACKET_RESERVED2		= 3,

		_PACKET_BIGPACKETBEGIN	= 4,
		_PACKET_BIGPACKETDATA	= 5,

		_PACKET_LISTWITHSN		= 6,
	};

	_byte	mVersion;
	_byte	mMagicCode;
	_byte	mCompressFlag;
	_byte	mPacketFlag;

	_word	mSecretCode;
	_word	mSessionID;
	_word	mConnectionID;
	_word	mPacketProperty;

	_dword	mReserved;

	_byte	mConfirmFlag;
	_byte	mEncryptFlag;
	_word	mPacketSize;
	_word	mPacketSN;
	_word	mConfirmSN;

	Packet( _word packetsize )
		: mVersion( _PACKET_VERSION ), mMagicCode( _MAGIC_CODE ), mCompressFlag( 0 ), mPacketFlag( 0 ), mSecretCode( 0 ), mSessionID( 0 ), mConnectionID( 0 ), mPacketProperty( 0 ), mReserved( 0 ), mConfirmFlag( 0 ), mEncryptFlag( 0 ), mPacketSize( packetsize ), mPacketSN( 0 ), mConfirmSN( 0 ) { }

	Packet( _byte magiccode, _word packetsize )
		: mVersion( _PACKET_VERSION ), mMagicCode( magiccode ), mCompressFlag( 0 ), mPacketFlag( 0 ), mSecretCode( 0 ), mSessionID( 0 ), mConnectionID( 0 ), mPacketProperty( 0 ), mReserved( 0 ), mConfirmFlag( 0 ), mEncryptFlag( 0 ), mPacketSize( packetsize ), mPacketSN( 0 ), mConfirmSN( 0 ) { }

	Packet( _byte magiccode, _word sessionid, _word packetsize )
		: mVersion( _PACKET_VERSION ), mMagicCode( magiccode ), mCompressFlag( 0 ), mPacketFlag( 0 ), mSecretCode( 0 ), mSessionID( sessionid ), mConnectionID( 0 ), mPacketProperty( 0 ), mReserved( 0 ), mConfirmFlag( 0 ), mEncryptFlag( 0 ), mPacketSize( packetsize ), mPacketSN( 0 ), mConfirmSN( 0 ) { }

	_bool CanBeCombined( ) const
		{ return mMagicCode == Packet::_MAGIC_CODE && ( mPacketFlag == Packet::_PACKET_NORMAL || mPacketFlag == Packet::_PACKET_LIST ); }
};

//----------------------------------------------------------------------------
// RawPacket
//----------------------------------------------------------------------------

struct RawPacket : public Packet
{
	enum { _MAGIC_CODE = 0x8A };

	RawPacket( _word packetsize )
		: Packet( (_byte) _MAGIC_CODE, packetsize ) { }
};

//----------------------------------------------------------------------------
// PacketDivision
//----------------------------------------------------------------------------

struct PacketDivision
{
	_dword	mPacketSize;

	PacketDivision( _dword packetsize )
		: mPacketSize( packetsize ) { }
};

//----------------------------------------------------------------------------
// PacketEnding
//----------------------------------------------------------------------------

struct PacketEnding
{
	_word	mPacketNumber;
	_word	mPacketFlag;

	PacketEnding( _word packetnumber, _word packetflag )
		: mPacketNumber( packetnumber ), mPacketFlag( packetflag ) { }
};

//----------------------------------------------------------------------------
// PacketTickcount
//----------------------------------------------------------------------------

struct PacketTickcount
{
	_dword	mChannel;
	_dword	mSendTickcount;
	_dword	mDelayTickcount;
	_dword	mDelayReference;
	_dword	mPacketReference;
	_dword	mReserved;

	PacketTickcount( ) { }

	PacketTickcount( _dword channel, _dword sendtickcount, _dword delaytickcount, _dword delayreference, _dword packetreference )
		: mChannel( channel ), mSendTickcount( sendtickcount ), mDelayTickcount( delaytickcount ), mDelayReference( delayreference ), mPacketReference( packetreference ), mReserved( 0 ) { }
};

//----------------------------------------------------------------------------
// BigPacketBegin
//----------------------------------------------------------------------------

struct BigPacketBegin
{
	_dword	mPacketSize;
	_dword	mPacketNumber;

	BigPacketBegin( _dword packetsize, _dword packetnumber )
		: mPacketSize( packetsize ), mPacketNumber( packetnumber ) { }
};

//----------------------------------------------------------------------------
// KeepAlivePacket
//----------------------------------------------------------------------------

struct KeepAlivePacket : public Packet
{
	enum { _MAGIC_CODE = 0xE1 };

	KeepAlivePacket( )
		: Packet( _MAGIC_CODE, sizeof( KeepAlivePacket ) ) { }
};

//----------------------------------------------------------------------------
// ConnectReqPacket
//----------------------------------------------------------------------------

struct ConnectReqPacket : public Packet
{
	enum { _MAGIC_CODE = 0xE2 };

	Address	mSourcePrivateAddr;
	Address	mTargetPublicAddr;

	ConnectReqPacket( _word sessionid, const Address& sourceprivateaddr, const Address& targetpublicaddr )
		: Packet( _MAGIC_CODE, sessionid, sizeof( ConnectReqPacket ) ), mSourcePrivateAddr( sourceprivateaddr ), mTargetPublicAddr( targetpublicaddr ) { }
};

//----------------------------------------------------------------------------
// ConnectHintPacket
//----------------------------------------------------------------------------

struct ConnectHintPacket : public Packet
{
	enum { _MAGIC_CODE = 0xE3 };

	Address	mSourcePrivateAddr;
	Address	mSourcePublicAddr;
	Address	mTargetPublicAddr;

	ConnectHintPacket( _word sessionid, const Address& sourceprivateaddr, const Address& sourcepublicaddr, const Address& targetpublicaddr )
		: Packet( _MAGIC_CODE, sessionid, sizeof( ConnectHintPacket ) ), mSourcePrivateAddr( sourceprivateaddr ), mSourcePublicAddr( sourcepublicaddr ), mTargetPublicAddr( targetpublicaddr ) { }
};

//----------------------------------------------------------------------------
// JoinHintPacket
//----------------------------------------------------------------------------

struct JoinHintPacket : public Packet
{
	enum { _MAGIC_CODE = 0x61 };

	Address	mTargetAddr;

	JoinHintPacket( _word sessionid, const Address& targetaddr )
		: Packet( _MAGIC_CODE, sessionid, sizeof( JoinHintPacket ) ), mTargetAddr( targetaddr ) { }
};

//----------------------------------------------------------------------------
// JoinReqPacket
//----------------------------------------------------------------------------

struct JoinReqPacket : public Packet
{
	enum { _MAGIC_CODE = 0x62 };

	// Local session ID.
	_word	mMySessionID;
	// Local connection ID.
	_word	mMyConnectionID;
	// Local tickcount.
	_dword	mMyTickcount;
	// Reserved field.
	_dword	mReserved;

	JoinReqPacket( _word mysessionid, _word myconnectionid )
		: Packet( _MAGIC_CODE, sizeof( JoinReqPacket ) ), mMySessionID( mysessionid ), mMyConnectionID( myconnectionid ), mMyTickcount( System::GetCurrentTickcount( ) ), mReserved( 0 ) { }
};

//----------------------------------------------------------------------------
// JoinAckPacket
//----------------------------------------------------------------------------

struct JoinAckPacket : public Packet
{
	enum { _MAGIC_CODE = 0x63 };

	// Local session ID, 0 means failed, other values means succeeded.
	_word	mMySessionID;
	// Local connection ID.
	_word	mMyConnectionID;
	// Local tickcount.
	_dword	mMyTickcount;
	// Remote tickcount.
	_dword	mYourTickcount;
	// Session tickcount.
	_dword	mSessionTickcount;

	JoinAckPacket( _word mysessionid, _word myconnectionid, _dword tickcount )
		: Packet( _MAGIC_CODE, sizeof( JoinAckPacket ) ), mMySessionID( mysessionid ), mMyConnectionID( myconnectionid ), mMyTickcount( System::GetCurrentTickcount( ) ), mYourTickcount( tickcount ), mSessionTickcount( 0 ) { }
};

//----------------------------------------------------------------------------
// CloseReqPacket
//----------------------------------------------------------------------------

struct CloseReqPacket : public Packet
{
	enum { _MAGIC_CODE = 0x64 };

	_dword	mReserved;

	CloseReqPacket( )
		: Packet( _MAGIC_CODE, sizeof( CloseReqPacket ) ), mReserved( 0 ) { }
};

//----------------------------------------------------------------------------
// CloseAckPacket
//----------------------------------------------------------------------------

struct CloseAckPacket : public Packet
{
	enum { _MAGIC_CODE = 0x65 };

	_dword	mReserved;

	CloseAckPacket( )
		: Packet( _MAGIC_CODE, sizeof( CloseAckPacket ) ), mReserved( 0 ) { }
};

//----------------------------------------------------------------------------
// ConnectionPropertyPacket
//----------------------------------------------------------------------------

struct ConnectionPropertyPacket : public Packet
{
	enum { _MAGIC_CODE = 0x66 };

	_dword	mTimeout;
	_dword	mPriority;
	_dword	mReserved[8];

	ConnectionPropertyPacket( _dword timeout, _dword priority )
		: Packet( _MAGIC_CODE, sizeof( ConnectionPropertyPacket ) ), mTimeout( timeout ), mPriority( priority ) { Memory::MemSet( mReserved, 0, sizeof( mReserved ) ); }
};

//----------------------------------------------------------------------------
// PingReqPacket
//----------------------------------------------------------------------------

struct PingReqPacket : public Packet
{
	enum { _MAGIC_CODE = 0xF1 };

	_dword	mTickcount;
	_dword	mLocalLatency;
	_dword	mReserved[4];

	PingReqPacket( _dword latency )
		: Packet( _MAGIC_CODE, sizeof( PingReqPacket ) ), mTickcount( System::GetCurrentTickcount( ) ), mLocalLatency( latency ) { Memory::MemSet( mReserved, 0, sizeof( mReserved ) ); }
};

//----------------------------------------------------------------------------
// PingAckPacket
//----------------------------------------------------------------------------

struct PingAckPacket : public Packet
{
	enum { _MAGIC_CODE = 0xF2 };

	_dword	mTickcount;
	_dword	mServerTickcount;
	_dword	mReserved[3];

	PingAckPacket( _dword tickcount )
		: Packet( _MAGIC_CODE, sizeof( PingAckPacket ) ), mTickcount( tickcount ), mServerTickcount( 0 ) { Memory::MemSet( mReserved, 0, sizeof( mReserved ) ); }
};

//----------------------------------------------------------------------------
// ConfirmSNPacket
//----------------------------------------------------------------------------

struct ConfirmSNPacket : public Packet
{
	enum { _MAGIC_CODE = 0xF3 };

	_dword	mReserved;

	ConfirmSNPacket( )
		: Packet( _MAGIC_CODE, sizeof( ConfirmSNPacket ) ), mReserved( 0 ) { mConfirmFlag = 1; }
};

#pragma pack( pop )

};
