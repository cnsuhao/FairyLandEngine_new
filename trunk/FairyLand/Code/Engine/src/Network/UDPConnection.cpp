//============================================================================
// UDPConnection.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// UDPConnection Implementation
//----------------------------------------------------------------------------

UDPConnection::UDPConnection( _dword socketid, const Address& localaddress, const Address& remoteaddress, BufferManager& buffermanager )
	: Connection( socketid, localaddress, remoteaddress, buffermanager ), mSendPacketQueue( 128, 64 )
{
	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	mUDPSocket = (UDPSocket*) networkmanager.SearchSocket( socketid );
	FL_ASSERT( mUDPSocket != _null )

	ClearPacketQueue( );
}

UDPConnection::~UDPConnection( )
{
	ClearPacketQueue( );
}

_bool UDPConnection::ResendHelper( Packet* packet )
{
	PacketBuffer sendbuffer;
	sendbuffer.mBuffer[0] = packet;
	sendbuffer.mLength[0] = packet->mPacketSize;

	// Create overlapped operation with given buffer, not create a new buffer.
	SocketOperation* operation = mBufferManager.CreateOperation( sendbuffer, _false );

	if ( operation == _null )
		return _false;

	// Set session id, connection id and buffer for operation.
	operation->mSessionID		= mLocalSessionID;
	operation->mConnectionID	= mLocalConnectionID;
	operation->mBuffer			= (_byte*) packet;
	operation->mLength			= packet->mPacketSize;

	// Set confirm SN and flag.
	packet->mConfirmSN			= mNextRecvPacketSN;
	packet->mConfirmFlag		= 0;

	if ( mConfirmTickcount != 0 )
	{
		packet->mConfirmFlag |= 1;
		mStartFastConfirm = _false;
	}

	#ifdef _DEBUG_UDP_CONNECTION
	PS_TRACE5( L"[UDPDEBUG] [%d] Confirm1 %d, RecvArray %d, SendQueue %d, Congestion %d\r\n", mLocalConnectionID, mNextRecvPacketSN, mRecvPacketArray.Number( ), mSendPacketQueue.Number( ), mCongestionWindow )
	#endif

	// Confirm is included in this packet, so no need confirm packet anymore.
	mConfirmTickcount = 0;

	// Post overlapped send operation.
	return mUDPSocket->Send( mInternalAddress, operation );
}

_bool UDPConnection::CheckPacketHelper( Packet* packet, _dword index )
{
	// Update the serial number of next packet to be received.
	mNextRecvPacketSN ++;

	// This packet contains more than one serial number.
	if ( packet->mPacketFlag == Packet::_PACKET_LISTWITHSN )
	{
		packet->mPacketFlag = Packet::_PACKET_LIST;

		if ( packet->mPacketSize < sizeof( Packet ) + sizeof( PacketEnding ) )
			return _false;

		packet->mPacketSize -= sizeof( PacketEnding );

		PacketEnding* packetending = (PacketEnding*) ( (_byte*) packet + packet->mPacketSize );
		mNextRecvPacketSN += packetending->mPacketNumber - 1;

		// Check if we can drop some packets already received.
		for ( _dword i = index; i < mRecvPacketArray.Number( ); i ++ )
		{
			if ( PacketUseful( mNextRecvPacketSN, mRecvPacketArray[i]->mPacketSN ) )
				break;

			// Release the packet buffer.
			mBufferManager.ReleaseBuffer( (_byte*) mRecvPacketArray[i] );

			// Remove the processed packet.
			mRecvPacketArray.Remove( i -- );
		}
	}

	return _true;
}

_bool UDPConnection::NeedConfirmBack( ) const
{
	if ( mStartFastConfirm )
		return _true;

	// No packet to be confirmed.
	if ( mConfirmTickcount == 0 )
		return _false;

	// Calculate confirm interval.
	_dword interval = mLatency / 2;

	if ( interval < _MIN_CONFIRM_INTERVAL )
		interval = _MIN_CONFIRM_INTERVAL;

	if ( interval > _MAX_CONFIRM_INTERVAL )
		interval = _MAX_CONFIRM_INTERVAL;

	// No right time to send confirm serial number packet.
	if ( mConfirmTickcount + interval > System::GetCurrentTickcount( ) )
		return _false;

	return _true;
}

