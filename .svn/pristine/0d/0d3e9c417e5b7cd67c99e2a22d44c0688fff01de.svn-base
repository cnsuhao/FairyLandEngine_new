//============================================================================
// Triangle.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Triangle Implementation
//----------------------------------------------------------------------------

Triangle& Triangle::operator *= ( const Matrix4& matrix )
{
	mVector1 *= matrix;
	mVector2 *= matrix;
	mVector3 *= matrix;

	return *this;
}

Triangle Triangle::operator * ( const Matrix4& matrix ) const
{
	return Triangle( mVector1 * matrix, mVector2 * matrix, mVector3 * matrix );
}