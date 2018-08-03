//============================================================================
// Matrix4.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Matrix4 Implementation
//----------------------------------------------------------------------------

#define _mm_ror_ps( vec, i )	\
	( ( ( i ) % 4 ) ? ( _mm_shuffle_ps ( vec, vec,	\
	_MM_SHUFFLE( ( _byte )( i + 3 ) % 4, ( _byte )( i + 2 ) % 4, ( _byte )( i + 1 ) % 4, ( _byte ) ( i + 0 ) % 4 ) ) ) : ( vec ) )

const Matrix4 Matrix4::cZero( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
const Matrix4 Matrix4::cIdentity( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
const Matrix4 Matrix4::cClip2Image( 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f );

Matrix4& Matrix4::operator += ( const Matrix4& matrix )
{
	_float4* m1 = (_float4*) m;
	_float4* m2 = (_float4*) matrix.m;

	if ( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		*( m1 ) = _mm_add_ps( *m1, *m2 );
		*( m1 + 1 ) = _mm_add_ps( *( m1 + 1 ), *( m2 + 1 ) );
		*( m1 + 2 ) = _mm_add_ps( *( m1 + 2 ), *( m2 + 2 ) );
		*( m1 + 3 ) = _mm_add_ps( *( m1 + 3 ), *( m2 + 3 ) );
	}
	else
	{
		m[0][0] += matrix.m[0][0]; m[0][1] += matrix.m[0][1]; m[0][2] += matrix.m[0][2]; m[0][3] += matrix.m[0][3];
		m[1][0] += matrix.m[1][0]; m[1][1] += matrix.m[1][1]; m[1][2] += matrix.m[1][2]; m[1][3] += matrix.m[1][3];
		m[2][0] += matrix.m[2][0]; m[2][1] += matrix.m[2][1]; m[2][2] += matrix.m[2][2]; m[2][3] += matrix.m[2][3];
		m[3][0] += matrix.m[3][0]; m[3][1] += matrix.m[3][1]; m[3][2] += matrix.m[3][2]; m[3][3] += matrix.m[3][3];
	}

	return *this;
}

Matrix4& Matrix4::operator -= ( const Matrix4& matrix )
{
	_float4* m1 = (_float4*) m;
	_float4* m2 = (_float4*) matrix.m;

	if ( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		_float4* m1 = (_float4*) m;
		_float4* m2 = (_float4*) matrix.m;

		*( m1 ) = _mm_sub_ps( *m1, *m2 );
		*( m1 + 1 ) = _mm_sub_ps( *( m1 + 1 ), *( m2 + 1 ) );
		*( m1 + 2 ) = _mm_sub_ps( *( m1 + 2 ), *( m2 + 2 ) );
		*( m1 + 3 ) = _mm_sub_ps( *( m1 + 3 ), *( m2 + 3 ) );
	}
	else
	{
		m[0][0] -= matrix.m[0][0]; m[0][1] -= matrix.m[0][1]; m[0][2] -= matrix.m[0][2]; m[0][3] -= matrix.m[0][3];
		m[1][0] -= matrix.m[1][0]; m[1][1] -= matrix.m[1][1]; m[1][2] -= matrix.m[1][2]; m[1][3] -= matrix.m[1][3];
		m[2][0] -= matrix.m[2][0]; m[2][1] -= matrix.m[2][1]; m[2][2] -= matrix.m[2][2]; m[2][3] -= matrix.m[2][3];
		m[3][0] -= matrix.m[3][0]; m[3][1] -= matrix.m[3][1]; m[3][2] -= matrix.m[3][2]; m[3][3] -= matrix.m[3][3];
	}

	return *this;
}

Matrix4& Matrix4::operator *= ( const Matrix4& matrix )
{
	_float4* m1 = (_float4*) m;
	_float4* m2 = (_float4*) matrix.m;

	if ( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		_float4 temp;

		_float4 _L1, _L2, _L3, _L4;
		_L1 = *( m2 );
		_L2 = *( m2 + 1 );
		_L3 = *( m2 + 2 );
		_L4 = *( m2 + 3 );

		temp = _mm_mul_ps( _mm_set_ps1( operator()( 0, 0 ) ), _L1 );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 1 ) ), _L2 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 2 ) ), _L3 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 3 ) ), _L4 ) );
		*m1 = temp;

		temp = _mm_mul_ps( _mm_set_ps1( operator()( 1, 0 ) ), _L1 );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 1 ) ), _L2 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 2 ) ), _L3 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 3 ) ), _L4 ) );
		*( m1 + 1 ) = temp;

		temp = _mm_mul_ps( _mm_set_ps1( operator()( 2, 0 ) ), _L1 );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 1 ) ), _L2 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 2 ) ), _L3 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 3 ) ), _L4 ) );
		*( m1 + 2 ) = temp;

		temp = _mm_mul_ps( _mm_set_ps1( operator()( 3, 0 ) ), _L1 );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 3, 1 ) ), _L2 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 3, 2 ) ), _L3 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 3, 3 ) ), _L4 ) );
		*( m1 + 3 ) = temp;
	}
	else
	{
		_float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3], m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3],
			m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3], m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

		m[0][0] = m00 * matrix.m[0][0] + m01 * matrix.m[1][0] + m02 * matrix.m[2][0] + m03 * matrix.m[3][0];
		m[0][1] = m00 * matrix.m[0][1] + m01 * matrix.m[1][1] + m02 * matrix.m[2][1] + m03 * matrix.m[3][1];
		m[0][2] = m00 * matrix.m[0][2] + m01 * matrix.m[1][2] + m02 * matrix.m[2][2] + m03 * matrix.m[3][2];
		m[0][3] = m00 * matrix.m[0][3] + m01 * matrix.m[1][3] + m02 * matrix.m[2][3] + m03 * matrix.m[3][3];

		m[1][0] = m10 * matrix.m[0][0] + m11 * matrix.m[1][0] + m12 * matrix.m[2][0] + m13 * matrix.m[3][0];
		m[1][1] = m10 * matrix.m[0][1] + m11 * matrix.m[1][1] + m12 * matrix.m[2][1] + m13 * matrix.m[3][1];
		m[1][2] = m10 * matrix.m[0][2] + m11 * matrix.m[1][2] + m12 * matrix.m[2][2] + m13 * matrix.m[3][2];
		m[1][3] = m10 * matrix.m[0][3] + m11 * matrix.m[1][3] + m12 * matrix.m[2][3] + m13 * matrix.m[3][3];

		m[2][0] = m20 * matrix.m[0][0] + m21 * matrix.m[1][0] + m22 * matrix.m[2][0] + m23 * matrix.m[3][0];
		m[2][1] = m20 * matrix.m[0][1] + m21 * matrix.m[1][1] + m22 * matrix.m[2][1] + m23 * matrix.m[3][1];
		m[2][2] = m20 * matrix.m[0][2] + m21 * matrix.m[1][2] + m22 * matrix.m[2][2] + m23 * matrix.m[3][2];
		m[2][3] = m20 * matrix.m[0][3] + m21 * matrix.m[1][3] + m22 * matrix.m[2][3] + m23 * matrix.m[3][3];

		m[3][0] = m30 * matrix.m[0][0] + m31 * matrix.m[1][0] + m32 * matrix.m[2][0] + m33 * matrix.m[3][0];
		m[3][1] = m30 * matrix.m[0][1] + m31 * matrix.m[1][1] + m32 * matrix.m[2][1] + m33 * matrix.m[3][1];
		m[3][2] = m30 * matrix.m[0][2] + m31 * matrix.m[1][2] + m32 * matrix.m[2][2] + m33 * matrix.m[3][2];
		m[3][3] = m30 * matrix.m[0][3] + m31 * matrix.m[1][3] + m32 * matrix.m[2][3] + m33 * matrix.m[3][3];
	}

	return *this;
}