_void UDPConnection::ConfirmPacketSN( _word packetsn, _bool confirmflag )
{
	#ifdef _DEBUG_UDP_CONNECTION
	PS_TRACE4( L"[UDPDEBUG] [%d] ConfirmSN At %d, Packet %d (%d)\r\n", mLocalConnectionID, System::GetCurrentTickcount( ), packetsn, confirmflag )
	#endif

	// No packet need to confirm.
	if ( mSendPacketQueue.Number( ) == 0 )
		return;

	if ( packetsn != mLastConfirmSN )
		mBadConfirmNumber = 0;

	// This is duplicate confirm.
	if ( confirmflag && packetsn == mLastConfirmSN )
	{
		mBadConfirmNumber ++;

		// It's time to avoid congestion.
		if ( mBadConfirmNumber == 3 )
		{
			// Adjust congestion window.
			CongestionHappen( _false );

			mStartFastResend = _true;
		}
		// It's time to expand congestion window.
		else if ( mBadConfirmNumber > 3 )
		{
			mCongestionWindow += _MIN_CONGESTION;

			if ( mCongestionWindow > _MAX_CONGESTION )
				mCongestionWindow = _MAX_CONGESTION;
		}

		return;
	}

	// Calculate the first confirmed packet serial number.
	_dword firstpacketsn = mSendPacketQueue.Front( ).mPacket->mPacketSN;

	// Calculate the number of confirmed packet.
	_dword confirmednumber = ( 0x10000 + (_dword) packetsn - firstpacketsn ) % 0x10000;

	// Invalid confirm packet, skip it.
	if ( confirmednumber == 0 || mSendPacketQueue.Number( ) < confirmednumber )
		return;

	// Reset resend tickcount.
	mResendTickcount = System::GetCurrentTickcount( );

	// We are in avoid congestion phase, now reset congestion window size.
	if ( mBadConfirmNumber >= 3 )
	{
		// Fast resend phase end.
		if ( confirmednumber > 1 )
		{
			// Clear bad confirm number.
			mBadConfirmNumber = 0;
			mStartFastResend  = _false;
		}
		// Only confirm one packet, we need fast resend again.
		else
		{
			mStartFastResend  = _true;
		}
	}

	// Remove confirmed packets.
	for ( _dword i = 0; i < confirmednumber; i ++ )
	{
		FL_ASSERT( mSendPacketQueue.Number( ) > 0 )

		// Get the packet confirmed from front of send queue.
		UnconfirmedPacket unconfirmedpacket = mSendPacketQueue.Front( );

		// Update lantency.
		if ( i == 0 )
		{
			NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

			_dword tickcount  = System::GetCurrentTickcount( );
			_dword newlatency = tickcount > unconfirmedpacket.mPostTickcount ? ( tickcount - unconfirmedpacket.mPostTickcount ) / 2 : 0;

			mLatency = ( newlatency + mLatency * 15 ) / 16;

			networkmanager.CallUpdateLatencyCallback( mLocalSessionID, mLocalConnectionID, mLatency );

			INetworkSession* session = networkmanager.SearchSession( mLocalSessionID );
			FL_ASSERT( session != _null )

			NetworkStatistics* netstat = (NetworkStatistics*) session->GetStatistics( );
			FL_ASSERT( netstat != _null )

			netstat->StatisticLatency( mLatency );
		}

		// Adjust unconfirmed packet size.
		if ( mUnconfirmPacketSize > unconfirmedpacket.mPacket->mPacketSize )
			mUnconfirmPacketSize -= unconfirmedpacket.mPacket->mPacketSize;
		else
			mUnconfirmPacketSize = 0;

		// Now we are at avoid congestion phase.
		if ( mCongestionWindow > mSlowStartThreshold && mBadConfirmNumber < 3 )
		{
			mCongestionWindow += _MIN_CONGESTION * _MIN_CONGESTION / mCongestionWindow;
		}
		// Now we are at slow start phase.
		else
		{
			mCongestionWindow += _MIN_CONGESTION;
		}

		// Release the packet buffer.
		mBufferManager.ReleaseBuffer( (_byte*) unconfirmedpacket.mPacket );

		// Remove the confirmed packet from the front of send queue.
		mSendPacketQueue.Remove( );

		// Decrease unconfirmed packet number, maybe confirmed number is larger than unconfirmed number,
		// because some packet is saved in peer candidate packet array, and confirmed all in one time.
		if ( mSendPacketIndex > 0 )
			mSendPacketIndex --;

		if ( mCongestionWindow > _MAX_CONGESTION )
			mCongestionWindow = _MAX_CONGESTION;
	}

	#ifdef _DEBUG_UDP_CONNECTION
	PS_TRACE3( L"[UDPDEBUG] [%d] ConfirmPacket %d, SendPacketIndex %d\r\n", mLocalConnectionID, confirmednumber, mSendPacketIndex )
	#endif
}

