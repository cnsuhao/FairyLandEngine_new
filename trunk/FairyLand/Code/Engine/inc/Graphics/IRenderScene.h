//============================================================================
// IRenderScene.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IRenderScene
//----------------------------------------------------------------------------

class IRenderScene
{
public:
	typedef Array< Geometry >	GeometryArray;

	enum _PICK_TYPE
	{
		_PICK_MESH		= 1,
		_PICK_BOUNDBOX	= 2,
	};

	enum _PICK_FLAG
	{
		_PICK_ALL_ENTITY			= 0,

		_PICK_SAMEAS_USERDATA1		= 1,
		_PICK_NOTSAMEAS_USERDATA1	= 2,
		_PICK_LOGICOR_USERDATA1		= 3,
		_PICK_LOGICAND_USERDATA1	= 4,

		_PICK_SAMEAS_USERDATA2		= 5,
		_PICK_NOTSAMEAS_USERDATA2	= 6,
		_PICK_LOGICOR_USERDATA2		= 7,
		_PICK_LOGICAND_USERDATA2	= 8,

		_PICK_SAMEAS_PICKDATA		= 9,
		_PICK_NOTSAMEAS_PICKDATA	= 10,
		_PICK_LOGICOR_PICKDATA		= 11,
		_PICK_LOGICAND_PICKDATA		= 12,

		_PICK_PICKBOUNDBOX			= 0x80000000,
		_PICK_ONLYBOUNDBOX			= 0x40000000,
		_PICK_FORCEPICKMESH			= 0x20000000,
		_PICK_FORCEPICKBOUNDBOX		= 0x10000000,
		_PICK_MASK					= 0x0FFFFFFF,

		_PICK_DISABLE_PICKFLAG		= 0xFFFFFFFF,
	};

	virtual StringPtr		GetName( ) const = 0;
	virtual _dword			GetEntityNumber( ) const = 0;

	virtual _dword			GetFirstEntityID( ) const = 0;
	virtual _dword			GetNextEntityID( _dword previousid ) const = 0;

	virtual _dword			CreateEntityGroup( const AxisAlignedBox& boundbox, _dword userdata1 = 0, _dword userdata2 = 0 ) = 0;
	virtual _bool			RemoveEntityGroup( _dword groupid ) = 0;

	virtual _void			SetEntityGroupBoundBox( _dword groupid, const AxisAlignedBox& boundbox ) = 0;
	virtual AxisAlignedBox	GetEntityGroupBoundBox( _dword groupid ) const = 0;
	virtual _void			SetEntityGroupUserData1( _dword groupid, _dword userdata ) = 0;
	virtual _dword			GetEntityGroupUserData1( _dword groupid ) const = 0;
	virtual _void			SetEntityGroupUserData2( _dword groupid, _dword userdata ) = 0;
	virtual _dword			GetEntityGroupUserData2( _dword groupid ) const = 0;

	virtual _dword			CreateEntity( const Matrix4& transform, _dword userdata1 = 0, _dword userdata2 = 0, _dword groupid = 0 ) = 0;
	virtual _bool			RemoveEntity( _dword entityid ) = 0;

	virtual _dword			GetGeometryNumber( _dword entityid ) const = 0;
	virtual const Geometry&	GetGeometry( _dword entityid, _dword geometryindex ) const = 0;
	virtual	_void			SetGeometryTextureFactor( _dword entityid, _dword geometryindex, _dword factor ) = 0;
	virtual	_void			SetGeometryMaterialDiffuse( _dword entityid, _dword geometryindex, _dword factor ) = 0;
	virtual	_void			SetGeometryMaterialAmbient( _dword entityid, _dword geometryindex, _dword factor ) = 0;
	virtual	_void			SetGeometryMaterialSpecular( _dword entityid, _dword geometryindex, _dword factor ) = 0;
	virtual	_void			SetGeometryMaterialEmissive( _dword entityid, _dword geometryindex, _dword factor ) = 0;
	virtual	_void			SetGeometryMaterialPower( _dword entityid, _dword geometryindex, _float factor ) = 0;
	virtual	_void			SetGeometryDepthBias( _dword entityid, _dword geometryindex, _float factor ) = 0;

	virtual _bool			SetMaxGeometryNumber( _dword entityid, _dword number ) = 0;
	virtual _bool			AppendGeometry( _dword entityid, const Geometry& geometry ) = 0;
	virtual _bool			RemoveGeometry( _dword entityid, _dword geometryindex ) = 0;
	virtual _bool			ClearGeometry( _dword entityid ) = 0;

	virtual AxisAlignedBox	GetEntityBoundBox( _dword entityid ) const = 0;

	virtual _void			SetEntityTransform( _dword entityid, const Matrix4& transform ) = 0;
	virtual const Matrix4&	GetEntityTransform( _dword entityid ) const = 0;
	virtual _void			SetEntityUserData1( _dword entityid, _dword userdata ) = 0;
	virtual _dword			GetEntityUserData1( _dword entityid ) const = 0;
	virtual _void			SetEntityUserData2( _dword entityid, _dword userdata ) = 0;
	virtual _dword			GetEntityUserData2( _dword entityid ) const = 0;
	virtual _void			SetEntityPickType( _dword entityid, _dword picktype ) = 0;
	virtual _dword			GetEntityPickType( _dword entityid ) const = 0;
	virtual _void			SetEntityPickData( _dword entityid, _dword pickdata ) = 0;
	virtual _dword			GetEntityPickData( _dword entityid ) const = 0;
	virtual	_void			SetEntityRenderMethod( _dword entityid, IRenderMethod* rendermethod ) = 0;
	virtual	_bool			GetEntityVisible( _dword entityid ) const = 0;
	virtual	_void			SetEntityVisible( _dword entityid, _bool visible ) = 0;

	virtual _bool			Pick( const Ray& pickray, _float& distance, _dword pickflag = 0, _dword pickarg = 0 ) = 0;
	virtual _bool			Pick( const AxisAlignedBox& boundbox, _dword pickflag = 0, _dword pickarg = 0 ) = 0;
	virtual _dword			GetPickedEntityNumber( ) const = 0;
	virtual _dword			GetPickedEntityID( _dword pickindex ) const = 0;
	virtual _float			GetPickedEntityDistance( _dword pickindex ) const = 0;

	virtual _void			FrustumClip( const Frustum& viewfrustum ) = 0;
	virtual	_bool			IsEntityGroupInsideFrustum( _dword entitygroupid ) const = 0;
	virtual _bool			IsEntityInsideFrustum( _dword entityid ) const = 0;

	virtual _void			ClearScene( ) = 0;
	virtual	_void			RenderEntities( ) = 0;
};

};