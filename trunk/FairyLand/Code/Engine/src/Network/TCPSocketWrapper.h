//============================================================================
// TCPSocketWrapper.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// TCPSocketWrapper
//----------------------------------------------------------------------------

class TCPSocketWrapper : public ITCPSocket, public ReferencedObject
{
public:
	Lock&			mNetworkLock;
	BufferManager&	mBufferManager;
	TCPSocket*		mTCPSocket;
	_bool			mPollingMode;
	_bool			mPacketHeader;
	_bool			mReleased;
	_dword			mUserData;

public:
	TCPSocketWrapper( TCPSocket* socket, Lock& lock, BufferManager& buffermanager );

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

	virtual _bool	Listen( _dword queue );
	virtual _bool	Connect( const Address& address );
	virtual _bool	Send( const _void* buffer, _dword length );

	virtual Address	GetLocalAddress( );
	virtual Address	GetRemoteAddress( );
};

};