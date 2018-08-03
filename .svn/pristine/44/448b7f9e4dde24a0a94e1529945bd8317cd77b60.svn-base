//============================================================================
// NetworkSession.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// NetworkSession Implementation
//----------------------------------------------------------------------------

NetworkSession::NetworkSession( _dword sessionid, _dword maxconnection, Lock& lock, BufferManager& buffermanager )
	: mConnectionPool( maxconnection + 1 ), mNetworkLock( lock ), mBufferManager( buffermanager )
{
	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	mPollingMode			= _false;
	mWaitForUpdate			= _false;
	mClosed					= _false;
	mReleased				= _false;
	mRefCount				= 0;

	mFlushInterval			= 100;
	mFlushTickcount			= System::GetCurrentTickcount( );
	mLastUpdateTickcount	= System::GetCurrentTickcount( );
	mInitSessionTickcount	= 0;
	mEnableSessionTickcount	= _false;

	mSessionID				= sessionid;
	mProperty				= 0;
	mPriority				= INetworkManager::_NETWORK_PRIORITY_NORMAL;
	mNewConnectionID		= 0;
	mAliveConnectionNumber	= 0;
	mUserData				= 0;
	mGlobalToken			= networkmanager.CreateGlobalToken( );

	mCreateConnectionFunc	= _null;
	mDeleteConnectionFunc	= _null;
	mReceivePacketFunc		= _null;
	mUpdateLatencyFunc		= _null;
	mJoinFailedFunc			= _null;
	mUpdateSessionFunc		= _null;

	mSessionStatistics		= new NetworkStatistics( 1000, 10 );

	mConnectionPool.Insert( _null, 0 );
}

NetworkSession::~NetworkSession( )
{
	CloseSession( _false );

	delete mSessionStatistics;
}

_dword NetworkSession::GetGroupIndex( _dword groupid ) const
{
	FL_ASSERT( groupid != 0 )

	for ( _dword i = 0; i < mGroupArray.Number( ); i ++ )
	{
		Connection* connection = mGroupArray[i].mObject1;
		FL_ASSERT( connection != _null )

		if ( connection->mGroupID == groupid )
			return i;
	}

	return -1;
}

_bool NetworkSession::JoinSessionHelper( const Address& localaddress, const Address& remoteaddress, _dword groupid, _dword userdata, _dword sessionid, _bool ping )
{
	Socket* socket = CreateJoinSocket( localaddress, remoteaddress );

	// Cant create socket for join session, return false.
	if ( socket == _null )
		return _false;

	// Create a new connection.
	Connection* connection = CreateConnection( (_dword) (*socket), socket->GetLocalAddress( ), remoteaddress );

	// Cant create new connection, return false.
	if ( connection == _null )
		return _false;

	// Set remote session id.
	if ( sessionid != 0 )
		connection->mRemoteSessionID = sessionid;

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	// Set connection secret code.
	connection->mSecretCode = networkmanager.CreateSecretCode( );

	// Set connection group id.
	connection->mGroupID = groupid;

	// Set connection user data.
	connection->mUserData = userdata;

	// Set ping property.
	if ( ping )
		connection->mProperty |= Connection::_PROPERTY_PINGONLY;

	// Connecting.
	connection->Connect( );

	// Make join session always successful, only join failed at time out.
	return _true;
}

_bool NetworkSession::SendPacketHelper( Connection* connection, const _void* buffer, _dword length )
{
	// We need put packet tickcount as part of packet.
	if ( mInitSessionTickcount != 0 )
		length += sizeof( PacketTickcount );

	// Get max size of a packet.
	_dword maxpacketsize = mBufferManager.GetMaxPacketSize( );

	// Prepare send buffer.
	PacketBuffer sendbuffer;

	// Build packet tickcount using session tickcount.
	PacketTickcount packettickcount;

	// The buffer is too big to put into one network packet, we must split it.
	if ( length + sizeof( Packet ) > maxpacketsize )
	{
		connection->FlushDeferPacket( );

		// We need put packet tickcount as part of packet.
		if ( mInitSessionTickcount != 0 )
		{
			packettickcount = PacketTickcount( connection->mDelayChannel, GetSessionTickcount( ),
				connection->mDelayTickcount, connection->mDelayReference, connection->mSendPacketReference );

			sendbuffer.mBuffer[0] = &packettickcount;
			sendbuffer.mLength[0] = sizeof( PacketTickcount );
			sendbuffer.mBuffer[1] = (_void*) buffer;
			sendbuffer.mLength[1] = length - sizeof( PacketTickcount );
		}
		else
		{
			sendbuffer.mBuffer[0] = (_void*) buffer;
			sendbuffer.mLength[0] = length;
		}

		return connection->SendBigPacket( sendbuffer, maxpacketsize );
	}
	else
	{
		// Build packet header.
		Packet header( (_word) ( length + sizeof( Packet ) ) );

		// We need put packet tickcount as part of packet.
		if ( mInitSessionTickcount != 0 )
		{
			packettickcount = PacketTickcount( connection->mDelayChannel, GetSessionTickcount( ),
				connection->mDelayTickcount, connection->mDelayReference, connection->mSendPacketReference );

			sendbuffer.mBuffer[0] = &header;
			sendbuffer.mLength[0] = sizeof( Packet );
			sendbuffer.mBuffer[1] = &packettickcount;
			sendbuffer.mLength[1] = sizeof( PacketTickcount );
			sendbuffer.mBuffer[2] = (_void*) buffer;
			sendbuffer.mLength[2] = length - sizeof( PacketTickcount );
		}
		else
		{
			sendbuffer.mBuffer[0] = &header;
			sendbuffer.mLength[0] = sizeof( Packet );
			sendbuffer.mBuffer[1] = (_void*) buffer;
			sendbuffer.mLength[1] = length;
		}

		// Send data to this connection, includes packet header and data.
		return connection->SendCompressPacket( sendbuffer );
	}
}

