//============================================================================
// ParticleUpdater.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// ParticleUpdater Implementation
//----------------------------------------------------------------------------

ParticleUpdater::ParticleUpdater( _dword updaterid ) : mUpdaterID( updaterid ), mEnabled( _true )
{
}

_void ParticleUpdater::UpdateParticle( Particle& particle, _float alpha )
{
	if ( mKeyframeArray.Number( ) < 2 )
		return;

	_dword i = 0, j = mKeyframeArray.Number( ) - 1;

	if ( mKeyframeArray[i].mObject1 != 0.0f )
		return;

	if ( mKeyframeArray[j].mObject1 != 1.0f )
		return;

	// Find the keyframe by alpha.
	while ( i < j - 1 )
	{
		_dword m = ( i + j ) / 2;

		if ( alpha >= mKeyframeArray[m].mObject1 )
			i = m;
		else
			j = m;
	}

	_float alpha1 = mKeyframeArray[i].mObject1;
	_float value1 = mKeyframeArray[i].mObject2;
	_float alpha2 = mKeyframeArray[j].mObject1;
	_float value2 = mKeyframeArray[j].mObject2;

	_float factor = ( alpha - alpha1 ) / ( alpha2 - alpha1 );
	_float value  = value1 + ( value2 - value1 ) * factor;

	switch ( mUpdaterType )
	{
		case IParticleSystem::_UPDATER_SPEED:
		{
			particle.mSpeed = particle.mEmitSpeed * value;

			break;
		}

		case IParticleSystem::_UPDATER_SPIN:
		{
			particle.mSpin = particle.mEmitSpin * value;

			break;
		}

		case IParticleSystem::_UPDATER_COLOR_R:
		{
			value = Math::Clamp( value, 0.0f, 1.0f );

			Dword color( particle.mEmitColor );
			
			particle.mColor &= 0xFF00FFFF;
			particle.mColor |= ( (_dword) ( value * color.Byte3( ) ) ) << 16;

			break;
		}

		case IParticleSystem::_UPDATER_COLOR_G:
		{
			value = Math::Clamp( value, 0.0f, 1.0f );

			Dword color( particle.mEmitColor );

			particle.mColor &= 0xFFFF00FF;
			particle.mColor |= ( (_dword) ( value * color.Byte2( ) ) ) << 8;

			break;
		}

		case IParticleSystem::_UPDATER_COLOR_B:
		{
			value = Math::Clamp( value, 0.0f, 1.0f );

			Dword color( particle.mEmitColor );

			particle.mColor &= 0xFFFFFF00;
			particle.mColor |= ( (_dword) ( value * color.Byte1( ) ) );

			break;
		}

		case IParticleSystem::_UPDATER_COLOR_A:
		{
			value = Math::Clamp( value, 0.0f, 1.0f );

			Dword color( particle.mEmitColor );

			particle.mColor &= 0x00FFFFFF;
			particle.mColor |= ( (_dword) ( value * color.Byte4( ) ) ) << 24;

			break;
		}

		case IParticleSystem::_UPDATER_SIZE_X:
		{
			particle.mSize.x = particle.mEmitSize.x * value;

			break;
		}

		case IParticleSystem::_UPDATER_SIZE_Y:
		{
			particle.mSize.y = particle.mEmitSize.y * value;

			break;
		}

		case IParticleSystem::_UPDATER_SIZE_Z:
		{
			particle.mSize.z = particle.mEmitSize.z * value;

			break;
		}

		case IParticleSystem::_UPDATER_SIZE_XYZ:
		{
			particle.mSize.x = particle.mEmitSize.x * value;
			particle.mSize.y = particle.mEmitSize.y * value;
			particle.mSize.z = particle.mEmitSize.z * value;
			
			break;
		}
	}
}