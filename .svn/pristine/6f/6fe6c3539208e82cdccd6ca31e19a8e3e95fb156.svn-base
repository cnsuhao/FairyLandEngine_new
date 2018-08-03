//============================================================================
// IParticleFactory.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

namespace FL
{

//----------------------------------------------------------------------------
// IParticleFactory
//----------------------------------------------------------------------------

class IParticleFactory
{
public:
	virtual IParticleSystem*	CreateParticleSystem( ) = 0;
	virtual IParticleSystem*	CreateParticleSystem( StringPtr filename ) = 0;
	virtual IParticleSystem*	CloneParticleSystem( IParticleSystem* particlesystem, _bool reference = _true ) = 0;
	virtual _void				ReleaseParticleSystem( IParticleSystem*& particlesystem ) = 0;

	virtual IParticlePlayer*	CreateParticlePlayer( ) = 0;
	virtual IParticlePlayer*	CloneParticlePlayer( IParticlePlayer* particleplayer, _bool reference = _true ) = 0;
	virtual _void				ReleaseParticlePlayer( IParticlePlayer*& particleplayer ) = 0;
};

};