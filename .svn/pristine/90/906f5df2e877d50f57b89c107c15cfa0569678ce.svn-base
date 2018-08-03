//============================================================================
// OverlayClipper.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// OverlayClipper Implementation
//----------------------------------------------------------------------------

_bool OverlayClipper::ClipPoint( const Region& region, OverlayPoint& point )
{
	return	point.mPosition.x >= region.mLeftTop.x && point.mPosition.x <= region.mRightBottom.x &&
			point.mPosition.y >= region.mLeftTop.y && point.mPosition.y <= region.mRightBottom.y;
}

_bool OverlayClipper::ClipLine( const Region& region, OverlayLine& line )
{	
	return	line.mPosition1.x >= region.mLeftTop.x && line.mPosition1.x <= region.mRightBottom.x &&
			line.mPosition1.y >= region.mLeftTop.y && line.mPosition1.y <= region.mRightBottom.y &&
			line.mPosition2.x >= region.mLeftTop.x && line.mPosition2.x <= region.mRightBottom.x &&
			line.mPosition2.y >= region.mLeftTop.y && line.mPosition2.y <= region.mRightBottom.y;
}

_bool OverlayClipper::ClipTriangle( const Region& region, OverlayTriangle triangle[], _dword& number )
{
	number = 1;

	return _true;
}

_bool OverlayClipper::ClipTriangle( const Region& region, OverlayTriangleImage triangle[], _dword& number )
{
	number = 1;

	return _true;
}

_bool OverlayClipper::ClipRectangle( const Region& region, OverlayRectangle& rectangle )
{
	if ( region == Region::cNullRegion )
		return _false;

	_float x1 = Math::Max( region.mLeftTop.x, rectangle.mPositionLT.x );

	if ( x1 >= region.mRightBottom.x || x1 >= rectangle.mPositionRB.x )
		return _false;

	_float x2 = Math::Min( region.mRightBottom.x, rectangle.mPositionRB.x );

	if ( x2 <= region.mLeftTop.x || x2 <= rectangle.mPositionLT.x )
		return _false;

	_float y1 = Math::Max( region.mLeftTop.y, rectangle.mPositionLT.y );

	if ( y1 >= region.mRightBottom.y || y1 >= rectangle.mPositionRB.y )
		return _false;

	_float y2 = Math::Min( region.mRightBottom.y, rectangle.mPositionRB.y );

	if ( y2 <= region.mLeftTop.y || y2 <= rectangle.mPositionLT.y )
		return _false;

	rectangle.mPositionLT.x = x1;
	rectangle.mPositionLT.y = y1;
	rectangle.mPositionRB.x = x2;
	rectangle.mPositionRB.y = y2;

	return _true;
}

_bool OverlayClipper::ClipRectangle( const Region& region, OverlayRectangleImage& rectangle )
{
	if ( region == Region::cNullRegion )
		return _false;

	_float x1 = Math::Max( region.mLeftTop.x, rectangle.mPositionLT.x );

	if ( x1 >= region.mRightBottom.x || x1 >= rectangle.mPositionRB.x )
		return _false;

	_float x2 = Math::Min( region.mRightBottom.x, rectangle.mPositionRB.x );

	if ( x2 <= region.mLeftTop.x || x2 <= rectangle.mPositionLT.x )
		return _false;

	_float y1 = Math::Max( region.mLeftTop.y, rectangle.mPositionLT.y );

	if ( y1 >= region.mRightBottom.y || y1 >= rectangle.mPositionRB.y )
		return _false;

	_float y2 = Math::Min( region.mRightBottom.y, rectangle.mPositionRB.y );

	if ( y2 <= region.mLeftTop.y || y2 <= rectangle.mPositionLT.y )
		return _false;

	_float dx = rectangle.mPositionRB.x - rectangle.mPositionLT.x;
	_float dy = rectangle.mPositionRB.y - rectangle.mPositionLT.y;
	_float du = rectangle.mTexcoordRB.x - rectangle.mTexcoordLT.x;
	_float dv = rectangle.mTexcoordRB.y - rectangle.mTexcoordLT.y;

	_float u1 = du / dx * ( x1 - rectangle.mPositionLT.x ) + rectangle.mTexcoordLT.x;
	_float u2 = du / dx * ( x2 - rectangle.mPositionLT.x ) + rectangle.mTexcoordLT.x;
	_float v1 = dv / dy * ( y1 - rectangle.mPositionLT.y ) + rectangle.mTexcoordLT.y;
	_float v2 = dv / dy * ( y2 - rectangle.mPositionLT.y ) + rectangle.mTexcoordLT.y;

	rectangle.mPositionLT.x = x1;
	rectangle.mPositionLT.y = y1;
	rectangle.mPositionRB.x = x2;
	rectangle.mPositionRB.y = y2;

	rectangle.mTexcoordLT.x = u1;
	rectangle.mTexcoordLT.y = v1;
	rectangle.mTexcoordRB.x = u2;
	rectangle.mTexcoordRB.y = v2;

	return _true;
}

