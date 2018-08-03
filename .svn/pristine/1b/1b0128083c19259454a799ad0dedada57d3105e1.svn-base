//============================================================================
// ModelSaver.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// ModelSaver Implementation
//----------------------------------------------------------------------------

_bool ModelSaver::Create( StringPtr filename )
{
	// Create file to save model.
	if ( mFile.Open( filename, File::_CREATE_ALWAYS, File::_OPERATION_WRITE, File::_SHARE_READ ) == _false )
		return _false;

	ModelFileHeader fileheader;

	fileheader.mFileID		= ModelFileHeader::_FILE_ID;
	fileheader.mFileVersion	= ModelFileHeader::_FILE_VERSION;

	// Write file version.
	if ( mFile.Write( &fileheader, sizeof( ModelFileHeader ) ) == _false )
		return _false;

	return _true;
}

_bool ModelSaver::SaveChunk( const ModelTextureAnimation& animation )
{
	// Write chunk id.
	_dword chunkid = ModelFileChunk::_CHUNKID_TAN;
	if ( mFile.Write( &chunkid, sizeof( _dword ) ) == _false )
		return _false;

	// Skip chunk Size now.
	_dword chunksize = 0;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	_dword chunkoffset1 = mFile.GetOffset( );

	// Write target texture name length.
	_dword namelength = animation.mTargetTexName.Length( );
	if ( mFile.Write( &namelength, sizeof( _dword ) ) == _false )
		return _false;

	// Write target texture name.
	if ( mFile.Write( (const _char*) animation.mTargetTexName, animation.mTargetTexName.SizeOfBytes( ) ) == _false )
		return _false;

	// Write animation duration.
	if ( mFile.Write( &animation.mDuration, sizeof( _float ) ) == _false )
		return _false;

	// Write keyframe number.
	_dword keyframenumber = animation.mKeyframeArray.Number( );
	if ( mFile.Write( &keyframenumber, sizeof( _dword ) ) == _false )
		return _false;

	for ( _dword i = 0; i < keyframenumber; i ++ )
	{
		const ModelTextureAnimationKeyframe& keyframe = animation.mKeyframeArray[i];

		// Write keyframe time.
		if ( mFile.Write( &keyframe.mTime, sizeof( _float ) ) == _false )
			return _false;

		// Write texture name length.
		namelength = keyframe.mTexName.Length( );
		if ( mFile.Write( &namelength, sizeof( _dword ) ) == _false )
			return _false;

		// Write texture name.
		if ( mFile.Write( (const _char*) keyframe.mTexName, keyframe.mTexName.SizeOfBytes( ) ) == _false )
			return _false;
	}

	_dword chunkoffest2 = mFile.GetOffset( );

	// Move file pointer to chunk begin.
	if ( mFile.SeekFromBegin( chunkoffset1 - sizeof( _dword ) ) == _false )
		return _false;

	// Write chunk size.
	chunksize = chunkoffest2 - chunkoffset1;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	// Move file pointer to chunk end.
	if ( mFile.SeekFromBegin( chunkoffest2 ) == _false )
		return _false;

	return _true;
}

