//============================================================================
// BufferManager.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// PacketBuffer
//----------------------------------------------------------------------------

struct PacketBuffer
{
	enum { _MAX_BUFFER_NUMBER = 4 };

	_void*		mBuffer[ _MAX_BUFFER_NUMBER ];
	_dword		mLength[ _MAX_BUFFER_NUMBER ];

	inline PacketBuffer( )
	{
		mBuffer[0] = mBuffer[1] = mBuffer[2] = mBuffer[3] = _null;
		mLength[0] = mLength[1] = mLength[2] = mLength[3] = 0;
	}

	inline _dword Length( ) const
	{
		return mLength[0] + mLength[1] + mLength[2] + mLength[3];
	}
};

//----------------------------------------------------------------------------
// BufferAllocation
//----------------------------------------------------------------------------

struct BufferAllocation
{
	_dword		mFlag;
	_byte*		mBuffer;
	_dword		mSize;
	_dword		mRefCount;
};

//----------------------------------------------------------------------------
// BufferCheck
//----------------------------------------------------------------------------

struct BufferCheck
{
	_dword		mCheckToken[4];
};

//----------------------------------------------------------------------------
// BufferManager
//----------------------------------------------------------------------------

class BufferManager
{
private:
	enum { _BUFFER_FLAG = 0x4642544E };

	typedef Link< BufferAllocation > BufferLink;
	typedef Link< SocketOperation* > OperationLink;

	BufferLink		mUsingBufferLink;
	BufferLink		mFreeBufferLink;

	OperationLink	mUsingOperationLink;
	OperationLink	mFreeOperationLink;

	_dword			mAllocBufferSize;
	_dword			mTotalBufferSize;
	_dword			mTotalBufferNumber;
	_dword			mTotalOperationNumber;

	_dword			mMaxPacketSize;
	_dword			mMaxBufferLimit;
	_bool			mEnableBufferLimit;

public:
	BufferManager( );
	~BufferManager( );

	inline _dword	GetMaxPacketSize( ) const;
	inline _void	SetMaxBufferLimit( _dword limit );
	inline _dword	GetMaxBufferLimit( ) const;

	_byte*	CreateBuffer( );
	_byte*	CreateBuffer( _dword size );
	_void	ReserveBuffer( _byte* buffer );
	_void	ReleaseBuffer( _byte* buffer );

	_void	GetBufferToken( _byte* buffer, BufferCheck& check );
	_void	CheckBuffer( _byte* buffer, const BufferCheck& check );

	SocketOperation* CreateOperationHelper( _void* buffer, _dword length );
	SocketOperation* CreateOperation( );
	SocketOperation* CreateOperation( const PacketBuffer& buffer, _bool copybuffer = _true );
	_void	ReleaseOperation( SocketOperation*& operation );
};

//----------------------------------------------------------------------------
// BufferManager Implementation
//----------------------------------------------------------------------------

_dword BufferManager::GetMaxPacketSize( ) const
{
	return mMaxPacketSize;
}

_void BufferManager::SetMaxBufferLimit( _dword limit )
{
	mMaxBufferLimit = limit;
}

_dword BufferManager::GetMaxBufferLimit( ) const
{
	return mMaxBufferLimit;
}

};