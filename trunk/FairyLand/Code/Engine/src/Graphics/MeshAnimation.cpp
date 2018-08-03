//============================================================================
// MeshAnimation.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// MeshAnimation Implementation
//----------------------------------------------------------------------------

MeshAnimation::MeshAnimation( StringPtr name ) : Animation( name )
{
	mMesh			= _null;
	mTrackDataArray = _null;

	IncreaseRefCount( );
}

MeshAnimation::~MeshAnimation( )
{
	GetModelFactory( ).ReleaseMesh( mMesh );

	if ( mTrackDataArray != _null )
	{
		if ( mTrackDataArray->DecreaseRefCount( ) == 0 )
		{
			for ( _dword i = 0; i < mTrackDataArray->Number( ); i ++ )
				delete[] (*mTrackDataArray)[i].mKeyframeBuffer;

			delete mTrackDataArray;
		}
	}
}

_void MeshAnimation::GetKeyframe( const TrackData& track, TrackInstance& trackinstance, _float time, ModelMeshAnimationKeyframe& keyframe1, ModelMeshAnimationKeyframe& keyframe2 )
{
	_long i = 0, j = 0;

	if ( trackinstance.mLastKeyframe1 >= track.mKeyframeArray.Number( ) || trackinstance.mLastKeyframe2 >= track.mKeyframeArray.Number( ) )
	{
		// Get keyframe from array using binary search.
		BinarySearch< ModelMeshAnimationKeyframe, _float > binarysearch;
		binarysearch.SearchAscending( track.mKeyframeArray.GetArrayBuffer( ), time, track.mKeyframeArray.Number( ), i, j );
	}
	else
	{
		i = trackinstance.mLastKeyframe1;
		j = trackinstance.mLastKeyframe2;

		// Try to fast adjust time to keyframe.
		if ( time < track.mKeyframeArray[i].mTime )
		{
			i = 0;
			j = 1;
		}
		else if ( time > track.mKeyframeArray[j].mTime )
		{
			i ++;
			j ++;
		}

		// Still cant get right keyframe, ust binary search.
		if ( time < track.mKeyframeArray[i].mTime || time > track.mKeyframeArray[j].mTime )
		{
			BinarySearch< ModelMeshAnimationKeyframe, _float > binarysearch;
			binarysearch.SearchAscending( track.mKeyframeArray.GetArrayBuffer( ), time, track.mKeyframeArray.Number( ), i, j );
		}
	}

	keyframe1 = track.mKeyframeArray[i];
	keyframe2 = track.mKeyframeArray[j];

	trackinstance.mLastKeyframe1 = i;
	trackinstance.mLastKeyframe2 = j;
}

