//============================================================================
// Connection.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// Connection Implementation
//----------------------------------------------------------------------------

Connection::Connection( _dword socketid, const Address& localaddress, const Address& remoteaddress, BufferManager& buffermanager )
	: mBufferManager( buffermanager )
{
	mGlobalToken			= 0;

	mSocketID				= socketid;
	mSecretCode				= 0;

	mLocalSessionID			= 0;
	mLocalConnectionID		= 0;
	mRemoteSessionID		= 0;
	mRemoteConnectionID		= 0;

	mProperty				= 0;
	mPriority				= INetworkManager::_NETWORK_PRIORITY_NORMAL;
	mPhase					= INetworkSession::_PHASE_UNKNOWN;
	mCloseReason			= INetworkSession::_REASON_UNKNOWN;

	mGroupID				= 0;
	mUserData				= 0;
	mTimer					= -1;
	mTimeout				= _DEFAULT_TIMEOUT;
	mConnectInterval		= _CONNECT_INTERVAL;

	mLastTickcount			= System::GetCurrentTickcount( );
	mCreateTickcount		= System::GetCurrentTickcount( );
	mKeepAliveTickcount		= System::GetCurrentTickcount( );
	mWaitAliveTickcount		= System::GetCurrentTickcount( );
	mUpdateSpeedTickcount	= System::GetCurrentTickcount( );
	mLatency				= -1;

	mPacketTickcount		= 0;
	mSendPacketReference	= 0;
	mRecvPacketReference	= 0;
	mDelayChannel			= 0;
	mDelayTickcount			= 0;
	mDelayReference			= 0;

	mSentBytes				= 0;
	mReceivedBytes			= 0;

	mReservedBuffer			= _null;

	mDeferBuffer			= _null;
	mDeferIndex				= 0;
	mDeferPacketNumber		= 0;

	mIncompleteBuffer		= _null;
	mIncompleteIndex		= 0;
	mIncompleteSize			= 0;
	mIncompletePacketNumber	= 0;

	mLocalAddress			= localaddress;
	mRemoteAddress			= remoteaddress;
	mInternalAddress		= remoteaddress;

	mInputBuffer			= _null;
	mInputIndex				= 0;
	mInputSize				= 0;
	mInputLogicSize			= 0;
	mInputRealSize			= 0;

	mOutputBuffer			= _null;
	mOutputIndex			= 0;
	mOutputLogicSize		= 0;
	mOutputRealSize			= 0;

	mNextConnectionInGroup	= _null;

	for ( _dword i = 0; i < 10; i ++ )
	{
		mRecentSentBytes[i]		= 0;
		mRecentReceivedBytes[i]	= 0;
	}
}

Connection::~Connection( )
{
	ClearDelayPacket( );

	if ( mDeferBuffer != _null )
		mBufferManager.ReleaseBuffer( mDeferBuffer );

	if ( mIncompleteBuffer != _null )
		mBufferManager.ReleaseBuffer( mIncompleteBuffer );

	if ( mInputBuffer != _null )
		mBufferManager.ReleaseBuffer( mInputBuffer );

	if ( mOutputBuffer != _null )
		mBufferManager.ReleaseBuffer( mOutputBuffer );

	if ( mInputStream.mInitalized )
	{
		_char buffer1[1024], buffer2[1024];
		StringFormatter::FormatBytesBuffer( buffer1, 1024, mInputRealSize );
		StringFormatter::FormatBytesBuffer( buffer2, 1024, mInputLogicSize );

		_float rate = mInputLogicSize == 0 ? 0.0f : (_float) mInputRealSize / (_float) mInputLogicSize * 100.0f;

		PS_TRACE3( L"[NK] Decompress for input %s / %s -> %3.2f%%\r\n", buffer1, buffer2, rate );

		Compression::FinishDecompress( mInputStream );
	}

	if ( mOutputStream.mInitalized )
	{
		_char buffer1[1024], buffer2[1024];
		StringFormatter::FormatBytesBuffer( buffer1, 1024, mOutputRealSize );
		StringFormatter::FormatBytesBuffer( buffer2, 1024, mOutputLogicSize );

		_float rate = mOutputLogicSize == 0 ? 0.0f : (_float) mOutputRealSize / (_float) mOutputLogicSize * 100.0f;

		PS_TRACE3( L"[NK] Compress for output %s / %s -> %3.2f%%\r\n", buffer1, buffer2, rate );

		Compression::FinishCompress( mOutputStream );
	}
}

_bool Connection::SendPacket( _void* buffer )
{
	Packet* header = (Packet*) buffer;

	return Send( mBufferManager.CreateOperationHelper( buffer, header->mPacketSize ) );
}

_bool Connection::SendPacket( _void* buffer, _dword length )
{
	PacketBuffer sendbuffer;
	sendbuffer.mBuffer[0] = buffer;
	sendbuffer.mLength[0] = length;

	return SendPacket( sendbuffer );
}

_bool Connection::SendPacket( const PacketBuffer& buffer )
{
	if ( FlushDeferPacket( ) == _false )
		return _false;

	return Send( mBufferManager.CreateOperation( buffer ) );
}