_void UDPConnection::CongestionHappen( _bool timeout )
{
	if ( mPhase != INetworkSession::_PHASE_CONNECTED )
	{
		mUnconfirmPacketSize = 0;
		mSendPacketIndex	 = 0;
		return;
	}

	// Dont adjust threshold for high priority connection.
	if ( mPriority <= INetworkManager::_NETWORK_PRIORITY_NORMAL )
	{
		// Set threshold as half congestion window.
		mSlowStartThreshold = mCongestionWindow / 2;

		// Make threshold as _MIN_CONGESTION multiple.
		mSlowStartThreshold = ( mSlowStartThreshold + _MIN_CONGESTION / 2 ) / _MIN_CONGESTION * _MIN_CONGESTION;

		if ( mSlowStartThreshold < _MIN_THRESHOLD )
			mSlowStartThreshold = _MIN_THRESHOLD;

		if ( mSlowStartThreshold > _MAX_THRESHOLD )
			mSlowStartThreshold = _MAX_THRESHOLD;
	}

	// Congestion happens because of timeout.
	if ( timeout )
	{
		// Dont adjust congestion window for high priority connection.
		if ( mPriority <= INetworkManager::_NETWORK_PRIORITY_NORMAL )
			mCongestionWindow = _MIN_CONGESTION;

		// Clear unconfirmed packet number, ready to resend all.
		mUnconfirmPacketSize = 0;
		mSendPacketIndex	 = 0;

		#ifdef _DEBUG_UDP_CONNECTION
		PS_TRACE1( L"[UDPDEBUG] [%d] Congestion happens because of timeout\r\n", mLocalConnectionID )
		#endif
	}
	// Congestion happens because of bad confirm.
	else
	{
		// Dont adjust congestion window for high priority connection.
		if ( mPriority <= INetworkManager::_NETWORK_PRIORITY_NORMAL )
		{
			mCongestionWindow = mSlowStartThreshold + _MIN_CONGESTION * 3;

			if ( mCongestionWindow > _MAX_CONGESTION )
				mCongestionWindow = _MAX_CONGESTION;
		}

		#ifdef _DEBUG_UDP_CONNECTION
		PS_TRACE1( L"[UDPDEBUG] [%d] Congestion happens because of bad confirm\r\n", mLocalConnectionID )
		#endif
	}
}

_void UDPConnection::FlushPacketList( _byte*& packetlist, _dword& bufferindex, _dword& packetnumber )
{
	// Copy packet ending.
	Memory::MemCpy( packetlist + bufferindex, &PacketEnding( (_word) packetnumber, 0 ), sizeof( PacketEnding ) );
	bufferindex += sizeof( PacketEnding );

	FL_ASSERT( bufferindex <= mBufferManager.GetMaxPacketSize( ) )

	( (Packet*) packetlist )->mPacketFlag = Packet::_PACKET_LISTWITHSN;
	( (Packet*) packetlist )->mPacketSize = (_word) bufferindex;

	// Resend the packet, if success, the buffer will be released when finished, so we dont need care.
	if ( ResendHelper( (Packet*) packetlist ) )
		packetlist = _null;

	bufferindex  = 0;
	packetnumber = 0;
}

_void UDPConnection::SendConfirmPacket( )
{
	if ( NeedConfirmBack( ) == _false )
		return;

	mStartFastConfirm = _false;

	// Build confirm serial number packet.
	ConfirmSNPacket confirmsn;

	// Send confirm serial number packet.
	SendPacket( &confirmsn, confirmsn.mPacketSize );
}