_void MeshAnimation::UpdateMesh( _float current )
{
	if ( mMesh == _null )
		return;

	if ( mTrackDataArray == _null )
		return;

	if ( current < 0.0f )
		current = 0.0f;

	Animation::Update( mCurrent, current );

	if ( mCircular == _true )
	{
		if ( current >= mDuration )
			current = Math::Fmod( current, mDuration );
	}
	else
	{
		if ( current > mDuration )
			current = mDuration;
	}

	for ( _dword i = 0; i < mTrackDataArray->Number( ); i ++ )
	{
		const TrackData& track = (*mTrackDataArray)[i];

		if ( track.mMeshID >= mMesh->GetModelNumber( ) )
			continue;

		Geometry& geometry = mMesh->GetModelGeometry( track.mMeshID );

		// Check vertex number.
		if ( track.mVertexNumber != geometry.mVertexNumber )
			continue;

		// Check if the animation format is vaild for geometry format.
		if ( ModelHelper::CheckVertexFormatVSAnimationFormat( geometry.mFormatFlag, track.mFormatFlag ) == _false )
			continue;

		_dword geotexnumber = ModelHelper::GetTexcoordNumberFromFormat( geometry.mFormatFlag );

		ModelMeshAnimationKeyframe keyframe1;
		ModelMeshAnimationKeyframe keyframe2;

		// Get animation keyframe.
		GetKeyframe( track, mTrackInstanceArray[i], current, keyframe1, keyframe2 );

		_float factor = ( current - keyframe1.mTime ) / ( keyframe2.mTime - keyframe1.mTime );

		_byte* vertexbuffer1 = keyframe1.mVertexBuffer.mBuffer;
		_byte* vertexbuffer2 = keyframe2.mVertexBuffer.mBuffer;

		// Lock vertex buffer, ready to update.
		_byte* vertexbuffer = (_byte*) geometry.mVertexBuffer->Lock( geometry.mStartVertex * geometry.mVertexSize, geometry.mVertexNumber * geometry.mVertexSize );

		if ( vertexbuffer == _null )
		{
			geometry.mVertexBuffer->Unlock( );
			continue;
		}

		// Update each vertex data.
		for ( _dword i = 0; i < geometry.mVertexNumber; i ++ )
		{
			// Update position.
			if ( track.mFormatFlag & Geometry::_FORMATFLAG_POSITION )
			{
				*( (Vector3*) vertexbuffer ) = Vector3::Lerp(
					*( (Vector3*) vertexbuffer1 ), *( (Vector3*) vertexbuffer2 ), factor );

				vertexbuffer1	+= sizeof( Vector3 );
				vertexbuffer2	+= sizeof( Vector3 );
				vertexbuffer	+= sizeof( Vector3 );
			}
			// Skip position, vertex buffer will sure have position.
			else
			{
				vertexbuffer += sizeof( Vector3 );
			}

			// Update normal.
			if ( track.mFormatFlag & Geometry::_FORMATFLAG_NORMAL )
			{
				*( (Vector3*) vertexbuffer ) = Vector3::Lerp(
					*( (Vector3*) vertexbuffer1 ), *( (Vector3*) vertexbuffer2 ), factor );

				vertexbuffer1	+= sizeof( Vector3 );
				vertexbuffer2	+= sizeof( Vector3 );
				vertexbuffer	+= sizeof( Vector3 );
			}
			// Skip normal.
			else if ( geometry.mFormatFlag & Geometry::_FORMATFLAG_NORMAL )
			{
				vertexbuffer += sizeof( Vector3 );
			}

			// Update diffuse.
			if ( track.mFormatFlag & Geometry::_FORMATFLAG_DIFFUSE )
			{
				*( (_dword*) vertexbuffer ) = Color::Lerp(
					Color( *( (_dword*) vertexbuffer1 ) ), Color( *( (_dword*) vertexbuffer2 ) ), factor );

				vertexbuffer1	+= sizeof( _dword );
				vertexbuffer2	+= sizeof( _dword );
				vertexbuffer	+= sizeof( _dword );
			}
			// Skip diffuse.
			else if ( geometry.mFormatFlag & Geometry::_FORMATFLAG_DIFFUSE )
			{
				vertexbuffer += sizeof( _dword );
			}

			// Update specular.
			if ( track.mFormatFlag & Geometry::_FORMATFLAG_SPECULAR )
			{
				*( (_dword*) vertexbuffer ) = Color::Lerp(
					Color( *( (_dword*) vertexbuffer1 ) ), Color( *( (_dword*) vertexbuffer2 ) ), factor );

				vertexbuffer1	+= sizeof( _dword );
				vertexbuffer2	+= sizeof( _dword );
				vertexbuffer	+= sizeof( _dword );
			}
			// Skip specular.
			else if ( geometry.mFormatFlag & Geometry::_FORMATFLAG_SPECULAR )
			{
				vertexbuffer += sizeof( _dword );
			}

			// Update texcoord1.
			if ( track.mFormatFlag & Geometry::_FORMATFLAG_TEXCOORD1 )
			{
				*( (Vector2*) vertexbuffer ) = Vector2::Lerp(
					*( (Vector2*) vertexbuffer1 ), *( (Vector2*) vertexbuffer2 ), factor );

				vertexbuffer1	+= sizeof( Vector2 );
				vertexbuffer2	+= sizeof( Vector2 );
				vertexbuffer	+= sizeof( Vector2 );
			}
			// Skip texcoord1.
			else if ( geotexnumber >= 1 )
			{
				vertexbuffer += sizeof( Vector2 );
			}

			// Update texcoord2.
			if ( track.mFormatFlag & Geometry::_FORMATFLAG_TEXCOORD2 )
			{
				*( (Vector2*) vertexbuffer ) = Vector2::Lerp(
					*( (Vector2*) vertexbuffer1 ), *( (Vector2*) vertexbuffer2 ), factor );

				vertexbuffer1	+= sizeof( Vector2 );
				vertexbuffer2	+= sizeof( Vector2 );
				vertexbuffer	+= sizeof( Vector2 );
			}
			// Skip texcoord2.
			else if ( geotexnumber >= 2 )
			{
				vertexbuffer += sizeof( Vector2 );
			}

			// Update texcoord3.
			if ( track.mFormatFlag & Geometry::_FORMATFLAG_TEXCOORD3 )
			{
				*( (Vector2*) vertexbuffer ) = Vector2::Lerp(
					*( (Vector2*) vertexbuffer1 ), *( (Vector2*) vertexbuffer2 ), factor );

				vertexbuffer1	+= sizeof( Vector2 );
				vertexbuffer2	+= sizeof( Vector2 );
				vertexbuffer	+= sizeof( Vector2 );
			}
			// Skip texcoord3.
			else if ( geotexnumber >= 3 )
			{
				vertexbuffer += sizeof( Vector2 );
			}

			// Update texcoord4.
			if ( track.mFormatFlag & Geometry::_FORMATFLAG_TEXCOORD4 )
			{
				*( (Vector2*) vertexbuffer ) = Vector2::Lerp(
					*( (Vector2*) vertexbuffer1 ), *( (Vector2*) vertexbuffer2 ), factor );

				vertexbuffer1	+= sizeof( Vector2 );
				vertexbuffer2	+= sizeof( Vector2 );
				vertexbuffer	+= sizeof( Vector2 );
			}
			// Skip texcoord4.
			else if ( geotexnumber >= 4 )
			{
				vertexbuffer += sizeof( Vector2 );
			}

			// Skip binormal.
			if ( geometry.mFormatFlag & Geometry::_FORMATFLAG_BINORMAL )
				vertexbuffer += sizeof( Vector3 );

			// Skip tangent.
			if ( geometry.mFormatFlag & Geometry::_FORMATFLAG_TANGENT )
				vertexbuffer += sizeof( Vector3 );
		}

		// Update finished, unlock vertex buffer.
		geometry.mVertexBuffer->Unlock( );
	}

	mCurrent = current;
}

