//============================================================================
// RenderToken.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// RenderToken
//----------------------------------------------------------------------------

struct RenderToken
{
	_dword			mOverlayNoneTexVN;
	_dword			mOverlayWithTexVN;
	_dword			mPrimitiveNoneTexVN;
	_dword			mPrimitiveWithTexVN;
	_dword			mBillboardVN;

	const Matrix4*	mWorldTransform;
	const Matrix4*	mViewTransform;
	const Matrix4*	mProjectionTransform;
	const Matrix4*	mTextureTransform[ Geometry::_MAX_TEXTURE_NUMBER ];
	const Viewport*	mViewport;

	_dword			mRenderUnitNumber;
	_dword			mLastRenderUnitIndex;

	RenderToken( );

	_void Clear( );
};

};