_void UDPConnection::ResendPacketQueue( _dword sendnumber )
{
	FL_ASSERT( mSendPacketIndex <= mSendPacketQueue.Number( ) )

	if ( mSendPacketQueue.Number( ) == 0 )
		return;

	#ifdef _DEBUG_UDP_CONNECTION
	PS_TRACE4( L"[UDPDEBUG] [%d] ResendPacketQueue %d, Threshold %d, Congestion %d\r\n", mLocalConnectionID, mSendPacketQueue.Number( ), mSlowStartThreshold, mCongestionWindow )
	#endif

	if ( sendnumber == -1 )
	{
		// So many packet unconfirmed, or no more packet need to resend, just return.
		if ( mUnconfirmPacketSize >= mCongestionWindow )
		{
			if ( NeedConfirmBack( ) == _false )
				return;

			// If we need confirm back, we must send some packet now, so just increase congestion window.
			mCongestionWindow += _MIN_CONGESTION;

			#ifdef _DEBUG_UDP_CONNECTION
			PS_TRACE2( L"[UDPDEBUG] [%d] Enlarge congestion window => %d\r\n", mLocalConnectionID, mCongestionWindow )
			#endif
		}
	}

	_byte* packetlist = _null; _dword bufferindex = 0, packetnumber = 0;

	// Get the packet index need to resend.
	_dword startindex = ( sendnumber == -1 ) ? mSendPacketIndex : 0;

	// Resend all packets in the send queue.
	for ( PacketQueue::Iterator it = mSendPacketQueue.GetIterator( startindex ); it.Valid( ); it ++ )
	{
		Packet* packet = ( (UnconfirmedPacket&) it ).mPacket;

		if ( sendnumber == -1 )
		{
			// So many unconfirmed packet sending, stop sending others.
			if ( mUnconfirmPacketSize + packet->mPacketSize > mCongestionWindow )
				break;

			// Increase unconfirmed packet size.
			mUnconfirmPacketSize += packet->mPacketSize;

			// Increase send packet index.
			mSendPacketIndex ++;
		}

		// This packet is sent more than one time.
		if ( packet->mVersion != 0xFF )
		{
			NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

			networkmanager.StatisticResentPacket( packet->mPacketSize );

			#ifdef _DEBUG_UDP_CONNECTION
			PS_TRACE2( L"[UDPDEBUG] [%d] ResendPacket %d\r\n", mLocalConnectionID, packet->mPacketSN )
			#endif
		}
		else
		{
			// Set packet version.
			packet->mVersion = Packet::_PACKET_VERSION;

			#ifdef _DEBUG_UDP_CONNECTION
			PS_TRACE2( L"[UDPDEBUG] [%d] SendPacket %d\r\n", mLocalConnectionID, packet->mPacketSN )
			#endif
		}

		// Get buffer size of the packet, exclude packet header.
		_dword buffersize = packet->mPacketSize - sizeof( Packet );

		// Decide if we can resend use packet list.
		_bool enableresendlist = packet->CanBeCombined( ) && mPhase == INetworkSession::_PHASE_CONNECTED;

		if ( ( mProperty & _PROPERTY_RESENDWITHSN ) == 0 )
			enableresendlist = _false;
		if ( ( mProperty & _PROPERTY_COMPRESSPACKET ) != 0 )
			enableresendlist = _false;

		// Now use current buffer size and next buffer size to decide if we can put it into list.
		if ( enableresendlist )
		{
			enableresendlist = _false;

			// 1. We already have some packets in list.
			if ( packetnumber > 0 )
			{
				_dword needspace = bufferindex + buffersize + sizeof( PacketEnding );

				if ( packet->mPacketFlag == Packet::_PACKET_NORMAL )
					needspace += sizeof( PacketDivision );

				// This packet can fit into list, we can combine.
				if ( needspace <= mBufferManager.GetMaxPacketSize( ) )
					enableresendlist = _true;
			}

			// 2. We try to combine with next packet.
			if ( enableresendlist == _false )
			{
				_dword needspace = sizeof( Packet ) + buffersize + sizeof( PacketEnding );

				if ( packet->mPacketFlag == Packet::_PACKET_NORMAL )
					needspace += sizeof( PacketDivision );

				// Get next packet size, used for combine as packet list.
				_dword nextbuffersize = 0;

				it ++;

				if ( it.Valid( ) )
				{
					Packet* nextpacket = ( (UnconfirmedPacket&) it ).mPacket;

					// Make sure it can be combined.
					if ( packet->CanBeCombined( ) )
					{
						Packet* nextpacket = ( (UnconfirmedPacket&) it ).mPacket;

						nextbuffersize = nextpacket->mPacketSize - sizeof( Packet );
						needspace += nextbuffersize;

						if ( nextpacket->mPacketFlag == Packet::_PACKET_NORMAL )
							needspace += sizeof( PacketDivision );
					}
				}

				it --;

				// 2.1 Next packet can be combined too, and its size is small enough too.
				if ( nextbuffersize != 0 && needspace <= mBufferManager.GetMaxPacketSize( ) )
					enableresendlist = _true;
			}
		}

		// Try to combine packet into packet list, resend together.
		if ( enableresendlist )
		{
			_dword needspace = buffersize + sizeof( PacketEnding );

			if ( packet->mPacketFlag == Packet::_PACKET_NORMAL )
				needspace += sizeof( PacketDivision );

			// Buffer already too big, flush it first.
			if ( bufferindex + needspace > mBufferManager.GetMaxPacketSize( ) )
			{
				FlushPacketList( packetlist, bufferindex, packetnumber );

				// We just need send the first one packet, so stop now.
				if ( sendnumber != -1 )
					sendnumber --;
				if ( sendnumber == 0 )
					break;
			}

			// Create buffer to send packet list.
			if ( packetlist == _null )
				packetlist = mBufferManager.CreateBuffer( );

			// Copy packet header.
			if ( bufferindex == 0 )
			{
				Memory::MemCpy( packetlist, packet, sizeof( Packet ) );
				bufferindex += sizeof( Packet );
			}

			if ( packet->mPacketFlag == Packet::_PACKET_NORMAL )
			{
				// Copy packet division.
				Memory::MemCpy( packetlist + bufferindex, &PacketDivision( buffersize + sizeof( PacketDivision ) ), sizeof( PacketDivision ) );
				bufferindex += sizeof( PacketDivision );
			}

			// Copy packet buffer.
			Memory::MemCpy( packetlist + bufferindex, (_byte*) packet + sizeof( Packet ), buffersize );
			bufferindex += buffersize;

			packetnumber ++;
		}
		// Send directly.
		else
		{
			// Flush unfinished packet list.
			if ( bufferindex != 0 )
			{
				FlushPacketList( packetlist, bufferindex, packetnumber );

				// We just need send the first one packet, so stop now.
				if ( sendnumber != -1 )
					sendnumber --;
				if ( sendnumber == 0 )
					break;
			}

			// Resend the packet, if success, the buffer will be released when finished, so we need reserve again.
			if ( ResendHelper( packet ) )
				mBufferManager.ReserveBuffer( (_byte*) packet );

			// We just need send the first one packet, so stop now.
			if ( sendnumber != -1 )
				sendnumber --;
			if ( sendnumber == 0 )
				break;
		}
	}

	// Flush unfinished packet list.
	if ( bufferindex != 0 )
		FlushPacketList( packetlist, bufferindex, packetnumber );

	// Release packet list buffer.
	if ( packetlist != _null )
		mBufferManager.ReleaseBuffer( packetlist );
}

