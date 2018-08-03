//============================================================================
// FairyLandNetwork.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandNetwork.h"

//----------------------------------------------------------------------------
// CreateNetworkManager Implementation
//----------------------------------------------------------------------------

INetworkManager* CreateNetworkManager( )
{
	INetworkManager* networkmanager = GetNetworkManagerPointer( );

	if ( networkmanager != _null )
		return networkmanager;

	networkmanager = new NetworkManager( );
	GetFairyLandGlobal( ).mNetworkManager = networkmanager;

	PS_TRACE( L"[KL] NetworkManager Created\r\n" )

	return networkmanager;
}