_bool Connection::SendBigPacket( const PacketBuffer& buffer, _dword maxpacketsize )
{
	MemFile bigpacket;
	bigpacket.Open( (_byte*) buffer.mBuffer[0], buffer.mLength[0], _false );
	bigpacket.EnableAutoDeleteBuffer( _false );

	// Put buffer list into a big buffer.
	for ( _dword i = 1; i < PacketBuffer::_MAX_BUFFER_NUMBER; i ++ )
	{
		if ( buffer.mLength[i] == 0 )
			break;

		bigpacket.Append( (_byte*) buffer.mBuffer[i], buffer.mLength[i] );
	}

	_byte* wholebuffer = bigpacket.GetBuffer( );
	_dword wholelength = buffer.Length( );

	// The packet is not that big, so make max packet size less.
	if ( sizeof( Packet ) + sizeof( BigPacketBegin ) + wholelength < maxpacketsize )
		maxpacketsize = sizeof( Packet ) + sizeof( BigPacketBegin ) + wholelength;

	// Build packet header.
	Packet header( (_word) maxpacketsize );

	// Set packet flag.
	header.mPacketFlag = Packet::_PACKET_BIGPACKETBEGIN;

	// Build big packet begin.
	BigPacketBegin packetbegin( wholelength, mDeferPacketNumber );

	_dword bytesremain = maxpacketsize - sizeof( Packet ) - sizeof( BigPacketBegin );

	PacketBuffer sendbuffer;
	sendbuffer.mBuffer[0] = &header;
	sendbuffer.mLength[0] = sizeof( Packet );
	sendbuffer.mBuffer[1] = &packetbegin;
	sendbuffer.mLength[1] = sizeof( BigPacketBegin );
	sendbuffer.mBuffer[2] = wholebuffer;
	sendbuffer.mLength[2] = bytesremain;

	// Send begin packet first.
	if ( SendCompressPacket( sendbuffer ) == _false )
		return _false;

	wholebuffer = wholebuffer + bytesremain;
	bytesremain = wholelength - bytesremain;

	// Send remain data.
	while ( bytesremain > 0 )
	{
		_dword bufferlength = maxpacketsize - sizeof( Packet );

		if ( bufferlength > bytesremain )
			bufferlength = bytesremain;

		// Build packet header.
		Packet header( (_word) ( bufferlength + sizeof( Packet ) ) );

		// Set packet flag.
		header.mPacketFlag = Packet::_PACKET_BIGPACKETDATA;

		sendbuffer = PacketBuffer( );
		sendbuffer.mBuffer[0] = &header;
		sendbuffer.mLength[0] = sizeof( Packet );
		sendbuffer.mBuffer[1] = wholebuffer;
		sendbuffer.mLength[1] = bufferlength;

		// Send data packet.
		if ( SendCompressPacket( sendbuffer ) == _false )
			return _false;

		wholebuffer  = wholebuffer + bufferlength;
		bytesremain	-= bufferlength;
	}

	return _true;
}

_bool Connection::SendDeferPacket( const PacketBuffer& buffer )
{
	_dword length = buffer.Length( );

	// Create defer buffer, with packet header.
	if ( mDeferBuffer == _null )
	{
		mDeferBuffer = mBufferManager.CreateBuffer( );
		mDeferIndex  = sizeof( Packet );
	}

	_dword needbuffersize = mDeferIndex + sizeof( PacketDivision ) + length;

	// The packet is too big to put into defer buffer.
	if ( needbuffersize > mBufferManager.GetMaxPacketSize( ) )
	{
		// Auto flush defer buffer when it is full.
		FlushDeferPacket( );
	}

	// Create defer buffer, with packet header.
	if ( mDeferBuffer == _null )
	{
		mDeferBuffer = mBufferManager.CreateBuffer( );
		mDeferIndex  = sizeof( Packet );
	}

	FL_ASSERT( mDeferIndex + sizeof( PacketDivision ) + length <= mBufferManager.GetMaxPacketSize( ) )

	// Build packet division.
	PacketDivision division( length + sizeof( PacketDivision ) );

	// Copy packet division into buffer.
	Memory::MemCpy( mDeferBuffer + mDeferIndex, &division, sizeof( PacketDivision ) );
	mDeferIndex += sizeof( PacketDivision );

	// Copy packet data into buffer.
	for ( _dword i = 0; i < PacketBuffer::_MAX_BUFFER_NUMBER; i ++ )
	{
		if ( buffer.mLength[i] == 0 )
			break;

		Memory::MemCpy( mDeferBuffer + mDeferIndex, buffer.mBuffer[i], buffer.mLength[i] );
		mDeferIndex += buffer.mLength[i];
	}

	// Increase defer packet number.
	mDeferPacketNumber ++;

	// The buffer already too big, we need flush it.
	if ( mDeferIndex >= mBufferManager.GetMaxPacketSize( ) )
		FlushDeferPacket( );

	return _true;
}