Matrix4& Matrix4::operator *= ( _float scalar )
{
	if ( ( (_dword) m ) % 16 == 0 )
	{
		_float4* start = (_float4*) m;
		_float4 temp = _mm_set_ps1( scalar );

		*( start ) = _mm_mul_ps( *( start ), temp );
		*( start + 1 ) = _mm_mul_ps( *( start + 1 ), temp );
		*( start + 2 ) = _mm_mul_ps( *( start + 2 ), temp );
		*( start + 3 ) = _mm_mul_ps( *( start + 3 ), temp );
	}
	else
	{
		m[0][0] *= scalar; m[0][1] *= scalar; m[0][2] *= scalar; m[0][3] *= scalar;
		m[1][0] *= scalar; m[1][1] *= scalar; m[1][2] *= scalar; m[1][3] *= scalar;
		m[2][0] *= scalar; m[2][1] *= scalar; m[2][2] *= scalar; m[2][3] *= scalar;
		m[3][0] *= scalar; m[3][1] *= scalar; m[3][2] *= scalar; m[3][3] *= scalar;
	}

	return *this;
}

Matrix4& Matrix4::operator /= ( _float scalar )
{
	if ( ( (_dword) m ) % 16 == 0 )
	{
		_float4* start = (_float4*) m;
		_float4 temp = _mm_set_ps1( scalar );

		*( start ) = _mm_div_ps( *( start ), temp );
		*( start + 1 ) = _mm_div_ps( *( start + 1 ), temp );
		*( start + 2 ) = _mm_div_ps( *( start + 2 ), temp );
		*( start + 3 ) = _mm_div_ps( *( start + 3 ), temp );
	}
	else
	{
		m[0][0] /= scalar; m[0][1] /= scalar; m[0][2] /= scalar; m[0][3] /= scalar;
		m[1][0] /= scalar; m[1][1] /= scalar; m[1][2] /= scalar; m[1][3] /= scalar;
		m[2][0] /= scalar; m[2][1] /= scalar; m[2][2] /= scalar; m[2][3] /= scalar;
		m[3][0] /= scalar; m[3][1] /= scalar; m[3][2] /= scalar; m[3][3] /= scalar;
	}

	return *this;
}

Matrix4 Matrix4::operator + ( const Matrix4& matrix ) const
{
	Matrix4 returnmatrix;
	_float4* m1 = (_float4*) m;
	_float4* m2 = (_float4*) matrix.m;
	
	if ( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		_float4* result = (_float4*)returnmatrix.m;

		*( result ) = _mm_add_ps( *m1, *m2 );
		*( result + 1 ) = _mm_add_ps( *( m1 + 1 ), *( m2 + 1 ) );
		*( result + 2 ) = _mm_add_ps( *( m1 + 2 ), *( m2 + 2 ) );
		*( result + 3 ) = _mm_add_ps( *( m1 + 3 ), *( m2 + 3 ) );
	}
	else
	{
		returnmatrix = *this;
		returnmatrix += matrix;
	}

	return returnmatrix;
}

