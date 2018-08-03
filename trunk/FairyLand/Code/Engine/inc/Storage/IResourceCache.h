//============================================================================
// IResourceCache.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IResourceCache
//----------------------------------------------------------------------------

class IResourceCache
{
public:
	typedef _bool (*OnReleaseCache)( _void* buffer, _bool uselessonly, _bool deleteobject );

public:
	virtual _void	Release( )																						= 0;

	virtual _void	Update( _dword elapse )																			= 0;

	virtual _void*	ObtainCache( StringPtr resname, OnReleaseCache releasefunc, _bool deepsearch = _false )			= 0;
	virtual _void	CreateCache( StringPtr resname, _void* buffer, OnReleaseCache releasefunc, _dword size = 0 )	= 0;
	virtual _bool	ReleaseCache( StringPtr resname, _void* buffer = _null )										= 0;
	virtual _void	ClearCache( _dword label = 0, _bool clearusingobject = _false )									= 0;

	virtual _bool	PreloadTexture( StringPtr filename, StringPtr preloadmethod = L"" )								= 0;
	virtual _bool	PreloadMesh( StringPtr filename, StringPtr preloadmethod = L"" )								= 0;
	virtual _bool	PreloadMeshAnimation( StringPtr filename )														= 0;
	virtual _bool	PreloadSkeleton( StringPtr filename, _bool onlyroot = _false )									= 0;
	virtual _bool	PreloadSkeletonAnimation( StringPtr filename, _bool onlyroot = _false )							= 0;
	virtual _bool	PreloadBoneShadow( StringPtr filename )															= 0;
	virtual _bool	PreloadParticleSystem( StringPtr filename )														= 0;

	virtual _bool	PreloadResource( StringPtr resname )															= 0;
	virtual _bool	InsertPreloadedResource( StringPtr resname, MemFile& memfile )									= 0;
	virtual _bool	ObtainPreloadedResource( StringPtr resname, MemFile& memfile )									= 0;
	virtual _void	ClearPreloadedResource( )																		= 0;

	virtual _void	UpdateCacheLabel( _dword label )																= 0;
	virtual _void	SetCacheLabel( _dword label )																	= 0;
	virtual _dword	GetCacheLabel( ) const																			= 0;

	virtual _void	SetCacheTimeout( _dword timeout )																= 0;
	virtual _dword	GetCacheTimeout( ) const																		= 0;
	virtual _dword  GetDefaultCacheTimeout( ) const																	= 0;

	virtual _void	SetPreloadTimeout( _dword timeout )																= 0;
	virtual _dword	GetPreloadTimeout( ) const																		= 0;
	virtual _dword  GetDefaultPreloadTimeout( ) const																= 0;

	virtual _dword	GetCachedObjectNumber( ) const																	= 0;
	virtual _dword	GetCachedMemorySize( ) const																	= 0;
	virtual _dword	GetPreloadedResourceNumber( ) const																= 0;
	virtual _dword	GetPreloadedMemorySize( ) const																	= 0;
	virtual _float	GetCacheHitPercentage( ) const																	= 0;
	virtual _float	GetPreloadHitPercentage( ) const																= 0;

	virtual _void	EnableCache( _bool enable )																		= 0;
	virtual _bool	IsCacheEnabled( ) const																			= 0;

	virtual _void	EnableStableResource( _bool enable )															= 0;
	virtual _bool	IsStableResourceEnabled( ) const																= 0;
};

};