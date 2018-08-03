//============================================================================
// Vector4SSE.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

class Matrix4SSE;
class Quaternion;

//----------------------------------------------------------------------------
// Vector4SSE
//----------------------------------------------------------------------------

//! This class represents a 4D vector.
class Vector4SSE
{
	friend class Matrix4SSE;

public:
	//! The origin vector.
	const static Vector4SSE cOrigin;
	//! The identity vector.
	const static Vector4SSE cIdentity;
	//! The x-axis vector.
	const static Vector4SSE cXAxis;
	//! The y-axis vector.
	const static Vector4SSE cYAxis;
	//! The z-axis vector.
	const static Vector4SSE cZAxis;
	//! The w-axis vector.
	const static Vector4SSE cWAxis;
	//! The maximal vector.
	const static Vector4SSE cMaxVector;
	//! The minimal vector.
	const static Vector4SSE cMinVector;


private:
	//! The component of the vector. Extra element for 16 bytes alignment
	_float	m[8];

	//! The start pointer
	_float4* start;

public:
	//! Constructor, create a vector without sets x, y, z, w components.
	//! @param		none
	inline Vector4SSE( );
	//! Constructor, create a vector and sets x, y, z components.
	//! @param		xx			X-component of the vector.
	//! @param		yy			Y-component of the vector.
	//! @param		zz			Z-component of the vector.
	inline Vector4SSE( _float xx, _float yy, _float zz, _float ww );
	//! Constructor, create a 4D vector from a 3D vector and a w-component.
	//! @param		vector		The 3D vector, used to set x, y, z components.
	//! @param		ww			W-component of the vector.
	inline Vector4SSE( const Vector3& vector, _float ww = 0.0f );
	//! Copy Constructor,create a vector from other vector.
	//! @param		vector		The vector create from.
	inline Vector4SSE( const Vector4SSE& vector );

	//! Get component from vector by index number.
	//! @param		index		Index number of the vector.
	//! @return		The component reference of the vector.
	inline const _float& operator ( ) ( _dword index ) const;
	//! Get component from vector by index number.
	//! @param		index		Index number of the vector.
	//! @return		The component reference of the vector.
	inline _float& operator ( ) ( _dword index );

	//! Type conversion, get the 3D vector from x, y, z components.
	//! @param		none
	//! @return		The 3D vector.
	inline operator Vector3 ( ) const;

	//! Copy components from another vector.
	//! @param		m		The other vector.
	//! @return		The reference of current vector.
	inline Vector4SSE& operator = ( const Vector3 &vector );
	//! Copy components from another vector.
	//! @param		m		The other vector.
	//! @return		The reference of current vector.
	inline Vector4SSE& operator = ( const Vector4SSE &vector );

	//! Add a vector to current vector, and save result in current one.
	//! @param		vector		The vector to add.
	//! @return		The result vector of addition.
	inline Vector4SSE& operator += ( const Vector4SSE& vector );
	//! Subtract a vector from current vector, and save result in current one.
	//! @param		vector		The vector to subtract.
	//! @return		The result vector of subtraction.
	inline Vector4SSE& operator -= ( const Vector4SSE& vector );
	//! Multiply a scalar to current vector, and save result in current one.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result vector of multiplication.
	inline Vector4SSE& operator *= ( _float scalar );
	//! Multiply a vector to current vector, and save result in current one.
	//! @param		vector		The vector to multiply.
	//! @return		The result vector of multiplication.
	inline Vector4SSE& operator *= ( const Vector4SSE& vector );
	//! Divide a scalar to current vector, and save result in current one.
	//! @param		scalar		The scalar to divide.
	//! @return		The result vector of division.
	inline Vector4SSE& operator /= ( _float scalar );
	//! Divide a vector to current vector, and save result in current one.
	//! @param		vector		The vector to divide.
	//! @return		The result vector of division.
	inline Vector4SSE& operator /= ( const Vector4SSE& vector );

