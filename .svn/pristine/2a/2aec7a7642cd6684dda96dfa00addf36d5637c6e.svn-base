//============================================================================
// ModelFactory.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ModelFactory
//----------------------------------------------------------------------------

class ModelFactory : public IModelFactory
{
private:
	_dword		mModelGroup;

	static _bool OnReleaseMesh( _void* buffer, _bool uselessonly = _false, _bool deleteobject = _true );
	static _bool OnReleaseMeshAnimation( _void* buffer, _bool uselessonly = _false, _bool deleteobject = _true );
	static _bool OnReleaseVertexAnimation( _void* buffer, _bool uselessonly = _false, _bool deleteobject = _true );
	static _bool OnReleaseSkeleton( _void* buffer, _bool uselessonly = _false, _bool deleteobject = _true );
	static _bool OnReleaseSkeletonAnimation( _void* buffer, _bool uselessonly = _false, _bool deleteobject = _true );

public:
	ModelFactory( );
	~ModelFactory( );

	virtual IMesh*				CreateMesh( StringPtr filename );
	virtual IMesh*				CreateMesh( ISkeleton* skeleton, StringPtr filename );
	virtual IMesh*				CloneMesh( IMesh* mesh, _bool reference, _bool cloneall, ISkeleton* skeleton );
	virtual _bool				CombineMesh( IMesh* mesh, StringPtr filename );
	virtual _bool				CombineMesh( IMesh* mesh, IMesh*& combinedmesh );
	virtual _void				ReleaseMesh( IMesh*& mesh );

	virtual IMeshAnimation*		CreateMeshAnimation( IMesh* mesh, StringPtr filename );
	virtual IMeshAnimation*		CloneMeshAnimation( IMeshAnimation* animation, _bool reference, IMesh* mesh );
	virtual _void				ReleaseMeshAnimation( IMeshAnimation*& animation );

	virtual IVertexAnimation*	CreateVertexAnimation( IMesh* mesh, _dword format, _float duration );
	virtual IVertexAnimation*	CloneVertexAnimation( IVertexAnimation* animation, _bool reference, IMesh* mesh );
	virtual _void				ReleaseVertexAnimation( IVertexAnimation*& animation );

	virtual ISkeleton*			CreateSkeleton( StringPtr filename, _bool onlyroot );
	virtual ISkeleton*			CloneSkeleton( ISkeleton* skeleton, _bool reference );
	virtual _void				ReleaseSkeleton( ISkeleton*& skeleton );

	virtual ISkeletonAnimation*	CreateSkeletonAnimation( ISkeleton* skeleton );
	virtual ISkeletonAnimation*	CreateSkeletonAnimation( ISkeleton* skeleton, StringPtr filename, _bool onlyroot );
	virtual ISkeletonAnimation*	CloneSkeletonAnimation( ISkeletonAnimation* animation, _bool reference, ISkeleton* skeleton );
	virtual _bool				CombineSkeletonAnimation( ISkeletonAnimation* animation, StringPtr filename, _dword times );
	virtual _bool				CombineSkeletonAnimation( ISkeletonAnimation* animation, ISkeletonAnimation*& combinedanimation, _dword times );
	virtual _void				ReleaseSkeletonAnimation( ISkeletonAnimation*& animation );
};

};