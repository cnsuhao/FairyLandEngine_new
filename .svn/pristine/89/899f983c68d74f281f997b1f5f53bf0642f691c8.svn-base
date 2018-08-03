//============================================================================
// Matrix3.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Matrix3
//----------------------------------------------------------------------------

//! This class represents a 3D Matrix.
class Matrix3
{
	friend class Matrix4;
	friend class Matrix3x4;

private:
	//! The component of the matrix.
	_float	m[3][3];

public:
	//! The zero matrix.
	const static Matrix3 cZero;
	//! The identity matrix.
	const static Matrix3 cIdentity;

	//! Constructor, create a matrix without sets components.
	//! @param		none
	inline Matrix3( );
	//! Constructor, create a matrix and sets components.
	//! @param		m00~m22		The component of the matrix.
	inline Matrix3( _float m00, _float m01, _float m02, _float m10, _float m11, _float m12, _float m20, _float m21, _float m22 );

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
	inline Matrix3 operator + ( ) const;
	//! Get negative matrix.
	//! @param		none
	//! @return		The negative matrix.
	inline Matrix3 operator - ( ) const;

	//! Get a row from matrix.
	//! @param		row			Rom number of the matrix.
	//! @return		The row vector.
	inline Vector3 GetRow( _dword row ) const;
	//! Get a column from matrix.
	//! @param		column		Column number of the matrix.
	//! @return		The column vector.
	inline Vector3 GetColumn( _dword column ) const;

	//! Add a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to add.
	//! @return		The result matrix of addition.
	Matrix3& operator += ( const Matrix3& matrix );
	//! Subtract a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to subtract.
	//! @return		The result matrix of subtraction.
	Matrix3& operator -= ( const Matrix3& matrix );
	//! Multiply a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result matrix of multiplication.
	Matrix3& operator *= ( const Matrix3& matrix );
	//! Multiply a scalar to current matrix, and save result in current one.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result matrix of multiplication.
	Matrix3& operator *= ( _float scalar );
	//! Divide a scalar to current matrix, and save result in current one.
	//! @param		scalar		The scalar to divide.
	//! @return		The result quaternion of division.
	Matrix3& operator /= ( _float scalar );

	//! Add a matrix to current matrix, return a new matrix to save result.
	//! @param		matrix		The matrix to add.
	//! @return		The result matrix of addition.
	Matrix3 operator + ( const Matrix3& matrix ) const;
	//! Subtract a matrix to current matrix, return a new matrix to save result.
	//! @param		matrix		The matrix to subtract.
	//! @return		The result matrix of subtraction.
    Matrix3 operator - ( const Matrix3& matrix ) const;
	//! Multiply a matrix to current matrix, return a new matrix to save result.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result matrix of multiplication.
    Matrix3 operator * ( const Matrix3& matrix ) const;
	//! Multiply a scalar to current matrix, return a new matrix to save result.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result matrix of multiplication.
    Matrix3 operator * ( _float scalar ) const;
	//! Divide a scalar to current matrix, return a new matrix to save result.
	//! @param		scalar		The scalar to divide.
	//! @return		The result quaternion of division.
    Matrix3 operator / ( _float scalar ) const;

	//! Compare two matrix.
	//! @param		matrix		The matrix to compare with.
	//! @return		True if they are identical, false otherwise.
	_bool operator == ( const Matrix3& matrix ) const;
	//! Compare two matrix.
	//! @param		matrix		The matrix to compare with.
	//! @return		True if they are not identical, false otherwise.
	_bool operator != ( const Matrix3& matrix ) const;

	//! Calculates the determinant of the matrix.
	//! @param		none
	//! @return		The determinant of the matrix.
	_float Determinant( ) const;

	//! Calculates identity matrix, and save result in current one.
	//! @param		none
	//! @return		The identity matrix.
	Matrix3& Identity( );
	//! Calculates transpose matrix, and save result in current one.
	//! @param		none
	//! @return		The transpose matrix.
	Matrix3& Transpose( );
	//! Calculates inverse matrix, and save result in current one.
	//! @param		none
	//! @return		The inverse matrix.
	Matrix3& Inverse( );
	//! Calculates adjoint matrix, and save result in current one.
	//! @param		none
	//! @return		The adjoint matrix.
	Matrix3& Adjoint( );

	//! Create a translation matrix.
	//! @param		x			The x-component of translation.
	//! @param		y			The y-component of translation.
	//! @return		The translation matrix.
	Matrix3& Translation( _float x, _float y );
	//! Create a translation matrix.
	//! @param		vector		The translation vector.
	//! @return		The translation matrix.
	Matrix3& Translation( const Vector2& vector );

