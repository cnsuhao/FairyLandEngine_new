//============================================================================
// ParticleFactory.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// ParticleFactory Implementation
//----------------------------------------------------------------------------

ParticleFactory::ParticleFactory( )
{
	mParticleGroup = 0x80000000;
}

ParticleFactory::~ParticleFactory( )
{
}

_bool ParticleFactory::OnReleaseParticleSystem( _void* buffer, _bool uselessonly, _bool deleteobject )
{
	ParticleSystem* particlesystem = (ParticleSystem*) buffer;

	if ( uselessonly && particlesystem->GetRefCount( ) > 0 )
		return _false;

	// Decrease reference count.
	if ( particlesystem->DecreaseRefCount( ) == 0 && deleteobject )
	{
		delete particlesystem;
		return _true;
	}

	return _false;
}

IParticleSystem* ParticleFactory::CreateParticleSystem( )
{
	ParticleSystem* particlesystem = new ParticleSystem( L"" );

	return particlesystem;
}

IParticleSystem* ParticleFactory::CreateParticleSystem( StringPtr filename )
{
	// Search in resource cache first.
	if ( GetResourceCachePointer( ) != _null )
	{
		IParticleSystem* cachedparticlesystem = (IParticleSystem*) GetResourceCache( ).ObtainCache( filename, OnReleaseParticleSystem, _true );

		// Cache processmethod.
		if ( cachedparticlesystem != _null )
		{
			// Clone the cached particle system.
			IParticleSystem* newparticlesystem = CloneParticleSystem( cachedparticlesystem, _false );

			if ( newparticlesystem != _null )
				return newparticlesystem;
		}
	}

	TxtFile pfxfile;

	// Load particle system from resource manager.
	if ( GetResourceManagerPointer( ) != _null )
	{
		if ( GetResourceManagerPointer( )->LoadResource( filename, pfxfile ) == _false )
			return _null;
	}
	else
	{
		if ( pfxfile.Open( filename ) == _false )
			return _null;
	}

	ParticleSystem* particlesystem = new ParticleSystem( pfxfile.GetFileName( ) );

	if ( particlesystem->LoadParticleSystem( pfxfile, ++ mParticleGroup ) == _false )
	{
		delete particlesystem;
		return _null;
	}

	// Insert particle system into cache.
	if ( GetResourceCachePointer( ) != _null )
		GetResourceCachePointer( )->CreateCache( particlesystem->GetName( ), particlesystem, OnReleaseParticleSystem );

	return particlesystem;
}

IParticleSystem* ParticleFactory::CloneParticleSystem( IParticleSystem* particlesystem, _bool reference )
{
	if ( particlesystem == _null )
		return _null;

	if ( reference || ( (ParticleSystem*) particlesystem )->GetRefCount( ) == 0 )
	{
		// Reset for same ptr use again
		if ( ( (ParticleSystem*) particlesystem )->GetRefCount( ) == 0 )
			particlesystem->Reset( );

		// Increase reference count.
		( (ParticleSystem*) particlesystem )->IncreaseRefCount( );

		return particlesystem;
	}
	else
	{
		IParticleSystem* clonedparticlesystem = ( (ParticleSystem*) particlesystem )->Clone( ++ mParticleGroup );

		return clonedparticlesystem;
	}
}

_void ParticleFactory::ReleaseParticleSystem( IParticleSystem*& particlesystem )
{
	if ( particlesystem == _null )
		return;

	if ( ( (ParticleSystem*) particlesystem )->DecreaseRefCount( ) == 0 )
	{
		if ( GetResourceCachePointer( ) == _null || GetResourceCache( ).ReleaseCache( particlesystem->GetName( ), particlesystem ) == _false )
			OnReleaseParticleSystem( particlesystem );
	}

	particlesystem = _null;
}

IParticlePlayer* ParticleFactory::CreateParticlePlayer( )
{
	ParticlePlayer* particleplayer = new ParticlePlayer;

	return particleplayer;
}

IParticlePlayer* ParticleFactory::CloneParticlePlayer( IParticlePlayer* particleplayer, _bool reference )
{
	if ( particleplayer == _null )
		return _null;

	// Increase reference count.
	( (ParticlePlayer*) particleplayer )->IncreaseRefCount( );

	return particleplayer;
}

_void ParticleFactory::ReleaseParticlePlayer( IParticlePlayer*& particleplayer )
{
	if ( particleplayer == _null )
		return;

	if ( ( (ParticlePlayer*) particleplayer )->DecreaseRefCount( ) == 0 )
		delete (ParticlePlayer*) particleplayer;

	particleplayer = _null;
}