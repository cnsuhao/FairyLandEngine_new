//============================================================================
// Model.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ModelTextureAnimationKeyframe
//----------------------------------------------------------------------------

struct ModelTextureAnimationKeyframe
{
	_float		mTime;
	String		mTexName;

	operator _float ( ) const
		{ return mTime; }
};

//----------------------------------------------------------------------------
// ModelTextureAnimation
//----------------------------------------------------------------------------

struct ModelTextureAnimation
{
	typedef Array< ModelTextureAnimationKeyframe > KeyframeArray;

	String			mTargetTexName;
	_float			mDuration;
	KeyframeArray	mKeyframeArray;
};

//----------------------------------------------------------------------------
// ModelBuffer
//----------------------------------------------------------------------------

struct ModelBuffer
{
	_byte*		mBuffer;
	_dword		mLength;
};

//----------------------------------------------------------------------------
// ModelVertexAddon
//----------------------------------------------------------------------------

struct ModelVertexAddon
{
	_bool	mEnableAdditionalNormal;
	Vector3	mDefaultAdditionalNormal;

	_bool	mEnableAdditionalDiffuse;
	_dword	mDefaultAdditionalDiffuse;

	_bool	mEnableAdditionalSpecular;
	_dword	mDefaultAdditionalSpecular;

	_bool	mEnableAdditionalTexcoord;
	Vector2	mDefaultAdditionalTexcoord;

	ModelVertexAddon( );

	_bool IsEnabled( ) const;

	ModelVertexAddon& EnableAdditionalNormal( const Vector3& normal );
	ModelVertexAddon& EnableAdditionalDiffuse( _dword diffuse );
	ModelVertexAddon& EnableAdditionalSpecular( _dword specular );
	ModelVertexAddon& EnableAdditionalTexcoord( const Vector2& texcoord );

	_dword UpdateFormat( _dword format ) const;
};

//----------------------------------------------------------------------------
// ModelMesh
//----------------------------------------------------------------------------

struct ModelMesh
{
	String			mGroupName;
	String			mMeshName;
	String			mTexName1;
	String			mTexName2;
	String			mTexName3;
	String			mTexName4;

	_dword			mFormatFlag;
	_dword			mRenderFlag;
	_dword			mModelLayer;

	_dword			mVertexNumber;
	_dword			mFaceNumber;
	_dword			mMaxInfluenceNumber;

	AxisAlignedBox	mBoundBox;

	ModelBuffer		mVertexBuffer;
	ModelBuffer		mHelperBuffer;
	ModelBuffer		mIndexBuffer;
};

//----------------------------------------------------------------------------
// ModelMeshAnimationKeyframe
//----------------------------------------------------------------------------

struct ModelMeshAnimationKeyframe
{
	_float			mTime;
	ModelBuffer		mVertexBuffer;
};

//----------------------------------------------------------------------------
// ModelMeshAnimationTrack
//----------------------------------------------------------------------------

struct ModelMeshAnimationTrack
{
	typedef Array< ModelMeshAnimationKeyframe > KeyframeArray;

	_dword			mMeshID;
	_dword			mFormatFlag;
	_dword			mVertexNumber;

	KeyframeArray	mKeyframeArray;
};

//----------------------------------------------------------------------------
// ModelMeshAnimation
//----------------------------------------------------------------------------

struct ModelMeshAnimation
{
	typedef Array< ModelMeshAnimationTrack > TrackArray;

	_float		mDuration;
	TrackArray	mTrackArray;
};

//----------------------------------------------------------------------------
// ModelBone
//----------------------------------------------------------------------------

struct ModelBone
{
	String		mBoneName;
	_dword		mParentID;

	Vector3		mRelativeTranslation;
	Quaternion	mRelativeRotation;

	Vector3		mLocalTranslation;
	Quaternion	mLocalRotation;
};

//----------------------------------------------------------------------------
// ModelDummy
//----------------------------------------------------------------------------

struct ModelDummy
{
	String		mDummyName;
	_dword		mParentID;

	Vector3		mRelativeTranslation;
	Quaternion	mRelativeRotation;
};

//----------------------------------------------------------------------------
// ModelSkeleton
//----------------------------------------------------------------------------

struct ModelSkeleton
{
	typedef Array< ModelBone >	BoneArray;
	typedef Array< ModelDummy >	DummyArray;

	BoneArray	mBoneArray;
	DummyArray	mDummyArray;
};

