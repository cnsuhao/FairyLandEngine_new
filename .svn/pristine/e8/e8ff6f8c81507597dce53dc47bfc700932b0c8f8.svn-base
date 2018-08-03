//============================================================================
// Vector4.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Vector4 Implementation
//----------------------------------------------------------------------------

const Vector4 Vector4::cOrigin( 0.0f, 0.0f, 0.0f, 0.0f );
const Vector4 Vector4::cIdentity( 1.0f, 1.0f, 1.0f, 1.0f );
const Vector4 Vector4::cXAxis( 1.0f, 0.0f, 0.0f, 0.0f );
const Vector4 Vector4::cYAxis( 0.0f, 1.0f, 0.0f, 0.0f );
const Vector4 Vector4::cZAxis( 0.0f, 0.0f, 1.0f, 0.0f );
const Vector4 Vector4::cWAxis( 0.0f, 0.0f, 0.0f, 1.0f );
const Vector4 Vector4::cMaxVector( Math::cMaxFloat, Math::cMaxFloat, Math::cMaxFloat, Math::cMaxFloat );
const Vector4 Vector4::cMinVector( Math::cMinFloat, Math::cMinFloat, Math::cMinFloat, Math::cMinFloat );

Vector4& Vector4::operator *= ( const Matrix4& matrix )
{
	_float4* m1 = (_float4*) &x;
	_float4* m2 = (_float4*) &matrix.m;

	if ( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		_float4 temp;

		temp = _mm_mul_ps( _mm_set_ps1( x ), *( m2 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( y ), *( m2 + 1 ) ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( z ), *( m2 + 2 ) ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( w ), *( m2 + 3 ) ) );

		*m1 = temp;
	}
	else
	{
		_float xx = x, yy = y, zz = z, ww = w;

		x = xx * matrix( 0, 0 ) + yy * matrix( 1, 0 ) + zz * matrix( 2, 0 ) + ww * matrix( 3, 0 );
		y = xx * matrix( 0, 1 ) + yy * matrix( 1, 1 ) + zz * matrix( 2, 1 ) + ww * matrix( 3, 1 );
		z = xx * matrix( 0, 2 ) + yy * matrix( 1, 2 ) + zz * matrix( 2, 2 ) + ww * matrix( 3, 2 );
		w = xx * matrix( 0, 3 ) + yy * matrix( 1, 3 ) + zz * matrix( 2, 3 ) + ww * matrix( 3, 3 );
	}

	return *this;
}

Vector4 Vector4::operator * ( const Matrix4& matrix ) const
{
	Vector4 result;
	_float4* m1 = (_float4*) &x;
	_float4* m2 = (_float4*) &matrix.m;

	if ( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		_float4 temp;
		_float4* pointer = (_float4*) &result.x;

		temp = _mm_mul_ps( _mm_set_ps1( x ), *( m2 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( y ), *( m2 + 1 ) ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( z ), *( m2 + 2 ) ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( w ), *( m2 + 3 ) ) );

		if ( ( (_dword) pointer ) % 16 == 0 )
		{
			*pointer = temp;
		}
		else
		{
			_float* start = (_float*) &temp;
			result.x = *( start );
			result.y = *( start + 1 );
			result.z = *( start + 2 );
			result.w = *( start + 3 );
		}
	}
	else
	{
		result = *this;
		result *= matrix;
	}

	return result;
}

Vector4 Vector4::Project( const Vector4& vector1, const Vector4& vector2 )
{
	Vector4 normal = vector2;
	normal.Normalize( );

	return normal * Dot( vector1, normal );
}

Vector4 Vector4::Plumb( const Vector4& vector1, const Vector4& vector2 )
{
	return vector1 - Vector4::Project( vector1, vector2 );
}

Vector4 Vector4::Lerp( const Vector4& vector1, const Vector4& vector2, _float factor )
{
	factor = Math::Clamp( factor, 0.0f, 1.0f );

	return vector1 + ( vector2 - vector1 ) * factor;
}