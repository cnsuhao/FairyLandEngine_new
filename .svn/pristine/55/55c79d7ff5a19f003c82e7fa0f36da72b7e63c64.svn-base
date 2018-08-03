//============================================================================
// AxisAlignedBox.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// AxisAlignedBox
//----------------------------------------------------------------------------

class AxisAlignedBox
{
public:
	const static AxisAlignedBox cNullBox;
	const static AxisAlignedBox cInitBox;

public:
	union
	{
		struct
		{
			_float	x1, y1, z1;
			_float	x2, y2, z2;
		};

		struct
		{
			Vector3	mMinVector;
			Vector3	mMaxVector;
		};
	};

public:
	inline AxisAlignedBox( );
	inline AxisAlignedBox( _float xx1, _float yy1, _float zz1, _float xx2, _float yy2, _float zz2 );
	inline AxisAlignedBox( const Vector3& minvector, const Vector3& maxvector );

	inline _bool operator == ( const AxisAlignedBox& box ) const;
	inline _bool operator != ( const AxisAlignedBox& box ) const;

	inline _bool Equal( const AxisAlignedBox& box, _float margin = Math::cEpsilon ) const;

	inline AxisAlignedBox& operator += ( const AxisAlignedBox& box );
	inline AxisAlignedBox& operator += ( const Vector3& vector );
	       AxisAlignedBox& operator *= ( const Matrix4& matrix );

	inline AxisAlignedBox operator + ( const AxisAlignedBox& box ) const;
	inline AxisAlignedBox operator + ( const Vector3& vector ) const;
	inline AxisAlignedBox operator * ( const Matrix4& matrix ) const;

	inline _bool VectorInBox( const Vector3& vector ) const;

	inline Vector3 CenterVector( ) const;
	inline Vector3 BoxSize( ) const;
};

//----------------------------------------------------------------------------
// AxisAlignedBox Implementation
//----------------------------------------------------------------------------

AxisAlignedBox::AxisAlignedBox( )
	: mMinVector( Vector3::cOrigin ), mMaxVector( Vector3::cOrigin )
{
}

AxisAlignedBox::AxisAlignedBox( _float xx1, _float yy1, _float zz1, _float xx2, _float yy2, _float zz2 )
	: x1( xx1 ), y1( yy1 ), z1( zz1 ), x2( xx2 ), y2( yy2 ), z2( zz2 )
{
}

AxisAlignedBox::AxisAlignedBox( const Vector3& minvector, const Vector3& maxvector )
{
	if ( minvector.x < maxvector.x )
		{ x1 = minvector.x; x2 = maxvector.x; }
	else
		{ x1 = maxvector.x; x2 = minvector.x; }

	if ( minvector.y < maxvector.y )
		{ y1 = minvector.y; y2 = maxvector.y; }
	else
		{ y1 = maxvector.y; y2 = minvector.y; }

	if ( minvector.z < maxvector.z )
		{ z1 = minvector.z; z2 = maxvector.z; }
	else
		{ z1 = maxvector.z; z2 = minvector.z; }
}

_bool AxisAlignedBox::operator == ( const AxisAlignedBox& box ) const
{
	return mMinVector == box.mMinVector && mMaxVector == box.mMaxVector;
}

_bool AxisAlignedBox::operator != ( const AxisAlignedBox& box ) const
{
	return mMinVector != box.mMinVector || mMaxVector != box.mMaxVector;
}

_bool AxisAlignedBox::Equal( const AxisAlignedBox& box, _float margin ) const
{
	return mMinVector.Equal( box.mMinVector, margin ) && mMaxVector.Equal( box.mMaxVector, margin );
}

AxisAlignedBox& AxisAlignedBox::operator += ( const AxisAlignedBox& box )
{
	mMinVector.x = Math::Min( mMinVector.x, box.mMinVector.x );
	mMinVector.y = Math::Min( mMinVector.y, box.mMinVector.y );
	mMinVector.z = Math::Min( mMinVector.z, box.mMinVector.z );
	mMaxVector.x = Math::Max( mMaxVector.x, box.mMaxVector.x );
	mMaxVector.y = Math::Max( mMaxVector.y, box.mMaxVector.y );
	mMaxVector.z = Math::Max( mMaxVector.z, box.mMaxVector.z );

	return *this;
}

