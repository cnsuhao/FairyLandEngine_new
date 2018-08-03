//============================================================================
// OverlayClipper.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// OverlayClipper
//----------------------------------------------------------------------------

class OverlayClipper
{
public:
	static _bool	ClipPoint( const Region& region, OverlayPoint& point );
	static _bool	ClipLine( const Region& region, OverlayLine& line );
	static _bool	ClipTriangle( const Region& region, OverlayTriangle triangle[], _dword& number );
	static _bool	ClipTriangle( const Region& region, OverlayTriangleImage triangle[], _dword& number );
	static _bool	ClipRectangle( const Region& region, OverlayRectangle& rectangle );
	static _bool	ClipRectangle( const Region& region, OverlayRectangleImage& rectangle );
	static _bool	ClipQuadrangle( const Region& region, OverlayQuadrangle& quadrangle );
	static _bool	ClipQuadrangle( const Region& region, OverlayQuadrangleImage& quadrangle );
};

};