	//! Multiply a quaternion to current vector, and save result in current one.
	//! @param		quaternioon		The quaternion to multiply.
	//! @return		The result vector of multiplication.
	Vector4SSE& operator *= ( const Quaternion& quaternion );
	//! Multiply a matrix to current vector, and save result in current one.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector4SSE& operator *= ( const Matrix4SSE& matrix );

	//! Add a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to add.
	//! @return		The result vector of addition.
	inline Vector4SSE operator + ( const Vector4SSE& vector ) const;
	//! Subtract a vector from current vector, return a new vector to save result.
	//! @param		vector		The vector to subtract.
	//! @return		The result vector of subtraction.
	inline Vector4SSE operator - ( const Vector4SSE& vector ) const;
	//! Multiply a scalar to current vector, return a new vector to save result.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result vector of multiplication.
	inline Vector4SSE operator * ( _float scalar ) const;
	//! Multiply a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to multiply.
	//! @return		The result vector of multiplication.
	inline Vector4SSE operator * ( const Vector4SSE& vector ) const;
	//! Divide a scalar to current vector, return a new vector to save result.
	//! @param		scalar		The scalar to divide.
	//! @return		The result vector of division.
	inline Vector4SSE operator / ( _float scalar ) const;
	//! Divide a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to divide.
	//! @return		The result vector of division.
	inline Vector4SSE operator / ( const Vector4SSE& vector ) const;

	//! Compare two vectors.
	//! @param		vector		The vector to compare with.
	//! @return		True if they are identical, false otherwise.
	inline _bool operator == ( const Vector4SSE& vector ) const;
	//! Compare two vectors.
	//! @param		vector		The vector to compare with.
	//! @return		True if they are not identical, false otherwise.
	inline _bool operator != ( const Vector4SSE& vector ) const;

	//! Multiply a matrix to current vector, return a new vector to save result.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector4SSE operator * ( const Matrix4SSE& matrix ) const;

	//! Compare two vectors with error margin.
	//! @param		vector		The vector to compare with.
	//! @param		margin		The margin that allows some error.
	//! @return		True if they are identical, false otherwise.
	inline _bool Equal( const Vector4SSE& vector, _float margin = Math::cEpsilon ) const;

	//! Make the current vector absolute, that all coordinates are nonnegative.
	//! @param		none
	//! @return		The absolute vector.
	inline Vector4SSE& Absolute( );
	//! Make the current vector normalized, that has one unit magnitude.
	//! @param		none
	//! @return		The normalized vector.
	inline Vector4SSE& Normalize( );

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
	inline static _float Dot( const Vector4SSE& vector1, const Vector4SSE& vector2 );
	//! Calculates cross product of two vectors.
	//! @param		vector1		Source vector 1.
	//! @param		vector2		Source vector 2.
	//! @return		The cross product result.
	inline static Vector4SSE Cross( const Vector4SSE& vector1, const Vector4SSE& vector2 );
	//! Calculates vector by project one vector to another.
	//! @param		vector1		The vector to be projected.
	//! @param		vector2		The project reference vector.
	//! @return		The projected vector, that has same direction of vector2.
	static Vector4SSE Project( const Vector4SSE& vector1, const Vector4SSE& vector2 );
	//! Calculates vector by plumb one vector to another.
	//! @param		vector1		The vector to be projected.
	//! @param		vector2		The project reference vector.
	//! @return		The plumbed vector, that is vertical of vector2.
	static Vector4SSE Plumb( const Vector4SSE& vector1, const Vector4SSE& vector2 );
	//! Calculates linear interpolation of two vectors.
	//! @param		vector1		Source vector 1.
	//! @param		vector2		Source vector 2.
	//! @param		factor		The interpolation factor, will clamp to between 0 to 1.
	//! @return		The result vector of interpolation, vector1 + ( vector2 - vector1 ) * factor.
	static Vector4SSE Lerp( const Vector4SSE& vector1, const Vector4SSE& vector2, _float factor );
};

