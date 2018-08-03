//============================================================================
// RenderScene.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// RenderScene Implementation
//----------------------------------------------------------------------------

RenderScene::Entity::Entity( )
{
	mEntityID		= 0;
	mGroupID		= 0;
	mTransform		= Matrix4::cIdentity;
	mUserData1		= 0;
	mUserData2		= 0;
	mPickType		= _PICK_MESH;
	mPickData		= 0;
	mInternal1		= 0;
	mInternal2		= 0;
	mInsideFrustum	= _false;
	mVisible		= _true;
	mRenderMethod	= _null;
}

RenderScene::Entity::~Entity( )
{
	// Release geometry.
	for ( _dword i = 0; i < mGeometryArray.Number( ); i ++ )
		GetGeometryFactory( ).ReleaseGeometry( mGeometryArray[i] );

	GetTechniqueFactory( ).ReleaseRenderMethod( mRenderMethod );
}

RenderScene::EntityGroup::EntityGroup( )
{
	mGroupID		= 0;
	mBoundBox		= AxisAlignedBox::cNullBox;
	mUserData1		= 0;
	mUserData2		= 0;
	mInternal1		= 0;
	mInternal2		= 0;
	mInsideFrustum	= _false;
}

RenderScene::RenderScene( StringPtr name, _dword maxentitynumber ) : 
	mName( name ), mEntityPool( maxentitynumber ), mEntityGroupPool( maxentitynumber )
{
	mRenderSet			= _null;
	mRefreshRenderSet	= _false;
	mRenderSetHitTimes	= 0;
	mTotalRenderTimes	= 1;

	mRenderSet = GetTechniqueFactory( ).CreateRenderSet( );

	// Create a default entity group.
	mEntityGroupPool.Create( new EntityGroup );

	IncreaseRefCount( );
}

RenderScene::~RenderScene( )
{
	for ( EntityPool::Iterator it = mEntityPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Entity* e = it;
		delete e;
	}

	for ( EntityGroupPool::Iterator it = mEntityGroupPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		EntityGroup* e = it;
		delete e;
	}

	GetTechniqueFactory( ).ReleaseRenderSet( mRenderSet );
}

StringPtr RenderScene::GetName( ) const
{
	return mName;
}

_dword RenderScene::GetEntityNumber( ) const
{
	return mEntityPool.Number( );
}

_dword RenderScene::GetFirstEntityID( ) const
{
	EntityPool::Iterator it = mEntityPool.GetHeadIterator( );

	if ( it.Valid( ) == _false )
		return -1;

	Entity* entity = it;

	return entity->mEntityID;
}

_dword RenderScene::GetNextEntityID( _dword previousid ) const
{
	EntityPool::Iterator it = mEntityPool.GetIterator( previousid );

	if ( it.Valid( ) == _false )
		return -1;

	it ++;

	if ( it.Valid( ) == _false )
		return -1;

	Entity* entity = it;

	return entity->mEntityID;
}

_dword RenderScene::CreateEntityGroup( const AxisAlignedBox& boundbox, _dword userdata1, _dword userdata2 )
{
	if ( mEntityGroupPool.Number( ) == mEntityGroupPool.Size( ) )
		return -1;

	EntityGroup* group	= new EntityGroup;	
	group->mGroupID		= mEntityGroupPool.Create( group );
	group->mBoundBox	= boundbox;
	group->mUserData1	= userdata1;
	group->mUserData2	= userdata2;	

	mRefreshRenderSet	= _true;

	return group->mGroupID;
}

_bool RenderScene::RemoveEntityGroup( _dword groupid )
{
	if ( mEntityGroupPool.Index( groupid ) == _null )
		return _false;

	EntityGroup* group = mEntityGroupPool[ groupid ];

	for ( _dword i = 0; i < group->mEntityArray.Number( ); i ++ )
	{
		_dword entityid	= group->mEntityArray[i]->mEntityID;

		delete mEntityPool[ entityid ];
		mEntityPool.Remove( entityid );
	}

	delete mEntityGroupPool[ groupid ];
	mEntityGroupPool.Remove( groupid );

	mRefreshRenderSet = _true;

	return _true;
}

