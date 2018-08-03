//============================================================================
// Rect.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Rect Implementation
//----------------------------------------------------------------------------

const Rect Rect::cNullRect( 0, 0, 0, 0 );

Rect Rect::Intersect( const Rect& rect1, const Rect& rect2 )
{
	if ( rect1.Width( ) == 0 || rect1.Height( ) == 0 )
		return rect2;

	if ( rect2.Width( ) == 0 || rect2.Height( ) == 0 )
		return rect1;

	_long l = Math::Max( rect1.l, rect2.l );

	if ( l >= rect1.r || l >= rect2.r )
		return cNullRect;

	_long r = Math::Min( rect1.r, rect2.r );

	if ( r <= rect1.l || r <= rect2.l )
		return cNullRect;

	_long t = Math::Max( rect1.t, rect2.t );

	if ( t >= rect1.b || t >= rect2.b )
		return cNullRect;

	_long b = Math::Min( rect1.b, rect2.b );

	if ( b <= rect1.t || b <= rect2. t )
		return cNullRect;

	return Rect( l, t, r, b );
}

Rect Rect::Union( const Rect& rect1, const Rect& rect2 )
{
	if ( rect1.Width( ) == 0 || rect1.Height( ) == 0 )
		return rect2;

	if ( rect2.Width( ) == 0 || rect2.Height( ) == 0 )
		return rect1;

	_long l = Math::Min( rect1.l, rect2.l );
	_long r = Math::Max( rect1.r, rect2.r );
	_long t = Math::Min( rect1.t, rect2.t );
	_long b = Math::Max( rect1.b, rect2.b );

	return Rect( l, t, r, b );
}