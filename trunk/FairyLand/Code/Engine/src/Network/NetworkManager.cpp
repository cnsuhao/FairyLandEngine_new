//============================================================================
// NetworkManager.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// NetworkManager Implementation
//----------------------------------------------------------------------------

NetworkManager::NetworkManager( )
	: mSocketPool( 65536 ), mSessionPool( 65536 ), mNATHelper( mBufferManager ), mAgentHelper( mBufferManager )
{
	WSADATA data;
	::WSAStartup( 0x0202, &data );

	mTCPSocketAcceptFunc		= _null;
	mTCPSocketConnectFunc		= _null;
	mTCPSocketDisconnectFunc	= _null;
	mTCPSocketReceiveFunc		= _null;
	mUDPSocketReceiveFunc		= _null;

	mCreateConnectionFunc		= _null;
	mDeleteConnectionFunc		= _null;
	mReceivePacketFunc			= _null;
	mUpdateLatencyFunc			= _null;
	mJoinFailedFunc				= _null;
	mUpdateSessionFunc			= _null;
	mNetworkTimerFunc			= _null;

	mLastTickcount				= System::GetCurrentTickcount( );
	mGlobalToken				= 0;
	mNetworkLatency				= 0;
	mLastReservedBuffer			= _null;

	mEnablePollingMode			= _true;

	mSocketIOCP.Create( );

	mNetworkThread.RegisterUpdateFunc( IOCPThread, this, _null );
	mNetworkThread.RegisterUpdateFunc( TimerThread, this, _null );
	mNetworkThread.Create( 1 );

	mSocketPool.Insert( _null, 0 );
	mSessionPool.Insert( _null, 0 );

	// Create and UDP socket, for all outside connection.
	Socket* socket = CreateSocket( Address( 0, Address::_TYPE_UDP, 0 ) );
	if ( socket != _null )
		socket->IncreaseRefCount( );

	ClearNetworkStatistic( );
}

NetworkManager::~NetworkManager( )
{
	mNetworkThread.Close( );

	mNetworkLock.Enter( );

	// Close all sessions.
	for ( SessionPool::Iterator it = mSessionPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		INetworkSession* session = it;

		ReleaseSession( session );
	}

	// Close all sockets.
	for ( SocketPool::Iterator it = mSocketPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Socket* socket = it;

		if ( socket == _null )
			continue;

		// Release socket defer queue.
		if ( socket->mDeferQueue.Number( ) > 0 )
		{
			for ( SocketOperationQueue::Iterator it = socket->mDeferQueue.GetHeadIterator( ); it.Valid( ); it ++ )
				mBufferManager.ReleaseOperation( (SocketOperation*) it );
		}

		// Release socket accept buffer.
		if ( socket->mAcceptBuffer != _null )
			mBufferManager.ReleaseBuffer( socket->mAcceptBuffer );

		delete socket;
	}

	// Free delay operations.
	for ( _dword i = 0; i < mDelayOperationArray.Number( ); i ++ )
		mBufferManager.ReleaseBuffer( mDelayOperationArray[i].mOperation.mBuffer );

	// Free defer TCP connect operations.
	for ( _dword i = 0; i < mDeferConnectArray.Number( ); i ++ )
		mBufferManager.ReleaseOperation( mDeferConnectArray[i] );

	// Free unfinished operations.
	while ( _true )
	{
		SocketOperation* operation = _null; _dword bytestransferred = 0, completionkey = 0;

		mSocketIOCP.GetQueuedStatus( bytestransferred, completionkey, (Overlapped*&) operation, 0 );

		// Free the memory used by operation.
		if ( operation != _null )
			mBufferManager.ReleaseOperation( operation );

		if ( ::GetLastError( ) == WAIT_TIMEOUT )
			break;
	}

	mNetworkLock.Leave( );

	::WSACleanup( );

	// Dump network statistic.
	DumpNetworkStatistic( );

	GetFairyLandGlobal( ).mNetworkManager = _null;
}

_bool NetworkManager::IOCPThread( _void* parameter1, _void* parameter2 )
{
	NetworkManager* networkmanager = (NetworkManager*) parameter1;
	FL_ASSERT( networkmanager != _null )

	//networkmanager->mNetworkLock.Enter( );

	//// Begin process async operation.
	//networkmanager->mAsyncNetworkManager.BeginProcess( );

	//AsyncOperationArray& operationarray = networkmanager->mAsyncNetworkManager.GetProcessOperationArray( );

	//for ( _dword i = 0; i < operationarray.Number( ); i ++ )
	//{
	//	AsyncOperation operation = operationarray[i];

	//	// Search the session of the operation on.
	//	NetworkSession* session = (NetworkSession*) networkmanager->SearchSession( operation.mSessionID );

	//	if ( session == _null )
	//		continue;

	//	switch ( operation.mOperationType )
	//	{
	//		case AsyncOperation::_ASYNC_SENDPACKET:
	//			session->SendPacket( operation.mParameter1, operation.mBuffer, operation.mLength );
	//			break;

	//		case AsyncOperation::_ASYNC_SENDDEFERPACKET:
	//			session->SendDeferPacket( operation.mParameter1, operation.mBuffer, operation.mLength );
	//			break;

	//		case AsyncOperation::_ASYNC_FLUSHDEFERPACKET:
	//			session->FlushDeferPacket( operation.mParameter1 );
	//			break;

	//		case AsyncOperation::_ASYNC_SENDGROUPPACKET:
	//			session->SendGroupPacket( operation.mParameter1, operation.mParameter2, operation.mBuffer, operation.mLength );
	//			break;

	//		case AsyncOperation::_ASYNC_SENDGROUPDEFERPACKET:
	//			session->SendGroupDeferPacket( operation.mParameter1, operation.mParameter2, operation.mBuffer, operation.mLength );
	//			break;

	//		case AsyncOperation::_ASYNC_FLUSHGROUPDEFERPACKET:
	//			session->FlushGroupDeferPacket( operation.mParameter1, operation.mParameter2 );
	//			break;

	//		case AsyncOperation::_ASYNC_SENDSESSIONPACKET:
	//			session->SendSessionPacket( operation.mParameter1, operation.mParameter2, operation.mBuffer, operation.mLength );
	//			break;

	//		case AsyncOperation::_ASYNC_SENDSESSIONDEFERPACKET:
	//			session->SendSessionDeferPacket( operation.mParameter1, operation.mParameter2, operation.mBuffer, operation.mLength );
	//			break;

	//		case AsyncOperation::_ASYNC_FLUSHSESSIONDEFERPACKET:
	//			session->FlushSessionDeferPacket( operation.mParameter1, operation.mParameter2 );
	//			break;
	//	}
	//}

	//// End process async operation.
	//networkmanager->mAsyncNetworkManager.EndProcess( );

	//networkmanager->mNetworkLock.Leave( );

	// Process defer connection.
	if ( networkmanager->mDeferConnectArray.Number( ) > 0 )
	{
		networkmanager->mNetworkLock.Enter( );

		// TODO, handle connect failed event.
		for ( _dword i = 0; i < networkmanager->mDeferConnectArray.Number( ); i ++ )
		{
			SocketOperation* tcpconnect = networkmanager->mDeferConnectArray[i];

			// Get the socket.
			Socket* socket = networkmanager->SearchSocket( tcpconnect->mSocketID );
			if ( socket == _null )
				continue;

			// Wait for connect finish.
			if ( ::WaitForSingleObject( tcpconnect->mEvent, 0 ) != 0 )
				continue;

			// Close socket event.
			::WSACloseEvent( tcpconnect->mEvent );

			// Raw socket.
			if ( socket->mSocketWrapper != _null )
			{
				networkmanager->CallTCPSocketConnect( (ITCPSocket*) socket->mSocketWrapper, 0 );
			}
			else
			{
				// TCP connect successful.
				networkmanager->OnTCPConnect( tcpconnect->mSocketID, tcpconnect->mSessionID, tcpconnect->mConnectionID );
			}

			// Remove the processed operation from defer array.
			networkmanager->mDeferConnectArray.Remove( i );

			// Free the memory used by operation.
			networkmanager->mBufferManager.ReleaseOperation( tcpconnect );
		}

		networkmanager->mNetworkLock.Leave( );
	}

	SocketOperation* operation = _null;

	_dword bytestransferred = 0, completionkey = 0, tickcount = System::GetCurrentTickcount( );

	// There are some operations finished.
	_bool result = _false;
	while ( ( result = networkmanager->mSocketIOCP.GetQueuedStatus( bytestransferred, completionkey, (Overlapped*&) operation, 0 ) ) || ::GetLastError( ) != WAIT_TIMEOUT )
	{
		if ( operation == _null )
			continue;

		networkmanager->mNetworkLock.Enter( );

		_dword operationtype = operation->mType;

		// Get socket by id.
		Socket* socket = networkmanager->SearchSocket( operation->mSocketID );

		// The socket maybe already closed before overlapped operation finished.
		if ( socket != _null )
		{
			switch ( operationtype )
			{
				case SocketOperation::_TCP_SEND:
				case SocketOperation::_UDP_SEND:
				{
					socket->DoneSendOperation( );

					if ( socket->IsSendBusy( ) == _false )
						socket->FlushQueue( );

					break;
				}

				case SocketOperation::_TCP_RECV:
				case SocketOperation::_UDP_RECV:
				{
					socket->DoneRecvOperation( );

					break;
				}
			}

			// When TCP receive operation got 0 bytes, means bad operation.
			if ( operationtype == SocketOperation::_TCP_RECV && bytestransferred == 0 )
				result = _false;

			// The operation is successful.
			if ( result == _true )
			{ 
				// Process the operation.
				if ( networkmanager->mNetworkLatency == 0 || operationtype != SocketOperation::_UDP_RECV )
				{
					networkmanager->ProcessOperation( socket, operation, completionkey, bytestransferred );
				}
				// Delay udp receive operation.
				else
				{
					networkmanager->ReserveOperation( socket, operation, completionkey, bytestransferred );
				}
			}
			// The operation is failed.
			else
			{
				if ( operationtype != SocketOperation::_UDP_SEND && operationtype != SocketOperation::_UDP_RECV )
				{
					// Rawsocket.
					if ( socket->mSocketWrapper != _null )
					{
						if ( operation->mType == SocketOperation::_TCP_CONNECT )
						{
							networkmanager->CallTCPSocketConnect( (ITCPSocket*) socket->mSocketWrapper, ::WSAGetLastError( ) );
						}
						else if ( operation->mType == SocketOperation::_TCP_RECV )
						{
							networkmanager->CallTCPSocketDisconnect( (ITCPSocket*) socket->mSocketWrapper, ::WSAGetLastError( ) );
						}
					}

					networkmanager->OnError( operation->mSocketID, operation->mSessionID, operation->mConnectionID, operation->mType, completionkey );
				}
			}
		}

		// Free the memory used by operation.
		networkmanager->mBufferManager.ReleaseOperation( operation );

		// Continue post receive operation.
		if ( socket != _null && socket->mSocketWrapper == _null && operationtype == SocketOperation::_UDP_RECV )
		{
			// Continue post receive operation.
			( (UDPSocket*) socket )->Recv( networkmanager->mBufferManager.CreateOperation( ) );
		}

		networkmanager->mNetworkLock.Leave( );

		// Process function will take at most some time, then we call timer or break.
		if ( System::GetCurrentTickcount( ) - tickcount > 50 )
			break;

		tickcount = System::GetCurrentTickcount( );

		networkmanager->mNetworkLock.Enter( );

		// Call user defined timer callback.
		networkmanager->CallNetworkTimerCallback( tickcount );

		networkmanager->mNetworkLock.Leave( );
	}

	return _true;
}