_bool Connection::FlushDeferPacket( )
{
	if ( mPhase == INetworkSession::_PHASE_CLOSED )
		return _false;

	if ( mDeferIndex <= sizeof( Packet ) )
		return _true;

	FL_ASSERT( mDeferBuffer != _null )

	_dword size = mDeferIndex;
	mDeferIndex = sizeof( Packet );

	// We need send as big packet, without packet header.
	if ( size > mBufferManager.GetMaxPacketSize( ) )
	{
		PacketBuffer sendbuffer;
		sendbuffer.mBuffer[0] = mDeferBuffer + sizeof( Packet );
		sendbuffer.mLength[0] = size - sizeof( Packet );

		SendBigPacket( sendbuffer, mBufferManager.GetMaxPacketSize( ) );
	}
	else
	{
		// Build packet header.
		Packet header( (_word) size );
		header.mPacketFlag = Packet::_PACKET_LIST;

		// Send directly, to prevent one more memcpy.
		if ( ( mProperty & _PROPERTY_COMPRESSPACKET ) == 0 )
		{
			// Copy packet header to buffer begin.
			Memory::MemCpy( mDeferBuffer, &header, sizeof( header ) );

			// Send buffer without any copy.
			SendPacket( mDeferBuffer );

			// Just drop reference of the buffer.
			mDeferBuffer = _null;
			mDeferIndex  = 0;
		}
		// Send as compressed buffer.
		else
		{
			// We need build send buffer into two different part, header and data.
			PacketBuffer sendbuffer;
			sendbuffer.mBuffer[0] = &header;
			sendbuffer.mLength[0] = sizeof( Packet );
			sendbuffer.mBuffer[1] = mDeferBuffer + sizeof( Packet );
			sendbuffer.mLength[1] = size - sizeof( Packet );

			SendCompressPacket( sendbuffer );
		}
	}

	mDeferPacketNumber = 0;

	return _true;
}

_bool Connection::SendCompressPacket( PacketBuffer& buffer )
{
	// Not in compress mode, send as normal packet.
	if ( ( mProperty & _PROPERTY_COMPRESSPACKET ) == 0 )
		return SendPacket( buffer );

	FL_ASSERT( buffer.mLength[0] == sizeof( Packet ) )
	FL_ASSERT( buffer.mLength[1] != 0 )

	Packet header( 0 );
	Memory::MemCpy( &header, buffer.mBuffer[0], sizeof( Packet ) );

	// Adjust packet buffer, put content size into first buffer.
	_dword packetsize = buffer.Length( ) - sizeof( Packet );
	buffer.mBuffer[0] = &packetsize;
	buffer.mLength[0] = sizeof( _dword );

	mOutputLogicSize += packetsize + sizeof( Packet );
	mOutputRealSize += sizeof( Packet );

	// Create output buffer.
	if ( mOutputBuffer == _null )
		mOutputBuffer = mBufferManager.CreateBuffer( );

	// Compress each part of buffer.
	for ( _dword i = 0; i < PacketBuffer::_MAX_BUFFER_NUMBER; i ++ )
	{
		if ( buffer.mLength[i] == 0 )
			break;

		// Prepare compress buffer.
		Compression::PrepareCompress( mOutputStream, buffer.mBuffer[i], buffer.mLength[i] );

		_bool flush = ( i == PacketBuffer::_MAX_BUFFER_NUMBER - 1 ) || buffer.mLength[i + 1] == 0;

		while ( _true )
		{
			// Fill packet header at buffer begin.
			if ( mOutputIndex == 0 )
			{
				Memory::MemCpy( mOutputBuffer, &header, sizeof( Packet ) );
				mOutputIndex = sizeof( Packet );
			}

			// Compress packet, flush only at end of buffer.
			_dword bytes = mBufferManager.GetMaxPacketSize( ) - mOutputIndex;
			Compression::ProcessCompress( mOutputStream, mOutputBuffer + mOutputIndex, bytes, flush ? Compression::_FLUSH_SYNC : Compression::_FLUSH_NO );
			mOutputIndex += bytes;
			mOutputRealSize += bytes;

			// Finish compress when no buffer remain.
			if ( bytes == 0 )
				break;

			FL_ASSERT( mOutputIndex <= mBufferManager.GetMaxPacketSize( ) )

			// Output buffer is large enough, send it.
			if ( mOutputIndex == mBufferManager.GetMaxPacketSize( ) )
			{
				if ( FlushCompressPacket( ) == _false )
					return _false;
			}
		}
	}

	// Send output buffer.
	if ( mOutputIndex > 0 )
	{
		if ( FlushCompressPacket( ) == _false )
			return _false;
	}

	return _true;
}

_bool Connection::FlushCompressPacket( )
{
	if ( mOutputIndex == 0 )
		return _true;

	// Set packet size and compress flag.
	( (Packet*) mOutputBuffer )->mCompressFlag	= 1;
	( (Packet*) mOutputBuffer )->mPacketSize	= (_word) mOutputIndex;

	if ( Send( mBufferManager.CreateOperationHelper( mOutputBuffer, mOutputIndex ) ) == _false )
		return _false;

	// Because the output buffer is used for send, so we need create a new one.
	mOutputBuffer = mBufferManager.CreateBuffer( );
	mOutputIndex  = 0;

	return _true;
}

