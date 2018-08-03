//============================================================================
// ParticleFactory.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ParticleFactory
//----------------------------------------------------------------------------

class ParticleFactory : public IParticleFactory
{
private:
	_dword		mParticleGroup;

	static _bool OnReleaseParticleSystem( _void* buffer, _bool uselessonly = _false, _bool deleteobject = _true );

public:
	ParticleFactory( );
	~ParticleFactory( );

	virtual IParticleSystem*	CreateParticleSystem( );
	virtual IParticleSystem*	CreateParticleSystem( StringPtr filename );
	virtual IParticleSystem*	CloneParticleSystem( IParticleSystem* particlesystem, _bool reference );
	virtual _void				ReleaseParticleSystem( IParticleSystem*& particlesystem );

	virtual IParticlePlayer*	CreateParticlePlayer( );
	virtual IParticlePlayer*	CloneParticlePlayer( IParticlePlayer* particleplayer, _bool reference );
	virtual _void				ReleaseParticlePlayer( IParticlePlayer*& particleplayer );
};

};