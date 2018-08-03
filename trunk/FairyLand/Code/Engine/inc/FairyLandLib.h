//============================================================================
// FairyLandLib.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

#include "FairyLand.h"

FL::IFairyLandWorker*		CreateFairyLandWorker();
FL::IResourceCache*			CreateResourceCache( );
FL::IResourceManager*		CreateResourceManager( );
FL::IResourceDownloader*	CreateResourceDownloader( FL::IFileDownloader* downloader );
FL::IResourceReplacer*		CreateResourceReplacer( );
FL::IGraphicsManager*		CreateGraphicsManager( );
FL::ISoundDevice*			CreateSoundDevice( FL::_handle module, FL::_dword maxchannels );
FL::INetworkManager*		CreateNetworkManager( );
FL::IDatabaseManager*		CreateDatabaseManager( );

#ifndef _DEBUG
#pragma comment( lib, "FairyLandStorage" )
#pragma comment( lib, "FairyLandGraphics" )
#pragma comment( lib, "FairyLandSound" )
#pragma comment( lib, "FairyLandNetwork" )
#pragma comment( lib, "FairyLandDatabase" )
#else
#pragma comment( lib, "FairyLandStorage_d" )
#pragma comment( lib, "FairyLandGraphics_d" )
#pragma comment( lib, "FairyLandSound_d" )
#pragma comment( lib, "FairyLandNetwork_d" )
#pragma comment( lib, "FairyLandDatabase_d" )
#endif