//============================================================================
// RenderScene.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// RenderScene
//----------------------------------------------------------------------------

class RenderScene : public IRenderScene, public ReferencedObject
{
public:
	enum _SCENE_TYPE
	{
		_SCENE_FRUSTUM	= 1,
		_SCENE_QUADTREE	= 2,
		_SCENE_OCTTREE	= 3,
	};

protected:
	struct Entity
	{
		_dword			mEntityID;
		_dword			mGroupID;		
		Matrix4			mTransform;
		_dword			mUserData1;
		_dword			mUserData2;
		_dword			mPickType;
		_dword			mPickData;
		_dword			mInternal1;
		_dword			mInternal2;
		_bool			mInsideFrustum;
		_bool			mVisible;
		IRenderMethod*	mRenderMethod;
		GeometryArray	mGeometryArray;

		Entity( );
		~Entity( );
	};

	typedef Array< Entity* > EntityArray;

	struct EntityGroup
	{
		_dword			mGroupID;
		AxisAlignedBox	mBoundBox;
		_dword			mUserData1;
		_dword			mUserData2;
		_dword			mInternal1;
		_dword			mInternal2;
		_bool			mInsideFrustum;
		EntityArray		mEntityArray;

		EntityGroup( );
	};

	typedef Pool< Entity* >			EntityPool;
	typedef Pool< EntityGroup* >	EntityGroupPool;
	typedef Pair< _float, _dword >	PickedEntity;
	typedef Array< PickedEntity >	PickedArray;

	IRenderSet*		mRenderSet;
	_bool			mRefreshRenderSet;
	Frustum			mLastFrustum;
	_dword			mRenderSetHitTimes;
	_dword			mTotalRenderTimes;

	String			mName;
	EntityPool		mEntityPool;
	EntityGroupPool	mEntityGroupPool;
	PickedArray		mPickedArray;

	inline _bool Pickable( Entity* entity, _dword pickflag, _dword pickarg );

public:
	RenderScene( StringPtr name, _dword maxentitynumber );
	~RenderScene( );

	virtual StringPtr		GetName( ) const;
	virtual _dword			GetEntityNumber( ) const;

	virtual _dword			GetFirstEntityID( ) const;
	virtual _dword			GetNextEntityID( _dword previousid ) const;

	virtual _dword			CreateEntityGroup( const AxisAlignedBox& boundbox, _dword userdata1 = 0, _dword userdata2 = 0 );
	virtual _bool			RemoveEntityGroup( _dword groupid );

	virtual _void			SetEntityGroupBoundBox( _dword groupid, const AxisAlignedBox& boundbox );
	virtual AxisAlignedBox	GetEntityGroupBoundBox( _dword groupid ) const;
	virtual _void			SetEntityGroupUserData1( _dword groupid, _dword userdata );
	virtual _dword			GetEntityGroupUserData1( _dword groupid ) const;
	virtual _void			SetEntityGroupUserData2( _dword groupid, _dword userdata );
	virtual _dword			GetEntityGroupUserData2( _dword groupid ) const;

	virtual _dword			CreateEntity( const Matrix4& transform, _dword userdata1 = 0, _dword userdata2 = 0, _dword groupid = 0 );
	virtual _bool			RemoveEntity( _dword entityid );

	virtual _dword			GetGeometryNumber( _dword entityid ) const;
	virtual const Geometry&	GetGeometry( _dword entityid, _dword geometryindex ) const;
	virtual	_void			SetGeometryTextureFactor( _dword entityid, _dword geometryindex, _dword factor );
	virtual	_void			SetGeometryMaterialDiffuse( _dword entityid, _dword geometryindex, _dword factor );
	virtual	_void			SetGeometryMaterialAmbient( _dword entityid, _dword geometryindex, _dword factor );
	virtual	_void			SetGeometryMaterialSpecular( _dword entityid, _dword geometryindex, _dword factor );
	virtual	_void			SetGeometryMaterialEmissive( _dword entityid, _dword geometryindex, _dword factor );
	virtual	_void			SetGeometryMaterialPower( _dword entityid, _dword geometryindex, _float factor );
	virtual	_void			SetGeometryDepthBias( _dword entityid, _dword geometryindex, _float factor );

