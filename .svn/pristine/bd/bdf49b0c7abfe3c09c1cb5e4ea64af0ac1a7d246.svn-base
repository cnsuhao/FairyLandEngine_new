//============================================================================
// Vector3.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

class Matrix3;
class Matrix3x4;
class Matrix4;
class Quaternion;

//----------------------------------------------------------------------------
// Vector3
//----------------------------------------------------------------------------

//! This class represents a 3D vector.
class Vector3
{
public:
	//! The origin vector.
	const static Vector3 cOrigin;
	//! The identity vector.
	const static Vector3 cIdentity;
	//! The x-axis vector.
	const static Vector3 cXAxis;
	//! The y-axis vector.
	const static Vector3 cYAxis;
	//! The z-axis vector.
	const static Vector3 cZAxis;
	//! The maximal vector.
	const static Vector3 cMaxVector;
	//! The minimal vector.
	const static Vector3 cMinVector;

	//! X-component of the vector.
	_float	x;
	//! Y-component of the vector.
	_float	y;
	//! Z-component of the vector.
	_float	z;

public:
	//! Constructor, create a vector without sets x, y, z components.
	//! @param		none
	inline Vector3( );
	//! Constructor, create a vector and sets x, y, z components.
	//! @param		xx			X-component of the vector.
	//! @param		yy			Y-component of the vector.
	//! @param		zz			Z-component of the vector.
	inline Vector3( _float xx, _float yy, _float zz );
	//! Constructor, create a vector and sets x, y, z components from a float4.
	//! @param		ssevalue	A float4.
	inline Vector3( const _float4& ssevalue );

	//! Get positive vector.
	//! @param		none
	//! @return		The positive vector.
	inline Vector3 operator + ( ) const;
	//! Get negative vector.
	//! @param		none
	//! @return		The negative vector.
	inline Vector3 operator - ( ) const;

	//! Add a vector to current vector, and save result in current one.
	//! @param		vector		The vector to add.
	//! @return		The result vector of addition.
	inline Vector3& operator += ( const Vector3& vector );
	//! Subtract a vector from current vector, and save result in current one.
	//! @param		vector		The vector to subtract.
	//! @return		The result vector of subtraction.
	inline Vector3& operator -= ( const Vector3& vector );
	//! Multiply a scalar to current vector, and save result in current one.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result vector of multiplication.
	inline Vector3& operator *= ( _float scalar );
	//! Multiply a vector to current vector, and save result in current one.
	//! @param		vector		The vector to multiply.
	//! @return		The result vector of multiplication.
	inline Vector3& operator *= ( const Vector3& vector );
	//! Divide a scalar to current vector, and save result in current one.
	//! @param		scalar		The scalar to divide.
	//! @return		The result vector of division.
	inline Vector3& operator /= ( _float scalar );
	//! Divide a vector to current vector, and save result in current one.
	//! @param		vector		The vector to divide.
	//! @return		The result vector of division.
	inline Vector3& operator /= ( const Vector3& vector );

	//! Multiply a matrix to current vector, and save result in current one.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector3& operator *= ( const Matrix3& matrix );
	//! Multiply a matrix to current vector, and save result in current one.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector3& operator *= ( const Matrix3x4& matrix );
	//! Multiply a matrix to current vector, and save result in current one.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector3& operator *= ( const Matrix4& matrix );
	//! Multiply a quaternion to current vector, and save result in current one.
	//! @param		matrix		The quaternion to multiply.
	//! @return		The result vector of multiplication.
	Vector3& operator *= ( const Quaternion& quaternion );

	//! Add a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to add.
	//! @return		The result vector of addition.
	inline Vector3 operator + ( const Vector3& vector ) const;
	//! Subtract a vector from current vector, return a new vector to save result.
	//! @param		vector		The vector to subtract.
	//! @return		The result vector of subtraction.
	inline Vector3 operator - ( const Vector3& vector ) const;
	//! Multiply a scalar to current vector, return a new vector to save result.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result vector of multiplication.
	inline Vector3 operator * ( _float scalar ) const;
	//! Multiply a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to multiply.
	//! @return		The result vector of multiplication.
	inline Vector3 operator * ( const Vector3& vector ) const;
	//! Divide a scalar to current vector, return a new vector to save result.
	//! @param		scalar		The scalar to divide.
	//! @return		The result vector of division.
	inline Vector3 operator / ( _float scalar ) const;
	//! Divide a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to divide.
	//! @return		The result vector of division.
	inline Vector3 operator / ( const Vector3& vector ) const;

