//============================================================================
// TCPConnection.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// TCPConnection Implementation
//----------------------------------------------------------------------------

TCPConnection::TCPConnection( _dword socketid, const Address& localaddress, const Address& remoteaddress, BufferManager& buffermanager )
	: Connection( socketid, localaddress, remoteaddress, buffermanager ), mUnprocessedBuffer( _null ), mUnprocessedSize( 0 )
{
	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	mTCPSocket = (TCPSocket*) networkmanager.SearchSocket( socketid );
	FL_ASSERT( mTCPSocket != _null )

	// TCP Connection use longer time out.
	mTimeout = _DEFAULT_TIMEOUT * 2;
}

TCPConnection::~TCPConnection( )
{
	if ( mUnprocessedBuffer != _null )
		mBufferManager.ReleaseBuffer( mUnprocessedBuffer );
}

_dword TCPConnection::GetConnectionType( )
{
	return Address::_TYPE_TCP;
}

_bool TCPConnection::Connect( )
{
	if ( mPhase == INetworkSession::_PHASE_CLOSED )
		return _false;

	// Reset tickcount.
	mLastTickcount		= System::GetCurrentTickcount( );
	mCreateTickcount	= System::GetCurrentTickcount( );
	mKeepAliveTickcount	= System::GetCurrentTickcount( );
	mWaitAliveTickcount	= System::GetCurrentTickcount( );

	// Set initiative flag.
	mProperty |= _PROPERTY_INITIATIVE;

	// Set connection phase -> _PHASE_CONNECTING.
	mPhase = INetworkSession::_PHASE_CONNECTING;

	// Create overlapped operation.
	SocketOperation* operation = mBufferManager.CreateOperation( );

	if ( operation == _null )
		return _false;

	// Set session id and connection id.
	operation->mSessionID		= mLocalSessionID;
	operation->mConnectionID	= mLocalConnectionID;

	// Start connecting.
	return mTCPSocket->Connect( mRemoteAddress, operation );
}

_void TCPConnection::Close( )
{
	if ( mPhase == INetworkSession::_PHASE_CONNECTED )
	{
		FlushDeferPacket( );
	}

	ClearDelayPacket( );

	// Set connection phase -> _PHASE_CLOSED.
	mPhase = INetworkSession::_PHASE_CLOSED;
}

_bool TCPConnection::Send( SocketOperation* operation )
{
	if ( operation == _null )
		return _false;

	// Set session id and connection id.
	operation->mSessionID		= mLocalSessionID;
	operation->mConnectionID	= mLocalConnectionID;

	Packet* packet = (Packet*) operation->mBuffer;

	// Check the buffer to be a valid packet.
	FL_ASSERT( operation->mLength >= sizeof( Packet ) && operation->mLength == packet->mPacketSize )

	// Set secret code, session id and target connection id.
	packet->mSecretCode		= (_word) mSecretCode;
	packet->mSessionID		= (_word) mRemoteSessionID;
	packet->mConnectionID	= (_word) mRemoteConnectionID;

	// Post overlapped send operation.
	return mTCPSocket->Send( operation );
}

_bool TCPConnection::Recv( )
{
	if ( mPhase == INetworkSession::_PHASE_CLOSED )
		return _false;

	// Create overlapped operation.
	SocketOperation* operation = mBufferManager.CreateOperation( );

	if ( operation == _null )
		return _false;

	// Set session id and connection id.
	operation->mSessionID		= mLocalSessionID;
	operation->mConnectionID	= mLocalConnectionID;

	// Post overlapped received operation.
	return mTCPSocket->Recv( operation );
}