Matrix4 Matrix4::operator - ( const Matrix4& matrix ) const
{
	Matrix4 returnmatrix;
	_float4* m1 = (_float4*) m;
	_float4* m2 = (_float4*) matrix.m;
	
	if ( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		_float4* result = (_float4*)returnmatrix.m;
		_bool align = ( (_dword)result ) % 16 == 0 ? _true : _false;

		if ( align )
		{
			*( result ) = _mm_sub_ps( *m1, *m2 );
			*( result + 1 ) = _mm_sub_ps( *( m1 + 1 ), *( m2 + 1 ) );
			*( result + 2 ) = _mm_sub_ps( *( m1 + 2 ), *( m2 + 2 ) );
			*( result + 3 ) = _mm_sub_ps( *( m1 + 3 ), *( m2 + 3 ) );
		}
		else
		{
			_float4 temp;
			_float* pointer = (_float*) &temp;

			temp = _mm_sub_ps( *m1, *m2 );
			returnmatrix.m[0][0] = *pointer;
			returnmatrix.m[0][1] = *( pointer + 1 );
			returnmatrix.m[0][2] = *( pointer + 2 );
			returnmatrix.m[0][3] = *( pointer + 3 );
			
			temp = _mm_sub_ps( *( m1 + 1 ), *( m2 + 1 ) );
			returnmatrix.m[1][0] = *pointer;
			returnmatrix.m[1][1] = *( pointer + 1 );
			returnmatrix.m[1][2] = *( pointer + 2 );
			returnmatrix.m[1][3] = *( pointer + 3 );

			temp = _mm_sub_ps( *( m1 + 2 ), *( m2 + 2 ) );
			returnmatrix.m[2][0] = *pointer;
			returnmatrix.m[2][1] = *( pointer + 1 );
			returnmatrix.m[2][2] = *( pointer + 2 );
			returnmatrix.m[2][3] = *( pointer + 3 );

			temp = _mm_sub_ps( *( m1 + 3 ), *( m2 + 3 ) );
			returnmatrix.m[2][0] = *pointer;
			returnmatrix.m[2][1] = *( pointer + 1 );
			returnmatrix.m[2][2] = *( pointer + 2 );
			returnmatrix.m[2][3] = *( pointer + 3 );
		}
	}
	else
	{
		returnmatrix = *this;
		returnmatrix -= matrix;
	}

	return returnmatrix;
}

Matrix4 Matrix4::operator * ( const Matrix4& matrix ) const
{
	Matrix4 returnmatrix;
	_float4* m1 = (_float4*) m;
	_float4* m2 = (_float4*) matrix.m;
	
	if ( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		_float4 temp;
		_float4* start = (_float4*) returnmatrix.m;
		_float* pointer = (_float*) &temp;
		_bool align = ( (_dword)start ) % 16 == 0 ? _true : _false;

		_float4 _L1, _L2, _L3, _L4;
		_L1 = *( m2 );
		_L2 = *( m2 + 1 );
		_L3 = *( m2 + 2 );
		_L4 = *( m2 + 3 );

		temp = _mm_mul_ps( _mm_set_ps1( operator()( 0, 0 ) ), _L1 );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 1 ) ), _L2 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 2 ) ), _L3 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 3 ) ), _L4 ) );

		if ( align )
		{
			*start = temp;
		}
		else
		{
			returnmatrix.m[0][0] = *pointer;
			returnmatrix.m[0][1] = *( pointer + 1 );
			returnmatrix.m[0][2] = *( pointer + 2 );
			returnmatrix.m[0][3] = *( pointer + 3 );
		}

		temp = _mm_mul_ps( _mm_set_ps1( operator()( 1, 0 ) ), _L1 );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 1 ) ), _L2 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 2 ) ), _L3 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 3 ) ), _L4 ) );

		if ( align )
		{
			*( start + 1 ) = temp;
		}
		else
		{
			returnmatrix.m[1][0] = *pointer;
			returnmatrix.m[1][1] = *( pointer + 1 );
			returnmatrix.m[1][2] = *( pointer + 2 );
			returnmatrix.m[1][3] = *( pointer + 3 );
		}

		temp = _mm_mul_ps( _mm_set_ps1( operator()( 2, 0 ) ), _L1 );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 1 ) ), _L2 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 2 ) ), _L3 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 3 ) ), _L4 ) );

		if ( align )
		{
			*( start + 2 ) = temp;
		}
		else
		{
			returnmatrix.m[2][0] = *pointer;
			returnmatrix.m[2][1] = *( pointer + 1 );
			returnmatrix.m[2][2] = *( pointer + 2 );
			returnmatrix.m[2][3] = *( pointer + 3 );
		}

		temp = _mm_mul_ps( _mm_set_ps1( operator()( 3, 0 ) ), _L1 );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 3, 1 ) ), _L2 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 3, 2 ) ), _L3 ) );
		temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 3, 3 ) ), _L4 ) );

		if ( align )
		{
			*( start + 3 ) = temp;
		}
		else
		{
			returnmatrix.m[3][0] = *pointer;
			returnmatrix.m[3][1] = *( pointer + 1 );
			returnmatrix.m[3][2] = *( pointer + 2 );
			returnmatrix.m[3][3] = *( pointer + 3 );
		}
	}
	else
	{
		returnmatrix = *this;
		returnmatrix *= matrix;
	}

	return returnmatrix;
}

Matrix4 Matrix4::operator * ( _float scalar ) const
{
	Matrix4 returnmatrix;
	_float4* m1 = (_float4*) m;
	_float4* m2 = (_float4*) returnmatrix.m;
	
	if ( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		_float4 scalar4 = _mm_set_ps1( scalar );
		_bool align = ( (_dword)m2 ) % 16 == 0 ? _true : _false;

		if ( align )
		{
			*( m2 ) = _mm_mul_ps( *m1, scalar4 );
			*( m2 + 1 ) = _mm_mul_ps( *( m1 + 1 ), scalar4 );
			*( m2 + 2 ) = _mm_mul_ps( *( m1 + 2 ), scalar4 );
			*( m2 + 3 ) = _mm_mul_ps( *( m1 + 3 ), scalar4 );
		}
		else
		{
			_float4 temp;
			_float* pointer = (_float*) &temp;

			temp = _mm_mul_ps( *m1, scalar4 );
			returnmatrix.m[0][0] = *pointer;
			returnmatrix.m[0][1] = *( pointer + 1 );
			returnmatrix.m[0][2] = *( pointer + 2 );
			returnmatrix.m[0][3] = *( pointer + 3 );
			
			temp = _mm_mul_ps( *( m1 + 1 ), scalar4 );
			returnmatrix.m[1][0] = *pointer;
			returnmatrix.m[1][1] = *( pointer + 1 );
			returnmatrix.m[1][2] = *( pointer + 2 );
			returnmatrix.m[1][3] = *( pointer + 3 );

			temp = _mm_mul_ps( *( m1 + 2 ), scalar4 );
			returnmatrix.m[2][0] = *pointer;
			returnmatrix.m[2][1] = *( pointer + 1 );
			returnmatrix.m[2][2] = *( pointer + 2 );
			returnmatrix.m[2][3] = *( pointer + 3 );

			temp = _mm_mul_ps( *( m1 + 3 ), scalar4 );
			returnmatrix.m[2][0] = *pointer;
			returnmatrix.m[2][1] = *( pointer + 1 );
			returnmatrix.m[2][2] = *( pointer + 2 );
			returnmatrix.m[2][3] = *( pointer + 3 );
		}
	}
	else
	{
		returnmatrix = *this;
		returnmatrix *= scalar;
	}

	return returnmatrix;
}

