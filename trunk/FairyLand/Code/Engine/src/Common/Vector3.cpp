//============================================================================
// Vector3.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Vector3 Implementation
//----------------------------------------------------------------------------

const Vector3 Vector3::cOrigin( 0.0f, 0.0f, 0.0f );
const Vector3 Vector3::cIdentity( 1.0f, 1.0f, 1.0f );
const Vector3 Vector3::cXAxis( 1.0f, 0.0f, 0.0f );
const Vector3 Vector3::cYAxis( 0.0f, 1.0f, 0.0f );
const Vector3 Vector3::cZAxis( 0.0f, 0.0f, 1.0f );
const Vector3 Vector3::cMaxVector( Math::cMaxFloat, Math::cMaxFloat, Math::cMaxFloat );
const Vector3 Vector3::cMinVector( Math::cMinFloat, Math::cMinFloat, Math::cMinFloat );

Vector3& Vector3::operator *= ( const Matrix3& matrix )
{
	_float xx = x, yy = y, zz = z;

	x = xx * matrix( 0, 0 ) + yy * matrix( 1, 0 ) + zz * matrix( 2, 0 );
	y = xx * matrix( 0, 1 ) + yy * matrix( 1, 1 ) + zz * matrix( 2, 1 );
	z = xx * matrix( 0, 2 ) + yy * matrix( 1, 2 ) + zz * matrix( 2, 2 );

	return *this;
}

Vector3& Vector3::operator *= ( const Matrix3x4& matrix )
{
	_float xx = x, yy = y, zz = z;

	x = xx * matrix( 0, 0 ) + yy * matrix( 0, 1 ) + zz * matrix( 0, 2 ) + matrix( 0, 3 );
	y = xx * matrix( 1, 0 ) + yy * matrix( 1, 1 ) + zz * matrix( 1, 2 ) + matrix( 1, 3 );
	z = xx * matrix( 2, 0 ) + yy * matrix( 2, 1 ) + zz * matrix( 2, 2 ) + matrix( 2, 3 );

	return *this;
}

Vector3& Vector3::operator *= ( const Matrix4& matrix )
{
	_float xx = x, yy = y, zz = z;

	_float w = xx * matrix( 0, 3 ) + yy * matrix( 1, 3 ) + zz * matrix( 2, 3 ) + matrix( 3, 3 );

	x = xx * matrix( 0, 0 ) + yy * matrix( 1, 0 ) + zz * matrix( 2, 0 ) + matrix( 3, 0 );
	y = xx * matrix( 0, 1 ) + yy * matrix( 1, 1 ) + zz * matrix( 2, 1 ) + matrix( 3, 1 );
	z = xx * matrix( 0, 2 ) + yy * matrix( 1, 2 ) + zz * matrix( 2, 2 ) + matrix( 3, 2 );

	if ( w != 0.0f )
	{
		x /= w;
		y /= w;
		z /= w;
	}

	return *this;
}

Vector3& Vector3::operator *= ( const Quaternion& quaternion )
{
	Quaternion tempquaternion = quaternion;
	tempquaternion *= Quaternion( x, y, z, 0.0f );
	tempquaternion *= - quaternion;

	x = tempquaternion.x;
	y = tempquaternion.y;
	z = tempquaternion.z;

	return *this;
}

Vector3 Vector3::operator * ( const Matrix3& matrix ) const
{
	Vector3 returnvector( *this );

	return returnvector *= matrix;
}

Vector3 Vector3::operator * ( const Matrix3x4& matrix ) const
{
	Vector3 returnvector( *this );

	return returnvector *= matrix;
}

Vector3 Vector3::operator * ( const Matrix4& matrix ) const
{
	Vector3 returnvector( *this );

	return returnvector *= matrix;
}

Vector3 Vector3::operator * ( const Quaternion& quaternion ) const
{
	Vector3 returnvector( *this );

	return returnvector *= quaternion;
}

Vector3& Vector3::Cartesian2Spherical( )
{
	_float xx = Math::Sqrt( x * x + y * y + z * z );
	_float yy = Math::Acos( z / xx );
	_float zz = Math::Asin( y / Math::Sqrt( x * x + y * y ) );

	if ( x < 0.0f )
		zz = Math::cPi - zz;

	if ( zz < 0.0f )
		zz += Math::c2Pi;

	x = xx;
	y = yy;
	z = zz;

	return *this;
}

Vector3& Vector3::Spherical2Cartesian( )
{
	_float xx = x * Math::Sin( y ) * Math::Cos( z );
	_float yy = x * Math::Sin( y ) * Math::Sin( z );
	_float zz = x * Math::Cos( y );

	x = xx;
	y = yy;
	z = zz;

	return *this;
}

Vector3& Vector3::Cartesian2Cylindrical( )
{
	_float xx = Math::Atan( y / x );
	_float yy = Math::Sqrt( x * x + y * y );

	if ( x < 0.0f )
		xx += Math::cPi;

	x = xx;
	y = yy;

	return *this;
}

Vector3& Vector3::Cylindrical2Cartesian( )
{
	_float xx = y * Math::Cos( x );
	_float yy = y * Math::Sin( x );

	x = xx;
	y = yy;

	return *this;
}

Vector3 Vector3::Project( const Vector3& vector1, const Vector3& vector2 )
{
	Vector3 normal = vector2;
	normal.Normalize( );

	return normal * Dot( vector1, normal );
}

Vector3 Vector3::Plumb( const Vector3& vector1, const Vector3& vector2 )
{
	return vector1 - Vector3::Project( vector1, vector2 );
}

Vector3 Vector3::Lerp( const Vector3& vector1, const Vector3& vector2, _float factor )
{
	factor = Math::Clamp( factor, 0.0f, 1.0f );

	return vector1 + ( vector2 - vector1 ) * factor;
}