_bool MeshAnimation::LoadAnimation( IMesh* mesh, BinFile& modelfile )
{
	ModelLoader loader( modelfile );

	if ( mesh == _null )
		mesh = mMesh;

	if ( loader.IsFormatValid( ) == _false )
		return _false;

	// Read chunks in the file.
	ModelFileChunk chunk;
	while ( loader.LoadChunk( chunk ) )
	{
		if ( chunk.mChunkID == ModelFileChunk::_CHUNKID_MAN )
		{
			ModelMeshAnimation animation;

			if ( loader.LoadChunk( animation ) == _false )
				return _false;

			if ( animation.mDuration <= 0.0f )
				return _false;

			mDuration = animation.mDuration;

			// Load each track.
			for ( _dword i = 0; i < animation.mTrackArray.Number( ); i ++ )
			{
				// Invalid animation.
				if ( animation.mTrackArray[i].mKeyframeArray.Number( ) == 0 )
					continue;

				_dword vertexsize = animation.mTrackArray[i].mKeyframeArray[0].mVertexBuffer.mLength,
					keyframenumber = animation.mTrackArray[i].mKeyframeArray.Number( );

				// Invalid animation.
				if ( vertexsize == 0 || keyframenumber == 0 )
					continue;

				if ( mesh != _null )
				{
					// Invalid mesh id.
					if ( animation.mTrackArray[i].mMeshID >= mesh->GetModelNumber( ) )
						continue;

					Geometry& geometry = mesh->GetModelGeometry( animation.mTrackArray[i].mMeshID );

					// Invalid vertex number.
					if ( animation.mTrackArray[i].mVertexNumber != geometry.mVertexNumber )
						continue;

					// Invalid animation format.
					if ( ModelHelper::CheckVertexFormatVSAnimationFormat( geometry.mFormatFlag, animation.mTrackArray[i].mFormatFlag ) == _false )
						continue;
				}

				if ( mTrackDataArray == _null )
					mTrackDataArray = new TrackDataArray;

				TrackData track;
				track.mMeshID			= animation.mTrackArray[i].mMeshID;
				track.mFormatFlag		= animation.mTrackArray[i].mFormatFlag;
				track.mVertexNumber		= animation.mTrackArray[i].mVertexNumber;
				track.mKeyframeBuffer	= new _byte[ vertexsize * keyframenumber ];

				// First allocate memory for keyframe array.
				track.mKeyframeArray.Clear( keyframenumber, 0 );

				_byte* vertexbuffer = track.mKeyframeBuffer;

				// Copy each keyframe.
				for ( _dword j = 0; j < animation.mTrackArray[i].mKeyframeArray.Number( ); j ++ )
				{
					ModelMeshAnimationKeyframe keyframe;

					// Copy vertex buffer in this keyframe.
					Memory::MemCpy( vertexbuffer, animation.mTrackArray[i].mKeyframeArray[j].mVertexBuffer.mBuffer, vertexsize );

					keyframe.mTime					= animation.mTrackArray[i].mKeyframeArray[j].mTime;
					keyframe.mVertexBuffer.mLength	= vertexsize;
					keyframe.mVertexBuffer.mBuffer	= vertexbuffer;

					track.mKeyframeArray.Append( keyframe );

					vertexbuffer += vertexsize;
				}

				mTrackDataArray->Append( track );

				TrackInstance trackinstance;
				trackinstance.mLastKeyframe1 = -1;
				trackinstance.mLastKeyframe2 = -1;
				mTrackInstanceArray.Append( trackinstance );
			}
		}
		else if ( chunk.mChunkID == ModelFileChunk::_CHUNKID_TAG )
		{
			ModelTimeTag timetag;

			if ( loader.LoadChunk( timetag ) == _false )
				return _false;

			InsertEvent( timetag.mTagTime, timetag.mTagName );
		}
		else
		{
			if ( loader.SkipChunk( chunk ) == _false )
				return _false;
		}
	}

	if ( mTrackDataArray == _null )
		return _false;

	if ( mTrackDataArray->Number( ) == 0 )
		return _false;

	return _true;
}

