//============================================================================
// AxisAlignedBox.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// AxisAlignedBox Implementation
//----------------------------------------------------------------------------

const AxisAlignedBox AxisAlignedBox::cNullBox( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
const AxisAlignedBox AxisAlignedBox::cInitBox( Math::cMaxFloat, Math::cMaxFloat, Math::cMaxFloat, Math::cMinFloat, Math::cMinFloat, Math::cMinFloat );

AxisAlignedBox& AxisAlignedBox::operator *= ( const Matrix4& matrix )
{
	Box box( mMinVector, mMaxVector ); box *= matrix;

	mMinVector = mMaxVector = box.mVectors[0];

	for ( _dword i = 1; i < 8; i ++ )
	{
		mMinVector.x = Math::Min( mMinVector.x, box.mVectors[i].x );
		mMinVector.y = Math::Min( mMinVector.y, box.mVectors[i].y );
		mMinVector.z = Math::Min( mMinVector.z, box.mVectors[i].z );
		mMaxVector.x = Math::Max( mMaxVector.x, box.mVectors[i].x );
		mMaxVector.y = Math::Max( mMaxVector.y, box.mVectors[i].y );
		mMaxVector.z = Math::Max( mMaxVector.z, box.mVectors[i].z );
	}

	return *this;
}