_bool NetworkSession::SendDeferPacketHelper( Connection* connection, const _void* buffer, _dword length )
{
	// We need put packet tickcount as part of packet.
	if ( mInitSessionTickcount != 0 )
		length += sizeof( PacketTickcount );

	_dword maxpacketsize = mBufferManager.GetMaxPacketSize( );

	// The buffer is too big to put into one network packet, we must split it, so we send it right now.
	if ( length + sizeof( Packet ) + sizeof( PacketDivision ) > maxpacketsize )
	{
		// We need put packet tickcount as part of packet.
		if ( mInitSessionTickcount != 0 )
			length -= sizeof( PacketTickcount );

		return SendPacketHelper( connection, buffer, length );
	}
	else
	{
		// Prepare send buffer.
		PacketBuffer sendbuffer;

		// Build packet tickcount using session tickcount.
		PacketTickcount packettickcount;

		// We need put packet tickcount as part of packet.
		if ( mInitSessionTickcount != 0 )
		{
			packettickcount = PacketTickcount( connection->mDelayChannel, GetSessionTickcount( ),
				connection->mDelayTickcount, connection->mDelayReference, connection->mSendPacketReference );

			sendbuffer.mBuffer[0] = &packettickcount;
			sendbuffer.mLength[0] = sizeof( PacketTickcount );
			sendbuffer.mBuffer[1] = (_void*) buffer;
			sendbuffer.mLength[1] = length - sizeof( PacketTickcount );
		}
		else
		{
			sendbuffer.mBuffer[0] = (_void*) buffer;
			sendbuffer.mLength[0] = length;
		}

		// Send data to this connection, includes packet header and data.
		return connection->SendDeferPacket( sendbuffer );
	}
}

_bool NetworkSession::SendConnectionPropertyHelper( _dword connectionid, _dword mask, _bool enable )
{
	if ( connectionid != 0 || connectionid != -1 )
	{
		if ( enable )
			mProperty |= mask;
		else
			mProperty &= ~mask;
	}
	else
	{
		// Set all connections.
		for ( ConnectionPool::Iterator it = mConnectionPool.GetHeadIterator( ); it.Valid( ); it ++ )
		{
			Connection* connection = it;

			if ( connection == _null )
				continue;

			connection->FlushDeferPacket( );

			if ( enable )
				connection->mProperty |= mask;
			else
				connection->mProperty &= ~mask;
		}
	}

	return _true;
}

Socket* NetworkSession::CreateJoinSocket( const Address& localaddress, const Address& remoteaddress )
{
	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	// Search the existing socket from network networkmanager by address.
	Socket* socket = networkmanager.SearchSocket( localaddress, _false );

	// Can not use a socket as both local and remote.
	if ( socket != _null && socket->GetLocalAddress( ) == remoteaddress )
		socket = _null;

	// Found the socket.
	if ( socket != _null )
	{
		// Only the udp socket can be shared.
		if ( localaddress.mType != Address::_TYPE_UDP )
		{
			PS_TRACE( L"[NK] Create Join Socket failed, can not reuse TCP socket\r\n" )

			return _null;
		}
	}

	// If there is no existing socket with same address, create a new socket.
	if ( socket == _null )
		socket = networkmanager.CreateSocket( localaddress );

	// Create socket failed, return false.
	if ( socket == _null )
		return _null;

	// It is a new udp port, so post many receive operations.
	if ( localaddress.mType == Address::_TYPE_UDP && socket->mTotalOptNumber == 0 )
	{
		// Post many receive operations.
		for ( _dword i = 0; i < SocketOperation::_MAX_RECVOPT_NUMBER; i ++ )
			( (UDPSocket*) socket )->Recv( mBufferManager.CreateOperation( ) );
	}

	return socket;
}

Connection*	NetworkSession::Reconnect( Connection* connection )
{
	// Cant reconnect
	if ( connection->mBackupAddress == Address( ) )
		return _null;

	connection->mRemoteAddress	 = connection->mBackupAddress;
	connection->mInternalAddress = connection->mBackupAddress;

	// Just retry another address.
	if ( connection->GetConnectionType( ) == connection->mBackupAddress.mType )
	{
		connection->mBackupAddress = Address( );

		// Reconnect.
		if ( connection->Connect( ) == _false )
			return _null;
	}
	// Protocol change.
	else
	{
		connection->mBackupAddress = Address( );

		// Reset local address.
		Address localaddress  = Address( connection->mRemoteAddress.mType );
		Address remoteaddress = connection->mRemoteAddress;

		// Create a new socket for connection.
		Socket* newsocket = CreateJoinSocket( localaddress, remoteaddress );

		if ( newsocket == _null )
			return _null;

		// Increase new socket reference.
		newsocket->IncreaseRefCount( );

		Connection* newconnection = _null;

		// UDP => TCP.
		if ( localaddress.mType == Address::_TYPE_TCP )
		{
			// Recreate a TCP connection.
			newconnection = new TCPConnection( (_dword) (*newsocket), localaddress, remoteaddress, mBufferManager );
		}
		else
		{
			// Recreate a UDP connection.
			newconnection = new UDPConnection( (_dword) (*newsocket), localaddress, remoteaddress, mBufferManager );
		}

		// Copy connection attribute.
		newconnection->mGlobalToken			= connection->mGlobalToken;
		newconnection->mLocalSessionID		= connection->mLocalSessionID;
		newconnection->mRemoteSessionID		= connection->mRemoteSessionID;
		newconnection->mLocalConnectionID	= connection->mLocalConnectionID;
		newconnection->mProperty			= connection->mProperty;
		newconnection->mPriority			= connection->mPriority;
		newconnection->mUserData			= connection->mUserData;
		newconnection->mGroupID				= connection->mGroupID;

		// Reconnect.
		if ( newconnection->Connect( ) == _false )
		{
			delete newconnection;
			return _null;
		}

		// Reset connection to the new connection.
		mConnectionPool[ connection->mLocalConnectionID ] = newconnection;
		connection = newconnection;
	}

	return connection;
}

