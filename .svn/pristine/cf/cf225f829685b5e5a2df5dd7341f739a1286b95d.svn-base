//============================================================================
// ModelLoader.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// ModelLoader
//----------------------------------------------------------------------------

ModelLoader::ModelLoader( BinFile& file ) : mFile( file )
{
}

_bool ModelLoader::IsFormatValid( )
{
	if ( mFile.OffsetFromBegin( 0 ) == _false )
		return _false;

	ModelFileHeader fileheader;

	// Load file header.
	if ( mFile.ReadBuffer( &fileheader, sizeof( ModelFileHeader ) ) == _false )
		return _false;

	// Check file id.
	if ( fileheader.mFileID != ModelFileHeader::_FILE_ID )
		return _false;

	// Check file version.
	if ( fileheader.mFileVersion != ModelFileHeader::_FILE_VERSION )
		return _false;

	return _true;
}

_bool ModelLoader::LoadChunk( ModelFileChunk& chunk )
{
	// Load file header.
	if ( mFile.ReadBuffer( &chunk, sizeof( ModelFileChunk ) ) == _false )
		return _false;

	return _true;
}

_bool ModelLoader::SkipChunk( ModelFileChunk& chunk )
{
	if ( mFile.OffsetFromCurrent( chunk.mChunkSize ) == _false )
		return _false;

	return _true;
}

_bool ModelLoader::LoadChunk( ModelTextureAnimation& animation )
{
	_dword namelength = 0;
	if ( mFile.ReadDword( namelength ) == _false )
		return _false;

	// Load keyframe texture name.
	if ( mFile.ReadString( animation.mTargetTexName ) == _false )
		return _false;

	// Load sequence duration.
	if ( mFile.ReadFloat( animation.mDuration ) == _false )
		return _false;

	// Load keyframe number.
	_dword keyframenumber = 0;
	if ( mFile.ReadDword( keyframenumber ) == _false )
		return _false;

	// Load each keyframe.
	for ( _dword i = 0; i < keyframenumber; i ++ )
	{
		ModelTextureAnimationKeyframe keyframe;

		// Load keyframe time.
		if ( mFile.ReadFloat( keyframe.mTime ) == _false )
			return _false;

		namelength = 0;
		if ( mFile.ReadDword( namelength ) == _false )
			return _false;

		// Load keyframe texture name.
		if ( mFile.ReadString( keyframe.mTexName ) == _false )
			return _false;

		animation.mKeyframeArray.Append( keyframe );
	}

	return _true;
}