_bool NetworkManager::TimerThread( _void* parameter1, _void* parameter2 )
{
	NetworkManager* networkmanager = (NetworkManager*) parameter1;
	FL_ASSERT( networkmanager != _null )

	networkmanager->mNetworkLock.Enter( );

	// Call user defined timer callback.
	networkmanager->CallNetworkTimerCallback( System::GetCurrentTickcount( ) );

	networkmanager->mNetworkLock.Leave( );

	return _true;
}

_void NetworkManager::ProcessOperation( Socket* socket, SocketOperation* operation, _dword completionkey, _dword bytestransferred )
{
	NetworkSession* session = _null;

	// Get session from operation.
	if ( mSessionPool.Index( operation->mSessionID ) != _null )
		session = (NetworkSession*) mSessionPool[ operation->mSessionID ];

	switch ( operation->mType )
	{
		// Accept operation for TCP socket.
		case SocketOperation::_TCP_ACCEPT:
		{
			// Post another socekt for accept.
			TCPSocket* acceptsocket = (TCPSocket*) CreateSocket( Address::_TYPE_TCP );

			if ( acceptsocket != _null )
			{
				// Create overlapped operation.
				SocketOperation* newoperation = mBufferManager.CreateOperation( );

				// Set session id.
				if ( newoperation != _null )
					newoperation->mSessionID = operation->mSessionID;

				// Post accept operation.
				acceptsocket->Accept( completionkey, newoperation );
			}

			// Raw socket.
			if ( socket->mSocketWrapper != _null )
			{
				Address localaddress, remoteaddress;
				GetConnectionAddress( operation->mBuffer, localaddress, remoteaddress );

				TCPSocketWrapper* tcpsocket = new TCPSocketWrapper( (TCPSocket*) socket, mNetworkLock, mBufferManager );

				tcpsocket->mTCPSocket->SetRemoteAddress( remoteaddress );
				tcpsocket->Recv( );

				CallTCPSocketAccept( tcpsocket );
			}
			// We need some information for this connection, but not enough yet, so receive more.
			else if ( bytestransferred < sizeof( Packet ) )
			{
				mBufferManager.ReserveBuffer( operation->mBuffer );

				// Set address buffer for this new socket.
				socket->mAddressBuffer = operation->mBuffer;
				// Set accept socket for this new socket.
				socket->mAcceptSocket = completionkey;

				if ( bytestransferred > 0 )
				{
					// Create buffer to handle accept operation.
					if ( socket->mAcceptBuffer == _null )
						socket->mAcceptBuffer = mBufferManager.CreateBuffer( );

					// Save buffer for later use.
					Memory::MemCpy( socket->mAcceptBuffer, operation->mBuffer, bytestransferred );
					socket->mAcceptLength += bytestransferred;
				}

				// Create overlapped operation.
				SocketOperation* newoperation = mBufferManager.CreateOperation( );

				// Set session id as unknown.
				if ( newoperation != _null )
					newoperation->mSessionID = -1;

				// Continue receive more data to finish accept operation.
				( (TCPSocket*) socket )->Recv( newoperation );
			}
			// We can process accept operation now.
			else
			{
				OnTCPAccept( operation->mSocketID, operation->mSessionID, completionkey, operation->mBuffer, bytestransferred, operation->mBuffer );
			}

			break;
		}

		// Connect operation for TCP socket.
		case SocketOperation::_TCP_CONNECT:
		{
			// Raw socket.
			if ( socket->mSocketWrapper != _null )
			{
				TCPSocketWrapper* tcpsocket = (TCPSocketWrapper*) socket->mSocketWrapper;
				tcpsocket->Recv( );

				CallTCPSocketConnect( tcpsocket, 0 );
			}
			else
			{
				OnTCPConnect( operation->mSocketID, operation->mSessionID, operation->mConnectionID );
			}

			break;
		}

		// Send operation for TCP socket.
		case SocketOperation::_TCP_SEND:
		{
			mNetworkStatistic.mTotalTCPSendTime		+= 1;
			mNetworkStatistic.mTotalTCPSendBytes	+= bytestransferred;

			if ( session != _null )
			{
				NetworkStatistics* netstat = (NetworkStatistics*) session->GetStatistics( );
				FL_ASSERT( netstat != _null )

				netstat->StatisticSend( bytestransferred );
			}

			if ( socket->mSocketWrapper == _null )
				OnTCPSend( operation->mSocketID, operation->mSessionID, operation->mConnectionID, operation->mBuffer, bytestransferred );

			break;
		}

		// Receive operation for TCP socket.
		case SocketOperation::_TCP_RECV:
		{
			mNetworkStatistic.mTotalTCPReceiveTime	+= 1;
			mNetworkStatistic.mTotalTCPReceiveBytes	+= bytestransferred;

			// Raw socket.
			if ( socket->mSocketWrapper != _null )
			{
				TCPSocketWrapper* tcpsocket = (TCPSocketWrapper*) socket->mSocketWrapper;
				tcpsocket->Recv( );

				if ( bytestransferred > sizeof( Packet ) && tcpsocket->IsPacketHeaderEnabled( ) )
				{
					CallTCPSocketReceive( tcpsocket, operation->mBuffer + sizeof( Packet ), bytestransferred - sizeof( Packet ), operation->mBuffer );
				}
				else
				{
					
					CallTCPSocketReceive( tcpsocket, operation->mBuffer, bytestransferred, operation->mBuffer );
				}
			}
			// This is part of unfinished accept operation.
			else if ( operation->mSessionID == -1 )
			{
				// Create buffer to handle accept operation.
				if ( socket->mAcceptBuffer == _null )
					socket->mAcceptBuffer = mBufferManager.CreateBuffer( );

				if ( bytestransferred >= mBufferManager.GetMaxPacketSize( ) - socket->mAcceptLength )
					bytestransferred = mBufferManager.GetMaxPacketSize( ) - socket->mAcceptLength;

				// Build these buffer together.
				Memory::MemCpy( socket->mAcceptBuffer + socket->mAcceptLength, operation->mBuffer, bytestransferred );
				socket->mAcceptLength += bytestransferred;

				// We can process this accept operation now.
				if ( socket->mAcceptLength >= sizeof( Packet ) )
				{
					_dword socketid = (_dword) *socket;

					// In subsequent process, this socket maybe released, so we add reference now.
					// Current socket refcont is 0, so we need increase twice.
					socket->IncreaseRefCount( );
					socket->IncreaseRefCount( );

					OnTCPAccept( operation->mSocketID, operation->mSessionID, socket->mAcceptSocket, socket->mAcceptBuffer, socket->mAcceptLength, socket->mAddressBuffer );

					// Clear the accept buffer and address buffer.
					mBufferManager.ReleaseBuffer( socket->mAcceptBuffer );
					mBufferManager.ReleaseBuffer( socket->mAddressBuffer );

					socket->mAcceptBuffer = _null;
					socket->mAcceptLength = 0;
					socket->mAcceptSocket = 0;

					// oK, we dont need it now.
					ReleaseSocket( socketid );
					ReleaseSocket( socketid );
				}
				// Shit, still not enough, receive more.
				else
				{
					// Create overlapped operation.
					SocketOperation* newoperation = mBufferManager.CreateOperation( );

					// Set session id as unknown.
					if ( newoperation != _null )
						newoperation->mSessionID = -1;

					( (TCPSocket*) socket)->Recv( newoperation );
				}
			}
			else
			{
				OnTCPRecv( operation->mSocketID, operation->mSessionID, operation->mConnectionID, operation->mBuffer, bytestransferred );
			}

			break;
		}

		// Send operation for UDP socket.
		case SocketOperation::_UDP_SEND:
		{
			mNetworkStatistic.mTotalUDPSendTime		+= 1;
			mNetworkStatistic.mTotalUDPSendBytes	+= bytestransferred;

			if ( session != _null )
			{
				NetworkStatistics* netstat = (NetworkStatistics*) session->GetStatistics( );
				FL_ASSERT( netstat != _null )

				netstat->StatisticSend( bytestransferred );
			}

			if ( socket->mSocketWrapper == _null )
				OnUDPSend( operation->mSocketID, operation->mSessionID, operation->mConnectionID, operation->mBuffer, bytestransferred );

			break;
		}

		// Receive operation for UDP socket.
		case SocketOperation::_UDP_RECV:
		{
			mNetworkStatistic.mTotalUDPReceiveTime	+= 1;
			mNetworkStatistic.mTotalUDPReceiveBytes	+= bytestransferred;

			// Get address from socket address.
			Address address( Dword( operation->mAddress.mIP ).InverseByteOrder( ),
				Address::_TYPE_UDP, Word( operation->mAddress.mPort ).InverseByteOrder( ) );

			// Raw socket.
			if ( socket->mSocketWrapper != _null )
			{
				UDPSocketWrapper* udpsocket = (UDPSocketWrapper*) socket->mSocketWrapper;
				udpsocket->Recv( );

				if ( mUDPSocketReceiveFunc == _null )
					return;

				if ( bytestransferred > sizeof( Packet ) && udpsocket->IsPacketHeaderEnabled( ) )
				{
					Packet* packet = (Packet*) operation->mBuffer;

					if ( bytestransferred >= packet->mPacketSize )
					{
						if ( ProcessNATRequest( packet, operation->mSocketID, address ) == _false )
						{
							if ( packet->mMagicCode == RawPacket::_MAGIC_CODE )
								CallUDPSocketReceive( udpsocket, address, operation->mBuffer + sizeof( Packet ), bytestransferred - sizeof( Packet ), operation->mBuffer );
							else
								OnUDPRecv( operation->mSocketID, address, operation->mBuffer, bytestransferred );
						}
					}
				}
				else
				{
					CallUDPSocketReceive( udpsocket, address, operation->mBuffer, bytestransferred, operation->mBuffer );
				}
			}
			else if ( bytestransferred > sizeof( Packet ) )
			{
				Packet* packet = (Packet*) operation->mBuffer;

				if ( bytestransferred >= packet->mPacketSize )
				{
					if ( ProcessNATRequest( packet, operation->mSocketID, address ) == _false )
						OnUDPRecv( operation->mSocketID, address, operation->mBuffer, bytestransferred );
				}
			}

			break;
		}
	}
}

_void NetworkManager::ReserveOperation( Socket* socket, SocketOperation* operation, _dword completionkey, _dword bytestransferred )
{
	DelayOperation delayoperation;

	delayoperation.mDelayTime			= Math::Random( mNetworkLatency / 2 ) + mNetworkLatency / 2;
	delayoperation.mSocketID			= (_dword) (*socket);
	delayoperation.mCompletionKey		= completionkey;
	delayoperation.mBytesTransferred	= bytestransferred;
	delayoperation.mOperation			= *operation;

	mDelayOperationArray.Append( delayoperation );

	// Reserve the buffer for later process.
	mBufferManager.ReserveBuffer( operation->mBuffer );
}