_bool ModelSaver::SaveChunk( const ModelMesh& mesh, _bool userbuffer )
{
	// Write chunk id.
	_dword chunkid = ModelFileChunk::_CHUNKID_MSH;
	if ( mFile.Write( &chunkid, sizeof( _dword ) ) == _false )
		return _false;

	// Skip chunk Size now.
	_dword chunksize = 0;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	_dword chunkoffset1 = mFile.GetOffset( );

	// Write checksum.
	_dword checksum = 0;
	if ( mFile.Write( &checksum, sizeof( _dword ) ) == _false )
		return _false;

	// Write mesh name length.
	_dword namelength = mesh.mMeshName.Length( );
	if ( mFile.Write( &namelength, sizeof( _dword ) ) == _false )
		return _false;

	// Write mesh name.
	if ( mFile.Write( (const _char*) mesh.mMeshName, mesh.mMeshName.SizeOfBytes( ) ) == _false )
		return _false;

	// Write format flag.
	if ( mFile.Write( &mesh.mFormatFlag, sizeof( _dword ) ) == _false )
		return _false;

	// Write render flag.
	if ( mFile.Write( &mesh.mRenderFlag, sizeof( _dword ) ) == _false )
		return _false;

	// Write model layer.
	if ( mFile.Write( &mesh.mModelLayer, sizeof( _dword ) ) == _false )
		return _false;

	// Write vertex number.
	if ( mFile.Write( &mesh.mVertexNumber, sizeof( _dword ) ) == _false )
		return _false;

	// Write face number.
	if ( mFile.Write( &mesh.mFaceNumber, sizeof( _dword ) ) == _false )
		return _false;

	// Write texture number.
	_dword texnumber = 1;
	if ( mFile.Write( &texnumber, sizeof( _dword ) ) == _false )
		return _false;

	// Write material id.
	_dword materialid = 0;
	if ( mFile.Write( &materialid, sizeof( _dword ) ) == _false )
		return _false;

	// Write bound box.
	AxisAlignedBox boundbox = ModelHelper::GetBoundBoxFromVertexBuffer( mesh.mFormatFlag, mesh.mVertexBuffer );
	if ( mFile.Write( &boundbox, sizeof( AxisAlignedBox ) ) == _false )
		return _false;

	// If vertex format with influence, use different format.
	if ( mesh.mFormatFlag & Geometry::_FORMATFLAG_INFLUENCE )
	{
		_dword vertexsize = ModelHelper::GetVertexSizeFromFormat( mesh.mFormatFlag );
		_dword helpersize = ModelHelper::GetHelperSizeFromFormat( mesh.mFormatFlag );

		_byte* vertexbuffer = mesh.mVertexBuffer.mBuffer;
		_byte* helperbuffer = mesh.mHelperBuffer.mBuffer;

		for ( _dword j = 0; j < mesh.mVertexNumber; j ++ )
		{
			// Write vertex data without influence.
			if ( mFile.Write( vertexbuffer, vertexsize ) == _false )
				return _false;

			// Get influence list pointer.
			const ModelInfluenceGroup& influencegroup = *(ModelInfluenceGroup*) ( helperbuffer + helpersize - sizeof( ModelInfluenceGroup ) );

			// Get influence number.
			_dword influencenumber = 0;

			for ( _dword k = 0; k < ModelInfluenceGroup::_MAX_INFLUENCE_NUMBER; k ++ )
			{
				if ( influencegroup.mBoneIDList[k] == 0xFF )
					break;

				influencenumber ++;
			}

			// Write influence number.
			if ( mFile.Write( &influencenumber, sizeof( _dword ) ) == _false )
				return _false;

			// Wirte influences.
			for ( _dword k = 0; k < influencenumber; k ++ )
			{
				_dword boneid = influencegroup.mBoneIDList[k];
				_float weight = influencegroup.mWeightList[k];

				if ( mFile.Write( &boneid, sizeof( _dword ) ) == _false )
					return _false;

				if ( mFile.Write( &weight, sizeof( _float ) ) == _false )
					return _false;
			}

			vertexbuffer += vertexsize;
			helperbuffer += helpersize;
		}
	}
	// Otherwise, just directly write whole buffer.
	else
	{
		if ( mFile.Write( mesh.mVertexBuffer.mBuffer, mesh.mVertexBuffer.mLength ) == _false )
			return _false;
	}

	// Write index data.
	if ( mFile.Write( mesh.mIndexBuffer.mBuffer, mesh.mIndexBuffer.mLength ) == _false )
		return _false;

	texnumber = ModelHelper::GetTexcoordNumberFromFormat( mesh.mFormatFlag );

	// Write texture name length.
	namelength = mesh.mTexName1.Length( );
	if ( mFile.Write( &namelength, sizeof( _dword ) ) == _false )
		return _false;

	// Write texture name.
	if ( mFile.Write( (const _char*) mesh.mTexName1, mesh.mTexName1.SizeOfBytes( ) ) == _false )
		return _false;

	if ( texnumber >= 2 )
	{
		// Write texture name length.
		namelength = mesh.mTexName2.Length( );
		if ( mFile.Write( &namelength, sizeof( _dword ) ) == _false )
			return _false;

		// Write texture name.
		if ( mFile.Write( (const _char*) mesh.mTexName2, mesh.mTexName2.SizeOfBytes( ) ) == _false )
			return _false;
	}

	if ( texnumber >= 3 )
	{
		// Write texture name length.
		namelength = mesh.mTexName3.Length( );
		if ( mFile.Write( &namelength, sizeof( _dword ) ) == _false )
			return _false;

		// Write texture name.
		if ( mFile.Write( (const _char*) mesh.mTexName3, mesh.mTexName3.SizeOfBytes( ) ) == _false )
			return _false;
	}

	if ( texnumber >= 4 )
	{
		// Write texture name length.
		namelength = mesh.mTexName4.Length( );
		if ( mFile.Write( &namelength, sizeof( _dword ) ) == _false )
			return _false;

		// Write texture name.
		if ( mFile.Write( (const _char*) mesh.mTexName4, mesh.mTexName4.SizeOfBytes( ) ) == _false )
			return _false;
	}

	_dword chunkoffest2 = mFile.GetOffset( );

	// Move file pointer to chunk begin.
	if ( mFile.SeekFromBegin( chunkoffset1 - sizeof( _dword ) ) == _false )
		return _false;

	// Write chunk size.
	chunksize = chunkoffest2 - chunkoffset1;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	// Move file pointer to chunk end.
	if ( mFile.SeekFromBegin( chunkoffest2 ) == _false )
		return _false;

	if ( userbuffer )
	{
		delete[] mesh.mVertexBuffer.mBuffer;
		delete[] mesh.mHelperBuffer.mBuffer;
		delete[] mesh.mIndexBuffer.mBuffer;
	}

	return _true;
}