	//! Compare two vectors.
	//! @param		vector		The vector to compare with.
	//! @return		True if they are identical, false otherwise.
	inline _bool operator == ( const Vector3& vector ) const;
	//! Compare two vectors.
	//! @param		vector		The vector to compare with.
	//! @return		True if they are not identical, false otherwise.
	inline _bool operator != ( const Vector3& vector ) const;

	//! Multiply a matrix to current vector, return a new vector to save result.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector3 operator * ( const Matrix3& matrix ) const;
	//! Multiply a matrix to current vector, return a new vector to save result.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector3 operator * ( const Matrix3x4& matrix ) const;
	//! Multiply a matrix to current vector, return a new vector to save result.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector3 operator * ( const Matrix4& matrix ) const;
	//! Multiply a matrix to current vector, return a new vector to save result.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector3 operator * ( const Quaternion& quaternion ) const;

	//! Compare two vectors with error margin.
	//! @param		vector		The vector to compare with.
	//! @param		margin		The margin that allows some error.
	//! @return		True if they are identical, false otherwise.
	inline _bool Equal( const Vector3& vector, _float margin = Math::cEpsilon ) const;

	//! Make the current vector absolute, that all coordinates are nonnegative.
	//! @param		none
	//! @return		The absolute vector.
	inline Vector3& Absolute( );
	//! Make the current vector normalized, that has one unit magnitude.
	//! @param		none
	//! @return		The normalized vector.
	inline Vector3& Normalize( );

	//! Convert vector from cartesian to spherical coordinates.
	//! @param		none
	//! The spherical coordinates vector, in rho, phi, theta format.
	Vector3& Cartesian2Spherical( );
	//! Convert vector from spherical to cartesian coordinates.
	//! @param		none
	//! The spherical coordinates vector, in x, y, z format.
	Vector3& Spherical2Cartesian( );

	//! Convert vector from cartesian to cylindrical coordinates.
	//! @param		none
	//! The spherical coordinates vector, in rho, phi, z format.
	Vector3& Cartesian2Cylindrical( );
	//! Convert vector from cylindrical to cartesian coordinates.
	//! @param		none
	//! The spherical coordinates vector, in x, y, z format.
	Vector3& Cylindrical2Cartesian( );
	

	//! Calculates vector magnitude.
	//! @param		none
	//! @return		The magnitude of vector.
	inline _float Magnitude( ) const;
	//! Calculates the square of vector magnitude, this function is faster than Magnitude.
	//! @param		none
	//! @return		The square of magnitude of vector.
	inline _float MagnitudeSquared( ) const;

	//! Calculates dot product of two vectors.
	//! @param		vector1		Source vector 1.
	//! @param		vector2		Source vector 2.
	//! @return		The dot product result.
	inline static _float Dot( const Vector3& vector1, const Vector3& vector2 );
	//! Calculates cross product of two vectors.
	//! @param		vector1		Source vector 1.
	//! @param		vector2		Source vector 2.
	//! @return		The cross product result.
	inline static Vector3 Cross( const Vector3& vector1, const Vector3& vector2 );
	//! Calculates vector by project one vector to another.
	//! @param		vector1		The vector to be projected.
	//! @param		vector2		The project reference vector.
	//! @return		The projected vector, that has same direction of vector2.
	static Vector3 Project( const Vector3& vector1, const Vector3& vector2 );
	//! Calculates vector by plumb one vector to another.
	//! @param		vector1		The vector to be projected.
	//! @param		vector2		The project reference vector.
	//! @return		The plumbed vector, that is vertical of vector2.
	static Vector3 Plumb( const Vector3& vector1, const Vector3& vector2 );
	//! Calculates linear interpolation of two vectors.
	//! @param		vector1		Source vector 1.
	//! @param		vector2		Source vector 2.
	//! @param		factor		The interpolation factor, will clamp to between 0 to 1.
	//! @return		The result vector of interpolation, vector1 + ( vector2 - vector1 ) * factor.
	static Vector3 Lerp( const Vector3& vector1, const Vector3& vector2, _float factor );
};

