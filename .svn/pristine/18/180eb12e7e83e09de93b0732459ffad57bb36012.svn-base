//============================================================================
// Matrix4SSE.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

class Vector4SSE;

//----------------------------------------------------------------------------
// Matrix4SSE
//----------------------------------------------------------------------------

//! This class represents a 4D Matrix with operation of SSE support.
class Matrix4SSE
{
	friend class Vector3SSE;
	friend class Vector4SSE;

private:
	//! The component of the matrix. Extra element for 16 bytes alignment
	_float	m[20];

	//! The start pointer
	_float4* start;

public:
	//! The zero matrix.
	const static Matrix4SSE cZero;
	//! The identity matrix.
	const static Matrix4SSE cIdentity;

	//! Constructor, create a matrix without sets components.
	//! @param		none
	inline Matrix4SSE( );
	//! Constructor, create a matrix and sets components.
	//! @param		m00~m33		The component of the matrix.
	Matrix4SSE( _float m00, _float m01, _float m02, _float m03, _float m10, _float m11, _float m12, _float m13,
		_float m20, _float m21, _float m22, _float m23, _float m30, _float m31, _float m32, _float m33 );
	//! Constructor, create a matrix from data.
	//! @param		buffer		The component data buffer of the 3 dimension vectors.
	//! @param		num			The number of vector3.
	Matrix4SSE( const _float* buffer, _dword num );
	//! Constructor, create a matrix from a 3X3 matrix.
	//! @param		m		The 3X3 matrix to create from.
	Matrix4SSE( const Matrix3 &m );
	//! Copy Constructor, create a matrix and sets components by a given matrix.
	//! @param		m		The matrix create from.
	inline Matrix4SSE( const Matrix4SSE &m );

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
	inline Matrix4SSE& operator = ( const Matrix3 &m );
	//! Copy components from another matrix.
	//! @param		m		The other matrix.
	//! @return		The reference of current matrix.
	inline Matrix4SSE& operator = ( const Matrix4 &m );
	//! Copy components from another matrix.
	//! @param		m		The other matrix.
	//! @return		The reference of current matrix.
	inline Matrix4SSE& operator = ( const Matrix4SSE &m );

	//! Add a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to add.
	//! @return		The result matrix of addition.
	Matrix4SSE& operator += ( const Matrix4SSE& matrix );
	//! Subtract a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to subtract.
	//! @return		The result matrix of subtraction.
	Matrix4SSE& operator -= ( const Matrix4SSE& matrix );
	//! Multiply a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result matrix of multiplication.
	Matrix4SSE& operator *= ( const Matrix4SSE& matrix );
	//! Multiply a scalar to current matrix, and save result in current one.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result matrix of multiplication.
	Matrix4SSE& operator *= ( _float scalar );
	//! Divide a scalar to current matrix, and save result in current one.
	//! @param		scalar		The scalar to divide.
	//! @return		The result quaternion of division.
	Matrix4SSE& operator /= ( _float scalar );

	//! Add a matrix to current matrix, return a new matrix to save result.
	//! @param		matrix		The matrix to add.
	//! @return		The result matrix of addition.
	Matrix4SSE operator + ( const Matrix4SSE& matrix ) const;
	//! Subtract a matrix to current matrix, return a new matrix to save result.
	//! @param		matrix		The matrix to subtract.
	//! @return		The result matrix of subtraction.
    Matrix4SSE operator - ( const Matrix4SSE& matrix ) const;
	//! Multiply a matrix to current matrix, return a new matrix to save result.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result matrix of multiplication.
    Matrix4SSE operator * ( const Matrix4SSE& matrix ) const;
	//! Multiply a scalar to current matrix, return a new matrix to save result.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result matrix of multiplication.
    Matrix4SSE operator * ( _float scalar ) const;
	//! Divide a scalar to current matrix, return a new matrix to save result.
	//! @param		scalar		The scalar to divide.
	//! @return		The result quaternion of division.
    Matrix4SSE operator / ( _float scalar ) const;
	//! Multiply current matrix to a vector, return a new vector to save result.
	//! @param		vector		The vector to multiply.
	//! @return		The result vector of multiplication.
    Vector4SSE operator * ( const Vector4SSE& vector ) const;

	//! Compare two matrix.
	//! @param		matrix		The matrix to compare with.
	//! @return		True if they are identical, false otherwise.
	_bool operator == ( const Matrix4SSE& matrix ) const;
	//! Compare two matrix.
	//! @param		matrix		The matrix to compare with.
	//! @return		True if they are not identical, false otherwise.
	_bool operator != ( const Matrix4SSE& matrix ) const;

