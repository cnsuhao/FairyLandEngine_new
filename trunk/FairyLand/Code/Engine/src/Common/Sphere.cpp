//============================================================================
// Sphere.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Sphere Implementation
//----------------------------------------------------------------------------

Sphere& Sphere::operator *= ( const Matrix4& matrix )
{
	mCenter *= matrix;

	return *this;
}

Sphere Sphere::operator * ( const Matrix4& matrix ) const
{
	return Sphere( mCenter * matrix, mRadius );
}