Connection* NetworkSession::CreateConnection( _dword socketid, const Address& localaddress, const Address& remoteaddress )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// If the connection pool is full, then return false.
	if ( IsSessionFull( ) )
	{
		PS_TRACE1( L"Create connection failed, max connection number %d\r\n", mConnectionPool.Number( ) - 1 )

		return _null;
	}

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	Connection* connection = _null;

	switch ( localaddress.mType )
	{
		case Address::_TYPE_TCP:
		{
			connection = new TCPConnection( socketid, localaddress, remoteaddress, mBufferManager );

			break;
		}

		case Address::_TYPE_UDP:
		{
			connection = new UDPConnection( socketid, localaddress, remoteaddress, mBufferManager );

			break;
		}
	}

	FL_ASSERT( connection != _null )

	connection->mGlobalToken = networkmanager.CreateGlobalToken( );

	// Insert the new connection into pool
	_dword connectionid = mConnectionPool.Create( connection );

	// Failed to put into pool, return false.
	if ( connectionid == -1 )
	{
		PS_TRACE1( L"Create connection failed, current connection number %d\r\n", mConnectionPool.Number( ) )
		delete connection;

		return _null;
	}

	// Set connection attribute.
	connection->mLocalSessionID		 = mSessionID;
	connection->mRemoteSessionID	 = mSessionID;
	connection->mLocalConnectionID	 = connectionid;
	connection->mProperty			|= mProperty;
	connection->mPriority			 = mPriority;

	// Get socket by id.
	Socket* socket = networkmanager.SearchSocket( socketid );

	// Increase the reference count of this socket.
	socket->IncreaseRefCount( );

	networkmanager.StatisticCreateConnection( );

	mNewConnectionID = connectionid;

	return connection;
}

Connection* NetworkSession::SearchConnection( _dword socketid, const Address& address ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// Search existing connection by socket id and addresss.
	for ( ConnectionPool::Iterator it = mConnectionPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Connection* connection = it;

		if ( connection == _null )
			continue;

		// Socket not match, skip it.
		if ( connection->mSocketID != socketid )
			continue;

		// Internal remote address not match, skip it.
		if ( connection->mInternalAddress != address )
			continue;

		// All match, found it.
		return connection;
	}

	return _null;
}

Connection* NetworkSession::SearchConnection( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// Search existing connection with connection id.
	Connection** connectionpointer = mConnectionPool.Index( connectionid );

	return connectionpointer != _null ? *connectionpointer : _null;
}

Connection* NetworkSession::GetGroupFirstConnection( _dword groupid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	for ( _dword i = 0; i < mGroupArray.Number( ); i ++ )
	{
		Connection* connection = mGroupArray[i].mObject1;
		FL_ASSERT( connection != _null )

		if ( connection->mGroupID == groupid )
			return connection;
	}

	return _null;
}

_void NetworkSession::UpdateSession( _dword tickcount )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	_bool flushdeferpacket = _false;

	// Decide if we need flush defer packet.
	if ( mFlushInterval != 0 && mFlushInterval != -1 )
	{
		if ( mFlushTickcount + mFlushInterval <= tickcount )
		{
			mFlushTickcount = tickcount;

			flushdeferpacket = _true;
		}
	}

	// Check all connections.
	for ( ConnectionPool::Iterator it = mConnectionPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Connection* connection = it;

		if ( connection == _null )
			continue;

		// Flush all defer packet of the connection.
		if ( flushdeferpacket )
			connection->FlushDeferPacket( );

		// Check if a connection is still alive, if not, close it.
		if ( connection->KeepAlive( tickcount ) )
			continue;

		if ( connection->mPhase == _PHASE_CONNECTING || connection->mPhase == _PHASE_SENTJOINREQ )
		{
			Connection* newconnection = Reconnect( connection );

			// Reconnect happens.
			if ( newconnection != _null )
			{
				// Connection changed, we delete the old one.
				if ( connection != newconnection )
				{
					NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

					// Release the socket attached to the connection.
					networkmanager.ReleaseSocket( connection->mSocketID );

					delete connection;
				}

				continue;
			}
		}

		// Close the dead connection.
		CloseConnection( connection->mLocalConnectionID );

		// Dont delete connection here, because in pooling mode, user callback function maybe not called yet.
	}

	if ( mLastUpdateTickcount + 1000 > tickcount )
		return;

	_dword elapse = tickcount - mLastUpdateTickcount;

	// Update reference tick array.
	for ( _dword i = 0; i < mRefTickcountArray.Number( ); i ++ )
	{
		if ( mRefTickcountArray[i].mObject3 > elapse )
		{
			mRefTickcountArray[i].mObject3 -= elapse;
			continue;
		}

		mRefTickcountArray.Remove( i -- );
	}

	mLastUpdateTickcount = tickcount;

	// Update session statistics.
	mSessionStatistics->Update( );
}

_void NetworkSession::CheckDeadConnection( )
{
	// Check all connections.
	for ( ConnectionPool::Iterator it = mConnectionPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Connection* connection = it;

		if ( connection == _null )
			continue;

		// The connection is closed, we can delete it.
		if ( connection->mPhase == _PHASE_CLOSED )
		{
			// Remove it from connection pool.
			mConnectionPool.Remove( connection->mLocalConnectionID );

			// Delete the connection.
			delete connection;
		}
	}
}

_bool NetworkSession::OpenPort( const Address& localaddress, _dword groupid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	// Search the existing socket from network networkmanager by address.
	Socket* socket = networkmanager.SearchSocket( localaddress, _true );

	// If there is no existing socket with same address, create a new socket.
	if ( socket == _null )
		socket = networkmanager.CreateSocket( localaddress );

	// Create socket failed, return false.
	if ( socket == _null )
		return _false;

	// The socket is hosting by the session.
	if ( networkmanager.IsSocketHosting( (_dword) (*socket), mSessionID, groupid ) == _true )
		return _true;

	// Attach the socket to current session.
	networkmanager.SocketAttachSession( (_dword) (*socket), mSessionID, groupid );

	// This is not the new socket, so we dont need process more.
	if ( socket->mTotalOptNumber > 0 )
		return _true;

	// The address is a tcp port.
	if ( localaddress.mType == Address::_TYPE_TCP )
	{
		// Start listen.
		if ( ( (TCPSocket*) socket )->Listen( SocketOperation::_MAX_ACCEPTOPT_NUMBER ) == _false )
		{
			networkmanager.ReleaseSocket( (_dword) (*socket) );

			return _false;
		}

		// Create many sockets for accepting connection.
		for ( _dword i = 0; i < SocketOperation::_MAX_ACCEPTOPT_NUMBER; i ++ )
		{
			// Create socekt to accept new connection.
			Socket* acceptsocket = networkmanager.CreateSocket( Address::_TYPE_TCP );

			if ( acceptsocket != _null )
			{
				acceptsocket->SetLocalAddress( localaddress );

				// Create overlapped operation.
				SocketOperation* newoperation = mBufferManager.CreateOperation( );

				// Set session id.
				if ( newoperation != _null )
					newoperation->mSessionID = mSessionID;

				// Post accept operation.
				( (TCPSocket*) acceptsocket )->Accept( (_dword) (*socket), newoperation );
			}
		}
	}
	// The address is a udp port.
	else
	{
		// Post many receive operations.
		for ( _dword i = 0; i < SocketOperation::_MAX_RECVOPT_NUMBER; i ++ )
			( (UDPSocket*) socket )->Recv( mBufferManager.CreateOperation( ) );
	}

	return _true;
}

