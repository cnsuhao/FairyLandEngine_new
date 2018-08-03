//============================================================================
// Matrix4SSE.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Matrix4SSE Implementation
//----------------------------------------------------------------------------

#define _mm_ror_ps( vec, i )	\
	( ( ( i ) % 4 ) ? ( _mm_shuffle_ps ( vec, vec,	\
	_MM_SHUFFLE( ( _byte )( i + 3 ) % 4, ( _byte )( i + 2 ) % 4, ( _byte )( i + 1 ) % 4, ( _byte ) ( i + 0 ) % 4 ) ) ) : ( vec ) )

const Matrix4SSE Matrix4SSE::cZero( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
const Matrix4SSE Matrix4SSE::cIdentity( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );

Matrix4SSE::Matrix4SSE( _float m00, _float m01, _float m02, _float m03, _float m10, _float m11, _float m12, _float m13,
	_float m20, _float m21, _float m22, _float m23, _float m30, _float m31, _float m32, _float m33 )
{
	start = (_float4*) ( (_byte*) m + 16 - (_dword) m % 16 );

	*start = _mm_set_ps( m03, m02, m01, m00 );
	*( start + 1 ) = _mm_set_ps( m13, m12, m11, m10 );
	*( start + 2 ) = _mm_set_ps( m23, m22, m21, m20 );
	*( start + 3 ) = _mm_set_ps( m33, m32, m31, m30 );
}

Matrix4SSE::Matrix4SSE( const _float* buffer, _dword num )
{
	FL_ASSERT( num <= 4 );

	start = (_float4*) ( (_byte*)m  + 16 - (_dword) m % 16 );

	for( _dword i = 0; i < num; i ++ )
	{
		*( start + i ) = _mm_set_ps( 0.0f, buffer[2], buffer[1], buffer[0] );
		buffer = buffer + 3;
	}
}

Matrix4SSE::Matrix4SSE( const Matrix3 &matrix )
{
	start = (_float4*) ( (_byte*) m + 16 - (_dword) m % 16 );

	*start = _mm_set_ps( 0.0f, matrix( 0, 2 ), matrix( 0, 1 ), matrix( 0, 0 ) );
	*( start + 1 ) = _mm_set_ps( 0.0f, matrix( 1, 2 ), matrix( 1, 1 ), matrix( 1, 0 ) );
	*( start + 2 ) = _mm_set_ps( 0.0f, matrix( 2, 2 ), matrix( 2, 1 ), matrix( 2, 0 ) );
	*( start + 3 ) = _mm_setzero_ps();
}

Matrix4SSE& Matrix4SSE::operator += ( const Matrix4SSE& matrix )
{
	*( start ) = _mm_add_ps( *start, *matrix.start );
	*( start + 1 ) = _mm_add_ps( *( start + 1 ), *( matrix.start + 1 ) );
	*( start + 2 ) = _mm_add_ps( *( start + 2 ), *( matrix.start + 2 ) );
	*( start + 3 ) = _mm_add_ps( *( start + 3 ), *( matrix.start + 3 ) );

	return *this;
}

Matrix4SSE& Matrix4SSE::operator -= ( const Matrix4SSE& matrix )
{
	*( start ) = _mm_sub_ps( *start, *matrix.start );
	*( start + 1 ) = _mm_sub_ps( *( start + 1 ), *( matrix.start + 1 ) );
	*( start + 2 ) = _mm_sub_ps( *( start + 2 ), *( matrix.start + 2 ) );
	*( start + 3 ) = _mm_sub_ps( *( start + 3 ), *( matrix.start + 3 ) );

	return *this;
}

Matrix4SSE& Matrix4SSE::operator *= ( const Matrix4SSE& matrix )
{
	_float4 temp;

	_float4 _L1, _L2, _L3, _L4;
	_L1 = *( matrix.start );
	_L2 = *( matrix.start + 1 );
	_L3 = *( matrix.start + 2 );
	_L4 = *( matrix.start + 3 );

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 0, 0 ) ), _L1 );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 1 ) ), _L2 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 2 ) ), _L3 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 3 ) ), _L4 ) );
	*( start ) = temp;

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 1, 0 ) ), _L1 );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 1 ) ), _L2 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 2 ) ), _L3 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 3 ) ), _L4 ) );
	*( start + 1 ) = temp;

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 2, 0 ) ), _L1 );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 1 ) ), _L2 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 2 ) ), _L3 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 3 ) ), _L4 ) );
	*( start + 2 ) = temp;

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 3, 0 ) ), _L1 );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 3, 1 ) ), _L2 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 3, 2 ) ), _L3 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 3, 3 ) ), _L4 ) );
	*( start + 3 ) = temp;

	return *this;
}

