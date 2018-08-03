//============================================================================
// Matrix4.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Matrix4
//----------------------------------------------------------------------------

#pragma pack( push, 16 )

//! This class represents a 4D Matrix.
class Matrix4
{
	friend class Vector4;

private:
	//! The component of the matrix.
	_float	m[4][4];

public:
	//! The zero matrix.
	const static Matrix4 cZero;
	//! The identity matrix.
	const static Matrix4 cIdentity;
	//! The clip space to image space matrix.
	const static Matrix4 cClip2Image;

	//! Constructor, create a matrix without sets components.
	//! @param		none
	inline Matrix4( );
	//! Constructor, create a matrix and sets components.
	//! @param		m00~m33		The component of the matrix.
	inline Matrix4( _float m00, _float m01, _float m02, _float m03, _float m10, _float m11, _float m12, _float m13,
		_float m20, _float m21, _float m22, _float m23, _float m30, _float m31, _float m32, _float m33 );
	//! Constructor, create a matrix and sets components.
	//! @param		m		The matrix create from.
	inline Matrix4( const Matrix3 &m );
	//! Copy Constructor, create a matrix and sets components by a given matrix.
	//! @param		m		The matrix create from.
	inline Matrix4( const Matrix4 &m );

	//! Copy components from another matrix.
	//! @param		m		The other matrix.
	//! @return		The reference of current matrix.
	inline Matrix4& operator = ( const Matrix3 &m );
	//! Copy components from another matrix.
	//! @param		m		The other matrix.
	//! @return		The reference of current matrix.
	inline Matrix4& operator = ( const Matrix4 &m );
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

	//! Get positive matrix.
	//! @param		none
	//! @return		The positive matrix.
	inline Matrix4 operator + ( ) const;
	//! Get negative matrix.
	//! @param		none
	//! @return		The negative matrix.
	inline Matrix4 operator - ( ) const;

	//! Get a row from matrix.
	//! @param		row			Rom number of the matrix.
	//! @return		The row vector.
	inline Vector4 GetRow( _dword row ) const;
	//! Get a column from matrix.
	//! @param		column		Column number of the matrix.
	//! @return		The column vector.
	inline Vector4 GetColumn( _dword column ) const;

	//! Get the rotation matrix, without translation.
	//! @param		none
	//! @return		The rotation matrix.
	inline Matrix3 GetRotationMatrix( ) const;

	//! Get the translation vector, without rotation.
	//! @param		none
	//! @return		The translation vector.
	inline Vector3 GetTranslationVector( ) const;

	//! Add a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to add.
	//! @return		The result matrix of addition.
	Matrix4& operator += ( const Matrix4& matrix );
	//! Subtract a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to subtract.
	//! @return		The result matrix of subtraction.
	Matrix4& operator -= ( const Matrix4& matrix );
	//! Multiply a matrix to current matrix, and save result in current one.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result matrix of multiplication.
	Matrix4& operator *= ( const Matrix4& matrix );
	//! Multiply a scalar to current matrix, and save result in current one.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result matrix of multiplication.
	Matrix4& operator *= ( _float scalar );
	//! Divide a scalar to current matrix, and save result in current one.
	//! @param		scalar		The scalar to divide.
	//! @return		The result quaternion of division.
	Matrix4& operator /= ( _float scalar );

	//! Add a matrix to current matrix, return a new matrix to save result.
	//! @param		matrix		The matrix to add.
	//! @return		The result matrix of addition.
	Matrix4 operator + ( const Matrix4& matrix ) const;
	//! Subtract a matrix to current matrix, return a new matrix to save result.
	//! @param		matrix		The matrix to subtract.
	//! @return		The result matrix of subtraction.
    Matrix4 operator - ( const Matrix4& matrix ) const;
	//! Multiply a matrix to current matrix, return a new matrix to save result.
	//! @param		matrix		The matrix to multiply.
	//! @return		The result matrix of multiplication.
    Matrix4 operator * ( const Matrix4& matrix ) const;
	//! Multiply a scalar to current matrix, return a new matrix to save result.
	//! @param		scalar		The scalar to multiply.
	//! @return		The result matrix of multiplication.
    Matrix4 operator * ( _float scalar ) const;
	//! Divide a scalar to current matrix, return a new matrix to save result.
	//! @param		scalar		The scalar to divide.
	//! @return		The result quaternion of division.
    Matrix4 operator / ( _float scalar ) const;