_void NetworkSession::ClosePort( const Address& localaddress )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	// Detach socket from current session by address
	networkmanager.SocketDetachSession( mSessionID, localaddress );
}

_void NetworkSession::ClosePort( _dword groupid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	// Detach socket from current session by group id.
	networkmanager.SocketDetachSession( mSessionID, groupid );

	if ( groupid != 0 )
		CloseGroup( groupid );
}

_bool NetworkSession::IsPortOpened( const Address& localaddress, _dword& groupid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	// Search the existing socket from network networkmanager by address.
	Socket* socket = networkmanager.SearchSocket( localaddress, _false );

	// Not found the socket, just return.
	if ( socket == _null )
		return _false;

	return networkmanager.IsSocketHosting( (_dword) (*socket), mSessionID, groupid );
}

_bool NetworkSession::IsPortOpened( _dword groupid, Address& localaddress )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	_dword socketid = 0;

	// There is no group host in this sesssion with the group id, return false.
	if ( networkmanager.IsGroupHosting( mSessionID, groupid, socketid ) == _false )
		return _false;

	// Search the existing socket from network networkmanager by address.
	Socket* socket = networkmanager.SearchSocket( socketid );

	// Get the local address.
	localaddress = socket->GetLocalAddress( );

	return _true;
}

_bool NetworkSession::PingSession( const Address& remoteaddress, _dword groupid, _dword userdata, _dword sessionid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	Address localaddress;
	localaddress.mIP	= 0;
	localaddress.mType	= remoteaddress.mType;
	localaddress.mPort	= 0;

	return JoinSessionHelper( localaddress, remoteaddress, groupid, userdata, sessionid, _true );
}

_bool NetworkSession::PingSession( const Address& localaddress, const Address& remoteaddress, _dword groupid, _dword userdata, _dword sessionid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	return JoinSessionHelper( localaddress, remoteaddress, groupid, userdata, sessionid, _true );
}

_bool NetworkSession::JoinSession( const Address& remoteaddress, _dword groupid, _dword userdata, _dword sessionid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	Address localaddress;
	localaddress.mIP	= 0;
	localaddress.mType	= remoteaddress.mType;
	localaddress.mPort	= 0;

	return JoinSessionHelper( localaddress, remoteaddress, groupid, userdata, sessionid, _false );
}

_bool NetworkSession::JoinSession( const Address& localaddress, const Address& remoteaddress, _dword groupid, _dword userdata, _dword sessionid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	return JoinSessionHelper( localaddress, remoteaddress, groupid, userdata, sessionid, _false );
}

_dword NetworkSession::GetSessionID( ) const
{
	return mSessionID;
}

_dword NetworkSession::GetSessionGlobalToken( ) const
{
	return mGlobalToken;
}

_dword NetworkSession::GetNewConnectionID( ) const
{
	return mNewConnectionID;
}

_void NetworkSession::EnablePollingMode( _bool enable )
{
	mPollingMode = enable;
}

_bool NetworkSession::IsPollingModeEnabled( ) const
{
	return mPollingMode;
}

_void NetworkSession::SetPriority( _dword priority )
{
	mPriority = priority;
}

_dword NetworkSession::GetPriority( ) const
{
	return mPriority;
}

_void NetworkSession::SetUserData( _dword userdata )
{
	mUserData = userdata;
}

_dword NetworkSession::GetUserData( ) const
{
	return mUserData;
}

_void NetworkSession::SetFlushInterval( _dword interval )
{
	mFlushInterval = interval;
}

_dword NetworkSession::GetFlushInterval( ) const
{
	return mFlushInterval;
}

_dword NetworkSession::GetAliveConnectionNumber( ) const
{
	return mAliveConnectionNumber;
}

_dword NetworkSession::GetSessionConnectionNumber( ) const
{
	FL_ASSERT( mConnectionPool.Number( ) > 0 )

	return mConnectionPool.Number( ) - 1;
}

_dword NetworkSession::GetSessionFirstConnectionID( ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	ConnectionPool::Iterator it = mConnectionPool.GetHeadIterator( );

	if ( it.Valid( ) == _false )
		return 0;

	it ++;

	return it.Valid( ) ? ( (Connection*) it )->mLocalConnectionID : 0;
}

_dword NetworkSession::GetSessionNextConnectionID( _dword previousid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	ConnectionPool::Iterator it = mConnectionPool.GetIterator( previousid );

	if ( it.Valid( ) == _false )
		return 0;

	it ++;

	return it.Valid( ) ? ( (Connection*) it )->mLocalConnectionID : 0;
}

_dword NetworkSession::GetGroupConnectionNumber( _dword groupid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	for ( _dword i = 0; i < mGroupArray.Number( ); i ++ )
	{
		Connection* connection = mGroupArray[i].mObject1;
		FL_ASSERT( connection != _null )

		if ( connection->mGroupID == groupid )
			return mGroupArray[i].mObject2;
	}

	return 0;
}

_dword NetworkSession::GetGroupFirstConnectionID( _dword groupid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	Connection* connection = GetGroupFirstConnection( groupid );
	if ( connection == _null )
		return 0;

	return connection->mLocalConnectionID;
}

_dword NetworkSession::GetGroupNextConnectionID( _dword groupid, _dword previousid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	Connection* connection = SearchConnection( previousid );
	if ( connection == _null )
		return 0;

	connection = connection->mNextConnectionInGroup;
	if ( connection == _null )
		return 0;

	return connection->mLocalConnectionID;
}

