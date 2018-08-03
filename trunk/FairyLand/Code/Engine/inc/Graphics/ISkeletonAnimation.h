//============================================================================
// ISkeletonAnimation.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ISkeletonAnimation
//----------------------------------------------------------------------------

class ISkeletonAnimation : public IAnimation
{
public:
	virtual ISkeleton*			GetAttachedSkeleton( ) const = 0;

	virtual _void				SetAffectTransform( _bool affecttransform ) = 0;
	virtual _bool				GetAffectTransform( ) const = 0;

	virtual _dword				GetTrackNumber( ) const = 0;
	virtual _dword				GetTrackBoneID( _dword trackindex ) const = 0;
	virtual _dword				GetTrackKeyframeNumber( _dword trackindex ) const = 0;
	virtual _dword				GetTrackCurrentKeyframe1( _dword trackindex ) const = 0;
	virtual _dword				GetTrackCurrentKeyframe2( _dword trackindex ) const = 0;
	virtual _float				GetTrackKeyframeTime( _dword trackindex, _dword keyframeindex ) const = 0;
	virtual const Vector3&		GetTrackKeyframeTranslation( _dword trackindex, _dword keyframeindex ) const = 0;
	virtual const Quaternion&	GetTrackKeyframeRotation( _dword trackindex, _dword keyframeindex ) const = 0;

	virtual const Matrix4&		GetTotalTransform( ) const = 0;
	virtual const Matrix4&		GetDeltaTransform( ) const = 0;

	virtual _void				SaveCurrentStatus( ) = 0;
	virtual _void				LoadCurrentStatus( ) = 0;
	virtual _void				ActiveAlteration( ) = 0;

	virtual _void				EnableAllTrack( ) = 0;
	virtual _void				DisableAllTrack( ) = 0;
	virtual _void				EnableChildTrack( _dword boneid ) = 0;
	virtual _void				DisableChildTrack( _dword boneid ) = 0;

	virtual	_void				UpdateSkeleton( _float last, _float current, _float weight, _bool onlyroot ) = 0;
};

};