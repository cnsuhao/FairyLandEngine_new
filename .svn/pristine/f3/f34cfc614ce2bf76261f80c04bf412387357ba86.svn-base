//============================================================================
// SkeletonAnimation.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// SkeletonAnimation Implementation
//----------------------------------------------------------------------------

SkeletonAnimation::SkeletonAnimation( StringPtr name, _bool careevent ) : Animation( name ), mCareEvent( careevent )
{
	mSkeleton				= _null;
	mTrackDataArray			= _null;
	mAlterationArray		= _null;

	mTrackFilter			= _null;

	mAffectTransform		= _false;
	mCareEvent				= careevent;

	mLastTranslation		= Vector3::cOrigin;
	mLastRotation			= Quaternion::cIdentity;
	mAccumTranslation		= Vector3::cOrigin;
	mAccumRotation			= Quaternion::cIdentity;

	mSavedLastTranslation	= Vector3::cOrigin;
	mSavedLastRotation		= Quaternion::cIdentity;
	mSavedAccumTranslation	= Vector3::cOrigin;
	mSavedAccumRotation		= Quaternion::cIdentity;

	mTotalTransform			= Matrix4::cIdentity;
	mDeltaTransform			= Matrix4::cIdentity;

	IncreaseRefCount( );
}

SkeletonAnimation::~SkeletonAnimation( )
{
	GetModelFactory( ).ReleaseSkeleton( mSkeleton );

	if ( mTrackDataArray != _null )
	{
		if ( mTrackDataArray->DecreaseRefCount( ) == 0 )
			delete mTrackDataArray;
	}

	if ( mAlterationArray != _null )
	{
		if ( mAlterationArray->DecreaseRefCount( ) == 0 )
			delete mAlterationArray;
	}

	if ( mTrackFilter != _null )
		delete[] mTrackFilter;
}

_void SkeletonAnimation::GetKeyframe( const TrackData& track, TrackInstance& trackinstance, _float time, Vector3& translation, Quaternion& rotation )
{
	_long i = 0, j = 0;

	// Search from backward.
	if ( mBackward )
		time = mDuration - time;

	if ( trackinstance.mLastKeyframe1 >= track.Number( ) || trackinstance.mLastKeyframe2 >= track.Number( ) )
	{
		// Get keyframe from array using binary search.
		BinarySearch< Keyframe, _float > binarysearch;
		binarysearch.SearchAscending( track.GetArrayBuffer( ), time, track.Number( ), i, j );
	}
	else
	{
		i = trackinstance.mLastKeyframe1;
		j = trackinstance.mLastKeyframe2;

		// Try to fast adjust time to keyframe.
		if ( time < track[i].mTime )
		{
			i = 0;
			j = 1;
		}
		else if ( time > track[j].mTime )
		{
			i ++;
			j ++;
		}

		// Still cant get right keyframe, use binary search.
		if ( time < track[i].mTime || time > track[j].mTime )
		{
			BinarySearch< Keyframe, _float > binarysearch;
			binarysearch.SearchAscending( track.GetArrayBuffer( ), time, track.Number( ), i, j );
		}
	}

	const Keyframe& frame1 = track[i];
	const Keyframe& frame2 = track[j];

	if ( frame1.mTime == frame2.mTime )
	{
		translation	= frame1.mTranslation;
		rotation	= frame1.mRotation;
	}
	else
	{
		_float factor = ( time - frame1.mTime ) / ( frame2.mTime - frame1.mTime );

		// Linear interpolation bone translation.
		translation = Vector3::Lerp( frame1.mTranslation, frame2.mTranslation, factor );

		// Spherical linear interpolation bone roation use precomputed parameters.
		rotation = Quaternion::Slerp( frame1.mRotation, frame2.mRotation,
			frame1.mPrecomputeDot, frame1.mPrecomputeTheta, frame1.mPrecomputeRecipsqrt, factor );
	}

	trackinstance.mLastKeyframe1 = i;
	trackinstance.mLastKeyframe2 = j;
}

