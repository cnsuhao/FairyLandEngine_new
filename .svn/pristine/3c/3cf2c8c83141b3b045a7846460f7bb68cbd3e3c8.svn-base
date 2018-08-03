//============================================================================
// ResourceCache.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// ResourceCache Implementation
//----------------------------------------------------------------------------

ResourceCache::ResourceCache( ) : mCacheHash( 36027 ), mPreloadHash( 36027 )
{
	mCacheTimeout			= _DEFAULT_CACHE_TIMEOUT;
	mCacheMemory			= 0;
	mPreloadTimeout			= _DEFAULT_PRELOAD_TIMEOUT;
	mPreloadMemory			= 0;
	mCacheEnable			= _true;
	mStableResourceEnable	= _false;
	mReleasingCache			= _false;

	mCacheLabel				= 0;
	mTotalCacheTimes		= 0;
	mHitCacheTimes			= 0;
	mTotalPreloadTimes		= 0;
	mHitPreloadTimes		= 0;

	mHappenCacheTime		= 0;
	mHappenCacheHit			= 0;

	mAccumulatedElapse		= 0;
}

ResourceCache::~ResourceCache( )
{
	ClearCache( 0, _false );
	ClearPreloadedResource( );

	GetFairyLandGlobal( ).mResourceCache = _null;
}

_void ResourceCache::Release( )
{
	delete this;
}

_void ResourceCache::Update( _dword elapse )
{
	mAccumulatedElapse += elapse;

	// Check cache every 1 second.
	if ( mAccumulatedElapse < 1000 )
		return;

	elapse = mAccumulatedElapse;
	mAccumulatedElapse = 0;

	ResourceManager& resourcemanager = (ResourceManager&) GetResourceManager( );

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( resourcemanager.GetResourceLock( ) );

	// Process all cached object.
	for ( CacheHash::Iterator it = mCacheHash.GetTailIterator( ); it.Valid( ); it -- )
	{
		CachedObject& cachedobject = it;

		// Skip the using object.
		if ( cachedobject.mTimeout == -1 )
			continue;

		if ( cachedobject.mBuffer != _null )
		{
			// The cached object is not timeout yet.
			if ( cachedobject.mTimeout > elapse )
			{
				cachedobject.mTimeout -= elapse;

				continue;
			}

			if ( cachedobject.mReleaseFunc != _null )
				(*cachedobject.mReleaseFunc)( cachedobject.mBuffer, _true, _true );
		}

		mCacheMemory -= cachedobject.mSize;

		// Remove the timeout object.
		mCacheHash.Remove( it );
	}

	// Process all preloaded resource.
	for ( PreloadHash::Iterator it = mPreloadHash.GetTailIterator( ); it.Valid( ); it -- )
	{
		PreloadedResource& preloadedresource = it;

		// Skip the using object.
		if ( preloadedresource.mTimeout == -1 )
			continue;

		// The preloaded object is not timeout yet.
		if ( preloadedresource.mTimeout > elapse )
		{
			preloadedresource.mTimeout -= elapse;

			continue;
		}

		preloadedresource.mMemFile.EnableAutoDeleteBuffer( _true );
		preloadedresource.mMemFile.Close( );

		mPreloadMemory -= preloadedresource.mMemFile.GetLength( );

		// Remove the timeout object.
		mPreloadHash.Remove( it );
	}
}