_bool NetworkManager::OnTCPAccept( _dword socketid, _dword sessionid, _dword listensocketid, _byte* acceptbuffer, _dword length, _byte* addressbuffer )
{
	FL_ASSERT( length >= sizeof( Packet ) )

	// We have some packet coming with the TCP accept operation.
	Packet* packet = (Packet*) acceptbuffer;

	// Set the target session id.
	sessionid = packet->mSessionID;

	////////////////////////////////////////////////////////////////////////////////
	// One tcp socket used by only one session, and one connection.
	////////////////////////////////////////////////////////////////////////////////

	// Get session by id.
	NetworkSession* session = (NetworkSession*) SearchSession( sessionid );
	if ( session == _null )
		return _false;

	_dword groupid = 0;

	if ( IsSocketHosting( listensocketid, sessionid, groupid ) == _false )
	{
		ReleaseSocket( socketid );
		return _true;
	}

	Address localaddress, remoteaddress;
	GetConnectionAddress( addressbuffer, localaddress, remoteaddress );

	// Create a new connection.
	TCPConnection* connection = (TCPConnection*) session->CreateConnection( socketid, localaddress, remoteaddress );

	if ( connection != _null )
	{
		Socket* socket = SearchSocket( socketid );
		FL_ASSERT( socket != _null )

		socket->SetRemoteAddress( remoteaddress );

		// Set connection phase -> _PHASE_WAITJOINREQ.
		connection->mPhase = INetworkSession::_PHASE_WAITJOINREQ;

		// Set connection group id.
		connection->mGroupID = groupid;

		// Post receive operation.
		connection->Recv( );

		// Process the packet too.
		OnTCPRecv( socketid, sessionid, connection->mLocalConnectionID, acceptbuffer, length );
	}
	// Release the socket if create connection failed ( maybe reach maximal connection number ).
	else
	{
		ReleaseSocket( socketid );
	}

	return _true;
}

_bool NetworkManager::OnTCPConnect( _dword socketid, _dword sessionid, _dword connectionid )
{
	////////////////////////////////////////////////////////////////////////////////
	// One tcp socket used by only one session, and one connection.
	////////////////////////////////////////////////////////////////////////////////

	// Get session by id.
	NetworkSession* session = (NetworkSession*) SearchSession( sessionid );
	if ( session == _null )
		return _false;

	// Get connection by id.
	TCPConnection* connection = (TCPConnection*) session->SearchConnection( connectionid );
	if ( connection == _null )
		return _false;

	// Ensure the connection is in right phase.
	FL_ASSERT( connection->mPhase == INetworkSession::_PHASE_CONNECTING )

	// Set connection phase -> _PHASE_SENTJOINREQ.
	connection->mPhase = INetworkSession::_PHASE_SENTJOINREQ;

	// Build join req packet.
	JoinReqPacket joinreq( (_word) connection->mLocalSessionID, (_word) connection->mLocalConnectionID );

	// Ask for compress mode.
	if ( connection->mProperty & Connection::_PROPERTY_ASKCOMPRESS )
		joinreq.mPacketProperty |= Connection::_PROPERTY_ASKCOMPRESS;

	// Ping mode.
	if ( connection->mProperty & Connection::_PROPERTY_PINGONLY )
		joinreq.mPacketProperty |= Connection::_PROPERTY_PINGONLY;

	// Send join req packet.
	connection->SendPacket( &joinreq, joinreq.mPacketSize );

	// Post receive operations.
	connection->Recv( );

	return _true;
}

_bool NetworkManager::OnTCPSend( _dword socketid, _dword sessionid, _dword connectionid, _byte* buffer, _dword length )
{
	////////////////////////////////////////////////////////////////////////////////
	// One tcp socket used by only one session, and one connection.
	////////////////////////////////////////////////////////////////////////////////

	// Get session by id.
	NetworkSession* session = (NetworkSession*) SearchSession( sessionid );
	if ( session == _null )
		return _false;

	// Get connection by id.
	TCPConnection* connection = (TCPConnection*) session->SearchConnection( connectionid );
	if ( connection == _null )
		return _false;

	if ( length == 0 )
	{
		if ( connection->mCloseReason == INetworkSession::_REASON_UNKNOWN )
			connection->mCloseReason = INetworkSession::_REASON_SENDERROR;

		// Close the connection.
		session->CloseConnection( connectionid );
	}
	else
	{
		// Update the bytes sent from this connection.
		connection->mSentBytes += length;
		connection->mRecentSentBytes[0] += length;
	}

	return _true;
}

_bool NetworkManager::OnTCPRecv( _dword socketid, _dword sessionid, _dword connectionid, _byte* buffer, _dword length )
{
	////////////////////////////////////////////////////////////////////////////////
	// One tcp socket used by only one session, and one connection.
	////////////////////////////////////////////////////////////////////////////////

	// Get session by id.
	NetworkSession* session = (NetworkSession*) SearchSession( sessionid );
	if ( session == _null )
		return _false;

	NetworkStatistics* netstat = (NetworkStatistics*) session->GetStatistics( );
	FL_ASSERT( netstat != _null )

	netstat->StatisticReceive( length );

	// Get connection by id.
	TCPConnection* connection = (TCPConnection*) session->SearchConnection( connectionid );
	if ( connection == _null )
		return _false;

	if ( length == 0 )
	{
		if ( connection->mCloseReason == INetworkSession::_REASON_UNKNOWN )
			connection->mCloseReason = INetworkSession::_REASON_RECVERROR;

		// Close the connection.
		session->CloseConnection( connectionid );
	}
	else
	{
		// Update the bytes received from this connection.
		connection->mReceivedBytes += length;
		connection->mRecentReceivedBytes[0] += length;

		// Process data received.
		if ( connection->ProcessData( buffer, length ) == _true )
		{
			// Update wait alive tickcount.
			if ( connection->mPhase == INetworkSession::_PHASE_CONNECTED )
				connection->mWaitAliveTickcount = System::GetCurrentTickcount( );

			// Continue post receive operation.
			connection->Recv( );
		}
		else
		{
			if ( connection->mCloseReason == INetworkSession::_REASON_UNKNOWN )
				connection->mCloseReason = INetworkSession::_REASON_INVALIDPACKET;

			// Close this connection.
			session->CloseConnection( connectionid );
		}
	}

	return _true;
}

_bool NetworkManager::OnUDPSend( _dword socketid, _dword sessionid, _dword connectionid, _byte* buffer, _dword length )
{
	////////////////////////////////////////////////////////////////////////////////
	// One udp socket used by more than one session, and more than one connection.
	////////////////////////////////////////////////////////////////////////////////

	// Get session by id.
	NetworkSession* session = (NetworkSession*) SearchSession( sessionid );
	if ( session == _null )
		return _false;

	// Get connection by id.
	UDPConnection* connection = (UDPConnection*) session->SearchConnection( connectionid );
	if ( connection == _null )
		return _false;

	if ( length == 0 )
	{
		if ( connection->mCloseReason == INetworkSession::_REASON_UNKNOWN )
			connection->mCloseReason = INetworkSession::_REASON_SENDERROR;

		// Close the connection.
		session->CloseConnection( connectionid );
	}
	else
	{
		// Update the bytes sent from this connection.
		connection->mSentBytes += length;
		connection->mRecentSentBytes[0] += length;
	}

	return _true;
}

_bool NetworkManager::OnUDPRecv( _dword socketid, const Address& sourceaddress, _byte* buffer, _dword length )
{
	////////////////////////////////////////////////////////////////////////////////
	// One udp socket used by more than one session, and more than one connection.
	////////////////////////////////////////////////////////////////////////////////

	Packet* packet = (Packet*) buffer;

	// Check packet version.
	if ( packet->mVersion != Packet::_PACKET_VERSION )
		return _false;

	// Get socket by id.
	Socket* socket = mSocketPool[ socketid ];
	if ( socket == _null )
		return _false;

	// Get session id from packet.
	_dword sessionid = packet->mSessionID;

	// Get session by id.
	NetworkSession* session = (NetworkSession*) SearchSession( sessionid );
	if ( session == _null )
		return _false;

	NetworkStatistics* netstat = (NetworkStatistics*) session->GetStatistics( );
	FL_ASSERT( netstat != _null )

	netstat->StatisticReceive( length );

	UDPConnection* connection = _null;

	if ( packet->mMagicCode != JoinReqPacket::_MAGIC_CODE )
	{
		// Get connection by connection id.
		if ( packet->mConnectionID != 0 )
		{
			connection = (UDPConnection*) session->SearchConnection( packet->mConnectionID );
		}
		// Get connection by address.
		else
		{
			connection = (UDPConnection*) session->SearchConnection( socketid, sourceaddress );
		}
	}

	// Process join hint from some remote pc, and its address maybe not same as the connection connecting.
	if ( packet->mMagicCode == JoinHintPacket::_MAGIC_CODE )
	{
		JoinHintPacket* joinhint = (JoinHintPacket*) packet;

		// Build the original address from the packet.
		Address originaladdress = joinhint->mTargetAddr;

		PS_TRACE10( L"[NK] Some one you want to join give you a new address %d.%d.%d.%d:%d => %d.%d.%d.%d:%d\r\n",
			sourceaddress.mIP4, sourceaddress.mIP3, sourceaddress.mIP2, sourceaddress.mIP1, sourceaddress.mPort,
			originaladdress.mIP4, originaladdress.mIP3, originaladdress.mIP2, originaladdress.mIP1, originaladdress.mPort )

		// Get connection by original address.
		connection = (UDPConnection*) session->SearchConnection( socketid, originaladdress );

		if ( connection != _null )
		{
			if ( connection->mInternalAddress == sourceaddress )
				return _true;

			if ( connection->mPhase != INetworkSession::_PHASE_SENTJOINREQ )
				return _true;

			// Adjust the internal address of the connection.
			connection->mInternalAddress = sourceaddress;
		}
	}
	// Create a new connection when a join req received.
	else if ( packet->mMagicCode == JoinReqPacket::_MAGIC_CODE )
	{
		JoinReqPacket* joinreq = (JoinReqPacket*) packet;

		_dword groupid = 0; _bool pingok = _false;

		if ( IsSocketHosting( socketid, sessionid, groupid ) )
		{
			// This is for ping only.
			if ( joinreq->mPacketProperty & Connection::_PROPERTY_PINGONLY )
			{
				pingok = session->IsSessionFull( ) == _false;
			}
			else
			{
				// Create a new connection.
				connection = (UDPConnection*) session->CreateConnection( socketid, socket->GetLocalAddress( ), sourceaddress );
			}
		}
		else
		{
			PS_TRACE2( L"[NK] Not hosting SID: %d, GID %d\r\n", sessionid, groupid )
			PS_ERROR2( L"[NK] Not hosting SID: %d, GID %d\r\n", sessionid, groupid )

			FL_ASSERT( connection == _null )
		}

		// Create new connection OK.
		if ( connection != _null )
		{
			// Set connection phase -> _PHASE_WAITJOINREQ.
			connection->mPhase		= INetworkSession::_PHASE_WAITJOINREQ;

			// Set connection secret code.
			connection->mSecretCode = packet->mSecretCode;

			// Set connection group id.
			connection->mGroupID	= groupid;
		}
		// Cant create the connection, so send a failed ack to source.
		else
		{
			if ( pingok == _false )
			{
				PS_TRACE5( L"[NK] Create connection failed: %d.%d.%d.%d:%d\r\n",
					sourceaddress.mIP4, sourceaddress.mIP3, sourceaddress.mIP2, sourceaddress.mIP1, sourceaddress.mPort )
			}

			// Build join ack packet.
			JoinAckPacket joinack( 0, 0, joinreq->mMyTickcount );

			// For ping packet, we can set session ID, so client will know ping succeeded.
			if ( pingok )
				joinack.mMySessionID = (_word) session->mSessionID;

			// Set secret code, session id and target connection id.
			joinack.mSecretCode		= (_word) joinreq->mSecretCode;
			joinack.mSessionID		= (_word) joinreq->mMySessionID;
			joinack.mConnectionID	= (_word) joinreq->mMyConnectionID;

			PacketBuffer sendbuffer;
			sendbuffer.mBuffer[0] = &joinack;
			sendbuffer.mLength[0] = joinack.mPacketSize;

			// Send packet to source.
			( (UDPSocket*) socket )->Send( sourceaddress, mBufferManager.CreateOperation( sendbuffer ) );
		}
	}

	if ( connection == _null )
		return _false;

	// Check secrete code in the packet.
	if ( packet->mSecretCode != 0 && packet->mSecretCode != (_word) connection->mSecretCode )
		return _false;

	// Check connection id in the packet.
	if ( packet->mConnectionID != 0 && packet->mConnectionID != (_word) connection->mLocalConnectionID )
		return _false;

	// Update the bytes received from this connection.
	connection->mReceivedBytes += length;
	connection->mRecentReceivedBytes[0] += length;

	// Process data received.
	if ( connection->ProcessData( buffer, length ) == _true )
	{
		// Update wait alive tickcount.
		if ( connection->mPhase == INetworkSession::_PHASE_CONNECTED )
			connection->mWaitAliveTickcount = System::GetCurrentTickcount( );
	}
	// Dont close connection in connecting phase, user will know it when timeout.
	else if ( connection->mPhase != INetworkSession::_PHASE_SENTJOINREQ )
	{
		if ( connection->mCloseReason == INetworkSession::_REASON_UNKNOWN )
			connection->mCloseReason = INetworkSession::_REASON_INVALIDPACKET;

		session->CloseConnection( connection->mLocalConnectionID );
	}

	return _true;
}