_bool ModelSaver::SaveChunk( const ModelMeshAnimation& animation )
{
	// Write chunk id.
	_dword chunkid = ModelFileChunk::_CHUNKID_MAN;
	if ( mFile.Write( &chunkid, sizeof( _dword ) ) == _false )
		return _false;

	// Skip chunk Size now.
	_dword chunksize = 0;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	_dword chunkoffset1 = mFile.GetOffset( );

	// Write animation duration.
	if ( mFile.Write( &animation.mDuration, sizeof( _float ) ) == _false )
		return _false;

	// Write track number.
	_dword tracknumber = animation.mTrackArray.Number( );
	if ( mFile.Write( &tracknumber, sizeof( _dword ) ) == _false )
		return _false;

	// Write each track.
	for ( _dword i = 0; i < tracknumber; i ++ )
	{
		const ModelMeshAnimationTrack& track = animation.mTrackArray[i];

		// Write mesh id.
		if ( mFile.Write( &track.mMeshID, sizeof( _dword ) ) == _false )
			return _false;

		// Write animation format.
		if ( mFile.Write( &track.mFormatFlag, sizeof( _dword ) ) == _false )
			return _false;

		// Write vertex number.
		if ( mFile.Write( &track.mVertexNumber, sizeof( _dword ) ) == _false )
			return _false;

		// Write keyframe number.
		_dword keyframenumber = track.mKeyframeArray.Number( );
		if ( mFile.Write( &keyframenumber, sizeof( _dword ) ) == _false )
			return _false;

		// Write each keyframe.
		for ( _dword j = 0; j < keyframenumber; j ++ )
		{
			const ModelMeshAnimationKeyframe& keyframe = track.mKeyframeArray[j];

			// Write keyframe time.
			if ( mFile.Write( &keyframe.mTime, sizeof( _float ) ) == _false )
				return _false;

			// Write keyframe vertex data.
			if ( mFile.Write( keyframe.mVertexBuffer.mBuffer, keyframe.mVertexBuffer.mLength ) == _false )
				return _false;
		}
	}

	_dword chunkoffest2 = mFile.GetOffset( );

	// Move file pointer to chunk begin.
	if ( mFile.SeekFromBegin( chunkoffset1 - sizeof( _dword ) ) == _false )
		return _false;

	// Write chunk size.
	chunksize = chunkoffest2 - chunkoffset1;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	// Move file pointer to chunk end.
	if ( mFile.SeekFromBegin( chunkoffest2 ) == _false )
		return _false;

	return _false;
}