_bool ModelLoader::LoadChunk( ModelMesh& mesh, _bool userbuffer )
{
	// Load checksum.
	_dword checksum = 0;
	if ( mFile.ReadDword( checksum ) == _false )
		return _false;

	_dword namelength = 0;
	if ( mFile.ReadDword( namelength ) == _false )
		return _false;

	// Load mesh name.
	if ( mFile.ReadString( mesh.mMeshName ) == _false )
		return _false;

	mesh.mMeshName.Lowercase( );

	// Load format flag.
	if ( mFile.ReadDword( mesh.mFormatFlag ) == _false )
		return _false;

	// Load render flag.
	if ( mFile.ReadDword( mesh.mRenderFlag ) == _false )
		return _false;

	// Load model layer.
	if ( mFile.ReadDword( mesh.mModelLayer ) == _false )
		return _false;

	// Load vertex number.
	if ( mFile.ReadDword( mesh.mVertexNumber ) == _false )
		return _false;

	// Load face number.
	if ( mFile.ReadDword( mesh.mFaceNumber ) == _false )
		return _false;

	// Load texture number.
	_dword texnumber = 0;
	if ( mFile.ReadDword( texnumber ) == _false )
		return _false;

	// Load material id.
	_dword materialid = 0;
	if ( mFile.ReadDword( materialid ) == _false )
		return _false;

	// Load bound box.
	if ( mFile.ReadBuffer( &mesh.mBoundBox, sizeof( AxisAlignedBox ) ) == _false )
		return _false;

	// Compute vertex size from format.
	_dword vertexsize = ModelHelper::GetVertexSizeFromFormat( mesh.mFormatFlag );

	mesh.mVertexBuffer.mBuffer = mFile.GetPointer( );

	if ( mesh.mFormatFlag & Geometry::_FORMATFLAG_INFLUENCE )
	{
		mesh.mHelperBuffer.mBuffer = mFile.GetPointer( );

		// Skip vertex data one bye one.
		for ( _dword i = 0; i < mesh.mVertexNumber; i ++ )
		{
			// Skip vertex data without influence.
			if ( mFile.OffsetFromCurrent( vertexsize ) == _false )
				return _false;

			// Get influence number.
			_dword influencenumber = 0;
			if ( mFile.ReadDword( influencenumber ) == _false )
				return _false;

			// Skip vertex influence.
			if ( mFile.OffsetFromCurrent( sizeof( ModelInfluence ) * influencenumber ) == _false )
				return _false;
		}
	}
	else
	{
		mesh.mHelperBuffer.mBuffer = _null;
		mesh.mHelperBuffer.mLength = 0;

		// Skip vertex data.
		if ( mFile.OffsetFromCurrent( vertexsize * mesh.mVertexNumber ) == _false )
			return _false;
	}

	// Set vertex buffer length.
	mesh.mVertexBuffer.mLength = (_dword) ( mFile.GetPointer( ) - mesh.mVertexBuffer.mBuffer );

	if ( mesh.mFormatFlag & Geometry::_FORMATFLAG_INFLUENCE )
		mesh.mHelperBuffer.mLength = (_dword) ( mFile.GetPointer( ) - mesh.mHelperBuffer.mBuffer );

	mesh.mIndexBuffer.mBuffer = mFile.GetPointer( );
	mesh.mIndexBuffer.mLength = sizeof( _word ) * 3 * mesh.mFaceNumber;

	// Skip index data.
	if ( mFile.OffsetFromCurrent( mesh.mIndexBuffer.mLength ) == _false )
		return _false;

	texnumber = ModelHelper::GetTexcoordNumberFromFormat( mesh.mFormatFlag );

	if ( mFile.ReadDword( namelength ) == _false )
		return _false;

	// Load texture name.
	if ( mFile.ReadString( mesh.mTexName1 ) == _false )
		return _false;

	if ( texnumber >= 2 )
	{
		if ( mFile.ReadDword( namelength ) == _false )
			return _false;

		// Load texture name.
		if ( mFile.ReadString( mesh.mTexName2 ) == _false )
			return _false;
	}

	if ( texnumber >= 3 )
	{
		if ( mFile.ReadDword( namelength ) == _false )
			return _false;

		// Load texture name.
		if ( mFile.ReadString( mesh.mTexName3 ) == _false )
			return _false;
	}

	if ( texnumber >= 4 )
	{
		if ( mFile.ReadDword( namelength ) == _false )
			return _false;

		// Load texture name.
		if ( mFile.ReadString( mesh.mTexName4 ) == _false )
			return _false;
	}

	mesh.mTexName1.Lowercase( );
	mesh.mTexName2.Lowercase( );
	mesh.mTexName3.Lowercase( );
	mesh.mTexName4.Lowercase( );

	if ( userbuffer )
	{
		_dword vertexsize = ModelHelper::GetVertexSizeFromFormat( mesh.mFormatFlag );
		_dword helpersize = ModelHelper::GetHelperSizeFromFormat( mesh.mFormatFlag );

		_byte* vertexbuffer = new _byte[ vertexsize * mesh.mVertexNumber ];
		_byte* helperbuffer = _null;

		if ( helpersize > 0 )
		{
			helperbuffer = new _byte[ helpersize * mesh.mVertexNumber ];

			ModelHelper::CopyVertexBufferIntoMemory( mesh.mFormatFlag,
				mesh.mVertexNumber, vertexbuffer, helperbuffer, mesh.mVertexBuffer.mBuffer, ModelVertexAddon( ) );
		}
		else
		{
			ModelHelper::CopyVertexBufferIntoMemory( mesh.mFormatFlag,
				mesh.mVertexNumber, vertexbuffer, mesh.mVertexBuffer.mBuffer, ModelVertexAddon( ) );
		}

		_byte* indexbuffer = new _byte[ mesh.mFaceNumber * 3 * sizeof( _word ) ];
		ModelHelper::CopyIndexBufferIntoMemory( mesh.mFaceNumber * 3, indexbuffer, mesh.mIndexBuffer.mBuffer );

		mesh.mVertexBuffer.mBuffer	= vertexbuffer;
		mesh.mVertexBuffer.mLength	= vertexsize * mesh.mVertexNumber;
		mesh.mHelperBuffer.mBuffer	= helperbuffer;
		mesh.mHelperBuffer.mLength	= helpersize * mesh.mVertexNumber;
		mesh.mIndexBuffer.mBuffer	= indexbuffer;
		mesh.mIndexBuffer.mLength	= mesh.mFaceNumber * 3 * sizeof( _word );
	}

	return _true;
}

