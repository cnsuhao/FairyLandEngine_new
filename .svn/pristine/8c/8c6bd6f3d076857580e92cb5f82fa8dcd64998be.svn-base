//============================================================================
// FairyLandDll.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

#include "FairyLand.h"

extern "C" __declspec( dllexport ) FL::IFairyLandWorker*    CreateFairyLandWorker();
extern "C" __declspec( dllexport ) FL::IResourceCache*		CreateResourceCache( );
extern "C" __declspec( dllexport ) FL::IResourceManager*	CreateResourceManager( );
extern "C" __declspec( dllexport ) FL::IResourceDownloader*	CreateResourceDownloader( FL::IFileDownloader* downloader );
extern "C" __declspec( dllexport ) FL::IResourceReplacer*	CreateResourceReplacer( );
extern "C" __declspec( dllexport ) FL::IRenderDevice*		CreateRenderDevice( FL::_handle window, FL::_dword devicetype );
extern "C" __declspec( dllexport ) FL::ISoundDevice*		CreateSoundDevice( FL::_handle module, FL::_dword maxchannels );
extern "C" __declspec( dllexport ) FL::INetworkManager*		CreateNetworkManager( );
extern "C" __declspec( dllexport ) FL::IDatabaseManager*	CreateDatabaseManager( );
//extern "C" __declspec( dllexport ) FL::IScriptManager*		CreateScriptManager(Script_Type stype);

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