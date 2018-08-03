//============================================================================
// Skeleton.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// Skeleton Implementation
//----------------------------------------------------------------------------

Skeleton::Skeleton( StringPtr name ) : mName( name )
{
	mUpdateInterval		= 0;
	mUpdateTickcount	= 0;
	mUserData			= 0;
	mChanged			= _false;

	IncreaseRefCount( );
}

Skeleton::~Skeleton( )
{
	for ( _dword i = 0; i < mBoneArray.Number( ); i ++ )
	{
		if ( mBoneArray[i].mAttachedTagPoint == _null )
			continue;

		GetPhysicsFactory( ).ReleaseTagPoint( mBoneArray[i].mAttachedTagPoint );
	}

	for ( _dword j = 0; j < mDummyArray.Number( ); j ++ )
	{
		if ( mDummyArray[j].mAttachedTagPoint == _null )
			continue;

		GetPhysicsFactory( ).ReleaseTagPoint( mDummyArray[j].mAttachedTagPoint );
	}
}

_bool Skeleton::LoadSkeleton( BinFile& modelfile, _bool onlyroot )
{
	ModelLoader loader( modelfile );

	if ( loader.IsFormatValid( ) == _false )
		return _false;

	// Read chunks in the file.
	ModelFileChunk	chunk;
	ModelSkeleton	skeleton;
	while ( loader.LoadChunk( chunk ) )
	{
		if ( chunk.mChunkID == ModelFileChunk::_CHUNKID_SKL )
		{
			if ( loader.LoadChunk( skeleton ) == _false )
				return _false;
		}
		// Found camera chunk.
		else if ( chunk.mChunkID == ModelFileChunk::_CHUNKID_CAM ||
				  chunk.mChunkID == ModelFileChunk::_CHUNKID_FOV )
		{
			ModelCamera camera;

			// Load camera chunk.
			if ( loader.LoadChunk( camera ) == _false )
				return _false;

			if ( camera.mCamera.mFOVDegree == 0.0f )
				camera.mCamera.mFOVDegree = 58.0f;

			// Insert into camera array.
			mCameraArray.Append( camera );
		}
		else
		{
			if ( loader.SkipChunk( chunk ) == _false )
				return _false;
		}
	}

	// Load all bones and dummys.
	if ( onlyroot == _false )
	{
		mMapArray.Clear( skeleton.mBoneArray.Number( ), 10 );
		mBoneArray.Clear( skeleton.mBoneArray.Number( ), 10 );
		mDummyArray.Clear( skeleton.mDummyArray.Number( ), 10 );

		// Get each bone.
		for ( _dword i = 0; i < skeleton.mBoneArray.Number( ); i ++ )
		{
			Bone bone;
			bone.mBoneName					= skeleton.mBoneArray[i].mBoneName;
			bone.mParentID					= skeleton.mBoneArray[i].mParentID;
			bone.mRelativeTranslation		= skeleton.mBoneArray[i].mRelativeTranslation;
			bone.mRelativeRotation			= skeleton.mBoneArray[i].mRelativeRotation;
			bone.mInitTranslation			= skeleton.mBoneArray[i].mRelativeTranslation;
			bone.mInitRotation				= skeleton.mBoneArray[i].mRelativeRotation;
			bone.mLocalTranslationBoneSpace	= skeleton.mBoneArray[i].mLocalTranslation;
			bone.mLocalRotationBoneSpace	= skeleton.mBoneArray[i].mLocalRotation;
			bone.mAttachedTagPoint			= _null;

			bone.mInitBaseTransform			= _false;
			bone.mBaseTransform				= Matrix4::cIdentity;

			mMapArray.Append( mBoneArray.Number( ) );
			mBoneArray.Append( bone );
		}

		// Get each dummy.
		for ( _dword j = 0; j < skeleton.mDummyArray.Number( ); j ++ )
		{
			Dummy dummy;
			dummy.mDummyName				= skeleton.mDummyArray[j].mDummyName;
			dummy.mParentID					= skeleton.mDummyArray[j].mParentID;
			dummy.mRelativeTranslation		= skeleton.mDummyArray[j].mRelativeTranslation;
			dummy.mRelativeRotation			= skeleton.mDummyArray[j].mRelativeRotation;
			dummy.mAttachedTagPoint			= _null;

			mDummyArray.Append( dummy );
		}
	}
	// Only load bone with root.
	else
	{
		for ( _dword i = 0; i < skeleton.mBoneArray.Number( ); i ++ )
		{
			Bone bone;
			bone.mParentID					= skeleton.mBoneArray[i].mParentID;
			bone.mRelativeTranslation		= skeleton.mBoneArray[i].mRelativeTranslation;
			bone.mRelativeRotation			= skeleton.mBoneArray[i].mRelativeRotation;
			bone.mInitTranslation			= skeleton.mBoneArray[i].mRelativeTranslation;
			bone.mInitRotation				= skeleton.mBoneArray[i].mRelativeRotation;
			bone.mLocalTranslationBoneSpace	= skeleton.mBoneArray[i].mLocalTranslation;
			bone.mLocalRotationBoneSpace	= skeleton.mBoneArray[i].mLocalRotation;
			bone.mAttachedTagPoint			= _null;

			mBoneArray.Clear( 1, 10 );
			mBoneArray.Append( bone );

			break;
		}
	}

	mSkinTransform.Resize( mBoneArray.Number( ) );

	Update( _true );

	return skeleton.mBoneArray.Number( ) > 0;
}

