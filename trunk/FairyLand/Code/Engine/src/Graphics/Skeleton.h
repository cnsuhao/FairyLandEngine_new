//============================================================================
// Skeleton.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Skeleton
//----------------------------------------------------------------------------

class Skeleton : public ISkeleton, public ReferencedObject
{
public:
	struct Bone
	{
		String		mBoneName;
		_dword		mParentID;

		Vector3		mRelativeTranslation;
		Quaternion	mRelativeRotation;
		Vector3		mAbsoluteTranslation;
		Quaternion	mAbsoluteRotation;
		Vector3		mInitTranslation;
		Quaternion	mInitRotation;

		_bool		mInitBaseTransform;
		Matrix4		mBaseTransform;
		Matrix4		mAlterTransform;

		Vector3		mLocalTranslationBoneSpace;
		Quaternion	mLocalRotationBoneSpace;
		Vector3		mLocalTranslation;
		Matrix3		mLocalRotation;

		ITagPoint*	mAttachedTagPoint;
	};

	struct SkinTransform
	{
		_dword mBoneNumber;

		Matrix3x4* mLocalTransform;

		_bool* mNeedAlter;
		Matrix3x4* mAlterTransform;

		SkinTransform( )
		{
			mBoneNumber = 0;
			mLocalTransform = _null;

			mNeedAlter = _null;
			mAlterTransform = _null;
		}

		~SkinTransform( )
		{
			Release();
		}

		_void Resize( _dword mBoneNumber )
		{
			Release( );

			this->mBoneNumber = mBoneNumber;

			mLocalTransform = ( Matrix3x4* ) SSEMemory::Allocate( mBoneNumber * sizeof( Matrix3x4 ) );
			mNeedAlter = ( _bool* ) SSEMemory::Allocate( mBoneNumber * sizeof( _bool ) );
			mAlterTransform = ( Matrix3x4* ) SSEMemory::Allocate( mBoneNumber * sizeof( Matrix3x4 ) );

			for( _dword i = 0; i < mBoneNumber; i ++ )
			{
				mNeedAlter[i] = _false;
			}
		}

		_void Release( )
		{
			mBoneNumber = 0;

			if( mLocalTransform != _null )
			{
				SSEMemory::Deallocate( mLocalTransform );
				mLocalTransform = _null;
			}

			if( mNeedAlter != _null )
			{
				SSEMemory::Deallocate( mNeedAlter );
				mNeedAlter = _null;
			}

			if( mAlterTransform != _null )
			{
				SSEMemory::Deallocate( mAlterTransform );
				mAlterTransform = _null;
			}
		}

		SkinTransform& operator = ( const SkinTransform& transform )
		{
			Release( );

			Resize( transform.mBoneNumber );

			for( _dword i = 0; i < transform.mBoneNumber; i ++ )
			{
				mNeedAlter[i] = transform.mNeedAlter[i];
				mLocalTransform[i] = transform.mLocalTransform[i];
				mAlterTransform[i] = transform.mAlterTransform[i];
			}

			return *this;
		}

		SkinTransform Clone( ) const
		{
			SkinTransform newone;
			newone.Resize( mBoneNumber );

			for( _dword i = 0; i < mBoneNumber; i ++ )
			{
				newone.mNeedAlter[i] = mNeedAlter[i];
				newone.mLocalTransform[i] = mLocalTransform[i];
				newone.mAlterTransform[i] = mAlterTransform[i];
			}

			return newone;
		}
	};

	struct Dummy
	{
		String		mDummyName;
		_dword		mParentID;

		Vector3		mRelativeTranslation;
		Quaternion	mRelativeRotation;
		Vector3		mAbsoluteTranslation;
		Quaternion	mAbsoluteRotation;

		ITagPoint*	mAttachedTagPoint;
	};

	struct Influence
	{
		_dword		mBoneID;
		Vector3		mTranslation;
		Quaternion	mRotation;
	};

	typedef Array< _dword >		  MapArray;
	typedef Array< Bone >		  BoneArray;
	typedef Array< Dummy >		  DummyArray;
	typedef Array< Influence >	  InfluenceArray;
	typedef Array< ModelCamera >  CameraArray;

private:
	String			mName;

	MapArray		mMapArray;
	BoneArray		mBoneArray;
	DummyArray		mDummyArray;
	CameraArray		mCameraArray;
	InfluenceArray	mInfluenceArray;
	SkinTransform	mSkinTransform;

	_dword			mUpdateInterval;
	_dword			mUpdateTickcount;
	_dword			mUserData;

	_bool			mChanged;

public:
	Skeleton( StringPtr name );
	~Skeleton( );