	//! Compare two matrix with error margin.
	//! @param		matrix		The matrix to compare with.
	//! @param		margin		The margin that allows some error.
	//! @return		True if they are identical, false otherwise.
	_bool Equal( const Matrix4SSE& matrix, _float margin = Math::cEpsilon ) const;

	//! Calculates the determinant of the matrix.
	//! @param		none
	//! @return		The determinant of the matrix.
	_float Determinant( ) const;

	//! Calculates identity matrix, and save result in current one.
	//! @param		none
	//! @return		The identity matrix.
	Matrix4SSE& Identity( );
	//! Calculates transpose matrix, and save result in current one.
	//! @param		none
	//! @return		The transpose matrix.
	Matrix4SSE& Transpose( );
	//! Calculates inverse matrix, and save result in current one.
	//! @param		none
	//! @return		The inverse matrix.
	Matrix4SSE& Inverse( );
	//! Calculates adjoint matrix, and save result in current one.
	//! @param		none
	//! @return		The adjoint matrix.
	Matrix4SSE& Adjoint( );

	//! Create a translation matrix.
	//! @param		vector		The translation vector.
	//! @return		The translation matrix.
	Matrix4SSE& Translation( const Vector3& vector );

	//! Create a matrix from a quaternion
	//! @param		rotation	The quaternion of rotation.
	//! @return		The reference of current matrix.
	Matrix4SSE& FromRotation( const Quaternion& rotation );

	//! Get the rotation matrix, without translation.
	//! @param		none
	//! @return		The rotation matrix.
	Matrix3 GetRotationMatrix( ) const;

	//! Write the matrix element data to buffer.
	//! @param		buffer		The component data buffer of the 3 X 3 matrix.
	//! @param		num			The number of vector3 to write.
	//! @return		none.
	_void Write( _float* buffer, _dword num ) const;
};

Matrix4SSE::Matrix4SSE( )
{
	start = (_float4*) ( (_byte*) m + 16 - (_dword) m % 16 );
}

Matrix4SSE::Matrix4SSE( const Matrix4SSE &matrix )
{
	start = (_float4*) ( (_byte*) m + 16 - (_dword) m % 16 );

	_float4* start2 = matrix.start;

	*start = *start2;
	*( start + 1 ) = *( start2 + 1 );
	*( start + 2 ) = *( start2 + 2 );
	*( start + 3 ) = *( start2 + 3 );
}

Matrix4SSE& Matrix4SSE::operator= ( const Matrix3 &matrix )
{
	*start = _mm_set_ps( 0.0f, matrix( 0, 2 ), matrix( 0, 1 ), matrix( 0, 0 ) );
	*( start + 1 ) = _mm_set_ps( 0.0f, matrix( 1, 2 ), matrix( 1, 1 ), matrix( 1, 0 ) );
	*( start + 2 ) = _mm_set_ps( 0.0f, matrix( 2, 2 ), matrix( 2, 1 ), matrix( 2, 0 ) );
	*( start + 3 ) = _mm_setzero_ps();

	return *this;
}

Matrix4SSE& Matrix4SSE::operator= ( const Matrix4 &matrix )
{
	*start = _mm_set_ps( matrix( 0, 3 ), matrix( 0, 2 ), matrix( 0, 1 ), matrix( 0, 0 ) );
	*( start + 1 ) = _mm_set_ps( matrix( 1, 3 ), matrix( 1, 2 ), matrix( 1, 1 ), matrix( 1, 0 ) );
	*( start + 2 ) = _mm_set_ps( matrix( 2, 3 ), matrix( 2, 2 ), matrix( 2, 1 ), matrix( 2, 0 ) );
	*( start + 2 ) = _mm_set_ps( matrix( 3, 3 ), matrix( 3, 2 ), matrix( 3, 1 ), matrix( 3, 0 ) );

	return *this;
}

Matrix4SSE& Matrix4SSE::operator= ( const Matrix4SSE &matrix )
{
	*start = *matrix.start;
	*( start + 1 ) = *( matrix.start + 1 );
	*( start + 2 ) = *( matrix.start + 2 );
	*( start + 3 ) = *( matrix.start + 3 );

	return *this;
}

const _float& Matrix4SSE::operator ( ) ( _dword row, _dword column ) const
{
	FL_ASSERT( row < 4 && column < 4 )

	return *( (_float*)( start + row ) + column );
}

_float& Matrix4SSE::operator ( ) ( _dword row, _dword column )
{
	FL_ASSERT( row < 4 && column < 4 )

	return *( (_float*)( start + row ) + column );
}

};