_void RenderScene::SetEntityGroupBoundBox( _dword groupid, const AxisAlignedBox& boundbox )
{
	EntityGroup* group = mEntityGroupPool[ groupid ];

	if ( group->mBoundBox != boundbox )
	{
		group->mBoundBox = boundbox;

		mRefreshRenderSet = _true;
	}
}

AxisAlignedBox RenderScene::GetEntityGroupBoundBox( _dword groupid ) const
{
	EntityGroup* group = mEntityGroupPool[ groupid ];

	return group->mBoundBox;
}

_void RenderScene::SetEntityGroupUserData1( _dword groupid, _dword userdata )
{
	EntityGroup* group = mEntityGroupPool[ groupid ];

	group->mUserData1 = userdata;
}

_dword RenderScene::GetEntityGroupUserData1( _dword groupid ) const
{
	EntityGroup* group = mEntityGroupPool[ groupid ];

	return group->mUserData1;
}

_void RenderScene::SetEntityGroupUserData2( _dword groupid, _dword userdata )
{
	EntityGroup* group = mEntityGroupPool[ groupid ];

	group->mUserData2 = userdata;
}

_dword RenderScene::GetEntityGroupUserData2( _dword groupid ) const
{
	EntityGroup* group = mEntityGroupPool[ groupid ];

	return group->mUserData2;
}

_dword RenderScene::CreateEntity( const Matrix4& transform, _dword userdata1, _dword userdata2, _dword groupid )
{
	if ( mEntityPool.Number( ) == mEntityPool.Size( ) )
		return -1;

	// No parent entity group.
	if ( groupid != 0 && mEntityGroupPool.Index( groupid ) == _null )
		return -1;

	Entity* entity		= new Entity;
	entity->mEntityID	= mEntityPool.Create( entity );
	entity->mGroupID	= groupid;
	entity->mTransform	= transform;
	entity->mUserData1	= userdata1;
	entity->mUserData2	= userdata2;

	EntityGroup* group = mEntityGroupPool[ groupid ];
	FL_ASSERT( group != _null )

	group->mEntityArray.Append( entity );

	mRefreshRenderSet = _true;

	return entity->mEntityID;
}

_bool RenderScene::RemoveEntity( _dword entityid )
{
	if ( mEntityPool.Index( entityid ) == _null )
		return _false;

	Entity* entity = mEntityPool[ entityid ];

	EntityGroup* group = mEntityGroupPool[ entity->mGroupID ];
	FL_ASSERT( group != _null )

	for ( _dword i = 0; i < group->mEntityArray.Number( ); i ++ )
	{
		if ( group->mEntityArray[i] == entity )
		{
			group->mEntityArray.Remove( i );
			break;
		}
	}
	
	delete mEntityPool[ entityid ];
	mEntityPool.Remove( entityid );

	mRefreshRenderSet = _true;

	return _true;
}

_dword RenderScene::GetGeometryNumber( _dword entityid ) const
{
	Entity* entity = mEntityPool[ entityid ];

	return entity->mGeometryArray.Number( );
}

const Geometry& RenderScene::GetGeometry( _dword entityid, _dword geometryindex ) const
{
	Entity* entity = mEntityPool[ entityid ];

	return entity->mGeometryArray[ geometryindex ];
}

_void RenderScene::SetGeometryTextureFactor( _dword entityid, _dword geometryindex, _dword factor )
{
	Entity* entity = mEntityPool[ entityid ];

	if ( entity->mGeometryArray[ geometryindex ].mTextureFactor != factor )
	{
		entity->mGeometryArray[ geometryindex ].mTextureFactor = factor;

		mRefreshRenderSet = _true;
	}
}

_void RenderScene::SetGeometryMaterialDiffuse( _dword entityid, _dword geometryindex, _dword color )
{
	Entity* entity = mEntityPool[ entityid ];

	if ( entity->mGeometryArray[ geometryindex ].mMaterialDiffuse != color )
	{
		entity->mGeometryArray[ geometryindex ].mMaterialDiffuse = color;

		mRefreshRenderSet = _true;
	}
}

_void RenderScene::SetGeometryMaterialAmbient( _dword entityid, _dword geometryindex, _dword color )
{
	Entity* entity = mEntityPool[ entityid ];

	if ( entity->mGeometryArray[ geometryindex ].mMaterialAmbient != color )
	{
		entity->mGeometryArray[ geometryindex ].mMaterialAmbient = color;

		mRefreshRenderSet = _true;
	}
}

