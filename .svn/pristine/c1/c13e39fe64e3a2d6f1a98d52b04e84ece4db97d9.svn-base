//============================================================================
// MeshAnimation.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// MeshAnimation
//----------------------------------------------------------------------------

class MeshAnimation : public IMeshAnimation, public Animation
{
private:
	struct TrackData : public ModelMeshAnimationTrack
	{
		_byte*	mKeyframeBuffer;
	};

	struct TrackInstance
	{
		_dword	mLastKeyframe1;
		_dword	mLastKeyframe2;
	};

	struct TrackDataArray : public ReferencedObject, public Array< TrackData >
	{
		TrackDataArray( )
			{ IncreaseRefCount( ); }
	};

	typedef Array< TrackInstance > TrackInstanceArray;

	IMesh*				mMesh;
	TrackDataArray*		mTrackDataArray;

	TrackInstanceArray	mTrackInstanceArray;

	_void GetKeyframe( const TrackData& track, TrackInstance& trackinstance, _float time, ModelMeshAnimationKeyframe& keyframe1, ModelMeshAnimationKeyframe& keyframe2 );
	_void UpdateMesh( _float current );

public:
	MeshAnimation( StringPtr name );
	~MeshAnimation( );

	_bool LoadAnimation( IMesh* mesh, BinFile& modelfile );
	_void AttachMesh( IMesh* mesh );

	MeshAnimation* Clone( ) const;

	virtual StringPtr	GetName( ) const;
	virtual _float		GetDuration( ) const;

	virtual _void		SetCurrent( _float current );
	virtual _float		GetCurrent( ) const;
	virtual _void		SetSpeed( _float speed );
	virtual _float		GetSpeed( ) const;
	virtual _void		SetCircular( _bool circular );
	virtual _bool		GetCircular( ) const;
	virtual _void		SetBackward( _bool backward );
	virtual _bool		GetBackward( ) const;
	virtual _void		SetAutoClearEvent( _bool autoclear );
	virtual _bool		GetAutoClearEvent( ) const;
	virtual _void		SetUserData( _dword userdata );
	virtual _dword		GetUserData( ) const;
	virtual _void		SetUserData2( _dword userdata );
	virtual _dword		GetUserData2( ) const;

	virtual _dword		GetTotalEventNumber( ) const;
	virtual _float		GetTotalEventTime( _dword index ) const;
	virtual StringPtr	GetTotalEventName( _dword index ) const;

	virtual _dword		GetHappenedEventNumber( ) const;
	virtual _float		GetHappenedEventTime( _dword index ) const;
	virtual StringPtr	GetHappenedEventName( _dword index ) const;

	virtual _bool		InsertEvent( _float time, StringPtr name );
	virtual _bool		RemoveEvent( _dword index );
	virtual _bool		ClearEvent( );

	virtual _void		Update( _dword elapse, _float weight, _bool onlyroot );
	virtual _void		Reset( );

	virtual IMesh*		GetAttachedMesh( ) const;
};

};