//----------------------------------------------------------------------------
// Vector4SSE Implementation
//----------------------------------------------------------------------------

Vector4SSE::Vector4SSE( )
{
	start = (_float4*) ( (_byte*) m + 16 - (_dword) m % 16 );
}

Vector4SSE::Vector4SSE( _float xx, _float yy, _float zz, _float ww )
{
	start = (_float4*) ( (_byte*) m + 16 - (_dword) m % 16 );

	*start = _mm_set_ps( ww, zz, yy, xx );
}

Vector4SSE::Vector4SSE( const Vector3& vector, _float ww )
{
	start = (_float4*) ( (_byte*) m + 16 - (_dword) m % 16 );

	*start = _mm_set_ps( ww, vector.z, vector.y, vector.x );
}

Vector4SSE::Vector4SSE( const Vector4SSE& vector )
{
	start = (_float4*) ( (_byte*) m + 16 - (_dword) m % 16 );

	*start = *vector.start;
}

const _float& Vector4SSE::operator ( ) ( _dword index ) const
{
	FL_ASSERT( index < 4 );

	return *( (_float*)start + index );
}

_float& Vector4SSE::operator ( ) ( _dword index )
{
	FL_ASSERT( index < 4 );

	return *( (_float*)start + index );
}

Vector4SSE::operator Vector3 ( ) const
{
	_float* pointer = (_float*) start;

	return Vector3( *pointer, *( pointer + 1 ), *( pointer + 2 ) );
}

Vector4SSE& Vector4SSE::operator = ( const Vector3 &vector )
{
	_float* temp = (_float*)start;
	*temp = vector.x;
	*( temp + 1 ) = vector.y;
	*( temp + 2 ) = vector.z;
	*( temp + 3 ) = 0.0f;

	return *this;
}

Vector4SSE& Vector4SSE::operator = ( const Vector4SSE &vector )
{
	*start = *vector.start;

	return *this;
}

Vector4SSE& Vector4SSE::operator += ( const Vector4SSE& vector )
{
	*start = _mm_add_ps( *start, *vector.start );

	return *this;
}

Vector4SSE& Vector4SSE::operator -= ( const Vector4SSE& vector )
{
	*start = _mm_sub_ps( *start, *vector.start );

	return *this;
}

Vector4SSE& Vector4SSE::operator *= ( _float scalar )
{
	*start = _mm_mul_ps( *start, _mm_set_ps1( scalar ) );

	return *this;
}

Vector4SSE& Vector4SSE::operator *= ( const Vector4SSE& vector )
{
	*start = _mm_mul_ps( *start, *vector.start );

	return *this;
}

Vector4SSE& Vector4SSE::operator /= ( _float scalar )
{
	*start = _mm_div_ps( *start, _mm_set_ps1( scalar ) );

	return *this;
}

Vector4SSE& Vector4SSE::operator /= ( const Vector4SSE& vector )
{
	*start = _mm_div_ps( *start, *vector.start );

	return *this;
}

Vector4SSE Vector4SSE::operator + ( const Vector4SSE& vector ) const
{
	Vector4SSE result;

	*result.start = _mm_add_ps( *start, *vector.start );

	return result;
}

Vector4SSE Vector4SSE::operator - ( const Vector4SSE& vector ) const
{
	Vector4SSE result;

	*result.start = _mm_sub_ps( *start, *vector.start );

	return result;
}

Vector4SSE Vector4SSE::operator * ( _float scalar ) const
{
	Vector4SSE result;

	*result.start = _mm_mul_ps( *start, _mm_set_ps1( scalar ) );

	return result;
}

Vector4SSE Vector4SSE::operator * ( const Vector4SSE& vector ) const
{
	Vector4SSE result;

	*result.start = _mm_mul_ps( *start, *vector.start );

	return result;
}

