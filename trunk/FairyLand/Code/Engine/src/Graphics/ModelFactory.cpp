//============================================================================
// ModelFactory.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// ModelFactory Implementation
//----------------------------------------------------------------------------

ModelFactory::ModelFactory( )
{
	mModelGroup = 0x00000000;
}

ModelFactory::~ModelFactory( )
{
}

_bool ModelFactory::OnReleaseMesh( _void* buffer, _bool uselessonly, _bool deleteobject )
{
	Mesh* mesh = (Mesh*) buffer;

	if ( uselessonly && mesh->GetRefCount( ) > 0 )
		return _false;

	// Decrease reference count.
	if ( mesh->DecreaseRefCount( ) == 0 && deleteobject )
	{
		delete mesh;
		return _true;
	}

	return _false;
}

_bool ModelFactory::OnReleaseMeshAnimation( _void* buffer, _bool uselessonly, _bool deleteobject )
{
	MeshAnimation* animation = (MeshAnimation*) buffer;

	if ( uselessonly && animation->GetRefCount( ) > 0 )
		return _false;

	// Decrease reference count.
	if ( animation->DecreaseRefCount( ) == 0 && deleteobject )
	{
		delete animation;
		return _true;
	}

	return _false;
}

_bool ModelFactory::OnReleaseSkeleton( _void* buffer, _bool uselessonly, _bool deleteobject )
{
	Skeleton* skeleton = (Skeleton*) buffer;

	if ( uselessonly && skeleton->GetRefCount( ) > 0 )
		return _false;

	// Decrease reference count.
	if ( skeleton->DecreaseRefCount( ) == 0 && deleteobject )
	{
		delete skeleton;
		return _true;
	}

	return _false;
}

_bool ModelFactory::OnReleaseSkeletonAnimation( _void* buffer, _bool uselessonly, _bool deleteobject )
{
	SkeletonAnimation* animation = (SkeletonAnimation*) buffer;

	if ( uselessonly && animation->GetRefCount( ) > 0 )
		return _false;

	// Decrease reference count.
	if ( animation->DecreaseRefCount( ) == 0 && deleteobject )
	{
		delete animation;
		return _true;
	}

	return _false;
}

IMesh* ModelFactory::CreateMesh( StringPtr filename )
{
	return CreateMesh( _null, filename );
}

IMesh* ModelFactory::CreateMesh( ISkeleton* skeleton, StringPtr filename )
{
	// Search in resource cache first.
	if ( GetResourceCachePointer( ) != _null )
	{
		IMesh* cachedmesh = (IMesh*) GetResourceCache( ).ObtainCache( filename, OnReleaseMesh );

		// Cache processmethod.
		if ( cachedmesh != _null )
		{
			// Clone the cached mesh.
			IMesh* newmesh = CloneMesh( cachedmesh, skeleton == _null, _false, skeleton );

			if ( newmesh != _null )
				return newmesh;
		}
	}

	BinFile modelfile;

	// Load mesh from resource manager.
	if ( GetResourceManagerPointer( ) != _null )
	{
		if ( GetResourceManagerPointer( )->LoadResource( filename, modelfile ) == _false )
			return _null;
	}
	else
	{
		if ( modelfile.Open( filename ) == _false )
			return _null;
	}

	Mesh* mesh = new Mesh( modelfile.GetFileName( ) );

	if ( mesh->LoadModel( modelfile, ++ mModelGroup ) == _false )
	{
		delete mesh;
		return _null;
	}

	// Attach to skeleton.
	mesh->AttachSkeleton( skeleton );

	// Put the new created resource into cache.
	if ( GetResourceCachePointer( ) != _null )
		GetResourceCachePointer( )->CreateCache( mesh->GetName( ), mesh, OnReleaseMesh );


	return mesh;
}

