//============================================================================
// ParticleUpdater.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ParticleUpdater
//----------------------------------------------------------------------------

class ParticleUpdater
{
public:
	typedef Pair< _float, _float >	Keyframe;
	typedef Array< Keyframe >		KeyframeArray;

	_dword			mUpdaterID;
	_dword			mUpdaterType;
	KeyframeArray	mKeyframeArray;
	_bool			mEnabled;

public:
	ParticleUpdater( _dword updaterid );
	
	_void UpdateParticle( Particle& particle, _float alpha );
};

};