_void UDPConnection::CheckResendPacket( )
{
	// We can start fast resend.
	if ( mStartFastResend )
	{
		#ifdef _DEBUG_UDP_CONNECTION
		PS_TRACE4( L"[UDPDEBUG] [%d] Fast resend At %d, Threshold %d, Congestion %d\r\n", mLocalConnectionID, System::GetCurrentTickcount( ), mSlowStartThreshold, mCongestionWindow );
		#endif

		// Start to resend packets.
		ResendPacketQueue( 1 );

		mStartFastResend = _false;
	}

	// Start to resend packets.
	ResendPacketQueue( -1 );
}

_void UDPConnection::ClearPacketQueue( )
{
	mCongestionWindow		= _MIN_CONGESTION;
	mSlowStartThreshold		= _MAX_THRESHOLD;
	mUnconfirmPacketSize	= 0;
	mSendPacketIndex		= 0;

	mBadConfirmNumber		= 0;
	mLastConfirmSN			= 0xFFFF;
	mNextSendPacketSN		= 0;
	mNextRecvPacketSN		= 0;

	mStartFastResend		= _false;
	mStartFastConfirm		= _false;
	mConfirmTickcount		= 0;
	mResendTickcount		= System::GetCurrentTickcount( );
	mResendNumber			= 0;

	// Clear all packets in the send queue.
	for ( PacketQueue::Iterator it1 = mSendPacketQueue.GetHeadIterator( ); it1.Valid( ); it1 ++ )
	{
		Packet* packet = ( (UnconfirmedPacket&) it1 ).mPacket;

		// Release the packet buffer.
		mBufferManager.ReleaseBuffer( (_byte*) packet );
	}

	// Clear all packets in the receive array.
	for ( PacketArray::Iterator it2 = mRecvPacketArray.GetHeadIterator( ); it2.Valid( ); it2 ++ )
	{
		Packet* packet = it2;

		// Release the packet buffer.
		mBufferManager.ReleaseBuffer( (_byte*) packet );
	}

	mSendPacketQueue.Clear( );
	mRecvPacketArray.Clear( );
}

_dword UDPConnection::GetConnectionType( )
{
	return Address::_TYPE_UDP;
}

_bool UDPConnection::Connect( )
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

	// Build join req packet.
	JoinReqPacket joinreq( (_word) mLocalSessionID, (_word) mLocalConnectionID );
	joinreq.mPacketProperty |= _PROPERTY_RELIABLEPING;

	// Ask for compress mode.
	if ( mProperty & _PROPERTY_ASKCOMPRESS )
		joinreq.mPacketProperty |= _PROPERTY_ASKCOMPRESS;

	// Ping mode.
	if ( mProperty & _PROPERTY_PINGONLY )
		joinreq.mPacketProperty |= _PROPERTY_PINGONLY;

	// Set connection phase -> _PHASE_SENTJOINREQ.
	mPhase = INetworkSession::_PHASE_SENTJOINREQ;

	// Send join req packet.
	if ( SendPacket( &joinreq, joinreq.mPacketSize ) == _false )
		return _false;

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	// Send connect req packet to NAT helper server at same time.
	if ( ( mProperty & _PROPERTY_PINGONLY ) == 0 )
		networkmanager.ConnectUseHelper( mSocketID, mLocalSessionID, mRemoteAddress );

	return _true;
}

