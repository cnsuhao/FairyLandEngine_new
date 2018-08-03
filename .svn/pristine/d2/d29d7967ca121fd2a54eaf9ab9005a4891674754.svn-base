//============================================================================
// Matrix3SSE.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Matrix3SSE Implementation
//----------------------------------------------------------------------------

const Matrix3SSE Matrix3SSE::cZero( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
const Matrix3SSE Matrix3SSE::cIdentity( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f );

Matrix3SSE::Matrix3SSE( _float m00, _float m01, _float m02, 
	_float m10, _float m11, _float m12, _float m20, _float m21, _float m22 )
{
	start = ( _float4* ) ( ( _byte* )m + 16 - ( _dword ) m % 16 );

	*start = _mm_set_ps( 0.0f, m02, m01, m00 );
	*( start + 1 ) = _mm_set_ps( 0.0f, m12, m11, m10 );
	*( start + 2 ) = _mm_set_ps( 0.0f, m22, m21, m20 );
}

Matrix3SSE::Matrix3SSE( const Matrix3 &matrix )
{
	start = ( _float4* ) ( ( _byte* )m + 16 - ( _dword ) m % 16 );

	*start = _mm_set_ps( 0.0f, matrix( 0, 2 ), matrix( 0, 1 ), matrix( 0, 0 ) );
	*( start + 1 ) = _mm_set_ps( 0.0f, matrix( 1, 2 ), matrix( 1, 1 ), matrix( 1, 0 ) );
	*( start + 2 ) = _mm_set_ps( 0.0f, matrix( 2, 2 ), matrix( 2, 1 ), matrix( 2, 0 ) );
}

Matrix3SSE& Matrix3SSE::operator += ( const Matrix3SSE& matrix )
{
	*( start ) = _mm_add_ps( *start, *matrix.start );
	*( start + 1 ) = _mm_add_ps( *( start + 1 ), *( matrix.start + 1 ) );
	*( start + 2 ) = _mm_add_ps( *( start + 2 ), *( matrix.start + 2 ) );

	return *this;
}

Matrix3SSE& Matrix3SSE::operator -= ( const Matrix3SSE& matrix )
{
	*( start ) = _mm_sub_ps( *start, *matrix.start );
	*( start + 1 ) = _mm_sub_ps( *( start + 1 ), *( matrix.start + 1 ) );
	*( start + 2 ) = _mm_sub_ps( *( start + 2 ), *( matrix.start + 2 ) );

	return *this;
}

Matrix3SSE& Matrix3SSE::operator *= ( const Matrix3SSE& matrix )
{
	_float4 temp;

	_float4 _L1, _L2, _L3;
	_L1 = *( matrix.start );
	_L2 = *( matrix.start + 1 );
	_L3 = *( matrix.start + 2 );

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 0, 0 ) ), _L1 );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 1 ) ), _L2 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 2 ) ), _L3 ) );
	*( start ) = temp;

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 1, 0 ) ), _L1 );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 1 ) ), _L2 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 2 ) ), _L3 ) );
	*( start + 1 ) = temp;

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 2, 0 ) ), _L1 );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 1 ) ), _L2 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 2 ) ), _L3 ) );
	*( start + 2 ) = temp;

	return *this;
}

Matrix3SSE& Matrix3SSE::operator *= ( _float scalar )
{
	_float4 temp = _mm_set_ps1( scalar );

	*( start ) = _mm_mul_ps( *( start ), temp );
	*( start + 1 ) = _mm_mul_ps( *( start + 1 ), temp );
	*( start + 2 ) = _mm_mul_ps( *( start + 2 ), temp );

	return *this;
}

Matrix3SSE& Matrix3SSE::operator /= ( _float scalar )
{
	_float4 temp = _mm_set_ps1( scalar );

	*( start ) = _mm_div_ps( *( start ), temp );
	*( start + 1 ) = _mm_div_ps( *( start + 1 ), temp );
	*( start + 2 ) = _mm_div_ps( *( start + 2 ), temp );

	return *this;
}

Matrix3SSE Matrix3SSE::operator + ( const Matrix3SSE& matrix ) const
{
	Matrix3SSE result;

	*( result.start ) = _mm_add_ps( *start, *matrix.start );
	*( result.start + 1 ) = _mm_add_ps( *( start + 1 ), *( matrix.start + 1 ) );
	*( result.start + 2 ) = _mm_add_ps( *( start + 2 ), *( matrix.start + 2 ) );

	return result;
}

Matrix3SSE Matrix3SSE::operator - ( const Matrix3SSE& matrix ) const
{
	Matrix3SSE result;

	*( result.start ) = _mm_sub_ps( *start, *matrix.start );
	*( result.start + 1 ) = _mm_sub_ps( *( start + 1 ), *( matrix.start + 1 ) );
	*( result.start + 2 ) = _mm_sub_ps( *( start + 2 ), *( matrix.start + 2 ) );

	return result;
}