_bool NetworkManager::OnError( _dword socketid, _dword sessionid, _dword connectionid, _dword operationtype, _dword completionkey )
{
	////////////////////////////////////////////////////////////////////////////////
	// Handle both TCP error and UDP error.
	////////////////////////////////////////////////////////////////////////////////

	// Get socket by id.
	Socket* socket = SearchSocket( socketid );
	if ( socket == _null )
		return _false;

	// This socket has 0 reference count, just delete it.
	if ( socket->GetRefCount( ) == 0 )
	{
		// Post another socekt for accept.
		if ( operationtype == SocketOperation::_TCP_ACCEPT )
		{
			TCPSocket* acceptsocket = (TCPSocket*) CreateSocket( Address::_TYPE_TCP );

			if ( acceptsocket != _null )
			{
				// Create overlapped operation.
				SocketOperation* newoperation = mBufferManager.CreateOperation( );

				// Set session id.
				if ( newoperation != _null )
					newoperation->mSessionID = sessionid;

				// Post accept operation.
				acceptsocket->Accept( completionkey, newoperation );
			}
		}

		ReleaseSocket( socketid );

		return _false;
	}

	// Get session by id.
	NetworkSession* session = (NetworkSession*) SearchSession( sessionid );
	if ( session == _null )
		return _false;

	// Get connection by id.
	Connection* connection = session->SearchConnection( connectionid );
	if ( connection == _null )
		return _false;

	if ( connection->mCloseReason == INetworkSession::_REASON_UNKNOWN )
		connection->mCloseReason = INetworkSession::_REASON_RECVERROR;

	// Close the connection, but for connecting phase, just leave it there till timeout.
	if ( connection->mPhase != INetworkSession::_PHASE_CONNECTING )
		session->CloseConnection( connectionid );

	return _true;
}

_dword NetworkManager::GetLocalIP( _dword adapter )
{
	_chara hostname[1024];

	// Get host name.
	if ( ::gethostname( hostname, 1024 ) != 0 )
		return 0;

	addrinfo* addressinfo = _null;
	addrinfo* addresslink = _null;

	_long error = ::getaddrinfo( hostname, _null, _null, &addressinfo );

	// Get host address information link.
	if ( error != 0 )
		return 0;

	addresslink = addressinfo;

	_dword i = 0;

	while ( addresslink != _null )
	{
		// Skip other than ipv4.
		if ( addresslink->ai_family != AF_INET )
		{
			addresslink = addresslink->ai_next;

			continue;
		}

		// Found it.
		if ( i == adapter )
			break;

		addresslink = addresslink->ai_next;

		i ++;
	}

	_dword ip = 0;

	if ( addresslink != _null )
		ip = Dword( ( (sockaddr_in*) addresslink->ai_addr )->sin_addr.s_addr ).InverseByteOrder( );

	// Free the memory used by address link.
	::freeaddrinfo( addressinfo );

	return ip;
}

_void NetworkManager::GetConnectionAddress( _byte* addressbuffer, Address& localaddress, Address& remoteaddress )
{
	SocketAddress* localsockaddr	= _null; _long length1 = sizeof( SocketAddress );
	SocketAddress* remotesockaddr	= _null; _long length2 = sizeof( SocketAddress );

	// Get socket address from accept buffer.
	::GetAcceptExSockaddrs( addressbuffer, 0, 32, 32, (SOCKADDR**) &localsockaddr, &length1, (SOCKADDR**) &remotesockaddr, &length2 );

	// Get local address by socket address.
	localaddress = Address( Dword( localsockaddr->mIP ).InverseByteOrder( ),
		Address::_TYPE_TCP, Word( localsockaddr->mPort ).InverseByteOrder( ) );

	// Get remote address by socket address.
	remoteaddress = Address( Dword( remotesockaddr->mIP ).InverseByteOrder( ),
		Address::_TYPE_TCP, Word( remotesockaddr->mPort ).InverseByteOrder( ) );
}

_dword NetworkManager::CreateSecretCode( )
{
	return ( System::GetCurrentTickcount( ) + Math::Random( ) ) & 0x0000FFFF;
}

_dword NetworkManager::CreateGlobalToken( )
{
	return ++ mGlobalToken;
}

_void NetworkManager::CallTCPSocketAccept( ITCPSocket* socket )
{
	if ( mTCPSocketAcceptFunc == _null )
		return;

	(*mTCPSocketAcceptFunc)( socket );
}

_void NetworkManager::CallTCPSocketConnect( ITCPSocket* socket, _dword errorcode )
{
	if ( mTCPSocketConnectFunc == _null )
		return;

	if ( socket->IsPollingModeEnabled( ) && mEnablePollingMode )
	{
		( (TCPSocketWrapper*) socket )->IncreaseRefCount( );

		PollingOperation operation;
		operation.mOperationType	= PollingOperation::_TCP_CONNECT;
		operation.mSocket			= socket;
		operation.mError			= errorcode;
		mPollingOperationQueue.Append( operation );

		return;
	}

	(*mTCPSocketConnectFunc)( socket, errorcode );
}

_void NetworkManager::CallTCPSocketDisconnect( ITCPSocket* socket, _dword errorcode )
{
	if ( mTCPSocketDisconnectFunc == _null )
		return;

	if ( socket->IsPollingModeEnabled( ) && mEnablePollingMode )
	{
		( (TCPSocketWrapper*) socket )->IncreaseRefCount( );

		PollingOperation operation;
		operation.mOperationType	= PollingOperation::_TCP_DISCONNECT;
		operation.mSocket			= socket;
		operation.mError			= errorcode;
		mPollingOperationQueue.Append( operation );

		return;
	}

	(*mTCPSocketDisconnectFunc)( socket, errorcode );
}

_void NetworkManager::CallTCPSocketReceive( ITCPSocket* socket, const _void* buffer, _dword length, _byte* reservedbuffer )
{
	if ( mTCPSocketReceiveFunc == _null )
		return;

	if ( socket->IsPollingModeEnabled( ) && mEnablePollingMode )
	{
		( (TCPSocketWrapper*) socket )->IncreaseRefCount( );

		PollingOperation operation;
		operation.mOperationType	= PollingOperation::_TCP_RECEIVE;
		operation.mSocket			= socket;
		operation.mBuffer			= (_byte*) buffer;
		operation.mSize				= length;
		operation.mReservedBuffer	= reservedbuffer;
		mPollingOperationQueue.Append( operation );

		if ( reservedbuffer != _null )
			mBufferManager.ReserveBuffer( reservedbuffer );

		return;
	}

	// Set the buffer we current using.
	mLastReservedBuffer = reservedbuffer;

	(*mTCPSocketReceiveFunc)( socket, buffer, length );

	mLastReservedBuffer = _null;
}

_void NetworkManager::CallUDPSocketReceive( IUDPSocket* socket, const Address& address, const _void* buffer, _dword length, _byte* reservedbuffer )
{
	if ( mUDPSocketReceiveFunc == _null )
		return;

	if ( socket->IsPollingModeEnabled( ) && mEnablePollingMode )
	{
		( (UDPSocketWrapper*) socket )->IncreaseRefCount( );

		PollingOperation operation;
		operation.mOperationType	= PollingOperation::_UDP_RECEIVE;
		operation.mSocket			= socket;
		operation.mBuffer			= (_byte*) buffer;
		operation.mSize				= length;
		operation.mAddress			= address;
		operation.mReservedBuffer	= reservedbuffer;
		mPollingOperationQueue.Append( operation );

		if ( reservedbuffer != _null )
			mBufferManager.ReserveBuffer( reservedbuffer );

		return;
	}

	// Set the buffer we current using.
	mLastReservedBuffer = reservedbuffer;

	(*mUDPSocketReceiveFunc)( socket, address, buffer, length );

	mLastReservedBuffer = _null;
}

_void NetworkManager::CallCreateConnectionCallback( _dword sessionid, _dword connectionid )
{
	if ( mSessionPool.Index( sessionid ) == _null )
		return;

	NetworkSession* session = (NetworkSession*) mSessionPool[ sessionid ];
	FL_ASSERT( session != _null )

	if ( session->mPollingMode && mEnablePollingMode )
	{
		PollingOperation operation;
		operation.mOperationType	= PollingOperation::_CREATE_CONNECTION;
		operation.mSessionID		= sessionid;
		operation.mConnectionID		= connectionid;
		mPollingOperationQueue.Append( operation );

		return;
	}

	session->mRefCount ++;

	// Increase alive connection number.
	session->mAliveConnectionNumber ++;

	// Set connection priority same as session priority.
	session->SetConnectionPriority( connectionid, session->mPriority );

	if ( session->mCreateConnectionFunc != _null )
		(*session->mCreateConnectionFunc)( session, connectionid );
	else if ( mCreateConnectionFunc != _null )
		(*mCreateConnectionFunc)( session, connectionid );

	// Close connection if it is for ping only.
	Connection* connection = session->SearchConnection( connectionid );
	if ( connection != _null )
	{
		if ( connection->mProperty & Connection::_PROPERTY_PINGONLY )
			session->CloseConnection( connectionid, _false );
	}

	session->mRefCount --;
}