_void* ResourceCache::ObtainCache( StringPtr resname, OnReleaseCache releasefunc, _bool deepsearch )
{
	if ( resname == L"" )
		return _null;

	ResourceManager& resourcemanager = (ResourceManager&) GetResourceManager( );

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( resourcemanager.GetResourceLock( ) );

	if ( mCacheEnable == _false )
		return _null;

	mTotalCacheTimes ++;
	mHappenCacheTime ++;

	String lowercasedname  = resname;
	lowercasedname.Lowercase( );
	lowercasedname.ReplaceAll( '/', '\\' );

	// Search cached object in hash.
	CachedObject* cachedobject = mCacheHash.Index( lowercasedname );

	// Not found cached object.
	if ( cachedobject == _null )
	{
		if ( deepsearch == _false )
			return _null;

		// Only deep search when resource has no path.
		if ( resname.SearchL2R( '\\' ) == -1 && resname.SearchL2R( '/' ) == -1 )
		{
			_char filename[1024];

			// Try to deep search the object using search path, from back to front.
			for ( _dword i = 0; i < resourcemanager.GetSearchPathNumber( ); i ++ )
			{
				StringPtr searchpath = resourcemanager.GetSearchPath( resourcemanager.GetSearchPathNumber( ) - i - 1 );

				// Build file name by add search path in front of resource name.
				StringFormatter::FormatBuffer( filename, 1024, L"%s\\%s", (const _char*) searchpath, (const _char*) lowercasedname );

				// Search using full name.
				cachedobject = mCacheHash.Index( filename );

				// Found the object.
				if ( cachedobject != _null )
					break;
			}
		}

		// Still not found.
		if ( cachedobject == _null )
			return _null;
	}

	// Release function not match.
	if ( releasefunc != _null && cachedobject->mReleaseFunc != releasefunc )
		return _null;

	// PS_TRACE1( L"[RM] ObtainCache %s\r\n", lowercasedname )

	mHitCacheTimes ++;
	mHappenCacheHit ++;

	// Set using flag.
	cachedobject->mTimeout = -1;

	return cachedobject->mBuffer;
}

_void ResourceCache::CreateCache( StringPtr resname, _void* buffer, OnReleaseCache releasefunc, _dword size )
{
	if ( resname == L"" )
		return;

	ResourceManager& resourcemanager = (ResourceManager&) GetResourceManager( );

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( resourcemanager.GetResourceLock( ) );

	if ( mCacheEnable == _false )
		return;

	String lowercasedname  = resname;
	lowercasedname.Lowercase( );
	lowercasedname.ReplaceAll( '/', '\\' );

	// Search cached object in hash.
	CachedObject* cachedobject = mCacheHash.Index( lowercasedname );

	// Not found cached object.
	if ( cachedobject == _null )
	{
		mCacheMemory += size;

		CachedObject newcachedobject;
		newcachedobject.mName			= lowercasedname;
		newcachedobject.mBuffer			= buffer;
		newcachedobject.mStable			= mStableResourceEnable;
		newcachedobject.mLabel			= mCacheLabel;
		newcachedobject.mSize			= size;
		newcachedobject.mTimeout		= -1;
		newcachedobject.mReleaseFunc	= releasefunc;

		// PS_TRACE1( L"[RM] CreateCache %s\r\n", lowercasedname )

		mCacheHash.Insert( newcachedobject );
	}
	// Update the old cached object.
	else
	{
		mCacheMemory -= cachedobject->mSize;
		mCacheMemory += size;

		// Release the old cached object if it is useless.
		if ( cachedobject->mReleaseFunc != _null && cachedobject->mBuffer != _null )
			(*cachedobject->mReleaseFunc)( cachedobject->mBuffer, _true, _true );

		cachedobject->mBuffer		= buffer;
		cachedobject->mSize			= size;
		cachedobject->mTimeout		= -1;
		cachedobject->mReleaseFunc	= releasefunc;
	}
}

_bool ResourceCache::ReleaseCache( StringPtr resname, _void* buffer )
{
	if ( resname == L"" )
		return _false;

	ResourceManager& resourcemanager = (ResourceManager&) GetResourceManager( );

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( resourcemanager.GetResourceLock( ) );

	String lowercasedname  = resname;
	lowercasedname.Lowercase( );
	lowercasedname.ReplaceAll( '/', '\\' );

	// Search cached object in hash.
	CachedObject* cachedobject = mCacheHash.Index( lowercasedname );

	// Not found cached object.
	if ( cachedobject == _null )
		return _false;

	if ( buffer != _null && cachedobject->mBuffer != buffer )
		return _false;

	// Not ready to remove the cached object yet, we delay it for a while.
	if ( mCacheTimeout > 0 && mCacheEnable )
	{
		if ( cachedobject->mReleaseFunc != _null && cachedobject->mBuffer != _null )
			(*cachedobject->mReleaseFunc)( cachedobject->mBuffer, _false, _false );

		// Set timeout for this object.
		cachedobject->mTimeout = mCacheTimeout;
	}
	// Remove the cached object.
	else if ( cachedobject->mStable == _false )
	{
		if ( cachedobject->mReleaseFunc != _null && cachedobject->mBuffer != _null )
			(*cachedobject->mReleaseFunc)( cachedobject->mBuffer, _false, _true );

		mCacheMemory -= cachedobject->mSize;

		// We are in releasing mode, so dont remove object out of cache ( break cache link ).
		if ( mReleasingCache )
		{
			cachedobject->mBuffer	= _null;
			cachedobject->mSize		= 0;
		}
		// We can remove the cached object.
		else
		{
			// PS_TRACE1( L"[RM] ReleaseCache %s\r\n", lowercasedname )

			mCacheHash.Remove( lowercasedname );
		}
	}

	return _true;
}

