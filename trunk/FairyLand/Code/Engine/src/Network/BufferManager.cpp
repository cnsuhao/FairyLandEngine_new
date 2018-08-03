//============================================================================
// BufferManager.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// BufferManager Implementation
//----------------------------------------------------------------------------

BufferManager::BufferManager( )
{
	mAllocBufferSize		= 0;
	mTotalBufferSize		= 0;
	mTotalBufferNumber		= 0;
	mTotalOperationNumber	= 0;

	mMaxPacketSize			= 1024 + 256;
	mMaxBufferLimit			= -1;
	mEnableBufferLimit		= _false;
}

BufferManager::~BufferManager( )
{
	for ( OperationLink::Iterator it = mFreeOperationLink.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		SocketOperation* operation = it;

		delete[] ( (_byte*) operation - sizeof( OperationLink::Node ) );
	}

	for ( OperationLink::Iterator it = mUsingOperationLink.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		SocketOperation* operation = it;

		delete[] ( (_byte*) operation - sizeof( OperationLink::Node ) );
	}

	for ( BufferLink::Iterator it = mFreeBufferLink.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		BufferAllocation& buffer = it;

		delete[] ( buffer.mBuffer - sizeof( BufferLink::Node ) );
	}

	for ( BufferLink::Iterator it = mUsingBufferLink.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		BufferAllocation& buffer = it;

		delete[] ( buffer.mBuffer - sizeof( BufferLink::Node ) );
	}
}

_byte* BufferManager::CreateBuffer( )
{
	return CreateBuffer( mMaxPacketSize );
}

_byte* BufferManager::CreateBuffer( _dword size )
{
	mTotalBufferNumber ++;

	BufferLink::Iterator it = mFreeBufferLink.GetHeadIterator( );

	// Try to use cached buffer.
	if ( it.Valid( ) )
	{
		BufferAllocation& buffer = it;

		FL_ASSERT( buffer.mFlag == _BUFFER_FLAG )
		FL_ASSERT( buffer.mRefCount == 0 )

		// This buffer is big enough.
		if ( buffer.mSize >= size )
		{
			// Every buffer has a header.
			BufferLink::Node* linknode = (BufferLink::Node*) ( buffer.mBuffer - sizeof( BufferLink::Node ) );

			// Remove from free buffer link.
			mFreeBufferLink.Remove( linknode );

			// Put into using buffer link.
			mUsingBufferLink.InsertTail( linknode );

			buffer.mRefCount = 1;

			mTotalBufferSize += buffer.mSize;

			return buffer.mBuffer;
		}
	}

	if ( mAllocBufferSize > mMaxBufferLimit && mEnableBufferLimit )
		return _null;

	// Use max buffer size.
	size = Math::Max( size, mMaxPacketSize );

	// Create a buffer, with a buffer header.
	BufferLink::Node* linknode = (BufferLink::Node*) ( new _byte[ sizeof( BufferLink::Node ) + size ] );

	linknode->mElement.mFlag		= _BUFFER_FLAG;
	linknode->mElement.mBuffer		= (_byte*) linknode + sizeof( BufferLink::Node );
	linknode->mElement.mSize		= size;
	linknode->mElement.mRefCount	= 1;

	// Put into using buffer link.
	linknode->mPrev = _null;
	linknode->mNext = _null;
	mUsingBufferLink.InsertTail( linknode );

	mAllocBufferSize += linknode->mElement.mSize;
	mTotalBufferSize += linknode->mElement.mSize;

	return linknode->mElement.mBuffer;
}

_void BufferManager::ReserveBuffer( _byte* buffer )
{
	// Every buffer has a header.
	BufferLink::Node* linknode = (BufferLink::Node*) ( buffer - sizeof( BufferLink::Node ) );

	FL_ASSERT( linknode->mElement.mFlag == _BUFFER_FLAG )
	FL_ASSERT( linknode->mElement.mRefCount > 0 )

	// Increase buffer reference count.
	linknode->mElement.mRefCount ++;
}

_void BufferManager::ReleaseBuffer( _byte* buffer )
{
	// Every buffer has a header.
	BufferLink::Node* linknode = (BufferLink::Node*) ( buffer - sizeof( BufferLink::Node ) );

	FL_ASSERT( linknode->mElement.mFlag == _BUFFER_FLAG )

	if ( linknode->mElement.mRefCount == 0 )
		return;

	// Decrease buffer reference count.
	linknode->mElement.mRefCount --;

	if ( linknode->mElement.mRefCount != 0 )
		return;

	// Remove from current using buffer link.
	mUsingBufferLink.Remove( linknode );

	FL_ASSERT( mTotalBufferNumber > 0 )
	mTotalBufferNumber --;

	FL_ASSERT( mTotalBufferSize >= linknode->mElement.mSize )
	mTotalBufferSize -= linknode->mElement.mSize;

	// The buffer is too big, we dont need to cache it.
	if ( linknode->mElement.mSize > mMaxPacketSize || mFreeBufferLink.Number( ) >= 2048 )
	{
		FL_ASSERT( mAllocBufferSize >= linknode->mElement.mSize )
		mAllocBufferSize -= linknode->mElement.mSize;

		delete[] (_byte*) linknode;
	}
	// We cache the buffer for later use.
	else
	{
		linknode->mPrev = _null;
		linknode->mNext = _null;

		mFreeBufferLink.InsertTail( linknode );
	}
}

