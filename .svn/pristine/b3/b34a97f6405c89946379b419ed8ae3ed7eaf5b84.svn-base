//============================================================================
// ISkeleton.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

class ITagPoint;

//----------------------------------------------------------------------------
// ISkeleton
//----------------------------------------------------------------------------

class ISkeleton
{
public:
	virtual StringPtr			GetName( ) const = 0;

	virtual _dword				GetBoneNumber( ) const = 0;
	virtual _dword				GetBoneParent( _dword boneindex ) const = 0;
	virtual StringPtr			GetBoneName( _dword boneindex ) const = 0;
	virtual const Vector3&		GetBoneTranslation( _dword boneindex ) const = 0;
	virtual const Quaternion&	GetBoneRotation( _dword boneindex ) const = 0;
	virtual ITagPoint*			GetBoneTagPoint( _dword boneindex ) const = 0;

	virtual _dword				GetDummyNumber( ) const = 0;
	virtual _dword				GetDummyParent( _dword dummyindex ) const = 0;
	virtual StringPtr			GetDummyName( _dword dummyindex ) const = 0;
	virtual const Vector3&		GetDummyTranslation( _dword dummyindex ) const = 0;
	virtual const Quaternion&	GetDummyRotation( _dword dummyindex ) const = 0;
	virtual ITagPoint*			GetDummyTagPoint( _dword dummyindex ) const = 0;

	virtual _dword				GetCameraNumber( ) const = 0;
	virtual StringPtr			GetCameraName( _dword cameraindex ) const = 0;
	virtual const Camera&		GetCamera( _dword cameraindex ) const = 0;

	virtual _dword				SearchBone( StringPtr bonename ) const = 0;
	virtual _dword				SearchDummy( StringPtr dummyname ) const = 0;
	virtual _dword				SearchCamera( StringPtr cameraname ) const = 0;

	virtual ITagPoint*			CreateBoneTagPoint( _dword boneindex ) = 0;
	virtual ITagPoint*			CreateDummyTagPoint( _dword dummyindex ) = 0;

	virtual _bool				GetInfluence( _dword boneindex, Vector3& translation, Quaternion& rotation ) = 0;
	virtual _bool				SetInfluence( _dword boneindex, const Vector3& translation, const Quaternion& rotation ) = 0;
	virtual _void				ClearInfluence( ) = 0;

	virtual _void				SetUpdateInterval( _dword interval ) = 0;
	virtual _dword				GetUpdateInterval( ) const = 0;

	virtual _void				SetUserData( _dword userdata ) = 0;
	virtual _dword				GetUserData( ) const = 0;

	virtual _void				Update( _bool changed = _true ) = 0;
};

};