Skeleton* Skeleton::Clone( ) const
{
	// Clone an new skeleton.
	Skeleton* newskeleton = new Skeleton( mName );

	// Copy bone id map array.
	newskeleton->mMapArray		= mMapArray;

	// Copy bone array.
	newskeleton->mBoneArray		= mBoneArray;

	// Copy dummy array.
	newskeleton->mDummyArray	= mDummyArray;

	for ( _dword i = 0; i < mCameraArray.Number( ); i ++ )
		newskeleton->mCameraArray.Append( mCameraArray[i] );

	newskeleton->mSkinTransform = mSkinTransform;

	// Clone bone tag point.
	for ( _dword i = 0; i < mBoneArray.Number( ); i ++ )
		newskeleton->mBoneArray[i].mAttachedTagPoint = GetPhysicsFactory( ).CloneTagPoint( mBoneArray[i].mAttachedTagPoint, _true );

	// Clone dummy tag point.
	for ( _dword i = 0; i < mDummyArray.Number( ); i ++ )
		newskeleton->mDummyArray[i].mAttachedTagPoint = GetPhysicsFactory( ).CloneTagPoint( mDummyArray[i].mAttachedTagPoint, _true );

	return newskeleton;
}

StringPtr Skeleton::GetName( ) const
{
	return mName;
}

_dword Skeleton::GetBoneNumber( ) const
{
	return mBoneArray.Number( );
}

StringPtr Skeleton::GetBoneName( _dword boneindex ) const
{
	return mBoneArray[ boneindex ].mBoneName;
}

_dword Skeleton::GetBoneParent( _dword boneindex ) const
{
	return mBoneArray[ boneindex ].mParentID;
}

const Vector3& Skeleton::GetBoneTranslation( _dword boneindex ) const
{
	return mBoneArray[ boneindex ].mAbsoluteTranslation;
}

const Quaternion& Skeleton::GetBoneRotation( _dword boneindex ) const
{
	return mBoneArray[ boneindex ].mAbsoluteRotation;
}

ITagPoint* Skeleton::GetBoneTagPoint( _dword boneindex ) const
{
	return mBoneArray[ boneindex ].mAttachedTagPoint;
}

_dword Skeleton::GetDummyNumber( ) const
{
	return mDummyArray.Number( );
}

StringPtr Skeleton::GetDummyName( _dword dummyindex ) const
{
	return mDummyArray[ dummyindex ].mDummyName;
}

_dword Skeleton::GetDummyParent( _dword dummyindex ) const
{
	return mDummyArray[ dummyindex ].mParentID;
}

const Vector3& Skeleton::GetDummyTranslation( _dword dummyindex ) const
{
	return mDummyArray[ dummyindex ].mAbsoluteTranslation;
}

const Quaternion& Skeleton::GetDummyRotation( _dword dummyindex ) const
{
	return mDummyArray[ dummyindex ].mAbsoluteRotation;
}

ITagPoint* Skeleton::GetDummyTagPoint( _dword dummyindex ) const
{
	return mDummyArray[ dummyindex ].mAttachedTagPoint;
}

_dword Skeleton::GetCameraNumber( ) const
{
	return mCameraArray.Number( );
}

StringPtr Skeleton::GetCameraName( _dword cameraindex ) const
{
	return mCameraArray[ cameraindex ].mCameraName;
}