	//! Compare two matrix.
	//! @param		matrix		The matrix to compare with.
	//! @return		True if they are identical, false otherwise.
	_bool operator == ( const Matrix4& matrix ) const;
	//! Compare two matrix.
	//! @param		matrix		The matrix to compare with.
	//! @return		True if they are not identical, false otherwise.
	_bool operator != ( const Matrix4& matrix ) const;

	//! Compare two matrix with error margin.
	//! @param		matrix		The matrix to compare with.
	//! @param		margin		The margin that allows some error.
	//! @return		True if they are identical, false otherwise.
	_bool Equal( const Matrix4& matrix, _float margin = Math::cEpsilon ) const;

	//! If a matrix has only translation.
	//! @param		none
	//! @return		True if matrix has only translation, false otherwise.
	_bool OnlyTranslation( ) const;

	//! Calculates the determinant of the matrix.
	//! @param		none
	//! @return		The determinant of the matrix.
	_float Determinant( ) const;

	//! Calculates identity matrix, and save result in current one.
	//! @param		none
	//! @return		The identity matrix.
	Matrix4& Identity( );
	//! Calculates transpose matrix, and save result in current one.
	//! @param		none
	//! @return		The transpose matrix.
	Matrix4& Transpose( );
	//! Calculates inverse matrix, and save result in current one.
	//! @param		none
	//! @return		The inverse matrix.
	Matrix4& Inverse( );
	//! Calculates adjoint matrix, and save result in current one.
	//! @param		none
	//! @return		The adjoint matrix.
	Matrix4& Adjoint( );

	//! Create a translation matrix.
	//! @param		x			The x-component of translation.
	//! @param		y			The y-component of translation.
	//! @param		z			The z-component of translation.
	//! @return		The translation matrix.
	Matrix4& Translation( _float x, _float y, _float z );
	//! Create a translation matrix.
	//! @param		vector		The translation vector.
	//! @return		The translation matrix.
	Matrix4& Translation( const Vector3& vector );

	//! Create a scaling matrix.
	//! @param		x			The x-component of scaling.
	//! @param		y			The y-component of scaling.
	//! @param		z			The z-component of scaling.
	//! @return		The scaling matrix.
	Matrix4& Scaling( _float x, _float y, _float z );
	//! Create a scaling matrix.
	//! @param		vector		The scaling vector.
	//! @return		The scaling matrix.
	Matrix4& Scaling( const Vector3& vector );
	//! Create a scaling matrix by scale in an arbitrary direction.
	//! @param		vector		The normal vector defines the direction of scale.
	//! @param		factor		The scale factor.
	//! @return		The scaling matrix.
	Matrix4& Scaling( const Vector3& vector, _float factor );

	//! Create a rotation matrix by rotate around x-axis.
	//! @param		radian		The angle of rotation in radian.
	//! @return		The rotation matrix.
	Matrix4& RotationX( _float radian );
	//! Create a rotation matrix by rotate around y-axis.
	//! @param		radian		The angle of rotation in radian.
	//! @return		The rotation matrix.
	Matrix4& RotationY( _float radian );
	//! Create a rotation matrix by rotate around z-axis.
	//! @param		radian		The angle of rotation in radian.
	//! @return		The rotation matrix.
	Matrix4& RotationZ( _float radian );
	//! Create a rotation matrix by rotate around an arbitrary axis.
	//! @param		axis		The axis to rotate around.
	//! @param		radian		The angle of rotation in radian.
	//! @return		The rotation matrix.
	Matrix4& Rotation( const Vector3& axis, _float radian );
	//! Create a rotation matrix that rotate one vector into another.
	//! @param		srcvector	The source vector to be rotated.
	//! @param		desvector	The target vector to rotate into.
	//! @return		The rotation matrix.
	Matrix4& Rotation( const Vector3& srcvector, const Vector3& desvector );
	//! Create a rotation matrix from a quaternion.
	//! @param		rotation	The quaternion of rotation.
	//! @return		The rotation matrix.
	Matrix4& Rotation( const Quaternion& rotation );
	//! Create a rotation matrix from a euler angles.
	//! @param		eulerangles	The euler angles of rotation.
	//! @return		The rotation matrix.
	Matrix4& Rotation( const EulerAngles& eulerangles );

	//! Create a projection matrix by project onto xy-plane.
	//! @param		none
	//! @return		The projection matrix.
	Matrix4& ProjectXY( );
	//! Create a projection matrix by project onto yz-plane.
	//! @param		none
	//! @return		The projection matrix.
	Matrix4& ProjectYZ( );
	//! Create a projection matrix by project onto zx-plane.
	//! @param		none
	//! @return		The projection matrix.
	Matrix4& ProjectZX( );
	//! Create a projection matrix by project onto an arbitrary plane.
	//! @param		vector		The normal vector defines the project plane.
	//! @return		The projection matrix.
	Matrix4& Project( const Vector3& vector );