_bool OverlayClipper::ClipQuadrangle( const Region& region, OverlayQuadrangle& quadrangle )
{
	if ( region == Region::cNullRegion )
		return _false;

	_float x1 = Math::Max( region.mLeftTop.x, quadrangle.mPositionLT.x );

	if ( x1 >= region.mRightBottom.x || x1 >= quadrangle.mPositionRB.x )
		return _false;

	_float x2 = Math::Min( region.mRightBottom.x, quadrangle.mPositionRB.x );

	if ( x2 <= region.mLeftTop.x || x2 <= quadrangle.mPositionLT.x )
		return _false;

	_float y1 = Math::Max( region.mLeftTop.y, quadrangle.mPositionLT.y );

	if ( y1 >= region.mRightBottom.y || y1 >= quadrangle.mPositionRB.y )
		return _false;

	_float y2 = Math::Min( region.mRightBottom.y, quadrangle.mPositionRB.y );

	if ( y2 <= region.mLeftTop.y || y2 <= quadrangle.mPositionLT.y )
		return _false;

	quadrangle.mPositionLT.x = x1;
	quadrangle.mPositionLT.y = y1;
	quadrangle.mPositionRT.x = x2;
	quadrangle.mPositionRT.y = y1;

	quadrangle.mPositionLB.x = x1;
	quadrangle.mPositionLB.y = y2;
	quadrangle.mPositionRB.x = x2;
	quadrangle.mPositionRB.y = y2;

	return _true;
}

_bool OverlayClipper::ClipQuadrangle( const Region& region, OverlayQuadrangleImage& quadrangle )
{
	if ( region == Region::cNullRegion )
		return _false;

	_float x1 = Math::Max( region.mLeftTop.x, quadrangle.mPositionLT.x );

	if ( x1 >= region.mRightBottom.x || x1 >= quadrangle.mPositionRB.x )
		return _false;

	_float x2 = Math::Min( region.mRightBottom.x, quadrangle.mPositionRB.x );

	if ( x2 <= region.mLeftTop.x || x2 <= quadrangle.mPositionLT.x )
		return _false;

	_float y1 = Math::Max( region.mLeftTop.y, quadrangle.mPositionLT.y );

	if ( y1 >= region.mRightBottom.y || y1 >= quadrangle.mPositionRB.y )
		return _false;

	_float y2 = Math::Min( region.mRightBottom.y, quadrangle.mPositionRB.y );

	if ( y2 <= region.mLeftTop.y || y2 <= quadrangle.mPositionLT.y )
		return _false;

	_float dx = quadrangle.mPositionRB.x - quadrangle.mPositionLT.x;
	_float dy = quadrangle.mPositionRB.y - quadrangle.mPositionLT.y;
	_float du = quadrangle.mTexcoordRB.x - quadrangle.mTexcoordLT.x;
	_float dv = quadrangle.mTexcoordRB.y - quadrangle.mTexcoordLT.y;

	_float u1 = du / dx * ( x1 - quadrangle.mPositionLT.x ) + quadrangle.mTexcoordLT.x;
	_float u2 = du / dx * ( x2 - quadrangle.mPositionLT.x ) + quadrangle.mTexcoordLT.x;
	_float v1 = dv / dy * ( y1 - quadrangle.mPositionLT.y ) + quadrangle.mTexcoordLT.y;
	_float v2 = dv / dy * ( y2 - quadrangle.mPositionLT.y ) + quadrangle.mTexcoordLT.y;

	quadrangle.mPositionLT.x = x1;
	quadrangle.mPositionLT.y = y1;
	quadrangle.mPositionRT.x = x2;
	quadrangle.mPositionRT.y = y1;

	quadrangle.mPositionLB.x = x1;
	quadrangle.mPositionLB.y = y2;
	quadrangle.mPositionRB.x = x2;
	quadrangle.mPositionRB.y = y2;

	quadrangle.mTexcoordLT.x = u1;
	quadrangle.mTexcoordLT.y = v1;
	quadrangle.mTexcoordRT.x = u2;
	quadrangle.mTexcoordRT.y = v1;

	quadrangle.mTexcoordLB.x = u1;
	quadrangle.mTexcoordLB.y = v2;
	quadrangle.mTexcoordRB.x = u2;
	quadrangle.mTexcoordRB.y = v2;

	return _true;
}