_bool ModelSaver::SaveChunk( const ModelSkeleton& skeleton )
{
	// Write chunk id.
	_dword chunkid = ModelFileChunk::_CHUNKID_SKL;
	if ( mFile.Write( &chunkid, sizeof( _dword ) ) == _false )
		return _false;

	// Skip chunk Size now.
	_dword chunksize = 0;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	_dword chunkoffset1 = mFile.GetOffset( );

	// Write checksum.
	_dword checksum = 0;
	if ( mFile.Write( &checksum, sizeof( _dword ) ) == _false )
		return _false;

	// Write bone number.
	_dword bonenumber = skeleton.mBoneArray.Number( );
	if ( mFile.Write( &bonenumber, sizeof( _dword ) ) == _false )
		return _false;

	// Write bone number.
	_dword dummynumber = skeleton.mDummyArray.Number( );
	if ( mFile.Write( &dummynumber, sizeof( _dword ) ) == _false )
		return _false;

	if ( bonenumber > 0 )
	{
		// Write each bone.
		for ( _dword i = 0; i < skeleton.mBoneArray.Number( ); i ++ )
		{
			const ModelBone& bone = skeleton.mBoneArray[i];

			// Write bone name length.
			_dword namelength = bone.mBoneName.Length( );
			if ( mFile.Write( &namelength, sizeof( _dword ) ) == _false )
				return _false;

			// Write bone name.
			if ( mFile.Write( (const _char*) bone.mBoneName, bone.mBoneName.SizeOfBytes( ) ) == _false )
				return _false;

			// Write parent id.
			if ( mFile.Write( &bone.mParentID, sizeof( _dword ) ) == _false )
				return _false;

			// Write relative translation.
			if ( mFile.Write( &bone.mRelativeTranslation, sizeof( Vector3 ) ) == _false )
				return _false;

			// Write relative rotation.
			if ( mFile.Write( &bone.mRelativeRotation, sizeof( Quaternion ) ) == _false )
				return _false;

			// Write local translation.
			if ( mFile.Write( &bone.mLocalTranslation, sizeof( Vector3 ) ) == _false )
				return _false;

			// Write local rotation.
			if ( mFile.Write( &bone.mLocalRotation, sizeof( Quaternion ) ) == _false )
				return _false;
		}

		// Write each dummy.
		for ( _dword j = 0; j < skeleton.mDummyArray.Number( ); j ++ )
		{
			const ModelDummy& dummy = skeleton.mDummyArray[j];

			// Write dummy name length.
			_dword namelength = dummy.mDummyName.Length( );
			if ( mFile.Write( &namelength, sizeof( _dword ) ) == _false )
				return _false;

			// Write dummy name.
			if ( mFile.Write( (const _char*) dummy.mDummyName, dummy.mDummyName.SizeOfBytes( ) ) == _false )
				return _false;

			// Write parent id.
			if ( mFile.Write( &dummy.mParentID, sizeof( _dword ) ) == _false )
				return _false;

			// Write relative translation.
			if ( mFile.Write( &dummy.mRelativeTranslation, sizeof( Vector3 ) ) == _false )
				return _false;

			// Write relative rotation.
			if ( mFile.Write( &dummy.mRelativeRotation, sizeof( Quaternion ) ) == _false )
				return _false;
		}
	}

	_dword chunkoffest2 = mFile.GetOffset( );

	// Move file pointer to chunk begin.
	if ( mFile.SeekFromBegin( chunkoffset1 - sizeof( _dword ) ) == _false )
		return _false;

	// Write chunk size.
	chunksize = chunkoffest2 - chunkoffset1;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	// Move file pointer to chunk end.
	if ( mFile.SeekFromBegin( chunkoffest2 ) == _false )
		return _false;

	return _true;
}

_bool ModelSaver::SaveChunk( const ModelSkeletonAnimation& animation )
{
	// Write chunk id.
	_dword chunkid = ModelFileChunk::_CHUNKID_SAN;
	if ( mFile.Write( &chunkid, sizeof( _dword ) ) == _false )
		return _false;

	// Skip chunk Size now.
	_dword chunksize = 0;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	_dword chunkoffset1 = mFile.GetOffset( );

	// Write checksum.
	_dword checksum = 0;
	if ( mFile.Write( &checksum, sizeof( _dword ) ) == _false )
		return _false;

	// Write animation duration.
	if ( mFile.Write( &animation.mDuration, sizeof( _float ) ) == _false )
		return _false;

	// Write track number.
	_dword tracknumber = animation.mTrackArray.Number( );
	if ( mFile.Write( &tracknumber, sizeof( _dword ) ) == _false )
		return _false;

	// Write each track.
	for ( _dword i = 0; i < tracknumber; i ++ )
	{
		const ModelSkeletonAnimationTrack& track = animation.mTrackArray[i];

		// Write bone id.
		if ( mFile.Write( &track.mBoneID, sizeof( _dword ) ) == _false )
			return _false;

		// Write keyframe number.
		_dword keyframenumber= track.mKeyframeArray.Number( );
		if ( mFile.Write( &keyframenumber, sizeof( _dword ) ) == _false )
			return _false;

		// Write keyframes.
		if ( mFile.Write( track.mKeyframeArray.GetArrayBuffer( ), track.mKeyframeArray.GetArrayBufferLength( ) ) == _false )
			return _false;
	}

	_dword chunkoffest2 = mFile.GetOffset( );

	// Move file pointer to chunk begin.
	if ( mFile.SeekFromBegin( chunkoffset1 - sizeof( _dword ) ) == _false )
		return _false;

	// Write chunk size.
	chunksize = chunkoffest2 - chunkoffset1;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	// Move file pointer to chunk end.
	if ( mFile.SeekFromBegin( chunkoffest2 ) == _false )
		return _false;

	return _true;
}

