//============================================================================
// SceneFactory.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// SceneFactory
//----------------------------------------------------------------------------

class SceneFactory : public ISceneFactory
{
private:
	Array< RenderScene* >	mSceneArray;

public:
	SceneFactory( );
	~SceneFactory( );

	virtual IRenderScene*	CreateFrustumScene( StringPtr name, _dword maxentity );

	virtual IRenderScene*	SearchRenderScene( StringPtr name );

	virtual _void			ReleaseRenderScene( IRenderScene*& renderscene );
};

};