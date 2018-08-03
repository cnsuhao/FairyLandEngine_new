//============================================================================
// Mesh.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Mesh
//----------------------------------------------------------------------------

class Mesh : public IMesh, public ReferencedObject
{
public:
	enum
	{
		_UPDATE_NOTUPDATED		= 0,
		_UPDATE_INPUTREADY		= 1,
		_UPDATE_PROCESSING		= 2,
		_UPDATE_OUTPUTREADY		= 3,
		_UPDATE_FINISHUPDATE	= 4,
	};

	struct Model
	{
		String			mModelName;
		String			mUserString;
		_dword			mUserData;
		Geometry		mGeometry;
	};

	typedef Array< Model >			ModelArray;
	typedef Array< ModelMarker >	MarkerArray;
	typedef Array< ModelCamera >	CameraArray;

private:
	ISkeleton*		mSkeleton;
	String			mName;
	AxisAlignedBox	mBoundBox;

	ModelArray		mModelArray;
	MarkerArray		mMarkerArray;
	CameraArray		mCameraArray;

	_dword			mOriginalModelGroup;
	_dword			mOriginalModelNumber;
	_dword			mMaxInfluenceNumber;

	_dword			mUpdateInterval;
	_dword			mUpdateTickcount;
	_dword			mUpdateFlag;
	_dword			mUserData;

	_bool*			mModelChangeListener;

public:
	Mesh( StringPtr name );
	~Mesh( );

	_bool LoadModel( BinFile& modelfile, _dword modelgroup );
	_void AttachSkeleton( ISkeleton* skeleton );
	_void CombineModel( Mesh* mesh );
	_void CombineMarker( Mesh* mesh );

	_void SetupUpdateRef( );
	_void SwitchBuffer( );
	_void UpdateBuffer( );

	IMesh* Clone( _dword modelgroup, _bool cloneall ) const;

	virtual StringPtr				GetName( ) const;
	virtual const AxisAlignedBox&	GetBoundBox( ) const;

	virtual _dword					GetModelNumber( ) const;
	virtual StringPtr				GetModelName( _dword modelindex ) const;
	virtual const AxisAlignedBox&	GetModelBoundBox( _dword modelindex ) const;
	virtual ITexture*				GetModelTexture( _dword modelindex, _dword textureindex ) const;
	virtual Geometry&				GetModelGeometry( _dword modelindex );

	virtual _dword					GetMarkerNumber( ) const;
	virtual StringPtr				GetMarkerName( _dword markerindex ) const;
	virtual _dword					GetMarkerHandle( _dword markerindex ) const;
	virtual const Vector3&			GetMarkerTranslation( _dword markerindex ) const;
	virtual const Quaternion&		GetMarkerRotation( _dword markerindex ) const;
	virtual Matrix4					GetMarkerTransform( _dword markerindex ) const;

	virtual _dword					GetCameraNumber( ) const;
	virtual StringPtr				GetCameraName( _dword cameraindex ) const;
	virtual const Camera&			GetCamera( _dword cameraindex ) const;

	virtual _dword					SearchModel( StringPtr modelname ) const;
	virtual _dword					SearchMarker( StringPtr markername ) const;
	virtual _dword					SearchCamera( StringPtr cameraname ) const;

	virtual _bool					RemoveModel( _dword modelindex );
	virtual _bool					RemoveMarker( _dword markerindex );
	virtual _bool					RemoveCamera( _dword cameraindex );

	virtual _void					SetAllModelUserData( _dword userdata );
	virtual _void					SetModelUserData( _dword modelindex, _dword userdata );
	virtual _dword					GetModelUserData( _dword modelindex ) const;

	virtual _void					SetAllModelUserString( StringPtr string );
	virtual _void					SetModelUserString( _dword modelindex, StringPtr string );
	virtual StringPtr				GetModelUserString( _dword modelindex );

	virtual _void					SetModelChangeListener( _bool* listener );

	virtual _void					SetUpdateInterval( _dword interval );
	virtual _dword					GetUpdateInterval( ) const;

	virtual _void					SetUserData( _dword userdata );
	virtual _dword					GetUserData( ) const;

	virtual _void					Update( _bool forceupdate );
};

};