_bool ModelSaver::SaveChunk( const ModelTimeTag& timetag )
{
	// Write chunk id.
	_dword chunkid = ModelFileChunk::_CHUNKID_TAG;
	if ( mFile.Write( &chunkid, sizeof( _dword ) ) == _false )
		return _false;

	// Skip chunk Size now.
	_dword chunksize = 0;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	_dword chunkoffset1 = mFile.GetOffset( );

	// Write tag time.
	if ( mFile.Write( &timetag.mTagTime, sizeof( _float ) ) == _false )
		return _false;

	// Write tag name length.
	_dword namelength = timetag.mTagName.Length( );
	if ( mFile.Write( &namelength, sizeof( _dword ) ) == _false )
		return _false;

	// Write tag name.
	if ( mFile.Write( (const _char*) timetag.mTagName, timetag.mTagName.SizeOfBytes( ) ) == _false )
		return _false;

	_dword chunkoffest2 = mFile.GetOffset( );

	// Move file pointer to chunk begin.
	if ( mFile.SeekFromBegin( chunkoffset1 - sizeof( _dword ) ) == _false )
		return _false;

	// Write chunk size.
	chunksize = chunkoffest2 - chunkoffset1;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	// Move file pointer to chunk end.
	if ( mFile.SeekFromBegin( chunkoffest2 ) == _false )
		return _false;

	return _true;
}

_bool ModelSaver::SaveChunk( const ModelMarker& marker )
{
	// Write chunk id.
	_dword chunkid = ModelFileChunk::_CHUNKID_MRK;
	if ( mFile.Write( &chunkid, sizeof( _dword ) ) == _false )
		return _false;

	// Skip chunk Size now.
	_dword chunksize = 0;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	_dword chunkoffset1 = mFile.GetOffset( );

	String name = marker.mMarkerName;

	if ( marker.mMarkerHandle != 0 )
	{
		name += L"[handle]";
		name += Long( marker.mMarkerHandle ).ToString( );
	}

	// Write marker name length.
	_dword namelength = name.Length( );
	if ( mFile.Write( &namelength, sizeof( _dword ) ) == _false )
		return _false;

	// Write marker name.
	if ( mFile.Write( (const _char*) name, name.SizeOfBytes( ) ) == _false )
		return _false;

	// Write marker translation.
	if ( mFile.Write( &marker.mTranslation, sizeof( Vector3 ) ) == _false )
		return _false;

	// Write marker rotation.
	if ( mFile.Write( &marker.mRotation, sizeof( Quaternion ) ) == _false )
		return _false;

	_dword chunkoffest2 = mFile.GetOffset( );

	// Move file pointer to chunk begin.
	if ( mFile.SeekFromBegin( chunkoffset1 - sizeof( _dword ) ) == _false )
		return _false;

	// Write chunk size.
	chunksize = chunkoffest2 - chunkoffset1;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	// Move file pointer to chunk end.
	if ( mFile.SeekFromBegin( chunkoffest2 ) == _false )
		return _false;

	return _true;
}