_void MeshAnimation::AttachMesh( IMesh* mesh )
{
	if ( mMesh == mesh )
		return;

	GetModelFactory( ).ReleaseMesh( mMesh );
	mMesh = GetModelFactory( ).CloneMesh( mesh, _true, _false, _null );
}

MeshAnimation* MeshAnimation::Clone( ) const
{
	// Clone an new mesh animation.
	MeshAnimation* newanimation = new MeshAnimation( mName );

	// Reference mesh.
	newanimation->mMesh = GetModelFactory( ).CloneMesh( mMesh, _true, _false, _null );

	// Set animation duration.
	newanimation->mDuration	= mDuration;

	// Set animation circular type.
	newanimation->mCircular = mCircular;

	// Clone animation track data.
	newanimation->mTrackDataArray = mTrackDataArray;

	if ( mTrackDataArray != _null )
		mTrackDataArray->IncreaseRefCount( );

	// Copy animation tracks.
	for ( _dword i = 0; i < mTrackInstanceArray.Number( ); i ++ )
	{
		TrackInstance newtrack;
		newtrack.mLastKeyframe1 = -1;
		newtrack.mLastKeyframe2 = -1;

		newanimation->mTrackInstanceArray.Append( newtrack );
	}

	return newanimation;
}

StringPtr MeshAnimation::GetName( ) const
{
	return mName;
}