_void RenderScene::SetGeometryMaterialSpecular( _dword entityid, _dword geometryindex, _dword color )
{
	Entity* entity = mEntityPool[ entityid ];

	if ( entity->mGeometryArray[ geometryindex ].mMaterialSpecular != color )
	{
		entity->mGeometryArray[ geometryindex ].mMaterialSpecular = color;

		mRefreshRenderSet = _true;
	}
}

_void RenderScene::SetGeometryMaterialEmissive( _dword entityid, _dword geometryindex, _dword color )
{
	Entity* entity = mEntityPool[ entityid ];

	if ( entity->mGeometryArray[ geometryindex ].mMaterialEmissive != color )
	{
		entity->mGeometryArray[ geometryindex ].mMaterialEmissive = color;

		mRefreshRenderSet = _true;
	}
}

_void RenderScene::SetGeometryMaterialPower( _dword entityid, _dword geometryindex, _float power )
{
	Entity* entity = mEntityPool[ entityid ];

	if ( entity->mGeometryArray[ geometryindex ].mMaterialPower	!= power )
	{
		entity->mGeometryArray[ geometryindex ].mMaterialPower = power;

		mRefreshRenderSet = _true;
	}
}

_void RenderScene::SetGeometryDepthBias( _dword entityid, _dword geometryindex, _float depthbias )
{
	Entity* entity = mEntityPool[ entityid ];

	if ( entity->mGeometryArray[ geometryindex ].mDepthBias != depthbias )
	{
		entity->mGeometryArray[ geometryindex ].mDepthBias = depthbias;

		mRefreshRenderSet = _true;
	}
}

_bool RenderScene::SetMaxGeometryNumber( _dword entityid, _dword number )
{
	if ( mEntityPool.Index( entityid ) == _null )
		return _false;

	Entity* entity = mEntityPool[ entityid ];
	entity->mGeometryArray.Clear( number, 10 );

	mRefreshRenderSet = _true;

	return _true;
}

_bool RenderScene::AppendGeometry( _dword entityid, const Geometry& geometry )
{
	if ( mEntityPool.Index( entityid ) == _null )
		return _false;

	GetGeometryFactory( ).CloneGeometry( geometry, _true );

	Entity* entity = mEntityPool[ entityid ];
	entity->mGeometryArray.Append( geometry );

	mRefreshRenderSet = _true;

	return _true;
}

_bool RenderScene::RemoveGeometry( _dword entityid, _dword geometryindex )
{
	if ( mEntityPool.Index( entityid ) == _null )
		return _false;

	Entity* entity = mEntityPool[ entityid ];

	if ( geometryindex >= entity->mGeometryArray.Number( ) )
		return _false;

	GetGeometryFactory( ).ReleaseGeometry( entity->mGeometryArray[ geometryindex ] );
	entity->mGeometryArray.Remove( geometryindex );

	mRefreshRenderSet = _true;

	return _true;
}

_bool RenderScene::ClearGeometry( _dword entityid )
{
	if ( mEntityPool.Index( entityid ) == _null )
		return _false;

	Entity* entity = mEntityPool[ entityid ];

	for ( _dword i = 0; i < entity->mGeometryArray.Number( ); i ++ )
		GetGeometryFactory( ).ReleaseGeometry( entity->mGeometryArray[i] );

	entity->mGeometryArray.Clear( );

	mRefreshRenderSet = _true;

	return _true;
}
	
AxisAlignedBox RenderScene::GetEntityBoundBox( _dword entityid ) const
{
	if ( mEntityPool.Index( entityid ) == _null )
		return AxisAlignedBox::cNullBox;

	Entity* entity = mEntityPool[ entityid ];

	if ( entity->mGeometryArray.Number( ) == 0 )
		return AxisAlignedBox::cNullBox;

	AxisAlignedBox boundbox = entity->mGeometryArray[0].mVertexBuffer->GetBoundBox( );

	for ( _dword i = 1; i < entity->mGeometryArray.Number( ); i ++ )
		boundbox += entity->mGeometryArray[i].mVertexBuffer->GetBoundBox( );

	return boundbox * entity->mTransform;
}

