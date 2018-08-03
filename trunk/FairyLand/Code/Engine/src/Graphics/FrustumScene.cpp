//============================================================================
// FrustumScene.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// FrustumScene Implementation
//----------------------------------------------------------------------------

FrustumScene::FrustumScene( StringPtr name, _dword maxentity )
	: RenderScene( name, maxentity )
{
}

FrustumScene::~FrustumScene( )
{
}

_void FrustumScene::RenderEntities( const Frustum& viewfrustum )
{
	_bool clip = GetRenderConfig( ).IsViewFrustumClipEnabled( );

	GetRenderConfig( ).EnableViewFrustumClip( _false );

	// Render each entity.
	for ( EntityPool::Iterator it = mEntityPool.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		Entity* entity = it;

		for ( _dword i = 0; i < entity->mGeometryArray.Number( ); i ++ )
		{
			const Geometry& geometry = entity->mGeometryArray[i];

			// Skip invisible geometry.
			if ( viewfrustum.Inside( geometry.mVertexBuffer->GetBoundBox( ) * entity->mTransform ) == _false )
				continue;

			GetRenderDevice( ).Render( geometry, entity->mTransform );
		}
	}

	GetRenderConfig( ).EnableViewFrustumClip( clip );
}