IMesh* ModelFactory::CloneMesh( IMesh* mesh, _bool reference, _bool cloneall, ISkeleton* skeleton )
{
	if ( mesh == _null )
		return _null;

	if ( reference || ( (Mesh*) mesh )->GetRefCount( ) == 0 )
	{
		// Increase reference count.
		( (Mesh*) mesh )->IncreaseRefCount( );

		// Attach to new skeleton.
		if ( skeleton != _null )
			( (Mesh*) mesh )->AttachSkeleton( skeleton );

		return mesh;
	}
	else
	{
		IMesh* clonedmesh = ( (Mesh*) mesh )->Clone( ++ mModelGroup, cloneall );

		if ( clonedmesh != _null )
		{
			// Attach to new skeleton.
			if ( skeleton != _null )
				( (Mesh*) clonedmesh )->AttachSkeleton( skeleton );
		}

		return clonedmesh;
	}
}

_bool ModelFactory::CombineMesh( IMesh* mesh, StringPtr filename )
{
	if ( mesh == _null )
		return _false;

	IMesh* newmesh = CreateMesh( filename );

	if ( newmesh == _null )
		return _false;

	return CombineMesh( mesh, newmesh );
}

_bool ModelFactory::CombineMesh( IMesh* mesh, IMesh*& combinedmesh )
{
	if ( mesh == _null || combinedmesh == _null )
		return _false;

	if ( combinedmesh == mesh )
		return _true;

	( (Mesh*) mesh )->CombineModel( (Mesh*) combinedmesh );
	( (Mesh*) mesh )->CombineMarker( (Mesh*) combinedmesh );

	ReleaseMesh( combinedmesh );

	return _true;
}

_void ModelFactory::ReleaseMesh( IMesh*& mesh )
{
	if ( mesh == _null )
		return;

	if ( ( (Mesh*) mesh )->DecreaseRefCount( ) == 0 )
	{
		if ( GetResourceCachePointer( ) == _null || GetResourceCache( ).ReleaseCache( mesh->GetName( ), mesh ) == _false )
			OnReleaseMesh( mesh );
	}

	mesh = _null;
}

IVertexAnimation* ModelFactory::CreateVertexAnimation( IMesh* mesh, _dword format, _float duration )
{
	VertexAnimation* animation = new VertexAnimation( L"", format, duration );

	// Attach to the mesh.
	animation->AttachMesh( mesh );

	return animation;
}

IVertexAnimation* ModelFactory::CloneVertexAnimation( IVertexAnimation* animation, _bool reference, IMesh* mesh )
{
	if ( animation == _null )
		return _null;

	// Increase reference count.
	( (VertexAnimation*) animation )->IncreaseRefCount( );

	return animation;
}

_void ModelFactory::ReleaseVertexAnimation( IVertexAnimation*& animation )
{
	if ( animation == _null )
		return;

	if ( ( (VertexAnimation*) animation )->DecreaseRefCount( ) == 0 )
		delete (VertexAnimation*) animation;

	animation = _null;
}

IMeshAnimation* ModelFactory::CreateMeshAnimation( IMesh* mesh, StringPtr filename )
{
	// Search in resource cache first.
	if ( GetResourceCachePointer( ) != _null )
	{
		IMeshAnimation* cachedmeshanimation = (IMeshAnimation*) GetResourceCache( ).ObtainCache( filename, OnReleaseMeshAnimation );

		// Cache processmethod.
		if ( cachedmeshanimation != _null )
		{
			// Clone the cached mesh.
			IMeshAnimation* newmeshanimation = CloneMeshAnimation( cachedmeshanimation, _false, mesh );

			if ( newmeshanimation != _null )
				return newmeshanimation;
		}
	}

	BinFile animationfile;

	if ( GetResourceManagerPointer( ) != _null )
	{
		if ( GetResourceManagerPointer( )->LoadResource( filename, animationfile ) == _false )
			return _null;
	}
	else
	{
		if ( animationfile.Open( filename ) == _false )
			return _null;
	}

	MeshAnimation* animation = new MeshAnimation( animationfile.GetFileName( ) );

	if ( animation->LoadAnimation( mesh, animationfile ) == _false )
	{
		delete animation;
		return _null;
	}

	animation->AttachMesh( mesh );

	// Insert mesh animation into cache.
	if ( GetResourceCachePointer( ) != _null )
		GetResourceCachePointer( )->CreateCache( animation->GetName( ), animation, OnReleaseMeshAnimation );

	return animation;
}