_void NetworkManager::CallDeleteConnectionCallback( _dword sessionid, _dword connectionid )
{
	if ( mSessionPool.Index( sessionid ) == _null )
		return;

	NetworkSession* session = (NetworkSession*) mSessionPool[ sessionid ];
	FL_ASSERT( session != _null )

	if ( System::GetCurrentThreadID( ) == mNetworkThread.GetThreadID( ) )
	{
		if ( session->mPollingMode && mEnablePollingMode )
		{
			PollingOperation operation;
			operation.mOperationType	= PollingOperation::_DELETE_CONNECTION;
			operation.mSessionID		= sessionid;
			operation.mConnectionID		= connectionid;
			mPollingOperationQueue.Append( operation );

			return;
		}
	}

	session->mRefCount ++;

	// Decrease alive connection number.
	if ( session->mAliveConnectionNumber > 0 )
		session->mAliveConnectionNumber --;

	if ( session->mDeleteConnectionFunc != _null )
		(*session->mDeleteConnectionFunc)( session, connectionid );
	else if ( mDeleteConnectionFunc != _null )
		(*mDeleteConnectionFunc)( session, connectionid );

	session->mRefCount --;
}

_void NetworkManager::CallReceivePacketCallback( _dword sessionid, _dword connectionid, _byte* buffer, _dword size, _byte* reservedbuffer, _bool flush )
{
	if ( mSessionPool.Index( sessionid ) == _null )
		return;

	NetworkSession* session = (NetworkSession*) mSessionPool[ sessionid ];
	FL_ASSERT( session != _null )

	if ( session->mPollingMode && mEnablePollingMode )
	{
		PollingOperation operation;
		operation.mOperationType	= PollingOperation::_RECEIVE_PACKET;
		operation.mSessionID		= sessionid;
		operation.mConnectionID		= connectionid;
		operation.mBuffer			= buffer;
		operation.mSize				= size;
		operation.mFlush			= flush;
		operation.mReservedBuffer	= reservedbuffer;
		mPollingOperationQueue.Append( operation );

		if ( reservedbuffer != _null )
			mBufferManager.ReserveBuffer( reservedbuffer );

		return;
	}

	Connection* connection = session->SearchConnection( connectionid );
	if ( connection == _null )
		return;

	// Set the buffer we current using.
	mLastReservedBuffer = reservedbuffer;

	session->mRefCount ++;

	_dword sendtickcount = 0, delaychannel = 0, delaytickcount = 0, delayreference = 0, packetreference = 0;

	// This session has tickcount, so this packet must have PacketTickcount in front.
	if ( session->mInitSessionTickcount != 0 && size >= sizeof( PacketTickcount ) )
	{
		PacketTickcount* packettickcont = (PacketTickcount*) buffer;

		// Get delay tickcount.
		sendtickcount	= packettickcont->mSendTickcount;
		delaychannel	= packettickcont->mChannel;
		delaytickcount	= packettickcont->mDelayTickcount;
		delayreference	= packettickcont->mDelayReference;
		packetreference	= packettickcont->mPacketReference;

		// There is delay reference, we need adjust delay tickcount.
		if ( delayreference != 0 && delaytickcount != 0 && flush == _false )
		{
			delaytickcount += session->GetReferenceTickcount( delayreference );

			packettickcont->mDelayTickcount	= delaytickcount;
			packettickcont->mDelayReference	= 0;
		}

		// Adjust packet buffer.
		buffer += sizeof( PacketTickcount );
		size   -= sizeof( PacketTickcount );
	}

	// Get the session tickcount again.
	_dword sessiontickcount = session->GetSessionTickcount( );

	// We can process this packet now.
	if ( delaytickcount == 0 || delaytickcount <= sessiontickcount || flush )
	{
		// This packet has channel, so we maybe need to flush other packet inside same channel.
		if ( delaychannel != 0 && flush == _false )
			connection->FlushDelayPacket( delaychannel );

		// Set packet send tickcount to connection.
		if ( delaytickcount == 0 )
			connection->mPacketTickcount = sendtickcount;
		else
			connection->mPacketTickcount = delaytickcount;

		// Set packet reference to connection.
		connection->mRecvPacketReference = packetreference;

		// The packet tick or session tick is not match, we need adjust it, make sure session tickcount always >= packet tickcount.
		if ( connection->mPacketTickcount > sessiontickcount )
			connection->mPacketTickcount = sessiontickcount;

		BufferCheck check;
		mBufferManager.GetBufferToken( reservedbuffer, check );

		if ( session->mReceivePacketFunc != _null )
			(*session->mReceivePacketFunc)( session, connectionid, buffer, size );
		else if ( mReceivePacketFunc != _null )
			(*mReceivePacketFunc)( session, connectionid, buffer, size );

		// Check reserved buffer.
		mBufferManager.CheckBuffer( reservedbuffer, check );
	}
	// We must delay this packet a while.
	else
	{
		connection->CreateDelayPacket( delaychannel, delaytickcount, buffer - sizeof( PacketTickcount ), size + sizeof( PacketTickcount ), reservedbuffer );
	}

	session->mRefCount --;

	mLastReservedBuffer = _null;
}

_void NetworkManager::CallUpdateLatencyCallback( _dword sessionid, _dword connectionid, _dword latency )
{
	if ( mSessionPool.Index( sessionid ) == _null )
		return;

	NetworkSession* session = (NetworkSession*) mSessionPool[ sessionid ];
	FL_ASSERT( session != _null )

	if ( session->mPollingMode && mEnablePollingMode )
	{
		LockOwner lockowner( mNetworkLock );

		PollingOperation operation;
		operation.mOperationType	= PollingOperation::_UPDATE_LATENCY;
		operation.mSessionID		= sessionid;
		operation.mConnectionID		= connectionid;
		operation.mLatency			= latency;
		mPollingOperationQueue.Append( operation );

		return;
	}

	session->mRefCount ++;

	if ( session->mUpdateLatencyFunc != _null )
		(*session->mUpdateLatencyFunc)( session, connectionid, latency );
	else if ( mUpdateLatencyFunc != _null )
		(*mUpdateLatencyFunc)( session, connectionid, latency );

	session->mRefCount --;
}

_void NetworkManager::CallJoinFailedCallback( _dword sessionid, _dword connectionid )
{
	if ( mSessionPool.Index( sessionid ) == _null )
		return;

	NetworkSession* session = (NetworkSession*) mSessionPool[ sessionid ];
	FL_ASSERT( session != _null )

	if ( session->mPollingMode && mEnablePollingMode )
	{
		LockOwner lockowner( mNetworkLock );

		PollingOperation operation;
		operation.mOperationType	= PollingOperation::_JOIN_FAILED;
		operation.mSessionID		= sessionid;
		operation.mConnectionID		= connectionid;
		mPollingOperationQueue.Append( operation );

		return;
	}

	FinishTCPConnect( sessionid, connectionid );

	session->mRefCount ++;

	if ( session->mJoinFailedFunc != _null )
		(*session->mJoinFailedFunc)( session, connectionid );
	else if ( mJoinFailedFunc != _null )
		(*mJoinFailedFunc)( session, connectionid );

	session->mRefCount --;
}

_void NetworkManager::CallUpdateSessionCallback( _dword sessionid )
{
	if ( mSessionPool.Index( sessionid ) == _null )
		return;

	NetworkSession* session = (NetworkSession*) mSessionPool[ sessionid ];
	FL_ASSERT( session != _null )

	if ( session->mPollingMode && mEnablePollingMode )
	{
		LockOwner lockowner( mNetworkLock );

		// If this session is waiting for update, skip it.
		if ( session->mWaitForUpdate == _false )
		{
			PollingOperation operation;
			operation.mOperationType	= PollingOperation::_UPDATE_SESSION;
			operation.mSessionID		= sessionid;
			mPollingOperationQueue.Append( operation );

			// Set wait for update flag.
			session->mWaitForUpdate = _true;
		}

		return;
	}

	if ( session->mReleased )
	{
		delete session;
		session = _null;

		mSessionPool.Remove( sessionid );

		return;
	}

	if ( session->mClosed )
	{
		session->CloseSession( _false );
		session->mClosed = _false;
	}

	session->mRefCount ++;

	if ( session->mUpdateSessionFunc != _null )
		(*session->mUpdateSessionFunc)( session );
	else if ( mUpdateSessionFunc != _null )
		(*mUpdateSessionFunc)( session );

	// Check and delete all dead connection here, because all callback function should already called.
	session->CheckDeadConnection( );

	// Clear wait for update flag.
	session->mWaitForUpdate = _false;

	session->mRefCount --;
}

_void NetworkManager::CallNetworkTimerCallback( _dword tickcount )
{
	_dword elapse = tickcount - mLastTickcount;

	if ( elapse == 0 )
		return;

	mNetworkStatistic.mTotalDuration += elapse;

	// Check all delay operation.
	for ( _dword i = 0; i < mDelayOperationArray.Number( ); i ++ )
	{
		DelayOperation& delayoperation = mDelayOperationArray[i];

		// Not ready to process.
		if ( delayoperation.mDelayTime > elapse )
		{
			delayoperation.mDelayTime -= elapse;

			continue;
		}

		Socket* socket = SearchSocket( delayoperation.mSocketID );

		if ( socket != _null )
		{
			// Ready to process.
			ProcessOperation( socket, &delayoperation.mOperation, delayoperation.mCompletionKey, delayoperation.mBytesTransferred );
		}

		// Release data buffer.
		mBufferManager.ReleaseBuffer( delayoperation.mOperation.mBuffer );

		mDelayOperationArray.Remove( i -- );
	}

	// Check all connections in all sessions for time out operation.
	for ( SessionPool::Iterator it = mSessionPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		INetworkSession* session = it;

		if ( session == _null )
			continue;

		// Update session.
		( (NetworkSession*) session )->UpdateSession( System::GetCurrentTickcount( ) );

		CallUpdateSessionCallback( session->GetSessionID( ) );
	}

	// Keep NAT helper alive.
	if ( mNATHelper.IsHelperAvailable( ) && mNATHelper.NeedKeepAlive( tickcount ) )
	{
		for ( _dword i = 0; i < mSocketSessionArray.Number( ); i ++ )
		{
			const SocketSessionTriple& sockettriple = mSocketSessionArray[i];

			mNATHelper.KeepHelperAlive( sockettriple.mObject1 );
		}
	}

	// Keep agent helper alive.
	if ( mAgentHelper.IsHelperAvailable( ) && mAgentHelper.NeedKeepAlive( tickcount ) )
	{
		for ( _dword i = 0; i < mSocketSessionArray.Number( ); i ++ )
		{
			const SocketSessionTriple& sockettriple = mSocketSessionArray[i];

			mAgentHelper.KeepHelperAlive( sockettriple.mObject1 );
		}
	}

	// Call the user defined callback function.
	if ( mNetworkTimerFunc != _null )
		(*mNetworkTimerFunc)( tickcount );

	mLastTickcount = tickcount;
}