_bool Connection::ProcessNormalPacket( _byte* remainbuffer, _dword remainbytes, _dword packetflag, _byte* reservedbuffer )
{
	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	// Normal packet.
	if ( packetflag == Packet::_PACKET_NORMAL )
	{
		networkmanager.CallReceivePacketCallback( mLocalSessionID, mLocalConnectionID, remainbuffer, remainbytes, reservedbuffer );
	}
	// Packet list, split it into many packet.
	else if ( packetflag == Packet::_PACKET_LIST )
	{
		if ( ProcessPacketList( remainbuffer, remainbytes, reservedbuffer ) == _false )
			return _false;
	}
	// Big packet header.
	else if ( packetflag == Packet::_PACKET_BIGPACKETBEGIN )
	{
		if ( remainbytes < sizeof( BigPacketBegin ) )
			return _false;

		BigPacketBegin* bigpacket = (BigPacketBegin*) remainbuffer;

		remainbuffer += sizeof( BigPacketBegin );
		remainbytes  -= sizeof( BigPacketBegin );

		if ( remainbytes > bigpacket->mPacketSize )
			return _false;

		// Get packet size.
		mIncompleteSize			= bigpacket->mPacketSize;
		mIncompletePacketNumber	= bigpacket->mPacketNumber;

		// Release the old incomplete buffer.
		if ( mIncompleteBuffer != _null )
			mBufferManager.ReleaseBuffer( mIncompleteBuffer );

		// Create an new incomplete buffer.
		mIncompleteBuffer = mBufferManager.CreateBuffer( mIncompleteSize );
		mIncompleteIndex  = 0;

		if ( mIncompleteBuffer == _null )
			return _false;

		if ( ProcessBigPacket( remainbuffer, remainbytes ) == _false )
			return _false;
	}
	// Big packet data.
	else if ( packetflag == Packet::_PACKET_BIGPACKETDATA )
	{
		if ( mIncompleteBuffer == _null )
			return _false;

		if ( mIncompleteIndex + remainbytes > mIncompleteSize )
			return _false;

		if ( ProcessBigPacket( remainbuffer, remainbytes ) == _false )
			return _false;
	}
	else
	{
		mLostReason.Format( L"ProcessPacket, Bad Packet Flag %d", packetflag );

		return _false;
	}

	return _true;
}

_bool Connection::ProcessPacketList( _byte* remainbuffer, _dword remainbytes, _byte* reservedbuffer )
{
	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	// Process each packet separated by packet division.
	while ( remainbytes >= sizeof( PacketDivision ) )
	{
		PacketDivision* division = (PacketDivision*) remainbuffer;

		if ( division->mPacketSize < sizeof( PacketDivision ) )
		{
			mLostReason.Format( L"ProcessPacketList, division->mPacketSize(%d) < sizeof( PacketDivision )", division->mPacketSize );

			return _false;
		}

		// Bad packet.
		if ( division->mPacketSize > remainbytes )
		{
			mLostReason.Format( L"ProcessPacketList, division->mPacketSize(%d) > remainbytes(%d)", division->mPacketSize, remainbytes );

			return _false;
		}

		networkmanager.CallReceivePacketCallback( mLocalSessionID, mLocalConnectionID,
			remainbuffer + sizeof( PacketDivision ), division->mPacketSize - sizeof( PacketDivision ), reservedbuffer );

		remainbuffer += division->mPacketSize; 
		remainbytes	 -= division->mPacketSize;
	}

	// Still some data not processed, just return false.
	if ( remainbytes > 0 )
	{
		mLostReason.Format( L"ProcessPacketList, remainbytes(%d) > 0", remainbytes );

		return _false;
	}

	return _true;
}

_bool Connection::ProcessBigPacket( _byte* remainbuffer, _dword remainbytes )
{
	FL_ASSERT( mIncompleteIndex + remainbytes <= mIncompleteSize )

	// Copy part of buffer.
	Memory::MemCpy( mIncompleteBuffer + mIncompleteIndex, remainbuffer, remainbytes );

	mIncompleteIndex += remainbytes;

	// The packet incompleted.
	if ( mIncompleteIndex != mIncompleteSize )
		return _true;

	// The packet is compressed.
	if ( Compression::IsCompressed( mIncompleteBuffer, mIncompleteSize ) )
	{
		// Get original file size.
		_dword originalsize = *( (_dword*) ( mIncompleteBuffer + 16 ) );

		// Create buffer to decompress.
		_byte* newbuffer = mBufferManager.CreateBuffer( originalsize );

		// Decompress the packet.
		if ( Compression::Decompress( newbuffer, originalsize, mIncompleteBuffer + 20, mIncompleteSize - 20 ) == _false )
		{
			mLostReason.Format( L"ProcessBigPacket, Compression::Decompress( %d, %d )", originalsize, mIncompleteSize - 20 );

			mBufferManager.ReleaseBuffer( newbuffer );

			return _false;
		}

		// Release the compressed packet.
		mBufferManager.ReleaseBuffer( mIncompleteBuffer );

		mIncompleteBuffer	= newbuffer;
		mIncompleteSize		= originalsize;
	}

	// Only one packet, process it directly.
	if ( mIncompletePacketNumber == 0 )
	{
		NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

		networkmanager.CallReceivePacketCallback( mLocalSessionID, mLocalConnectionID,
			mIncompleteBuffer, mIncompleteSize, mIncompleteBuffer );
	}
	// More than one packet in list, process them as list.
	else
	{
		if ( ProcessPacketList( mIncompleteBuffer, mIncompleteSize, mIncompleteBuffer ) == _false )
			return _false;
	}

	mBufferManager.ReleaseBuffer( mIncompleteBuffer );

	mIncompleteBuffer		= _null;
	mIncompleteIndex		= 0;
	mIncompleteSize			= 0;
	mIncompletePacketNumber	= 0;

	return _true;
}