Matrix4SSE& Matrix4SSE::operator *= ( _float scalar )
{
	_float4 temp = _mm_set_ps1( scalar );

	*( start ) = _mm_mul_ps( *( start ), temp );
	*( start + 1 ) = _mm_mul_ps( *( start + 1 ), temp );
	*( start + 2 ) = _mm_mul_ps( *( start + 2 ), temp );
	*( start + 3 ) = _mm_mul_ps( *( start + 3 ), temp );

	return *this;
}

Matrix4SSE& Matrix4SSE::operator /= ( _float scalar )
{
	_float4 temp = _mm_set_ps1( scalar );

	*( start ) = _mm_div_ps( *( start ), temp );
	*( start + 1 ) = _mm_div_ps( *( start + 1 ), temp );
	*( start + 2 ) = _mm_div_ps( *( start + 2 ), temp );
	*( start + 3 ) = _mm_div_ps( *( start + 3 ), temp );

	return *this;
}

Matrix4SSE Matrix4SSE::operator + ( const Matrix4SSE& matrix ) const
{
	Matrix4SSE result;

	*( result.start ) = _mm_add_ps( *start, *matrix.start );
	*( result.start + 1 ) = _mm_add_ps( *( start + 1 ), *( matrix.start + 1 ) );
	*( result.start + 2 ) = _mm_add_ps( *( start + 2 ), *( matrix.start + 2 ) );
	*( result.start + 3 ) = _mm_add_ps( *( start + 3 ), *( matrix.start + 3 ) );

	return result;
}

Matrix4SSE Matrix4SSE::operator - ( const Matrix4SSE& matrix ) const
{
	Matrix4SSE result;

	*( result.start ) = _mm_sub_ps( *start, *matrix.start );
	*( result.start + 1 ) = _mm_sub_ps( *( start + 1 ), *( matrix.start + 1 ) );
	*( result.start + 2 ) = _mm_sub_ps( *( start + 2 ), *( matrix.start + 2 ) );
	*( result.start + 3 ) = _mm_sub_ps( *( start + 3 ), *( matrix.start + 3 ) );

	return result;
}

Matrix4SSE Matrix4SSE::operator * ( const Matrix4SSE& matrix ) const
{
	Matrix4SSE result;
	_float4 temp;

	_float4 _L1, _L2, _L3, _L4;
	_L1 = *( matrix.start );
	_L2 = *( matrix.start + 1 );
	_L3 = *( matrix.start + 2 );
	_L4 = *( matrix.start + 3 );

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 0, 0 ) ), _L1 );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 1 ) ), _L2 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 2 ) ), _L3 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 0, 3 ) ), _L4 ) );
	*( result.start ) = temp;

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 1, 0 ) ), _L1 );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 1 ) ), _L2 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 2 ) ), _L3 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1, 3 ) ), _L4 ) );
	*( result.start + 1 ) = temp;

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 2, 0 ) ), _L1 );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 1 ) ), _L2 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 2 ) ), _L3 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2, 3 ) ), _L4 ) );
	*( result.start + 2 ) = temp;

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 3, 0 ) ), _L1 );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 3, 1 ) ), _L2 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 3, 2 ) ), _L3 ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 3, 3 ) ), _L4 ) );
	*( result.start + 3 ) = temp;

	return result;
}

Matrix4SSE Matrix4SSE::operator * ( _float scalar ) const
{
	Matrix4SSE result;
	_float4 temp = _mm_set_ps1( scalar );

	*( result.start ) = _mm_mul_ps( *( start ), temp );
	*( result.start + 1 ) = _mm_mul_ps( *( start + 1 ), temp );
	*( result.start + 2 ) = _mm_mul_ps( *( start + 2 ), temp );
	*( result.start + 3 ) = _mm_mul_ps( *( start + 3 ), temp );

	return result;
}

Vector4SSE Matrix4SSE::operator * ( const Vector4SSE& vector ) const
{
	Matrix4SSE m = *this;
	return vector * m.Transpose();
}