Vector4SSE Vector4SSE::operator / ( _float scalar ) const
{
	Vector4SSE result;

	*result.start = _mm_div_ps( *start, _mm_set_ps1( scalar ) );

	return result;
}

Vector4SSE Vector4SSE::operator / ( const Vector4SSE& vector ) const
{
	Vector4SSE result;

	*result.start = _mm_div_ps( *start, *vector.start );

	return result;
}

_bool Vector4SSE::operator == ( const Vector4SSE& vector ) const
{
	_float* start1 = (_float*) start;
	_float* start2 = (_float*) vector.start;

	return *start1++ == *start2++ && *start1++ == *start2++ && *start1++ == *start2++ &&  *start1 == *start2;
}

_bool Vector4SSE::operator != ( const Vector4SSE& vector ) const
{
	return !( operator == ( vector ) );
}

_bool Vector4SSE::Equal( const Vector4SSE& vector, _float margin ) const
{
	_float4 sub = _mm_sub_ps( *start, *vector.start );
	_float* temp = (_float*) &sub;

	_float t = Math::Abs( *temp );

	if ( Math::Abs( *temp++ ) > margin )
		return _false;

	if ( Math::Abs( *temp++ ) > margin )
		return _false;

	if ( Math::Abs( *temp++ ) > margin )
		return _false;

	if ( Math::Abs( *temp ) > margin )
		return _false;

	return _true;
}

Vector4SSE& Vector4SSE::Absolute( )
{
	_float* temp = (_float*)start;

	if ( *temp < 0.0f )
		*temp = -*temp;
	temp++;

	if ( *temp < 0.0f )
		*temp = -*temp;
	temp++;

	if ( *temp < 0.0f )
		*temp = -*temp;
	temp++;

	if ( *temp < 0.0f )
		*temp = -*temp;
	temp++;

	return *this;
}

Vector4SSE& Vector4SSE::Normalize( )
{
	_float magnitude = Magnitude( );

	if ( magnitude > 0.0f )
	{
		*start = _mm_div_ps( *start, _mm_set_ps1( magnitude ) );
	}
	else
	{
		*start = _mm_setzero_ps();
		*(_float*)start = 1.0f;
	}

	return *this;
}

_float Vector4SSE::Magnitude( ) const
{
	return Math::Sqrt( Dot( *this, *this ) );
}

_float Vector4SSE::MagnitudeSquared( ) const
{
	return Dot( *this, *this );
}

_float Vector4SSE::Dot( const Vector4SSE& vector1, const Vector4SSE& vector2 )
{
	_float4 r = _mm_mul_ps( *vector1.start, *vector2.start );

	r = _mm_add_ps( _mm_movehl_ps( r, r ), r );

	return *(float *)&( _mm_add_ss( _mm_shuffle_ps( r, r, 1 ), r ) );
}

Vector4SSE Vector4SSE::Cross( const Vector4SSE& vector1, const Vector4SSE& vector2 )
{
	Vector4SSE result;

	_float4 l1, l2, m1, m2;
	l1 = _mm_shuffle_ps( *vector1.start, *vector1.start, _MM_SHUFFLE( 3, 1, 0, 2 ) );
	l2 = _mm_shuffle_ps( *vector2.start, *vector2.start, _MM_SHUFFLE( 3, 0, 2, 1 ) );
	m2 = _mm_mul_ps( l1, l2 );
	*( (_float*) &m2 + 3 ) = 0.0f;

	l1 = _mm_shuffle_ps( *vector1.start, *vector1.start, _MM_SHUFFLE( 3, 0, 2, 1 ) );
	l2 = _mm_shuffle_ps( *vector2.start, *vector2.start, _MM_SHUFFLE( 3, 1, 0, 2 ) );
	m1 = _mm_mul_ps( l1, l2 );

	*result.start = _mm_sub_ps( m1, m2 );

	return result;
}

};