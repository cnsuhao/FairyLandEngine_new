//============================================================================
// NetworkHelper.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// NetworkHelper
//----------------------------------------------------------------------------

class NetworkHelper
{
protected:
	enum { _KEEPALIVE_INTERVAL = 10 * 1000 };

	BufferManager&	mBufferManager;

	Address			mHelperServer;
	Address			mHelperPort;
	_dword			mHelperSocketID;
	_dword			mKeepAliveTickcount;

public:
	NetworkHelper( BufferManager& buffermanager );

	inline const Address& GetHelperServer( );
	inline const Address& GetHelperPort( );

	inline _bool IsHelperAvailable( ) const;

	_bool NeedKeepAlive( _dword tickcount );
	_void KeepHelperAlive( _dword socketid );

	_void SetHelperServer( const Address& address );
	_bool OpenHelperPort( const Address& address );
	_void CloseHelperPort( );

	_bool ProcessPacket( _dword socketid, Packet* packet, const Address& address );
};

//----------------------------------------------------------------------------
// NetworkHelper Implementation
//----------------------------------------------------------------------------

const Address& NetworkHelper::GetHelperServer( )
{
	return mHelperServer;
}

const Address& NetworkHelper::GetHelperPort( )
{
	return mHelperPort;
}

_bool NetworkHelper::IsHelperAvailable( ) const
{
	return mHelperServer.mIP != 0 && mHelperServer.mPort != 0 && mHelperServer.mType == Address::_TYPE_UDP;
}

};