Matrix4 Matrix4::operator / ( _float scalar ) const
{
	Matrix4 returnmatrix;
	_float4* m1 = (_float4*) m;
	_float4* m2 = (_float4*) returnmatrix.m;
	
	if ( ( (_dword) m1 ) % 16 == 0 && ( (_dword) m2 ) % 16 == 0 )
	{
		_float4 scalar4 = _mm_set_ps1( scalar );
		_bool align = ( (_dword)m2 ) % 16 == 0 ? _true : _false;

		if ( align )
		{
			*( m2 ) = _mm_div_ps( *m1, scalar4 );
			*( m2 + 1 ) = _mm_div_ps( *( m1 + 1 ), scalar4 );
			*( m2 + 2 ) = _mm_div_ps( *( m1 + 2 ), scalar4 );
			*( m2 + 3 ) = _mm_div_ps( *( m1 + 3 ), scalar4 );
		}
		else
		{
			_float4 temp;
			_float* pointer = (_float*) &temp;

			temp = _mm_div_ps( *m1, scalar4 );
			returnmatrix.m[0][0] = *pointer;
			returnmatrix.m[0][1] = *( pointer + 1 );
			returnmatrix.m[0][2] = *( pointer + 2 );
			returnmatrix.m[0][3] = *( pointer + 3 );
			
			temp = _mm_div_ps( *( m1 + 1 ), scalar4 );
			returnmatrix.m[1][0] = *pointer;
			returnmatrix.m[1][1] = *( pointer + 1 );
			returnmatrix.m[1][2] = *( pointer + 2 );
			returnmatrix.m[1][3] = *( pointer + 3 );

			temp = _mm_div_ps( *( m1 + 2 ), scalar4 );
			returnmatrix.m[2][0] = *pointer;
			returnmatrix.m[2][1] = *( pointer + 1 );
			returnmatrix.m[2][2] = *( pointer + 2 );
			returnmatrix.m[2][3] = *( pointer + 3 );

			temp = _mm_div_ps( *( m1 + 3 ), scalar4 );
			returnmatrix.m[2][0] = *pointer;
			returnmatrix.m[2][1] = *( pointer + 1 );
			returnmatrix.m[2][2] = *( pointer + 2 );
			returnmatrix.m[2][3] = *( pointer + 3 );
		}
	}
	else
	{
		returnmatrix = *this;
		returnmatrix *= scalar;
	}

	return returnmatrix;
}

_bool Matrix4::operator == ( const Matrix4& matrix ) const
{
	for ( _dword i = 0; i < 4; i ++ )
	{
		for ( _dword j = 0; j < 4; j ++ )
		{
			if ( m[i][j] != matrix.m[i][j] )
				return _false;
		}
	}

	return _true;
}

_bool Matrix4::operator != ( const Matrix4& matrix ) const
{
	return ( operator == ( matrix ) ) == _false;
}

_bool Matrix4::Equal( const Matrix4& matrix, _float margin ) const
{
	return	GetRow( 0 ).Equal( matrix.GetRow( 0 ), margin ) && 
		GetRow( 1 ).Equal( matrix.GetRow( 1 ), margin ) && 
		GetRow( 2 ).Equal( matrix.GetRow( 2 ), margin ) && 
		GetRow( 3 ).Equal( matrix.GetRow( 3 ), margin );
}

_bool Matrix4::OnlyTranslation( ) const
{
	return m[0][0] == 1.0f && m[0][1] == 0.0f && m[0][2] == 0.0f &&
		   m[1][0] == 0.0f && m[1][1] == 1.0f && m[1][2] == 0.0f &&
		   m[2][0] == 0.0f && m[2][1] == 0.0f && m[2][2] == 1.0f;
}

_float Matrix4::Determinant( ) const
{
	if ( ( (_dword) m ) % 16 == 0 )
	{
		_float4 Va, Vb, Vc;
		_float4 r1, r2, r3, t1, t2, sum;
		_float4 Det;

		_float4* start = (_float4*) m;

		// First, Let's calculate the first four minterms of the first line
		t1 = *( start + 3 );
		t2 = _mm_ror_ps( *( start + 2 ), 1 ); 
		Vc = _mm_mul_ps( t2, _mm_ror_ps( t1, 0 ) );
		Va = _mm_mul_ps( t2, _mm_ror_ps( t1, 2 ) );
		Vb = _mm_mul_ps( t2, _mm_ror_ps( t1, 3 ) );

		r1 = _mm_sub_ps( _mm_ror_ps( Va, 1 ), _mm_ror_ps( Vc, 2 ) );
		r2 = _mm_sub_ps( _mm_ror_ps( Vb, 2 ), _mm_ror_ps( Vb, 0 ) );
		r3 = _mm_sub_ps( _mm_ror_ps( Va, 0 ), _mm_ror_ps( Vc, 1 ) );

		Va = _mm_ror_ps( *( start + 1 ), 1 );
		sum = _mm_mul_ps( Va, r1 );
		Vb = _mm_ror_ps( Va, 1 );
		sum = _mm_add_ps( sum, _mm_mul_ps( Vb, r2 ) );
		Vc = _mm_ror_ps( Vb, 1 );
		sum = _mm_add_ps( sum, _mm_mul_ps( Vc, r3 ) );

		// Now we can calculate the determinant:
		Det = _mm_mul_ps( sum, *start );
		Det = _mm_add_ps( Det, _mm_movehl_ps( Det, Det ) );
		Det = _mm_sub_ss( Det, _mm_shuffle_ps( Det, Det, 1 ) );

		return *( (_float*) &Det );
	}
	else
	{
		return	m[0][0] * Matrix3( m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3] ).Determinant( )
			-	m[0][1] * Matrix3( m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3] ).Determinant( )
			+	m[0][2] * Matrix3( m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3] ).Determinant( )
			-	m[0][3] * Matrix3( m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2] ).Determinant( );
	}
}

