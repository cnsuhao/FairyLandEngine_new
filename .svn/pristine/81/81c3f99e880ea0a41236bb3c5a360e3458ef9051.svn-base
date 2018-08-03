//============================================================================
// IMesh.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IMesh
//----------------------------------------------------------------------------

class IMesh
{
public:
	virtual StringPtr				GetName( ) const = 0;
	virtual const AxisAlignedBox&	GetBoundBox( ) const = 0;

	virtual _dword					GetModelNumber( ) const = 0;
	virtual StringPtr				GetModelName( _dword modelindex ) const = 0;
	virtual const AxisAlignedBox&	GetModelBoundBox( _dword modelindex ) const = 0;
	virtual ITexture*				GetModelTexture( _dword modelindex, _dword textureindex ) const = 0;
	virtual Geometry&				GetModelGeometry( _dword modelindex ) = 0;

	virtual _dword					GetMarkerNumber( ) const = 0;
	virtual StringPtr				GetMarkerName( _dword markerindex ) const = 0;
	virtual _dword					GetMarkerHandle( _dword markerindex ) const = 0;
	virtual const Vector3&			GetMarkerTranslation( _dword markerindex ) const = 0;
	virtual const Quaternion&		GetMarkerRotation( _dword markerindex ) const = 0;
	virtual Matrix4					GetMarkerTransform( _dword markerindex ) const = 0;

	virtual _dword					GetCameraNumber( ) const = 0;
	virtual StringPtr				GetCameraName( _dword cameraindex ) const = 0;
	virtual const Camera&			GetCamera( _dword cameraindex ) const = 0;

	virtual _dword					SearchModel( StringPtr modelname ) const = 0;
	virtual _dword					SearchMarker( StringPtr markername ) const = 0;
	virtual _dword					SearchCamera( StringPtr cameraname ) const = 0;

	virtual _bool					RemoveModel( _dword modelindex ) = 0;
	virtual _bool					RemoveMarker( _dword markerindex ) = 0;
	virtual _bool					RemoveCamera( _dword cameraindex ) = 0;

	virtual _void					SetAllModelUserData( _dword userdata ) = 0;
	virtual _void					SetModelUserData( _dword modelindex, _dword userdata ) = 0;
	virtual _dword					GetModelUserData( _dword modelindex ) const = 0;

	virtual _void					SetAllModelUserString( StringPtr string ) = 0;
	virtual _void					SetModelUserString( _dword modelindex, StringPtr string ) = 0;
	virtual StringPtr				GetModelUserString( _dword modelindex ) = 0;

	virtual _void					SetModelChangeListener( _bool* listener ) = 0;

	virtual _void					SetUpdateInterval( _dword interval ) = 0;
	virtual _dword					GetUpdateInterval( ) const = 0;

	virtual _void					SetUserData( _dword userdata ) = 0;
	virtual _dword					GetUserData( ) const = 0;

	virtual _void					Update( _bool forceupdate = _true ) = 0;
};

};