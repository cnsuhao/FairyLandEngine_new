//============================================================================
// RenderStatistics.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// RenderStatistics Implementation
//----------------------------------------------------------------------------

RenderStatistics::RenderStatistics( )
{
	mFramePerSecond		= 0.0f;
	mFrameTickcount		= System::GetCurrentTickcount( );
	mFrameNumber		= 0;
	mTotalFrameCount	= 1;
	mTextureMemoryUsed	= 0;
	mVertexMemoryUsed	= 0;
	mIndexMemoryUsed	= 0;
	mPolygonNumber		= 0;
	mVertexNumber		= 0;
	mUseTextureTimes	= 0;
	mDrawPrimitiveTimes	= 0;
}

RenderStatistics::~RenderStatistics( )
{
}

_void RenderStatistics::BeforePresent( )
{
	mTextureMemoryUsed	= 0;
	mVertexMemoryUsed	= 0;
	mIndexMemoryUsed	= 0;
	mPolygonNumber		= 0;
	mVertexNumber		= 0;
	mUseTextureTimes	= 0;
	mDrawPrimitiveTimes	= 0;
}

_void RenderStatistics::AfterPresent( )
{
	_dword tickcount = System::GetCurrentTickcount( );

	mFrameNumber ++;
	mTotalFrameCount ++;

	// Update FPS every 500 ms.
	if ( mFrameTickcount + 500 <= tickcount )
	{
		mFramePerSecond	= (_float) mFrameNumber * 1000.0f / (_float) ( tickcount - mFrameTickcount );

		mFrameTickcount	= tickcount;
		mFrameNumber	= 0;
	}
}

_void RenderStatistics::DrawPrimitive( _dword vertex, _dword polygon )
{
	mVertexNumber	+= vertex;
	mPolygonNumber	+= polygon;

	mDrawPrimitiveTimes ++;
}

_float RenderStatistics::GetFramePerSecond( ) const
{
	return mFramePerSecond;
}

_dword RenderStatistics::GetTextureMemoryUsed( ) const
{
	return mTextureMemoryUsed;
}

_dword RenderStatistics::GetVertexMemoryUsed( ) const
{
	return mVertexMemoryUsed;
}

_dword RenderStatistics::GetIndexMemoryUsed( ) const
{
	return mIndexMemoryUsed;
}

_dword RenderStatistics::GetPolygonNumber( ) const
{
	return mPolygonNumber;
}

_dword RenderStatistics::GetVertexNumber( ) const
{
	return mVertexNumber;
}

_dword RenderStatistics::GetUseTextureTimes( ) const
{
	return mUseTextureTimes;
}

_dword RenderStatistics::GetDrawPrimitiveTimes( ) const
{
	return mDrawPrimitiveTimes;
}