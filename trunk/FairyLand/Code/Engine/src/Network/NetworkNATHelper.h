//============================================================================
// NetworkNATHelper.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// NetworkNATHelper
//----------------------------------------------------------------------------

class NetworkNATHelper : public NetworkHelper
{
public:
	NetworkNATHelper( BufferManager& buffermanager );

	_bool ProcessPacket( _dword socketid, Packet* packet, const Address& address );
	_void ConnectUseHelper( _dword socketid, _dword sessionid, const Address& address );
};

};