_bool ModelSaver::SaveChunk( const ModelCamera& camera )
{
	// Write chunk id.
	_dword chunkid = ModelFileChunk::_CHUNKID_FOV;
	if ( mFile.Write( &chunkid, sizeof( _dword ) ) == _false )
		return _false;

	// Skip chunk Size now.
	_dword chunksize = 0;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	_dword chunkoffset1 = mFile.GetOffset( );

	// Write camera name length.
	_dword namelength = camera.mCameraName.Length( );
	if ( mFile.Write( &namelength, sizeof( _dword ) ) == _false )
		return _false;

	// Write camera name.
	if ( mFile.Write( (const _char*) camera.mCameraName, camera.mCameraName.SizeOfBytes( ) ) == _false )
		return _false;

	// Write camera position.
	if ( mFile.Write( &camera.mCamera.mPosition, sizeof( Vector3 ) ) == _false )
		return _false;

	// Write camera look.
	if ( mFile.Write( &camera.mCamera.mLook, sizeof( Vector3 ) ) == _false )
		return _false;

	// Write camera up.
	if ( mFile.Write( &camera.mCamera.mUp, sizeof( Vector3 ) ) == _false )
		return _false;

	// Write camera fov.
	_float fov = camera.mCamera.GetFOVDegree( );
	if ( mFile.Write( &fov, sizeof( _float ) ) == _false )
		return _false;

	// Write reserved data.
	if ( mFile.Write( &camera.mReserved, sizeof( camera.mReserved ) ) == _false )
		return _false;

	_dword chunkoffest2 = mFile.GetOffset( );

	// Move file pointer to chunk begin.
	if ( mFile.SeekFromBegin( chunkoffset1 - sizeof( _dword ) ) == _false )
		return _false;

	// Write chunk size.
	chunksize = chunkoffest2 - chunkoffset1;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	// Move file pointer to chunk end.
	if ( mFile.SeekFromBegin( chunkoffest2 ) == _false )
		return _false;

	return _true;
}

_bool ModelSaver::SaveChunk( const ModelModifier& modifier )
{
	// Write chunk id.
	_dword chunkid = ModelFileChunk::_CHUNKID_MDF;
	if ( mFile.Write( &chunkid, sizeof( _dword ) ) == _false )
		return _false;

	// Skip chunk Size now.
	_dword chunksize = 0;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	_dword chunkoffset1 = mFile.GetOffset( );

	// Write translation modifier.
	if ( mFile.Write( &modifier.mTranslation, sizeof( Vector3 ) ) == _false )
		return _false;

	// Write scaling modifier.
	if ( mFile.Write( &modifier.mScaling, sizeof( Vector3 ) ) == _false )
		return _false;

	// Write magic value
	if ( mFile.Write( &modifier.mSpeedMagicValue, sizeof( _dword ) ) == _false )
		return _false;

	// Write speed scaling
	if ( mFile.Write( &modifier.mSpeedScaling, sizeof( _float ) ) == _false )
		return _false;

	// Write reserved data.
	if ( mFile.Write( modifier.mReserved, sizeof( modifier.mReserved ) ) == _false )
		return _false;

	_dword chunkoffest2 = mFile.GetOffset( );

	// Move file pointer to chunk begin.
	if ( mFile.SeekFromBegin( chunkoffset1 - sizeof( _dword ) ) == _false )
		return _false;

	// Write chunk size.
	chunksize = chunkoffest2 - chunkoffset1;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	// Move file pointer to chunk end.
	if ( mFile.SeekFromBegin( chunkoffest2 ) == _false )
		return _false;

	return _true;
}

_bool ModelSaver::SaveChunk( const ModelAlteration& alteration )
{
	// Write chunk id.
	_dword chunkid = ModelFileChunk::_CHUNKID_ALT;
	if ( mFile.Write( &chunkid, sizeof( _dword ) ) == _false )
		return _false;

	// Skip chunk Size now.
	_dword chunksize = 0;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	_dword chunkoffset1 = mFile.GetOffset( );

	// Write alteration key frame time.
	if ( mFile.Write( &alteration.mTime, sizeof( _float ) ) == _false )
		return _false;

	// Write source bone id.
	if ( mFile.Write( &alteration.mSourceBoneID, sizeof( _dword ) ) == _false )
		return _false;

	// Write target bone id.
	if ( mFile.Write( &alteration.mTargetBoneID, sizeof( _dword ) ) == _false )
		return _false;

	// Write reserved data.
	if ( mFile.Write( &alteration.mReserved, sizeof( alteration.mReserved ) ) == _false )
		return _false;

	_dword chunkoffest2 = mFile.GetOffset( );

	// Move file pointer to chunk begin.
	if ( mFile.SeekFromBegin( chunkoffset1 - sizeof( _dword ) ) == _false )
		return _false;

	// Write chunk size.
	chunksize = chunkoffest2 - chunkoffset1;
	if ( mFile.Write( &chunksize, sizeof( _dword ) ) == _false )
		return _false;

	// Move file pointer to chunk end.
	if ( mFile.SeekFromBegin( chunkoffest2 ) == _false )
		return _false;

	return _true;
}