_bool TCPConnection::ProcessData( _byte* buffer, _dword length )
{
	_byte* backupbuffer = buffer;

	if ( mPhase == INetworkSession::_PHASE_CLOSED )
		return _false;

	// Clear reserved buffer first.
	mReservedBuffer = _null;

	// There is some unprocessed data remain.
	if ( mUnprocessedBuffer != _null )
	{
		// If the packet header is still not integrated.
		if ( mUnprocessedSize < sizeof( Packet ) )
		{
			_dword bytescopyed = Math::Min( length, sizeof( Packet ) - mUnprocessedSize );

			// Makesure the buffer can hold at least one packet.
			FL_ASSERT( mUnprocessedSize + bytescopyed <= mBufferManager.GetMaxPacketSize( ) )

			// Copy packet header into the unprocessed buffer.
			Memory::MemCpy( mUnprocessedBuffer + mUnprocessedSize, buffer, bytescopyed );

			buffer += bytescopyed; length -= bytescopyed; mUnprocessedSize += bytescopyed;
		}

		// Still not enough to build a packet header, so return.
		if ( mUnprocessedSize < sizeof( Packet ) )
			return _true;

		Packet* packet = (Packet*) mUnprocessedBuffer;

		// Bad packet.
		if ( packet->mPacketSize > mBufferManager.GetMaxPacketSize( ) )
			return _false;

		// If the packet is not integrated.
		if ( mUnprocessedSize < packet->mPacketSize )
		{
			_dword bytescopyed = Math::Min( length, packet->mPacketSize - mUnprocessedSize );

			// Make sure the buffer can hold at least one packet.
			FL_ASSERT( mUnprocessedSize + bytescopyed <= mBufferManager.GetMaxPacketSize( ) )

			// Copy packet into the unprocessed buffer.
			Memory::MemCpy( mUnprocessedBuffer + mUnprocessedSize, buffer, bytescopyed );

			buffer += bytescopyed; length -= bytescopyed; mUnprocessedSize += bytescopyed;
		}

		// Still not enough to build a packet, so return.
		if ( mUnprocessedSize < packet->mPacketSize )
			return _true;

		INetworkSession* session = GetNetworkManager( ).SearchSession( mLocalSessionID );
		FL_ASSERT( session != _null )

		// In polling mode, we need back up the buffer.
		if ( session->IsPollingModeEnabled( ) )
		{
			_byte* newbuffer = mBufferManager.CreateBuffer( packet->mPacketSize );
			Memory::MemCpy( newbuffer, packet, packet->mPacketSize );

			// Use new buffer as reserved buffer.
			mReservedBuffer = newbuffer;

			if ( ProcessPacket( (Packet*) newbuffer ) == _false )
			{
				mBufferManager.ReleaseBuffer( newbuffer );
				return _false;
			}

			mBufferManager.ReleaseBuffer( newbuffer );
		}
		else
		{
			// Now the packet is integrated, process it.
			if ( ProcessPacket( packet ) == _false )
				return _false;
		}

		Memory::MemMove( mUnprocessedBuffer, mUnprocessedBuffer + packet->mPacketSize, mUnprocessedSize - packet->mPacketSize );
		mUnprocessedSize -= packet->mPacketSize;
	}

	// Try to analyse data buffer into packet.
	while ( length >= sizeof( Packet ) )
	{
		Packet* packet = (Packet*) buffer;

		// Invalid packet, just skip it.
		if ( packet->mPacketSize > length )
			break;

		buffer += packet->mPacketSize;
		length -= packet->mPacketSize;

		// Use input buffer as reserved buffer.
		mReservedBuffer = backupbuffer;

		// Process the packet.
		if ( ProcessPacket( packet ) == _false )
			return _false;
	}

	// There is data remain, and cant be processed into packet, copy it to unprocessed buffer.
	if ( length > 0 )
	{
		// Create unprocessed buffer when needed.
		if ( mUnprocessedBuffer == _null )
			mUnprocessedBuffer = mBufferManager.CreateBuffer( );

		FL_ASSERT( mUnprocessedBuffer != _null )

		// Copy remain data into the unprocessed buffer.
		Memory::MemCpy( mUnprocessedBuffer + mUnprocessedSize, buffer, length );

		mUnprocessedSize += length;
	}
	// Data all processed.
	else
	{
		// Release unprocessed buffer when needed.
		if ( mUnprocessedBuffer != _null )
			mBufferManager.ReleaseBuffer( mUnprocessedBuffer );

		mUnprocessedBuffer	= _null;
		mUnprocessedSize	= 0;
	}

	return _true;
}

_bool TCPConnection::ProcessPacket( Packet* packet )
{
	// Check packet version.
	if ( packet->mVersion != Packet::_PACKET_VERSION )
		return _false;

	// Set connection secret code.
	if ( packet->mMagicCode == JoinReqPacket::_MAGIC_CODE )
		mSecretCode = packet->mSecretCode;

	return Connection::ProcessPacket( packet );
}