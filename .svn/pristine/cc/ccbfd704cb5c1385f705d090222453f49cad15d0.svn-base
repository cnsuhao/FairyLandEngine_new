//============================================================================
// RenderUnit.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

class RenderLayer;

//----------------------------------------------------------------------------
// RenderUnit
//----------------------------------------------------------------------------

struct RenderUnit
{
	const RenderLayer*	mSubLayer;
	const Renderable*	mRenderable;
	const Geometry*		mGeometry;

	const Matrix4*		mWorldTransform;
	const Matrix4*		mViewTransform;
	const Matrix4*		mProjectionTransform;
	const Matrix4*		mTextureTransform[ Geometry::_MAX_TEXTURE_NUMBER ];
	const Viewport*		mViewport;

	Vector3				mRenderCenter;

	TechniqueUnit		mTechnique;

	Renderable*			mLinkHead;
	Renderable*			mLinkTail;
	_dword				mLinkNumber;

	_dword				mLayer;

	inline RenderUnit( )
	{
		mSubLayer				= _null;
		mRenderable				= _null;
		mGeometry				= _null;

		mWorldTransform			= &Matrix4::cIdentity;
		mViewTransform			= &Matrix4::cIdentity;
		mProjectionTransform	= &Matrix4::cIdentity;
		mTextureTransform[0]	= &Matrix4::cIdentity;
		mTextureTransform[1]	= &Matrix4::cIdentity;
		mTextureTransform[2]	= &Matrix4::cIdentity;
		mTextureTransform[3]	= &Matrix4::cIdentity;
		mTextureTransform[4]	= &Matrix4::cIdentity;
		mTextureTransform[5]	= &Matrix4::cIdentity;
		mViewport				= _null;

		mRenderCenter			= Vector3::cOrigin;

		mLinkHead				= _null;
		mLinkTail				= _null;
		mLinkNumber				= 0;

		mLayer					= 0;
	}
};

typedef Array< RenderUnit > RenderUnitArray;

//----------------------------------------------------------------------------
// RenderSortUnit
//----------------------------------------------------------------------------

struct RenderSortUnit
{
	_float		mPriority;
	_dword		mIndex;

	inline operator _float ( ) const
		{ return mPriority; }
};

typedef Array< RenderSortUnit, _float > RenderSortUnitArray;

};