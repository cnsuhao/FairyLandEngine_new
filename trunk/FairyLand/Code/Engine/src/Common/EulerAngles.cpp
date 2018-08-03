//============================================================================
// EulerAngles.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// EulerAngles Implementation
//----------------------------------------------------------------------------

const EulerAngles EulerAngles::cIdentity( 0.0f, 0.0f, 0.0f );

EulerAngles& EulerAngles::Identity( )
{
	mHeading	= 0.0f;
	mPitch		= 0.0f;
	mBank		= 0.0f;

	return *this;
}

EulerAngles& EulerAngles::Canonize( )
{
	// First, wrap pitch in range ¨Cpi ... pi.
	mPitch = Math::WrapPi( mPitch );

	// Check for "the back side" of the matrix pitch outside the canonical range of ¨Cpi/2 ... pi/2.
	if ( mPitch < Math::cPi * -0.5f )
	{
		mPitch		= -Math::cPi - mPitch;
		mHeading	= mHeading + Math::cPi;
		mBank		= mBank + Math::cPi;
	}
	else if ( mPitch > Math::cPi * 0.5f )
	{
		mPitch		= Math::cPi - mPitch;
		mHeading	= mHeading + Math::cPi;
		mBank		= mBank + Math::cPi;
	}

	// Check for the gimbel lock case (within a slight tolerance).
	if ( Math::Abs( mPitch ) > Math::cPi * 0.5f - Math::cEpsilon )
	{
		// In gimbel lock. Assign all rotation about the vertical axis to heading.
		mHeading	= mHeading + mBank;
		mBank		= 0.0f;
	}
	else
	{
		// Not in gimbel lock. Wrap the bank angle in canonical range.
		mBank		= Math::WrapPi( mBank );
	}

	// Wrap heading in canonical range.
	mHeading = Math::WrapPi( mHeading );

	return *this;
}