_bool SkeletonAnimation::LoadAnimation( ISkeleton* skeleton, BinFile& modelfile, _bool onlyroot )
{
	ModelLoader loader( modelfile );

	if ( skeleton == _null )
		skeleton = mSkeleton;

	if ( loader.IsFormatValid( ) == _false )
		return _false;

	// Read chunks in the file.
	ModelFileChunk chunk;
	while ( loader.LoadChunk( chunk ) )
	{
		if ( chunk.mChunkID == ModelFileChunk::_CHUNKID_SAN )
		{
			ModelSkeletonAnimation animation;

			if ( loader.LoadChunk( animation ) == _false )
				return _false;

			if ( animation.mDuration <= 0.0f )
				return _false;

			// Load only root track.
			if ( onlyroot )
			{
				ModelSkeletonAnimationTrack rootrack;

				for ( _dword i = 0; i < animation.mTrackArray.Number( ); i ++ )
				{
					if ( animation.mTrackArray[i].mBoneID == 0 )
					{
						rootrack.mBoneID		= 0;
						rootrack.mKeyframeArray = animation.mTrackArray[i].mKeyframeArray;
						break;
					}
				}

				if ( rootrack.mKeyframeArray.Number( ) == 0 )
					return _false;

				animation.mTrackArray.Clear( 1, 20 );
				animation.mTrackArray.Append( rootrack );
			}

			if ( skeleton != _null )
				mTrackInstanceArray.Clear( Math::Min( skeleton->GetBoneNumber( ), animation.mTrackArray.Number( ) ), 20 );
			else
				mTrackInstanceArray.Clear( animation.mTrackArray.Number( ), 20 );

			mDuration = animation.mDuration;

			// Only load track of bone if it existing in skeleton.
			for ( _dword i = 0; i < animation.mTrackArray.Number( ); i ++ )
			{
				const ModelSkeletonAnimationTrack& animationtrack = animation.mTrackArray[i];

				if ( animationtrack.mKeyframeArray.Number( ) < 2 )
					continue;

				if ( skeleton != _null )
				{
					// Skip other bones.
					if ( animationtrack.mBoneID >= skeleton->GetBoneNumber( ) )
						continue;
				}

				if ( mTrackDataArray == _null )
				{
					mTrackDataArray = new TrackDataArray;
					mTrackDataArray->Clear( mTrackInstanceArray.Size( ), 20 );
				}

				// Put the track data into array.
				mTrackDataArray->Append( TrackData( ) );

				// Now fill the track.
				TrackData& track = (TrackData&) mTrackDataArray->GetTailElement( );

				// Set bone ID for the track.
				track.mBoneID = animationtrack.mBoneID;

				track.Clear( animationtrack.mKeyframeArray.Number( ), 20 );

				// Fill each keyframe into track.
				for ( _dword t = 0; t < animationtrack.mKeyframeArray.Number( ); t ++ )
				{
					Keyframe newkeyframe;

					newkeyframe.mTime			= animationtrack.mKeyframeArray[t].mTime;
					newkeyframe.mTranslation	= animationtrack.mKeyframeArray[t].mTranslation;
					newkeyframe.mRotation		= animationtrack.mKeyframeArray[t].mRotation;

					// Precompute parameter.
					if ( t < animationtrack.mKeyframeArray.Number( ) - 1 )
					{
						Quaternion::PrecomputedSlerpParameters( newkeyframe.mRotation, animationtrack.mKeyframeArray[t + 1].mRotation,
							newkeyframe.mPrecomputeDot, newkeyframe.mPrecomputeTheta, newkeyframe.mPrecomputeRecipsqrt );
					}
					// Just set all parameter to 0.0f.
					else
					{
						newkeyframe.mPrecomputeDot			= 0.0f;
						newkeyframe.mPrecomputeTheta		= 0.0f;
						newkeyframe.mPrecomputeRecipsqrt	= 0.0f;
					}

					// Put the track into keyframe.
					track.Append( newkeyframe );
				}

				TrackInstance trackinstance;
				trackinstance.mLastKeyframe1 = -1;
				trackinstance.mLastKeyframe2 = -1;
				mTrackInstanceArray.Append( trackinstance );
			}
		}
		else if ( chunk.mChunkID == ModelFileChunk::_CHUNKID_ALT )
		{
			ModelAlteration modelalteration;

			if ( loader.LoadChunk( modelalteration ) == _false )
				return _false;

			if ( skeleton != _null )
			{
				if ( modelalteration.mSourceBoneID >= skeleton->GetBoneNumber( ) )
					continue;

				if ( modelalteration.mTargetBoneID >= skeleton->GetBoneNumber( ) )
					continue;
			}

			if ( mAlterationArray == _null )
				mAlterationArray = new AlterationArray;

			Alteration alteration;
			alteration.mTime			= modelalteration.mTime;
			alteration.mSourceBoneID	= modelalteration.mSourceBoneID;
			alteration.mTargetBoneID	= modelalteration.mTargetBoneID;

			mAlterationArray->Append( alteration );
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

_void SkeletonAnimation::AttachSkeleton( ISkeleton* skeleton )
{
	if ( mSkeleton == skeleton )
		return;

	GetModelFactory( ).ReleaseSkeleton( mSkeleton );
	mSkeleton = GetModelFactory( ).CloneSkeleton( skeleton, _true );
}

_void SkeletonAnimation::AppendAnimation( SkeletonAnimation* animation )
{
	if ( animation->mTrackDataArray == _null )
		return;

	if ( mTrackDataArray == _null )
		mTrackDataArray = new TrackDataArray;

	Vector3 rootoffest = Vector3::cOrigin;

	if ( mTrackDataArray->Number( ) > 0 )
	{
		const TrackData& roottrack = (*mTrackDataArray)[0];

		if ( roottrack.Number( ) > 0 )
			rootoffest = roottrack[ roottrack.Number( ) - 1 ].mTranslation;
	}

	// Copy each track of animation.
	for ( _dword i = 0; i < animation->mTrackDataArray->Number( ); i ++ )
	{
		TrackData newtrack = (*animation->mTrackDataArray)[i];

		// Skip empty track.
		if ( newtrack.Number( ) == 0 )
			continue;

		// Update each keyframe of the track.
		for ( _dword j = 0; j < newtrack.Number( ); j ++ )
		{
			newtrack[j].mTime += mDuration;

			if ( i == 0 )
				newtrack[j].mTranslation += rootoffest;
		}

		_bool combined = _false;

		// Combine the new track into track array.
		for ( _dword j = 0; j < mTrackDataArray->Number( ); j ++ )
		{
			TrackData& oldtrack = (*mTrackDataArray)[j];

			// This existing track has same bone id, we can combine with new track.
			if ( oldtrack.mBoneID == newtrack.mBoneID )
			{
				oldtrack.AppendArrayFromBuffer( newtrack.GetArrayBuffer( ), newtrack.Number( ), _true );

				combined = _true;
				break;
			}
		}

		// Not combined yet, we put the new track into track array.
		if ( combined == _false )
		{
			// This track is not start from 0.0, caused by previous animation have no this track.
			if ( newtrack[0].mTime > 0.0f )
			{
				Keyframe beginframe = newtrack[0];
				beginframe.mTime = 0.0f;

				// Put a begin key frame into track.
				newtrack.Insert( beginframe, 0 );
			}

			mTrackDataArray->Append( newtrack );
		}
	}

	// Append animation event.
	for ( _dword i = 0; i < animation->mEventArray.Number( ); i ++ )
	{
		EventPair eventpair = animation->mEventArray[i];
		eventpair.mObject1 += mDuration;

		mEventArray.Append( eventpair );
	}

	// Create track instance for new track data.
	for ( _dword i = mTrackInstanceArray.Number( ); i < mTrackDataArray->Number( ); i ++ )
	{
		TrackInstance trackinstance;
		trackinstance.mLastKeyframe1 = -1;
		trackinstance.mLastKeyframe2 = -1;
		mTrackInstanceArray.Append( trackinstance );
	}

	mDuration += animation->mDuration;
}

_void SkeletonAnimation::UpdateSkeleton( _float last, _float current, _float weight, _bool onlyroot )
{
	if ( mSkeleton == _null )
		return;

	if ( mTrackDataArray == _null )
		return;

	Skeleton* skeleton = (Skeleton*) mSkeleton;

	if ( last < 0.0f )
		last = 0.0f;

	if ( current < 0.0f )
		current = 0.0f;

	if ( mCareEvent )
		Animation::Update( last, current );

	Matrix4		accumtransform	= Matrix4::cIdentity;
	Quaternion	accumrotation	= mAccumRotation;

	if ( mCircular == _true )
	{
		if ( mAffectTransform && mDuration > 0.1f )
		{
			for ( _float temp = last + mDuration; temp < current; temp += mDuration )
				accumtransform = mTotalTransform * accumtransform;
		}

		if ( last >= mDuration )
			last = Math::Fmod( last, mDuration );

		if ( current >= mDuration )
			current = Math::Fmod( current, mDuration );
	}
	else
	{
		if ( last > mDuration )
			last = mDuration;

		if ( current > mDuration )
			current = mDuration;
	}

	Vector3 deltatranslation = Vector3::cOrigin;
	Quaternion deltarotation = Quaternion::cIdentity;

	for ( _dword i = 0; i < mTrackDataArray->Number( ); i ++ )
	{
		const TrackData& track = (*mTrackDataArray)[i];

		if ( track.mBoneID >= skeleton->GetBoneNumber( ) )
			continue;

		if ( onlyroot && track.mBoneID != 0 )
			continue;

		// We dont need this bone.
		if ( mTrackFilter != _null && mTrackFilter[i] == 0 )
			continue;

		Skeleton::Bone& bone = skeleton->GetBone( track.mBoneID );

		Vector3 currenttranslation; Quaternion currentrotation;
		GetKeyframe( track, mTrackInstanceArray[i], current, currenttranslation, currentrotation );

		// Update root bone.
		if ( bone.mParentID == -1 && mAffectTransform == _true )
		{
			// Reach animation end, accumulate rotation.
			if ( current == mDuration || current < last )
			{
				Quaternion rotation1 = (*mTrackDataArray)[0][0].mRotation;
				Quaternion rotation2 = skeleton->GetBone( 0 ).mRelativeRotation;

				// Accumulate rotation for delta transform.
				accumrotation = rotation1 / rotation2;
			}

			// Compute delta transform by current - last.
			if ( current > last )
			{
				deltatranslation	= currenttranslation - mLastTranslation;
				deltarotation		= currentrotation / mLastRotation;
			}
			// Reach animation end, compute delta transform by ( current - begin ) + ( end - last ).
			else
			{
				// Notice, current maybe equal last, because of last maybe behind of current exactly one loop.

				_dword k1 = 0, k2 = track.Number( ) - 1;

				Vector3 translation1 = track[k1].mTranslation;
				Quaternion rotation1 = track[k1].mRotation;

				Vector3 translation2 = track[k2].mTranslation;
				Quaternion rotation2 = track[k2].mRotation;

				deltatranslation = ( currenttranslation - translation1 ) + ( translation2 - mLastTranslation );
				deltarotation	 = ( currentrotation / rotation1 ) * ( rotation2 / mLastRotation );
			}

			mLastTranslation = currenttranslation;
			mLastRotation	 = currentrotation;
		}
		// Update child bone.
		else
		{
			// Blend to previous animation with weight.
			currenttranslation	= Vector3::Lerp( bone.mRelativeTranslation, currenttranslation, weight );
			currentrotation		= Quaternion::Slerp( bone.mRelativeRotation, currentrotation, weight );

			bone.mRelativeTranslation	= currenttranslation;
			bone.mRelativeRotation		= currentrotation;
		}
	}

	// Check alter
	if ( mAlterationArray != _null )
	{
		for ( _dword i = 0; i < mAlterationArray->Number( ); i ++ )
		{
			const Alteration& alteration = (*mAlterationArray)[i];

			if ( alteration.mTime < last )
				continue;

			if ( alteration.mTime > current )
				continue;

			skeleton->AlterBone( alteration.mSourceBoneID, alteration.mTargetBoneID );
		}
	}

	// Update delta transform when needed.
	if ( mAffectTransform )
	{
		mDeltaTransform = accumtransform;

		mDeltaTransform *= Matrix4( ).Translation( -mAccumTranslation );
		mDeltaTransform *= Matrix4( ).Rotation( -( mAccumRotation * deltarotation ).Normalize( ) );
		mDeltaTransform *= Matrix4( ).Translation( deltatranslation );
		mDeltaTransform *= Matrix4( ).Rotation( mAccumRotation.Normalize( ) );
		mDeltaTransform *= Matrix4( ).Translation( mAccumTranslation );

		mAccumRotation = accumrotation * deltarotation;
	}

	mCurrent = current;
}

SkeletonAnimation* SkeletonAnimation::Clone( ) const
{
	// Clone an new skeleton animation.
	SkeletonAnimation* newanimation = new SkeletonAnimation( mName, mCareEvent );

	// Reference skeleton.
	newanimation->mSkeleton = GetModelFactory( ).CloneSkeleton( mSkeleton, _true );

	// Set animation duration.
	newanimation->mDuration	= mDuration;

	// Set animation circular type.
	newanimation->mCircular = mCircular;

	// Clone animation track data.
	newanimation->mTrackDataArray = mTrackDataArray;

	if ( mTrackDataArray != _null )
		mTrackDataArray->IncreaseRefCount( );

	// Clone alteration data.
	newanimation->mAlterationArray = mAlterationArray;

	if ( mAlterationArray != _null )
		mAlterationArray->IncreaseRefCount( );

	// Clone event data.
	for ( _dword i = 0; i < mEventArray.Number( ); i ++ )
		newanimation->mEventArray.Append( mEventArray[i] );

	// Copy animation tracks.
	newanimation->mTrackInstanceArray.Clear( mTrackInstanceArray.Number( ), 20 );
	for ( _dword i = 0; i < mTrackInstanceArray.Number( ); i ++ )
	{
		TrackInstance newtrack;
		newtrack.mLastKeyframe1 = -1;
		newtrack.mLastKeyframe2 = -1;

		newanimation->mTrackInstanceArray.Append( newtrack );
	}

	// Set affect transform attribute
	newanimation->mAffectTransform	= mAffectTransform;

	// Copy root transform.
	newanimation->mLastTranslation	= mLastTranslation;
	newanimation->mLastRotation		= mLastRotation;
	newanimation->mAccumTranslation	= mAccumTranslation;
	newanimation->mAccumRotation	= mAccumRotation;
	newanimation->mTotalTransform	= mTotalTransform;
	newanimation->mDeltaTransform	= mDeltaTransform;

	return newanimation;
}

StringPtr SkeletonAnimation::GetName( ) const
{
	return mName;
}

_float SkeletonAnimation::GetDuration( ) const
{
	return mDuration;
}

_void SkeletonAnimation::SetCurrent( _float current )
{
	UpdateSkeleton( mCurrent, current, 1.0f, _false );
}

_float SkeletonAnimation::GetCurrent( ) const
{
	return mCurrent;
}

_void SkeletonAnimation::SetSpeed( _float speed )
{
	if ( speed < 0.0f )
		speed = 0.0f;

	mSpeed = speed;
}

_float SkeletonAnimation::GetSpeed( ) const
{
	return mSpeed;
}

_void SkeletonAnimation::SetCircular( _bool circular )
{
	mCircular = circular;
}

_bool SkeletonAnimation::GetCircular( ) const
{
	return mCircular;
}

_void SkeletonAnimation::SetBackward( _bool backward )
{
	mBackward = backward;
}

_bool SkeletonAnimation::GetBackward( ) const
{
	return mBackward;
}

_void SkeletonAnimation::SetAutoClearEvent( _bool autoclear )
{
	mAutoClearEvent = autoclear;
}

_bool SkeletonAnimation::GetAutoClearEvent( ) const
{
	return mAutoClearEvent;
}

_void SkeletonAnimation::SetUserData( _dword userdata )
{
	mUserData1 = userdata;
}

_dword SkeletonAnimation::GetUserData( ) const
{
	return mUserData1;
}

_void SkeletonAnimation::SetUserData2( _dword userdata )
{
	mUserData2 = userdata;
}

_dword SkeletonAnimation::GetUserData2( ) const
{
	return mUserData2;
}

_void SkeletonAnimation::SetAffectTransform( _bool affecttransform )
{
	mAffectTransform = affecttransform;

	if ( affecttransform )
	{
		Reset( );

		// Update one time, used to get total transform.
		UpdateSkeleton( 0.0f, mDuration, 1.0f, _true );

		// Get total transform from delta transform.
		mTotalTransform = mDeltaTransform;

		Reset( );
	}
}

_bool SkeletonAnimation::GetAffectTransform( ) const
{
	return mAffectTransform;
}

_dword SkeletonAnimation::GetTotalEventNumber( ) const
{
	return Animation::GetTotalEventNumber( );
}

_float SkeletonAnimation::GetTotalEventTime( _dword index ) const
{
	return Animation::GetTotalEventTime( index );
}

StringPtr SkeletonAnimation::GetTotalEventName( _dword index ) const
{
	return Animation::GetTotalEventName( index );
}

_dword SkeletonAnimation::GetHappenedEventNumber( ) const
{
	return Animation::GetHappenedEventNumber( );
}

_float SkeletonAnimation::GetHappenedEventTime( _dword index ) const
{
	return Animation::GetHappenedEventTime( index );
}

StringPtr SkeletonAnimation::GetHappenedEventName( _dword index ) const
{
	return Animation::GetHappenedEventName( index );
}

_bool SkeletonAnimation::InsertEvent( _float time, StringPtr name )
{
	return Animation::InsertEvent( time, name );
}

_bool SkeletonAnimation::RemoveEvent( _dword index )
{
	return Animation::RemoveEvent( index );
}

_bool SkeletonAnimation::ClearEvent( )
{
	return Animation::ClearEvent( );
}

_void SkeletonAnimation::ClearEventHappened( )
{
	Animation::ClearEventHappened( );
}

_void SkeletonAnimation::Update( _dword elapse, _float weight, _bool onlyroot )
{
	if ( elapse == 0 )
		return;

	UpdateSkeleton( mCurrent, mCurrent + elapse / 1000.0f * mSpeed, weight, onlyroot );
}

_void SkeletonAnimation::Reset( )
{
	if ( mSkeleton == _null )
		return;

	if ( mTrackDataArray == _null )
		return;

	mCurrent = 0.0f;

	if ( mAffectTransform == _false )
		return;

	if ( mSkeleton->GetBoneNumber( ) == 0 )
		return;

	if ( mTrackDataArray->Number( ) == 0 )
		return;

	Vector3 translation1 = (*mTrackDataArray)[0][0].mTranslation;
	Quaternion rotation1 = (*mTrackDataArray)[0][0].mRotation;

	Vector3 translation2 = ( (Skeleton*) mSkeleton )->GetBone( 0 ).mInitTranslation;
	Quaternion rotation2 = ( (Skeleton*) mSkeleton )->GetBone( 0 ).mInitRotation;

	mLastTranslation	= translation2;
	mLastRotation		= rotation2;
	mAccumTranslation	= translation1;
	mAccumRotation		= Quaternion::cIdentity;

	for ( _dword i = 0; i < mTrackInstanceArray.Number( ); i ++ )
	{
		mTrackInstanceArray[i].mLastKeyframe1 = 0;
		mTrackInstanceArray[i].mLastKeyframe2 = 1;
	}
}

ISkeleton* SkeletonAnimation::GetAttachedSkeleton( ) const
{
	return mSkeleton;
}

_dword SkeletonAnimation::GetTrackNumber( ) const
{
	return mTrackInstanceArray.Number( );
}

_dword SkeletonAnimation::GetTrackBoneID( _dword trackindex ) const
{
	if ( mTrackDataArray == _null )
		return 0;

	return (*mTrackDataArray)[ trackindex ].mBoneID;
}

_dword SkeletonAnimation::GetTrackKeyframeNumber( _dword trackindex ) const
{
	if ( mTrackDataArray == _null )
		return 0;

	return (*mTrackDataArray)[ trackindex ].Number( );
}

_dword SkeletonAnimation::GetTrackCurrentKeyframe1( _dword trackindex ) const
{
	return mTrackInstanceArray[ trackindex ].mLastKeyframe1;
}

_dword SkeletonAnimation::GetTrackCurrentKeyframe2( _dword trackindex ) const
{
	return mTrackInstanceArray[ trackindex ].mLastKeyframe2;
}

_float SkeletonAnimation::GetTrackKeyframeTime( _dword trackindex, _dword keyframeindex ) const
{
	if ( mTrackDataArray == _null )
		return 0.0f;

	return (*mTrackDataArray)[ trackindex ][ keyframeindex ].mTime;
}

const Vector3& SkeletonAnimation::GetTrackKeyframeTranslation( _dword trackindex, _dword keyframeindex ) const
{
	if ( mTrackDataArray == _null )
		return Vector3::cOrigin;

	return (*mTrackDataArray)[ trackindex ][ keyframeindex ].mTranslation;
}

const Quaternion& SkeletonAnimation::GetTrackKeyframeRotation( _dword trackindex, _dword keyframeindex ) const
{
	if ( mTrackDataArray == _null )
		return Quaternion::cIdentity;

	return (*mTrackDataArray)[ trackindex ][ keyframeindex ].mRotation;
}

const Matrix4& SkeletonAnimation::GetTotalTransform( ) const
{
	return mTotalTransform;
}

const Matrix4& SkeletonAnimation::GetDeltaTransform( ) const
{
	return mDeltaTransform;
}

_void SkeletonAnimation::SaveCurrentStatus( )
{
	mSavedLastTranslation	= mLastTranslation;
	mSavedLastRotation		= mLastRotation;
	mSavedAccumTranslation	= mAccumTranslation;
	mSavedAccumRotation		= mAccumRotation;
}

_void SkeletonAnimation::LoadCurrentStatus( )
{
	mLastTranslation	= mSavedLastTranslation;
	mLastRotation		= mSavedLastRotation;
	mAccumTranslation	= mSavedAccumTranslation;
	mAccumRotation		= mSavedAccumRotation;
}

_void SkeletonAnimation::ActiveAlteration( )
{
	if ( mAlterationArray == _null )
		return;

	for ( _dword i = 0; i < mAlterationArray->Number( ); i ++ )
	{
		const Alteration& alteration = (*mAlterationArray)[i];

		( (Skeleton*) mSkeleton )->AlterBone( alteration.mSourceBoneID, alteration.mTargetBoneID );
	}
}

_void SkeletonAnimation::EnableAllTrack( )
{
	if ( mSkeleton == _null || mSkeleton->GetBoneNumber( ) == 0 )
		return;

	_dword bonenumber = mSkeleton->GetBoneNumber( );

	if ( mTrackFilter == _null )
		mTrackFilter = new _byte[ bonenumber ];

	for ( _dword i = 0; i < bonenumber; i ++ )
		mTrackFilter[i] = _true;
}

_void SkeletonAnimation::DisableAllTrack( )
{
	if ( mSkeleton == _null || mSkeleton->GetBoneNumber( ) == 0 )
		return;

	_dword bonenumber = mSkeleton->GetBoneNumber( );

	if ( mTrackFilter == _null )
		mTrackFilter = new _byte[ bonenumber ];

	for ( _dword i = 0; i < bonenumber; i ++ )
		mTrackFilter[i] = _false;
}

_void SkeletonAnimation::EnableChildTrack( _dword boneid )
{
	if ( mSkeleton == _null || boneid >= mSkeleton->GetBoneNumber( ) )
		return;

	_dword bonenumber = mSkeleton->GetBoneNumber( );

	if ( mTrackFilter == _null )
		mTrackFilter = new _byte[ bonenumber ];

	mTrackFilter[ boneid ] = _true;

	for ( _dword i = boneid + 1; i < bonenumber; i ++ )
	{
		if ( mTrackFilter[ mSkeleton->GetBoneParent( i ) ] == _true )
			mTrackFilter[i] = _true;
	}
}

_void SkeletonAnimation::DisableChildTrack( _dword boneid )
{
	if ( mSkeleton == _null || boneid >= mSkeleton->GetBoneNumber( ) )
		return;

	_dword bonenumber = mSkeleton->GetBoneNumber( );

	if ( mTrackFilter == _null )
		mTrackFilter = new _byte[ bonenumber ];

	mTrackFilter[ boneid ] = _false;

	for ( _dword i = boneid + 1; i < bonenumber; i ++ )
	{
		if ( mTrackFilter[ mSkeleton->GetBoneParent( i ) ] == _false )
			mTrackFilter[i] = _false;
	}
}