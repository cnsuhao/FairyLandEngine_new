//============================================================================
// UDPSocketWrapper.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// UDPSocketWrapper
//----------------------------------------------------------------------------

class UDPSocketWrapper : public IUDPSocket, public ReferencedObject
{
public:
	Lock&			mNetworkLock;
	BufferManager&	mBufferManager;
	UDPSocket*		mUDPSocket;
	_bool			mPollingMode;
	_bool			mPacketHeader;
	_bool			mReleased;
	_dword			mUserData;

public:
	UDPSocketWrapper( UDPSocket* socket, Lock& lock, BufferManager& buffermanager );

	_bool Recv( );

	inline _void Release( )
		{ mReleased = _true; }
	_bool IsReleased( ) const
		{ return mReleased; }

	virtual _void	EnablePollingMode( _bool enable );
	virtual _bool	IsPollingModeEnabled( ) const;

	virtual _void	EnablePacketHeader( _bool enable );
	virtual _bool	IsPacketHeaderEnabled( ) const;

	virtual _void	SetUserData( _dword userdata );
	virtual _dword	GetUserData( ) const;

	virtual _bool	Listen( );
	virtual _bool	Send( const Address& address, const _void* buffer, _dword length );

	virtual Address	GetLocalAddress( );
};

};