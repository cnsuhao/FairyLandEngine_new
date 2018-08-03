//============================================================================
// ParticleGravity.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ParticleGravity
//----------------------------------------------------------------------------

class ParticleGravity
{
public:
	_dword			mGravityID;
	_dword			mGravityType;
	_bool			mEnabled;

	_float			mPower;
	_float			mDistance;
	Vector3			mPosition;
	Vector3			mDirection;

	ITagPoint*		mAttachedTagPoint;
	// get particle system
	ParticleSystem*	mParticleSystem;

public:
	ParticleGravity( _dword gravityid );
	~ParticleGravity( );

	_void AffectTagPoint( _dword elapse, _dword transonemit );
	_void AffectParticle( Particle& particle, _dword elapse, _dword transonemit );
};

};