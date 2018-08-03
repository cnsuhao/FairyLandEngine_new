//============================================================================
// Matrix3x4.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Matrix3x4
//----------------------------------------------------------------------------

//! This class represents a 3x4 Matrix.
class Matrix3x4
{
private:
	//! The component of the matrix, column major.
	_float	m[3][4];

public:
	//! The zero matrix.
	const static Matrix3x4 cZero;
	//! The identity matrix.
	const static Matrix3x4 cIdentity;

	//! Constructor, create a matrix without sets components.
	//! @param		none
	inline Matrix3x4( );
	//! Constructor, create a matrix and sets components.
	//! @param		m00~m23		The component of the matrix.
	inline Matrix3x4( _float m00, _float m01, _float m02, _float m03, _float m10, _float m11, _float m12, _float m13, _float m20, _float m21, _float m22, _float m23 );
	//! Constructor, create a matrix and sets components.
	//! @param		translation	The vector of translation.
	//! @param		rotation	The matrix of rotation.
	inline Matrix3x4( const Vector3& translation, const Matrix3& rotation );
	//! Constructor, create a matrix and sets components.
	//! @param		ssevalue1 ~ ssevalue3	SSE values.
	inline Matrix3x4( const _float4& ssevalue1, const _float4& ssevalue2, const _float4& ssevalue3 );

	//! Get a row from matrix by row number.
	//! @param		row			Rom number of the matrix.
	//! @return		The row of the matrix.
	inline const _float* operator [] ( _dword row ) const;
	//! Get component from matrix by row number and column number.
	//! @param		row			Rom number of the matrix.
	//! @param		column		Column number of the matrix.
	//! @return		The component of the matrix.
	inline _float operator ( ) ( _dword row, _dword column ) const;
	//! Get component from matrix by row number and column number.
	//! @param		row			Rom number of the matrix.
	//! @param		column		Column number of the matrix.
	//! @return		The component reference of the matrix.
	inline _float& operator ( ) ( _dword row, _dword column );

	//! Get positive matrix.
	//! @param		none
	//! @return		The positive matrix.
	inline Matrix3x4 operator + ( ) const;
	//! Get negative matrix.
	//! @param		none
	//! @return		The negative matrix.
	inline Matrix3x4 operator - ( ) const;
	//! Get 3 x 4 matrix from a 4 x 4 matrix.
	//! @param		matrix
	//! @return		The 3 x 4 matrix.
	inline Matrix3x4& operator = ( const Matrix4& matrix );


	//! Add a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to add.
	//! @return		The result matrix of addition.
	Matrix3x4& operator += ( const Matrix3x4& matrix );
	//! Subtract a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to subtract.
	//! @return		The result matrix of subtraction.
	Matrix3x4& operator -= ( const Matrix3x4& matrix );
	//! Multiply a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result matrix of multiplication.
	//! Multiply a scalar to current matrix, and save result in current one.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result matrix of multiplication.
	Matrix3x4& operator *= ( _float scalar );
	//! Divide a scalar to current matrix, and save result in current one.
	//! @param		scalar		The scalar to divide.
	//! @return		The result quaternion of division.
	Matrix3x4& operator /= ( _float scalar );

	//! Add a matrix to current matrix, return a new matrix to save result.
	//! @param		matrix		The matrix to add.
	//! @return		The result matrix of addition.
	Matrix3x4 operator + ( const Matrix3x4& matrix ) const;
	//! Subtract a matrix to current matrix, return a new matrix to save result.
	//! @param		matrix		The matrix to subtract.
	//! @return		The result matrix of subtraction.
    Matrix3x4 operator - ( const Matrix3x4& matrix ) const;
	//! Multiply a scalar to current matrix, return a new matrix to save result.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result matrix of multiplication.
    Matrix3x4 operator * ( _float scalar ) const;
	//! Divide a scalar to current matrix, return a new matrix to save result.
	//! @param		scalar		The scalar to divide.
	//! @return		The result quaternion of division.
    Matrix3x4 operator / ( _float scalar ) const;

	//! Get the rotation matrix, without translation.
	//! @param		none
	//! @return		The rotation matrix.
	inline Matrix3 GetRotationMatrix( ) const;

	//! Get the translation vector, without rotation.
	//! @param		none
	//! @return		The translation vector.
	inline Vector3 GetTranslationVector( ) const;

	//! Load into sse value..
	//! @param		ssevalue1 ~ ssevalue3	SSE values.
	inline _void LoadIntoFloat4( _float4& ssevalue1, _float4& ssevalue2, _float4& ssevalue3 );

	//! Create a translation matrix.
	//! @param		x			The x-component of translation.
	//! @param		y			The y-component of translation.
	//! @param		z			The z-component of translation.
	//! @return		The translation matrix.
	Matrix3x4& Translation( _float x, _float y, _float z );
	//! Create a translation matrix.
	//! @param		vector		The translation vector.
	//! @return		The translation matrix.
	Matrix3x4& Translation( const Vector3& vector );

