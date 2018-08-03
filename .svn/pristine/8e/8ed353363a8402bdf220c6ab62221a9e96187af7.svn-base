//============================================================================
// ParticleEmitter.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ParticleEmitter
//----------------------------------------------------------------------------

class ParticleSystem;

class ParticleEmitter
{
public:
	enum { _MAX_NUMBER = 256 };

	typedef Pool< Particle > ParticlePool;

	_dword			mEmitterID;
	String			mName;
	_dword			mEmitterType;
	_dword			mDirectionType;
	_dword			mNormalType;
	_bool			mEnabled;

	_dword			mCurrTickcount;
	_dword			mLastTickcount;
	_dword			mLastParticleID;

	_dword			mEndTickcount;

	_dword			mParticleType;
	_dword			mMinParticleDelay;
	_dword			mMaxParticleDelay;
	_dword			mMinParticleLife;
	_dword			mMaxParticleLife;
	_float			mMinParticleSpeed;
	_float			mMaxParticleSpeed;
	Vector3			mParticleRotation;
	Vector3			mParticleRotateSpeed;
	_float			mMinParticleSpin;
	_float			mMaxParticleSpin;
	_bool			mFixParticleColorRate;
	Color			mMinParticleColor;
	Color			mMaxParticleColor;
	_bool			mFixParticleSizeRate;
	Vector3			mMinParticleSize;
	Vector3			mMaxParticleSize;
	_dword			mParticleBindEmitterID;
	_dword			mParticleBindGravityID;

	_dword			mEmitRate;
	_dword			mEmitDelay;
	_dword			mEmitNumber;
	Vector3			mSizeInner;
	Vector3			mSizeOuter;
	Vector3			mPosition;
	EulerAngles		mRotation;
	Matrix4			mRotationMatrix;

	IRenderMethod*	mRenderMethod;
	ITagPoint*		mAttachedTagPoint[2];
	IMesh*			mAttachedMesh;
	Geometry		mAttachedGeometry;
	ISkeleton*		mAttachedSkeleton;

	Vector3			mLastEmitPosition;
	_float			mEmitByDistance;

	Array< _dword >	mGravityArray;
	Array< _dword >	mUpdaterArray;
	Array< _dword >	mBlockerArray;

	ParticlePool	mParticlePool;

	// bind name
	String			mBindName[2];
	// Trail UV
	_float			mLength;
	// Frame texture
	_dword			mRandomFrame;
	_dword			mColumn;
	_dword			mRow;
	_dword			mTime;
	_dword			mCurrRow;
	_dword			mCurrColumn;
	_dword			mLastFrameTickcount;
	// Emitter speed
	_float			mSpeed;
	// Emitter scale
	_float			mScale;
	// geometry as a particle
	Geometry		mGeometry;
	// get particle system
	ParticleSystem*	mParticleSystem;
	// 0 use transform on rend it
	// 1 use transform on emit it
	_dword			mEmitWorldtransform;
	// 0 not use marker transform
	// 1 use marker transform
	_dword			mMarkerTransform;
	// 0 strip extend from middle to two side
	// 1 strip extend from point1 to up 
	// 2 strip extend from point1 to down
	// 3 strip extend from point2 to up 
	// 4 strip extend from point2 to down
	_dword			mStripExtend;

	ITexture*				mTexture;
	
	ISkeleton*				mSkeleton;
	IMesh*					mMesh;
	ISkeletonAnimation*		mSkeletonAnimation;	

	_void SetParticleProperty( Particle& particle );
	_void SetParticlePosition( Particle& particle );
	_void SetParticleDirection( Particle& particle );

public:
	ParticleEmitter( _dword emitterid );
	~ParticleEmitter( );

	_dword GetAvailableParticleNumber( );
	_void  CreateParticle( Particle& particle );
};

};