//============================================================================
// VertexAnimation.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// VertexAnimation
//----------------------------------------------------------------------------

class VertexAnimation : public IVertexAnimation, public Animation
{
private:
	struct VertexKeyframe
	{
		_float	mTime;
		Vector3	mPosition;
		Vector3	mNormal;
		_dword	mDiffuse;
		_dword	mSpecular;

		operator _float ( ) const
			{ return mTime; }
	};

	typedef Array< VertexKeyframe > KeyframeArray;

	IMesh*			mMesh;
	_dword			mKeyframeFormat;
	_dword			mLastKeyframe1;
	_dword			mLastKeyframe2;
	KeyframeArray	mKeyframeArray;

	VertexKeyframe& AddKeyframe( _float time );
	_void GetKeyframe( _float time, VertexKeyframe& keyframe1, VertexKeyframe& keyframe2 );
	_void UpdateMesh( _float current );

public:
	VertexAnimation( StringPtr name, _dword format, _float duration );
	~VertexAnimation( );

	_void AttachMesh( IMesh* mesh );

	VertexAnimation* Clone( ) const;

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

	virtual _bool		SetPositionKeyframe( _float time, const Vector3& position );
	virtual _bool		SetNormalKeyframe( _float time, const Vector3& normal );
	virtual _bool		SetDiffuseKeyframe( _float time, _dword diffuse );
	virtual _bool		SetSpecularKeyframe( _float time, _dword specular );
};

};