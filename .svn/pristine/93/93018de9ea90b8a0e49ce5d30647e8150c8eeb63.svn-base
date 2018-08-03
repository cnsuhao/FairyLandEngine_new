//============================================================================
// Matrix3.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Matrix3 Implementation
//----------------------------------------------------------------------------

const Matrix3 Matrix3::cZero( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
const Matrix3 Matrix3::cIdentity( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f );

Matrix3& Matrix3::operator += ( const Matrix3& matrix )
{
	m[0][0] += matrix.m[0][0]; m[0][1] += matrix.m[0][1]; m[0][2] += matrix.m[0][2];
	m[1][0] += matrix.m[1][0]; m[1][1] += matrix.m[1][1]; m[1][2] += matrix.m[1][2];
	m[2][0] += matrix.m[2][0]; m[2][1] += matrix.m[2][1]; m[2][2] += matrix.m[2][2];

	return *this;
}

Matrix3& Matrix3::operator -= ( const Matrix3& matrix )
{
	m[0][0] -= matrix.m[0][0]; m[0][1] -= matrix.m[0][1]; m[0][2] -= matrix.m[0][2];
	m[1][0] -= matrix.m[1][0]; m[1][1] -= matrix.m[1][1]; m[1][2] -= matrix.m[1][2];
	m[2][0] -= matrix.m[2][0]; m[2][1] -= matrix.m[2][1]; m[2][2] -= matrix.m[2][2];

	return *this;
}

Matrix3& Matrix3::operator *= ( const Matrix3& matrix )
{
	_float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m10 = m[1][0], m11 = m[1][1],
		m12 = m[1][2], m20 = m[2][0], m21 = m[2][1], m22 = m[2][2];

	m[0][0] = m00 * matrix.m[0][0] + m01 * matrix.m[1][0] + m02 * matrix.m[2][0];
	m[0][1] = m00 * matrix.m[0][1] + m01 * matrix.m[1][1] + m02 * matrix.m[2][1];
	m[0][2] = m00 * matrix.m[0][2] + m01 * matrix.m[1][2] + m02 * matrix.m[2][2];

	m[1][0] = m10 * matrix.m[0][0] + m11 * matrix.m[1][0] + m12 * matrix.m[2][0];
	m[1][1] = m10 * matrix.m[0][1] + m11 * matrix.m[1][1] + m12 * matrix.m[2][1];
	m[1][2] = m10 * matrix.m[0][2] + m11 * matrix.m[1][2] + m12 * matrix.m[2][2];

	m[2][0] = m20 * matrix.m[0][0] + m21 * matrix.m[1][0] + m22 * matrix.m[2][0];
	m[2][1] = m20 * matrix.m[0][1] + m21 * matrix.m[1][1] + m22 * matrix.m[2][1];
	m[2][2] = m20 * matrix.m[0][2] + m21 * matrix.m[1][2] + m22 * matrix.m[2][2];

	return *this;
}

Matrix3& Matrix3::operator *= ( _float scalar )
{
	m[0][0] *= scalar; m[0][1] *= scalar; m[0][2] *= scalar;
	m[1][0] *= scalar; m[1][1] *= scalar; m[1][2] *= scalar;
	m[2][0] *= scalar; m[2][1] *= scalar; m[2][2] *= scalar;

	return *this;
}

Matrix3& Matrix3::operator /= ( _float scalar )
{
	m[0][0] /= scalar; m[0][1] /= scalar; m[0][2] /= scalar;
	m[1][0] /= scalar; m[1][1] /= scalar; m[1][2] /= scalar;
	m[2][0] /= scalar; m[2][1] /= scalar; m[2][2] /= scalar;

	return *this;
}

Matrix3 Matrix3::operator + ( const Matrix3& matrix ) const
{
	Matrix3 returnmatrix = *this;

	return returnmatrix += matrix;
}

Matrix3 Matrix3::operator - ( const Matrix3& matrix ) const
{
	Matrix3 returnmatrix = *this;

	return returnmatrix -= matrix;
}

Matrix3 Matrix3::operator * ( const Matrix3& matrix ) const
{
	Matrix3 returnmatrix = *this;

	return returnmatrix *= matrix;
}

Matrix3 Matrix3::operator * ( _float scalar ) const
{
	Matrix3 returnmatrix = *this;

	return returnmatrix *= scalar;
}

Matrix3 Matrix3::operator / ( _float scalar ) const
{
	Matrix3 returnmatrix = *this;

	return returnmatrix /= scalar;
}

_bool Matrix3::operator == ( const Matrix3& matrix ) const
{
	for ( _dword i = 0; i < 3; i ++ )
	{
		for ( _dword j = 0; j < 3; j ++ )
		{
			if ( m[i][j] != matrix.m[i][j] )
				return _false;
		}
	}

	return _true;
}

_bool Matrix3::operator != ( const Matrix3& matrix ) const
{
	return ( operator == ( matrix ) ) == _false;
}

_float Matrix3::Determinant( ) const
{
	return m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1]
		 - m[0][0] * m[1][2] * m[2][1] - m[0][1] * m[1][0] * m[2][2] - m[0][2] * m[1][1] * m[2][0];
}