_void ResourceCache::ClearCache( _dword label, _bool clearusingobject )
{
	PS_TRACE( L"[RM] ClearCache\r\n" )

	ResourceManager& resourcemanager = (ResourceManager&) GetResourceManager( );

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( resourcemanager.GetResourceLock( ) );

	_bool cacheenable = mCacheEnable;

	mCacheEnable	= _false;
	mReleasingCache	= _true;

	// Release all cached object.
	for ( CacheHash::Iterator it = mCacheHash.GetTailIterator( ); it.Valid( ); it -- )
	{
		CachedObject& cachedobject = it;

		if ( cachedobject.mBuffer == _null )
			continue;

		// Skip cached object with different label.
		if ( label != 0 && label != cachedobject.mLabel )
			continue;

		_bool removeobject = _true;

		// Call release function.
		if ( cachedobject.mReleaseFunc != _null && cachedobject.mBuffer != _null )
			removeobject = (*cachedobject.mReleaseFunc)( cachedobject.mBuffer, clearusingobject == _false, _true );

		// Release the cached object.
		if ( removeobject )
		{
			PS_TRACE1( L"[RM] Released %s\r\n", (const _char*) cachedobject.mName )

			mCacheMemory -= cachedobject.mSize;

			mCacheHash.Remove( it );
		}
	}

	mCacheEnable	= cacheenable;
	mReleasingCache	= _false;

	if ( clearusingobject == _false )
	{
		// Release all empty objects, these objects is released with in cachedobject.mReleaseFunc callback function.
		for ( CacheHash::Iterator it = mCacheHash.GetTailIterator( ); it.Valid( ); it -- )
		{
			CachedObject& cachedobject = it;

			if ( cachedobject.mBuffer != _null )
				continue;

			PS_TRACE1( L"[RM] Released %s\r\n", (const _char*) cachedobject.mName )

			mCacheMemory -= cachedobject.mSize;

			mCacheHash.Remove( it );
		}
	}
	else
	{
		mCacheMemory = 0;

		mCacheHash.Clear( );
	}
}

_bool ResourceCache::PreloadTexture( StringPtr filename, StringPtr preloadmethod )
{
	if ( mCacheEnable == _false )
		return _false;

//	if ( GetRenderDeviceSingleton( ) == _null )
//		return _false;
//
//	GetRenderDeviceSingleton( )->SetProcessResourceMethod( preloadmethod );
//	ITexture* resource = GetRenderDeviceSingleton( )->CreateTexture( filename );
//	GetRenderDeviceSingleton( )->ClearProcessResourceMethod( );
//
//	if ( resource == _null )
//		return _false;
//
//#ifdef _DEBUG
//	PS_TRACE1( L"[RM] PreloadTexture %s\r\n", resource->GetName( ) )
//#endif
//
//	_dword cachetimeoutback = mCacheTimeout;
//
//	mCacheTimeout = -1;
//	GetRenderDeviceSingleton( )->ReleaseTexture( resource, _true );
//	mCacheTimeout = cachetimeoutback;

	return _true;
}

