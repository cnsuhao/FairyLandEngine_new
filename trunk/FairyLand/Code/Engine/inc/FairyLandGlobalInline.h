//============================================================================
// FairyLandGlobalInline.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

PS_INLINE FairyLandGlobal& GetFairyLandGlobal( )
{
	return GetGlobalHelper( );
}

PS_INLINE IResourceManager* GetResourceManagerPointer( )
{
	return GetGlobalHelper( ).mResourceManager;
}

PS_INLINE IResourceManager& GetResourceManager( )
{
	return *( GetGlobalHelper( ).mResourceManager );
}

PS_INLINE IResourceCache* GetResourceCachePointer( )
{
	return GetGlobalHelper( ).mResourceCache;
}

PS_INLINE IResourceCache& GetResourceCache( )
{
	return *( GetGlobalHelper( ).mResourceCache );
}

PS_INLINE IResourceDownloader* GetResourceDownloaderPointer( )
{
	return GetGlobalHelper( ).mResourceDownloader;
}

PS_INLINE IResourceDownloader& GetResourceDownloader( )
{
	return *( GetGlobalHelper( ).mResourceDownloader );
}

PS_INLINE IResourceReplacer* GetResourceReplacerPointer( )
{
	return GetGlobalHelper( ).mResourceReplacer;
}

PS_INLINE IResourceReplacer& GetResourceReplacer( )
{
	return *( GetGlobalHelper( ).mResourceReplacer );
}

PS_INLINE IGraphicsManager* GetGraphicsManagerPointer( )
{
	return GetGlobalHelper( ).mGraphicsManager;
}

PS_INLINE IGraphicsManager& GetGraphicsManager( )
{
	return *( GetGlobalHelper( ).mGraphicsManager );
}
PS_INLINE IRenderDevice* GetRenderDevicePointer( )
{
	return  GetGlobalHelper( ).mRenderDevice;
}
PS_INLINE IRenderDevice& GetRenderDevice( )
{
	return *( GetGlobalHelper( ).mRenderDevice );
}

PS_INLINE IGeometryFactory* GetGeometryFactoryPointer( )
{
	return GetGlobalHelper( ).mGeometryFactory ;
}
PS_INLINE IGeometryFactory& GetGeometryFactory( )
{
	return *( GetGlobalHelper( ).mGeometryFactory );
}

PS_INLINE ITextureFactory* GetTextureFactoryPointer( )
{
	return GetGlobalHelper( ).mTextureFactory;
}
PS_INLINE ITextureFactory& GetTextureFactory( )
{
	return *( GetGlobalHelper( ).mTextureFactory );
}

PS_INLINE ITechniqueFactory* GetTechniqueFactoryPointer( )
{
	return GetGlobalHelper( ).mTechniqueFactory ;
}

PS_INLINE ITechniqueFactory& GetTechniqueFactory( )
{
	return *( GetGlobalHelper( ).mTechniqueFactory );
}

PS_INLINE IModelFactory* GetModelFactoryPointer( )
{
	return  GetGlobalHelper( ).mModelFactory ;
}

PS_INLINE IModelFactory& GetModelFactory( )
{
	return *( GetGlobalHelper( ).mModelFactory );
}

PS_INLINE IParticleFactory* GetParticleFactoryPointer( )
{
	return  GetGlobalHelper( ).mParticleFactory ;
}

PS_INLINE IParticleFactory& GetParticleFactory( )
{
	return *( GetGlobalHelper( ).mParticleFactory );
}

PS_INLINE IPhysicsFactory& GetPhysicsFactory( )
{
	return *( GetGlobalHelper( ).mPhysicsFactory );
}

PS_INLINE ISceneFactory& GetSceneFactory( )
{
	return *( GetGlobalHelper( ).mSceneFactory );
}

PS_INLINE IRenderConfig& GetRenderConfig( )
{
	return *( GetGlobalHelper( ).mRenderConfig );
}

PS_INLINE IRenderStatistics& GetRenderStatistics( )
{
	return *( GetGlobalHelper( ).mRenderStatistics );
}

PS_INLINE ISoundDevice* GetSoundDevicePointer( )
{
	return GetGlobalHelper( ).mSoundDevice;
}

PS_INLINE ISoundDevice& GetSoundDevice( )
{
	return *( GetGlobalHelper( ).mSoundDevice );
}

PS_INLINE INetworkManager* GetNetworkManagerPointer( )
{
	return GetGlobalHelper( ).mNetworkManager;
}

PS_INLINE INetworkManager& GetNetworkManager( )
{
	return *( GetGlobalHelper( ).mNetworkManager );
}

PS_INLINE IDatabaseManager* GetDatabaseManagerPointer( )
{
	return GetGlobalHelper( ).mDatabaseManager;
}

PS_INLINE IDatabaseManager& GetDatabaseManager( )
{
	return *( GetGlobalHelper( ).mDatabaseManager );
}

PS_INLINE IFairyLandWorker& GetFairyLandWorker( )
{
	return *(GetGlobalHelper( ).mFairyLandWorker);
}
PS_INLINE IFairyLandWorker* GetFairyLandWorkerPointer( )
{
	return GetGlobalHelper( ).mFairyLandWorker;
}
/*

PS_INLINE IScriptManager* GetScriptManagerPointer( )
{
	return GetGlobalHelper( ).mScriptManager;
}

PS_INLINE IScriptManager& GetScriptManager( )
{
	return *( GetGlobalHelper( ).mScriptManager );
}

*/
};