//----------------------------------------------------------------------------
// Vector3 Implementation
//----------------------------------------------------------------------------

Vector3::Vector3( )
{
}

Vector3::Vector3( _float xx, _float yy, _float zz ) : x( xx ), y( yy ), z( zz )
{
}

Vector3::Vector3( const _float4& ssevalue )
{
	*this = *( (Vector3*) &ssevalue );
}

Vector3 Vector3::operator + ( ) const
{
	return Vector3( x, y, z );
}

Vector3 Vector3::operator - ( ) const
{
	return Vector3( -x, -y, -z );
}

Vector3& Vector3::operator += ( const Vector3& vector )
{
	x += vector.x;
	y += vector.y;
	z += vector.z;

	return *this;
}

Vector3& Vector3::operator -= ( const Vector3& vector )
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;

	return *this;
}

Vector3& Vector3::operator *= ( _float scalar )
{
	x *= scalar;
	y *= scalar;
	z *= scalar;

	return *this;
}

Vector3& Vector3::operator *= ( const Vector3& vector )
{
	x *= vector.x;
	y *= vector.y;
	z *= vector.z;

	return *this;
}

Vector3& Vector3::operator /= ( _float scalar )
{
	x /= scalar;
	y /= scalar;
	z /= scalar;

	return *this;
}

Vector3& Vector3::operator /= ( const Vector3& vector )
{
	x /= vector.x;
	y /= vector.y;
	z /= vector.z;

	return *this;
}

Vector3 Vector3::operator + ( const Vector3& vector ) const
{
	return Vector3( x + vector.x, y + vector.y, z + vector.z );
}

Vector3 Vector3::operator - ( const Vector3& vector ) const
{
	return Vector3( x - vector.x, y - vector.y, z - vector.z );
}

Vector3 Vector3::operator * ( _float scalar ) const
{
	return Vector3( x * scalar, y * scalar, z * scalar );
}

Vector3 Vector3::operator * ( const Vector3& vector ) const
{
	return Vector3( x * vector.x, y * vector.y, z * vector.z );
}

Vector3 Vector3::operator / ( _float scalar ) const
{
	return Vector3( x / scalar, y / scalar, z / scalar );
}

Vector3 Vector3::operator / ( const Vector3& vector ) const
{
	return Vector3( x / vector.x, y / vector.y, z / vector.z );
}

_bool Vector3::operator == ( const Vector3& vector ) const
{
	return x == vector.x && y == vector.y && z == vector.z;
}

_bool Vector3::operator != ( const Vector3& vector ) const
{
	return x != vector.x || y != vector.y || z != vector.z;
}

_bool Vector3::Equal( const Vector3& vector, _float margin ) const
{
	if ( Math::Abs( x - vector.x ) > margin )
		return _false;

	if ( Math::Abs( y - vector.y ) > margin )
		return _false;

	if ( Math::Abs( z - vector.z ) > margin )
		return _false;

	return _true;
}

Vector3& Vector3::Absolute( )
{
	if ( x < 0.0f )
		x = -x;

	if ( y < 0.0f )
		y = -y;

	if ( z < 0.0f )
		z = -z;

	return *this;
}

Vector3& Vector3::Normalize( )
{
	_float magnitude = Magnitude( );

	if ( magnitude > 0.0f )
	{
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}
	else
	{
		x = 1.0f;
		y = 0.0f;
		z = 0.0f;
	}

	return *this;
}

_float Vector3::Magnitude( ) const
{
	return Math::Sqrt( x * x + y * y + z * z );
}

_float Vector3::MagnitudeSquared( ) const
{
	return x * x + y * y + z * z;
}

_float Vector3::Dot( const Vector3& vector1, const Vector3& vector2 )
{
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

Vector3 Vector3::Cross( const Vector3& vector1, const Vector3& vector2 )
{
	return Vector3(
		vector1.y * vector2.z - vector1.z * vector2.y,
		vector1.z * vector2.x - vector1.x * vector2.z,
		vector1.x * vector2.y - vector1.y * vector2.x );
}

};