_void NetworkSession::SetGroupUserData( _dword groupid, _dword userdata )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	_dword index = GetGroupIndex( groupid );
	if ( index == -1 )
		return;

	mGroupArray[ index ].mObject3 = userdata;
}

_dword NetworkSession::GetGroupUserData( _dword groupid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	_dword index = GetGroupIndex( groupid );
	if ( index == -1 )
		return 0;

	return mGroupArray[ index ].mObject3;
}

_void NetworkSession::SetConnectionGroup( _dword connectionid, _dword groupid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	// The new group id is same as the old one, just return.
	if ( connection->mGroupID == groupid )
		return;

	// Already in a group, remove from it first.
	if ( connection->mGroupID != 0 )
	{
		_dword index = GetGroupIndex( connection->mGroupID );

		// Bad group information, should be a bug here.
		if ( index == -1 )
		{
			connection->mGroupID = groupid;
			return;
		}

		// Get the first connection of the groupid.
		Connection* connection1 = mGroupArray[ index ].mObject1;
		FL_ASSERT( connection1 != _null )

		// Found the connection to be removed.
		if ( connection1 == connection )
		{
			// There is at least one more connection in the group, set it as first connection.
			if ( connection1->mNextConnectionInGroup != _null )
			{
				FL_ASSERT( mGroupArray[ index ].mObject2 > 1 )

				mGroupArray[ index ].mObject1 = connection1->mNextConnectionInGroup;
				mGroupArray[ index ].mObject2 = mGroupArray[ index ].mObject2 - 1;
			}
			// There is no connection left in the group, so remove the group.
			else
			{
				FL_ASSERT( mGroupArray[ index ].mObject2 == 1 )

				mGroupArray.Remove( index );
			}
		}
		else
		{
			Connection* connection2 = connection1->mNextConnectionInGroup;

			while ( connection2 != _null )
			{
				// Found the connection.
				if ( connection2 == connection )
				{
					FL_ASSERT( mGroupArray[ index ].mObject2 > 1 )

					// Relink group link.
					connection1->mNextConnectionInGroup = connection2->mNextConnectionInGroup;

					mGroupArray[ index ].mObject2 = mGroupArray[ index ].mObject2 - 1;

					break;
				}

				connection1 = connection2;
				connection2 = connection1->mNextConnectionInGroup;
			}
		}
	}

	// Put connection into another groupid.
	if ( groupid != 0 )
	{
		_dword index = GetGroupIndex( groupid );

		// There is no groupid, so insert a new one.
		if ( index == -1 )
		{
			mGroupArray.Append( GroupTriple( connection, 1, 0 ) );
		}
		else
		{
			// Get the first connection of the groupid.
			Connection* connection1 = mGroupArray[ index ].mObject1;
			FL_ASSERT( connection1 != _null )

			// Relink group link.
			connection->mNextConnectionInGroup = connection1;

			mGroupArray[ index ].mObject1 = connection;
			mGroupArray[ index ].mObject2 = mGroupArray[ index ].mObject2 + 1;
		}
	}

	connection->mGroupID = groupid;
}

_dword NetworkSession::GetConnectionGroup( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mGroupID;
}

_void NetworkSession::SetConnectionUserData( _dword connectionid, _dword userdata )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	connection->mUserData = userdata;
}

_dword NetworkSession::GetConnectionUserData( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mUserData;
}

_void NetworkSession::SetConnectionTimer( _dword connectionid, _dword timer )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	connection->mTimer = timer;
}

_dword NetworkSession::GetConnectionTimer( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mTimer;
}

_void NetworkSession::SetConnectionTimeout( _dword connectionid, _dword timeout )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	connection->mTimeout = timeout;
}

_dword NetworkSession::GetConnectionTimeout( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mTimeout;
}

_void NetworkSession::SetConnectionConnectInterval( _dword connectionid, _dword interval )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	connection->mConnectInterval = interval;
}

_dword NetworkSession::GetConnectionConnectInterval( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mConnectInterval;
}

_void NetworkSession::SetConnectionPriority( _dword connectionid, _dword priority )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	connection->mPriority = priority;
}

_dword NetworkSession::GetConnectionPriority( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mPriority;
}

_dword NetworkSession::GetConnectionGlobalToken( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mGlobalToken;
}

_dword NetworkSession::GetConnectionLatency( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mLatency;
}

_dword NetworkSession::GetConnectionDuration( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return System::GetCurrentTickcount( ) - connection->mCreateTickcount;
}

_dword NetworkSession::GetConnectionPhase( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return _PHASE_CLOSED;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mPhase;
}

_dword NetworkSession::GetConnectionSentBytes( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mSentBytes;
}

_dword NetworkSession::GetConnectionRecentSentBytes( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mRecentSentBytes[0];
}

_float NetworkSession::GetConnectionSendSpeed( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	_dword bytes = 0;
	for ( _dword i = 0; i < 10; i ++ )
		bytes += connection->mRecentSentBytes[i];

	return (_float) bytes / 1024.0f / 10.0f;
}

_dword NetworkSession::GetConnectionReceivedBytes( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mReceivedBytes;
}

_dword NetworkSession::GetConnectionRecentReceivedBytes( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mRecentReceivedBytes[0];
}

_float NetworkSession::GetConnectionReceiveSpeed( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	_dword bytes = 0;
	for ( _dword i = 0; i < 10; i ++ )
		bytes += connection->mRecentReceivedBytes[i];

	return (_float) bytes / 1024.0f / 10.0f;
}

_dword NetworkSession::GetConnectionCloseReason( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return _REASON_UNKNOWN;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mCloseReason;
}

StringPtr NetworkSession::GetConnectionCloseReasonString( _dword connectionid ) const
{
	_dword closereason = GetConnectionCloseReason( connectionid );

	// Convert close reason from id to string.
	switch ( closereason )
	{
		case INetworkSession::_REASON_TIMEOUT:			return L"TIME OUT";
		case INetworkSession::_REASON_LOCALCLOSE:		return L"LOCAL CLOSE";
		case INetworkSession::_REASON_REMOTECLOSE:		return L"REMOTE CLOSE";
		case INetworkSession::_REASON_INVALIDPACKET:	return L"INVALID PACKET";
		case INetworkSession::_REASON_SENDERROR:		return L"SEND ERROR";
		case INetworkSession::_REASON_RECVERROR:		return L"RECV ERROR";
	}

	return L"UNKNOWN";
}