	inline Bone&			GetBone( _dword boneindex );
	inline Bone&			GetBoneByMap( _dword boneindex );

	inline const SkinTransform&	GetSkinTransform( ) const;
	inline _bool			NeedBoneAlterTransform( _dword boneindex ) const;
	inline const Matrix4*	GetBoneAlterTransform( _dword boneindex ) const;
	inline _void			AlterBone( _dword sourceboneid, _dword targetboneid );

	inline _bool			HasChanged( ) const;

	_bool LoadSkeleton( BinFile& modelfile, _bool onlyroot );

	Skeleton* Clone( ) const;

	virtual StringPtr			GetName( ) const;

	virtual _dword				GetBoneNumber( ) const;
	virtual _dword				GetBoneParent( _dword boneindex ) const;
	virtual StringPtr			GetBoneName( _dword boneindex ) const;
	virtual const Vector3&		GetBoneTranslation( _dword boneindex ) const;
	virtual const Quaternion&	GetBoneRotation( _dword boneindex ) const;
	virtual ITagPoint*			GetBoneTagPoint( _dword boneindex ) const;

	virtual _dword				GetDummyNumber( ) const;
	virtual _dword				GetDummyParent( _dword dummyindex ) const;
	virtual StringPtr			GetDummyName( _dword dummyindex ) const;
	virtual const Vector3&		GetDummyTranslation( _dword dummyindex ) const;
	virtual const Quaternion&	GetDummyRotation( _dword dummyindex ) const;
	virtual ITagPoint*			GetDummyTagPoint( _dword dummyindex ) const;

	virtual _dword				GetCameraNumber( ) const;
	virtual StringPtr			GetCameraName( _dword cameraindex ) const;
	virtual const Camera&		GetCamera( _dword cameraindex ) const;

	virtual _dword				SearchBone( StringPtr bonename ) const;
	virtual _dword				SearchDummy( StringPtr dummyname ) const;
	virtual _dword				SearchCamera( StringPtr cameraname ) const;

	virtual ITagPoint*			CreateBoneTagPoint( _dword boneindex );
	virtual ITagPoint*			CreateDummyTagPoint( _dword dummyindex );

	virtual _bool				GetInfluence( _dword boneindex, Vector3& translation, Quaternion& rotation );
	virtual _bool				SetInfluence( _dword boneindex, const Vector3& translation, const Quaternion& rotation );
	virtual _void				ClearInfluence( );

	virtual _void				SetUpdateInterval( _dword interval );
	virtual _dword				GetUpdateInterval( ) const;

	virtual _void				SetUserData( _dword userdata );
	virtual _dword				GetUserData( ) const;

	virtual _void				Update( _bool changed );
};

//----------------------------------------------------------------------------
// Skeleton Implementation
//----------------------------------------------------------------------------

Skeleton::Bone& Skeleton::GetBone( _dword boneindex )
{
	return mBoneArray[ boneindex ];
}

Skeleton::Bone& Skeleton::GetBoneByMap( _dword boneindex )
{
	if ( mMapArray.Number( ) == 0 )
		return mBoneArray[ boneindex ];

	return mBoneArray[ mMapArray[ boneindex ] ];
}

const Skeleton::SkinTransform& Skeleton::GetSkinTransform( ) const
{
	return mSkinTransform;
}

_bool Skeleton::NeedBoneAlterTransform( _dword boneindex ) const
{
	if ( mMapArray.Number( ) == 0 )
		return _false;

	return mMapArray[ boneindex ] != boneindex;
}

const Matrix4* Skeleton::GetBoneAlterTransform( _dword boneindex ) const
{
	return &mBoneArray[ boneindex ].mAlterTransform;
}

_void Skeleton::AlterBone( _dword sourceboneid, _dword targetboneid )
{
	if ( sourceboneid >= mMapArray.Number( ) )
		return;

	if ( targetboneid >= mMapArray.Number( ) )
		return;

	mMapArray[ sourceboneid ] = targetboneid;

	Bone& oldbone = mBoneArray[ sourceboneid ];
	const Bone& newbone = mBoneArray[ targetboneid ];

	Matrix4 newtrans = newbone.mBaseTransform;
	Matrix4 oldtrans = oldbone.mBaseTransform;
	
	oldbone.mAlterTransform = oldtrans.Inverse( ) * newtrans;

	mSkinTransform.mNeedAlter[ sourceboneid ] = _true;
	mSkinTransform.mAlterTransform[ sourceboneid ] = oldbone.mAlterTransform;
}

_bool Skeleton::HasChanged( ) const
{
	return mChanged;
}

};