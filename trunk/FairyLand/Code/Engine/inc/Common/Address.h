//============================================================================
// Address.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Address
//----------------------------------------------------------------------------

#pragma pack( push, 1 )

struct Address
{
	enum _PORT_TYPE
	{
		_TYPE_TCP = 1,
		_TYPE_UDP = 2,
	};

	union
	{
		struct
		{
			_dword	mIP;
		};

		struct
		{
			_byte	mIP1;
			_byte	mIP2;
			_byte	mIP3;
			_byte	mIP4;
		};
	};

	_word	mType;
	_word	mPort;

	Address( )
		: mIP( 0 ), mType( 0 ), mPort( 0 ) { }

	Address( _word type )
		: mIP( 0 ), mType( type ), mPort( 0 ) { }

	Address( _dword ip, _word type, _word port )
		: mIP( ip ), mType( type ), mPort( port ) { }

	_bool operator == ( const Address& address ) const
		{ return mIP == address.mIP && mType == address.mType && mPort == address.mPort; }

	_bool operator != ( const Address& address ) const
		{ return mIP != address.mIP || mType != address.mType || mPort != address.mPort; }

	_bool operator < ( const Address &address ) const
		{ return mIP != address.mIP ? mIP < address.mIP : ( mType != address.mType ? mType < address.mType : mPort < address.mPort );}

	_bool operator > ( const Address &address ) const
		{ return address < *this; }
};

//----------------------------------------------------------------------------
// SessionAddress
//----------------------------------------------------------------------------

struct SessionAddress
{
	Address	mAddress;
	_word	mSessionID;
	_word	mReserved;

	SessionAddress( )
		: mSessionID( 0 ), mReserved( 0 ) { }

	SessionAddress( const Address& address, _dword sessionid )
		: mAddress( address ), mSessionID( (_word) sessionid ), mReserved( 0 ) { }

	_bool operator == ( const SessionAddress& address ) const
		{ return mAddress == address.mAddress && mSessionID == address.mSessionID && mReserved == address.mReserved; }

	_bool operator != ( const SessionAddress& address ) const
		{ return mAddress != address.mAddress || mSessionID != address.mSessionID || mReserved != address.mReserved; }
};

//----------------------------------------------------------------------------
// P2PAddress
//----------------------------------------------------------------------------
class P2PAddress
{
public:
	P2PAddress():mSessionId(0) {}
	P2PAddress(const Address &pub, const Address &pri, _dword session)
		:mPublicAddr(pub),mPrivateAddr(pri),mSessionId(session) {}

    const _qword& Id() const 
		{ return *reinterpret_cast<const _qword*>(&mPublicAddr); }
    const Address& PublicAddr() const { return mPublicAddr; }
    const Address& PrivateAddr() const { return mPrivateAddr; }
	_dword Session() const { return mSessionId; }

    _void SetPrivateAddr(const Address &pri) { mPrivateAddr = pri; }
	_bool IsSameLan(const P2PAddress &addr) const
		{ return mPublicAddr.mIP == addr.mPublicAddr.mIP; }

    P2PAddress Clone( _word t ) const 
        { P2PAddress temp = *this; temp.mPublicAddr.mType = temp.mPrivateAddr.mType = t; return temp; }

private:
	Address mPublicAddr;
	Address mPrivateAddr;
	_dword	mSessionId;
};

//----------------------------------------------------------------------------
// PeerAddr: This class was abandoned, keep this declare here just for
//			 compatibility, should not use this class in any new codes.
//----------------------------------------------------------------------------
class PeerAddr
{
public:
	PeerAddr():mId(0),mSessionId(0) {}
	PeerAddr(const _qword &id, const Address &addr, _dword sessionid)
		:mId(id), mAddr(addr), mSessionId(sessionid) {}
	explicit PeerAddr(const P2PAddress &addr)
		:mId(addr.Id()),mAddr(addr.PublicAddr()),mSessionId(addr.Session()) {}

	operator _qword() const { return mId; }
	const _qword& Id() const { return mId; }
	const Address& Addr() const { return mAddr; }
	_dword SessionId() const { return mSessionId; }
	P2PAddress ToP2PAddress() const { return P2PAddress(mAddr, mAddr, mSessionId); }

private:
	_qword	mId;
	Address	mAddr;
	_dword	mSessionId;
};

#pragma pack(pop)

};