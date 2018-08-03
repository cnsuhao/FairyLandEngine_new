//============================================================================
// Region.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Region Implementation
//----------------------------------------------------------------------------

const Region Region::cNullRegion( Vector2( 0.0f, 0.0f ), Vector2( 0.0f, 0.0f ) );
const Region Region::cInitRegion( Vector2( Math::cMaxFloat, Math::cMaxFloat ), Vector2( Math::cMinFloat, Math::cMinFloat ) );

Region Region::Intersect( const Region& region1, const Region& region2 )
{
	if ( region1.Width( ) == 0.0f || region1.Height( ) == 0.0f )
		return region2;

	if ( region2.Width( ) == 0.0f || region2.Height( ) == 0.0f )
		return region1;

	_float l = Math::Max( region1.l, region2.l );

	if ( l >= region1.r || l >= region2.r )
		return cNullRegion;

	_float r = Math::Min( region1.r, region2.r );

	if ( r <= region1.l || r <= region2.l )
		return cNullRegion;

	_float t = Math::Max( region1.t, region2.t );

	if ( t >= region1.b || t >= region2.b )
		return cNullRegion;

	_float b = Math::Min( region1.b, region2.b );

	if ( b <= region1.t || b <= region2. t )
		return cNullRegion;

	return Region( l, t, r, b );
}

Region Region::Union( const Region& region1, const Region& region2 )
{
	if ( region1.Width( ) == 0.0f || region1.Height( ) == 0.0f )
		return region2;

	if ( region2.Width( ) == 0.0f || region2.Height( ) == 0.0f )
		return region1;

	_float l = Math::Min( region1.l, region2.l );
	_float r = Math::Max( region1.r, region2.r );
	_float t = Math::Min( region1.t, region2.t );
	_float b = Math::Max( region1.b, region2.b );

	return Region( l, t, r, b );
}