_bool ResourceCache::PreloadMesh( StringPtr filename, StringPtr preloadmethod )
{
	if ( mCacheEnable == _false )
		return _false;

//	if ( GetRenderDeviceSingleton( ) == _null )
//		return _false;
//
//	GetRenderDeviceSingleton( )->SetProcessResourceMethod( preloadmethod );
//	IMesh* resource = GetRenderDeviceSingleton( )->CreateMesh( filename );
//	GetRenderDeviceSingleton( )->ClearProcessResourceMethod( );
//
//	if ( resource == _null )
//		return _false;
//
//#ifdef _DEBUG
//	PS_TRACE1( L"[RM] PreloadMesh %s\r\n", resource->GetName( ) )
//#endif
//
//	_dword cachetimeoutback = mCacheTimeout;
//
//	mCacheTimeout = -1;
//	GetRenderDeviceSingleton( )->ReleaseMesh( resource, _true );
//	mCacheTimeout = cachetimeoutback;

	return _true;
}

_bool ResourceCache::PreloadMeshAnimation( StringPtr filename )
{
	if ( mCacheEnable == _false )
		return _false;

//	if ( GetRenderDeviceSingleton( ) == _null )
//		return _false;
//
//	IMeshAnimation* resource = GetRenderDeviceSingleton( )->CreateMeshAnimation( _null, filename );
//	if ( resource == _null )
//		return _false;
//
//#ifdef _DEBUG
//	PS_TRACE1( L"[RM] PreloadMeshAnimation %s\r\n", resource->GetName( ) )
//#endif
//
//	_dword cachetimeoutback = mCacheTimeout;
//
//	mCacheTimeout = -1;
//	GetRenderDeviceSingleton( )->ReleaseMeshAnimation( resource, _true );
//	mCacheTimeout = cachetimeoutback;

	return _true;
}

_bool ResourceCache::PreloadSkeleton( StringPtr filename, _bool onlyroot )
{
	if ( mCacheEnable == _false )
		return _false;

//	if ( GetRenderDeviceSingleton( ) == _null )
//		return _false;
//
//	ISkeleton* resource = GetRenderDeviceSingleton( )->CreateSkeleton( filename, onlyroot );
//	if ( resource == _null )
//		return _false;
//
//#ifdef _DEBUG
//	PS_TRACE1( L"[RM] PreloadSkeleton %s\r\n", resource->GetName( ) )
//#endif
//
//	_dword cachetimeoutback = mCacheTimeout;
//
//	mCacheTimeout = -1;
//	GetRenderDeviceSingleton( )->ReleaseSkeleton( resource, _true );
//	mCacheTimeout = cachetimeoutback;

	return _true;
}

_bool ResourceCache::PreloadSkeletonAnimation( StringPtr filename, _bool onlyroot )
{
	if ( mCacheEnable == _false )
		return _false;

//	if ( GetRenderDeviceSingleton( ) == _null )
//		return _false;
//
//	ISkeletonAnimation* resource = GetRenderDeviceSingleton( )->CreateSkeletonAnimation( _null, filename, onlyroot );
//	if ( resource == _null )
//		return _false;
//
//#ifdef _DEBUG
//	PS_TRACE1( L"[RM] PreloadSkeletonAnimation %s\r\n", resource->GetName( ) )
//#endif
//
//	_dword cachetimeoutback = mCacheTimeout;
//
//	mCacheTimeout = -1;
//	GetRenderDeviceSingleton( )->ReleaseSkeletonAnimation( resource, _true );
//	mCacheTimeout = cachetimeoutback;

	return _true;
}

_bool ResourceCache::PreloadBoneShadow( StringPtr filename )
{
	if ( mCacheEnable == _false )
		return _false;

//	if ( GetRenderDeviceSingleton( ) == _null )
//		return _false;
//
//	IBoneShadow* resource = GetRenderDeviceSingleton( )->CreateBoneShadow( _null, filename );
//	if ( resource == _null )
//		return _false;
//
//#ifdef _DEBUG
//	PS_TRACE1( L"[RM] PreloadBoneShadow %s\r\n", resource->GetName( ) )
//#endif
//
//	_dword cachetimeoutback = mCacheTimeout;
//
//	mCacheTimeout = -1;
//	GetRenderDeviceSingleton( )->ReleaseBoneShadow( resource, _true );
//	mCacheTimeout = cachetimeoutback;

	return _true;
}

