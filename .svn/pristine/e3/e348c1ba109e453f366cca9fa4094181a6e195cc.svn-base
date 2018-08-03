//============================================================================
// Vector2.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

class Matrix2;
class Matrix3;

//----------------------------------------------------------------------------
// Vector2
//----------------------------------------------------------------------------

//! This class represents a 2D vector.
class Vector2
{
public:
	//! The origin vector.
	const static Vector2 cOrigin;
	//! The identity vector.
	const static Vector2 cIdentity;
	//! The x-axis vector.
	const static Vector2 cXAxis;
	//! The y-axis vector.
	const static Vector2 cYAxis;
	//! The maximal vector.
	const static Vector2 cMaxVector;
	//! The minimal vector.
	const static Vector2 cMinVector;

	//! X-component of the vector.
	_float	x;
	//! Y-component of the vector.
	_float	y;

public:
	//! Constructor, create a vector without sets x, y components.
	//! @param		none
	inline Vector2( );
	//! Constructor, create a vector and sets x, y components.
	//! @param		xx			X-component of the vector.
	//! @param		yy			Y-component of the vector.
	inline Vector2( _float xx, _float yy );

	//! Get positive vector.
	//! @param		none
	//! @return		The positive vector.
	inline Vector2 operator + ( ) const;
	//! Get negative vector.
	//! @param		none
	//! @return		The negative vector.
	inline Vector2 operator - ( ) const;

	//! Add a vector to current vector, and save result in current one.
	//! @param		vector		The vector to add.
	//! @return		The result vector of addition.
	inline Vector2& operator += ( const Vector2& vector );
	//! Subtract a vector from current vector, and save result in current one.
	//! @param		vector		The vector to subtract.
	//! @return		The result vector of subtraction.
	inline Vector2& operator -= ( const Vector2& vector );
	//! Multiply a scalar to current vector, and save result in current one.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result vector of multiplication.
	inline Vector2& operator *= ( _float scalar );
	//! Multiply a vector to current vector, and save result in current one.
	//! @param		vector		The vector to multiply.
	//! @return		The result vector of multiplication.
	inline Vector2& operator *= ( const Vector2& vector );
	//! Divide a scalar to current vector, and save result in current one.
	//! @param		scalar		The scalar to divide.
	//! @return		The result vector of division.
	inline Vector2& operator /= ( _float scalar );
	//! Divide a vector to current vector, and save result in current one.
	//! @param		vector		The vector to divide.
	//! @return		The result vector of division.
	inline Vector2& operator /= ( const Vector2& vector );

	//! Multiply a matrix to current vector, and save result in current one.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector2& operator *= ( const Matrix2& matrix );
	//! Multiply a matrix to current vector, and save result in current one.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector2& operator *= ( const Matrix3& matrix );

	//! Add a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to add.
	//! @return		The result vector of addition.
	inline Vector2 operator + ( const Vector2& vector ) const;
	//! Subtract a vector from current vector, return a new vector to save result.
	//! @param		vector		The vector to subtract.
	//! @return		The result vector of subtraction.
	inline Vector2 operator - ( const Vector2& vector ) const;
	//! Multiply a scalar to current vector, return a new vector to save result.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result vector of multiplication.
	inline Vector2 operator * ( _float scalar ) const;
	//! Multiply a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to multiply.
	//! @return		The result vector of multiplication.
	inline Vector2 operator * ( const Vector2& vector ) const;
	//! Divide a scalar to current vector, return a new vector to save result.
	//! @param		scalar		The scalar to divide.
	//! @return		The result vector of division.
	inline Vector2 operator / ( _float scalar ) const;
	//! Divide a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to divide.
	//! @return		The result vector of division.
	inline Vector2 operator / ( const Vector2& vector ) const;

	//! Compare two vectors.
	//! @param		vector		The vector to compare with.
	//! @return		True if they are identical, false otherwise.
	inline _bool operator == ( const Vector2& vector ) const;
	//! Compare two vectors.
	//! @param		vector		The vector to compare with.
	//! @return		True if they are not identical, false otherwise.
	inline _bool operator != ( const Vector2& vector ) const;

	//! Multiply a matrix to current vector, return a new vector to save result.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector2 operator * ( const Matrix2& matrix ) const;
	//! Multiply a matrix to current vector, return a new vector to save result.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector2 operator * ( const Matrix3& matrix ) const;

	//! Compare two vectors with error margin.
	//! @param		vector		The vector to compare with.
	//! @param		margin		The margin that allows some error.
	//! @return		True if they are identical, false otherwise.
	inline _bool Equal( const Vector2& vector, _float margin = Math::cEpsilon ) const;

