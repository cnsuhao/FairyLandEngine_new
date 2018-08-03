//============================================================================
// FrustumScene.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// FrustumScene
//----------------------------------------------------------------------------

class FrustumScene : public RenderScene
{
public:
	FrustumScene( StringPtr name, _dword maxentity );
	~FrustumScene( );

	virtual _void RenderEntities( const Frustum& viewfrustum );
};

};