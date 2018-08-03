//============================================================================
// SkeletonAnimation.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// SkeletonAnimation
//----------------------------------------------------------------------------

class SkeletonAnimation : public ISkeletonAnimation, public Animation
{
private:
	struct Keyframe
	{
		_float		mTime;
		_float		mPrecomputeDot;
		_float		mPrecomputeTheta;
		_float		mPrecomputeRecipsqrt;
		Vector3		mTranslation;
		Quaternion	mRotation;

		operator _float ( ) const
			{ return mTime; }
	};

	struct TrackData : public Array< Keyframe >
	{
		_dword		mBoneID;
	};

	struct TrackInstance
	{
		_dword	mLastKeyframe1;
		_dword	mLastKeyframe2;
	};

	struct Alteration
	{
		_float		mTime;
		_dword		mSourceBoneID;
		_dword		mTargetBoneID;
	};

	struct TrackDataArray : public ReferencedObject, public Array< TrackData >
	{
		TrackDataArray( )
			{ IncreaseRefCount( ); }
	};

	struct AlterationArray : public ReferencedObject, public Array< Alteration >
	{
		AlterationArray( )
			{ IncreaseRefCount( ); }
	};

	typedef Array< TrackInstance > TrackInstanceArray;

	ISkeleton*			mSkeleton;
	TrackDataArray*		mTrackDataArray;
	AlterationArray*	mAlterationArray;

	_byte*				mTrackFilter;

	TrackInstanceArray	mTrackInstanceArray;
	_bool				mAffectTransform;
	_bool				mCareEvent;

	Vector3				mLastTranslation;
	Quaternion			mLastRotation;
	Vector3				mAccumTranslation;
	Quaternion			mAccumRotation;

	Vector3				mSavedLastTranslation;
	Quaternion			mSavedLastRotation;
	Vector3				mSavedAccumTranslation;
	Quaternion			mSavedAccumRotation;

	Matrix4				mTotalTransform;
	Matrix4				mDeltaTransform;

	_void GetKeyframe( const TrackData& track, TrackInstance& trackinstance, _float time, Vector3& translation, Quaternion& rotation );

public:
	SkeletonAnimation( StringPtr name, _bool careevent );
	~SkeletonAnimation( );

	_bool LoadAnimation( ISkeleton* skeleton, BinFile& modelfile, _bool onlyroot );
	_void AttachSkeleton( ISkeleton* skeleton );
	_void AppendAnimation( SkeletonAnimation* animation );

	_void UpdateSkeleton( _float last, _float current, _float weight, _bool onlyroot );

	SkeletonAnimation* Clone( ) const;

	virtual StringPtr			GetName( ) const;
	virtual _float				GetDuration( ) const;

	virtual _void				SetCurrent( _float current );
	virtual _float				GetCurrent( ) const;
	virtual _void				SetSpeed( _float speed );
	virtual _float				GetSpeed( ) const;
	virtual _void				SetCircular( _bool circular );
	virtual _bool				GetCircular( ) const;
	virtual _void				SetBackward( _bool backward );
	virtual _bool				GetBackward( ) const;
	virtual _void				SetAutoClearEvent( _bool autoclear );
	virtual _bool				GetAutoClearEvent( ) const;
	virtual _void				SetUserData( _dword userdata );
	virtual _dword				GetUserData( ) const;
	virtual _void				SetUserData2( _dword userdata );
	virtual _dword				GetUserData2( ) const;
	virtual _void				SetAffectTransform( _bool affecttransform );
	virtual _bool				GetAffectTransform( ) const;

	virtual _dword				GetTotalEventNumber( ) const;
	virtual _float				GetTotalEventTime( _dword index ) const;
	virtual StringPtr			GetTotalEventName( _dword index ) const;

	virtual _dword				GetHappenedEventNumber( ) const;
	virtual _float				GetHappenedEventTime( _dword index ) const;
	virtual StringPtr			GetHappenedEventName( _dword index ) const;
	virtual _void				ClearEventHappened( );

	virtual _bool				InsertEvent( _float time, StringPtr name );
	virtual _bool				RemoveEvent( _dword index );
	virtual _bool				ClearEvent( );

	virtual _void				Update( _dword elapse, _float weight, _bool onlyroot );
	virtual _void				Reset( );

	virtual ISkeleton*			GetAttachedSkeleton( ) const;

	virtual _dword				GetTrackNumber( ) const;
	virtual _dword				GetTrackBoneID( _dword trackindex ) const;
	virtual _dword				GetTrackKeyframeNumber( _dword trackindex ) const;
	virtual _dword				GetTrackCurrentKeyframe1( _dword trackindex ) const;
	virtual _dword				GetTrackCurrentKeyframe2( _dword trackindex ) const;
	virtual _float				GetTrackKeyframeTime( _dword trackindex, _dword keyframeindex ) const;
	virtual const Vector3&		GetTrackKeyframeTranslation( _dword trackindex, _dword keyframeindex ) const;
	virtual const Quaternion&	GetTrackKeyframeRotation( _dword trackindex, _dword keyframeindex ) const;

	virtual const Matrix4&		GetTotalTransform( ) const;
	virtual const Matrix4&		GetDeltaTransform( ) const;

	virtual _void				SaveCurrentStatus( );
	virtual _void				LoadCurrentStatus( );
	virtual _void				ActiveAlteration( );

	virtual _void				EnableAllTrack( );
	virtual _void				DisableAllTrack( );
	virtual _void				EnableChildTrack( _dword boneid );
	virtual _void				DisableChildTrack( _dword boneid );
};

};