_void UDPConnection::Close( )
{
	if ( mPhase == INetworkSession::_PHASE_CONNECTED )
	{
		FlushDeferPacket( );

		if ( ( mProperty & _PROPERTY_PINGONLY ) == 0 )
		{
			// Build close req packet.
			CloseReqPacket closereq;

			// Send close req packet.
			SendPacket( &closereq, closereq.mPacketSize );
		}
	}

	ClearDelayPacket( );

	// Set connection phase -> _PHASE_CLOSED.
	mPhase = INetworkSession::_PHASE_CLOSED;
}

_bool UDPConnection::Send( SocketOperation* operation )
{
	if ( operation == _null )
		return _false;

	// Set session id and connection id.
	operation->mSessionID		= mLocalSessionID;
	operation->mConnectionID	= mLocalConnectionID;

	Packet* packet = (Packet*) operation->mBuffer;

	// This packet no need confirm
	if ( mProperty & _PROPERTY_NONEEDCONFIRM )
	{
		// These two packet must be confirmed.
		if ( packet->mPacketFlag != Packet::_PACKET_BIGPACKETBEGIN && packet->mPacketFlag != Packet::_PACKET_BIGPACKETDATA )
			packet->mPacketProperty |= _PROPERTY_NONEEDCONFIRM;
	}

	// Check the buffer to be a valid packet.
	FL_ASSERT( operation->mLength >= sizeof( Packet ) && operation->mLength == packet->mPacketSize )

	// Set secret code, session id and target connection id.
	packet->mSecretCode		= (_word) mSecretCode;
	packet->mSessionID		= (_word) mRemoteSessionID;
	packet->mConnectionID	= (_word) mRemoteConnectionID;

	// Set confirm SN and flag.
	packet->mConfirmSN		= mNextRecvPacketSN;
	packet->mConfirmFlag	= 0;

	// Check if this packet need confirm.
	_bool needconfirm = PacketNeedConfirm( packet->mMagicCode, packet->mPacketProperty ), sendnow = _true;

	if ( needconfirm )
	{
		// Set packet serial number.
		packet->mPacketSN = mNextSendPacketSN;

		// Update the serial number of next packet to be sent.
		mNextSendPacketSN ++;

		// Reset resend tickcount.
		if ( mSendPacketQueue.Number( ) == 0 )
			mResendTickcount = System::GetCurrentTickcount( );

		// There are some packet in queue before this one, we cant send now.
		if ( mSendPacketIndex < mSendPacketQueue.Number( ) || mUnconfirmPacketSize + packet->mPacketSize > mCongestionWindow )
			sendnow = _false;

		// Maybe need resend this packet, so push it into send queue.
		UnconfirmedPacket unconfirmedpacket;
		unconfirmedpacket.mPacket		 = packet;
		unconfirmedpacket.mPostTickcount = System::GetCurrentTickcount( );
		mSendPacketQueue.Append( unconfirmedpacket );

		// Reserve the buffer for later resend.
		mBufferManager.ReserveBuffer( operation->mBuffer );
	}
	else
	{
		if ( mUnconfirmPacketSize + _MIN_CONGESTION > mCongestionWindow )
			sendnow = _false;
	}

	#ifdef _DEBUG_UDP_CONNECTION
	PS_TRACE4( L"[UDPDEBUG] [%d] Send packet, SendPacketQueue %d, Unconfirmed %d, Congestion %d\r\n", mLocalConnectionID, mSendPacketQueue.Number( ), mUnconfirmPacketSize, mCongestionWindow )
	#endif

	// We need stop send packet other than confirm.
	if ( packet->mMagicCode != ConfirmSNPacket::_MAGIC_CODE )
	{
		// So many unconfirmed packet sending, stop sending others.
		if ( sendnow == _false )
		{
			// Set delay send flag.
			packet->mVersion = 0xFF;

			// Send operation not performed, so release the operation.
			mBufferManager.ReleaseOperation( operation );

			#ifdef _DEBUG_UDP_CONNECTION
			PS_TRACE3( L"[UDPDEBUG] [%d] Defer because of Unconfirmed %d and Congestion %d\r\n", mLocalConnectionID, mUnconfirmPacketSize, mCongestionWindow )
			#endif

			return _true;
		}
	}

	// Data packet need flow control, now ready to send it, increase send queue index.
	if ( needconfirm )
	{
		mUnconfirmPacketSize += packet->mPacketSize;
		mSendPacketIndex ++;
	}

	// Set confirm SN and flag.
	if ( mConfirmTickcount != 0 )
	{
		packet->mConfirmFlag |= 1;
		mStartFastConfirm = _false;
	}

	// Confirm is included in this packet, so no need confirm packet anymore.
	mConfirmTickcount = 0;

	#ifdef _DEBUG_UDP_CONNECTION
	PS_TRACE2( L"[UDPDEBUG] [%d] SendPacket %d\r\n", mLocalConnectionID, packet->mPacketSN )
	PS_TRACE5( L"[UDPDEBUG] [%d] Confirm2 %d, RecvArray %d, SendQueue %d, Congestion %d\r\n", mLocalConnectionID, mNextRecvPacketSN, mRecvPacketArray.Number( ), mSendPacketQueue.Number( ), mCongestionWindow )
	#endif

	// Post overlapped send operation.
	return mUDPSocket->Send( mInternalAddress, operation );
}