	//! Create a reflection matrix by flip around xy-plane.
	//! @param		none
	//! @return		The reflection matrix.
	Matrix4& RefectionXY( );
	//! Create a reflection matrix by flip around yz-plane.
	//! @param		none
	//! @return		The reflection matrix.
	Matrix4& RefectionYZ( );
	//! Create a reflection matrix by flip around zx-plane.
	//! @param		none
	//! @return		The reflection matrix.
	Matrix4& RefectionZX( );
	//! Create a reflection matrix by flip around an arbitrary plane.
	//! @param		vector		The normal vector defines the reflect plane.
	//! @return		The reflection matrix.
	Matrix4& Refection( const Vector3& vector );

	//! Create a matrix from a matrix and a vector.
	//! @param		translation	The vector of translation.
	//! @param		rotation	The matrix of rotation.
	//! @return		The reference of current matrix.
	Matrix4& FromTranslationRotation( const Vector3& translation, const Matrix3& rotation );
	//! Get the translation vector and roation matrix from current matrix.
	//! @param		translation	The vector of translation, get from this function.
	//! @param		rotation	The matrix of rotation, get from this function.
	_void ToTranslationRotation( Vector3& translation, Matrix3& rotation ) const;

	//! Create a matrix from a quaternion and a vector.
	//! @param		translation	The vector of translation.
	//! @param		rotation	The quaternion of rotation.
	//! @return		The reference of current matrix.
	Matrix4& FromTranslationRotation( const Vector3& translation, const Quaternion& rotation );
	//! Get the translation vector and roation quaternion from current matrix.
	//! @param		translation	The vector of translation, get from this function.
	//! @param		rotation	The quaternion of rotation, get from this function.
	_void ToTranslationRotation( Vector3& translation, Quaternion& rotation ) const;

	//! Builds a left-handed orthogonal projection matrix.
	//! @param		width		Width of the view volume.
	//! @param		height		Height of the view volume.
	//! @param		znear		Z-value of the near view plane.
	//! @param		zfar		Z-value of the far view plane.
	//! @return		The left-handed orthogonal projection matrix.
	static Matrix4 CreateOrthoLH( _float width, _float height, _float znear, _float zfar );

	//! Builds a left-handed orthogonal projection matrix.
	//! @param		left		Minimum x-value of the view volume.
	//! @param		right		Maximum x-value of the view volume.
	//! @param		bottom		Minimum y-value of the view volume.
	//! @param		top			Maximum y-value of the view volume.
	//! @param		znear		Minimum z-value of the view volume.
	//! @param		zfar		Maximum z-value of the view volume.
	//! @return		The left-handed orthogonal projection matrix.
	static Matrix4 CreateOrthoOffCenterLH( _float left, _float right, _float bottom, _float top, _float znear, _float zfar );

	//! Builds a right-handed orthogonal projection matrix.
	//! @param		width		Width of the view volume.
	//! @param		height		Height of the view volume.
	//! @param		znear		Z-value of the near view plane.
	//! @param		zfar		Z-value of the far view plane.
	//! @return		The right-handed orthogonal projection matrix.
	static Matrix4 CreateOrthoRH( _float width, _float height, _float znear, _float zfar );

	//! Builds a right-handed orthogonal projection matrix.
	//! @param		left		Minimum x-value of the view volume.
	//! @param		right		Maximum x-value of the view volume.
	//! @param		bottom		Minimum y-value of the view volume.
	//! @param		top			Maximum y-value of the view volume.
	//! @param		znear		Minimum z-value of the view volume.
	//! @param		zfar		Maximum z-value of the view volume.
	//! @return		The right-handed orthogonal projection matrix.
	static Matrix4 CreateOrthoOffCenterRH( _float left, _float right, _float bottom, _float top, _float znear, _float zfar );

	//! Builds a left-handed perspective projection matrix.
	//! @param		width		Width of the view volume at the near view-plane.
	//! @param		height		Height of the view volume at the near view-plane.
	//! @param		znear		Z-value of the near view plane.
	//! @param		zfar		Z-value of the far view plane.
	//! @return		The left-handed perspective projection matrix.
	static Matrix4 CreatePerspectiveLH( _float width, _float height, _float znear, _float zfar );