_void BufferManager::GetBufferToken( _byte* buffer, BufferCheck& check )
{
	// Every buffer has a header.
	BufferLink::Node* linknode = (BufferLink::Node*) ( buffer - sizeof( BufferLink::Node ) );

	Memory::MemCpy( &check, linknode, sizeof( BufferCheck ) ); 
}

_void BufferManager::CheckBuffer( _byte* buffer, const BufferCheck& check )
{
	// Every buffer has a header.
	BufferLink::Node* linknode = (BufferLink::Node*) ( buffer - sizeof( BufferLink::Node ) );

	PS_VERIFY( Memory::MemCmp( &check, linknode, sizeof( BufferCheck ) ) )
}

SocketOperation* BufferManager::CreateOperationHelper( _void* buffer, _dword length )
{
	OperationLink::Node* linknode = _null;
	SocketOperation* operation = _null;

	OperationLink::Iterator it = mFreeOperationLink.GetTailIterator( );

	// Try to use cached operation.
	if ( it.Valid( ) )
	{
		operation = it;

		// Every operation has a header.
		linknode = (OperationLink::Node*) ( (_byte*) operation - sizeof( OperationLink::Node ) );

		mFreeOperationLink.Remove( linknode );
	}
	// Create new operation.
	else
	{
		// Create an new operation with a header.
		linknode = (OperationLink::Node*) ( new _byte[ sizeof( OperationLink::Node ) + sizeof( SocketOperation ) ] );

		linknode->mElement = (SocketOperation*) ( (_byte*) linknode + sizeof( OperationLink::Node ) );

		operation = linknode->mElement;
	}

	// Initialize operation.
	operation->mInternal			= 0;
	operation->mInternalHigh		= 0;
	operation->mOffset				= 0;
	operation->mOffsetHigh			= 0;
	operation->mEvent				= _null;
	operation->mType				= 0;
	operation->mSocketID			= 0;
	operation->mSessionID			= 0;
	operation->mConnectionID		= 0;
	operation->mBuffer				= (_byte*) buffer;
	operation->mLength				= length;
	operation->mAddressSize			= sizeof( SocketAddress );
	operation->mAddress.mFamily		= 2;
	operation->mAddress.mPort		= 0;
	operation->mAddress.mIP			= 0;
	operation->mAddress.mReserve1	= 0;
	operation->mAddress.mReserve2	= 0;

	// Put into using operation link.
	linknode->mNext = _null;
	linknode->mPrev = _null;

	mUsingOperationLink.InsertTail( linknode );

	mTotalOperationNumber ++;

	return operation;
}

SocketOperation* BufferManager::CreateOperation( )
{
	return CreateOperationHelper( CreateBuffer( mMaxPacketSize ), mMaxPacketSize );
}

SocketOperation* BufferManager::CreateOperation( const PacketBuffer& buffer, _bool copybuffer )
{
	// These buffer need to copy, or made by some pieces, so we need make a big buffer to hold it.
	if ( copybuffer || buffer.mLength[1] > 0 )
	{
		_dword length = buffer.Length( );

		// Create buffer from manager.
		mEnableBufferLimit = _true;
		_byte* newbuffer = CreateBuffer( length );
		mEnableBufferLimit = _false;

		if ( newbuffer == _null )
			return _null;

		_byte* newpointer = newbuffer;
		for ( _dword i = 0; i < PacketBuffer::_MAX_BUFFER_NUMBER; i ++ )
		{
			if ( buffer.mLength[i] == 0 )
				break;

			Memory::MemCpy( newpointer, buffer.mBuffer[i], buffer.mLength[i] );
			newpointer += buffer.mLength[i];
		}

		return CreateOperationHelper( newbuffer, length );
	}
	// This buffer can be just simply used.
	else
	{
		return CreateOperationHelper( buffer.mBuffer[0], buffer.mLength[0] );
	}
}

_void BufferManager::ReleaseOperation( SocketOperation*& operation )
{
	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	networkmanager.StatisticDecreaseSocketOperation( operation->mType );

	// Release the buffer used by operation.
	if ( operation->mBuffer != _null )
	{
		ReleaseBuffer( operation->mBuffer );
		operation->mBuffer = _null;
	}

	// Get link node of operation.
	OperationLink::Node* linknode = (OperationLink::Node*) ( (_byte*) operation - sizeof( OperationLink::Node ) );

	// Remove from current using operation link.
	mUsingOperationLink.Remove( linknode );

	if ( mFreeOperationLink.Number( ) >= 2048 )
	{
		delete[] (_byte*) linknode;
	}
	else
	{
		linknode->mPrev = _null;
		linknode->mNext = _null;

		mFreeOperationLink.InsertTail( linknode );
	}

	operation = _null;

	FL_ASSERT( mTotalOperationNumber > 0 )
	mTotalOperationNumber --;
}