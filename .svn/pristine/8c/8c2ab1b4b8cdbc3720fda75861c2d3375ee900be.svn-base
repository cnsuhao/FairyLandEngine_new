//============================================================================
// RenderLayer.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// RenderLayer Implementation
//----------------------------------------------------------------------------

RenderLayer::RenderLayer( RenderLayer* parent ) : mParent( parent )
{
}

_void RenderLayer::AppendRenderUnit( RenderUnit& renderunit )
{
	switch ( renderunit.mLayer & 0x0000FFFF )
	{
		// Background layer.
		case IRenderDevice::_LAYER_BACKGROUND:
			mBackgroundUnitArray.Append( renderunit );
			break;
		// Environment layer.
		case IRenderDevice::_LAYER_ENVIRONMENT:
			mEnvironmentUnitArray.Append( renderunit );
			break;
		// Normal layer.
		case IRenderDevice::_LAYER_NORMAL:
			mNormalUnitArray.Append( renderunit );
			break;
		// Foreground layer.
		case IRenderDevice::_LAYER_FOREGROUND:
			mForegroundUnitArray.Append( renderunit );
			break;
		// Overlay layer.
		case IRenderDevice::_LAYER_OVERLAY:
			mOverlayUnitArray.Append( renderunit );
			break;
	}
}

_void RenderLayer::SortRenderUnit( const Frustum& frustum )
{
	if ( mEnvironmentUnitArray.Number( ) > 1 )
		SortRenderUnit( frustum, mEnvironmentUnitArray );

	if ( mNormalUnitArray.Number( ) > 1 )
		SortRenderUnit( frustum, mNormalUnitArray );

	if ( mForegroundUnitArray.Number( ) > 1 )
		SortRenderUnit( frustum, mForegroundUnitArray );
}

_void RenderLayer::SortRenderUnit( const Frustum& frustum, RenderUnitArray& renderunitarray )
{
	RenderUnitArray temparray = renderunitarray;

	RenderSortUnitArray normalarray;
	RenderSortUnitArray decalarray;
	RenderSortUnitArray colorkeyarray;
	RenderSortUnitArray alphaarray;

	const Plane& viewplane = frustum.mPlanes[0];

	_float frustumdepth = frustum.Depth( );

	// Compute render priority for each renderable.
	for ( _dword i = 0; i < renderunitarray.Number( ); i ++ )
	{
		RenderUnit& renderunit = renderunitarray[i];

		RenderSortUnit unit;
		unit.mIndex		= i;
		unit.mPriority	= - viewplane.Distance( renderunit.mRenderCenter );

		switch ( renderunit.mTechnique.GetRenderOrder( ) )
		{
			case IRenderMethod::_RENDERORDER_NORMAL:
				unit.mPriority = frustumdepth - unit.mPriority;
				normalarray.Append( unit );
				break;
			case IRenderMethod::_RENDERORDER_DECAL:
				decalarray.Append( unit );
				break;
			case IRenderMethod::_RENDERORDER_COLORKEY:
				colorkeyarray.Append( unit );
				break;
			case IRenderMethod::_RENDERORDER_ALPHA:
				alphaarray.Append( unit );
				break;
		}
	}

	renderunitarray.Clear( );

	if ( normalarray.Number( ) > 0 )
	{
		QuickSort< RenderSortUnit > quicksort;
		quicksort.SortDescending( (RenderSortUnit*) normalarray.GetArrayBuffer( ), normalarray.Number( ) );

		for ( _dword i = 0; i < normalarray.Number( ); i ++ )
			renderunitarray.Append( temparray[ normalarray[i].mIndex ] );
	}

	if ( decalarray.Number( ) > 0 )
	{
		for ( _dword i = 0; i < decalarray.Number( ); i ++ )
			renderunitarray.Append( temparray[ decalarray[i].mIndex ] );
	}

	if ( colorkeyarray.Number( ) > 0 )
	{
		QuickSort< RenderSortUnit > quicksort;
		quicksort.SortDescending( (RenderSortUnit*) colorkeyarray.GetArrayBuffer( ), colorkeyarray.Number( ) );

		for ( _dword i = 0; i < colorkeyarray.Number( ); i ++ )
			renderunitarray.Append( temparray[ colorkeyarray[i].mIndex ] );
	}

	if ( alphaarray.Number( ) > 0 )
	{
		QuickSort< RenderSortUnit > quicksort;
		quicksort.SortDescending( (RenderSortUnit*) alphaarray.GetArrayBuffer( ), alphaarray.Number( ) );

		for ( _dword i = 0; i < alphaarray.Number( ); i ++ )
			renderunitarray.Append( temparray[ alphaarray[i].mIndex ] );
	}
}

_void RenderLayer::Clear( )
{
	mBackgroundUnitArray.Clear( );
	mEnvironmentUnitArray.Clear( );
	mNormalUnitArray.Clear( );
	mForegroundUnitArray.Clear( );
	mOverlayUnitArray.Clear( );
}