	//! Builds a left-handed perspective projection matrix.
	//! @param		fovy		Field of view in the y direction, in radians.
	//! @param		aspect		Aspect ratio, defined as view space width divided by height.
	//! @param		znear		Z-value of the near view plane.
	//! @param		zfar		Z-value of the far view plane.
	//! @return		The left-handed perspective projection matrix.
	static Matrix4 CreatePerspectiveFovLH( _float fovy, _float aspect, _float znear, _float zfar );

	//! Builds a left-handed perspective projection matrix.
	//! @param		left		Minimum x-value of the view volume.
	//! @param		right		Maximum x-value of the view volume.
	//! @param		bottom		Minimum y-value of the view volume.
	//! @param		top			Maximum y-value of the view volume.
	//! @param		znear		Minimum z-value of the view volume.
	//! @param		zfar		Maximum z-value of the view volume.
	//! @return		The left-handed perspective projection matrix.
	static Matrix4 CreatePerspectiveOffCenterLH( _float left, _float right, _float bottom, _float top, _float znear, _float zfar );

	//! Builds a right-handed perspective projection matrix.
	//! @param		width		Width of the view volume at the near view-plane.
	//! @param		height		Height of the view volume at the near view-plane.
	//! @param		znear		Z-value of the near view plane.
	//! @param		zfar		Z-value of the far view plane.
	//! @return		The right-handed perspective projection matrix.
	static Matrix4 CreatePerspectiveRH( _float width, _float height, _float znear, _float zfar );

	//! Builds a right-handed perspective projection matrix.
	//! @param		fovy		Field of view in the y direction, in radians.
	//! @param		aspect		Aspect ratio, defined as view space width divided by height.
	//! @param		znear		Z-value of the near view plane.
	//! @param		zfar		Z-value of the far view plane.
	//! @return		The right-handed perspective projection matrix.
	static Matrix4 CreatePerspectiveFovRH( _float fovy, _float aspect, _float znear, _float zfar );

	//! Builds a right-handed perspective projection matrix.
	//! @param		left		Minimum x-value of the view volume.
	//! @param		right		Maximum x-value of the view volume.
	//! @param		bottom		Minimum y-value of the view volume.
	//! @param		top			Maximum y-value of the view volume.
	//! @param		znear		Minimum z-value of the view volume.
	//! @param		zfar		Maximum z-value of the view volume.
	//! @return		The right-handed perspective projection matrix.
	static Matrix4 CreatePerspectiveOffCenterRH( _float left, _float right, _float bottom, _float top, _float znear, _float zfar );

	//Builds a left-handed look-at matrix.
	//! @param		eye			The eye point.
	//! @param		lookat		The camera look-at target.
	//! @param		upaxis		The current world's up ( not the camera's up ).
	//! @return		The left-handed look-at matrix.
	static Matrix4 CreateLookAtLH( const Vector3& eye, const Vector3& lookat, const Vector3& upaxis );
	//Builds a right-handed look-at matrix.
	//! @param		eye			The eye point.
	//! @param		lookat		The camera look-at target.
	//! @param		upaxis		The current world's up ( not the camera's up ).
	//! @return		The right-handed look-at matrix.
	static Matrix4 CreateLookAtRH( const Vector3& eye, const Vector3& lookat, const Vector3& upaxis );
};

#pragma pack( pop ) 

//----------------------------------------------------------------------------
// Matrix4 Implementation
//----------------------------------------------------------------------------

Matrix4::Matrix4( )
{
}

Matrix4::Matrix4( _float m00, _float m01, _float m02, _float m03, _float m10, _float m11, _float m12, _float m13,
	_float m20, _float m21, _float m22, _float m23, _float m30, _float m31, _float m32, _float m33 )
{
	m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
	m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
	m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
	m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
}

Matrix4::Matrix4( const Matrix3 &matrix )
{
	m[0][0] = matrix( 0, 0 ); m[0][1] = matrix( 0, 1 ); m[0][2] = matrix( 0, 2 ); m[0][3] = 0.0f;
	m[1][0] = matrix( 1, 0 ); m[1][1] = matrix( 1, 1 ); m[1][2] = matrix( 1, 2 ); m[1][3] = 0.0f;
	m[2][0] = matrix( 2, 0 ); m[2][1] = matrix( 2, 1 ); m[2][2] = matrix( 2, 2 ); m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 0.0f;
}