IMeshAnimation* ModelFactory::CloneMeshAnimation( IMeshAnimation* animation, _bool reference, IMesh* mesh )
{
	if ( animation == _null )
		return _null;

	if ( reference || ( (MeshAnimation*) animation )->GetRefCount( ) == 0 )
	{
		// Increase reference count.
		( (MeshAnimation*) animation )->IncreaseRefCount( );

		if ( mesh != _null )
			( (MeshAnimation*) animation )->AttachMesh( mesh );

		return animation;
	}
	else
	{
		IMeshAnimation* clonedanimation = ( (MeshAnimation*) animation )->Clone( );

		if ( clonedanimation != _null )
		{
			if ( mesh != _null )
				( (MeshAnimation*) clonedanimation )->AttachMesh( mesh );
		}

		return clonedanimation;
	}
}

_void ModelFactory::ReleaseMeshAnimation( IMeshAnimation*& animation )
{
	if ( animation == _null )
		return;

	if ( ( (MeshAnimation*) animation )->DecreaseRefCount( ) == 0 )
	{
		if ( GetResourceCachePointer( ) == _null || GetResourceCache( ).ReleaseCache( animation->GetName( ), animation ) == _false )
			OnReleaseMeshAnimation( animation );
	}

	animation = _null;
}

ISkeleton* ModelFactory::CreateSkeleton( StringPtr filename, _bool onlyroot )
{
	// Search in resource cache first.
	if ( GetResourceCachePointer( ) != _null )
	{
		ISkeleton* cachedskeleton = (ISkeleton*) GetResourceCache( ).ObtainCache( filename, OnReleaseSkeleton );

		// Cache processmethod.
		if ( cachedskeleton != _null )
		{
			// Clone the cached mesh.
			ISkeleton* newskeleton = CloneSkeleton( cachedskeleton, _false );

			if ( newskeleton != _null )
				return newskeleton;
		}
	}

	BinFile skeletonfile;

	if ( GetResourceManagerPointer( ) != _null )
	{
		if ( GetResourceManagerPointer( )->LoadResource( filename, skeletonfile ) == _false )
			return _null;
	}
	else
	{
		if ( skeletonfile.Open( filename ) == _false )
			return _null;
	}

	Skeleton* skeleton = new Skeleton( skeletonfile.GetFileName( ) );

	if ( skeleton->LoadSkeleton( skeletonfile, onlyroot ) == _false )
	{
		delete skeleton;
		return _null;
	}

	// Insert skeleton into cache.
	if ( GetResourceCachePointer( ) != _null )
		GetResourceCachePointer( )->CreateCache( skeleton->GetName( ), skeleton, OnReleaseSkeleton );

	return skeleton;
}

ISkeleton* ModelFactory::CloneSkeleton( ISkeleton* skeleton, _bool reference )
{
	if ( skeleton == _null )
		return _null;

	if ( reference || ( (Skeleton*) skeleton )->GetRefCount( ) == 0 )
	{
		// Increase reference count.
		( (Skeleton*) skeleton )->IncreaseRefCount( );

		return skeleton;
	}
	else
	{
		ISkeleton* clonedskeleton = ( (Skeleton*) skeleton )->Clone( );

		return clonedskeleton;
	}
}

_void ModelFactory::ReleaseSkeleton( ISkeleton*& skeleton )
{
	if ( skeleton == _null )
		return;

	if ( ( (Skeleton*) skeleton )->DecreaseRefCount( ) == 0 )
	{
		if ( GetResourceCachePointer( ) == _null || GetResourceCache( ).ReleaseCache( skeleton->GetName( ), skeleton ) == _false )
			OnReleaseSkeleton( skeleton );
	}

	skeleton = _null;
}

ISkeletonAnimation* ModelFactory::CreateSkeletonAnimation( ISkeleton* skeleton )
{
	SkeletonAnimation* animation = new SkeletonAnimation( L"", _true );

	animation->AttachSkeleton( skeleton );

	return animation;
}