Address NetworkSession::GetConnectionLocalAddress( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return Address( );

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mLocalAddress;
}

Address NetworkSession::GetConnectionRemoteAddress( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return Address( );

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mRemoteAddress;
}

_dword NetworkSession::GetConnectionRemoteSessionID( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mRemoteSessionID;
}

_bool NetworkSession::IsLANConnection( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return _false;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return ( connection->mProperty & Connection::_PROPERTY_LAN ) != 0;
}

_bool NetworkSession::SetConnectionBackupAddress( _dword connectionid, const Address& remoteaddress )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return _false;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	connection->mBackupAddress = remoteaddress;

	// We have another backup address.
	if ( remoteaddress != Address( ) )
	{
		// Make connect timeout fast.
		if ( connection->mConnectInterval == Connection::_CONNECT_INTERVAL )
			connection->mConnectInterval /= 2;
	}

	return _true;
}

_dword NetworkSession::SearchConnection( const Address& remoteaddress, _dword groupid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// Search existing connection by socket id and addresss.
	for ( ConnectionPool::Iterator it = mConnectionPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Connection* connection = it;

		if ( connection == _null )
			continue;

		// Group not match, skip it.
		if ( groupid != 0 && connection->mGroupID != groupid )
			continue;

		// Internal remote address not match, skip it.
		if ( connection->mRemoteAddress != remoteaddress && connection->mInternalAddress != remoteaddress )
			continue;

		// Bad connection phase.
		if ( connection->mPhase == _PHASE_INIVCLOSE || connection->mPhase == _PHASE_PASVCLOSE || connection->mPhase == _PHASE_CLOSED )
			return 0;

		// All match, found it.
		return connection->mLocalConnectionID;
	}

	return 0;
}

_void NetworkSession::EnableSessionTickcount( _bool enable )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	mEnableSessionTickcount = enable;
}

_bool NetworkSession::IsSessionTickcountEnabled( ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	return mEnableSessionTickcount;
}

_void NetworkSession::InitSessionTickcount( _dword tickcount )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	_dword oldtick = GetSessionTickcount( );

	do
	{
		mInitSessionTickcount = System::GetCurrentTickcount( ) - tickcount;
	}
	while ( mInitSessionTickcount == 0 );

	_dword newtick = GetSessionTickcount( );

	// Calcuate delta tickcount.
	_dword delta = newtick - oldtick;

	if ( delta == 0 )
		return;

	// Update reference tick array.
	for ( _dword i = 0; i < mRefTickcountArray.Number( ); i ++ )
		mRefTickcountArray[i].mObject2 += delta;

	// Update connection delay packet array.
	for ( ConnectionPool::Iterator it = mConnectionPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Connection* connection = it;

		if ( connection == _null )
			continue;

		connection->UpdateDelayPacket( delta );
	}
}

_dword NetworkSession::GetSessionTickcount( ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( mInitSessionTickcount == 0 )
		return 0;

	_dword tickcount = System::GetCurrentTickcount( );

	// Make sure get session tickcount not result in 0.
	while ( tickcount == mInitSessionTickcount )
		tickcount = System::GetCurrentTickcount( );

	return tickcount - mInitSessionTickcount;
}

_dword NetworkSession::GetPacketTickcount( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mPacketTickcount;
}

_void NetworkSession::SetPacketReference( _dword connectionid, _dword referenceid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	connection->mSendPacketReference = referenceid;
}

_dword NetworkSession::GetPacketReference( _dword connectionid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return 0;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	return connection->mRecvPacketReference;
}

_void NetworkSession::SetReferenceTickcount( _dword referenceid, _dword sessiontickcount, _dword duration )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	_dword index = mRefTickcountArray.SearchAscending( referenceid ).mIndex;

	if ( index == -1 )
	{
		mRefTickcountArray.InsertAscending( RefTickcount( referenceid, sessiontickcount, duration ) );
	}
	else
	{
		mRefTickcountArray[ index ].mObject2 = sessiontickcount;
		mRefTickcountArray[ index ].mObject3 = duration;
	}
}

_dword NetworkSession::GetReferenceTickcount( _dword referenceid ) const
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	_dword index = mRefTickcountArray.SearchAscending( referenceid ).mIndex;

	if ( index == -1 )
		return 0;

	return mRefTickcountArray[ index ].mObject2;
}

_bool NetworkSession::SetDelayTickcount( _dword connectionid, _dword channel, _dword sessiontickcount, _dword referenceid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return _false;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	connection->mDelayChannel	= channel;
	connection->mDelayTickcount	= sessiontickcount;
	connection->mDelayReference	= referenceid;

	return _true;
}

_bool NetworkSession::SetGroupDelayTickcount( _dword groupid, _dword excludeconnection, _dword channel, _dword sessiontickcount, _dword referenceid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	for ( Connection* connection = GetGroupFirstConnection( groupid ); connection != _null; connection = connection->mNextConnectionInGroup )
	{
		// Skip the connection excluded.
		if ( connection->mLocalConnectionID == excludeconnection )
			continue;

		// Skip not connected connection.
		if ( connection->mPhase != _PHASE_CONNECTED )
			continue;

		connection->mDelayChannel	= channel;
		connection->mDelayTickcount	= sessiontickcount;
		connection->mDelayReference	= referenceid;
	}

	return _true;
}

_bool NetworkSession::SetSessionDelayTickcount( _dword excludegroup, _dword excludeconnection, _dword channel, _dword sessiontickcount, _dword referenceid )
{
	for ( ConnectionPool::Iterator it = mConnectionPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Connection* connection = it;

		if ( connection == _null )
			continue;

		// Skip not connected connection.
		if ( connection->mPhase != _PHASE_CONNECTED )
			continue;

		// Skip the connection excluded.
		if ( excludegroup != 0 && connection->mGroupID == excludegroup )
			continue;

		// Skip the connection excluded.
		if ( connection->mLocalConnectionID == excludeconnection )
			continue;

		connection->mDelayChannel	= channel;
		connection->mDelayTickcount	= sessiontickcount;
		connection->mDelayReference	= referenceid;
	}

	return _true;
}