Matrix4SSE Matrix4SSE::operator / ( _float scalar ) const
{
	Matrix4SSE result;
	_float4 temp = _mm_set_ps1( scalar );

	*( result.start ) = _mm_div_ps( *( start ), temp );
	*( result.start + 1 ) = _mm_div_ps( *( start + 1 ), temp );
	*( result.start + 2 ) = _mm_div_ps( *( start + 2 ), temp );
	*( result.start + 3 ) = _mm_div_ps( *( start + 3 ), temp );

	return result;
}

_bool Matrix4SSE::operator == ( const Matrix4SSE& matrix ) const
{
	for ( _dword i = 0; i < 4; i ++ )
	{
		for ( _dword j = 0; j < 4; j ++ )
		{
			if ( operator()( i, j ) != matrix( i, j ) )
				return _false;
		}
	}

	return _true;
}

_bool Matrix4SSE::operator != ( const Matrix4SSE& matrix ) const
{
	return !( operator == ( matrix ) );
}

_bool Matrix4SSE::Equal( const Matrix4SSE& matrix, _float margin ) const
{
	_float4 temp;
	_float* ptr;

	for( _dword i = 0; i < 4; i++ )
	{
		temp = _mm_sub_ps( *( start + i ), *( matrix.start + i ) );

		ptr = (_float*)&temp;

		for( _dword j = 0; j < 4; j++ )
		{
			if( Math::Abs( *ptr++ ) > margin )
			{
				return _false;
			}
		}
	}

	return _true;
}

_float Matrix4SSE::Determinant( ) const
{
	_float4 Va, Vb, Vc;
	_float4 r1, r2, r3, t1, t2, sum;
	_float4 Det;

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

	return *( (_float*)&Det );

}

Matrix4SSE& Matrix4SSE::Identity( )
{
	_float4 zerone = _mm_set_ps( 1.0f, 0.0f, 0.0f, 1.0f );

	_mm_storel_pi( ( _float2 * )&operator( )( 0, 0 ), zerone );
	_mm_storel_pi( ( _float2 * )&operator( )( 0, 2 ), _mm_setzero_ps() );
	_mm_storeh_pi( ( _float2 * )&operator( )( 1, 0 ), zerone );
	_mm_storel_pi( ( _float2 * )&operator( )( 1, 2 ), _mm_setzero_ps() );
	_mm_storel_pi( ( _float2 * )&operator( )( 2, 0 ), _mm_setzero_ps() );
	_mm_storel_pi( ( _float2 * )&operator( )( 2, 2 ), zerone );
	_mm_storel_pi( ( _float2 * )&operator( )( 3, 0 ), _mm_setzero_ps() );
	_mm_storeh_pi( ( _float2 * )&operator( )( 3, 2 ), zerone );

	return *this;
}

Matrix4SSE& Matrix4SSE::Transpose( )
{
	_float4 *_L1, *_L2, *_L3, *_L4;
	_L1 = start;
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

	return *this;
}

Matrix4SSE& Matrix4SSE::Inverse( )
{
	_float determinant = Determinant( );

	if ( determinant != 0.0f )
	{
		Adjoint( );

		_float4 temp = _mm_set_ps1( determinant );

		*start = _mm_div_ps( *start, temp );
		*( start + 1 ) = _mm_div_ps( *( start + 1 ), temp );
		*( start + 2 ) = _mm_div_ps( *( start + 2 ), temp );
		*( start + 3 ) = _mm_div_ps( *( start + 3 ), temp );
	}

	return *this;
}

