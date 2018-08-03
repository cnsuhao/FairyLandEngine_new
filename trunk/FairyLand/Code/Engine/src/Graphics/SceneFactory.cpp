//============================================================================
// SceneFactory.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// SceneFactory Implementation
//----------------------------------------------------------------------------

SceneFactory::SceneFactory( )
{
}

SceneFactory::~SceneFactory( )
{
	for ( _dword i = 0; i < mSceneArray.Number( ); i ++ )
		delete mSceneArray[i];
}

IRenderScene* SceneFactory::CreateFrustumScene( StringPtr name, _dword maxentity )
{
	RenderScene* renderscene = new FrustumScene( name, maxentity );

	mSceneArray.Append( renderscene );

	return renderscene;
}

IRenderScene* SceneFactory::SearchRenderScene( StringPtr name )
{
	for ( _dword i = 0; i < mSceneArray.Number( ); i ++ )
	{
		if ( mSceneArray[i]->GetName( ) == name )
			return mSceneArray[i];
	}

	return _null;
}

_void SceneFactory::ReleaseRenderScene( IRenderScene*& renderscene )
{
	if ( renderscene == _null )
		return;

	if ( ( (RenderScene*) renderscene )->DecreaseRefCount( ) == 0 )
	{
		delete (RenderScene*) renderscene;

		for ( _dword i = 0; i < mSceneArray.Number( ); i ++ )
		{
			if ( mSceneArray[i] == renderscene )
			{
				mSceneArray.Remove( i );
				break;
			}
		}
	}

	renderscene = _null;
}