Matrix3SSE Matrix3SSE::operator * ( const Matrix3SSE& matrix ) const
{
	Matrix3SSE result;
	_float4 temp;

	_float4 _L1, _L2, _L3;
	_L1 = *( matrix.start );
	_L2 = *( matrix.start + 1 );
	_L3 = *( matrix.start + 2 );

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 0, 0 ) ), _L1 );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 1 ) ), _L2 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 2 ) ), _L3 ) );
	*( result.start ) = temp;

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 1, 0 ) ), _L1 );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 1 ) ), _L2 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 2 ) ), _L3 ) );
	*( result.start + 1 ) = temp;

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 2, 0 ) ), _L1 );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 1 ) ), _L2 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 2 ) ), _L3 ) );
	*( result.start + 2 ) = temp;

	return result;
}

Matrix3SSE Matrix3SSE::operator * ( _float scalar ) const
{
	Matrix3SSE result;
	_float4 temp = _mm_set_ps1( scalar );

	*( result.start ) = _mm_mul_ps( *( start ), temp );
	*( result.start + 1 ) = _mm_mul_ps( *( start + 1 ), temp );
	*( result.start + 2 ) = _mm_mul_ps( *( start + 2 ), temp );

	return result;
}

Vector3SSE Matrix3SSE::operator * ( const Vector3SSE& vector ) const
{
	Vector3SSE result;

	*result.start = _mm_mul_ps( *( start ), *vector.start );
	*result.start = _mm_add_ps( *result.start, _mm_mul_ps( *( start + 1 ), *vector.start ) );
	*result.start = _mm_add_ps( *result.start, _mm_mul_ps( *( start + 2 ), *vector.start ) );

	return result;
}

Matrix3SSE Matrix3SSE::operator / ( _float scalar ) const
{
	Matrix3SSE result;
	_float4 temp = _mm_set_ps1( scalar );

	*( result.start ) = _mm_div_ps( *( start ), temp );
	*( result.start + 1 ) = _mm_div_ps( *( start + 1 ), temp );
	*( result.start + 2 ) = _mm_div_ps( *( start + 2 ), temp );

	return result;
}

_bool Matrix3SSE::operator == ( const Matrix3SSE& matrix ) const
{
	for ( _dword i = 0; i < 3; i ++ )
	{
		for ( _dword j = 0; j < 3; j ++ )
		{
			if ( operator()( i, j ) != matrix( i, j ) )
				return _false;
		}
	}

	return _true;
}

_bool Matrix3SSE::operator != ( const Matrix3SSE& matrix ) const
{
	return !( operator == ( matrix ) );
}

_bool Matrix3SSE::Equal( const Matrix3SSE& matrix, _float margin ) const
{
	_float4 temp;
	_float* ptr;

	for( _dword i = 0; i < 3; i++ )
	{
		temp = _mm_sub_ps( *( start + i ), *( matrix.start + i ) );

		ptr = ( _float* )&temp;

		for( _dword j = 0; j < 3; j++ )
		{
			if( Math::Abs( *ptr++ ) > margin )
			{
				return _false;
			}
		}
	}

	return _true;
}

Matrix3SSE& Matrix3SSE::FromRotation( const Quaternion& rotation )
{
	_float xx = rotation.x * rotation.x * 2.0f, yy = rotation.y * rotation.y * 2.0f, zz = rotation.z * rotation.z * 2.0f;
	_float xy = rotation.x * rotation.y * 2.0f, zw = rotation.z * rotation.w * 2.0f, xz = rotation.x * rotation.z * 2.0f;
	_float yw = rotation.y * rotation.w * 2.0f, yz = rotation.y * rotation.z * 2.0f, xw = rotation.x * rotation.w * 2.0f;

	*start = _mm_set_ps( 0.0f, xz - yw, xy + zw, 1.0f - yy - zz );
	*( start + 1 ) = _mm_set_ps( 0.0f, yz + xw, 1.0f - xx - zz, xy - zw );
	*( start + 2 ) = _mm_set_ps( 0.0f, 1.0f - xx - yy, yz - xw, xz + yw );

	return *this;
}


Matrix3SSE::operator Matrix3 ( ) const
{
	return Matrix3( operator( )( 0, 0 ), operator( )( 0, 1 ), operator( )( 0, 2 ), 
		operator( )( 1, 0 ), operator( )( 1, 1 ), operator( )( 1, 2 ), 
		operator( )( 2, 0 ), operator( )( 2, 1 ), operator( )( 2, 2 ) );
}