_bool ResourceCache::PreloadParticleSystem( StringPtr filename )
{
	if ( mCacheEnable == _false )
		return _false;

//	if ( GetRenderDeviceSingleton( ) == _null )
//		return _false;
//
//	IParticleSystem* resource = GetRenderDeviceSingleton( )->CreateParticleSystem( filename );
//	if ( resource == _null )
//		return _false;
//
//#ifdef _DEBUG
//	PS_TRACE1( L"[RM] PreloadParticleSystem %s\r\n", resource->GetName( ) )
//#endif
//
//	_dword cachetimeoutback = mCacheTimeout;
//
//	mCacheTimeout = -1;
//	GetRenderDeviceSingleton( )->ReleaseParticleSystem( resource, _true );
//	mCacheTimeout = cachetimeoutback;

	return _true;
}

_bool ResourceCache::PreloadResource( StringPtr resname )
{
	if ( resname == L"" )
		return _false;

	ResourceManager& resourcemanager = (ResourceManager&) GetResourceManager( );

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( resourcemanager.GetResourceLock( ) );

	String lowercasedname  = resname;
	lowercasedname.Lowercase( );
	lowercasedname.ReplaceAll( '/', '\\' );

	// The resource already in cache.
	if ( mCacheHash.Index( lowercasedname ) != _null )
		return _true;

	// Search preloaded resource in hash.
	PreloadedResource* preloadedresource = mPreloadHash.Index( lowercasedname );

	// The resource already preloaded.
	if ( preloadedresource != _null )
	{
		// Reset preload timeout.
		preloadedresource->mTimeout = mPreloadTimeout;

		return _true;
	}

	MemFile resmemfile;
	resmemfile.EnableAutoDeleteBuffer( _false );

	// Load resource from manager.
	if ( resourcemanager.LoadResource( lowercasedname, resmemfile, _false ) == _false )
		return _false;

	PreloadedResource newpreloadedresource;
	newpreloadedresource.mName		= lowercasedname;
	newpreloadedresource.mMemFile	= resmemfile;
	newpreloadedresource.mTimeout	= mPreloadTimeout;

	mPreloadMemory += newpreloadedresource.mMemFile.GetLength( );

	// Put the new preloaded resource into hash.
	mPreloadHash.Insert( newpreloadedresource );

	return _true;
}

_bool ResourceCache::InsertPreloadedResource( StringPtr resname, MemFile& memfile )
{
	if ( resname == L"" )
		return _false;

	ResourceManager& resourcemanager = (ResourceManager&) GetResourceManager( );

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( resourcemanager.GetResourceLock( ) );

	String lowercasedname  = resname;
	lowercasedname.Lowercase( );
	lowercasedname.ReplaceAll( '/', '\\' );

	// The resource already in cache.
	if ( mCacheHash.Index( lowercasedname ) != _null )
		return _true;

	// Search preloaded resource in hash.
	PreloadedResource* preloadedresource = mPreloadHash.Index( lowercasedname );

	// The resource already preloaded.
	if ( preloadedresource != _null )
	{
		// Reset preload timeout.
		preloadedresource->mTimeout = mPreloadTimeout;

		return _true;
	}

	MemFile resmemfile;
	resmemfile.EnableAutoDeleteBuffer( _false );

	// Copy preloaded resource.
	if ( resmemfile.Open( memfile ) == _false )
		return _false;

	PreloadedResource newpreloadedresource;
	newpreloadedresource.mName		= lowercasedname;
	newpreloadedresource.mMemFile	= resmemfile;
	newpreloadedresource.mTimeout	= mPreloadTimeout;

	mPreloadMemory += newpreloadedresource.mMemFile.GetLength( );

	// Put the new preloaded resource into hash.
	mPreloadHash.Insert( newpreloadedresource );

	return _true;
}

