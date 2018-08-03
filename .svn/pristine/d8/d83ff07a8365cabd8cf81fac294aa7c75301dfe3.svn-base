//============================================================================
// EulerAngles.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// EulerAngles
//----------------------------------------------------------------------------

//! This class represents an euler angles, a sequence of three rotations about three mutually perpendicular axes.
class EulerAngles
{
public:
	//! The identity euler angles.
	const static EulerAngles cIdentity;

	//! Rotation angle about x-axis.
	_float	mPitch;
	//! Rotation angle about y-axis.
	_float	mHeading;
	//! Rotation angle about z-axis.
	_float	mBank;

	//! Constructor, create an euler angles without sets components.
	//! @param		none
	inline EulerAngles( );
	//! Constructor, create an euler angles and sets components.
	//! @param		pitch		Rotation angle about x-axis.
	//! @param		heading		Rotation angle about y-axis.
	//! @param		bank		Rotation angle about z-axis.
	inline EulerAngles( _float pitch, _float heading, _float bank );

	//! Calculates identity euler angles, and save result in current one.
	//! @param		none
	//! @return		The identity euler angles.
	EulerAngles& Identity( );
	//! Calculates canonica euler angles, and save result in current one.
	//! @param		none
	//! @return		The canonical euler angles.
	EulerAngles& Canonize( );
};

//----------------------------------------------------------------------------
// EulerAngles Implementation
//----------------------------------------------------------------------------

EulerAngles::EulerAngles( )
{
}

EulerAngles::EulerAngles( _float pitch, _float heading, _float bank )
	: mPitch( pitch ), mHeading( heading ), mBank( bank )
{
}

};