AxisAlignedBox& AxisAlignedBox::operator += ( const Vector3& vector )
{
	mMinVector.x = Math::Min( mMinVector.x, vector.x );
	mMinVector.y = Math::Min( mMinVector.y, vector.y );
	mMinVector.z = Math::Min( mMinVector.z, vector.z );
	mMaxVector.x = Math::Max( mMaxVector.x, vector.x );
	mMaxVector.y = Math::Max( mMaxVector.y, vector.y );
	mMaxVector.z = Math::Max( mMaxVector.z, vector.z );

	return *this;
}

AxisAlignedBox AxisAlignedBox::operator + ( const AxisAlignedBox& box ) const
{
	AxisAlignedBox returnbox( *this );

	return returnbox += box;
}

AxisAlignedBox AxisAlignedBox::operator + ( const Vector3& vector ) const
{
	AxisAlignedBox returnbox( *this );

	return returnbox += vector;
}

AxisAlignedBox AxisAlignedBox::operator * ( const Matrix4& matrix ) const
{
	AxisAlignedBox returnbox( *this );

	return returnbox *= matrix;
}

_bool AxisAlignedBox::VectorInBox( const Vector3& vector ) const
{
	return	vector.x >= mMinVector.x && vector.x <= mMaxVector.x &&
			vector.y >= mMinVector.y && vector.y <= mMaxVector.y &&
			vector.z >= mMinVector.z && vector.z <= mMaxVector.z;
}

Vector3 AxisAlignedBox::CenterVector( ) const
{
	return ( mMinVector + mMaxVector ) / 2.0f;
}

Vector3 AxisAlignedBox::BoxSize( ) const
{
	return ( mMaxVector - mMinVector ).Absolute( );
}

//----------------------------------------------------------------------------
// AxisAlignedBoxMaker
//----------------------------------------------------------------------------

__declspec( align(16) ) class AxisAlignedBoxMaker
{
private:
	_float4		mMinFloat4;
	_float4		mMaxFloat4;

public:
	inline AxisAlignedBoxMaker( );

	inline AxisAlignedBox GetAxisAlignedBox( ) const;

	inline _void Expand( const Vector3& vector );
	inline _void Expand( const _float4& vector );
	inline _void Expand( const _float4& v0, const _float4& v1, const _float4& v2, const _float4& v3 );
};

AxisAlignedBoxMaker::AxisAlignedBoxMaker( )
{
	mMinFloat4 = _mm_set_ps( 0.0f, Math::cMaxFloat, Math::cMaxFloat, Math::cMaxFloat );
	mMaxFloat4 = _mm_set_ps( 0.0f, Math::cMinFloat, Math::cMinFloat, Math::cMinFloat );
}

AxisAlignedBox AxisAlignedBoxMaker::GetAxisAlignedBox( ) const
{
	return AxisAlignedBox( *( (Vector3*) &mMinFloat4 ), *( (Vector3*) &mMaxFloat4 ) );
}

_void AxisAlignedBoxMaker::Expand( const Vector3& vector )
{
	_float4 ssevalue = _mm_set_ps( 0.0f, vector.z, vector.y, vector.x );

	mMinFloat4 = _mm_min_ps( mMinFloat4, ssevalue );
	mMaxFloat4 = _mm_max_ps( mMaxFloat4, ssevalue );
}

_void AxisAlignedBoxMaker::Expand( const _float4& ssevalue )
{
	mMinFloat4 = _mm_min_ps( mMinFloat4, ssevalue );
	mMaxFloat4 = _mm_max_ps( mMaxFloat4, ssevalue );
}

_void AxisAlignedBoxMaker::Expand( const _float4& v0, const _float4& v1, const _float4& v2, const _float4& v3 )
{
	mMinFloat4 = _mm_min_ps( mMinFloat4, _mm_min_ps( _mm_min_ps( v0, v1 ), _mm_min_ps( v2, v3 ) ) );
	mMaxFloat4 = _mm_max_ps( mMaxFloat4, _mm_max_ps( _mm_max_ps( v0, v1 ), _mm_max_ps( v2, v3 ) ) );
}

};