_bool NetworkSession::SendPacket( _dword connectionid, const _void* buffer, _dword length )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( length == 0 )
		return _true;

	FL_ASSERT( buffer != _null )

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return _false;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	SendPacketHelper( connection, buffer, length );

	return _true;
}

_bool NetworkSession::SendDeferPacket( _dword connectionid, const _void* buffer, _dword length )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( mFlushInterval == 0 )
		return SendPacket( connectionid, buffer, length );

	if ( length == 0 )
		return _true;

	FL_ASSERT( buffer != _null )

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return _false;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	SendDeferPacketHelper( connection, buffer, length );

	return _true;
}

_bool NetworkSession::FlushDeferPacket( _dword connectionid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( connectionid == 0 || mConnectionPool.Index( connectionid ) == _null )
		return _false;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	FL_ASSERT( connection != _null )

	connection->FlushDeferPacket( );
	connection->FlushFinish( );

	return _true;
}

_bool NetworkSession::SendGroupPacket( _dword groupid, _dword excludeconnection, const _void* buffer, _dword length )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( length == 0 )
		return _true;

	FL_ASSERT( buffer != _null )

	for ( Connection* connection = GetGroupFirstConnection( groupid ); connection != _null; connection = connection->mNextConnectionInGroup )
	{
		// Skip the connection excluded.
		if ( connection->mLocalConnectionID == excludeconnection )
			continue;

		// Skip not connected connection.
		if ( connection->mPhase != _PHASE_CONNECTED )
			continue;

		SendPacketHelper( connection, buffer, length );
	}

	return _true;
}

_bool NetworkSession::SendGroupDeferPacket( _dword groupid, _dword excludeconnection, const _void* buffer, _dword length )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( mFlushInterval == 0 )
		return SendGroupPacket( groupid, excludeconnection, buffer, length );

	if ( length == 0 )
		return _true;

	FL_ASSERT( buffer != _null )

	for ( Connection* connection = GetGroupFirstConnection( groupid ); connection != _null; connection = connection->mNextConnectionInGroup )
	{
		// Skip the connection excluded.
		if ( connection->mLocalConnectionID == excludeconnection )
			continue;

		// Skip not connected connection.
		if ( connection->mPhase != _PHASE_CONNECTED )
			continue;

		SendDeferPacketHelper( connection, buffer, length );
	}

	return _true;
}

_bool NetworkSession::FlushGroupDeferPacket( _dword groupid, _dword excludeconnection )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	for ( Connection* connection = GetGroupFirstConnection( groupid ); connection != _null; connection = connection->mNextConnectionInGroup )
	{
		// Skip the connection excluded.
		if ( connection->mLocalConnectionID == excludeconnection )
			continue;

		// Skip not connected connection.
		if ( connection->mPhase != _PHASE_CONNECTED )
			continue;

		connection->FlushDeferPacket( );
		connection->FlushFinish( );
	}

	return _true;
}

_bool NetworkSession::SendSessionPacket( _dword excludegroup, _dword excludeconnection, const _void* buffer, _dword length )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( length == 0 )
		return _true;

	FL_ASSERT( buffer != _null )

	// Send to all connections.
	for ( ConnectionPool::Iterator it = mConnectionPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Connection* connection = it;

		if ( connection == _null )
			continue;

		// Skip not connected connection.
		if ( connection->mPhase != _PHASE_CONNECTED )
			continue;

		// Skip the connection excluded.
		if ( excludegroup != 0 && connection->mGroupID == excludegroup )
			continue;

		// Skip the connection excluded.
		if ( connection->mLocalConnectionID == excludeconnection )
			continue;

		SendPacketHelper( connection, buffer, length );
	}

	return _true;
}

_bool NetworkSession::SendSessionDeferPacket( _dword excludegroup, _dword excludeconnection, const _void* buffer, _dword length )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( mFlushInterval == 0 )
		return SendSessionPacket( excludegroup, excludeconnection, buffer, length );

	if ( length == 0 )
		return _true;

	FL_ASSERT( buffer != _null )

	// Send to all connections.
	for ( ConnectionPool::Iterator it = mConnectionPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Connection* connection = it;

		if ( connection == _null )
			continue;

		// Skip not connected connection.
		if ( connection->mPhase != _PHASE_CONNECTED )
			continue;

		// Skip the connection excluded.
		if ( excludegroup != 0 && connection->mGroupID == excludegroup )
			continue;

		// Skip the connection excluded.
		if ( connection->mLocalConnectionID == excludeconnection )
			continue;

		SendDeferPacketHelper( connection, buffer, length );
	}

	return _true;
}

_bool NetworkSession::FlushSessionDeferPacket( _dword excludegroup, _dword excludeconnection )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// Send to all connections.
	for ( ConnectionPool::Iterator it = mConnectionPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Connection* connection = it;

		if ( connection == _null )
			continue;

		// Skip not connected connection.
		if ( connection->mPhase != _PHASE_CONNECTED )
			continue;

		// Skip the connection excluded.
		if ( excludegroup != 0 && connection->mGroupID == excludegroup )
			continue;

		// Skip the connection excluded.
		if ( connection->mLocalConnectionID == excludeconnection )
			continue;

		connection->FlushDeferPacket( );
		connection->FlushFinish( );
	}

	return _true;
}

_bool NetworkSession::EnableAutoReconnect( _dword connectionid, _bool enable )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	SendConnectionPropertyHelper( connectionid, Connection::_PROPERTY_AUTORECONNECT, enable );

	return _true;
}

_bool NetworkSession::EnableCompressPacket( _dword connectionid, _bool enable )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	SendConnectionPropertyHelper( connectionid, Connection::_PROPERTY_ASKCOMPRESS, enable );

	return _true;
}

_bool NetworkSession::EnableResendPacketList( _dword connectionid, _bool enable )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	SendConnectionPropertyHelper( connectionid, Connection::_PROPERTY_RESENDWITHSN, enable );

	return _true;
}

_bool NetworkSession::EnableNoNeedConfirmPacket( _dword connectionid, _bool enable )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	SendConnectionPropertyHelper( connectionid, Connection::_PROPERTY_NONEEDCONFIRM, enable );

	return _true;
}