_void Connection::FlushDelayPacket( _dword channel )
{
	for ( _dword i = 0; i < mDelayPacketArray.Number( ); i ++ )
	{
		DelayPacket delaypacket = mDelayPacketArray[i];

		// Skip different channel.
		if ( delaypacket.mChannel != channel )
			continue;

		NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

		// Call receive packet call back.
		networkmanager.CallReceivePacketCallback( mLocalSessionID, mLocalConnectionID,
			delaypacket.mBuffer, delaypacket.mSize, delaypacket.mReservedBuffer, _true );

		// Release buffer.
		mBufferManager.ReleaseBuffer( delaypacket.mReservedBuffer );

		// Remove the delay packet.
		mDelayPacketArray.Remove( i -- );
	}
}

_void Connection::CreateDelayPacket( _dword channel, _dword tickcount, _byte* buffer, _dword size, _byte* reservedbuffer )
{
	if ( channel != 0 )
	{
		for ( _dword i = 0; i < mDelayPacketArray.Number( ); i ++ )
		{
			DelayPacket& delaypacket = mDelayPacketArray[i];

			// Skip different channel.
			if ( delaypacket.mChannel != channel )
				continue;

			// Make sure previous 
			if ( delaypacket.mTickcount > tickcount )
				delaypacket.mTickcount = tickcount;
		}
	}

	DelayPacket delaypacket;
	delaypacket.mChannel		= channel;
	delaypacket.mTickcount		= tickcount;
	delaypacket.mBuffer			= buffer;
	delaypacket.mSize			= size;
	delaypacket.mReservedBuffer	= reservedbuffer;

	mDelayPacketArray.Append( delaypacket );

	// We need reserve the buffer again.
	if ( reservedbuffer != _null )
		mBufferManager.ReserveBuffer( reservedbuffer );
}

_void Connection::CheckDelayPacket( _dword sessiontickcount )
{
	for ( _dword i = 0; i < mDelayPacketArray.Number( ); i ++ )
	{
		DelayPacket delaypacket = mDelayPacketArray[i];

		// This packet not ready to process yet.
		if ( delaypacket.mTickcount > sessiontickcount )
			continue;

		NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

		// Set delay tickcount before we really process it, because session tickcount maybe change, so we need adjust it.
		PacketTickcount* packettickcont = (PacketTickcount*) delaypacket.mBuffer;
		packettickcont->mDelayTickcount = delaypacket.mTickcount;

		// Call receive packet call back.
		networkmanager.CallReceivePacketCallback( mLocalSessionID, mLocalConnectionID,
			delaypacket.mBuffer, delaypacket.mSize, delaypacket.mReservedBuffer, _true );

		// Release buffer.
		mBufferManager.ReleaseBuffer( delaypacket.mReservedBuffer );

		// Remove the delay packet.
		mDelayPacketArray.Remove( i -- );
	}
}

_void Connection::UpdateDelayPacket( _dword deltatickcount )
{
	for ( _dword i = 0; i < mDelayPacketArray.Number( ); i ++ )
	{
		DelayPacket& delaypacket = mDelayPacketArray[i];

		delaypacket.mTickcount += deltatickcount;

		if ( delaypacket.mTickcount & 0x80000000 )
			delaypacket.mTickcount = 0;
	}
}

_void Connection::ClearDelayPacket( )
{
	for ( _dword i = 0; i < mDelayPacketArray.Number( ); i ++ )
	{
		DelayPacket& delaypacket = mDelayPacketArray[i];

		// Release the packet buffer.
		mBufferManager.ReleaseBuffer( delaypacket.mReservedBuffer );
	}

	mDelayPacketArray.Clear( );
}

