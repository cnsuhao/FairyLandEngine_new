//============================================================================
// Vector3SSE.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

class Quaternion;
class Matrix3SSE;
class Matrix4SSE;

//----------------------------------------------------------------------------
// Vector3SSE
//----------------------------------------------------------------------------

//! This class represents a 3D vector.
class Vector3SSE
{
public:
	//! The origin vector.
	const static Vector3SSE cOrigin;
	//! The identity vector.
	const static Vector3SSE cIdentity;
	//! The x-axis vector.
	const static Vector3SSE cXAxis;
	//! The y-axis vector.
	const static Vector3SSE cYAxis;
	//! The z-axis vector.
	const static Vector3SSE cZAxis;
	//! The maximal vector.
	const static Vector3SSE cMaxVector;
	//! The minimal vector.
	const static Vector3SSE cMinVector;


private:
	//! The component of the vector. Extra element for 16 bytes alignment
	_float	m[8];

	//! The start pointer
	_float4* start;

public:
	//! Constructor, create a vector without sets x, y, z components.
	//! @param		none
	inline Vector3SSE( );
	//! Constructor, create a vector and sets x, y, z components.
	//! @param		xx			X-component of the vector.
	//! @param		yy			Y-component of the vector.
	//! @param		zz			Z-component of the vector.
	inline Vector3SSE( _float xx, _float yy, _float zz );
	//! Copy components from another vector.
	//! @param		m		The other vector.
	//! @return		The reference of current vector.
	inline Vector3SSE( const Vector3 &vector );
	//! Copy Constructor,create a vector from other vector.
	//! @param		vector		The vector create from.
	inline Vector3SSE( const Vector3SSE& vector );

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
	inline Vector3SSE& operator = ( const Vector3 &vector );
	//! Copy components from another vector.
	//! @param		m		The other vector.
	//! @return		The reference of current vector.
	inline Vector3SSE& operator = ( const Vector3SSE &vector );

	//! Add a vector to current vector, and save result in current one.
	//! @param		vector		The vector to add.
	//! @return		The result vector of addition.
	inline Vector3SSE& operator += ( const Vector3SSE& vector );
	//! Subtract a vector from current vector, and save result in current one.
	//! @param		vector		The vector to subtract.
	//! @return		The result vector of subtraction.
	inline Vector3SSE& operator -= ( const Vector3SSE& vector );
	//! Multiply a scalar to current vector, and save result in current one.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result vector of multiplication.
	inline Vector3SSE& operator *= ( _float scalar );
	//! Multiply a vector to current vector, and save result in current one.
	//! @param		vector		The vector to multiply.
	//! @return		The result vector of multiplication.
	inline Vector3SSE& operator *= ( const Vector3SSE& vector );
	//! Divide a scalar to current vector, and save result in current one.
	//! @param		scalar		The scalar to divide.
	//! @return		The result vector of division.
	inline Vector3SSE& operator /= ( _float scalar );
	//! Divide a vector to current vector, and save result in current one.
	//! @param		vector		The vector to divide.
	//! @return		The result vector of division.
	inline Vector3SSE& operator /= ( const Vector3SSE& vector );

	//! Multiply a quaternion to current vector, and save result in current one.
	//! @param		quaternioon		The quaternion to multiply.
	//! @return		The result vector of multiplication.
	Vector3SSE& operator *= ( const Quaternion& quaternion );
	//! Multiply a matrix to current vector, and save result in current one.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector3SSE& operator *= ( const Matrix3SSE& matrix );
	//! Multiply a matrix to current vector, and save result in current one.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector3SSE& operator *= ( const Matrix4SSE& matrix );

	//! Add a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to add.
	//! @return		The result vector of addition.
	inline Vector3SSE operator + ( const Vector3SSE& vector ) const;
	//! Subtract a vector from current vector, return a new vector to save result.
	//! @param		vector		The vector to subtract.
	//! @return		The result vector of subtraction.
	inline Vector3SSE operator - ( const Vector3SSE& vector ) const;
	//! Multiply a scalar to current vector, return a new vector to save result.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result vector of multiplication.
	inline Vector3SSE operator * ( _float scalar ) const;
	//! Multiply a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to multiply.
	//! @return		The result vector of multiplication.
	inline Vector3SSE operator * ( const Vector3SSE& vector ) const;
	//! Divide a scalar to current vector, return a new vector to save result.
	//! @param		scalar		The scalar to divide.
	//! @return		The result vector of division.
	inline Vector3SSE operator / ( _float scalar ) const;
	//! Divide a vector to current vector, return a new vector to save result.
	//! @param		vector		The vector to divide.
	//! @return		The result vector of division.
	inline Vector3SSE operator / ( const Vector3SSE& vector ) const;

	//! Compare two vectors.
	//! @param		vector		The vector to compare with.
	//! @return		True if they are identical, false otherwise.
	inline _bool operator == ( const Vector3SSE& vector ) const;
	//! Compare two vectors.
	//! @param		vector		The vector to compare with.
	//! @return		True if they are not identical, false otherwise.
	inline _bool operator != ( const Vector3SSE& vector ) const;

	//! Multiply a matrix to current vector, return a new vector to save result.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result vector of multiplication.
	Vector3SSE operator * ( const Matrix3SSE& matrix ) const;

	//! Compare two vectors with error margin.
	//! @param		vector		The vector to compare with.
	//! @param		margin		The margin that allows some error.
	//! @return		True if they are identical, false otherwise.
	inline _bool Equal( const Vector3SSE& vector, _float margin = Math::cEpsilon ) const;

