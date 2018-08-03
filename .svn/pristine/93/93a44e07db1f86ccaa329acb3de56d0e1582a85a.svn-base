//============================================================================
// FairyLandStorage.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// CreateStorage Implementation
//----------------------------------------------------------------------------

IResourceCache* CreateResourceCache( )
{
	IResourceCache* resourcecache = GetResourceCachePointer( );

	if ( resourcecache != _null )
		return resourcecache;

	resourcecache = new ResourceCache;
	GetFairyLandGlobal( ).mResourceCache = resourcecache;

	PS_TRACE( L"[KL] ResourceCache Created\r\n" )

	return resourcecache;
}

IResourceManager* CreateResourceManager( )
{
	IResourceManager* resourcemanager = GetResourceManagerPointer( );

	if ( resourcemanager != _null )
		return resourcemanager;

	resourcemanager = new ResourceManager;
	GetFairyLandGlobal( ).mResourceManager = resourcemanager;

	PS_TRACE( L"[KL] ResourceManager Created\r\n" )

	return resourcemanager;
}

IResourceDownloader* CreateResourceDownloader( IFileDownloader* downloader )
{
	IResourceDownloader* resourcedownloader = GetResourceDownloaderPointer( );

	if ( resourcedownloader != _null )
		return resourcedownloader;

	resourcedownloader = new ResourceDownloader( downloader );
	GetFairyLandGlobal( ).mResourceDownloader = resourcedownloader;

	PS_TRACE( L"[KL] ResourceDownloader Created\r\n" )

	return resourcedownloader;
}

IResourceReplacer* CreateResourceReplacer( )
{
	IResourceReplacer* resourcereplacer = GetResourceReplacerPointer( );

	if ( resourcereplacer != _null )
		return resourcereplacer;

	resourcereplacer = new ResourceReplacer( );
	GetFairyLandGlobal( ).mResourceReplacer = resourcereplacer;

	PS_TRACE( L"[KL] ResourceReplacer Created\r\n" )

	return resourcereplacer;
}