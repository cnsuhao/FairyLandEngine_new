//============================================================================
// Math.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Math Implementation
//----------------------------------------------------------------------------

const _float Math::cZero		=  0.0f;
const _float Math::cOne			=  1.0f;
const _float Math::cPi			=  3.141592654f;
const _float Math::c2Pi			=  6.283185308f;
const _float Math::cE			=  2.718281828f;
const _float Math::cMaxFloat	=  3.402823466e+38f;
const _float Math::cMinFloat	= -3.402823466e+38f;
const _float Math::cEpsilon		=  0.00001f;

_float Math::Ceil( _float value )
{
	return ::ceilf( value );
}

_float Math::Floor( _float value )
{
	return ::floorf( value );
}

_float Math::Fmod( _float valuex, _float valuey )
{
	return ::fmodf( valuex, valuey );
}

_float Math::Lerp( _float value1, _float value2, _float factor )
{
	return value1 + ( value2 - value1 ) * Clamp( factor, 0.0f, 1.0f );
}

_long Math::Round( _float value )
{
	return (_long) ( value + 0.5f );
}

_float Math::Sqrt( _float value )
{
	return ::sqrtf( value );
}

_float Math::Exp( _float value )
{
	return ::expf( value );
}

_float Math::Log( _float value )
{
	return ::logf( value );
}

_float Math::Pow( _float base, _float exponent )
{
	return ::powf( base, exponent );
}

_float Math::WrapPi( _float radian )
{
	radian += cPi;
	radian -= Floor( radian / c2Pi ) * c2Pi;
	radian += cPi;

	return radian;
}

_float Math::Sin( _float radian )
{
	return ::sinf( radian );
}

_float Math::Cos( _float radian )
{
	return ::cosf( radian );
}

_float Math::Tan( _float radian )
{
	return ::tanf( radian );
}

_float Math::Cot( _float radian )
{
	return 1.0f / ::tanf( radian );
}

_float Math::Asin( _float value )
{
	if ( value > 1.0f )
		return cPi / 2.0f;

	if ( value < -1.0f )
		return cPi / -2.0f;

	return ::asinf( value );
}

_float Math::Acos( _float value )
{
	if ( value > 1.0f )
		return 0.0f;

	if ( value < -1.0f )
		return cPi;

	return ::acosf( value );
}

_float Math::Atan( _float value )
{
	return ::atanf( value );
}

_float Math::Atan( _float valuey, _float valuex )
{
	return ::atan2f( valuey, valuex );
}

_void Math::RandomSeed( _dword seed )
{
	GetFairyLandGlobal( ).mRandomSeed = seed;
}

_dword Math::Random( )
{
	_dword& seed = GetFairyLandGlobal( ).mRandomSeed;

	return ( ( seed = seed * 214013L + 2531011L ) >> 16 ) & 0x7fff;
}

_dword Math::Random( _dword maxvalue )
{
	if ( maxvalue == 0 )
		return 0;

	if ( maxvalue > 9 )
		return (_dword) ( ( Random( ) / 32767.0f ) * (_float) ( maxvalue - 1 ) + 0.5f );
	else
		return Random( ) % maxvalue;
}

_dword Math::Random( _dword minvalue, _dword maxvalue )
{
	if ( maxvalue > minvalue )
		return Random( maxvalue - minvalue + 1 ) + minvalue;
	else if ( maxvalue < minvalue )
		return Random( minvalue - maxvalue + 1 ) + maxvalue;
	else
		return minvalue;
}

_float Math::Random( _float minvalue, _float maxvalue )
{
	if ( maxvalue > minvalue )
		return Random( 10001 ) * 0.0001f * ( maxvalue - minvalue ) + minvalue;
	else if ( maxvalue < minvalue )
		return Random( 10001 ) * 0.0001f * ( minvalue - maxvalue ) + maxvalue;
	else
		return minvalue;
}

_long Math::RandomSign( )
{
	return Random( 2 ) == 0 ? -1 : 1;
}