_bool UDPConnection::Recv( )
{
	if ( mPhase == INetworkSession::_PHASE_CLOSED )
		return _false;

	// Post overlapped received operation.
	return mUDPSocket->Recv( mBufferManager.CreateOperation( ) );
}

_bool UDPConnection::ProcessData( _byte* buffer, _dword length )
{
	if ( mPhase == INetworkSession::_PHASE_CLOSED )
	{
		mLostReason.Format( L"ProcessData, mPhase == INetworkSession::_PHASE_CLOSED" );

		return _false;
	}

	// Some data received, reset resend number.
	mResendNumber = 0;

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	// Try to analyse data buffer into packet.
	while ( length >= sizeof( Packet ) )
	{
		Packet* packet = (Packet*) buffer;

		#ifdef _DEBUG_UDP_CONNECTION
		PS_TRACE2( L"[UDPDEBUG] [%d] ProcessData %d\r\n", mLocalConnectionID, packet->mPacketSN );
		#endif

		// Invalid packet, just skip it.
		if ( packet->mPacketSize > length )
		{
			mLostReason.Format( L"ProcessData, packet->mPacketSize(%d) > length(%d)", packet->mPacketSize, length );

			return _false;
		}

		// Try to confirm packet.
		ConfirmPacketSN( packet->mConfirmSN, packet->mConfirmFlag );

		// Set last confirm SN.
		mLastConfirmSN = packet->mConfirmSN;

		buffer += packet->mPacketSize;
		length -= packet->mPacketSize;

		// These packets need not check serial number.
		if ( PacketNeedConfirm( packet->mMagicCode, packet->mPacketProperty ) == _false )
		{
			networkmanager.StatisticPacketsInOrder( );

			// Process the packet.
			if ( ProcessPacket( packet ) == _false )
				return _false;

			continue;
		}

		// Ready to send confirm packet.
		mConfirmTickcount = System::GetCurrentTickcount( );

		// The packet is arrived in order, so process it.
		if ( packet->mPacketSN == mNextRecvPacketSN )
		{
			networkmanager.StatisticPacketsInOrder( );

			// Check packet, and update serial number.
			if ( CheckPacketHelper( packet, 0 ) == _false )
				return _false;

			// Process the packet.
			if ( ProcessPacket( packet ) == _false )
				return _false;

			// Maybe more packet can be processed.
			for ( _dword i = 0; i < mRecvPacketArray.Number( ); i ++ )
			{
				// Check serial number.
				if ( mNextRecvPacketSN != mRecvPacketArray[i]->mPacketSN )
					break;

				// Check packet, and update serial number.
				if ( CheckPacketHelper( mRecvPacketArray[i], i + 1 ) == _false )
					return _false;

				// Process the packet.
				if ( ProcessPacket( mRecvPacketArray[i] ) == _false )
					return _false;

				// Release the packet buffer.
				mBufferManager.ReleaseBuffer( (_byte*) mRecvPacketArray[i] );

				// Remove the processed packet.
				mRecvPacketArray.Remove( i -- );
			}

			// This is in order packet, we can delay confirm.
			mStartFastConfirm = _false;
		}
		// The packet is not arrived in order, save it for later use.
		else if ( PacketUseful( mNextRecvPacketSN, packet->mPacketSN ) )
		{
			_dword index = mRecvPacketArray.Number( );

			// Find the index to place the packet in received packet array.
			for ( _dword i = 0; i < mRecvPacketArray.Number( ); i ++ )
			{
				// Found the packet with same serial number, just drop it.
				if ( mRecvPacketArray[i]->mPacketSN == packet->mPacketSN )
				{
					networkmanager.StatisticPacketsDroped( );

					index = -1;
					break;
				}

				// Found the packet with bigger serial number, put the new packet before it.
				if ( mRecvPacketArray[i]->mPacketSN > packet->mPacketSN )
				{
					networkmanager.StatisticPacketsInDisorder( );

					index = i;
					break;
				}
			}

			if ( index != -1 )
			{
				// Reserve the buffer for later resend.
				mBufferManager.ReserveBuffer( (_byte*) packet );

				// Put the packet into array.
				mRecvPacketArray.Insert( packet, index );
			}
			else
			{
				networkmanager.StatisticPacketsDroped( );

				#ifdef _DEBUG_UDP_CONNECTION
				PS_TRACE2( L"[UDPDEBUG] [%d] DropA %d\r\n", mLocalConnectionID, packet->mPacketSN );
				#endif
			}

			// This is disorder packet, we need confirm now.
			mStartFastConfirm = _true;
		}
		else
		{
			networkmanager.StatisticPacketsDroped( );

			#ifdef _DEBUG_UDP_CONNECTION
			PS_TRACE2( L"[UDPDEBUG] [%d] DropB %d\r\n", mLocalConnectionID, packet->mPacketSN );
			#endif

			// This is disorder packet, we need confirm now.
			mStartFastConfirm = _true;
		}
	}

	// Invalid packet.
	if ( length > 0 )
	{
		mLostReason.Format( L"ProcessData, length(%d) > 0", length );

		return _false;
	}

	return _true;
}