Matrix4::Matrix4( const Matrix4 &matrix )
{
	_float4* m1 = (_float4*) m;
	_float4* m2 = (_float4*) matrix.m;

	if( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		*m1 = *m2;
		*( m1 + 1 ) = *( m2 + 1 );
		*( m1 + 2 ) = *( m2 + 2 );
		*( m1 + 3 ) = *( m2 + 3 );
	}
	else
	{
		m[0][0] = matrix.m[0][0]; m[0][1] = matrix.m[0][1]; m[0][2] = matrix.m[0][2]; m[0][3] = matrix.m[0][3];
		m[1][0] = matrix.m[1][0]; m[1][1] = matrix.m[1][1]; m[1][2] = matrix.m[1][2]; m[1][3] = matrix.m[1][3];
		m[2][0] = matrix.m[2][0]; m[2][1] = matrix.m[2][1]; m[2][2] = matrix.m[2][2]; m[2][3] = matrix.m[2][3];
		m[3][0] = matrix.m[3][0]; m[3][1] = matrix.m[3][1]; m[3][2] = matrix.m[3][2]; m[3][3] = matrix.m[3][3];
	}
}

Matrix4& Matrix4::operator = ( const Matrix3 &matrix )
{
	m[0][0] = matrix( 0, 0 ); m[0][1] = matrix( 0, 1 ); m[0][2] = matrix( 0, 2 ); m[0][3] = 0.0f;
	m[1][0] = matrix( 1, 0 ); m[1][1] = matrix( 1, 1 ); m[1][2] = matrix( 1, 2 ); m[1][3] = 0.0f;
	m[2][0] = matrix( 2, 0 ); m[2][1] = matrix( 2, 1 ); m[2][2] = matrix( 2, 2 ); m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 0.0f;

	return *this;
}

Matrix4& Matrix4::operator = ( const Matrix4 &matrix )
{
	_float4* m1 = (_float4*) m;
	_float4* m2 = (_float4*) matrix.m;

	if( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		*m1 = *m2;
		*( m1 + 1 ) = *( m2 + 1 );
		*( m1 + 2 ) = *( m2 + 2 );
		*( m1 + 3 ) = *( m2 + 3 );
	}
	else
	{
		m[0][0] = matrix.m[0][0]; m[0][1] = matrix.m[0][1]; m[0][2] = matrix.m[0][2]; m[0][3] = matrix.m[0][3];
		m[1][0] = matrix.m[1][0]; m[1][1] = matrix.m[1][1]; m[1][2] = matrix.m[1][2]; m[1][3] = matrix.m[1][3];
		m[2][0] = matrix.m[2][0]; m[2][1] = matrix.m[2][1]; m[2][2] = matrix.m[2][2]; m[2][3] = matrix.m[2][3];
		m[3][0] = matrix.m[3][0]; m[3][1] = matrix.m[3][1]; m[3][2] = matrix.m[3][2]; m[3][3] = matrix.m[3][3];
	}

	return *this;
}

const _float& Matrix4::operator ( ) ( _dword row, _dword column ) const
{
	FL_ASSERT( row < 4 && column < 4 )

	return m[ row ][ column ];
}

_float& Matrix4::operator ( ) ( _dword row, _dword column )
{
	FL_ASSERT( row < 4 && column < 4 )

	return m[ row ][ column ];
}

Matrix4 Matrix4::operator + ( ) const
{
	return Matrix4( m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1], m[1][2], m[1][3],
		m[2][0], m[2][1], m[2][2], m[2][3], m[3][0], m[3][1], m[3][2], m[3][3] );
}

Matrix4 Matrix4::operator - ( ) const
{
	return Matrix4( -m[0][0], -m[0][1], -m[0][2], -m[0][3], -m[1][0], -m[1][1], -m[1][2], -m[1][3],
		-m[2][0], -m[2][1], -m[2][2], -m[2][3], -m[3][0], -m[3][1], -m[3][2], -m[3][3] );
}

Vector4 Matrix4::GetRow( _dword row ) const
{
	FL_ASSERT( row < 4 )

	return Vector4( m[ row ][0], m[ row ][1], m[ row ][2], m[ row ][3] );
}

Vector4 Matrix4::GetColumn( _dword column ) const
{
	FL_ASSERT( column < 4 )

	return Vector4( m[0][ column ], m[1][ column ], m[2][ column ], m[3][ column ] );
}

Matrix3 Matrix4::GetRotationMatrix( ) const
{
	return Matrix3(
		m[0][0], m[0][1], m[0][2],
		m[1][0], m[1][1], m[1][2],
		m[2][0], m[2][1], m[2][2] );
}

Vector3 Matrix4::GetTranslationVector( ) const
{
	return Vector3( m[3][0], m[3][1], m[3][2] );
}

};