Matrix4& Matrix4::Identity( )
{
	if ( ( (_dword) m ) % 16 == 0 )
	{
		_float4 zerone = _mm_set_ps( 1.0f, 0.0f, 0.0f, 1.0f );

		_mm_storel_pi( (_float2*) &operator( )( 0, 0 ), zerone );
		_mm_storel_pi( (_float2*) &operator( )( 0, 2 ), _mm_setzero_ps() );
		_mm_storeh_pi( (_float2*) &operator( )( 1, 0 ), zerone );
		_mm_storel_pi( (_float2*) &operator( )( 1, 2 ), _mm_setzero_ps() );
		_mm_storel_pi( (_float2*) &operator( )( 2, 0 ), _mm_setzero_ps() );
		_mm_storel_pi( (_float2*) &operator( )( 2, 2 ), zerone );
		_mm_storel_pi( (_float2*) &operator( )( 3, 0 ), _mm_setzero_ps() );
		_mm_storeh_pi( (_float2*) &operator( )( 3, 2 ), zerone );
	}
	else
	{
		m[0][0]	= 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}

	return *this;
}

Matrix4& Matrix4::Transpose( )
{
	if ( ( (_dword) m ) % 16 == 0 )
	{
		_float4 *_L1, *_L2, *_L3, *_L4;
		_L1 = (_float4*) m;
		_L2 = _L1 + 1;
		_L3 = _L2 + 1;
		_L4 = _L3 + 1;

		_float4	xmm0 = _mm_unpacklo_ps( *_L1, *_L2 );
		_float4 xmm1 = _mm_unpacklo_ps( *_L3, *_L4 );
		_float4 xmm2 = _mm_unpackhi_ps( *_L1, *_L2 );
		_float4 xmm3 = _mm_unpackhi_ps( *_L3, *_L4 );

		*_L1 = _mm_movelh_ps( xmm0, xmm1 );
		*_L2 = _mm_movehl_ps( xmm1, xmm0 );
		*_L3 = _mm_movelh_ps( xmm2, xmm3 );
		*_L4 = _mm_movehl_ps( xmm3, xmm2 );
	}
	else
	{
		_float m01 = m[0][1], m02 = m[0][2], m03 = m[0][3], m10 = m[1][0], m12 = m[1][2], m13 = m[1][3],
			m20 = m[2][0], m21 = m[2][1], m23 = m[2][3], m30 = m[3][0], m31 = m[3][1], m32 = m[3][2];

		m[0][1] = m10; m[0][2] = m20; m[0][3] = m30;
		m[1][0] = m01; m[1][2] = m21; m[1][3] = m31;
		m[2][0] = m02; m[2][1] = m12; m[2][3] = m32;
		m[3][0] = m03; m[3][1] = m13; m[3][2] = m23;
	}

	return *this;
}

Matrix4& Matrix4::Inverse( )
{
	_float determinant = Determinant( );

	if ( determinant != 0.0f )
	{
		Adjoint( );

		if ( ( (_dword) m ) % 16 == 0 )
		{
			_float4 *start = (_float4*) m;
			_float4 temp = _mm_set_ps1( determinant );

			*start = _mm_div_ps( *start, temp );
			*( start + 1 ) = _mm_div_ps( *( start + 1 ), temp );
			*( start + 2 ) = _mm_div_ps( *( start + 2 ), temp );
			*( start + 3 ) = _mm_div_ps( *( start + 3 ), temp );
		}
		else
		{
			m[0][0] /= determinant; m[0][1] /= determinant; m[0][2] /= determinant; m[0][3] /= determinant;
			m[1][0] /= determinant; m[1][1] /= determinant; m[1][2] /= determinant; m[1][3] /= determinant;
			m[2][0] /= determinant; m[2][1] /= determinant; m[2][2] /= determinant; m[2][3] /= determinant;
			m[3][0] /= determinant; m[3][1] /= determinant; m[3][2] /= determinant; m[3][3] /= determinant;
		}
	}

	return *this;
}

Matrix4& Matrix4::Adjoint( )
{
	_float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3], m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3],
		m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3], m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

	m[0][0] =   Matrix3( m11, m12, m13, m21, m22, m23, m31, m32, m33 ).Determinant( );
	m[1][0] = - Matrix3( m10, m12, m13, m20, m22, m23, m30, m32, m33 ).Determinant( );
	m[2][0] =   Matrix3( m10, m11, m13, m20, m21, m23, m30, m31, m33 ).Determinant( );
	m[3][0] = - Matrix3( m10, m11, m12, m20, m21, m22, m30, m31, m32 ).Determinant( );

	m[0][1] = - Matrix3( m01, m02, m03, m21, m22, m23, m31, m32, m33 ).Determinant( );
	m[1][1] =   Matrix3( m00, m02, m03, m20, m22, m23, m30, m32, m33 ).Determinant( );
	m[2][1] = - Matrix3( m00, m01, m03, m20, m21, m23, m30, m31, m33 ).Determinant( );
	m[3][1] =   Matrix3( m00, m01, m02, m20, m21, m22, m30, m31, m32 ).Determinant( );

	m[0][2] =	Matrix3( m01, m02, m03, m11, m12, m13, m31, m32, m33 ).Determinant( );
	m[1][2] = - Matrix3( m00, m02, m03, m10, m12, m13, m30, m32, m33 ).Determinant( );
	m[2][2] =   Matrix3( m00, m01, m03, m10, m11, m13, m30, m31, m33 ).Determinant( );
	m[3][2] = - Matrix3( m00, m01, m02, m10, m11, m12, m30, m31, m32 ).Determinant( );

	m[0][3] = - Matrix3( m01, m02, m03, m11, m12, m13, m21, m22, m23 ).Determinant( );
	m[1][3] =   Matrix3( m00, m02, m03, m10, m12, m13, m20, m22, m23 ).Determinant( );
	m[2][3] = - Matrix3( m00, m01, m03, m10, m11, m13, m20, m21, m23 ).Determinant( );
	m[3][3] =   Matrix3( m00, m01, m02, m10, m11, m12, m20, m21, m22 ).Determinant( );

	return *this;
}

