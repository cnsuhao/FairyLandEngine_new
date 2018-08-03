//============================================================================
// RenderToken.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// RenderToken Implementation
//----------------------------------------------------------------------------

RenderToken::RenderToken( )
{
	Clear( );
}

_void RenderToken::Clear( )
{
	mOverlayNoneTexVN		= 0;
	mOverlayWithTexVN		= 0;
	mPrimitiveNoneTexVN		= 0;
	mPrimitiveWithTexVN		= 0;
	mBillboardVN			= 0;

	mWorldTransform			= _null;
	mViewTransform			= _null;
	mProjectionTransform	= _null;

	for ( _dword i = 0; i < Geometry::_MAX_TEXTURE_NUMBER; i ++ )
		mTextureTransform[i] = _null;

	mViewport				= _null;

	mRenderUnitNumber		= 0;
	mLastRenderUnitIndex	= -1;
}