//============================================================================
// ResourceCache.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ResourceCache
//----------------------------------------------------------------------------

class ResourceCache : public IResourceCache
{
private:
	enum
	{
		_DEFAULT_CACHE_TIMEOUT		= 0,
		_DEFAULT_PRELOAD_TIMEOUT	= 60 * 1000,
	};

	struct CachedObject
	{
		String			mName;
		_void*			mBuffer;
		_bool			mStable;
		_dword			mLabel;
		_dword			mSize;
		_dword			mTimeout;
		OnReleaseCache	mReleaseFunc;

		operator StringPtr( ) const
			{ return mName; }
	};

	struct PreloadedResource
	{
		String			mName;
		MemFile			mMemFile;
		_dword			mTimeout;

		operator StringPtr( ) const
			{ return mName; }
	};

	typedef Hash< CachedObject, StringPtr >			CacheHash;
	typedef Hash< PreloadedResource, StringPtr >	PreloadHash;

	CacheHash		mCacheHash;
	PreloadHash		mPreloadHash;

	_dword			mCacheTimeout;
	_dword			mCacheMemory;
	_dword			mPreloadTimeout;
	_dword			mPreloadMemory;
	_bool			mCacheEnable;
	_bool			mStableResourceEnable;
	_bool			mReleasingCache;

	_dword			mCacheLabel;
	_dword			mTotalCacheTimes;
	_dword			mHitCacheTimes;
	_dword			mTotalPreloadTimes;
	_dword			mHitPreloadTimes;

	_dword			mHappenCacheTime;
	_dword			mHappenCacheHit;

	_dword			mAccumulatedElapse;

public:
	ResourceCache( );
	~ResourceCache( );

	virtual _void	Release( );

	virtual _void	Update( _dword elapse );

	virtual _void*	ObtainCache( StringPtr resname, OnReleaseCache releasefunc, _bool deepsearch );
	virtual _void	CreateCache( StringPtr resname, _void* buffer, OnReleaseCache releasefunc, _dword size );
	virtual _bool	ReleaseCache( StringPtr resname, _void* buffer );
	virtual _void	ClearCache( _dword label, _bool clearusingobject );

	virtual _bool	PreloadTexture( StringPtr filename, StringPtr preloadmethod );
	virtual _bool	PreloadMesh( StringPtr filename, StringPtr preloadmethod );
	virtual _bool	PreloadMeshAnimation( StringPtr filename );
	virtual _bool	PreloadSkeleton( StringPtr filename, _bool onlyroot );
	virtual _bool	PreloadSkeletonAnimation( StringPtr filename, _bool onlyroot );
	virtual _bool	PreloadBoneShadow( StringPtr filename );
	virtual _bool	PreloadParticleSystem( StringPtr filename );

	virtual _bool	PreloadResource( StringPtr resname );
	virtual _bool	InsertPreloadedResource( StringPtr resname, MemFile& memfile );
	virtual _bool	ObtainPreloadedResource( StringPtr resname, MemFile& memfile );
	virtual _void	ClearPreloadedResource( );

	virtual _void	UpdateCacheLabel( _dword label );
	virtual _void	SetCacheLabel( _dword label );
	virtual _dword	GetCacheLabel( ) const;

	virtual _void	SetCacheTimeout( _dword timeout );
	virtual _dword	GetCacheTimeout( ) const;
	virtual _dword  GetDefaultCacheTimeout( ) const;

	virtual _void	SetPreloadTimeout( _dword timeout );
	virtual _dword	GetPreloadTimeout( ) const;
	virtual _dword  GetDefaultPreloadTimeout( ) const;

	virtual _dword	GetCachedObjectNumber( ) const;
	virtual _dword	GetCachedMemorySize( ) const;
	virtual _dword	GetPreloadedResourceNumber( ) const;
	virtual _dword	GetPreloadedMemorySize( ) const;
	virtual _float	GetCacheHitPercentage( ) const;
	virtual _float	GetPreloadHitPercentage( ) const;

	virtual _void	EnableCache( _bool enable );
	virtual _bool	IsCacheEnabled( ) const;

	virtual _void	EnableStableResource( _bool enable );
	virtual _bool	IsStableResourceEnabled( ) const;
};

};