Matrix4& Matrix4::Translation( _float x, _float y, _float z )
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] =    x; m[3][1] =    y; m[3][2] =    z; m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::Translation( const Vector3& vector )
{
	return Translation( vector.x, vector.y, vector.z );
}

Matrix4& Matrix4::Scaling( _float x, _float y, _float z )
{
	m[0][0]	=    x; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] =    y; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] =    z; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::Scaling( const Vector3& vector )
{
	return Scaling( vector.x, vector.y, vector.z );
}

Matrix4& Matrix4::Scaling( const Vector3& vector, _float factor )
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

	m[0][0]	= 1.0f + k * xx; m[0][1] =        k * xy; m[0][2] =        k * zx; m[0][3] = 0.0f;
	m[1][0]	=        k * xy; m[1][1] = 1.0f + k * yy; m[1][2] =        k * yz; m[1][3] = 0.0f;
    m[2][0]	=        k * zx; m[2][1] =        k * yz; m[2][2] = 1.0f + k * zz; m[2][3] = 0.0f;
	m[3][0] =          0.0f; m[3][1] =          0.0f; m[3][2] =          0.0f; m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::RotationX( _float radian )
{
	_float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

	m[0][0]	= 1.0f; m[0][1] =       0.0f; m[0][2] =     0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] =   cosvalue; m[1][2] = sinvalue; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = - sinvalue; m[2][2] = cosvalue; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] =       0.0f; m[3][2] =     0.0f; m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::RotationY( _float radian )
{
	_float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

	m[0][0]	= cosvalue; m[0][1] = 0.0f; m[0][2] = - sinvalue; m[0][3] = 0.0f;
	m[1][0] =     0.0f; m[1][1] = 1.0f; m[1][2] =       0.0f; m[1][3] = 0.0f;
	m[2][0] = sinvalue; m[2][1] = 0.0f; m[2][2] =   cosvalue; m[2][3] = 0.0f;
	m[3][0] =     0.0f; m[3][1] = 0.0f; m[3][2] =       0.0f; m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::RotationZ( _float radian )
{
	_float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian );

	m[0][0]	=   cosvalue; m[0][1] = sinvalue; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = - sinvalue; m[1][1] = cosvalue; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] =       0.0f; m[2][1] =     0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] =       0.0f; m[3][1] =     0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::Rotation( const Vector3& axis, _float radian )
{
	_float sinvalue = Math::Sin( radian ), cosvalue = Math::Cos( radian ), cosreverse = 1.0f - cosvalue;

	Vector3 axisnormal( axis );
	axisnormal.Normalize( );

	m[0][0] = cosreverse * axisnormal.x * axisnormal.x + cosvalue;
	m[0][1] = cosreverse * axisnormal.x * axisnormal.y + sinvalue * axisnormal.z;
	m[0][2] = cosreverse * axisnormal.x * axisnormal.z - sinvalue * axisnormal.y;
	m[0][3] = 0.0f;

	m[1][0] = cosreverse * axisnormal.x * axisnormal.y - sinvalue * axisnormal.z;
	m[1][1] = cosreverse * axisnormal.y * axisnormal.y + cosvalue;
	m[1][2] = cosreverse * axisnormal.y * axisnormal.z + sinvalue * axisnormal.x;
	m[1][3] = 0.0f;

	m[2][0] = cosreverse * axisnormal.x * axisnormal.z + sinvalue * axisnormal.y;
	m[2][1] = cosreverse * axisnormal.y * axisnormal.z - sinvalue * axisnormal.x;
	m[2][2] = cosreverse * axisnormal.z * axisnormal.z + cosvalue;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::Rotation( const Vector3& srcvector, const Vector3& desvector )
{
	Quaternion rotation;
	rotation.Rotation( srcvector, desvector );

	FromTranslationRotation( Vector3::cOrigin, rotation );

	return *this;
}

Matrix4& Matrix4::Rotation( const Quaternion& rotation )
{
	float xx = rotation.x * rotation.x * 2.0f, yy = rotation.y * rotation.y * 2.0f, zz = rotation.z * rotation.z * 2.0f;
	float xy = rotation.x * rotation.y * 2.0f, zw = rotation.z * rotation.w * 2.0f, xz = rotation.x * rotation.z * 2.0f;
	float yw = rotation.y * rotation.w * 2.0f, yz = rotation.y * rotation.z * 2.0f, xw = rotation.x * rotation.w * 2.0f;

	m[0][0] = 1.0f - yy - zz; m[0][1] =        xy + zw; m[0][2] =        xz - yw; m[0][3] = 0.0f;
	m[1][0] =        xy - zw; m[1][1] = 1.0f - xx - zz; m[1][2] =        yz + xw; m[1][3] = 0.0f;
	m[2][0] =        xz + yw; m[2][1] =        yz - xw; m[2][2] = 1.0f - xx - yy; m[2][3] = 0.0f;
	m[3][0] =           0.0f; m[3][1] =           0.0f; m[3][2] =           0.0f; m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::Rotation( const EulerAngles& eulerangles )
{
	*this = Matrix4( ).RotationX( eulerangles.mPitch ) * Matrix4( ).RotationY( eulerangles.mHeading ) * Matrix4( ).RotationZ( eulerangles.mBank );

	return *this;
}

Matrix4& Matrix4::ProjectXY( )
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 0.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::ProjectYZ( )
{
	m[0][0] = 0.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::ProjectZX( )
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 0.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::Project( const Vector3& vector )
{
	return Scaling( vector, 0.0f );
}

Matrix4& Matrix4::RefectionXY( )
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] =  0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] =  0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = -1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] =  0.0f; m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::RefectionYZ( )
{
	m[0][0] = -1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] =  0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] =  0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] =  0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::RefectionZX( )
{
	m[0][0] = 1.0f; m[0][1] =  0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = -1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] =  0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] =  0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

	return *this;
}