_bool Connection::ProcessPacket( Packet* packet )
{
	_byte* reservedbuffer = mReservedBuffer;

	// No pre set reserved buffer, so we use packet buffer as reserved buffer.
	if ( reservedbuffer == _null )
		reservedbuffer = (_byte*) packet;

	if ( mPhase == INetworkSession::_PHASE_CLOSED )
	{
		mLostReason.Format( L"ProcessPacket, mPhase == INetworkSession::_PHASE_CLOSED" );

		return _false;
	}

	// Check packet version.
	if ( packet->mVersion != Packet::_PACKET_VERSION )
	{
		mLostReason.Format( L"ProcessPacket, packet->mVersion(%d) != Packet::_PACKET_VERSION", packet->mVersion );

		return _false;
	}

	// Check packet size.
	if ( packet->mPacketSize < sizeof( Packet ) )
	{
		mLostReason.Format( L"ProcessPacket, packet->mPacketSize(%d) < sizeof( Packet )", packet->mPacketSize );

		return _false;
	}

	// Check secrete code in the packet.
	if ( packet->mSecretCode != (_word) mSecretCode )
	{
		mLostReason.Format( L"ProcessPacket, packet->mSecretCode(%d) != (_word) mSecretCode(%d)", packet->mSecretCode, mSecretCode );

		return _false;
	}

	// Check session id in the packet.
	if ( packet->mSessionID != (_word) mLocalSessionID )
	{
		mLostReason.Format( L"ProcessPacket, packet->mSessionID(%d) != (_word) mLocalSessionID(%d)", packet->mSessionID, mLocalSessionID );

		return _false;
	}

	// Check connection id in the packet.
	if ( packet->mConnectionID != 0 && packet->mConnectionID != (_word) mLocalConnectionID )
	{
		mLostReason.Format( L"ProcessPacket, packet->mConnectionID(%d) != (_word) mLocalConnectionID(%d)", packet->mConnectionID, mLocalConnectionID );

		return _false;
	}

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	INetworkSession* session = networkmanager.SearchSession( mLocalSessionID );
	FL_ASSERT( session != _null )

	switch ( packet->mMagicCode )
	{
		// Normal data packet.
		case Packet::_MAGIC_CODE:
		{
			_dword datalength = packet->mPacketSize - sizeof( Packet );

			if ( datalength == 0 )
				return _true;

			if ( mPhase != INetworkSession::_PHASE_CONNECTED )
				return _true;

			_byte* remainbuffer = (_byte*) packet + sizeof( Packet ); _dword remainbytes = datalength;

			if ( packet->mCompressFlag == 0 )
				return ProcessNormalPacket( remainbuffer, remainbytes, packet->mPacketFlag, reservedbuffer );

			// Not in compress mode, we cant handle this packet.
			if ( ( mProperty & _PROPERTY_COMPRESSPACKET ) == 0 )
				return _false;

			// Create decompress buffer.
			if ( mInputBuffer == _null )
				mInputBuffer = mBufferManager.CreateBuffer( );

			// Prepare decompress.
			Compression::PrepareDecompress( mInputStream, remainbuffer, remainbytes );

			mInputRealSize += remainbytes + sizeof( Packet );
			mInputLogicSize += sizeof( Packet );

			while ( _true )
			{
				// Get input buffer size first.
				if ( mInputSize == 0 )
				{
					if ( mInputIndex < sizeof( _dword ) )
					{
						// Decompress the packet.
						_dword bytes = sizeof( _dword ) - mInputIndex;
						Compression::ProcessDecompress( mInputStream, mInputBuffer + mInputIndex, bytes );
						mInputIndex += bytes;
						mInputLogicSize += bytes;
					}

					// Still not got packet size yet.
					if ( mInputIndex < sizeof( _dword ) )
						break;

					// Get input buffer size.
					mInputSize = *( (_dword*) mInputBuffer );
					mInputIndex -= sizeof( _dword );

					// Bad packet.
					if ( mInputSize == 0 || mInputSize > mBufferManager.GetMaxPacketSize( ) )
						return _false;

					// Move remain buffer to begin.
					if ( mInputIndex > 0 )
						Memory::MemMove( mInputBuffer, mInputBuffer + sizeof( _dword ), mInputIndex );
				}

				// Packet not complete yet.
				if ( mInputIndex < mInputSize )
				{
					// Decompress the packet.
					_dword bytes = mBufferManager.GetMaxPacketSize( ) - mInputIndex;
					Compression::ProcessDecompress( mInputStream, mInputBuffer + mInputIndex, bytes );
					mInputIndex += bytes;
					mInputLogicSize += bytes;

					// Nothing remain in buffer.
					if ( bytes == 0 )
						break;
				}

				// Packet not complete yet.
				if ( mInputIndex < mInputSize )
					return _true;

				// Process the packet.
				ProcessNormalPacket( mInputBuffer, mInputSize, packet->mPacketFlag, mInputBuffer );

				// Create another input buffer, because current buffer maybe used in pooling mode.
				_byte* oldbuffer = mInputBuffer;

				// Create a new buffer.
				mInputBuffer = mBufferManager.CreateBuffer( );

				// Calculate buffer remain.
				mInputIndex -= mInputSize;

				// Move remain buffer to begin.
				if ( mInputIndex > 0 )
					Memory::MemMove( mInputBuffer, oldbuffer + mInputSize, mInputIndex );

				mInputSize = 0;

				// Release the old buffer.
				mBufferManager.ReleaseBuffer( oldbuffer );
			}

			return _true;
		}

		// Join session req packet.
		case JoinReqPacket::_MAGIC_CODE:
		{
			JoinReqPacket* joinreq = (JoinReqPacket*) packet;

			// Check connection phase.
			if ( mPhase != INetworkSession::_PHASE_WAITJOINREQ )
				return _false;

			// Set remote session id according to the packet.
			mRemoteSessionID = joinreq->mMySessionID;

			// Set remote connection id according to the packet.
			mRemoteConnectionID = joinreq->mMyConnectionID;

			// Set connection phase -> _PHASE_RECVJOINREQ.
			mPhase = INetworkSession::_PHASE_RECVJOINREQ;

			// This connection allow compress packet.
			if ( joinreq->mPacketProperty & _PROPERTY_ASKCOMPRESS )
				mProperty |= _PROPERTY_COMPRESSPACKET;

			// This connection allow reliable ping.
			if ( joinreq->mPacketProperty & _PROPERTY_RELIABLEPING )
				mProperty |= _PROPERTY_RELIABLEPING;

			// Build join ack packet.
			JoinAckPacket joinack( (_word) mLocalSessionID, (_word) mLocalConnectionID, joinreq->mMyTickcount );

			// Apply some property.
			if ( joinreq->mPacketProperty & _PROPERTY_ASKCOMPRESS )
				joinack.mPacketProperty |= _PROPERTY_AGREECOMPRESS;
			if ( joinreq->mPacketProperty & _PROPERTY_RELIABLEPING )
				joinack.mPacketProperty |= _PROPERTY_RELIABLEPING;

			// We need put session tickcount into this packet.
			if ( session->IsSessionTickcountEnabled( ) )
				joinack.mSessionTickcount = session->GetSessionTickcount( );

			// Send join ack packet.
			if ( SendPacket( &joinack, joinack.mPacketSize ) == _false )
				return _false;

			return _true;
		}

		// Join session ack packet.
		case JoinAckPacket::_MAGIC_CODE:
		{
			JoinAckPacket* joinack = (JoinAckPacket*) packet;

			// Check connection phase.
			if ( mPhase != INetworkSession::_PHASE_SENTJOINREQ && mPhase != INetworkSession::_PHASE_RECVJOINREQ )
				return _false;

			// Join failed, we just return, and user will know it when timeout.
			if ( joinack->mMySessionID == 0 )
				return _false;

			// Set remote session id according to the packet.
			mRemoteSessionID = joinack->mMySessionID;

			// Set remote connection id according to the packet.
			mRemoteConnectionID = joinack->mMyConnectionID;

			// Update keep alive tickcount.
			mKeepAliveTickcount = System::GetCurrentTickcount( );

			// The connection is initiate from local, so send join request again.
			if ( mPhase == INetworkSession::_PHASE_SENTJOINREQ && ( mProperty & _PROPERTY_PINGONLY ) == 0 )
			{
				// Build join ack packet.
				JoinAckPacket returnjoinack( (_word) mLocalSessionID, (_word) mLocalConnectionID, joinack->mMyTickcount );
				returnjoinack.mPacketProperty = joinack->mPacketProperty;

				// Send join ack packet.
				if ( SendPacket( &returnjoinack, returnjoinack.mPacketSize ) == _false )
					return _false;
			}

			// This connection allow compress packet.
			if ( joinack->mPacketProperty & _PROPERTY_AGREECOMPRESS )
				mProperty |= _PROPERTY_COMPRESSPACKET;

			// This connection allow reliable ping.
			if ( joinack->mPacketProperty & _PROPERTY_RELIABLEPING )
				mProperty |= _PROPERTY_RELIABLEPING;

			// Set connection phase -> _PHASE_CONNECTED.
			mPhase = INetworkSession::_PHASE_CONNECTED;

			// Get connection local ip.
			_dword localip = mLocalAddress.mIP;
			if ( localip == 0 )
				localip = NetworkManager::GetLocalIP( 0 );

			// Get socket sub net mask.
			Socket* socket = networkmanager.SearchSocket( mSocketID );
			FL_ASSERT( socket != _null )

			_dword subnetmask = socket->mSubnetMask;

			// Decide if this is a LAN connection.
			if ( ( localip & subnetmask ) == ( mInternalAddress.mIP & subnetmask ) )
				mProperty |= Connection::_PROPERTY_LAN;

			if ( mGroupID != 0 )
			{
				// Save connection group id.
				_dword groupid = mGroupID;

				// Clear current group id first.
				mGroupID = 0;

				// Set connection group id, put connection into group link.
				session->SetConnectionGroup( mLocalConnectionID, groupid );
			}

			// Update network latency.
			mLatency = ( System::GetCurrentTickcount( ) - mCreateTickcount + 1 ) / 2;

			NetworkStatistics* netstat = (NetworkStatistics*) session->GetStatistics( );
			FL_ASSERT( netstat != _null )

			// Statistic latency.
			netstat->StatisticLatency( mLatency );

			// This packet contains tick information, we need use that to initial server tickcount.
			if ( ( mProperty & _PROPERTY_INITIATIVE ) && ( joinack->mSessionTickcount != 0 ) )
			{
				if ( session->IsSessionTickcountEnabled( ) )
				{
					session->InitSessionTickcount( joinack->mSessionTickcount + mLatency );
				}
				else
				{
					PS_ERROR6( L"[NK] Bad connection tickcount mode : %d.%d.%d.%d:%d (%d)\r\n",
						mRemoteAddress.mIP4, mRemoteAddress.mIP3, mRemoteAddress.mIP2, mRemoteAddress.mIP1, mRemoteAddress.mPort, joinack->mSessionTickcount );
				}
			}

			// Init compress stream.
			if ( mProperty & _PROPERTY_COMPRESSPACKET )
			{
				Compression::BeginDecompress( mInputStream );
				Compression::BeginCompress( mOutputStream );
			}

			// Call the callback function to inform application.
			networkmanager.CallCreateConnectionCallback( mLocalSessionID, mLocalConnectionID );

			return _true;
		}

		// Close req packet.
		case CloseReqPacket::_MAGIC_CODE:
		{
			CloseReqPacket* closereq = (CloseReqPacket*) packet;

			// Check connection phase.
			if ( mPhase != INetworkSession::_PHASE_CONNECTED )
				return _false;

			// Set connection phase -> _PHASE_PASVCLOSE.
			mPhase = INetworkSession::_PHASE_PASVCLOSE;

			// Set connection close reason.
			mCloseReason = INetworkSession::_REASON_REMOTECLOSE;

			mLostReason.Format( L"ProcessPacket, CloseReqPacket" );

			// Return false to close the connection.
			return _false;
		}

		// Update connection property.
		case ConnectionPropertyPacket::_MAGIC_CODE:
		{
			ConnectionPropertyPacket* connectionproperty = (ConnectionPropertyPacket*) packet;

			mTimeout	= connectionproperty->mTimeout;
			mPriority	= connectionproperty->mPriority;

			return _true;
		}

		// Ping req packet.
		case PingReqPacket::_MAGIC_CODE:
		{
			PingReqPacket* pingreq = (PingReqPacket*) packet;

			// Check connection phase.
			if ( mPhase != INetworkSession::_PHASE_CONNECTED )
			{
				mLostReason.Format( L"ProcessPacket, mPhase(%d) != INetworkSession::_PHASE_CONNECTED", mPhase );

				return _false;
			}

			// Update latency ( comes from connection peer ).
			if ( packet->mPacketSize == sizeof( PingReqPacket ) )
			{
				mLatency = ( pingreq->mLocalLatency + mLatency * 15 ) / 16;

				networkmanager.CallUpdateLatencyCallback( mLocalSessionID, mLocalConnectionID, mLatency );
	
				NetworkStatistics* netstat = (NetworkStatistics*) session->GetStatistics( );
				FL_ASSERT( netstat != _null )

				// Statistic latency.
				netstat->StatisticLatency( mLatency );
			}

			// Build ping ack packet.
			PingAckPacket pingack( pingreq->mTickcount );

			if ( ( mProperty & _PROPERTY_INITIATIVE ) == 0 && session->IsSessionTickcountEnabled( ) )
				pingack.mServerTickcount = session->GetSessionTickcount( );

			// Send ping ack packet back.
			SendPacket( &pingack, pingack.mPacketSize );

			return _true;
		}

		// Ping ack packet.
		case PingAckPacket::_MAGIC_CODE:
		{
			PingAckPacket* pingack = (PingAckPacket*) packet;

			_dword tickcount  = System::GetCurrentTickcount( );
			_dword newlatency = tickcount > pingack->mTickcount ? ( tickcount - pingack->mTickcount + 1 ) / 2 : 0;

			// Update latency.
			mLatency = ( newlatency + mLatency * 15 ) / 16;

			networkmanager.CallUpdateLatencyCallback( mLocalSessionID, mLocalConnectionID, mLatency );

			// Update session tickcount too.
			if ( session->IsSessionTickcountEnabled( ) != 0 && pingack->mServerTickcount != 0 )
				session->InitSessionTickcount( pingack->mServerTickcount + mLatency );

			NetworkStatistics* netstat = (NetworkStatistics*) session->GetStatistics( );
			FL_ASSERT( netstat != _null )

			// Statistic latency.
			netstat->StatisticLatency( mLatency );

			return _true;
		}

		default:
		{
			mLostReason.Format( L"ProcessPacket, Bad packet->mMagicCode(%d)", packet->mMagicCode );

			break;
		}
	}

	return _false;
}

