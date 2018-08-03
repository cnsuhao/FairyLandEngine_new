//============================================================================
// NetworkNATHelper.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// NetworkNATHelper Implementation
//----------------------------------------------------------------------------

NetworkNATHelper::NetworkNATHelper( BufferManager& buffermanager ) : NetworkHelper( buffermanager )
{
}

_bool NetworkNATHelper::ProcessPacket( _dword socketid, Packet* packet, const Address& address )
{
	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	// Get socket from socket pool by id.
	Socket* socket = networkmanager.SearchSocket( socketid );
	FL_ASSERT( socket != _null )

	switch ( packet->mMagicCode )
	{
		// Process keep nat alive, at NAT Server.
		case KeepAlivePacket::_MAGIC_CODE:
		{
			if ( mHelperSocketID != socketid )
				return _false;

			return _true;
		}

		// Process connect req, at NAT Server.
		case ConnectReqPacket::_MAGIC_CODE:
		{
			ConnectReqPacket* connectreq = (ConnectReqPacket*) packet;

			// Build connect hint packet.
			ConnectHintPacket connecthint( connectreq->mSessionID, connectreq->mSourcePrivateAddr, address, connectreq->mTargetPublicAddr );

			PacketBuffer sendbuffer;
			sendbuffer.mBuffer[0] = &connecthint;
			sendbuffer.mLength[0] = connecthint.mPacketSize;

			// Send connect hint packet to target address.
			( (UDPSocket*) socket )->Send( connectreq->mTargetPublicAddr, mBufferManager.CreateOperation( sendbuffer ) );

			return _true;
		}

		// Process connect hint, at target pc.
		case ConnectHintPacket::_MAGIC_CODE:
		{
			ConnectHintPacket* connecthint = (ConnectHintPacket*) packet;

			// Get source address from packet.
			Address sourceaddress = connecthint->mSourcePublicAddr;
			// Get target address from packet.
			Address targetaddress = connecthint->mTargetPublicAddr;

			// If source & target pc behind same NAT, then send to source private address directly.
			if ( targetaddress.mIP == sourceaddress.mIP )
				sourceaddress = connecthint->mSourcePrivateAddr;

			//PS_TRACE10( L"[NK] Some one want to join you, just give him a hint %d.%d.%d.%d:%d => %d.%d.%d.%d:%d\r\n",
			//	sourceaddress.mIP4, sourceaddress.mIP3, sourceaddress.mIP2, sourceaddress.mIP1, sourceaddress.mPort,
			//	targetaddress.mIP4, targetaddress.mIP3, targetaddress.mIP2, targetaddress.mIP1, targetaddress.mPort )

			// Build join hint packet.
			JoinHintPacket joinhint( connecthint->mSessionID, targetaddress );

			PacketBuffer sendbuffer;
			sendbuffer.mBuffer[0] = &joinhint;
			sendbuffer.mLength[0] = joinhint.mPacketSize;

			// Send join hint packet to target address in order to break some gateway.
			( (UDPSocket*) socket )->Send( sourceaddress, mBufferManager.CreateOperation( sendbuffer ) );

			return _true;
		}
	}

	return _false;
}

_void NetworkNATHelper::ConnectUseHelper( _dword socketid, _dword sessionid, const Address& address )
{
	NetworkManager& networkmanager = (NetworkManager&) GetNetworkManager( );

	if ( mHelperServer.mIP == 0 || mHelperServer.mPort == 0 || mHelperServer.mType != Address::_TYPE_UDP )
		return;

	// Get socket from socket pool by id.
	Socket* socket = networkmanager.SearchSocket( socketid );
	FL_ASSERT( socket != _null )

	// Get local address of the socket.
	Address localaddress = socket->GetLocalAddress( );

	if ( localaddress.mIP == 0 )
		localaddress.mIP = NetworkManager::GetLocalIP( 0 );

	// Build connect req packet.
	ConnectReqPacket connectreq( (_word) sessionid, localaddress, address );

	PacketBuffer sendbuffer;
	sendbuffer.mBuffer[0] = &connectreq;
	sendbuffer.mLength[0] = connectreq.mPacketSize;

	// Post overlapped send operation.
	( (UDPSocket*) socket )->Send( mHelperServer, mBufferManager.CreateOperation( sendbuffer ) );

	//PS_TRACE5( L"[NK] Send NAT connect request to %d.%d.%d.%d:%d",
	//	mHelperServer.mIP4, mHelperServer.mIP3, mHelperServer.mIP2, mHelperServer.mIP1, mHelperServer.mPort );
}