	//! Create a rotation matrix by rotate around x-axis.
	//! @param		radian		The angle of rotation in radian.
	//! @return		The rotation matrix.
	Matrix3x4& RotationX( _float radian );
	//! Create a rotation matrix by rotate around y-axis.
	//! @param		radian		The angle of rotation in radian.
	//! @return		The rotation matrix.
	Matrix3x4& RotationY( _float radian );
	//! Create a rotation matrix by rotate around z-axis.
	//! @param		radian		The angle of rotation in radian.
	//! @return		The rotation matrix.
	Matrix3x4& RotationZ( _float radian );

	//! Create a matrix from a matrix and a vector.
	//! @param		translation	The vector of translation.
	//! @param		rotation	The matrix of rotation.
	//! @return		The reference of current matrix.
	Matrix3x4& FromTranslationRotation( const Vector3& translation, const Matrix3& rotation );
	//! Get the translation vector and roation matrix from current matrix.
	//! @param		translation	The vector of translation, get from this function.
	//! @param		rotation	The matrix of rotation, get from this function.
	_void ToTranslationRotation( Vector3& translation, Matrix3& rotation ) const;
};

//----------------------------------------------------------------------------
// Matrix3x4 Implementation
//----------------------------------------------------------------------------

Matrix3x4::Matrix3x4( )
{
}


Matrix3x4::Matrix3x4( _float m00, _float m01, _float m02, _float m03, _float m10, _float m11, _float m12, _float m13, _float m20, _float m21, _float m22, _float m23 )
{
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
}

Matrix3x4::Matrix3x4( const Vector3& translation, const Matrix3& rotation )
{
	m[0][0] = rotation( 0, 0 ); m[0][1] = rotation( 1, 0 ); m[0][2] = rotation( 2, 0 ); m[0][3] = translation.x;
	m[1][0] = rotation( 0, 1 ); m[1][1] = rotation( 1, 1 ); m[1][2] = rotation( 2, 1 ); m[1][3] = translation.y;
	m[2][0] = rotation( 0, 2 ); m[2][1] = rotation( 1, 2 ); m[2][2] = rotation( 2, 2 ); m[2][3] = translation.z;
}

Matrix3x4::Matrix3x4( const _float4& ssevalue1, const _float4& ssevalue2, const _float4& ssevalue3 )
{
	Memory::MemCpy( m[0], &ssevalue1, sizeof( _float4 ) );
	Memory::MemCpy( m[1], &ssevalue2, sizeof( _float4 ) );
	Memory::MemCpy( m[2], &ssevalue3, sizeof( _float4 ) );
}

const _float* Matrix3x4::operator [] ( _dword row ) const
{
	FL_ASSERT( row < 3 );

	return m[ row ];
}

_float Matrix3x4::operator ( ) ( _dword row, _dword column ) const
{
	FL_ASSERT( row < 3 && column < 4 )

	return m[ row ][ column ];
}

_float& Matrix3x4::operator ( ) ( _dword row, _dword column )
{
	FL_ASSERT( row < 3 && column < 4 )

	return m[ row ][ column ];
}

Matrix3x4 Matrix3x4::operator + ( ) const
{
	return Matrix3x4( m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1], m[1][2], m[1][3], m[2][0], m[2][1], m[2][2], m[2][3] );
}

Matrix3x4 Matrix3x4::operator - ( ) const
{
	return Matrix3x4( -m[0][0], -m[0][1], -m[0][2], -m[0][3], -m[1][0], -m[1][1], -m[1][2], -m[1][3], -m[2][0], -m[2][1], -m[2][2], -m[2][3] );
}

Matrix3x4& Matrix3x4::operator = ( const Matrix4& matrix )
{
	m[0][0] = matrix( 0, 0 ); m[0][1] = matrix( 1, 0 ); m[0][2] = matrix( 2, 0 ); m[0][3] = matrix( 3, 0 );
	m[1][0] = matrix( 0, 1 ); m[1][1] = matrix( 1, 1 ); m[1][2] = matrix( 2, 1 ); m[1][3] = matrix( 3, 1 );
	m[2][0] = matrix( 0, 2 ); m[2][1] = matrix( 1, 2 ); m[2][2] = matrix( 2, 2 ); m[2][3] = matrix( 3, 2 );

	return *this;
}

Matrix3 Matrix3x4::GetRotationMatrix( ) const
{
	return Matrix3(
		m[0][0], m[1][0], m[2][0],
		m[0][1], m[1][1], m[2][1],
		m[0][2], m[1][2], m[2][2] );
}

Vector3 Matrix3x4::GetTranslationVector( ) const
{
	return Vector3( m[0][3], m[1][3], m[2][3] );
}

_void Matrix3x4::LoadIntoFloat4( _float4& ssevalue1, _float4& ssevalue2, _float4& ssevalue3 )
{
	ssevalue1 = _mm_load_ps( m[0] );
	ssevalue2 = _mm_load_ps( m[1] );
	ssevalue3 = _mm_load_ps( m[2] );
}

};