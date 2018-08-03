//============================================================================
// Particle.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Particle
//----------------------------------------------------------------------------

struct Particle
{
	_dword				mParticleType;
	_dword				mInternalFlags;

	_dword				mCurrTickcount;
	_dword				mLastTickcount;

	_dword				mDelay;
	_dword				mLife;
	_dword				mColor;
	_float				mSpeed;
	_float				mSpin;
	Vector3				mPosition;
	Vector3				mDirection;
	Vector3				mNormal;
	Vector3				mSize;
	Vector3				mUserRotation;

	Quaternion			mEmitWorldRotation;

	// For updater
	_float				mEmitSpeed;
	_float				mEmitSpin;
	_dword				mEmitColor;
	Vector3				mEmitSize;

	// For texture frame
	_dword				mCurrColumn;
	_dword				mCurrRow;
	_dword				mLastFrameTickcount;
	
	_dword				mStopTickCount;

	_dword				mPrevParticleID;

	ITexture*			mTexture;
	
	ISkeleton*			mSkeleton;
	IMesh*				mMesh;
	ISkeletonAnimation*	mSkeletonAnimation;	
};

};