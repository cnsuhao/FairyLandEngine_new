//============================================================================
// Color.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Color Implementation
//----------------------------------------------------------------------------

const Color Color::cNull( 0x00000000 );

const Color Color::cRed( 0xFFFF0000 );
const Color Color::cDarkRed( 0xFF8B0000 );
const Color Color::cIndianRed( 0xFFCD5C5C );
const Color Color::cOrangeRed( 0xFFFF4500 );

const Color Color::cGreen( 0xFF00FF00 );
const Color Color::cDarkGreen( 0xFF008B00 );
const Color Color::cLawnGreen( 0xFF7CFC00 );
const Color Color::cSeaGreen( 0xFF2E8B57 );

const Color Color::cBlue( 0xFF0000FF );
const Color Color::cDarkBlue( 0xFF00008B );
const Color Color::cSkyBlue( 0xFF00BFFF );
const Color Color::cMidnightBlue( 0xFF191970 );

const Color Color::cWhite( 0xFFFFFFFF );
const Color Color::cBlack( 0xFF000000 );
const Color Color::cGray( 0xFF808080 );
const Color Color::cSilver( 0xFFC0C0C0 );

const Color Color::cYellow( 0xFFFFFF00 );
const Color Color::cPurple( 0xFF800080 );
const Color Color::cTeal( 0xFF008080 );
const Color Color::cLime( 0xFF00FF00 );
const Color Color::cTomato( 0xFFFF6347 );
const Color Color::cOrange( 0xFFFFA500 );
const Color Color::cBrown( 0xFFA52A2A );
const Color Color::cBisque( 0xFFFFE4C4 );
const Color Color::cPink( 0xFFFFC0CB );
const Color Color::cViolet( 0xFFEE82EE );
const Color Color::cMagenta( 0xFFFF00FF );
const Color Color::cOrchid( 0xFFDA70D6 );
const Color Color::cChocolate( 0xFFD2691E );

_void Color::ToHSL( _float& h, _float& s, _float& l )
{
	h = 0;
    s = 0;
	l = 0;

	_float v, m, vm, r2, g2, b2;
    v = Math::Max( r, g );
	v = Math::Max( v, b );
	m = Math::Min( r, g );
	m = Math::Min( m, b );
    l = ( m + v ) / 2.0f;
    if ( l <= 0.0 ) 
    	return;    
    
    vm = v - m;
    s = vm;
    if ( s > 0.0f ) 
		s /= ( l <= 0.5f ) ? ( v + m ) : ( 2.0f - v - m );
    else 
		return;
    
    r2 = ( v - r ) / vm;
    g2 = ( v - g ) / vm;
    b2 = ( v - b ) / vm;
    if ( r == v ) 
		h = ( g == m ? 5.0f + b2 : 1.0f - g2 );
    else if ( g == v ) 
		h = ( b == m ? 1.0f + r2 : 3.0f - b2 );
    else 
		h = ( r == m ? 3.0f + g2 : 5.0f - r2 );
    h /= 6.0f;
}

_void Color::FromHSL( _float h, _float s, _float l )
{
	const _float rgbrange = 255.0f;
	
	h *= rgbrange * 6.0f;
	s *= rgbrange;
	l *= rgbrange;

	r = Math::Min( rgbrange, Math::Max( 0.0f, Math::Abs( 3.0f * rgbrange - Math::Fmod( h, 6.0f * rgbrange ) ) - rgbrange ) );
	g = Math::Min( rgbrange, Math::Max( 0.0f, Math::Abs( 3.0f * rgbrange - Math::Fmod( h + 4.0f * rgbrange, 6.0f * rgbrange ) ) - rgbrange ) );
	b = Math::Min( rgbrange, Math::Max( 0.0f, Math::Abs( 3.0f * rgbrange - Math::Fmod( h + 2.0f * rgbrange, 6.0f * rgbrange ) ) - rgbrange ) );

	if ( l > rgbrange / 2.0f )
	{
		r = ( l * 2.0f / rgbrange - 1.0f ) * ( rgbrange - r ) + r;
		g = ( l * 2.0f / rgbrange - 1.0f ) * ( rgbrange - g ) + g;
		b = ( l * 2.0f / rgbrange - 1.0f ) * ( rgbrange - b ) + b;
	}
	else 
	{
		r *= l * 2.0f / rgbrange;
		g *= l * 2.0f / rgbrange;
		b *= l * 2.0f / rgbrange;
	}

	_float average = ( r + g + b ) / 3.0f;
	r = ( 1.0f - s / rgbrange ) * ( average - r ) + r;
	g = ( 1.0f - s / rgbrange ) * ( average - g ) + g;
	b = ( 1.0f - s / rgbrange ) * ( average - b ) + b;
	
	r = r / 255.0f;
	g = g / 255.0f;
	b = b / 255.0f;
}

Color Color::Dot( const Color& color1, const Color& color2 )
{
	_float color = Math::Clamp( color1.r * color2.r +
		color1.g * color2.g + color1.b * color2.b, 0.0f, 1.0f );

	return Color( color, color, color, color );
}

Color Color::Lerp( const Color& color1, const Color& color2, _float alpha )
{
	alpha = Math::Clamp( alpha, 0.0f, 1.0f );

	return Color(
		Math::Clamp( color1.r + ( color2.r - color1.r ) * alpha, 0.0f, 1.0f ),
		Math::Clamp( color1.g + ( color2.g - color1.g ) * alpha, 0.0f, 1.0f ),
		Math::Clamp( color1.b + ( color2.b - color1.b ) * alpha, 0.0f, 1.0f ),
		Math::Clamp( color1.a + ( color2.a - color1.a ) * alpha, 0.0f, 1.0f ) );
}
