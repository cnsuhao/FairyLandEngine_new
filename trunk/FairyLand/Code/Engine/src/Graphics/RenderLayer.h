//============================================================================
// RenderLayer.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// RenderLayer
//----------------------------------------------------------------------------

class RenderLayer
{
private:
	RenderLayer*		mParent;

	RenderUnitArray		mBackgroundUnitArray;
	RenderUnitArray		mEnvironmentUnitArray;
	RenderUnitArray		mNormalUnitArray;
	RenderUnitArray		mForegroundUnitArray;
	RenderUnitArray		mOverlayUnitArray;

public:
	RenderLayer( RenderLayer* parent );

	inline RenderLayer* ParentLayer( ) const
		{ return mParent; }

	inline RenderUnitArray& GetBackgroundUnitArray( )
		{ return mBackgroundUnitArray; }

	inline RenderUnitArray& GetEnvironmentUnitArray( )
		{ return mEnvironmentUnitArray; }

	inline RenderUnitArray& GetNormalUnitArray( )
		{ return mNormalUnitArray; }

	inline RenderUnitArray& GetForegroundUnitArray( )
		{ return mForegroundUnitArray; }

	inline RenderUnitArray& GetOverlayUnitArray( )
		{ return mOverlayUnitArray; }

	_void	AppendRenderUnit( RenderUnit& renderunit );
	_void	SortRenderUnit( const Frustum& frustum );
	_void	SortRenderUnit( const Frustum& frustum, RenderUnitArray& renderunitarray );

	_void	Clear( );
};

};