//----------------------------------------------------------------------------
// ModelSkeletonAnimationKeyframe
//----------------------------------------------------------------------------

struct ModelSkeletonAnimationKeyframe
{
	_float		mTime;
	Vector3		mTranslation;
	Quaternion	mRotation;
};

//----------------------------------------------------------------------------
// ModelSkeletonAnimationTrack
//----------------------------------------------------------------------------

struct ModelSkeletonAnimationTrack
{
	typedef Array< ModelSkeletonAnimationKeyframe > KeyframeArray;

	_dword			mBoneID;
	KeyframeArray	mKeyframeArray;
};

//----------------------------------------------------------------------------
// ModelSkeletonAnimation
//----------------------------------------------------------------------------

struct ModelSkeletonAnimation
{
	typedef Array< ModelSkeletonAnimationTrack > TrackArray;

	_float		mDuration;
	TrackArray	mTrackArray;
};

//----------------------------------------------------------------------------
// ModelInfluence
//----------------------------------------------------------------------------

struct ModelInfluence
{
	_dword		mBoneID;
	_float		mWeight;
};

//----------------------------------------------------------------------------
// ModelInfluenceGroup
//----------------------------------------------------------------------------

struct ModelInfluenceGroup
{
	enum { _MAX_INFLUENCE_NUMBER = 8 };

	// Divide bone id and weight to two groups, in order to pass them to GPU register.

	_byte		mBoneIDList[ _MAX_INFLUENCE_NUMBER ];
	_float		mWeightList[ _MAX_INFLUENCE_NUMBER ];
};

//----------------------------------------------------------------------------
// ModelTimeTag
//----------------------------------------------------------------------------

struct ModelTimeTag
{
	_float		mTagTime;
	String		mTagName;
};

//----------------------------------------------------------------------------
// ModelMarker
//----------------------------------------------------------------------------

struct ModelMarker
{
	String		mMarkerName;

	Vector3		mTranslation;
	Quaternion	mRotation;

	_dword		mMarkerHandle;
};

//----------------------------------------------------------------------------
// ModelCamera
//----------------------------------------------------------------------------

struct ModelCamera
{
	String		mCameraName;
	Camera		mCamera;

	_byte		mReserved[64];
};

//----------------------------------------------------------------------------
// ModelModifier
//----------------------------------------------------------------------------

struct ModelModifier
{
	Vector3		mTranslation;
	Vector3		mScaling;
	_dword		mSpeedMagicValue;
	_float		mSpeedScaling;

	_byte		mReserved[56];
};

//----------------------------------------------------------------------------
// ModelAlteration
//----------------------------------------------------------------------------

struct ModelAlteration
{
	_float		mTime;
	_dword		mSourceBoneID;
	_dword		mTargetBoneID;

	_byte		mReserved[64];
};

//----------------------------------------------------------------------------
// ModelFileHeader
//----------------------------------------------------------------------------

struct ModelFileHeader
{
	enum _FILEFLAG
	{
		_FILE_ID		= 0x00525350,	// PSR\0
		_FILE_VERSION	= 0x00000300,	// 0.30
	};

	_dword		mFileID;
	_dword		mFileVersion;
};

//----------------------------------------------------------------------------
// ModelFileChunk
//----------------------------------------------------------------------------

struct ModelFileChunk
{
	enum _CHUNKID
	{
		_CHUNKID_TAN	= 0x004E4154,	// TAN\0
		_CHUNKID_MSH	= 0x0048534D,	// MSH\0
		_CHUNKID_MAN	= 0x004E414D,	// MAN\0
		_CHUNKID_SKL	= 0x004C4B53,	// SKL\0
		_CHUNKID_SAN	= 0x004E4153,	// SAN\0
		_CHUNKID_SKN	= 0x004E4B53,	// SKN\0
		_CHUNKID_TAG	= 0x00474154,	// TAG\0
		_CHUNKID_MRK	= 0x004B524D,	// MRK\0
		_CHUNKID_CAM	= 0x004D4143,	// CAM\0
		_CHUNKID_FOV	= 0x00564F46,	// FOV( camera with fov )\0
		_CHUNKID_MDF	= 0x0046444D,	// MDF\0
		_CHUNKID_ALT	= 0x00544C41,	// ALT\0
	};

	_dword		mChunkID;
	_dword		mChunkSize;
};

};