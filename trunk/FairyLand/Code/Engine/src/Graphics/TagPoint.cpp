//============================================================================
// TagPoint.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// TagPoint Implementation
//----------------------------------------------------------------------------

TagPoint::TagPoint( )
{
	mPosition	= Vector3::cOrigin;
	mDirection	= Vector3::cZAxis;
	mRotation	= Matrix3::cIdentity;

	IncreaseRefCount( );
}

_void TagPoint::SetPosition( const Vector3& position )
{
	mPosition = position;
}

const Vector3& TagPoint::GetPosition( ) const
{
	return mPosition;
}

_void TagPoint::SetDirection( const Vector3& direction )
{
	mDirection = direction;
	mDirection.Normalize( );
}

const Vector3& TagPoint::GetDirection( ) const
{
	return mDirection;
}

_void TagPoint::SetRotation( const Matrix3& rotation )
{
	mRotation = rotation;
}

const Matrix3& TagPoint::GetRotation( ) const
{
	return mRotation;
}