_bool UDPConnection::ProcessPacket( Packet* packet )
{
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

	if ( packet->mMagicCode == JoinHintPacket::_MAGIC_CODE )
	{
		JoinHintPacket* joinhint = (JoinHintPacket*) packet;

		// Clear packet queue before restart connect.
		ClearPacketQueue( );

		// Build join req packet.
		JoinReqPacket joinreq( (_word) mLocalSessionID, (_word) mLocalConnectionID );
		joinreq.mPacketProperty |= _PROPERTY_RELIABLEPING;

		// Ask for compress mode.
		if ( mProperty & _PROPERTY_ASKCOMPRESS )
			joinreq.mPacketProperty |= _PROPERTY_ASKCOMPRESS;

		// Send join req packet.
		if ( SendPacket( &joinreq, joinreq.mPacketSize ) == _false )
			return _false;
	}
	else if ( packet->mMagicCode != ConfirmSNPacket::_MAGIC_CODE )
	{
		// Process the packet.
		if ( Connection::ProcessPacket( packet ) == _false )
			return _false;
	}

	return _true;
}

_bool UDPConnection::KeepAlive( _dword tickcount )
{
	if ( Connection::KeepAlive( tickcount ) == _false )
		return _false;

	if ( mPriority != INetworkManager::_NETWORK_PRIORITY_MANAGED )
		CheckResendPacket( );

	// Calculate resend interval.
	_dword interval = mLatency * 2 + 100;

	if ( interval < _MIN_RESEND_INTERVAL )
		interval = _MIN_RESEND_INTERVAL;

	if ( interval > _MAX_RESEND_INTERVAL )
		interval = _MAX_RESEND_INTERVAL;

	// Increase resend interval by resend time.
	if ( mPriority != INetworkManager::_NETWORK_PRIORITY_MANAGED )
		interval += mResendNumber * _AVG_RESEND_INTERVAL;

	// It's time to resend unconfirmed packets.
	if ( mResendTickcount + interval <= System::GetCurrentTickcount( ) )
	{
		if ( mSendPacketQueue.Number( ) > 0 )
		{
			// Connecting, using NAT helper again.
			if ( mPhase == INetworkSession::_PHASE_SENTJOINREQ )
			{
				NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

				// Send connect req packet to NAT helper server at same time.
				if ( ( mProperty & _PROPERTY_PINGONLY ) == 0 )
					networkmanager.ConnectUseHelper( mSocketID, mLocalSessionID, mRemoteAddress );
			}

			#ifdef _DEBUG_UDP_CONNECTION
			PS_TRACE4( L"[UDPDEBUG] [%d] Timeout At %d, Threshold %d, Congestion %d\r\n", mLocalConnectionID, System::GetCurrentTickcount( ), mSlowStartThreshold, mCongestionWindow );
			#endif

			// Adjust congestion window.
			CongestionHappen( _true );

			// Start to resend packets.
			ResendPacketQueue( -1 );

			// Increase resend number, so resend interval will be longer next time.
			if ( mResendNumber < 3 )
				mResendNumber ++;
		}

		// Reset resend tickcount.
		mResendTickcount = System::GetCurrentTickcount( );
	}

	// It's time to send confirm serial number packet.
	if ( mPriority != INetworkManager::_NETWORK_PRIORITY_MANAGED )
		SendConfirmPacket( );

	return _true;
}

_void UDPConnection::FlushFinish( )
{
	if ( mPriority != INetworkManager::_NETWORK_PRIORITY_MANAGED )
		return;

	// Check if there is any packet need resend.
	CheckResendPacket( );

	// It's time to send confirm serial number packet.
	SendConfirmPacket( );
}