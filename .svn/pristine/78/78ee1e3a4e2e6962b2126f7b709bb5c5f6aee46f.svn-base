//============================================================================
// Vector2.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Vector2 Implementation
//----------------------------------------------------------------------------

const Vector2 Vector2::cOrigin( 0.0f, 0.0f );
const Vector2 Vector2::cIdentity( 1.0f, 1.0f );
const Vector2 Vector2::cXAxis( 1.0f, 0.0f );
const Vector2 Vector2::cYAxis( 0.0f, 1.0f );
const Vector2 Vector2::cMaxVector( Math::cMaxFloat, Math::cMaxFloat );
const Vector2 Vector2::cMinVector( Math::cMinFloat, Math::cMinFloat );

Vector2& Vector2::operator *= ( const Matrix2& matrix )
{
	_float xx = x, yy = y;

	x = xx * matrix( 0, 0 ) + yy * matrix( 1, 0 );
	y = xx * matrix( 0, 1 ) + yy * matrix( 1, 1 );

	return *this;
}

Vector2& Vector2::operator *= ( const Matrix3& matrix )
{
	_float xx = x, yy = y;

	x = xx * matrix( 0, 0 ) + yy * matrix( 1, 0 ) + matrix( 2, 0 );
	y = xx * matrix( 0, 1 ) + yy * matrix( 1, 1 ) + matrix( 2, 1 );

	return *this;
}

Vector2 Vector2::operator * ( const Matrix2& matrix ) const
{
	Vector2 returnvector( *this );

	return returnvector *= matrix;
}

Vector2 Vector2::operator * ( const Matrix3& matrix ) const
{
	Vector2 returnvector( *this );

	return returnvector *= matrix;
}

Vector2 Vector2::Project( const Vector2& vector1, const Vector2& vector2 )
{
	Vector2 normal = vector2;
	normal.Normalize( );

	return normal * Dot( vector1, normal );
}

Vector2 Vector2::Plumb( const Vector2& vector1, const Vector2& vector2 )
{
	return vector1 - Vector2::Project( vector1, vector2 );
}

Vector2 Vector2::Lerp( const Vector2& vector1, const Vector2& vector2, _float factor )
{
	factor = Math::Clamp( factor, 0.0f, 1.0f );

	return vector1 + ( vector2 - vector1 ) * factor;
}