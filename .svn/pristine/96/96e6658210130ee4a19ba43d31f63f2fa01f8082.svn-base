//============================================================================
// Matrix4Adjustor.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Matrix4Adjustor Implementation
//----------------------------------------------------------------------------

Matrix4Adjustor::Matrix4Adjustor( )
{
	mTranslation			= Vector3::cOrigin;
	mTranslationDuration	= 0;
	mTranslationTickcount	= 0;

	mScaling				= Vector3::cIdentity;
	mScalingDuration		= 0;
	mScalingTickcount		= 0;

	mRotation				= Vector3::cOrigin;
	mRotationDuration		= 0;
	mRotationTickcount		= 0;
}

_void Matrix4Adjustor::Translation( _float x, _float y, _float z, _dword duration )
{
	Translation( Vector3( x, y, z ), duration );
}

_void Matrix4Adjustor::Translation( const Vector3& vector, _dword duration )
{
	mTranslation = vector;

	mTranslationDuration  = duration;
	mTranslationTickcount = 0;
}

_void Matrix4Adjustor::Scaling( _float x, _float y, _float z, _dword duration )
{
	Scaling( Vector3( x, y, z ), duration );
}

_void Matrix4Adjustor::Scaling( const Vector3& vector, _dword duration )
{
	mScaling = vector;

	mScalingDuration  = duration;
	mScalingTickcount = 0;
}

_void Matrix4Adjustor::Rotation( _float x, _float y, _float z, _dword duration )
{
	Rotation( Vector3( x, y, z ), duration );
}

_void Matrix4Adjustor::Rotation( const Vector3& vector, _dword duration )
{
	mRotation = vector;

	mRotationDuration  = duration;
	mRotationTickcount = 0;
}

_void Matrix4Adjustor::Update( _dword elapse )
{
	if ( mTranslationDuration > 0 )
	{
		mTranslationTickcount += elapse;

		if ( mTranslationTickcount >= mTranslationDuration )
		{
			mTranslation			= Vector3::cOrigin;
			mTranslationDuration	= 0;
			mTranslationTickcount	= 0;
		}
	}

	if ( mScalingDuration > 0 )
	{
		mScalingTickcount += elapse;

		if ( mScalingTickcount >= mScalingDuration )
		{
			mScaling				= Vector3::cIdentity;
			mScalingDuration		= 0;
			mScalingTickcount		= 0;
		}
	}

	if ( mRotationDuration > 0 )
	{
		mRotationTickcount += elapse;

		if ( mRotationTickcount >= mRotationDuration )
		{
			mRotation				= Vector3::cOrigin;
			mRotationDuration		= 0;
			mRotationTickcount		= 0;
		}
	}
}

Matrix4 Matrix4Adjustor::AdjustMatrix( const Matrix4& matrix ) const
{
	if ( mTranslationTickcount == mTranslationDuration && mScalingTickcount == mScalingDuration && mRotationTickcount == mRotationDuration )
		return matrix;

	_float translationalpha	= GetTranslationProgress( );
	_float scalingalpha		= GetScalingProgress( );
	_float rotationalpha	= GetRotationProgress( );

	Matrix4 result = matrix;

	// Scaling.
	if ( scalingalpha > 0.0f )
	{
		scalingalpha *= scalingalpha;

		result = Matrix4( ).Scaling( mScaling * scalingalpha ) * result;
	}

	// Rotation.
	if ( rotationalpha > 0.0f )
	{
		rotationalpha *= rotationalpha;

		if ( mRotation.x != 0.0f )
			result = Matrix4( ).RotationZ( mRotation.x * rotationalpha ) * result;

		if ( mRotation.y != 0.0f )
			result = Matrix4( ).RotationY( mRotation.y * rotationalpha ) * result;

		if ( mRotation.z != 0.0f )
			result = Matrix4( ).RotationZ( mRotation.z * rotationalpha ) * result;
	}

	// Translation.
	if ( translationalpha > 0.0f )
	{
		translationalpha *= translationalpha;

		result( 3, 0 ) += mTranslation.x * translationalpha;
		result( 3, 1 ) += mTranslation.y * translationalpha;
		result( 3, 2 ) += mTranslation.z * translationalpha;
	}

	return result;
}