const Camera& Skeleton::GetCamera( _dword cameraindex ) const
{
	return mCameraArray[ cameraindex ].mCamera;
}

_dword Skeleton::SearchBone( StringPtr bonename ) const
{
	for ( _dword i = 0; i < mBoneArray.Number( ); i ++ )
	{
		if ( mBoneArray[i].mBoneName.CompareLowercase( bonename ) )
			return i;
	}

	return -1;
}

_dword Skeleton::SearchDummy( StringPtr dummyname ) const
{
	for ( _dword i = 0; i < mDummyArray.Number( ); i ++ )
	{
		if ( mDummyArray[i].mDummyName.CompareLowercase( dummyname ) )
			return i;
	}

	return -1;
}

_dword Skeleton::SearchCamera( StringPtr cameraname ) const
{
	for ( _dword i = 0; i < mCameraArray.Number( ); i ++ )
	{
		if ( mCameraArray[i].mCameraName.CompareLowercase( cameraname ) )
			return i;
	}

	return -1;
}

ITagPoint* Skeleton::CreateBoneTagPoint( _dword boneindex )
{
	if ( mBoneArray[ boneindex ].mAttachedTagPoint == _null )
		mBoneArray[ boneindex ].mAttachedTagPoint = GetPhysicsFactory( ).CreateTagPoint( );

	return mBoneArray[ boneindex ].mAttachedTagPoint;
}

ITagPoint* Skeleton::CreateDummyTagPoint( _dword dummyindex )
{
	if ( mDummyArray[ dummyindex ].mAttachedTagPoint == _null )
		mDummyArray[ dummyindex ].mAttachedTagPoint = GetPhysicsFactory( ).CreateTagPoint( );

	return mDummyArray[ dummyindex ].mAttachedTagPoint;
}

_bool Skeleton::GetInfluence( _dword boneindex, Vector3& translation, Quaternion& rotation )
{
	if ( boneindex >= mBoneArray.Number( ) )
		return _false;

	for ( _dword i = 0; i < mInfluenceArray.Number( ); i ++ )
	{
		if ( mInfluenceArray[i].mBoneID == boneindex )
		{
			translation = mInfluenceArray[i].mTranslation;
			rotation	= mInfluenceArray[i].mRotation;

			return _true;
		}
	}

	return _false;
}

_bool Skeleton::SetInfluence( _dword boneindex, const Vector3& translation, const Quaternion& rotation )
{
	if ( boneindex >= mBoneArray.Number( ) )
		return _false;

	for ( _dword i = 0; i < mInfluenceArray.Number( ); i ++ )
	{
		if ( mInfluenceArray[i].mBoneID == boneindex )
		{
			mInfluenceArray[i].mTranslation	= translation;
			mInfluenceArray[i].mRotation	= rotation;

			return _true;
		}
	}

	Influence influence;
	influence.mBoneID		= boneindex;
	influence.mTranslation	= translation;
	influence.mRotation		= rotation;

	mInfluenceArray.Append( influence );

	return _true;
}

_void Skeleton::ClearInfluence( )
{
	mInfluenceArray.Clear( );
}

_void Skeleton::SetUpdateInterval( _dword interval )
{
	mUpdateInterval = interval;
}

_dword Skeleton::GetUpdateInterval( ) const
{
	return mUpdateInterval;
}

_void Skeleton::SetUserData( _dword userdata )
{
	mUserData = userdata;
}

_dword Skeleton::GetUserData( ) const
{
	return mUserData;
}