_bool ResourceCache::ObtainPreloadedResource( StringPtr resname, MemFile& memfile )
{
	if ( resname == L"" )
		return _false;

	ResourceManager& resourcemanager = (ResourceManager&) GetResourceManager( );

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( resourcemanager.GetResourceLock( ) );

	if ( mPreloadHash.Number( ) == 0 )
		return _false;

	mTotalPreloadTimes ++;

	String lowercasedname  = resname;
	lowercasedname.Lowercase( );
	lowercasedname.ReplaceAll( '/', '\\' );

	// Search preloaded resource in hash.
	PreloadedResource* preloadedresource = mPreloadHash.Index( lowercasedname );

	// Not found preloaded resource.
	if ( preloadedresource == _null )
		return _false;

	// Copy preloaded resource.
	if ( memfile.Open( preloadedresource->mMemFile.GetBuffer( ), preloadedresource->mMemFile.GetLength( ), _false ) == _false )
		return _false;

	mPreloadMemory -= memfile.GetLength( );

	// Now the buffer should let new memory file handle.
	memfile.EnableAutoDeleteBuffer( _true );
	preloadedresource->mMemFile.EnableAutoDeleteBuffer( _false );
	preloadedresource->mMemFile.Close( );

	mHitPreloadTimes ++;

	mPreloadHash.Remove( lowercasedname );

	return _true;
}

_void ResourceCache::ClearPreloadedResource( )
{
	ResourceManager& resourcemanager = (ResourceManager&) GetResourceManager( );

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( resourcemanager.GetResourceLock( ) );

	// Release all preloaded resource.
	for ( PreloadHash::Iterator it = mPreloadHash.GetTailIterator( ); it.Valid( ); it -- )
	{
		PreloadedResource& preloadedresource = it;

		preloadedresource.mMemFile.EnableAutoDeleteBuffer( _true );
		preloadedresource.mMemFile.Close( );
	}

	mPreloadHash.Clear( );
}

_void ResourceCache::UpdateCacheLabel( _dword label )
{
	ResourceManager& resourcemanager = (ResourceManager&) GetResourceManager( );

	// Public function, we need using lock to make thread safe.
	LockOwner lockowner( resourcemanager.GetResourceLock( ) );

	// Release all cached object.
	for ( CacheHash::Iterator it = mCacheHash.GetTailIterator( ); it.Valid( ); it -- )
	{
		CachedObject& cachedobject = it;

		cachedobject.mLabel = label;
	}
}

_void ResourceCache::SetCacheLabel( _dword label )
{
	mCacheLabel = label;
}

_dword ResourceCache::GetCacheLabel( ) const
{
	return mCacheLabel;
}

_void ResourceCache::SetCacheTimeout( _dword timeout )
{
	mCacheTimeout = timeout;
}

_dword ResourceCache::GetCacheTimeout( ) const
{
	return mCacheTimeout;
}

_dword ResourceCache::GetDefaultCacheTimeout( ) const
{
	return _DEFAULT_CACHE_TIMEOUT;
}

_void ResourceCache::SetPreloadTimeout( _dword timeout )
{
	mPreloadTimeout = timeout;
}

_dword ResourceCache::GetPreloadTimeout( ) const
{
	return mPreloadTimeout;
}

_dword ResourceCache::GetDefaultPreloadTimeout( ) const
{
	return _DEFAULT_PRELOAD_TIMEOUT;
}

_dword ResourceCache::GetCachedObjectNumber( ) const
{
	return mCacheHash.Number( );
}

_dword ResourceCache::GetCachedMemorySize( ) const
{
	return mCacheMemory;
}

_dword ResourceCache::GetPreloadedResourceNumber( ) const
{
	return mPreloadHash.Number( );
}

_dword ResourceCache::GetPreloadedMemorySize( ) const
{
	return mPreloadMemory;
}

_float ResourceCache::GetCacheHitPercentage( ) const
{
	if ( mTotalCacheTimes == 0 )
		return 0.0f;

	return (_float) mHitCacheTimes / (_float) mTotalCacheTimes;
}

_float ResourceCache::GetPreloadHitPercentage( ) const
{
	if ( mTotalPreloadTimes == 0 )
		return 0.0f;

	return (_float) mHitPreloadTimes / (_float) mTotalPreloadTimes;
}

_void ResourceCache::EnableCache( _bool enable )
{
	mCacheEnable = enable;
}

_bool ResourceCache::IsCacheEnabled( ) const
{
	return mCacheEnable;
}

_void ResourceCache::EnableStableResource( _bool enable )
{
	mStableResourceEnable = enable;
}

_bool ResourceCache::IsStableResourceEnabled( ) const
{
	return mStableResourceEnable;
}