_bool Connection::KeepAlive( _dword tickcount )
{
	if ( mPhase == INetworkSession::_PHASE_CLOSED )
		return _false;

	if ( mTimer != -1 )
	{
		_dword elapse = tickcount - mLastTickcount;

		// Update connection timer.
		if ( mTimer > elapse )
			mTimer -= elapse;
		else
			mTimer = 0;
	}

	mLastTickcount = tickcount;

	// If the wait alive tickcount is not updated for a while, then the connection is not alive, return false.
	if ( mPhase == INetworkSession::_PHASE_CONNECTED )
	{
		if ( mTimeout != -1 && tickcount - mWaitAliveTickcount > mTimeout )
		{
			mCloseReason = INetworkSession::_REASON_TIMEOUT;

			return _false;
		}
	}
	// Now this connection is at connecting phase.
	else
	{
		if ( tickcount - mWaitAliveTickcount > mConnectInterval )
		{
			mCloseReason = INetworkSession::_REASON_TIMEOUT;

			return _false;
		}
	}

	// There are some packet in delay array, we need to check.
	if ( mDelayPacketArray.Number( ) > 0 )
	{
		NetworkSession* session = (NetworkSession*) GetNetworkManager( ).SearchSession( mLocalSessionID );
		FL_ASSERT( session != _null )

		FL_ASSERT( session->mInitSessionTickcount != 0 )

		// Use current session tickcount to check delay packet.
		CheckDelayPacket( session->GetSessionTickcount( ) );
	}

	// It's time to send ping packet to keep the connection alive.
	if ( tickcount - mKeepAliveTickcount > _KEEPALIVE_INTERVAL )
	{
		// Update keep alive tickcount.
		mKeepAliveTickcount = tickcount;

		// Not connected, no need ping.
		if ( mPhase != INetworkSession::_PHASE_CONNECTED )
			return _true;

		// Not initiative side, no need ping.
		if ( ( mProperty & _PROPERTY_INITIATIVE ) == 0 )
			return _true;

		// Build ping packet.
		PingReqPacket pingreq( mLatency );

		// Send ping packet.
		SendPacket( &pingreq, pingreq.mPacketSize );
	}

	// It's time to update send/receive speed.
	if ( tickcount - mUpdateSpeedTickcount > 1000 )
	{
		// Update speed tickcount.
		mUpdateSpeedTickcount = tickcount;

		for ( _dword i = 0; i <= 8; i ++ )
		{
			mRecentSentBytes[ 8 - i + 1 ]		= mRecentSentBytes[ 8 - i ];
			mRecentReceivedBytes[ 8 - i + 1 ]	= mRecentReceivedBytes[ 8 - i ];
		}

		mRecentSentBytes[0]		= 0;
		mRecentReceivedBytes[0]	= 0;
	}

	return _true;
}

_void Connection::FlushFinish( )
{
}