	//! Make the current vector absolute, that all coordinates are nonnegative.
	//! @param		none
	//! @return		The absolute vector.
	inline Vector2& Absolute( );
	//! Make the current vector normalized, that has one unit magnitude.
	//! @param		none
	//! @return		The normalized vector.
	inline Vector2& Normalize( );

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
	inline static _float Dot( const Vector2& vector1, const Vector2& vector2 );
	//! Calculates cross product of two vectors, return the z-component.
	//! @param		vector1		Source vector 1.
	//! @param		vector2		Source vector 2.
	//! @return		The z-component of cross product result.
	inline static _float Cross( const Vector2& vector1, const Vector2& vector2 );
	//! Calculates vector by project one vector to another.
	//! @param		vector1		The vector to be projected.
	//! @param		vector2		The project reference vector.
	//! @return		The projected vector, that has same direction of vector2.
	static Vector2 Project( const Vector2& vector1, const Vector2& vector2 );
	//! Calculates vector by plumb one vector to another.
	//! @param		vector1		The vector to be projected.
	//! @param		vector2		The project reference vector.
	//! @return		The plumbed vector, that is vertical of vector2.
	static Vector2 Plumb( const Vector2& vector1, const Vector2& vector2 );
	//! Calculates linear interpolation of two vectors.
	//! @param		vector1		Source vector 1.
	//! @param		vector2		Source vector 2.
	//! @param		factor		The interpolation factor, will clamp to between 0 to 1.
	//! @return		The result vector of interpolation, vector1 + ( vector2 - vector1 ) * factor.
	static Vector2 Lerp( const Vector2& vector1, const Vector2& vector2, _float factor );
};

//----------------------------------------------------------------------------
// Vector2 Implementation
//----------------------------------------------------------------------------

Vector2::Vector2( )
{
}

Vector2::Vector2( _float xx, _float yy ) : x( xx ), y( yy )
{
}

Vector2 Vector2::operator + ( ) const
{
	return Vector2( x, y ); 
}

Vector2 Vector2::operator - ( ) const
{
	return Vector2( -x, -y );
}

Vector2& Vector2::operator += ( const Vector2& vector )
{
	x += vector.x;
	y += vector.y;

	return *this;
}

Vector2& Vector2::operator -= ( const Vector2& vector )
{
	x -= vector.x;
	y -= vector.y;

	return *this;
}

Vector2& Vector2::operator *= ( _float scalar )
{
	x *= scalar;
	y *= scalar;

	return *this;
}

Vector2& Vector2::operator *= ( const Vector2& vector )
{
	x *= vector.x;
	y *= vector.y;

	return *this;
}

Vector2& Vector2::operator /= ( _float scalar )
{
	x /= scalar;
	y /= scalar;

	return *this;
}

Vector2& Vector2::operator /= ( const Vector2& vector )
{
	x /= vector.x;
	y /= vector.y;

	return *this;
}

Vector2 Vector2::operator + ( const Vector2& vector ) const
{
	return Vector2( x + vector.x, y + vector.y );
}

Vector2 Vector2::operator - ( const Vector2& vector ) const
{
	return Vector2( x - vector.x, y - vector.y ); 
}

Vector2 Vector2::operator * ( _float scalar ) const
{
	return Vector2( x * scalar, y * scalar );
}

Vector2 Vector2::operator * ( const Vector2& vector ) const
{
	return Vector2( x * vector.x, y * vector.y );
}

Vector2 Vector2::operator / ( _float scalar ) const
{
	return Vector2( x / scalar, y / scalar );
}

Vector2 Vector2::operator / ( const Vector2& vector ) const
{
	return Vector2( x / vector.x, y / vector.y );
}

_bool Vector2::operator == ( const Vector2& vector ) const
{
	return x == vector.x && y == vector.y;
}

_bool Vector2::operator != ( const Vector2& vector ) const
{
	return x != vector.x || y != vector.y;
}

_bool Vector2::Equal( const Vector2& vector, _float margin ) const
{
	if ( Math::Abs( x - vector.x ) > margin )
		return _false;

	if ( Math::Abs( y - vector.y ) > margin )
		return _false;

	return _true;
}

Vector2& Vector2::Absolute( )
{
	if ( x < 0.0f )
		x = -x;

	if ( y < 0.0f )
		y = -y;

	return *this;
}

Vector2& Vector2::Normalize( )
{
	_float magnitude = Magnitude( );

	if ( magnitude > 0.0f )
	{
		x /= magnitude;
		y /= magnitude;
	}
	else
	{
		x = 1.0f;
		y = 0.0f;
	}

	return *this;
}

_float Vector2::Magnitude( ) const
{
	return Math::Sqrt( x * x + y * y );
}

_float Vector2::MagnitudeSquared( ) const
{
	return x * x + y * y;
}

_float Vector2::Dot( const Vector2& vector1, const Vector2& vector2 )
{
	return vector1.x * vector2.x + vector1.y * vector2.y;
}

_float Vector2::Cross( const Vector2& vector1, const Vector2& vector2 )
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}

};