ISkeletonAnimation* ModelFactory::CreateSkeletonAnimation( ISkeleton* skeleton, StringPtr filename, _bool onlyroot )
{
	// Search in resource cache first.
	if ( GetResourceCachePointer( ) != _null )
	{
		ISkeletonAnimation* cachedskeletonanimation = (ISkeletonAnimation*) GetResourceCache( ).ObtainCache( filename, OnReleaseSkeletonAnimation );

		// Cache processmethod.
		if ( cachedskeletonanimation != _null )
		{
			// Clone the cached mesh.
			ISkeletonAnimation* newskeletonanimation = CloneSkeletonAnimation( cachedskeletonanimation, _false, skeleton );

			if ( newskeletonanimation != _null )
				return newskeletonanimation;
		}
	}

	BinFile animationfile;

	if ( GetResourceManagerPointer( ) != _null )
	{
		if ( GetResourceManagerPointer( )->LoadResource( filename, animationfile ) == _false )
			return _null;
	}
	else
	{
		if ( animationfile.Open( filename ) == _false )
			return _null;
	}

	SkeletonAnimation* animation = new SkeletonAnimation( animationfile.GetFileName( ), _true );

	if ( animation->LoadAnimation( skeleton, animationfile, onlyroot ) == _false )
	{
		delete animation;
		return _null;
	}

	animation->AttachSkeleton( skeleton );

	// Insert skeleton animation into cache.
	if ( GetResourceCachePointer( ) != _null )
		GetResourceCachePointer( )->CreateCache( animation->GetName( ), animation, OnReleaseSkeletonAnimation );

	return animation;
}

ISkeletonAnimation* ModelFactory::CloneSkeletonAnimation( ISkeletonAnimation* animation, _bool reference, ISkeleton* skeleton )
{
	if ( animation == _null )
		return _null;

	if ( reference || ( (SkeletonAnimation*) animation )->GetRefCount( ) == 0 )
	{
		// Increase reference count.
		( (SkeletonAnimation*) animation )->IncreaseRefCount( );

		if ( skeleton != _null )
			( (SkeletonAnimation*) animation )->AttachSkeleton( skeleton );

		return animation;
	}
	else
	{	
		ISkeletonAnimation* clonedanimation = ( (SkeletonAnimation*) animation )->Clone( );

		if ( clonedanimation != _null )
		{
			if ( skeleton != _null )
				( (SkeletonAnimation*) clonedanimation )->AttachSkeleton( skeleton );
		}

		return clonedanimation;
	}
}

_bool ModelFactory::CombineSkeletonAnimation( ISkeletonAnimation* animation, StringPtr filename, _dword times )
{
	if ( animation == _null )
		return _false;

	ISkeletonAnimation* newanimation = CreateSkeletonAnimation( animation->GetAttachedSkeleton( ), filename, _false );

	if ( newanimation == _null )
		return _false;

	return CombineSkeletonAnimation( animation, newanimation, times );
}

_bool ModelFactory::CombineSkeletonAnimation( ISkeletonAnimation* animation, ISkeletonAnimation*& combinedanimation, _dword times )
{
	if ( animation == _null || combinedanimation == _null || combinedanimation == animation )
		return _false;

	for ( _dword i = 0; i < times; i ++ )
		( (SkeletonAnimation*) animation )->AppendAnimation( (SkeletonAnimation*) combinedanimation );

	ReleaseSkeletonAnimation( combinedanimation );

	return _true;
}

_void ModelFactory::ReleaseSkeletonAnimation( ISkeletonAnimation*& animation )
{
	if ( animation == _null )
		return;

	if ( ( (SkeletonAnimation*) animation )->DecreaseRefCount( ) == 0 )
	{
		if ( GetResourceCachePointer( ) == _null || GetResourceCache( ).ReleaseCache( animation->GetName( ), animation ) == _false )
			OnReleaseSkeletonAnimation( animation );
	}

	animation = _null;
}