_void RenderScene::SetEntityTransform( _dword entityid, const Matrix4& transform )
{
	if ( mEntityPool.Index( entityid ) == _null )
		return;

	if ( mEntityPool[ entityid ]->mTransform != transform )
	{
		mEntityPool[ entityid ]->mTransform = transform;

		mRefreshRenderSet = _true;
	}
}

const Matrix4& RenderScene::GetEntityTransform( _dword entityid ) const
{
	if ( mEntityPool.Index( entityid ) == _null )
		return Matrix4::cIdentity;

	return mEntityPool[ entityid ]->mTransform;
}

_void RenderScene::SetEntityUserData1( _dword entityid, _dword userdata )
{
	if ( mEntityPool.Index( entityid ) == _null )
		return;

	mEntityPool[ entityid ]->mUserData1 = userdata;
}

_dword RenderScene::GetEntityUserData1( _dword entityid ) const
{
	if ( mEntityPool.Index( entityid ) == _null )
		return 0;

	return mEntityPool[ entityid ]->mUserData1;
}

_void RenderScene::SetEntityUserData2( _dword entityid, _dword userdata )
{
	if ( mEntityPool.Index( entityid ) == _null )
		return;

	mEntityPool[ entityid ]->mUserData2 = userdata;
}

_dword RenderScene::GetEntityUserData2( _dword entityid ) const
{
	if ( mEntityPool.Index( entityid ) == _null )
		return 0;

	return mEntityPool[ entityid ]->mUserData2;
}

_void RenderScene::SetEntityPickType( _dword entityid, _dword picktype )
{
	if ( mEntityPool.Index( entityid ) == _null )
		return;

	mEntityPool[ entityid ]->mPickType = picktype;
}

_dword RenderScene::GetEntityPickType( _dword entityid ) const
{
	if ( mEntityPool.Index( entityid ) == _null )
		return 0;

	return mEntityPool[ entityid ]->mPickType;
}

_void RenderScene::SetEntityPickData( _dword entityid, _dword pickdata )
{
	if ( mEntityPool.Index( entityid ) == _null )
		return;

	mEntityPool[ entityid ]->mPickData = pickdata;
}

_dword RenderScene::GetEntityPickData( _dword entityid ) const
{
	if ( mEntityPool.Index( entityid ) == _null )
		return 0;

	return mEntityPool[ entityid ]->mPickData;
}

_void RenderScene::SetEntityRenderMethod( _dword entityid, IRenderMethod* rendermethod )
{
	if ( mEntityPool.Index( entityid ) == _null )
		return;

	Entity* entity = mEntityPool[ entityid ];

	if ( entity->mRenderMethod != rendermethod )
	{
		// Release old render method.
		if ( entity->mRenderMethod )
			GetTechniqueFactory( ).ReleaseRenderMethod( entity->mRenderMethod );

		if ( rendermethod )
			entity->mRenderMethod = GetTechniqueFactory( ).CloneRenderMethod( rendermethod );

		mRefreshRenderSet = _true;
	}
}

_bool RenderScene::GetEntityVisible( _dword entityid ) const
{
	if ( mEntityPool.Index( entityid ) == _null )
		return _null;

	return mEntityPool[ entityid ]->mVisible;
}

_void RenderScene::SetEntityVisible( _dword entityid, _bool visible )
{
	if ( mEntityPool.Index( entityid ) == _null )
		return;

	if ( mEntityPool[ entityid ]->mVisible != visible )
	{
		mEntityPool[ entityid ]->mVisible = visible;

		mRefreshRenderSet = _true;
	}	
}