Matrix3& Matrix3::Identity( )
{
	m[0][0]	= 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;

	return *this;
}

Matrix3& Matrix3::Transpose( )
{
	_float m01 = m[0][1], m02 = m[0][2], m10 = m[1][0], m12 = m[1][2], m20 = m[2][0], m21 = m[2][1];

	m[0][1] = m10; m[0][2] = m20;
	m[1][0] = m01; m[1][2] = m21;
	m[2][0] = m02; m[2][1] = m12;

	return *this;
} 

Matrix3& Matrix3::Inverse( )
{
	_float determinant = Determinant( );

	if ( determinant != 0.0f )
	{
		Adjoint( );

		m[0][0] /= determinant; m[0][1] /= determinant; m[0][2] /= determinant;
		m[1][0] /= determinant; m[1][1] /= determinant; m[1][2] /= determinant;
		m[2][0] /= determinant; m[2][1] /= determinant; m[2][2] /= determinant;
	}

	return *this;
}

Matrix3& Matrix3::Adjoint( )
{
	_float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m10 = m[1][0], m11 = m[1][1],
		m12 = m[1][2], m20 = m[2][0], m21 = m[2][1], m22 = m[2][2];

	m[0][0] = m11 * m22 - m12 * m21; m[0][1] = m02 * m21 - m01 * m22; m[0][2] = m01 * m12 - m02 * m11;
	m[1][0] = m12 * m20 - m10 * m22; m[1][1] = m00 * m22 - m02 * m20; m[1][2] = m02 * m10 - m00 * m12;
	m[2][0] = m10 * m21 - m11 * m20; m[2][1] = m01 * m20 - m00 * m21; m[2][2] = m00 * m11 - m01 * m10;

	return *this;
}

Matrix3& Matrix3::Translation( _float x, _float y )
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
	m[2][0] =    x; m[2][1] =    y; m[2][2] = 1.0f;

	return *this;
}

Matrix3& Matrix3::Translation( const Vector2& vector )
{
	return Translation( vector.x, vector.y );
}

Matrix3& Matrix3::Scaling( _float x, _float y, _float z )
{
	m[0][0]	=    x; m[0][1] = 0.0f; m[0][2] = 0.0f;
	m[1][0] = 0.0f; m[1][1] =    y; m[1][2] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] =    z;

	return *this;
}

Matrix3& Matrix3::Scaling( const Vector3& vector )
{
	return Scaling( vector.x, vector.y, vector.z );
}

