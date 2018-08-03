//============================================================================
// FairyLandGlobal.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

struct FairyLandGlobal
{
	_dword					mRandomSeed;
	Memory*					mMemory;
	Trace*					mTrace;
	DebugHelperLibrary*		mDebug;
	MainWindow*				mMainWindow;

	IResourceManager*		mResourceManager;

	IResourceCache*			mResourceCache;

	IResourceDownloader*	mResourceDownloader;

	IResourceReplacer*		mResourceReplacer;

	IGraphicsManager*		mGraphicsManager;
	IRenderDevice*			mRenderDevice;
	IGeometryFactory*		mGeometryFactory;
	ITextureFactory*		mTextureFactory;
	ITechniqueFactory*		mTechniqueFactory;
	IModelFactory*			mModelFactory;
	IParticleFactory*		mParticleFactory;
	IPhysicsFactory*		mPhysicsFactory;
	ISceneFactory*			mSceneFactory;
	IRenderConfig*			mRenderConfig;
	IRenderStatistics*		mRenderStatistics;

	ISoundDevice*			mSoundDevice;

	INetworkManager*		mNetworkManager;

	IDatabaseManager*		mDatabaseManager;

//	IScriptManager*			mScriptManager;

	IFairyLandWorker*		mFairyLandWorker;

	#ifdef _MULTIGLOBAL_ENABLE
	_void*					mSingletons[256];
	#endif
};

#ifndef _MULTIGLOBAL_ENABLE

extern FairyLandGlobal gGlobal;

inline _void SetFairyLandGlobal( _void* global )
{
}

inline FairyLandGlobal& GetGlobalHelper( )
{
	return gGlobal;
}

#else

_dword					GetFairyLandSingletonToken( );
_void					NewFairyLandGlobalContext( );

IFairyLandWorker&		GetFairyLandWorker();
IFairyLandWorker*		GetFairyLandWorkerPointer();

_void					SetFairyLandGlobal( _void* global );
FairyLandGlobal&			GetFairyLandGlobal( );

IResourceManager*		GetResourceManagerPointer( );
IResourceManager&		GetResourceManager( );

IResourceCache*			GetResourceCachePointer( );
IResourceCache&			GetResourceCache( );

IResourceDownloader*	GetResourceDownloaderPointer( );
IResourceDownloader&	GetResourceDownloader( );

IResourceReplacer*		GetResourceReplacerPointer( );
IResourceReplacer&		GetResourceReplacer( );

IGraphicsManager*		GetGraphicsManagerPointer( );
IGraphicsManager&		GetGraphicsManager( );
IRenderDevice*			GetRenderDevicePointer( );
IRenderDevice&			GetRenderDevice( );

IGeometryFactory*		GetGeometryFactoryPointer( );
IGeometryFactory&		GetGeometryFactory( );

ITextureFactory*		GetTextureFactoryPointer( );
ITextureFactory&		GetTextureFactory( );


ITechniqueFactory*		GetTechniqueFactoryPointer( );
ITechniqueFactory&		GetTechniqueFactory( );

IModelFactory*			GetModelFactoryPointer();
IModelFactory&			GetModelFactory( );

IParticleFactory*		GetParticleFactoryPointer( );
IParticleFactory&		GetParticleFactory( );

IPhysicsFactory&		GetPhysicsFactory( );
ISceneFactory&			GetSceneFactory( );
IRenderConfig&			GetRenderConfig( );
IRenderStatistics&		GetRenderStatistics( );

ISoundDevice*			GetSoundDevicePointer( );
ISoundDevice&			GetSoundDevice( );

INetworkManager*		GetNetworkManagerPointer( );
INetworkManager&		GetNetworkManager( );

IDatabaseManager*		GetDatabaseManagerPointer( );
IDatabaseManager&		GetDatabaseManager( );

//IScriptManager*			GetScriptManagerPointer( );
//IScriptManager&			GetScriptManager();



#endif

_void					FreeAllFairyLandSingleton( );

};

#ifndef _MULTIGLOBAL_ENABLE

#define PS_INLINE inline
#include "FairyLandGlobalInline.h"

#endif