_bool RenderScene::Pick( const Ray& pickray, _float& distance, _dword pickflag, _dword pickarg )
{
	// Clear the old pick array.
	mPickedArray.Clear( );

	// Get pick flag.
	_bool	enablepickflag	= pickflag !=_PICK_DISABLE_PICKFLAG;
	_bool	onlyboundbox	= _false;
	_bool	pickboundbox	= _false;
	_dword	forcepicktype	= 0;

	if ( enablepickflag )
	{
		onlyboundbox = pickflag & _PICK_ONLYBOUNDBOX;
		pickboundbox = pickflag & _PICK_PICKBOUNDBOX;

		_bool forcepickmesh		= pickflag & _PICK_FORCEPICKMESH;
		_bool forcepickboundbox	= pickflag & _PICK_FORCEPICKBOUNDBOX;

		pickflag &= _PICK_MASK;	

		// Get force pick type.
		if ( forcepickmesh )
			forcepicktype |= _PICK_MESH;
		if ( forcepickboundbox )
			forcepicktype |= _PICK_BOUNDBOX;		
	}

	_float nearest = -1.0f, tempdistance = 0.0f; _dword pickedentity = -1;

	// First pick entity group.
	for ( EntityGroupPool::Iterator it = mEntityGroupPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		EntityGroup* group = it;

		// Pick group first, skip default group.
		if ( group->mGroupID > 0 )
		{
			if ( Intersection::RayVSAABox( pickray, group->mBoundBox ) == _false )
				continue;
		}

		// Pick each entity.
		for ( _dword e = 0; e < group->mEntityArray.Number( ); e ++ )
		{
			Entity* entity = group->mEntityArray[e];

			if ( entity == _null )
				continue;

			_dword picktype = entity->mPickType | forcepicktype;

			if ( enablepickflag && picktype == 0 )
				continue;

			if ( enablepickflag && Pickable( entity, pickflag, pickarg ) == _false )
				continue;

			Matrix4 worldinverse = entity->mTransform;
			worldinverse.Inverse( );

			// Convert ray from world space to mesh space.
			Ray tempray = pickray * worldinverse;

			for ( _dword i = 0; i < entity->mGeometryArray.Number( ); i ++ )
			{
				Geometry& geometry = entity->mGeometryArray[i];

				if ( geometry.mVertexBuffer == _null )
					continue;

				AxisAlignedBox boundbox = geometry.mVertexBuffer->GetBoundBox( );

				// We need pick bound box.
				if ( enablepickflag && pickedentity == -1 && ( picktype & _PICK_BOUNDBOX ) && ( onlyboundbox || pickboundbox ) )
				{
					// Intersect with bound box first.
					if ( Intersection::RayVSAABox( tempray, boundbox, tempdistance ) == _false )
						continue;

					// We picked this bound box, maybe it is useful, save it.
					mPickedArray.Append( PickedEntity( tempdistance, entity->mEntityID | 0x80000000 ) );
				}
				else
				{
					// Intersect with bound box first.
					if ( Intersection::RayVSAABox( tempray, boundbox ) == _false )
						continue;
				}

				// Dont pick mesh if we only need bound box.
				if ( enablepickflag && onlyboundbox )
					continue;

				if ( enablepickflag && ( picktype & _PICK_MESH ) == 0 )
					continue;

				_dword indexnumber = 0;

				if ( geometry.mGeometryType == Geometry::_GEOMETRY_TRIANGLELIST )
					indexnumber = geometry.mGeometryNumber * 3;
				else if ( geometry.mGeometryType == Geometry::_GEOMETRY_TRIANGLESTRIP )
					indexnumber = geometry.mGeometryNumber + 2;
				else if ( geometry.mGeometryType == Geometry::_GEOMETRY_TRIANGLEFAN )
					indexnumber = geometry.mGeometryNumber + 2;

				if ( indexnumber == 0 )
					continue;

				_byte* vertexbuffer = _null;
				if ( geometry.mVertexBuffer != _null )
				{
					vertexbuffer = (_byte*) geometry.mVertexBuffer->Lock( geometry.mStartVertex * geometry.mVertexSize,
						geometry.mVertexNumber * geometry.mVertexSize, IGeometryFactory::_FLAG_DISCARD );
				}

				_byte* indexbuffer = _null;
				if ( geometry.mIndexBuffer != _null )
				{
					indexbuffer = (_byte*) geometry.mIndexBuffer->Lock( geometry.mStartIndex * sizeof( _word ),
						indexnumber * sizeof( _word ), IGeometryFactory::_FLAG_DISCARD );
				}

				// Then interset with polygon.
				if ( Intersection::RayVSMesh( tempray, geometry.mVertexSize, geometry.mGeometryType, geometry.mGeometryNumber, vertexbuffer, indexbuffer, tempdistance ) )
				{
					if ( nearest < 0.0f || nearest > tempdistance )
						nearest = tempdistance;

					// Save the picked entity ID into array.
					mPickedArray.Append( PickedEntity( tempdistance, entity->mEntityID ) );

					pickedentity = entity->mEntityID;
				}

				if ( geometry.mVertexBuffer != _null )
					geometry.mVertexBuffer->Unlock( );

				if ( geometry.mIndexBuffer != _null )
					geometry.mIndexBuffer->Unlock( );
			}
		}
	}

	if ( mPickedArray.Number( ) == 0 )
		return _false;

	// Process picked entity array.
	for ( _dword i = 0; i < mPickedArray.Number( ); i ++ )
	{
		// This is a candidate bound box.
		if ( mPickedArray[i].mObject2 & 0x80000000 )
		{
			// We picked at least 1 entity, so remove candidate bound box.
			if ( pickedentity != -1 )
			{
				mPickedArray.Remove( i -- );
			}
			// Clear entity ID.
			else
			{
				mPickedArray[i].mObject2 &= 0x7FFFFFFF;
			}
		}
	}

	// Sort entity from near to far.
	QuickSort< PickedEntity > quicksort;
	quicksort.SortAscending( (PickedEntity*) mPickedArray.GetArrayBuffer( ), mPickedArray.Number( ) );

	distance = nearest;

	return _true;
}