Matrix4& Matrix4::Refection( const Vector3& vector )
{
	return Scaling( vector, -1.0f );
}

Matrix4& Matrix4::FromTranslationRotation( const Vector3& translation, const Matrix3& rotation )
{
	m[0][0] = rotation.m[0][0]; m[0][1] = rotation.m[0][1]; m[0][2] = rotation.m[0][2]; m[0][3] = 0.0f;
	m[1][0] = rotation.m[1][0]; m[1][1] = rotation.m[1][1]; m[1][2] = rotation.m[1][2]; m[1][3] = 0.0f;
	m[2][0] = rotation.m[2][0]; m[2][1] = rotation.m[2][1]; m[2][2] = rotation.m[2][2]; m[2][3] = 0.0f;
	m[3][0] =    translation.x; m[3][1] =    translation.y; m[3][2] =    translation.z; m[3][3] = 1.0f;

	return *this;
}

_void Matrix4::ToTranslationRotation( Vector3& translation, Matrix3& rotation ) const
{
	translation.x = m[3][0];
	translation.y = m[3][1];
	translation.z = m[3][2];

	rotation.m[0][0] = m[0][0]; rotation.m[0][1] = m[0][1]; rotation.m[0][2] = m[0][2];
	rotation.m[1][0] = m[1][0]; rotation.m[1][1] = m[1][1]; rotation.m[1][2] = m[1][2];
	rotation.m[2][0] = m[2][0]; rotation.m[2][1] = m[2][1]; rotation.m[2][2] = m[2][2];
}

Matrix4& Matrix4::FromTranslationRotation( const Vector3& translation, const Quaternion& rotation )
{
	_float xx = rotation.x * rotation.x * 2.0f, yy = rotation.y * rotation.y * 2.0f, zz = rotation.z * rotation.z * 2.0f;
	_float xy = rotation.x * rotation.y * 2.0f, zw = rotation.z * rotation.w * 2.0f, xz = rotation.x * rotation.z * 2.0f;
	_float yw = rotation.y * rotation.w * 2.0f, yz = rotation.y * rotation.z * 2.0f, xw = rotation.x * rotation.w * 2.0f;

	m[0][0] = 1.0f - yy - zz; m[0][1] =        xy + zw; m[0][2] =        xz - yw; m[0][3] = 0.0f;
	m[1][0] =        xy - zw; m[1][1] = 1.0f - xx - zz; m[1][2] =        yz + xw; m[1][3] = 0.0f;
	m[2][0] =        xz + yw; m[2][1] =        yz - xw; m[2][2] = 1.0f - xx - yy; m[2][3] = 0.0f;
	m[3][0] =  translation.x; m[3][1] =  translation.y; m[3][2] =  translation.z; m[3][3] = 1.0f;

	return *this;
}

