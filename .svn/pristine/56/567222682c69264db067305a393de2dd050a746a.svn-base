//============================================================================
// Matrix3x4.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Matrix3x4 Implementation
//----------------------------------------------------------------------------

const Matrix3x4 Matrix3x4::cZero( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
const Matrix3x4 Matrix3x4::cIdentity( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );

Matrix3x4& Matrix3x4::operator += ( const Matrix3x4& matrix )
{
	m[0][0] += matrix.m[0][0]; m[0][1] += matrix.m[0][1]; m[0][2] += matrix.m[0][2]; m[0][3] += matrix.m[0][3];
	m[1][0] += matrix.m[1][0]; m[1][1] += matrix.m[1][1]; m[1][2] += matrix.m[1][2]; m[1][3] += matrix.m[1][3];
	m[2][0] += matrix.m[2][0]; m[2][1] += matrix.m[2][1]; m[2][2] += matrix.m[2][2]; m[2][3] += matrix.m[2][3];

	return *this;
}

Matrix3x4& Matrix3x4::operator -= ( const Matrix3x4& matrix )
{
	m[0][0] -= matrix.m[0][0]; m[0][1] -= matrix.m[0][1]; m[0][2] -= matrix.m[0][2]; m[0][3] -= matrix.m[0][3];
	m[1][0] -= matrix.m[1][0]; m[1][1] -= matrix.m[1][1]; m[1][2] -= matrix.m[1][2]; m[1][3] -= matrix.m[1][3];
	m[2][0] -= matrix.m[2][0]; m[2][1] -= matrix.m[2][1]; m[2][2] -= matrix.m[2][2]; m[2][3] -= matrix.m[2][3];

	return *this;
}

Matrix3x4& Matrix3x4::operator *= ( _float scalar )
{
	m[0][0] *= scalar; m[0][1] *= scalar; m[0][2] *= scalar; m[0][3] *= scalar;
	m[1][0] *= scalar; m[1][1] *= scalar; m[1][2] *= scalar; m[1][3] *= scalar;
	m[2][0] *= scalar; m[2][1] *= scalar; m[2][2] *= scalar; m[2][3] *= scalar;

	return *this;
}

Matrix3x4& Matrix3x4::operator /= ( _float scalar )
{
	m[0][0] /= scalar; m[0][1] /= scalar; m[0][2] /= scalar; m[0][3] /= scalar;
	m[1][0] /= scalar; m[1][1] /= scalar; m[1][2] /= scalar; m[1][3] /= scalar;
	m[2][0] /= scalar; m[2][1] /= scalar; m[2][2] /= scalar; m[2][3] /= scalar;

	return *this;
}

Matrix3x4 Matrix3x4::operator + ( const Matrix3x4& matrix ) const
{
	Matrix3x4 returnmatrix = *this;

	return returnmatrix += matrix;
}

Matrix3x4 Matrix3x4::operator - ( const Matrix3x4& matrix ) const
{
	Matrix3x4 returnmatrix = *this;

	return returnmatrix -= matrix;
}

Matrix3x4 Matrix3x4::operator * ( _float scalar ) const
{
	Matrix3x4 returnmatrix = *this;

	return returnmatrix *= scalar;
}

Matrix3x4 Matrix3x4::operator / ( _float scalar ) const
{
	Matrix3x4 returnmatrix = *this;

	return returnmatrix /= scalar;
}

Matrix3x4& Matrix3x4::Translation( _float x, _float y, _float z )
{
	m[0][3] = x; m[1][3] = y; m[2][3] = z;

	return *this;
}

Matrix3x4& Matrix3x4::Translation( const Vector3& vector )
{
	return Translation( vector.x, vector.y, vector.z );
}

Matrix3x4& Matrix3x4::RotationX( _float radian )
{
	_float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

	m[0][0]	= 1.0f; m[1][0] =       0.0f; m[2][0] =     0.0f;
	m[0][1] = 0.0f; m[1][1] =   cosvalue; m[2][1] = sinvalue;
	m[0][2] = 0.0f; m[1][2] = - sinvalue; m[2][2] = cosvalue;

	return *this;
}

Matrix3x4& Matrix3x4::RotationY( _float radian )
{
	_float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

	m[0][0]	= cosvalue; m[1][0] = 0.0f; m[2][0] = - sinvalue;
	m[0][1] =     0.0f; m[1][1] = 1.0f; m[2][1] =       0.0f;
	m[0][2] = sinvalue; m[1][2] = 0.0f; m[2][2] =   cosvalue;

	return *this;
}

Matrix3x4& Matrix3x4::RotationZ( _float radian )
{
	_float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

	m[0][0]	=   cosvalue; m[1][0] = sinvalue; m[2][0] = 0.0f;
	m[0][1] = - sinvalue; m[1][1] = cosvalue; m[2][1] = 0.0f;
	m[0][2] =       0.0f; m[1][2] =     0.0f; m[2][2] = 1.0f;

	return *this;
}

Matrix3x4& Matrix3x4::FromTranslationRotation( const Vector3& translation, const Matrix3& rotation )
{
	m[0][0] = rotation.m[0][0]; m[1][0] = rotation.m[0][1]; m[2][0] = rotation.m[0][2];
	m[0][1] = rotation.m[1][0]; m[1][1] = rotation.m[1][1]; m[2][1] = rotation.m[1][2];
	m[0][2] = rotation.m[2][0]; m[1][2] = rotation.m[2][1]; m[2][2] = rotation.m[2][2];
	m[0][3] =    translation.x; m[1][3] =    translation.y; m[2][3] =    translation.z;

	return *this;
}

_void Matrix3x4::ToTranslationRotation( Vector3& translation, Matrix3& rotation ) const
{
	translation.x = m[0][3];
	translation.y = m[1][3];
	translation.z = m[2][3];

	rotation.m[0][0] = m[0][0]; rotation.m[0][1] = m[1][0]; rotation.m[0][2] = m[2][0];
	rotation.m[1][0] = m[0][1]; rotation.m[1][1] = m[1][1]; rotation.m[1][2] = m[2][1];
	rotation.m[2][0] = m[0][2]; rotation.m[2][1] = m[1][2]; rotation.m[2][2] = m[2][2];
}