_bool RenderScene::Pick( const AxisAlignedBox& boundbox, _dword pickflag, _dword pickarg )
{
	// Clear the old pick array.
	mPickedArray.Clear( );

	// Get force pick type.
	_dword forcepicktype = ( pickflag & _PICK_FORCEPICKBOUNDBOX ) ? _PICK_BOUNDBOX : 0;

	// First pick entity group.
	for ( EntityGroupPool::Iterator it = mEntityGroupPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		EntityGroup* group = it;

		// Pick group first, skip default group.
		if ( group->mGroupID > 0 )
		{
			if ( Intersection::AABoxVSAABox( boundbox, group->mBoundBox ) == _false )
				continue;
		}

		// Pick each entity.
		for ( _dword e = 0; e < group->mEntityArray.Number( ); e ++ )
		{
			Entity* entity = group->mEntityArray[e];

			if ( entity == _null )
				continue;

			_dword picktype = entity->mPickType | forcepicktype;

			if ( picktype == 0 )
				continue;

			if ( Pickable( entity, pickflag, pickarg ) == _false )
				continue;

			for ( _dword i = 0; i < entity->mGeometryArray.Number( ); i ++ )
			{
				Geometry& geometry = entity->mGeometryArray[i];

				if ( geometry.mVertexBuffer == _null )
					continue;

				// Intersect with bound box.
				if ( Intersection::AABoxVSAABox( boundbox, geometry.mVertexBuffer->GetBoundBox( ) * entity->mTransform ) == _false )
					continue;

				mPickedArray.Append( PickedEntity( 0.0f, entity->mEntityID ) );
			}
		}
	}

	return mPickedArray.Number( ) > 0;
}

_dword RenderScene::GetPickedEntityNumber( ) const
{
	return mPickedArray.Number( );
}

_dword RenderScene::GetPickedEntityID( _dword pickindex ) const
{
	return mPickedArray[ pickindex ].mObject2;
}

_float RenderScene::GetPickedEntityDistance( _dword pickindex ) const
{
	return mPickedArray[ pickindex ].mObject1;
}

_void RenderScene::FrustumClip( const Frustum& viewfrustum )
{
	if ( mRefreshRenderSet == _false && mLastFrustum == viewfrustum )
		return;

	// First clip entity group.
	for ( EntityGroupPool::Iterator it = mEntityGroupPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		EntityGroup* group = it;		

		// Clip group first, skip default group.
		if ( group->mGroupID > 0 )
		{
			group->mInsideFrustum = viewfrustum.Inside( group->mBoundBox );

			// Skip if group is not in the frustum.
			if ( !group->mInsideFrustum )
				continue;
		}

		// Clip each entity.
		for ( _dword e = 0; e < group->mEntityArray.Number( ); e ++ )
		{
			Entity* entity = group->mEntityArray[e];

			if ( entity == _null )
				continue;

			entity->mInsideFrustum = _false;

			// Detail clip with entity bound box.
			for ( _dword i = 0; i < entity->mGeometryArray.Number( ); i ++ )
			{
				const Geometry& geometry = entity->mGeometryArray[i];

				if ( geometry.mVertexBuffer == _null )
					continue;

				// Intersect with bound box.
				if ( viewfrustum.Inside( geometry.mVertexBuffer->GetBoundBox( ) * entity->mTransform ) )
				{
					entity->mInsideFrustum = _true;

					break;
				}
			}
		}
	}

	// Save last frustum.
	mLastFrustum = viewfrustum;

	// Set refresh flag.
	mRefreshRenderSet = _true;
}

