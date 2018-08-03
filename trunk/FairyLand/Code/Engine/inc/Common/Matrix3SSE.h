//============================================================================
// Matrix3SSE.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

class Vector3SSE;

//----------------------------------------------------------------------------
// Matrix3SSE
//----------------------------------------------------------------------------

//! This class represents a 3D Matrix with operation of SSE support.
class Matrix3SSE
{
private:
	//! The component of the matrix. Extra element for 16 bytes alignment
	_float	m[16];

	//! The start pointer
	_float4* start;

public:
	//! The zero matrix.
	const static Matrix3SSE cZero;
	//! The identity matrix.
	const static Matrix3SSE cIdentity;

	//! Constructor, create a matrix without sets components.
	//! @param		none
	inline Matrix3SSE( );
	//! Constructor, create a matrix and sets components.
	//! @param		m00~m22		The component of the matrix.
	Matrix3SSE( _float m00, _float m01, _float m02, 
		_float m10, _float m11, _float m12, _float m20, _float m21, _float m22 );
	//! Constructor, create a matrix and sets components by a given matrix.
	//! @param		m		The matrix create from.
	Matrix3SSE( const Matrix3 &m );

	//! Copy Constructor, create a matrix and sets components by a given matrix.
	//! @param		m		The matrix create from.
	inline Matrix3SSE( const Matrix3SSE &m );

	//! Get component from matrix by row number and column number.
	//! @param		row			Rom number of the matrix.
	//! @param		column		Column number of the matrix.
	//! @return		The component of the matrix.
	inline const _float& operator ( ) ( _dword row, _dword column ) const;
	//! Get component from matrix by row number and column number.
	//! @param		row			Rom number of the matrix.
	//! @param		column		Column number of the matrix.
	//! @return		The component reference of the matrix.
	inline _float& operator ( ) ( _dword row, _dword column );

	//! Copy components from another matrix.
	//! @param		m		The other matrix.
	//! @return		The reference of current matrix.
	inline Matrix3SSE& operator = ( const Matrix3 &m );
	//! Copy components from another matrix.
	//! @param		m		The other matrix.
	//! @return		The reference of current matrix.
	inline Matrix3SSE& operator = ( const Matrix3SSE &m );

	//! Add a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to add.
	//! @return		The result matrix of addition.
	Matrix3SSE& operator += ( const Matrix3SSE& matrix );
	//! Subtract a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to subtract.
	//! @return		The result matrix of subtraction.
	Matrix3SSE& operator -= ( const Matrix3SSE& matrix );
	//! Multiply a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result matrix of multiplication.
	Matrix3SSE& operator *= ( const Matrix3SSE& matrix );
	//! Multiply a scalar to current matrix, and save result in current one.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result matrix of multiplication.
	Matrix3SSE& operator *= ( _float scalar );
	//! Divide a scalar to current matrix, and save result in current one.
	//! @param		scalar		The scalar to divide.
	//! @return		The result quaternion of division.
	Matrix3SSE& operator /= ( _float scalar );

	//! Add a matrix to current matrix, return a new matrix to save result.
	//! @param		matrix		The matrix to add.
	//! @return		The result matrix of addition.
	Matrix3SSE operator + ( const Matrix3SSE& matrix ) const;
	//! Subtract a matrix to current matrix, return a new matrix to save result.
	//! @param		matrix		The matrix to subtract.
	//! @return		The result matrix of subtraction.
    Matrix3SSE operator - ( const Matrix3SSE& matrix ) const;
	//! Multiply a matrix to current matrix, return a new matrix to save result.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result matrix of multiplication.
    Matrix3SSE operator * ( const Matrix3SSE& matrix ) const;
	//! Multiply a scalar to current matrix, return a new matrix to save result.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result matrix of multiplication.
    Matrix3SSE operator * ( _float scalar ) const;
	//! Divide a scalar to current matrix, return a new matrix to save result.
	//! @param		scalar		The scalar to divide.
	//! @return		The result quaternion of division.
    Matrix3SSE operator / ( _float scalar ) const;
	//! Multiply current matrix to a vector, return a new vector to save result.
	//! @param		vector		The vector to multiply.
	//! @return		The result vector of multiplication.
    Vector3SSE operator * ( const Vector3SSE& vector ) const;

	//! Compare two matrix.
	//! @param		matrix		The matrix to compare with.
	//! @return		True if they are identical, false otherwise.
	_bool operator == ( const Matrix3SSE& matrix ) const;
	//! Compare two matrix.
	//! @param		matrix		The matrix to compare with.
	//! @return		True if they are not identical, false otherwise.
	_bool operator != ( const Matrix3SSE& matrix ) const;

	//! Type conversion, get the matrix3.
	//! @param		none
	//! @return		The 3D matrix.
	operator Matrix3 ( ) const;

	//! Compare two matrix with error margin.
	//! @param		matrix		The matrix to compare with.
	//! @param		margin		The margin that allows some error.
	//! @return		True if they are identical, false otherwise.
	_bool Equal( const Matrix3SSE& matrix, _float margin = Math::cEpsilon ) const;

	//! Create a matrix from a quaternion
	//! @param		rotation	The quaternion of rotation.
	//! @return		The reference of current matrix.
	Matrix3SSE& FromRotation( const Quaternion& rotation );

	friend class Vector3SSE;
};

Matrix3SSE::Matrix3SSE( )
{
	start = ( _float4* ) ( ( _byte* )m + 16 - ( _dword ) m % 16 );

	*start = _mm_setzero_ps();
	*( start + 1 ) = _mm_setzero_ps();
	*( start + 2 ) = _mm_setzero_ps();
}

Matrix3SSE::Matrix3SSE( const Matrix3SSE &matrix )
{
	start = ( _float4* ) ( ( _byte* )m + 16 - ( _dword ) m % 16 );

	_float4* start2 = matrix.start;

	*start = *start2;
	*( start + 1 ) = *( start2 + 1 );
	*( start + 2 ) = *( start2 + 2 );
}

Matrix3SSE& Matrix3SSE::operator= ( const Matrix3 &matrix )
{
	*start = _mm_set_ps( 0.0f, matrix( 0, 2 ), matrix( 0, 1 ), matrix( 0, 0 ) );
	*( start + 1 ) = _mm_set_ps( 0.0f, matrix( 1, 2 ), matrix( 1, 1 ), matrix( 1, 0 ) );
	*( start + 2 ) = _mm_set_ps( 0.0f, matrix( 2, 2 ), matrix( 2, 1 ), matrix( 2, 0 ) );

	return *this;
}

Matrix3SSE& Matrix3SSE::operator= ( const Matrix3SSE &matrix )
{
	*start = *matrix.start;
	*( start + 1 ) = *( matrix.start + 1 );
	*( start + 2 ) = *( matrix.start + 2 );

	return *this;
}

const _float& Matrix3SSE::operator ( ) ( _dword row, _dword column ) const
{
	FL_ASSERT( row < 3 && column < 3 )

	return *( ( _float* )( start + row ) + column );
}

_float& Matrix3SSE::operator ( ) ( _dword row, _dword column )
{
	FL_ASSERT( row < 3 && column < 3 )

	return *( ( _float* )( start + row ) + column );
}

};