_void Matrix4::ToTranslationRotation( Vector3& translation, Quaternion& rotation ) const
{
	translation.x = m[3][0];
	translation.y = m[3][1];
	translation.z = m[3][2];

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

Matrix4 Matrix4::CreateOrthoLH( _float width, _float height, _float znear, _float zfar )
{
	_float xs = 2.0f / width;
	_float ys = 2.0f / height;
	_float zf = 1.0f / ( zfar - znear );
	_float zn = - znear * zf;

	return Matrix4(
		  xs, 0.0f, 0.0f, 0.0f,
		0.0f,   ys, 0.0f, 0.0f,
		0.0f, 0.0f,   zf, 0.0f,
		0.0f, 0.0f,   zn, 1.0f );
}

Matrix4 Matrix4::CreateOrthoOffCenterLH( _float left, _float right, _float bottom, _float top, _float znear, _float zfar )
{
	_float xs1 = 2.0f / ( right - left );
	_float xs2 = ( left + right ) / ( left - right );
	_float ys1 = 2.0f / ( top - bottom );
	_float ys2 = ( bottom + top ) / ( bottom - top );
	_float zf  = 1.0f / ( zfar - znear );
	_float zn  = - znear * zf;

	return Matrix4(
		 xs1, 0.0f, 0.0f, 0.0f,
		0.0f,  ys1, 0.0f, 0.0f,
		0.0f, 0.0f,   zf, 0.0f,
		 xs2,  ys2,   zn, 1.0f );
}

Matrix4 Matrix4::CreateOrthoRH( _float width, _float height, _float znear, _float zfar )
{
	_float xs = 2.0f / width;
	_float ys = 2.0f / height;
	_float zf = 1.0f / ( znear - zfar );
	_float zn = znear * zf;

	return Matrix4(
		  xs, 0.0f, 0.0f, 0.0f,
		0.0f,   ys, 0.0f, 0.0f,
		0.0f, 0.0f,   zf, 0.0f,
		0.0f, 0.0f,   zn, 1.0f );
}

Matrix4 Matrix4::CreateOrthoOffCenterRH( _float left, _float right, _float bottom, _float top, _float znear, _float zfar )
{
	_float xs1 = 2.0f / ( right - left );
	_float xs2 = ( left + right ) / ( left - right );
	_float ys1 = 2.0f / ( top - bottom );
	_float ys2 = ( bottom + top ) / ( bottom - top );
	_float zf  = 1.0f / ( znear - zfar );
	_float zn  = znear * zf;

	return Matrix4(
		 xs1, 0.0f, 0.0f, 0.0f,
		0.0f,  ys1, 0.0f, 0.0f,
		0.0f, 0.0f,   zf, 0.0f,
		 xs2,  ys2,   zn, 1.0f );
}

Matrix4 Matrix4::CreatePerspectiveLH( _float width, _float height, _float znear, _float zfar )
{
	_float xs = 2.0f * znear / width;
	_float ys = 2.0f * znear / height;
	_float zf = zfar / ( zfar - znear );
	_float zn = - znear * zf;

	return Matrix4(
		  xs, 0.0f, 0.0f, 0.0f,
		0.0f,   ys, 0.0f, 0.0f,
		0.0f, 0.0f,   zf, 1.0f,
		0.0f, 0.0f,   zn, 0.0f );
}

Matrix4 Matrix4::CreatePerspectiveFovLH( _float fovy, _float aspect, _float znear, _float zfar )
{
	_float ys = Math::Cot( fovy / 2.0f );
	_float xs = ys / aspect;
	_float zf = zfar / ( zfar - znear );
	_float zn = - znear * zf;

	return Matrix4(
		  xs, 0.0f, 0.0f, 0.0f,
		0.0f,   ys, 0.0f, 0.0f,
		0.0f, 0.0f,   zf, 1.0f,
		0.0f, 0.0f,   zn, 0.0f );
}

Matrix4 Matrix4::CreatePerspectiveOffCenterLH( _float left, _float right, _float bottom, _float top, _float znear, _float zfar )
{
	_float xs1 = 2.0f * znear / ( right - left );
	_float xs2 = ( left + right ) / ( left - right );
	_float ys1 = 2.0f * znear / ( top - bottom );
	_float ys2 = ( bottom + top ) / ( bottom - top );
	_float zf  = zfar / ( zfar - znear );
	_float zn  = - znear * zf;

	return Matrix4(
		 xs1, 0.0f, 0.0f, 0.0f,
		0.0f,  ys1, 0.0f, 0.0f,
		 xs2,  ys2,   zf, 1.0f,
		0.0f, 0.0f,   zn, 0.0f );
}

Matrix4 Matrix4::CreatePerspectiveRH( _float width, _float height, _float znear, _float zfar )
{
	_float xs = 2.0f * znear / width;
	_float ys = 2.0f * znear / height;
	_float zf = zfar / ( znear - zfar );
	_float zn = znear * zf;

	return Matrix4(
		  xs, 0.0f, 0.0f,  0.0f,
		0.0f,   ys, 0.0f,  0.0f,
		0.0f, 0.0f,   zf, -1.0f,
		0.0f, 0.0f,   zn,  0.0f );
}

Matrix4 Matrix4::CreatePerspectiveFovRH( _float fovy, _float aspect, _float znear, _float zfar )
{
	_float ys = Math::Cot( fovy / 2.0f );
	_float xs = ys / aspect;
	_float zf = zfar / ( znear - zfar );
	_float zn = znear * zf;

	return Matrix4(
		  xs, 0.0f, 0.0f,  0.0f,
		0.0f,   ys, 0.0f,  0.0f,
		0.0f, 0.0f,   zf, -1.0f,
		0.0f, 0.0f,   zn,  0.0f );
}

Matrix4 Matrix4::CreatePerspectiveOffCenterRH( _float left, _float right, _float bottom, _float top, _float znear, _float zfar )
{
	_float xs1 = 2.0f * znear / ( right - left );
	_float xs2 = ( left + right ) / ( right - left );
	_float ys1 = 2.0f * znear / ( top - bottom );
	_float ys2 = ( bottom + top ) / ( top - bottom );
	_float zf  = zfar / ( znear - zfar );
	_float zn  = znear * zf;

	return Matrix4(
		 xs1, 0.0f, 0.0f,  0.0f,
		0.0f,  ys1, 0.0f,  0.0f,
		 xs2,  ys2,   zf, -1.0f,
		0.0f, 0.0f,   zn,  0.0f );
}

Matrix4 Matrix4::CreateLookAtLH( const Vector3& eye, const Vector3& lookat, const Vector3& upaxis )
{
	Vector3 zaxis = ( lookat - eye ).Normalize( );
	Vector3 xaxis = Vector3::Cross( upaxis, zaxis ).Normalize( );
	Vector3 yaxis = Vector3::Cross( zaxis, xaxis );

	_float xeye = - Vector3::Dot( xaxis, eye );
	_float yeye = - Vector3::Dot( yaxis, eye );
	_float zeye = - Vector3::Dot( zaxis, eye );

	return Matrix4(
		xaxis.x, yaxis.x, zaxis.x, 0.0f,
		xaxis.y, yaxis.y, zaxis.y, 0.0f,
		xaxis.z, yaxis.z, zaxis.z, 0.0f,
		   xeye,    yeye,    zeye, 1.0f );
}

Matrix4 Matrix4::CreateLookAtRH( const Vector3& eye, const Vector3& lookat, const Vector3& upaxis )
{
	Vector3 zaxis = ( eye - lookat ).Normalize( );
	Vector3 xaxis = Vector3::Cross( upaxis, zaxis ).Normalize( );
	Vector3 yaxis = Vector3::Cross( zaxis, xaxis );

	_float xeye = - Vector3::Dot( xaxis, eye );
	_float yeye = - Vector3::Dot( yaxis, eye );
	_float zeye = - Vector3::Dot( zaxis, eye );

	return Matrix4(
		xaxis.x, yaxis.x, zaxis.x, 0.0f,
		xaxis.y, yaxis.y, zaxis.y, 0.0f,
		xaxis.z, yaxis.z, zaxis.z, 0.0f,
		   xeye,    yeye,    zeye, 1.0f );
}