Socket* NetworkManager::CreateSocket( _dword type )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	Socket* socket = _null;

	// Create socket by type.
	switch ( type )
	{
		case Address::_TYPE_TCP: socket = new TCPSocket( mBufferManager ); break;
		case Address::_TYPE_UDP: socket = new UDPSocket( mBufferManager ); break;
	}

	if ( socket == _null )
		return _null;

	// Create socket.
	if ( socket->Create( type ) == _false )
	{
		PS_TRACE2( L"[NK] Create socket failed, socket %d, error %d\r\n", (_dword) (*socket), ::GetLastError( ) )

		delete socket;
		return _null;
	}

	// Attach to IO completion port.
	if ( mSocketIOCP.Attach( (_handle) (_dword) (*socket), (_dword) (*socket) ) == _false )
	{
		PS_TRACE2( L"[NK] Attach socket failed, socket %d, error %d\r\n", (_dword) (*socket), ::GetLastError( ) )

		delete socket;
		return _null;
	}

	// Insert the new socket to socket pool.
	mSocketPool.Insert( socket, (_dword) (*socket) );

	mNetworkStatistic.mTotalSocketUsed ++;
	mNetworkStatistic.mCurrentSocketUsed ++;

	if ( mNetworkStatistic.mCurrentSocketUsed > mNetworkStatistic.mMaximalSocketUsed )
		mNetworkStatistic.mMaximalSocketUsed = mNetworkStatistic.mCurrentSocketUsed;

	return socket;
}

Socket* NetworkManager::CreateSocket( const Address& address )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// Create socket by type.
	Socket* socket = CreateSocket( address.mType );

	if ( socket == _null )
	{
		PS_TRACE6( L"[NK] Create socket failed %s%d.%d.%d.%d:%d\r\n", address.mType == Address::_TYPE_TCP ? L"tcp://" : L"udp://",
			address.mIP4, address.mIP3, address.mIP2, address.mIP1, address.mPort )

		return _null;
	}

	// Bind to address.
	if ( socket->Bind( address ) == _false )
	{
		PS_TRACE6( L"[NK] Bind socket failed %s%d.%d.%d.%d:%d\r\n", address.mType == Address::_TYPE_TCP ? L"tcp://" : L"udp://",
			address.mIP4, address.mIP3, address.mIP2, address.mIP1, address.mPort )

		// Remove the socket from socket pool.
		mSocketPool.Remove( (_dword) (*socket) );

		delete socket;
		return _null;
	}

	Address localaddresss = socket->GetLocalAddress( );

	PS_TRACE6( L"[NK] Create socket succeeded %s%d.%d.%d.%d:%d\r\n", localaddresss.mType == Address::_TYPE_TCP ? L"tcp://" : L"udp://",
		localaddresss.mIP4, localaddresss.mIP3, localaddresss.mIP2, localaddresss.mIP1, localaddresss.mPort )

	return socket;
}

Socket* NetworkManager::SearchSocket( const Address& address, _bool hostsocket ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( address.mType == 0 )
		return _null;

	if ( address.mPort == 0 && address.mType != Address::_TYPE_UDP )
		return _null;

	// Search existing socket by addresss.
	for ( SocketPool::Iterator it = mSocketPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Socket* socket = it;

		if ( socket == _null )
			continue;

		// Skip sockets with 0 reference count, such as TCP accept socket.
		if ( socket->GetRefCount( ) == 0 )
			continue;

		Address localaddress = socket->GetLocalAddress( );

		if ( address.mIP != 0 && localaddress.mIP == 0 )
			localaddress.mIP = GetLocalIP( 0 );

		// Try to reuse udp socket.
		if ( address.mIP != 0 || address.mPort != 0 || address.mType != Address::_TYPE_UDP || localaddress.mType != Address::_TYPE_UDP )
		{
			// Local address not match, skip it.
			if ( localaddress != address )
				continue;

			// This socket is used for host, so it must not has remote address.
			if ( hostsocket )
			{
				Address remoteaddress = socket->GetRemoteAddress( );

				if ( remoteaddress != Address( ) )
					continue;
			}
		}

		PS_TRACE6( L"[NK] Using socket %s%d.%d.%d.%d:%d\r\n", localaddress.mType == Address::_TYPE_TCP ? L"tcp://" : L"udp://",
			localaddress.mIP4, localaddress.mIP3, localaddress.mIP2, localaddress.mIP1, localaddress.mPort )

		// All match, found it.
		return socket;
	}

	return _null;
}

Socket* NetworkManager::SearchSocket( _dword socketid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( socketid == 0 )
		return _null;

	// Get socket from socket pool.
	Socket** socketpointer = mSocketPool.Index( socketid );

	return socketpointer == _null ? _null : *socketpointer;
}

_void NetworkManager::ReleaseSocket( _dword socketid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( socketid == 0 || mSocketPool.Index( socketid ) == _null )
		return;

	// Get socket from socket pool by id.
	Socket* socket = mSocketPool[ socketid ];
	FL_ASSERT( socket != _null )

	// Delete and remove socket from pool when reference count decrease to zero.
	if ( socket->DecreaseRefCount( ) == 0 )
	{
		Address address = socket->GetLocalAddress( );

		StringPtr protocol = L"";

		if ( address.mType == Address::_TYPE_TCP )
			protocol = L"tcp://";
		else if ( address.mType == Address::_TYPE_TCP )
			protocol = L"udp://";

		PS_TRACE6( L"[NK] Release socket %s%d.%d.%d.%d:%d\r\n", protocol,
			address.mIP4, address.mIP3, address.mIP2, address.mIP1, address.mPort )

		mSocketPool.Remove( socketid );

		// Release socket defer queue.
		if ( socket->mDeferQueue.Number( ) > 0 )
		{
			for ( SocketOperationQueue::Iterator it = socket->mDeferQueue.GetHeadIterator( ); it.Valid( ); it ++ )
				mBufferManager.ReleaseOperation( (SocketOperation*) it );
		}

		// Release socket accept buffer.
		if ( socket->mAcceptBuffer != _null )
			mBufferManager.ReleaseBuffer( socket->mAcceptBuffer );

		// Delete the socket.
		delete socket;

		FL_ASSERT( mNetworkStatistic.mCurrentSocketUsed > 0 )
		mNetworkStatistic.mCurrentSocketUsed --;
	}
}

_void NetworkManager::SocketAttachSession( _dword socketid, _dword sessionid, _dword groupid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// Get socket from socket pool by id.
	Socket* socket = mSocketPool[ socketid ];
	FL_ASSERT( socket != _null )

	// Increase the reference count of this socket.
	socket->IncreaseRefCount( );

	// Put the socket id, session id and group id as triple in array.
	mSocketSessionArray.Append( SocketSessionTriple( socketid, sessionid, groupid ) );
}

_void NetworkManager::SocketDetachSession( _dword sessionid, const Address& address )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// Search the existing socket from by address.
	Socket* socket = SearchSocket( address, _true );

	// Not found the socket, just return.
	while ( socket != _null )
	{
		_dword socketid = (_dword) (*socket);

		// Search the triple from array by socket id and session id, the result will be exactly one.
		for ( _dword i = 0; i < mSocketSessionArray.Number( ); i ++ )
		{
			const SocketSessionTriple& sockettriple = mSocketSessionArray[i];

			// Fount it.
			if ( sockettriple.mObject1 == socketid && sockettriple.mObject2 == sessionid )
			{
				// Release socket when it hosting this session.
				ReleaseSocket( socketid );

				// Remove the triple from array.
				mSocketSessionArray.Remove( i );

				break;
			}
		}

		if ( address.mType == Address::_TYPE_UDP )
			break;

		socket = SearchSocket( address, _true );
	}
}

_void NetworkManager::SocketDetachSession( _dword sessionid, _dword groupid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// Search the triple from array by session id and group id, the result will be one or more.
	for ( _dword i = 0; i < mSocketSessionArray.Number( ); )
	{
		const SocketSessionTriple& sockettriple = mSocketSessionArray[i];

		// Fount it.
		if ( sockettriple.mObject2 == sessionid && ( groupid == 0 || sockettriple.mObject3 == groupid ) )
		{
			// Release the socket.
			ReleaseSocket( sockettriple.mObject1 );

			// Remove the triple from array.
			mSocketSessionArray.Remove( i );
		}
		else
		{
			i ++;
		}
	}
}

_bool NetworkManager::IsSocketHosting( _dword socketid, _dword sessionid, _dword& groupid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// Search the socket id and session id in array.
	for ( _dword i = 0; i < mSocketSessionArray.Number( ); i ++ )
	{
		const SocketSessionTriple& sockettriple = mSocketSessionArray[i];

		// Fount it.
		if ( sockettriple.mObject1 == socketid && sockettriple.mObject2 == sessionid )
		{
			groupid = sockettriple.mObject3;

			return _true;
		}
	}

	return _false;
}

_bool NetworkManager::IsGroupHosting( _dword sessionid, _dword groupid, _dword& socketid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// Search the session id and group id pair in array.
	for ( _dword i = 0; i < mSocketSessionArray.Number( ); i ++ )
	{
		const SocketSessionTriple& sockettriple = mSocketSessionArray[i];

		// Fount it, there maybe many socket host in this session for this group, just return the first one.
		if ( sockettriple.mObject2 == sessionid && sockettriple.mObject3 == groupid )
		{
			socketid = sockettriple.mObject1;

			return _true;
		}
	}

	return _false;
}

_void NetworkManager::ConnectUseHelper( _dword socketid, _dword sessionid, const Address& address )
{
	if ( address.mType != Address::_TYPE_UDP )
		return;

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	mNATHelper.ConnectUseHelper( socketid, sessionid, address );
}

_void NetworkManager::DeferTCPConnect( SocketOperation* operation )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	mDeferConnectArray.Append( operation );
}

_void NetworkManager::FinishTCPConnect( _dword sessionid, _dword connectionid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// This function is used for none-IOCP TCP connect.

	for ( _dword i = 0; i < mDeferConnectArray.Number( ); i ++ )
	{
		SocketOperation* tcpconnect = mDeferConnectArray[i];

		if ( tcpconnect->mSessionID != sessionid || tcpconnect->mConnectionID != connectionid )
			continue;

		// Remove the processed operation from defer array.
		mDeferConnectArray.Remove( i );

		// Close socket event
		::WSACloseEvent( tcpconnect->mEvent );

		// Free the memory used by operation.
		mBufferManager.ReleaseOperation( tcpconnect );

		break;
	}
}

_bool NetworkManager::ProcessNATRequest( Packet* packet, _dword socketid, const Address& address )
{
	// Process nat protocol packet.
	if ( packet->mMagicCode == KeepAlivePacket::_MAGIC_CODE ||
		 packet->mMagicCode == ConnectReqPacket::_MAGIC_CODE ||
		 packet->mMagicCode == ConnectHintPacket::_MAGIC_CODE )
	{
		// Only process none route packet.
		mNATHelper.ProcessPacket( socketid, packet, address );

		return _true;
	}
	else
	{
		return _false;
	}
}

_void NetworkManager::Release( )
{
	delete this;
}

