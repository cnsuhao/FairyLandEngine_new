//============================================================================
// Vector3SSE.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Vector3SSE Implementation
//----------------------------------------------------------------------------

const Vector3SSE Vector3SSE::cOrigin( 0.0f, 0.0f, 0.0f );
const Vector3SSE Vector3SSE::cIdentity( 1.0f, 1.0f, 1.0f );
const Vector3SSE Vector3SSE::cXAxis( 1.0f, 0.0f, 0.0f );
const Vector3SSE Vector3SSE::cYAxis( 0.0f, 1.0f, 0.0f );
const Vector3SSE Vector3SSE::cZAxis( 0.0f, 0.0f, 1.0f );
const Vector3SSE Vector3SSE::cMaxVector( Math::cMaxFloat, Math::cMaxFloat, Math::cMaxFloat );
const Vector3SSE Vector3SSE::cMinVector( Math::cMinFloat, Math::cMinFloat, Math::cMinFloat );


Vector3SSE& Vector3SSE::operator *= ( const Quaternion& quaternion )
{
	Quaternion tempquaternion = quaternion;
	tempquaternion *= Quaternion( operator()( 0 ), operator()( 1 ), operator()( 2 ), 0.0f );
	tempquaternion *= - quaternion;

	*start = _mm_set_ps( 0.0f, tempquaternion.z, tempquaternion.y, tempquaternion.x );

	return *this;
}

Vector3SSE& Vector3SSE::operator *= ( const Matrix3SSE& matrix )
{
	_float4 vector3 = start[0];
	_float4 t1 = _mm_mul_ps( _mm_shuffle_ps( vector3, vector3, _MM_SHUFFLE( 0, 0, 0, 0 ) ), vector3 );
	_float4 t2 = _mm_mul_ps( _mm_shuffle_ps( vector3, vector3, _MM_SHUFFLE( 1, 1, 1, 1 ) ), vector3 );
	t1 = _mm_add_ps( t1, t2 );
	
	t2 = _mm_mul_ps( _mm_shuffle_ps( vector3, vector3, _MM_SHUFFLE( 2, 2, 2, 2 ) ), vector3 );
	t1 = _mm_add_ps( t1, t2 );

	start[0] = t1;

	return *this;
}

Vector3SSE Vector3SSE::operator * ( const Matrix3SSE& matrix ) const
{
	Vector3SSE result;
	//temp = _mm_mul_ps( _mm_set_ps1( operator()( 0 ) ), *( matrix.start ) );
	//temp1 = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1 ) ), *( matrix.start + 1 ) ) );
	//temp = _mm_add_ps( temp1, _mm_mul_ps( _mm_set_ps1( operator()( 2 ) ), *( matrix.start + 2 ) ) );

	_float4 vector3 = start[0];
	_float4 t1 = _mm_mul_ps( _mm_shuffle_ps( vector3, vector3, _MM_SHUFFLE( 0, 0, 0, 0 ) ), *matrix.start );
	_float4 t2 = _mm_mul_ps( _mm_shuffle_ps( vector3, vector3, _MM_SHUFFLE( 1, 1, 1, 1 ) ), *( matrix.start + 1 ) );
	t1 = _mm_add_ps( t1, t2 );
	
	t2 = _mm_mul_ps( _mm_shuffle_ps( vector3, vector3, _MM_SHUFFLE( 2, 2, 2, 2 ) ), *( matrix.start + 2 ) );
	t1 = _mm_add_ps( t1, t2 );

	result.start[0] = t1;
	//_mm_stream_ps( ( _float*)result.start, temp ); 

	//_float4 temp1, temp2, temp3;

	//_float *ptemp = ( _float* )result.start;
	//_float *ptemp1 = ( _float* )&temp1;
	//_float *ptemp2 = ( _float* )&temp2;
	//_float *ptemp3 = ( _float* )&temp3;

	//temp1 = _mm_mul_ps( *start, *( matrix.start ) );
	//temp2 = _mm_mul_ps( *start, *( matrix.start + 1 ) );
	//temp3 = _mm_mul_ps( *start, *( matrix.start + 2 ) );

	//ptemp[0] = ptemp1[0] + ptemp1[1] + ptemp1[2];

	//*ptemp = *ptemp1 + *( ptemp1 + 1 ) + *( ptemp1 + 2 );
	//*( ptemp + 1 ) = *ptemp2 + *( ptemp2 + 1 ) + *( ptemp2 + 2 );
	//*( ptemp + 2 ) = *ptemp3 + *( ptemp3 + 1 ) + *( ptemp3 + 2 );

	return result;
}

Vector3SSE& Vector3SSE::operator *= ( const Matrix4SSE& matrix )
{
	_float* w;
	_float4 temp, vector3 = start[0];

	temp = _mm_mul_ps( _mm_shuffle_ps( vector3, vector3, _MM_SHUFFLE( 0, 0, 0, 0 ) ), matrix.start[0] );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_shuffle_ps( vector3, vector3, _MM_SHUFFLE( 1, 1, 1, 1 ) ), matrix.start[1] ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_shuffle_ps( vector3, vector3, _MM_SHUFFLE( 2, 2, 2, 2 ) ), matrix.start[2] ) );
	temp = _mm_add_ps( temp, matrix.start[3] );

	w = ( _float* ) &temp;

	if( w[3] != 0.0f )
	{
		temp = _mm_div_ps( temp, _mm_shuffle_ps( vector3, vector3, _MM_SHUFFLE( 3, 3, 3, 3 ) ) );
		w[3] = 0.0f;
	}

	start[0] = temp;

	return *this;
}
