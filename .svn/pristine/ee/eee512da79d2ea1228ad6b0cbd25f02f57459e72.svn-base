//============================================================================
// Ray.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Ray Implementation
//----------------------------------------------------------------------------

Ray::Ray( _float x, _float y, const Matrix4& viewtransform, const Matrix4& projectiontransform )
{
	Matrix4 viewinverse = viewtransform;
	viewinverse.Inverse( );

	Vector3 temp( x / projectiontransform( 0, 0 ), - y / projectiontransform( 1, 1 ), 1.0f );

	mOrigin.x		= viewinverse( 3, 0 );
	mOrigin.y		= viewinverse( 3, 1 );
	mOrigin.z		= viewinverse( 3, 2 );
	mDirection.x	= temp.x * viewinverse( 0, 0 ) + temp.y * viewinverse( 1, 0 ) + temp.z * viewinverse( 2, 0 );
	mDirection.y	= temp.x * viewinverse( 0, 1 ) + temp.y * viewinverse( 1, 1 ) + temp.z * viewinverse( 2, 1 );
	mDirection.z	= temp.x * viewinverse( 0, 2 ) + temp.y * viewinverse( 1, 2 ) + temp.z * viewinverse( 2, 2 );

	mDirection.Normalize( );
}