_void NetworkManager::Update( _dword elapse )
{
	if ( mPollingOperationQueue.Number( ) == 0 )
		return;

	mNetworkLock.Enter( );

	mEnablePollingMode = _false;

	_qword startcycle	= System::GetCurrentCycleCount( );
	_qword cyclefreq	= System::GetSystemCycleFrequency( );

	_dword number = mPollingOperationQueue.Number( );

	// Polling all delaied operation.
	for ( _dword i = 0; i < number; i ++ )
	{
		PollingOperation operation = mPollingOperationQueue.Front( );
		mPollingOperationQueue.Remove( );

		switch ( operation.mOperationType )
		{
			case PollingOperation::_TCP_ACCEPT:
			{
				ITCPSocket* socket = (ITCPSocket*) operation.mSocket;

				if ( ( (TCPSocketWrapper*) socket )->IsReleased( ) == _false )
					CallTCPSocketAccept( socket );

				if ( ( (TCPSocketWrapper*) socket )->GetRefCount( ) > 1 )
					( (TCPSocketWrapper*) socket )->DecreaseRefCount( );
				else
					ReleaseTCPSocket( socket );

				break;
			}

			case PollingOperation::_TCP_CONNECT:
			{
				ITCPSocket* socket = (ITCPSocket*) operation.mSocket;

				if ( ( (TCPSocketWrapper*) socket )->IsReleased( ) == _false )
					CallTCPSocketConnect( socket, operation.mError );

				if ( ( (TCPSocketWrapper*) socket )->GetRefCount( ) > 1 )
					( (TCPSocketWrapper*) socket )->DecreaseRefCount( );
				else
					ReleaseTCPSocket( socket );

				break;
			}

			case PollingOperation::_TCP_DISCONNECT:
			{
				ITCPSocket* socket = (ITCPSocket*) operation.mSocket;

				if ( ( (TCPSocketWrapper*) socket )->IsReleased( ) == _false )
					CallTCPSocketDisconnect( socket, operation.mError );

				if ( ( (TCPSocketWrapper*) socket )->GetRefCount( ) > 1 )
					( (TCPSocketWrapper*) socket )->DecreaseRefCount( );
				else
					ReleaseTCPSocket( socket );

				break;
			}

			case PollingOperation::_TCP_RECEIVE:
			{
				ITCPSocket* socket = (ITCPSocket*) operation.mSocket;

				if ( ( (TCPSocketWrapper*) socket )->IsReleased( ) == _false )
					CallTCPSocketReceive( socket, operation.mBuffer, operation.mSize, operation.mReservedBuffer );

				if ( ( (TCPSocketWrapper*) socket )->GetRefCount( ) > 1 )
					( (TCPSocketWrapper*) socket )->DecreaseRefCount( );
				else
					ReleaseTCPSocket( socket );

				if ( operation.mReservedBuffer != _null )
					mBufferManager.ReleaseBuffer( operation.mReservedBuffer );

				break;
			}

			case PollingOperation::_UDP_RECEIVE:
			{
				IUDPSocket* socket = (IUDPSocket*) operation.mSocket;

				if ( ( (UDPSocketWrapper*) socket )->IsReleased( ) == _false )
					CallUDPSocketReceive( socket, operation.mAddress, operation.mBuffer, operation.mSize, operation.mReservedBuffer );

				if ( ( (UDPSocketWrapper*) socket )->GetRefCount( ) > 1 )
					( (UDPSocketWrapper*) socket )->DecreaseRefCount( );
				else
					ReleaseUDPSocket( socket );

				if ( operation.mReservedBuffer != _null )
					mBufferManager.ReleaseBuffer( operation.mReservedBuffer );

				break;
			}

			case PollingOperation::_CREATE_CONNECTION:
			{
				CallCreateConnectionCallback( operation.mSessionID, operation.mConnectionID );

				break;
			}

			case PollingOperation::_DELETE_CONNECTION:
			{
				CallDeleteConnectionCallback( operation.mSessionID, operation.mConnectionID );

				break;
			}

			case PollingOperation::_RECEIVE_PACKET:
			{
				CallReceivePacketCallback( operation.mSessionID, operation.mConnectionID, operation.mBuffer, operation.mSize, operation.mReservedBuffer, operation.mFlush );

				if ( operation.mReservedBuffer != _null )
					mBufferManager.ReleaseBuffer( operation.mReservedBuffer );

				break;
			}

			case PollingOperation::_UPDATE_LATENCY:
			{
				CallUpdateLatencyCallback( operation.mSessionID, operation.mConnectionID, operation.mLatency );

				break;
			}

			case PollingOperation::_JOIN_FAILED:
			{
				CallJoinFailedCallback( operation.mSessionID, operation.mConnectionID );

				break;
			}

			case PollingOperation::_UPDATE_SESSION:
			{
				// We will process update callback at end, so append into queue again.
				mPollingOperationQueue.Append( operation );

				break;
			}
		}

		// Get process time, in millisecond.
		_float taketime = (_float) ( System::GetCurrentCycleCount( ) - startcycle ) / (_float) cyclefreq * 1000.0f;

		// Already process enough time.
		if ( taketime > 10.0f )
		{
			mEnablePollingMode = _true;
			mNetworkLock.Leave( );

			return;
		}
	}

	// Polling all update operation.
	while ( mPollingOperationQueue.Number( ) )
	{
		PollingOperation operation = mPollingOperationQueue.Front( );

		if ( operation.mOperationType != PollingOperation::_UPDATE_SESSION )
			break;

		CallUpdateSessionCallback( operation.mSessionID );
		mPollingOperationQueue.Remove( );
	}

	mEnablePollingMode = _true;

	mNetworkLock.Leave( );
}

_void NetworkManager::SetTCPSocketAcceptCallback( OnTCPSocketAccept funcpointer )
{
	mTCPSocketAcceptFunc = funcpointer;
}

_void NetworkManager::SetTCPSocketConnectCallback( OnTCPSocketConnect funcpointer )
{
	mTCPSocketConnectFunc = funcpointer;
}

_void NetworkManager::SetTCPSocketDisconnectCallback( OnTCPSocketDisconnect funcpointer )
{
	mTCPSocketDisconnectFunc = funcpointer;
}

_void NetworkManager::SetTCPSocketReceiveCallback( OnTCPSocketReceive funcpointer )
{
	mTCPSocketReceiveFunc = funcpointer;
}

_void NetworkManager::SetUDPSocketReceiveCallback( OnUDPSocketReceive funcpointer )
{
	mUDPSocketReceiveFunc = funcpointer;
}

_void NetworkManager::SetCreateConnectionCallback( OnCreateConnection funcpointer, _dword sessionid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( sessionid == 0 )
	{
		mCreateConnectionFunc = funcpointer;
	}
	else
	{
		NetworkSession* session = (NetworkSession*) mSessionPool[ sessionid ];
		FL_ASSERT( session != _null )

		session->mCreateConnectionFunc = funcpointer;
	}
}

_void NetworkManager::SetDeleteConnectionCallback( OnDeleteConnection funcpointer, _dword sessionid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( sessionid == 0 )
	{
		mDeleteConnectionFunc = funcpointer;
	}
	else
	{
		NetworkSession* session = (NetworkSession*) mSessionPool[ sessionid ];
		FL_ASSERT( session != _null )

		session->mDeleteConnectionFunc = funcpointer;
	}
}

_void NetworkManager::SetReceivePacketCallback( OnReceivePacket funcpointer, _dword sessionid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( sessionid == 0 )
	{
		mReceivePacketFunc = funcpointer;
	}
	else
	{
		NetworkSession* session = (NetworkSession*) mSessionPool[ sessionid ];
		FL_ASSERT( session != _null )

		session->mReceivePacketFunc = funcpointer;
	}
}

_void NetworkManager::SetUpdateLatencyCallback( OnUpdateLatency funcpointer, _dword sessionid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( sessionid == 0 )
	{
		mUpdateLatencyFunc = funcpointer;
	}
	else
	{
		NetworkSession* session = (NetworkSession*) mSessionPool[ sessionid ];
		FL_ASSERT( session != _null )

		session->mUpdateLatencyFunc = funcpointer;
	}
}

_void NetworkManager::SetJoinFailedCallback( OnJoinFailed funcpointer, _dword sessionid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( sessionid == 0 )
	{
		mJoinFailedFunc = funcpointer;
	}
	else
	{
		NetworkSession* session = (NetworkSession*) mSessionPool[ sessionid ];
		FL_ASSERT( session != _null )

		session->mJoinFailedFunc = funcpointer;
	}
}

_void NetworkManager::SetUpdateSessionCallback( OnUpdateSession funcpointer, _dword sessionid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( sessionid == 0 )
	{
		mUpdateSessionFunc = funcpointer;
	}
	else
	{
		NetworkSession* session = (NetworkSession*) mSessionPool[ sessionid ];
		FL_ASSERT( session != _null )

		session->mUpdateSessionFunc = funcpointer;
	}
}

_void NetworkManager::SetNetworkTimerCallback( OnNetworkTimer funcpointer )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	mNetworkTimerFunc = funcpointer;
}

_void NetworkManager::SetBufferLimit( _dword memorylimit )
{
	mBufferManager.SetMaxBufferLimit( memorylimit );
}

Address NetworkManager::CreateAddress( _word type, _word port, _dword adapter )
{
	FL_ASSERT( type == Address::_TYPE_TCP || type == Address::_TYPE_UDP )

	Address address;
	address.mIP		= GetLocalIP( adapter );
	address.mType	= type;
	address.mPort	= port;

	return address;
}

Address NetworkManager::CreateAddress( _word type, _dword ip, _word port )
{
	FL_ASSERT( type == Address::_TYPE_TCP || type == Address::_TYPE_UDP )

	Address address;
	address.mIP		= ip;
	address.mType	= type;
	address.mPort	= port;

	return address;
}

Address NetworkManager::CreateAddress( _word type, StringPtr ipaddress, _word port )
{
	if ( ipaddress.SearchL2RLowercase( L"tcp://" ) == 0 )
	{
		type = Address::_TYPE_TCP;
		ipaddress = (const _char*) ipaddress + 6;
	}
	else if ( ipaddress.SearchL2RLowercase( L"udp://" ) == 0 )
	{
		type = Address::_TYPE_UDP;
		ipaddress = (const _char*) ipaddress + 6;
	}

	FL_ASSERT( type == Address::_TYPE_TCP || type == Address::_TYPE_UDP )

	Address address;
	address.mType	= type;
	address.mPort	= port;

	_chara ansiaddress[1024];
	StringFormatter::UnicodeToAnsi( ansiaddress, 1024, ipaddress );

	// Converts a string ip to integer ip.
	address.mIP = Dword( ::inet_addr( ansiaddress ) ).InverseByteOrder( );

	// Can not analyze ip string, get from DNS.
	if ( address.mIP == -1 )
	{
		address.mIP = 0;

		if ( ipaddress.CompareLowercase( L"localhost" ) )
		{
			address.mIP = GetLocalIP( 0 );
		}
		else
		{
			addrinfo* addressinfo = _null;
			::getaddrinfo( ansiaddress, _null, _null, &addressinfo );

			if ( addressinfo != _null )
				address.mIP = Dword( ( (SOCKADDR_IN*) addressinfo->ai_addr )->sin_addr.s_addr ).InverseByteOrder( );

			::freeaddrinfo( addressinfo );
		}
	}

	return address;
}