_float MeshAnimation::GetDuration( ) const
{
	return mDuration;
}

_void MeshAnimation::SetCurrent( _float current )
{
	UpdateMesh( current );
}

_float MeshAnimation::GetCurrent( ) const
{
	return mCurrent;
}

_void MeshAnimation::SetSpeed( _float speed )
{
	mSpeed = speed;
}

_float MeshAnimation::GetSpeed( ) const
{
	return mSpeed;
}

_void MeshAnimation::SetCircular( _bool circular )
{
	mCircular = circular;
}

_bool MeshAnimation::GetCircular( ) const
{
	return mCircular;
}

_void MeshAnimation::SetBackward( _bool backward )
{
	mBackward = backward;
}

_bool MeshAnimation::GetBackward( ) const
{
	return mBackward;
}

_void MeshAnimation::SetAutoClearEvent( _bool autoclear )
{
	mAutoClearEvent = autoclear;
}

_bool MeshAnimation::GetAutoClearEvent( ) const
{
	return mAutoClearEvent;
}

_void MeshAnimation::SetUserData( _dword userdata )
{
	mUserData1 = userdata;
}

_dword MeshAnimation::GetUserData( ) const
{
	return mUserData1;
}

_void MeshAnimation::SetUserData2( _dword userdata )
{
	mUserData2 = userdata;
}

_dword MeshAnimation::GetUserData2( ) const
{
	return mUserData2;
}

_dword MeshAnimation::GetTotalEventNumber( ) const
{
	return Animation::GetTotalEventNumber( );
}

_float MeshAnimation::GetTotalEventTime( _dword index ) const
{
	return Animation::GetTotalEventTime( index );
}

StringPtr MeshAnimation::GetTotalEventName( _dword index ) const
{
	return Animation::GetTotalEventName( index );
}

_dword MeshAnimation::GetHappenedEventNumber( ) const
{
	return Animation::GetHappenedEventNumber( );
}

_float MeshAnimation::GetHappenedEventTime( _dword index ) const
{
	return Animation::GetHappenedEventTime( index );
}

StringPtr MeshAnimation::GetHappenedEventName( _dword index ) const
{
	return Animation::GetHappenedEventName( index );
}

_bool MeshAnimation::InsertEvent( _float time, StringPtr name )
{
	return Animation::InsertEvent( time, name );
}

_bool MeshAnimation::RemoveEvent( _dword index )
{
	return Animation::RemoveEvent( index );
}

_bool MeshAnimation::ClearEvent( )
{
	return Animation::ClearEvent( );
}

_void MeshAnimation::Update( _dword elapse, _float weight, _bool onlyroot )
{
	if ( elapse == 0 )
		return;

	UpdateMesh( mCurrent + elapse / 1000.0f * mSpeed );
}

_void MeshAnimation::Reset( )
{
	mCurrent = 0.0f;

	for ( _dword i = 0; i < mTrackInstanceArray.Number( ); i ++ )
	{
		mTrackInstanceArray[i].mLastKeyframe1 = 0;
		mTrackInstanceArray[i].mLastKeyframe2 = 1;
	}
}

IMesh* MeshAnimation::GetAttachedMesh( ) const
{
	return mMesh;
}