Matrix4SSE& Matrix4SSE::Adjoint( )
{
	_float m00 = operator()( 0, 0 ), m01 = operator()( 0, 1 ), m02 = operator()( 0, 2 ), m03 = operator()( 0, 3 ), 
		m10 = operator()( 1, 0 ), m11 = operator()( 1, 1 ), m12 = operator()( 1, 2 ), m13 = operator()( 1, 3 ),
		m20 = operator()( 2, 0 ), m21 = operator()( 2, 1 ), m22 = operator()( 2, 2 ), m23 = operator()( 2, 3 ), 
		m30 = operator()( 3, 0 ), m31 = operator()( 3, 1 ), m32 = operator()( 3, 2 ), m33 = operator()( 3, 3 );

	operator()( 0, 0 ) =   Matrix3( m11, m12, m13, m21, m22, m23, m31, m32, m33 ).Determinant( );
	operator()( 1, 0 ) = - Matrix3( m10, m12, m13, m20, m22, m23, m30, m32, m33 ).Determinant( );
	operator()( 2, 0 ) =   Matrix3( m10, m11, m13, m20, m21, m23, m30, m31, m33 ).Determinant( );
	operator()( 3, 0 ) = - Matrix3( m10, m11, m12, m20, m21, m22, m30, m31, m32 ).Determinant( );

	operator()( 0, 1 ) = - Matrix3( m01, m02, m03, m21, m22, m23, m31, m32, m33 ).Determinant( );
	operator()( 1, 1 ) =   Matrix3( m00, m02, m03, m20, m22, m23, m30, m32, m33 ).Determinant( );
	operator()( 2, 1 ) = - Matrix3( m00, m01, m03, m20, m21, m23, m30, m31, m33 ).Determinant( );
	operator()( 3, 1 ) =   Matrix3( m00, m01, m02, m20, m21, m22, m30, m31, m32 ).Determinant( );

	operator()( 0, 2 ) =	Matrix3( m01, m02, m03, m11, m12, m13, m31, m32, m33 ).Determinant( );
	operator()( 1, 2 ) = - Matrix3( m00, m02, m03, m10, m12, m13, m30, m32, m33 ).Determinant( );
	operator()( 2, 2 ) =   Matrix3( m00, m01, m03, m10, m11, m13, m30, m31, m33 ).Determinant( );
	operator()( 3, 2 ) = - Matrix3( m00, m01, m02, m10, m11, m12, m30, m31, m32 ).Determinant( );

	operator()( 0, 3 ) = - Matrix3( m01, m02, m03, m11, m12, m13, m21, m22, m23 ).Determinant( );
	operator()( 1, 3 ) =   Matrix3( m00, m02, m03, m10, m12, m13, m20, m22, m23 ).Determinant( );
	operator()( 2, 3 ) = - Matrix3( m00, m01, m03, m10, m11, m13, m20, m21, m23 ).Determinant( );
	operator()( 3, 3 ) =   Matrix3( m00, m01, m02, m10, m11, m12, m20, m21, m22 ).Determinant( );

	return *this;
}

Matrix4SSE& Matrix4SSE::Translation( const Vector3& vector )
{
	*start = _mm_setzero_ps();
	*( start + 1 ) = _mm_setzero_ps();
	*( start + 2 ) = _mm_setzero_ps();
	*( start + 3 ) = _mm_set_ps( 1.0f, vector.z, vector.y, vector.x );

	return *this;
}

Matrix4SSE& Matrix4SSE::FromRotation( const Quaternion& rotation )
{
	_float xx = rotation.x * rotation.x * 2.0f, yy = rotation.y * rotation.y * 2.0f, zz = rotation.z * rotation.z * 2.0f;
	_float xy = rotation.x * rotation.y * 2.0f, zw = rotation.z * rotation.w * 2.0f, xz = rotation.x * rotation.z * 2.0f;
	_float yw = rotation.y * rotation.w * 2.0f, yz = rotation.y * rotation.z * 2.0f, xw = rotation.x * rotation.w * 2.0f;

	*start = _mm_set_ps( 0.0f, xz - yw, xy + zw, 1.0f - yy - zz );
	*( start + 1 ) = _mm_set_ps( 0.0f, yz + xw, 1.0f - xx - zz, xy - zw );
	*( start + 2 ) = _mm_set_ps( 0.0f, 1.0f - xx - yy, yz - xw, xz + yw );
	*( start + 3 ) = _mm_setzero_ps();

	return *this;
}

Matrix3 Matrix4SSE::GetRotationMatrix( ) const
{
	return Matrix3(
		operator()( 0, 0 ), operator()( 0, 1 ), operator()( 0, 2 ),
		operator()( 1, 0 ), operator()( 1, 1 ), operator()( 1, 2 ),
		operator()( 2, 0 ), operator()( 2, 1 ), operator()( 2, 2 ) );
}

_void Matrix4SSE::Write( _float* buffer, _dword num ) const
{
	FL_ASSERT( num >= 1 && num <= 4 );

	_dword i = 0;

	for ( i = 0; i < num - 1; i ++ )
	{
		_mm_store_ps( buffer, *( start + i ) );
		buffer = buffer + 3;
	}

	_float* temp = (_float*)( start + i );
	*buffer = *temp;
	*( buffer + 1 ) = *( temp + 1 );
	*( buffer + 2 ) = *( temp + 2 );
}