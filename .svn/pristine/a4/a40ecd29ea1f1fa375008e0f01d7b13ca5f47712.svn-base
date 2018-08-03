//============================================================================
// Frustum.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Frustum Implementation
//----------------------------------------------------------------------------

Frustum::Frustum( const Matrix4& viewtransform, const Matrix4& projectiontransform )
{
	Matrix4 inversematrix = viewtransform * projectiontransform;
	inversematrix.Inverse( );

	mVectors[0] = Vector3( -1.0f, -1.0f, 0.0f ) * inversematrix;
	mVectors[1] = Vector3( -1.0f,  1.0f, 0.0f ) * inversematrix;
	mVectors[2] = Vector3(  1.0f,  1.0f, 0.0f ) * inversematrix;
	mVectors[3] = Vector3(  1.0f, -1.0f, 0.0f ) * inversematrix;
	mVectors[4] = Vector3( -1.0f, -1.0f, 1.0f ) * inversematrix;
	mVectors[5] = Vector3( -1.0f,  1.0f, 1.0f ) * inversematrix;
	mVectors[6] = Vector3(  1.0f,  1.0f, 1.0f ) * inversematrix;
	mVectors[7] = Vector3(  1.0f, -1.0f, 1.0f ) * inversematrix;

	mPlanes[0] = Plane( mVectors[0], mVectors[1], mVectors[2] ); // Near
	mPlanes[1] = Plane( mVectors[5], mVectors[7], mVectors[6] ); // Far
	mPlanes[2] = Plane( mVectors[1], mVectors[4], mVectors[5] ); // Left
	mPlanes[3] = Plane( mVectors[6], mVectors[7], mVectors[2] ); // Right
	mPlanes[4] = Plane( mVectors[1], mVectors[5], mVectors[2] ); // Top
	mPlanes[5] = Plane( mVectors[3], mVectors[4], mVectors[0] ); // Bottom
}

_bool	Frustum::operator == ( const Frustum& frustum ) const
{
	return	mPlanes[0] == frustum.mPlanes[0] && mPlanes[1] == frustum.mPlanes[1] && mPlanes[2] == frustum.mPlanes[2] && 
		mPlanes[3] == frustum.mPlanes[3] && mPlanes[4] == frustum.mPlanes[4] && mPlanes[5] == frustum.mPlanes[5];
}

_bool	Frustum::operator != ( const Frustum& frustum ) const
{
	return	mPlanes[0] != frustum.mPlanes[0] || mPlanes[1] != frustum.mPlanes[1] || mPlanes[2] != frustum.mPlanes[2] ||
		mPlanes[3] != frustum.mPlanes[3] || mPlanes[4] != frustum.mPlanes[4] || mPlanes[5] != frustum.mPlanes[5];
}

_bool Frustum::Inside( const AxisAlignedBox& box ) const
{
	Vector3 vmin;

	for ( _dword i = 0; i < 6; i ++ )
	{
		// X axis.
		vmin.x = mPlanes[i].a > 0.0f ? box.mMinVector.x : box.mMaxVector.x;

		// Y axis.
		vmin.y = mPlanes[i].b > 0.0f ? box.mMinVector.y : box.mMaxVector.y;

		// Z axis.
		vmin.z = mPlanes[i].c > 0.0f ? box.mMinVector.z : box.mMaxVector.z;

		if ( mPlanes[i].Distance( vmin ) > 0.0f )
			return _false;
	}

	return _true;
}

_bool Frustum::Inside( const Box& box ) const
{
	_byte outside[8] = { 0 };

	for ( _long i = 0; i < 8; i ++ )
	{
		for ( _long j = 0; j < 6; j ++ )
		{
			if ( mPlanes[j].Distance( box.mVectors[i] ) > 0.0f )
			{
				outside[i] |= 1 << j;
			}
		}

		if ( outside[i] == 0 )
			return _true;
	}

	return ( outside[0] & outside[1] & outside[2] & outside[3] & outside[4] & outside[5] & outside[6] & outside[7] ) == 0;
}