_bool ModelLoader::LoadChunk( ModelMeshAnimation& animation )
{
	// Load animation duration.
	if ( mFile.ReadFloat( animation.mDuration ) == _false )
		return _false;

	// Load track number, each track is an animation of a bone.
	_dword tracknumber = 0;
	if ( mFile.ReadDword( tracknumber ) == _false )
		return _false;

	if ( tracknumber > 0 )
		animation.mTrackArray.Clear( tracknumber, 0 );

	// Load each track.
	for ( _dword i = 0; i < tracknumber; i ++ )
	{
		ModelMeshAnimationTrack track;

		// Load mesh id.
		if ( mFile.ReadDword( track.mMeshID ) == _false )
			return _false;

		// Load animation format.
		if ( mFile.ReadDword( track.mFormatFlag ) == _false )
			return _false;

		_dword anivertexsize = ModelHelper::GetAnimationSizeFromFormat( track.mFormatFlag );

		// Load vertex number.
		if ( mFile.ReadDword( track.mVertexNumber ) == _false )
			return _false;

		// Load keyframe number.
		_dword keyframenumber = 0;
		if ( mFile.ReadDword( keyframenumber ) == _false )
			return _false;

		animation.mTrackArray.Append( track );

		// Prepare to load key frame.
		ModelMeshAnimationTrack::KeyframeArray& keyframearray = animation.mTrackArray[ animation.mTrackArray.Number( ) - 1 ].mKeyframeArray;

		if ( keyframenumber > 0 )
			keyframearray.Clear( keyframenumber, 0 );

		// Load each keyframe.
		for ( _dword j = 0; j < keyframenumber; j ++ )
		{
			ModelMeshAnimationKeyframe keyframe;

			// Load keyframe time.
			if ( mFile.ReadFloat( keyframe.mTime ) == _false )
				return _false;

			keyframe.mVertexBuffer.mLength	= anivertexsize * track.mVertexNumber;
			keyframe.mVertexBuffer.mBuffer	= mFile.GetPointer( );

			// Skip vertex data.
			mFile.OffsetFromCurrent( keyframe.mVertexBuffer.mLength );

			keyframearray.Append( keyframe );
		}
	}

	return _true;
}

