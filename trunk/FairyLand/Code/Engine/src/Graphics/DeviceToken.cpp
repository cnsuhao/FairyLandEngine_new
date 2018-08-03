//============================================================================
// DeviceToken.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// DeviceToken Implementation
//----------------------------------------------------------------------------

DeviceToken::DeviceToken( )
{
	Clear( );
}

_void DeviceToken::Clear( )
{
	mWorldTransformToken		= 0;
	mViewTransformToken			= 0;
	mProjectionTransformToken	= 0;
	mTextureTransformToken[0]	= 0;
	mTextureTransformToken[1]	= 0;
	mTextureTransformToken[2]	= 0;
	mTextureTransformToken[3]	= 0;
	mViewportToken				= 0;
}