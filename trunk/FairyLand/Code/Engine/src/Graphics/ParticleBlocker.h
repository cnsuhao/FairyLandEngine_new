//============================================================================
// ParticleBlocker.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ParticleBlocker
//----------------------------------------------------------------------------

class ParticleSystem;

class ParticleBlocker
{
public:
	_dword			mBlockerID;
	_dword			mBlockerType;
	_dword			mFunction;
	_float			mParameter;
	Plane			mPlane;
	// get particle system
	ParticleSystem*	mParticleSystem;

public:
	ParticleBlocker( _dword blockerid );

	_void BlockParticle( Particle& particle, const Vector3& oldposition, _dword transonemit );
};

};