_bool NetworkSession::SyncConnectionProperty( _dword connectionid )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( mConnectionPool.Index( connectionid ) == _null )
		return _false;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	if ( connection == _null )
		return _false;

	if ( connection->mPhase != _PHASE_CONNECTED )
		return _false;

	// Build connection property packet.
	ConnectionPropertyPacket connectionproperty( connection->mTimeout, connection->mPriority );

	// Send connection property packet.
	return connection->SendPacket( &connectionproperty, connectionproperty.mPacketSize );
}

_void NetworkSession::CloseConnection( _dword connectionid, _bool needcallback )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( mConnectionPool.Index( connectionid ) == _null )
		return;

	// Get connection from connection pool by id.
	Connection* connection = mConnectionPool[ connectionid ];
	if ( connection == _null )
		return;

	// This connection already closed.
	if ( connection->mPhase == _PHASE_CLOSED )
		return;

	// Set close connection reason.
	if ( connection->mCloseReason == _REASON_UNKNOWN )
		connection->mCloseReason = _REASON_LOCALCLOSE;

	StringPtr reason = L"UNKNOWN";

	switch ( connection->mCloseReason )
	{
		case _REASON_TIMEOUT:		reason = L"TIMEOUT"; break;
		case _REASON_LOCALCLOSE:	reason = L"LOCAL CLOSE"; break;
		case _REASON_REMOTECLOSE:	reason = L"REMOTE CLOSE"; break;
		case _REASON_INVALIDPACKET:	reason = L"INVALID PACKET"; break;
		case _REASON_SENDERROR:		reason = L"SEND ERROR"; break;
		case _REASON_RECVERROR:		reason = L"RECV ERROR"; break;
	}

	StringPtr phase = L"UNKNOWN";

	switch ( connection->mPhase )
	{
		case _PHASE_CONNECTING:		phase = L"CONNECTING"; break;
		case _PHASE_WAITJOINREQ:	phase = L"WAITJOINREQ"; break;
		case _PHASE_SENTJOINREQ:	phase = L"SENTJOINREQ"; break;
		case _PHASE_RECVJOINREQ:	phase = L"RECVJOINREQ"; break;
		case _PHASE_CONNECTED:		phase = L"CONNECTED"; break;
		case _PHASE_INIVCLOSE:		phase = L"INIVCLOSE"; break;
		case _PHASE_PASVCLOSE:		phase = L"PASVCLOSE"; break;
		case _PHASE_CLOSED:			phase = L"CLOSED"; break;
	}

	_char buffer[1024];
	StringFormatter::FormatBuffer( buffer, 1024, L"[NK] Close connection : SID(%d), CID(%d), Phase(%s), Reason(%s), %s://%d.%d.%d.%d:%d->%d.%d.%d.%d:%d",
		mSessionID, connectionid, phase, reason, connection->mLocalAddress.mType == Address::_TYPE_TCP ? L"tcp" : L"udp",
		connection->mLocalAddress.mIP4, connection->mLocalAddress.mIP3, connection->mLocalAddress.mIP2, connection->mLocalAddress.mIP1, connection->mLocalAddress.mPort,
		connection->mRemoteAddress.mIP4, connection->mRemoteAddress.mIP3,connection->mRemoteAddress.mIP2, connection->mRemoteAddress.mIP1, connection->mRemoteAddress.mPort );

	PS_TRACE1( L"%s\r\n", buffer );

	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	switch ( connection->mPhase )
	{
		case _PHASE_CONNECTING:
		case _PHASE_SENTJOINREQ:
		{
			// Call the callback function to inform application, cant join target session.
			if ( needcallback )
				networkmanager.CallJoinFailedCallback( mSessionID, connection->mLocalConnectionID );

			break;
		}

		case _PHASE_CONNECTED:
		case _PHASE_INIVCLOSE:
		case _PHASE_PASVCLOSE:
		{
			// Call the callback function to inform application, a connection is closed.
			if ( needcallback )
				networkmanager.CallDeleteConnectionCallback( mSessionID, connectionid );

			_dword groupid = connection->mGroupID;

			// Remove connection from group link.
			if ( connection->mGroupID != 0 )
				SetConnectionGroup( connectionid, 0 );

			// Save group ID back, just in order to give correct groupid at pooling mode DeleteConnectionCallback. 
			connection->mGroupID = groupid;

			break;
		}
	}

	connection->Close( );

	// Release the socket attached to the connection.
	networkmanager.ReleaseSocket( connection->mSocketID );

	networkmanager.StatisticDeleteConnection( );
}

_void NetworkSession::CloseGroup( _dword groupid, _bool needcallback )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	// Get the first connection in the group.
	Connection* connection = GetGroupFirstConnection( groupid );

	while ( connection != _null )
	{
		Connection* temp = connection;
		connection = connection->mNextConnectionInGroup;

		// Close the connection.
		CloseConnection( temp->mLocalConnectionID, needcallback );
	}
}

_void NetworkSession::CloseSession( _bool needcallback )
{
	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( mNetworkLock );

	if ( mRefCount > 0 )
	{
		mClosed = _true;
		return;
	}

	// Close all connections.
	for ( ConnectionPool::Iterator it = mConnectionPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Connection* connection = it;
		if ( connection == _null )
			continue;

		CloseConnection( connection->mLocalConnectionID, needcallback );

		// Delete the connection.
		delete connection;
	}

	mAliveConnectionNumber = 0;

	if ( mPollingMode == _false || needcallback == _false )
	{
		// Clear connection pool.
		mConnectionPool.Clear( );
		mConnectionPool.Insert( _null, 0 );
	}

	// Clear group array.
	mGroupArray.Clear( );

	ClosePort( 0 );
}

_dword NetworkSession::GetRecentSendTime( _dword seconds ) const
{
	return mSessionStatistics->GetRecentSendTime( seconds );
}

_dword NetworkSession::GetRecentSendBytes( _dword seconds ) const
{
	return mSessionStatistics->GetRecentSendBytes( seconds );
}

_dword NetworkSession::GetRecentReceiveTime( _dword seconds ) const
{
	return mSessionStatistics->GetRecentReceiveTime( seconds );
}

_dword NetworkSession::GetRecentReceiveBytes( _dword seconds ) const
{
	return mSessionStatistics->GetRecentReceiveBytes( seconds );
}

INetworkStatistics*	NetworkSession::GetStatistics( ) const
{
	return mSessionStatistics;
}