_bool ModelLoader::LoadChunk( ModelSkeleton& skeleton )
{
	// Load checksum.
	_dword checksum = 0;
	if ( mFile.ReadDword( checksum ) == _false )
		return _false;

	_dword bonenumber = 0;
	if ( mFile.ReadDword( bonenumber ) == _false )
		return _false;

	_dword dummynumber = 0;
	if ( mFile.ReadDword( dummynumber ) == _false )
		return _false;

	// Load each bone.
	for ( _dword i = 0; i < bonenumber; i ++ )
	{
		ModelBone bone;

		_dword namelength = 0;
		if ( mFile.ReadDword( namelength ) == _false )
			return _false;

		// Load bone name.
		if ( mFile.ReadString( bone.mBoneName ) == _false )
			return _false;

		bone.mBoneName.Lowercase( );

		// Load parent id.
		if ( mFile.ReadDword( bone.mParentID ) == _false )
			return _false;

		// Load relative translation.
		if ( mFile.ReadBuffer( &bone.mRelativeTranslation, sizeof( Vector3 ) ) == _false )
			return _false;

		// Load relative rotation.
		if ( mFile.ReadBuffer( &bone.mRelativeRotation, sizeof( Quaternion ) ) == _false )
			return _false;

		// Load local translation ( For attached skin ).
		if ( mFile.ReadBuffer( &bone.mLocalTranslation, sizeof( Vector3 ) ) == _false )
			return _false;

		// Load local rotation ( For attached skin ).
		if ( mFile.ReadBuffer( &bone.mLocalRotation, sizeof( Quaternion ) ) == _false )
			return _false;

		skeleton.mBoneArray.Append( bone );
	}

	// Load each dummy.
	for ( _dword j = 0; j < dummynumber; j ++ )
	{
		ModelDummy dummy;

		_dword namelength = 0;
		if ( mFile.ReadDword( namelength ) == _false )
			return _false;

		// Load dummy name.
		if ( mFile.ReadString( dummy.mDummyName ) == _false )
			return _false;

		dummy.mDummyName.Lowercase( );

		// Load parent id.
		if ( mFile.ReadDword( dummy.mParentID ) == _false )
			return _false;

		// Load relative translation.
		if ( mFile.ReadBuffer( &dummy.mRelativeTranslation, sizeof( Vector3 ) ) == _false )
			return _false;

		// Load relative rotation.
		if ( mFile.ReadBuffer( &dummy.mRelativeRotation, sizeof( Quaternion ) ) == _false )
			return _false;

		skeleton.mDummyArray.Append( dummy );
	}

	return _true;
}

_bool ModelLoader::LoadChunk( ModelSkeletonAnimation& animation )
{
	// Load checksum.
	_dword checksum;
	if ( mFile.ReadDword( checksum ) == _false )
		return _false;

	// Load animation duration.
	if ( mFile.ReadFloat( animation.mDuration ) == _false )
		return _false;

	// Load track number, each track is an animation of a bone.
	_dword tracknumber = 0;
	if ( mFile.ReadDword( tracknumber ) == _false )
		return _false;

	if ( tracknumber > 0 )
		animation.mTrackArray.Clear( tracknumber, 0 );

	// Load each track.
	for ( _dword i = 0; i < tracknumber; i ++ )
	{
		ModelSkeletonAnimationTrack track;

		// Load bone id of the track.
		if ( mFile.ReadDword( track.mBoneID ) == _false )
			return _false;

		// Load keyframe number of the track.
		_dword keyframenumber = 0;
		if ( mFile.ReadDword( keyframenumber ) == _false )
			return _false;

		animation.mTrackArray.Append( track );

		// Prepare to load key frame.
		ModelSkeletonAnimationTrack::KeyframeArray& keyframearray = animation.mTrackArray[ animation.mTrackArray.Number( ) - 1 ].mKeyframeArray;

		_void* keyframebuffer = mFile.GetPointer( );
		// Skip keyframe data.
		if ( mFile.OffsetFromCurrent( sizeof( ModelSkeletonAnimationKeyframe ) * keyframenumber ) == _false )
		{
			PS_ERROR1( L"[Model] LoadChunk %s\r\n", mFile.GetFileName( ) )
			return _false;
		}

		// Load keyframes.
		keyframearray.CreateArrayFromBuffer( (ModelSkeletonAnimationKeyframe*) keyframebuffer, keyframenumber );
	}

	return _true;
}

_bool ModelLoader::LoadChunk( ModelTimeTag& timetag )
{
	// Load tag time.
	if ( mFile.ReadFloat( timetag.mTagTime ) == _false )
		return _false;

	_dword namelength = 0;
	if ( mFile.ReadDword( namelength ) == _false )
		return _false;

	// Load tag name.
	if ( mFile.ReadString( timetag.mTagName ) == _false )
		return _false;

	return _true;
}

