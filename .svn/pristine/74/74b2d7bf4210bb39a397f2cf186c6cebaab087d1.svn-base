//============================================================================
// IModelFactory.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

namespace FL
{

//----------------------------------------------------------------------------
// IModelFactory
//----------------------------------------------------------------------------

class IModelFactory
{
public:
	virtual IMesh*					CreateMesh( StringPtr filename ) = 0;
	virtual IMesh*					CreateMesh( ISkeleton* skeleton, StringPtr filename ) = 0;
	virtual IMesh*					CloneMesh( IMesh* mesh, _bool reference = _true, _bool cloneall = _false, ISkeleton* skeleton = _null ) = 0;
	virtual _bool					CombineMesh( IMesh* mesh, StringPtr filename ) = 0;
	virtual _bool					CombineMesh( IMesh* mesh, IMesh*& combinedmesh ) = 0;
	virtual _void					ReleaseMesh( IMesh*& mesh ) = 0;

	virtual IVertexAnimation*		CreateVertexAnimation( IMesh* mesh, _dword format, _float duration ) = 0;
	virtual IVertexAnimation*		CloneVertexAnimation( IVertexAnimation* animation, _bool reference = _true, IMesh* mesh = _null ) = 0;
	virtual _void					ReleaseVertexAnimation( IVertexAnimation*& animation ) = 0;

	virtual IMeshAnimation*			CreateMeshAnimation( IMesh* mesh, StringPtr filename ) = 0;
	virtual IMeshAnimation*			CloneMeshAnimation( IMeshAnimation* animation, _bool reference = _true, IMesh* mesh = _null ) = 0;
	virtual _void					ReleaseMeshAnimation( IMeshAnimation*& animation ) = 0;

	virtual ISkeleton*				CreateSkeleton( StringPtr filename, _bool onlyroot = _false ) = 0;
	virtual ISkeleton*				CloneSkeleton( ISkeleton* skeleton, _bool reference = _true ) = 0;
	virtual _void					ReleaseSkeleton( ISkeleton*& skeleton ) = 0;

	virtual ISkeletonAnimation*		CreateSkeletonAnimation( ISkeleton* skeleton ) = 0;
	virtual ISkeletonAnimation*		CreateSkeletonAnimation( ISkeleton* skeleton, StringPtr filename, _bool onlyroot = _false ) = 0;
	virtual ISkeletonAnimation*		CloneSkeletonAnimation( ISkeletonAnimation* animation, _bool reference = _true, ISkeleton* skeleton = _null ) = 0;
	virtual _bool					CombineSkeletonAnimation( ISkeletonAnimation* animation, StringPtr filename, _dword times ) = 0;
	virtual _bool					CombineSkeletonAnimation( ISkeletonAnimation* animation, ISkeletonAnimation*& combinedanimation, _dword times ) = 0;
	virtual _void					ReleaseSkeletonAnimation( ISkeletonAnimation*& animation ) = 0;
};

};