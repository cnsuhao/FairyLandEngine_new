//============================================================================
// Matrix2.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Matrix2 Implementation
//----------------------------------------------------------------------------

const Matrix2 Matrix2::cZero( 0.0f, 0.0f, 0.0f, 0.0f );
const Matrix2 Matrix2::cIdentity( 1.0f, 0.0f, 0.0f, 1.0f );

Matrix2& Matrix2::operator += ( const Matrix2& matrix )
{
	m[0][0] += matrix.m[0][0]; m[0][1] += matrix.m[0][1];
	m[1][0] += matrix.m[1][0]; m[1][1] += matrix.m[1][1];

	return *this;
}

Matrix2& Matrix2::operator -= ( const Matrix2& matrix )
{
	m[0][0] -= matrix.m[0][0]; m[0][1] -= matrix.m[0][1];
	m[1][0] -= matrix.m[1][0]; m[1][1] -= matrix.m[1][1];

	return *this;
}

Matrix2& Matrix2::operator *= ( const Matrix2& matrix )
{
	_float m00 = m[0][0], m01 = m[0][1], m10 = m[1][0], m11 = m[1][1];

	m[0][0] = m00 * matrix.m[0][0] + m01 * matrix.m[1][0];
	m[0][1] = m00 * matrix.m[0][1] + m01 * matrix.m[1][1];
	m[1][0] = m10 * matrix.m[0][0] + m11 * matrix.m[1][0];
	m[1][1] = m10 * matrix.m[0][1] + m11 * matrix.m[1][1];

	return *this;
}

Matrix2& Matrix2::operator *= ( _float scalar )
{
	m[0][0] *= scalar; m[0][1] *= scalar;
	m[1][0] *= scalar; m[1][1] *= scalar;

	return *this;
}

Matrix2& Matrix2::operator /= ( _float scalar )
{
	m[0][0] /= scalar; m[0][1] /= scalar;
	m[1][0] /= scalar; m[1][1] /= scalar;

	return *this;
}

Matrix2 Matrix2::operator + ( const Matrix2& matrix ) const
{
	Matrix2 returnmatrix = *this;

	return returnmatrix += matrix;
}

Matrix2 Matrix2::operator - ( const Matrix2& matrix ) const
{
	Matrix2 returnmatrix = *this;

	return returnmatrix -= matrix;
}

Matrix2 Matrix2::operator * ( const Matrix2& matrix ) const
{
	Matrix2 returnmatrix = *this;

	return returnmatrix *= matrix;
}

Matrix2 Matrix2::operator * ( _float scalar ) const
{
	Matrix2 returnmatrix = *this;

	return returnmatrix *= scalar;
}

Matrix2 Matrix2::operator / ( _float scalar ) const
{
	Matrix2 returnmatrix = *this;

	return returnmatrix /= scalar;
}

_bool Matrix2::operator == ( const Matrix2& matrix ) const
{
	for ( _dword i = 0; i < 2; i ++ )
	{
		for ( _dword j = 0; j < 2; j ++ )
		{
			if ( m[i][j] != matrix.m[i][j] )
				return _false;
		}
	}

	return _true;
}

_bool Matrix2::operator != ( const Matrix2& matrix ) const
{
	return ( operator == ( matrix ) ) == _false;
}

_float Matrix2::Determinant( ) const
{
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

Matrix2& Matrix2::Identity( )
{
	m[0][0]	= 1.0f; m[0][1] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f;

	return *this;
}

Matrix2& Matrix2::Transpose( )
{
	_float m01 = m[0][1], m10 = m[1][0];

	m[0][1] = m10;
	m[1][0] = m01;

	return *this;
}

Matrix2& Matrix2::Inverse( )
{
	_float determinant = Determinant( );

	if ( determinant != 0.0f )
	{
		Adjoint( );

		m[0][0] /= determinant; m[0][1] /= determinant;
		m[1][0] /= determinant; m[1][1] /= determinant;
	}

	return *this;
}

Matrix2& Matrix2::Adjoint( )
{
	_float m00 = m[0][0], m01 = m[0][1], m10 = m[1][0], m11 = m[1][1];

	m[0][0] =   m11; m[0][1] = - m01;
	m[1][0] = - m10; m[1][1] =   m00;

	return *this;
}

Matrix2& Matrix2::Scaling( _float x, _float y )
{
	m[0][0]	=    x; m[0][1] = 0.0f;
	m[1][0] = 0.0f; m[1][1] =    y;

	return *this;
}

Matrix2& Matrix2::Scaling( const Vector2& vector )
{
	return Scaling( vector.x, vector.y );
}

Matrix2& Matrix2::Scaling( const Vector2& vector, _float factor )
{
	Vector2 normal( vector );
	normal.Normalize( );

	_float k  = factor - 1.0f;
	_float xx = normal.x * normal.x;
	_float yy = normal.y * normal.y;
	_float xy = normal.x * normal.y;

	m[0][0]	= 1.0f + k * xx; m[0][1] =        k * xy;
	m[1][0]	=        k * xy; m[1][1] = 1.0f + k * yy;

	return *this;
}

Matrix2& Matrix2::Rotation( _float radian )
{
	_float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

	m[0][0]	=   cosvalue; m[0][1] = sinvalue;
	m[1][0] = - sinvalue; m[1][1] = cosvalue;

	return *this;
}

Matrix2& Matrix2::ProjectX( )
{
	m[0][0] = 1.0f; m[0][1] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 0.0f;

	return *this;
}

Matrix2& Matrix2::ProjectY( )
{
	m[0][0] = 0.0f; m[0][1] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f;

	return *this;
}

Matrix2& Matrix2::Project( const Vector2& vector )
{
	return Scaling( vector, 0.0f );
}

Matrix2& Matrix2::RefectionX( )
{
	m[0][0] = 1.0f; m[0][1] =  0.0f;
	m[1][0] = 0.0f; m[1][1] = -1.0f;

	return *this;
}

Matrix2& Matrix2::RefectionY( )
{
	m[0][0] = -1.0f; m[0][1] = 0.0f;
	m[1][0] =  0.0f; m[1][1] = 1.0f;

	return *this;
}

Matrix2& Matrix2::Refection( const Vector2& vector )
{
	return Scaling( vector, -1.0f );
}