Matrix3& Matrix3::Scaling( const Vector3& vector, _float factor )
{
	Vector3 normal( vector );
	normal.Normalize( );

	_float k  = factor - 1.0f;
	_float xx = normal.x * normal.x;
	_float yy = normal.y * normal.y;
	_float zz = normal.z * normal.z;
	_float xy = normal.x * normal.y;
	_float yz = normal.y * normal.z;
	_float zx = normal.z * normal.x;

	m[0][0]	= 1.0f + k * xx; m[0][1] =        k * xy; m[0][2] =        k * zx;
	m[1][0]	=        k * xy; m[1][1] = 1.0f + k * yy; m[1][2] =        k * yz;
    m[2][0]	=        k * zx; m[2][1] =        k * yz; m[2][2] = 1.0f + k * zz;

	return *this;
}

Matrix3& Matrix3::Rotation( _float radian )
{
	_float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

	m[0][0]	=   cosvalue; m[0][1] = sinvalue; m[0][2] = 0.0f;
	m[1][0] = - sinvalue; m[1][1] = cosvalue; m[1][2] = 0.0f;
	m[2][0] =       0.0f; m[2][1] =     0.0f; m[2][2] = 1.0f;

	return *this;
}

Matrix3& Matrix3::RotationX( _float radian )
{
	_float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

	m[0][0]	= 1.0f; m[0][1] =       0.0f; m[0][2] =     0.0f;
	m[1][0] = 0.0f; m[1][1] =   cosvalue; m[1][2] = sinvalue;
	m[2][0] = 0.0f; m[2][1] = - sinvalue; m[2][2] = cosvalue;

	return *this;
}

Matrix3& Matrix3::RotationY( _float radian )
{
	_float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

	m[0][0]	= cosvalue; m[0][1] = 0.0f; m[0][2] = - sinvalue;
	m[1][0] =     0.0f; m[1][1] = 1.0f; m[1][2] =       0.0f;
	m[2][0] = sinvalue; m[2][1] = 0.0f; m[2][2] =   cosvalue;

	return *this;
}

Matrix3& Matrix3::RotationZ( _float radian )
{
	_float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

	m[0][0]	=   cosvalue; m[0][1] = sinvalue; m[0][2] = 0.0f;
	m[1][0] = - sinvalue; m[1][1] = cosvalue; m[1][2] = 0.0f;
	m[2][0] =       0.0f; m[2][1] =     0.0f; m[2][2] = 1.0f;

	return *this;
}

Matrix3& Matrix3::Rotation( const Vector3& axis, _float radian )
{
	_float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian ), cosreverse = 1.0f - cosvalue;

	Vector3 axisnormal( axis );
	axisnormal.Normalize( );

	m[0][0] = cosreverse * axisnormal.x * axisnormal.x + cosvalue;
	m[0][1] = cosreverse * axisnormal.x * axisnormal.y + sinvalue * axisnormal.z;
	m[0][2] = cosreverse * axisnormal.x * axisnormal.z - sinvalue * axisnormal.y;

	m[1][0] = cosreverse * axisnormal.x * axisnormal.y - sinvalue * axisnormal.z;
	m[1][1] = cosreverse * axisnormal.y * axisnormal.y + cosvalue;
	m[1][2] = cosreverse * axisnormal.y * axisnormal.z + sinvalue * axisnormal.x;

	m[2][0] = cosreverse * axisnormal.x * axisnormal.z + sinvalue * axisnormal.y;
	m[2][1] = cosreverse * axisnormal.y * axisnormal.z - sinvalue * axisnormal.x;
	m[2][2] = cosreverse * axisnormal.z * axisnormal.z + cosvalue;

	return *this;
}

Matrix3& Matrix3::ProjectXY( )
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 0.0f;

	return *this;
}

Matrix3& Matrix3::ProjectYZ( )
{
	m[0][0] = 0.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;

	return *this;
}

Matrix3& Matrix3::ProjectZX( )
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 0.0f; m[1][2] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;

	return *this;
}

