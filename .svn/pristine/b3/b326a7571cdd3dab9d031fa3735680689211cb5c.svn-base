//============================================================================
// Matrix4Adjustor.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Matrix4Adjustor
//----------------------------------------------------------------------------

class Matrix4Adjustor
{
private:
	Vector3		mTranslation;
	_dword		mTranslationDuration;
	_dword		mTranslationTickcount;

	Vector3		mScaling;
	_dword		mScalingDuration;
	_dword		mScalingTickcount;

	Vector3		mRotation;
	_dword		mRotationDuration;
	_dword		mRotationTickcount;

public:
	Matrix4Adjustor( );

	inline const Vector3& GetTranslation( ) const;
	inline const Vector3& GetScaling( ) const;
	inline const Vector3& GetRotation( ) const;

	inline _float GetTranslationProgress( ) const;
	inline _float GetScalingProgress( ) const;
	inline _float GetRotationProgress( ) const;

	_void Translation( _float x, _float y, _float z, _dword duration );
	_void Translation( const Vector3& vector, _dword duration );

	_void Scaling( _float x, _float y, _float z, _dword duration );
	_void Scaling( const Vector3& vector, _dword duration );

	_void Rotation( _float x, _float y, _float z, _dword duration );
	_void Rotation( const Vector3& vector, _dword duration );

	_void Update( _dword elapse );

	Matrix4 AdjustMatrix( const Matrix4& matrix ) const;
};

//----------------------------------------------------------------------------
// Matrix4Adjustor Implementation
//----------------------------------------------------------------------------

const Vector3& Matrix4Adjustor::GetTranslation( ) const
{
	return mTranslation;
}

const Vector3& Matrix4Adjustor::GetScaling( ) const
{
	return mScaling;
}

const Vector3& Matrix4Adjustor::GetRotation( ) const
{
	return mRotation;
}

_float Matrix4Adjustor::GetTranslationProgress( ) const
{
	return mTranslationDuration == 0 ? 0.0f : 1.0f - (_float) mTranslationTickcount / (_float) mTranslationDuration;
}

_float Matrix4Adjustor::GetScalingProgress( ) const
{
	return mScalingDuration == 0 ? 0.0f : 1.0f - (_float) mScalingTickcount / (_float) mScalingDuration;
}

_float Matrix4Adjustor::GetRotationProgress( ) const
{
	return mRotationDuration == 0 ? 0.0f : 1.0f - (_float) mRotationTickcount / (_float) mRotationDuration;
}

}