	//! Write the matrix element data to buffer.
	//! @param		buffer		The component data buffer of the 3 X 3 matrix.
	//! @return		none.
	inline _void Write( _float* buffer ) const;

	friend class Matrix3SSE;
};

//----------------------------------------------------------------------------
// Vector3SSE Implementation
//----------------------------------------------------------------------------

Vector3SSE::Vector3SSE( )
{
	start = ( _float4* ) ( ( _byte* )m + 16 - ( _dword ) m % 16 );

	*start = _mm_setzero_ps();
}

Vector3SSE::Vector3SSE( _float xx, _float yy, _float zz )
{
	start = ( _float4* ) ( ( _byte* )m + 16 - ( _dword ) m % 16 );

	*start = _mm_set_ps( 0.0f, zz, yy, xx );
}

Vector3SSE::Vector3SSE( const Vector3 &vector )
{
	start = ( _float4* ) ( ( _byte* )m + 16 - ( _dword ) m % 16 );

	*start = _mm_set_ps( 0.0f, vector.z, vector.y, vector.x );
}

Vector3SSE::Vector3SSE( const Vector3SSE& vector )
{
	start = ( _float4* ) ( ( _byte* )m + 16 - ( _dword ) m % 16 );

	*start = *vector.start;
}

const _float& Vector3SSE::operator ( ) ( _dword index ) const
{
	//FL_ASSERT( index < 3 )

	return *( ( _float* )start + index );
}

_float& Vector3SSE::operator ( ) ( _dword index )
{
	//FL_ASSERT( index < 3 )

	return *( ( _float* )start + index );
}

Vector3SSE::operator Vector3 ( ) const
{
	_float* pointer = ( _float* ) start;

	return Vector3( *pointer, *( pointer + 1 ), *( pointer + 2 ) );
}

Vector3SSE& Vector3SSE::operator = ( const Vector3 &vector )
{
	*start = _mm_set_ps( 0.0f, vector.z, vector.y, vector.x );

	return *this;
}

Vector3SSE& Vector3SSE::operator = ( const Vector3SSE &vector )
{
	*start = *vector.start;

	return *this;
}

Vector3SSE& Vector3SSE::operator += ( const Vector3SSE& vector )
{
	*start = _mm_add_ps( *start, *vector.start );

	return *this;
}

Vector3SSE& Vector3SSE::operator -= ( const Vector3SSE& vector )
{
	*start = _mm_sub_ps( *start, *vector.start );

	return *this;
}

Vector3SSE& Vector3SSE::operator *= ( _float scalar )
{
	*start = _mm_mul_ps( *start, _mm_set_ps1( scalar ) );

	return *this;
}

Vector3SSE& Vector3SSE::operator *= ( const Vector3SSE& vector )
{
	*start = _mm_mul_ps( *start, *vector.start );

	return *this;
}

Vector3SSE& Vector3SSE::operator /= ( _float scalar )
{
	*start = _mm_div_ps( *start, _mm_set_ps1( scalar ) );

	return *this;
}

Vector3SSE& Vector3SSE::operator /= ( const Vector3SSE& vector )
{
	*start = _mm_div_ps( *start, *vector.start );

	return *this;
}

Vector3SSE Vector3SSE::operator + ( const Vector3SSE& vector ) const
{
	Vector3SSE result;

	_mm_store_ps( ( _float*) result.start, _mm_add_ps( *start, *vector.start ) );

	return result;
}

Vector3SSE Vector3SSE::operator - ( const Vector3SSE& vector ) const
{
	Vector3SSE result;

	*result.start = _mm_sub_ps( *start, *vector.start );

	return result;
}

Vector3SSE Vector3SSE::operator * ( _float scalar ) const
{
	Vector3SSE result;

	*result.start = _mm_mul_ps( *start, _mm_set_ps1( scalar ) );

	return result;
}

Vector3SSE Vector3SSE::operator * ( const Vector3SSE& vector ) const
{
	Vector3SSE result;

	*result.start = _mm_mul_ps( *start, *vector.start );

	return result;
}

Vector3SSE Vector3SSE::operator / ( _float scalar ) const
{
	Vector3SSE result;

	*result.start = _mm_div_ps( *start, _mm_set_ps1( scalar ) );

	return result;
}

Vector3SSE Vector3SSE::operator / ( const Vector3SSE& vector ) const
{
	Vector3SSE result;

	*result.start = _mm_div_ps( *start, *vector.start );

	return result;
}

_bool Vector3SSE::operator == ( const Vector3SSE& vector ) const
{
	_float* start1 = ( _float* ) start;
	_float* start2 = ( _float* ) vector.start;

	return *start1++ == *start2++ && *start1++ == *start2++ &&  *start1 == *start2;
}

_bool Vector3SSE::operator != ( const Vector3SSE& vector ) const
{
	return !( operator == ( vector ) );
}

_bool Vector3SSE::Equal( const Vector3SSE& vector, _float margin ) const
{
	_float4 sub = _mm_sub_ps( *start, *vector.start );
	_float* temp = ( _float* ) &sub;

	_float t = Math::Abs( *temp );

	if ( Math::Abs( *temp++ ) > margin )
		return _false;

	if ( Math::Abs( *temp++ ) > margin )
		return _false;

	if ( Math::Abs( *temp ) > margin )
		return _false;

	return _true;
}

_void Vector3SSE::Write( _float* buffer ) const
{
	_float* temp = ( _float* )start;
	*buffer = *temp;
	*( buffer + 1 ) = *( temp + 1 );
	*( buffer + 2 ) = *( temp + 2 );
}

};
