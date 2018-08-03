//============================================================================
// ISceneFactory.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ISceneFactory
//----------------------------------------------------------------------------

class ISceneFactory
{
public:
	virtual IRenderScene*	CreateFrustumScene( StringPtr name, _dword maxentity ) = 0;

	virtual IRenderScene*	SearchRenderScene( StringPtr name ) = 0;

	virtual _void			ReleaseRenderScene( IRenderScene*& renderscene ) = 0;
};

};