Matrix3& Matrix3::Project( const Vector3& vector )
{
	return Scaling( vector, 0.0f );
}

Matrix3& Matrix3::RefectionXY( )
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] =  0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] =  0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = -1.0f;

	return *this;
}

Matrix3& Matrix3::RefectionYZ( )
{
	m[0][0] = -1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
	m[1][0] =  0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
	m[2][0] =  0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;

	return *this;
}

Matrix3& Matrix3::RefectionZX( )
{
	m[0][0] = 1.0f; m[0][1] =  0.0f; m[0][2] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = -1.0f; m[1][2] = 0.0f;
	m[2][0] = 0.0f; m[2][1] =  0.0f; m[2][2] = 1.0f;

	return *this;
}

Matrix3& Matrix3::Refection( const Vector3& vector )
{
	return Scaling( vector, -1.0f );
}

Matrix3& Matrix3::FromRotation( const Quaternion& rotation )
{
	_float xx = rotation.x * rotation.x * 2.0f, yy = rotation.y * rotation.y * 2.0f, zz = rotation.z * rotation.z * 2.0f;
	_float xy = rotation.x * rotation.y * 2.0f, zw = rotation.z * rotation.w * 2.0f, xz = rotation.x * rotation.z * 2.0f;
	_float yw = rotation.y * rotation.w * 2.0f, yz = rotation.y * rotation.z * 2.0f, xw = rotation.x * rotation.w * 2.0f;

	m[0][0] = 1.0f - yy - zz; m[0][1] =        xy + zw; m[0][2] =        xz - yw;
	m[1][0] =        xy - zw; m[1][1] = 1.0f - xx - zz; m[1][2] =        yz + xw;
	m[2][0] =        xz + yw; m[2][1] =        yz - xw; m[2][2] = 1.0f - xx - yy;

	return *this;
}

_void Matrix3::ToRotation( Quaternion& rotation ) const
{
	// Determine which of w, x, y, or z has the largest absolute value.
	_float fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];
	_float fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
	_float fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
	_float fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];

	_long biggestIndex = 0;
	_float fourBiggestSquaredMinus1 = fourWSquaredMinus1;

	if ( fourXSquaredMinus1 > fourBiggestSquaredMinus1 )
	{
		fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		biggestIndex = 1;
	}

	if ( fourYSquaredMinus1 > fourBiggestSquaredMinus1 )
	{
		fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		biggestIndex = 2;
	}

	if ( fourZSquaredMinus1 > fourBiggestSquaredMinus1 )
	{
		fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		biggestIndex = 3;
	}

	_float biggestVal = Math::Sqrt( fourBiggestSquaredMinus1 + 1.0f ) * 0.5f;
	_float mult = 0.25f / biggestVal;

	// Apply table to compute quaternion values.
	switch ( biggestIndex )
	{
		case 0:
			rotation.w = biggestVal;
			rotation.x = ( m[1][2] - m[2][1] ) * mult;
			rotation.y = ( m[2][0] - m[0][2] ) * mult;
			rotation.z = ( m[0][1] - m[1][0] ) * mult;
			break;

		case 1:
			rotation.x = biggestVal;
			rotation.w = ( m[1][2] - m[2][1] ) * mult;
			rotation.y = ( m[0][1] + m[1][0] ) * mult;
			rotation.z = ( m[2][0] + m[0][2] ) * mult;
			break;

		case 2:
			rotation.y = biggestVal;
			rotation.w = ( m[2][0] - m[0][2] ) * mult;
			rotation.x = ( m[0][1] + m[1][0] ) * mult;
			rotation.z = ( m[1][2] + m[2][1] ) * mult;
			break;

		case 3:
			rotation.z = biggestVal;
			rotation.w = ( m[0][1] - m[1][0] ) * mult;
			rotation.x = ( m[2][0] + m[0][2] ) * mult;
			rotation.y = ( m[1][2] + m[2][1] ) * mult;
			break;
	}
}