_bool RenderScene::IsEntityGroupInsideFrustum( _dword entitygroupid ) const
{
	// 0 is invalid entity group ID.
	if ( entitygroupid == 0 )
		return	_false;

	if ( mEntityGroupPool.Index( entitygroupid ) == _null )
		return _false;

	EntityGroup* entitygroup = mEntityGroupPool[ entitygroupid ];
	if ( entitygroup == _null )
		return _false;

	return entitygroup->mInsideFrustum;
}

_bool RenderScene::IsEntityInsideFrustum( _dword entityid ) const
{
	if ( mEntityPool.Index( entityid ) == _null )
		return _false;

	Entity* entity = mEntityPool[ entityid ];
	if ( entity == _null )
		return _false;

	// If parent group is not in the frustum, return false.
	if ( entity->mGroupID != 0 )
	{
		if ( IsEntityGroupInsideFrustum( entity->mGroupID ) == _false )
			return _false;
	}

	return entity->mInsideFrustum;
}

_void RenderScene::ClearScene( )
{
	for ( EntityPool::Iterator it = mEntityPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Entity* e = it;
		delete e;
	}

	for ( EntityGroupPool::Iterator it = mEntityGroupPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		EntityGroup* e = it;
		delete e;
	}

	mEntityPool.Clear( );
	mEntityGroupPool.Clear( );
	mPickedArray.Clear( );

	// Set refresh flag.
	mRefreshRenderSet = _true;

	// Create a default entity group.
	mEntityGroupPool.Create( new EntityGroup );
}

_void RenderScene::RenderEntities( )
{
	if ( mRefreshRenderSet )	
	{
		// Render to the render set
		mRenderSet->ClearRecord( );
		mRenderSet->BeginRecord( );

		_bool clip = GetRenderConfig( ).IsViewFrustumClipEnabled( );

		GetRenderConfig( ).EnableViewFrustumClip( _false );

		// Render each entity group.
		for ( EntityGroupPool::Iterator itGroup = mEntityGroupPool.GetHeadIterator( ); itGroup.Valid( ); itGroup ++ )
		{
			EntityGroup* entitygroup = itGroup;	

			if ( entitygroup->mGroupID != 0 && entitygroup->mInsideFrustum == _false )
				continue;

			// Render each entity in this group
			for ( EntityArray::Iterator itEntity = entitygroup->mEntityArray.GetHeadIterator( ); itEntity.Valid( ); itEntity ++ )
			{
				Entity* entity = itEntity;

				if ( entity->mInsideFrustum == _false )
					continue;

				if ( entity->mVisible == _false )
					continue;

				// If the entity has own render method, use it.
				IRenderMethod* oldrendermethod = GetRenderDevice( ).GetRenderMethod( );

				if ( entity->mRenderMethod )
					GetRenderDevice( ).SetRenderMethod( entity->mRenderMethod );

				// Render each geometry.
				for ( _dword i = 0; i < entity->mGeometryArray.Number( ); i ++ )
					GetRenderDevice( ).Render( entity->mGeometryArray[i], entity->mTransform );

				// Reset old render method.
				if ( entity->mRenderMethod )
					GetRenderDevice( ).SetRenderMethod( oldrendermethod );
			}
		}

		GetRenderConfig( ).EnableViewFrustumClip( clip );

		// Stop rendering to the render set
		mRenderSet->EndRecord( );

		// Reset refresh flag
		mRefreshRenderSet = _false;
	}
	else
	{
		mRenderSetHitTimes ++;
	}

	mTotalRenderTimes ++;

	GetRenderDevice( ).Render( mRenderSet );
}