_void Skeleton::Update( _bool changed )
{
	mChanged = _false;

	if ( changed == _false )
		return;

	if ( mUpdateInterval != 0 )
	{
		if ( System::GetCurrentTickcount( ) - mUpdateTickcount < mUpdateInterval )
			return;

		mUpdateTickcount = System::GetCurrentTickcount( );
	}

	// Update each bone.
	for ( _dword i = 0; i < mBoneArray.Number( ); i ++ )
	{
		Bone& bone = mBoneArray[i];

		Vector3 oldposition		= bone.mAbsoluteTranslation;
		Quaternion oldrotation	= bone.mAbsoluteRotation;

		if ( bone.mParentID >= mBoneArray.Number( ) )
		{
			bone.mAbsoluteTranslation	= bone.mRelativeTranslation;
			bone.mAbsoluteRotation		= bone.mRelativeRotation;
		}
		else
		{
			Bone& parent = mBoneArray[ bone.mParentID ];

			bone.mAbsoluteTranslation	 =  bone.mRelativeTranslation;
			bone.mAbsoluteTranslation	*= -parent.mAbsoluteRotation;
			bone.mAbsoluteTranslation	+=  parent.mAbsoluteTranslation;

			bone.mAbsoluteRotation		 = bone.mRelativeRotation;
			bone.mAbsoluteRotation		*= parent.mAbsoluteRotation;
		}

		if ( bone.mInitBaseTransform == _false )
		{
			bone.mBaseTransform.FromTranslationRotation( bone.mAbsoluteTranslation, -bone.mAbsoluteRotation );
			bone.mInitBaseTransform = _true;
		}

		for ( _dword j = 0; j < mInfluenceArray.Number( ); j ++ )
		{
			if ( i == mInfluenceArray[j].mBoneID )
			{
				bone.mAbsoluteTranslation	+= mInfluenceArray[j].mTranslation;
				bone.mAbsoluteRotation		*= mInfluenceArray[j].mRotation;

				break;
			}
		}

		// Update local translation for skin.
		bone.mLocalTranslation	 =  bone.mLocalTranslationBoneSpace;
		bone.mLocalTranslation	*= -bone.mAbsoluteRotation;
		bone.mLocalTranslation	+=  bone.mAbsoluteTranslation;

		// Build bone rotation matrix, vector * matrix faster than vector * quaternion.
		bone.mLocalRotation.FromRotation( - ( bone.mLocalRotationBoneSpace * bone.mAbsoluteRotation ) );

		// Update attached tag point is exist.
		if ( bone.mAttachedTagPoint != _null && bone.mParentID < mBoneArray.Number( ) )
		{
			Bone& parent = mBoneArray[ bone.mParentID ];

			bone.mAttachedTagPoint->SetPosition( bone.mAbsoluteTranslation );
			bone.mAttachedTagPoint->SetDirection( bone.mAbsoluteTranslation - parent.mAbsoluteTranslation );
			bone.mAttachedTagPoint->SetRotation( bone.mLocalRotation );
		}

		if ( mChanged == _false )
		{
			if ( oldposition.Equal( bone.mAbsoluteTranslation ) == _false || oldrotation.Equal( bone.mAbsoluteRotation ) == _false )
				mChanged = _true;
		}
	}

	// Update transform for skinning.
	for ( _dword i = 0; i < mBoneArray.Number( ); i ++ )
	{
		Bone &bone = mMapArray.Number( ) == 0 ? mBoneArray[ i ] : mBoneArray[ mMapArray[ i ] ];
		mSkinTransform.mLocalTransform[i] = Matrix3x4( bone.mLocalTranslation, bone.mLocalRotation );
	}

	// Update each dummy.
	for ( _dword j = 0; j < mDummyArray.Number( ); j ++ )
	{
		Dummy& dummy = mDummyArray[j];

		if ( dummy.mParentID == -1 )
		{
			dummy.mAbsoluteTranslation	= dummy.mRelativeTranslation;
			dummy.mAbsoluteRotation		= dummy.mRelativeRotation;
		}
		else
		{
			Bone& parent = mBoneArray[ dummy.mParentID ];

			dummy.mAbsoluteTranslation	 =  dummy.mRelativeTranslation;
			dummy.mAbsoluteTranslation	*= -parent.mAbsoluteRotation;
			dummy.mAbsoluteTranslation	+=  parent.mAbsoluteTranslation;

			dummy.mAbsoluteRotation		 = dummy.mRelativeRotation;
			dummy.mAbsoluteRotation		*= parent.mAbsoluteRotation;
		}

		// Update attached tag point is exist.
		if ( dummy.mAttachedTagPoint != _null && dummy.mParentID < mBoneArray.Number( ) )
		{
			Bone& parent = mBoneArray[ dummy.mParentID ];

			dummy.mAttachedTagPoint->SetPosition( dummy.mAbsoluteTranslation );
			dummy.mAttachedTagPoint->SetDirection( dummy.mAbsoluteTranslation - parent.mAbsoluteTranslation );
			dummy.mAttachedTagPoint->SetRotation( Matrix3( ).FromRotation( - ( parent.mLocalRotationBoneSpace * dummy.mAbsoluteRotation ) ) );
		}
	}
}