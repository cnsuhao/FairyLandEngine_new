//============================================================================
// FairyLandGraphics.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// CreateGraphicsManager Implementation
//----------------------------------------------------------------------------

IGraphicsManager* CreateGraphicsManager( )
{
	IGraphicsManager* graphicsmanager = GetGraphicsManagerPointer( );

	if ( graphicsmanager != _null )
		return graphicsmanager;

	graphicsmanager = new GraphicsManager( );
	GetFairyLandGlobal( ).mGraphicsManager = graphicsmanager;

	PS_TRACE( L"[KL] GraphicsManager Created\r\n" )

	return graphicsmanager;
}