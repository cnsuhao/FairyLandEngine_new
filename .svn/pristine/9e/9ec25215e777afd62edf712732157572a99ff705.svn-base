//============================================================================
// ParticleBlocker.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// ParticleBlocker Implementation
//----------------------------------------------------------------------------

ParticleBlocker::ParticleBlocker( _dword blockerid ) : mBlockerID( blockerid ), mParticleSystem( _null )
{
}

_void ParticleBlocker::BlockParticle( Particle& particle, const Vector3& oldposition, _dword transonemit )
{
	if ( mPlane.Distance( particle.mPosition ) * mPlane.Distance( oldposition ) > 0.0f )
		return;

	switch ( mFunction )
	{
		case IParticleSystem::_BLOCK_VANISH:
		{
			particle.mCurrTickcount = particle.mLife;

			break;
		}

		case IParticleSystem::_BLOCK_STOP:
		{
			particle.mDirection = Vector3::cOrigin;

			break;
		}

		case IParticleSystem::_BLOCK_REBOUND:
		{
			particle.mPosition	= oldposition;
			particle.mDirection = mPlane.Reflect( particle.mDirection ) * mParameter;

			break;
		}
	}
}