	virtual _bool			SetMaxGeometryNumber( _dword entityid, _dword number );
	virtual _bool			AppendGeometry( _dword entityid, const Geometry& geometry );
	virtual _bool			RemoveGeometry( _dword entityid, _dword geometryindex );
	virtual _bool			ClearGeometry( _dword entityid );

	virtual AxisAlignedBox	GetEntityBoundBox( _dword entityid ) const;

	virtual _void			SetEntityTransform( _dword entityid, const Matrix4& transform );
	virtual const Matrix4&	GetEntityTransform( _dword entityid ) const;
	virtual _void			SetEntityUserData1( _dword entityid, _dword userdata );
	virtual _dword			GetEntityUserData1( _dword entityid ) const;
	virtual _void			SetEntityUserData2( _dword entityid, _dword userdata );
	virtual _dword			GetEntityUserData2( _dword entityid ) const;
	virtual _void			SetEntityPickType( _dword entityid, _dword picktype );
	virtual _dword			GetEntityPickType( _dword entityid ) const;
	virtual _void			SetEntityPickData( _dword entityid, _dword pickdata );
	virtual _dword			GetEntityPickData( _dword entityid ) const;
	virtual	_void			SetEntityRenderMethod( _dword entityid, IRenderMethod* rendermethod );
	virtual	_bool			GetEntityVisible( _dword entityid ) const;
	virtual	_void			SetEntityVisible( _dword entityid, _bool visible );

	virtual _bool			Pick( const Ray& pickray, _float& distance, _dword pickflag = 0, _dword pickarg = 0 );
	virtual _bool			Pick( const AxisAlignedBox& boundbox, _dword pickflag = 0, _dword pickarg = 0 );
	virtual _dword			GetPickedEntityNumber( ) const;
	virtual _dword			GetPickedEntityID( _dword pickindex ) const;
	virtual _float			GetPickedEntityDistance( _dword pickindex ) const;

	virtual _void			FrustumClip( const Frustum& viewfrustum );
	virtual	_bool			IsEntityGroupInsideFrustum( _dword entitygroupid ) const;
	virtual _bool			IsEntityInsideFrustum( _dword entityid ) const;

	virtual _void			ClearScene( );
	virtual	_void			RenderEntities( );
};

//----------------------------------------------------------------------------
// RenderScene Implementation
//----------------------------------------------------------------------------

_bool RenderScene::Pickable( Entity* entity, _dword pickflag, _dword pickarg )
{
	switch ( pickflag )
	{
		case _PICK_SAMEAS_USERDATA1:
		{
			if ( pickarg != entity->mUserData1 )
				return _false;

			break;
		}

		case _PICK_NOTSAMEAS_USERDATA1:
		{
			if ( pickarg == entity->mUserData1 )
				return _false;

			break;
		}

		case _PICK_LOGICOR_USERDATA1:
		{
			if ( ( pickarg | entity->mUserData1 ) == 0 )
				return _false;

			break;
		}

		case _PICK_LOGICAND_USERDATA1:
		{
			if ( ( pickarg & entity->mUserData1 ) == 0 )
				return _false;

			break;
		}

		case _PICK_SAMEAS_USERDATA2:
		{
			if ( pickarg != entity->mUserData2 )
				return _false;

			break;
		}

		case _PICK_NOTSAMEAS_USERDATA2:
		{
			if ( pickarg == entity->mUserData2 )
				return _false;

			break;
		}

		case _PICK_LOGICOR_USERDATA2:
		{
			if ( ( pickarg | entity->mUserData2 ) == 0 )
				return _false;

			break;
		}

		case _PICK_LOGICAND_USERDATA2:
		{
			if ( ( pickarg & entity->mUserData2 ) == 0 )
				return _false;

			break;
		}

		case _PICK_SAMEAS_PICKDATA:
		{
			if ( pickarg != entity->mPickData )
				return _false;

			break;
		}

		case _PICK_NOTSAMEAS_PICKDATA:
		{
			if ( pickarg == entity->mPickData )
				return _false;

			break;
		}

		case _PICK_LOGICOR_PICKDATA:
		{
			if ( ( pickarg | entity->mPickData ) == 0 )
				return _false;

			break;
		}

		case _PICK_LOGICAND_PICKDATA:
		{
			if ( ( pickarg & entity->mPickData ) == 0 )
				return _false;

			break;
		}
	}

	return _true;
}

};