	//! Create a scaling matrix.
	//! @param		x			The x-component of scaling.
	//! @param		y			The y-component of scaling.
	//! @param		z			The z-component of scaling.
	//! @return		The scaling matrix.
	Matrix3& Scaling( _float x, _float y, _float z );
	//! Create a scaling matrix.
	//! @param		vector		The scaling vector.
	//! @return		The scaling matrix.
	Matrix3& Scaling( const Vector3& vector );
	//! Create a scaling matrix by scale in an arbitrary direction.
	//! @param		vector		The normal vector defines the direction of scale.
	//! @param		factor		The scale factor.
	//! @return		The scaling matrix.
	Matrix3& Scaling( const Vector3& vector, _float factor );
	
	//! Create a rotation matrix by rotate around origin, same as matrix2
	//! @param		radian		The angle of rotation in radian.
	//! @return		The rotation matrix.
	Matrix3& Rotation( _float radian );
	//! Create a rotation matrix by rotate around x-axis.
	//! @param		radian		The angle of rotation in radian.
	//! @return		The rotation matrix.
	Matrix3& RotationX( _float radian );
	//! Create a rotation matrix by rotate around y-axis.
	//! @param		radian		The angle of rotation in radian.
	//! @return		The rotation matrix.
	Matrix3& RotationY( _float radian );
	//! Create a rotation matrix by rotate around z-axis.
	//! @param		radian		The angle of rotation in radian.
	//! @return		The rotation matrix.
	Matrix3& RotationZ( _float radian );
	//! Create a rotation matrix by rotate around an arbitrary axis.
	//! @param		axis		The axis to rotate around.
	//! @param		radian		The angle of rotation in radian.
	//! @return		The rotation matrix.
	Matrix3& Rotation( const Vector3& axis, _float radian );

	//! Create a projection matrix by project onto xy-plane.
	//! @param		none
	//! @return		The projection matrix.
	Matrix3& ProjectXY( );
	//! Create a projection matrix by project onto yz-plane.
	//! @param		none
	//! @return		The projection matrix.
	Matrix3& ProjectYZ( );
	//! Create a projection matrix by project onto zx-plane.
	//! @param		none
	//! @return		The projection matrix.
	Matrix3& ProjectZX( );
	//! Create a projection matrix by project onto an arbitrary plane.
	//! @param		vector		The normal vector defines the project plane.
	//! @return		The projection matrix.
	Matrix3& Project( const Vector3& vector );

	//! Create a reflection matrix by flip around xy-plane.
	//! @param		none
	//! @return		The reflection matrix.
	Matrix3& RefectionXY( );
	//! Create a reflection matrix by flip around yz-plane.
	//! @param		none
	//! @return		The reflection matrix.
	Matrix3& RefectionYZ( );
	//! Create a reflection matrix by flip around zx-plane.
	//! @param		none
	//! @return		The reflection matrix.
	Matrix3& RefectionZX( );
	//! Create a reflection matrix by flip around an arbitrary plane.
	//! @param		vector		The normal vector defines the reflect plane.
	//! @return		The reflection matrix.
	Matrix3& Refection( const Vector3& vector );
	//! Create a matrix from a quaternion
	//! @param		rotation	The quaternion of rotation.
	//! @return		The reference of current matrix.
	Matrix3& FromRotation( const Quaternion& rotation );
	//! Get roation quaternion from current matrix.
	//! @param		rotation	The quaternion of rotation, get from this function.
	_void ToRotation( Quaternion& rotation ) const;
};

//----------------------------------------------------------------------------
// Matrix3 Implementation
//----------------------------------------------------------------------------

Matrix3::Matrix3( )
{
}

Matrix3::Matrix3( _float m00, _float m01, _float m02, _float m10, _float m11, _float m12, _float m20, _float m21, _float m22 )
{
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
}

_float Matrix3::operator ( ) ( _dword row, _dword column ) const
{
	FL_ASSERT( row < 3 && column < 3 )

	return m[ row ][ column ];
}

_float& Matrix3::operator ( ) ( _dword row, _dword column )
{
	FL_ASSERT( row < 3 && column < 3 )

	return m[ row ][ column ];
}

Matrix3 Matrix3::operator + ( ) const
{
	return Matrix3( m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2] );
}

Matrix3 Matrix3::operator - ( ) const
{
	return Matrix3( -m[0][0], -m[0][1], -m[0][2], -m[1][0], -m[1][1], -m[1][2], -m[2][0], -m[2][1], -m[2][2] );
}

Vector3 Matrix3::GetRow( _dword row ) const
{
	FL_ASSERT( row < 3 )

	return Vector3( m[ row ][0], m[ row ][1], m[ row ][2] );
}

Vector3 Matrix3::GetColumn( _dword column ) const
{
	FL_ASSERT( column < 3 )

	return Vector3( m[0][ column ], m[1][ column ], m[2][ column ] );
}

};