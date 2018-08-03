//============================================================================
// Vector4SSE.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Vector4SSE Implementation
//----------------------------------------------------------------------------

const Vector4SSE Vector4SSE::cOrigin( 0.0f, 0.0f, 0.0f, 0.0f );
const Vector4SSE Vector4SSE::cIdentity( 1.0f, 1.0f, 1.0f, 1.0f );
const Vector4SSE Vector4SSE::cXAxis( 1.0f, 0.0f, 0.0f, 0.0f );
const Vector4SSE Vector4SSE::cYAxis( 0.0f, 1.0f, 0.0f, 0.0f );
const Vector4SSE Vector4SSE::cZAxis( 0.0f, 0.0f, 1.0f, 0.0f );
const Vector4SSE Vector4SSE::cWAxis( 0.0f, 0.0f, 0.0f, 1.0f );
const Vector4SSE Vector4SSE::cMaxVector( Math::cMaxFloat, Math::cMaxFloat, Math::cMaxFloat, Math::cMaxFloat );
const Vector4SSE Vector4SSE::cMinVector( Math::cMinFloat, Math::cMinFloat, Math::cMinFloat, Math::cMinFloat );


Vector4SSE& Vector4SSE::operator *= ( const Quaternion& quaternion )
{
	Quaternion tempquaternion = quaternion;
	tempquaternion *= Quaternion( operator()( 0 ), operator()( 1 ), operator()( 2 ), 0.0f );
	tempquaternion *= - quaternion;

	*start = _mm_set_ps( 0.0f, tempquaternion.z, tempquaternion.y, tempquaternion.x );

	return *this;
}

Vector4SSE& Vector4SSE::operator *= ( const Matrix4SSE& matrix )
{
	_float4 temp;

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 0 ) ), *( matrix.start ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1 ) ), *( matrix.start + 1 ) ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2 ) ), *( matrix.start + 2 ) ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 3 ) ), *( matrix.start + 3 ) ) );

	*start = temp;

	return *this;
}

Vector4SSE Vector4SSE::operator * ( const Matrix4SSE& matrix ) const
{
	Vector4SSE result;
	_float4 temp;

	temp = _mm_mul_ps( _mm_set_ps1( operator()( 0 ) ), *( matrix.start ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 1 ) ), *( matrix.start + 1 ) ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 2 ) ), *( matrix.start + 2 ) ) );
	temp = _mm_add_ps( temp, _mm_mul_ps( _mm_set_ps1( operator()( 3 ) ), *( matrix.start + 3 ) ) );

	*result.start = temp;
	return result;
}

Vector4SSE Vector4SSE::Project( const Vector4SSE& vector1, const Vector4SSE& vector2 )
{
	Vector4SSE normal = vector2;
	normal.Normalize( );

	return normal * Dot( vector1, normal );
}

Vector4SSE Vector4SSE::Plumb( const Vector4SSE& vector1, const Vector4SSE& vector2 )
{
	return vector1 - Project( vector1, vector2 );
}

Vector4SSE Vector4SSE::Lerp( const Vector4SSE& vector1, const Vector4SSE& vector2, _float factor )
{
	factor = Math::Clamp( factor, 0.0f, 1.0f );

	return vector1 + ( vector2 - vector1 ) * factor;
}