Address NetworkManager::CreateAddress( _word type, StringPtr ipaddress )
{
	if ( ipaddress.SearchL2RLowercase( L"tcp://" ) == 0 )
	{
		type = Address::_TYPE_TCP;
		ipaddress = (const _char*) ipaddress + 6;
	}
	else if ( ipaddress.SearchL2RLowercase( L"udp://" ) == 0 )
	{
		type = Address::_TYPE_UDP;
		ipaddress = (const _char*) ipaddress + 6;
	}

	FL_ASSERT( type == Address::_TYPE_TCP || type == Address::_TYPE_UDP )

	Address address;
	address.mType	= type;
	address.mPort	= 0;

	// Search the colon for port separator.
	_dword index = ipaddress.SearchL2R( ':' );

	// There is colon exist, create address by ip and port.
	if ( index != -1 )
		address = CreateAddress( type, String( ipaddress, index ), Long( (const _char*) ipaddress + index + 1 ) );

	return address;
}

ITCPSocket* NetworkManager::CreateTCPSocket( const Address& bindaddress )
{
	FL_ASSERT( bindaddress.mType == Address::_TYPE_TCP )

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	TCPSocket* socket = (TCPSocket*) CreateSocket( bindaddress );
	if ( socket == _null )
		return _null;

	return new TCPSocketWrapper( socket, mNetworkLock, mBufferManager );
}

ITCPSocket* NetworkManager::SearchTCPSocket( const Address& bindaddress )
{
	FL_ASSERT( bindaddress.mType == Address::_TYPE_TCP )

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	TCPSocket* socket = (TCPSocket*) SearchSocket( bindaddress, _false );
	if ( socket == _null )
		return _null;

	return new TCPSocketWrapper( socket, mNetworkLock, mBufferManager );
}

_void NetworkManager::ReleaseTCPSocket( ITCPSocket*& socket )
{
	if ( socket == _null )
		return;

	if ( ( (TCPSocketWrapper*) socket )->DecreaseRefCount( ) == 0 )
	{
		Socket* rawsocket = ( (TCPSocketWrapper*) socket )->mTCPSocket;
		rawsocket->mSocketWrapper = _null;

		ReleaseSocket( rawsocket->mSocket );

		delete (TCPSocketWrapper*) socket;
	}
	else
	{
		( (TCPSocketWrapper*) socket )->Release( );
	}

	socket = _null;
}

IUDPSocket* NetworkManager::CreateUDPSocket( const Address& bindaddress )
{
	FL_ASSERT( bindaddress.mType == Address::_TYPE_UDP )

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	UDPSocket* socket = (UDPSocket*) CreateSocket( bindaddress );
	if ( socket == _null )
		return _null;

	return new UDPSocketWrapper( socket, mNetworkLock, mBufferManager );
}

IUDPSocket* NetworkManager::SearchUDPSocket( const Address& bindaddress )
{
	FL_ASSERT( bindaddress.mType == Address::_TYPE_UDP )

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	UDPSocket* socket = (UDPSocket*) SearchSocket( bindaddress, _false );
	if ( socket == _null )
		return _null;

	return new UDPSocketWrapper( socket, mNetworkLock, mBufferManager );
}

_void NetworkManager::ReleaseUDPSocket( IUDPSocket*& socket )
{
	if ( socket == _null )
		return;

	if ( ( (UDPSocketWrapper*) socket )->DecreaseRefCount( ) == 0 )
	{
		Socket* rawsocket = ( (UDPSocketWrapper*) socket )->mUDPSocket;
		rawsocket->mSocketWrapper = _null;

		ReleaseSocket( rawsocket->mSocket );

		delete (UDPSocketWrapper*) socket;
	}
	else
	{
		( (UDPSocketWrapper*) socket )->Release( );
	}

	socket = _null;
}

_dword NetworkManager::GetAvailableSessionID( ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	_dword index = mSessionPool.GetAvailableIndex( );

	if ( index == -1 )
		return 0;

	return index;
}

_dword NetworkManager::GetTotalSessionNumber( ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	return mSessionPool.Number( ) - 1;
}

INetworkSession* NetworkManager::CreateSession( _dword sessionid, _dword maxconnection )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// There is session with same id existing, return false.
	if ( mSessionPool.Index( sessionid ) != _null )
		return _null;

	NetworkSession* session = new NetworkSession( sessionid, maxconnection, mNetworkLock, mBufferManager );

	// Insert the new session into session pool.
	mSessionPool.Insert( session, sessionid );

	return session;
}

INetworkSession* NetworkManager::SearchSession( _dword sessionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// Search existing session with session id.
	INetworkSession** sessionpointer = mSessionPool.Index( sessionid );

	return sessionpointer != _null ? *sessionpointer : _null;
}

_void NetworkManager::ReleaseSession( INetworkSession*& networksession )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( networksession == _null )
		return;

	_dword sessionid = ( (NetworkSession*) networksession )->GetSessionID( );

	if ( ( (NetworkSession*) networksession )->mRefCount > 0 )
	{
		( (NetworkSession*) networksession )->mReleased = _true;
	}
	else
	{
		delete (NetworkSession*) networksession;

		// Remove it from session pool.
		mSessionPool.Remove( sessionid );
	}

	networksession = _null;
}

IFileDownloader* NetworkManager::CreateHTTPDownloader( )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	return new HTTPDownloader( );
}

_void NetworkManager::SetNATHelperServer( const Address& address )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( address.mType != Address::_TYPE_UDP )
		return;

	PS_TRACE5( L"[NK] Using NAT Helper %d.%d.%d.%d:%d\r\n", address.mIP4, address.mIP3, address.mIP2, address.mIP1, address.mPort )

	mNATHelper.SetHelperServer( address );
}

_bool NetworkManager::OpenNATHelperPort( const Address& address )
{
	FL_ASSERT( address.mType == Address::_TYPE_UDP )

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	return mNATHelper.OpenHelperPort( address );
}

_void NetworkManager::CloseNATHelperPort( )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	mNATHelper.CloseHelperPort( );
}

_void NetworkManager::SetAgentHelperServer( const Address& address )
{
	FL_ASSERT( address.mType == Address::_TYPE_UDP )

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	PS_TRACE5( L"[NK] Using Agent Helper %d.%d.%d.%d:%d\r\n", address.mIP4, address.mIP3, address.mIP2, address.mIP1, address.mPort )

	mAgentHelper.SetHelperServer( address );
}

_bool NetworkManager::OpenAgentHelperPort( const Address& address )
{
	FL_ASSERT( address.mType == Address::_TYPE_UDP )

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	return mAgentHelper.OpenHelperPort( address );
}

_void NetworkManager::CloseAgentHelperPort( )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	mAgentHelper.CloseHelperPort( );
}

Thread& NetworkManager::GetNetworkThread( )
{
	return mNetworkThread;
}

Lock& NetworkManager::GetNetworkLock( )
{
	return mNetworkLock;
}

_void NetworkManager::SetNetworkLatency( _dword latency )
{
	mNetworkLatency = latency;
}

_dword NetworkManager::GetNetworkLatency( ) const
{
	return mNetworkLatency;
}

const NetworkStatistic& NetworkManager::GetNetworkStatistic( ) const
{
	return mNetworkStatistic;
}

_void NetworkManager::DumpNetworkStatistic( ) const
{
	PS_TRACE( L"[NK] Network Statistic\r\n" )

	PS_TRACE1( L"  Total Duration : %s\r\n", (const _char*) StringFormatter::FormatTimeString( mNetworkStatistic.mTotalDuration ) )

	PS_TRACE1( L"  Maximal TCP Accept Operation : %u\r\n", mNetworkStatistic.mMaximalTCPAcceptOpt )
	PS_TRACE1( L"  Maximal TCP Connect Operation : %u\r\n", mNetworkStatistic.mMaximalTCPConnectOpt )
	PS_TRACE1( L"  Maximal TCP Send Operation : %u\r\n", mNetworkStatistic.mMaximalTCPSendOpt )
	PS_TRACE1( L"  Maximal TCP Receive Operation : %u\r\n", mNetworkStatistic.mMaximalTCPRecvOpt )
	PS_TRACE1( L"  Maximal UDP Send Operation : %u\r\n", mNetworkStatistic.mMaximalUDPSendOpt )
	PS_TRACE1( L"  Maximal UDP Receive Operation : %u\r\n", mNetworkStatistic.mMaximalUDPRecvOpt )

	PS_TRACE1( L"  Total Socket Used : %u\r\n", mNetworkStatistic.mTotalSocketUsed )
	PS_TRACE1( L"  Maximal Socket Used : %u\r\n", mNetworkStatistic.mMaximalSocketUsed )

	PS_TRACE1( L"  Total Connection Used : %u\r\n", mNetworkStatistic.mTotalConnectionUsed )
	PS_TRACE1( L"  Maximal Connection Used : %u\r\n", mNetworkStatistic.mMaximalConnectionUsed )

	PS_TRACE2( L"  Total TCP Send : %s (%u Time)\r\n", (const _char*) StringFormatter::FormatBytesString(
		mNetworkStatistic.mTotalTCPSendBytes ), mNetworkStatistic.mTotalTCPSendTime )

	PS_TRACE2( L"  Total TCP Receive : %s (%u Time)\r\n", (const _char*) StringFormatter::FormatBytesString(
		mNetworkStatistic.mTotalTCPReceiveBytes ), mNetworkStatistic.mTotalTCPReceiveTime )

	PS_TRACE2( L"  Total UDP Send : %s (%u Time)\r\n", (const _char*) StringFormatter::FormatBytesString(
		mNetworkStatistic.mTotalUDPSendBytes ), mNetworkStatistic.mTotalUDPSendTime )

	PS_TRACE2( L"  Total UDP Resend : %s (%u Time)\r\n", (const _char*) StringFormatter::FormatBytesString(
		mNetworkStatistic.mTotalUDPResendBytes ), mNetworkStatistic.mTotalUDPResendTime )

	PS_TRACE2( L"  Total UDP Receive : %s (%u Time)\r\n", (const _char*) StringFormatter::FormatBytesString(
		mNetworkStatistic.mTotalUDPReceiveBytes ), mNetworkStatistic.mTotalUDPReceiveTime )

	if ( mNetworkStatistic.mTotalUDPResendBytes > 0 )
	{
		PS_TRACE2( L"  UDP Resent Rate (Bytes/Time) : %3.1f%% / %3.1f%% \r\n",
			(_float) mNetworkStatistic.mTotalUDPResendBytes / (_float) mNetworkStatistic.mTotalUDPSendBytes * 100.0f,
			(_float) mNetworkStatistic.mTotalUDPResendTime / (_float) mNetworkStatistic.mTotalUDPSendTime * 100.0f )
	}

	PS_TRACE1( L"  Received Packets In Order (UDP) : %u\r\n", mNetworkStatistic.mUDPPacketsInOrder )
	PS_TRACE1( L"  Received Packets In Disorder (UDP) : %u\r\n", mNetworkStatistic.mUDPPacketsInDisorder )
	PS_TRACE1( L"  Received Packets Droped (UDP) : %u\r\n", mNetworkStatistic.mUDPPacketsDroped )
}

_void NetworkManager::ClearNetworkStatistic( )
{
	Memory::MemSet( &mNetworkStatistic, 0, sizeof( mNetworkStatistic ) );

	mNetworkStatistic.mTotalDuration = 1;
}