_bool ModelLoader::LoadChunk( ModelMarker& marker )
{
	marker.mMarkerHandle = 0;

	_dword namelength = 0;
	if ( mFile.ReadDword( namelength ) == _false )
		return _false;

	// Load marker name.
	if ( mFile.ReadString( marker.mMarkerName ) == _false )
		return _false;

	marker.mMarkerName.Lowercase( );

	// Load marker tanslation.
	if ( mFile.ReadBuffer( &marker.mTranslation, sizeof( Vector3 ) ) == _false )
		return _false;

	// Load marker rotation.
	if ( mFile.ReadBuffer( &marker.mRotation, sizeof( Quaternion ) ) == _false )
		return _false;

	// Try to get 3ds max handle from name
	String namecopy = marker.mMarkerName;
	_dword index = namecopy.SearchL2R( L"[handle]" );
	if ( index != -1 )
	{
		// Set real name
		marker.mMarkerName = namecopy.SubString( 0, index );

		// Set handle
		index += StringPtr( L"[handle]" ).Length( );
		
		marker.mMarkerHandle = Long( StringPtr( (const _char*) namecopy + index ) );
	}

	return _true;
}

_bool ModelLoader::LoadChunk( ModelCamera& camera )
{
	_dword namelength = 0;
	if ( mFile.ReadDword( namelength ) == _false )
		return _false;

	// Load camera name.
	if ( mFile.ReadString( camera.mCameraName ) == _false )
		return _false;

	camera.mCameraName.Lowercase( );

	Vector3 position, look, up;

	// Load camera position.
	if ( mFile.ReadBuffer( &position, sizeof( Vector3 ) ) == _false )
		return _false;

	// Load camera look.
	if ( mFile.ReadBuffer( &look, sizeof( Vector3 ) ) == _false )
		return _false;

	// Load camera up.
	if ( mFile.ReadBuffer( &up, sizeof( Vector3 ) ) == _false )
		return _false;

	camera.mCamera = Camera( position, look, up );

	_float fovdegree = 0.0f;
	if ( mFile.ReadBuffer( &fovdegree, sizeof( _float ) ) == _false )
		return _false;

	camera.mCamera.SetFOVDegree( fovdegree );

	// Load reserved data.
	if ( mFile.ReadBuffer( &camera.mReserved, sizeof( camera.mReserved ) ) == _false )
		return _false;

	return _true;
}

_bool ModelLoader::LoadChunk( ModelModifier& modifier )
{
	// Load translation modifier.
	if ( mFile.ReadBuffer( &modifier.mTranslation, sizeof( Vector3 ) ) == _false )
		return _false;

	// Load scaling modifier.
	if ( mFile.ReadBuffer( &modifier.mScaling, sizeof( Vector3 ) ) == _false )
		return _false;

	// Load speed magic value
	if ( mFile.ReadBuffer( &modifier.mSpeedMagicValue, sizeof( _dword ) ) == _false )
		return _false;

	// Load speed scaling value
	if ( mFile.ReadBuffer( &modifier.mSpeedScaling, sizeof( _float ) ) == _false )
		return _false;

	// Load reserved data.
	if ( mFile.ReadBuffer( &modifier.mReserved, sizeof( modifier.mReserved ) ) == _false )
		return _false;

	return _true;
}

_bool ModelLoader::LoadChunk( ModelAlteration& alteration )
{
	// Load alteration keyframe time.
	if ( mFile.ReadBuffer( &alteration.mTime, sizeof( _float ) ) == _false )
		return _false;

	// Load source bone ID.
	if ( mFile.ReadBuffer( &alteration.mSourceBoneID, sizeof( _dword ) ) == _false )
		return _false;

	// Load target bone ID.
	if ( mFile.ReadBuffer( &alteration.mTargetBoneID, sizeof( _dword ) ) == _false )
		return _false;

	// Load reserved data.
	if ( mFile.ReadBuffer( &alteration.mReserved, sizeof( alteration.mReserved ) ) == _false )
		return _false;

	return _true;
}