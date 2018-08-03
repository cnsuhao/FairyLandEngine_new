//============================================================================
// SoundBuffer.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandSound.h"

//----------------------------------------------------------------------------
// SoundBuffer Implementation
//----------------------------------------------------------------------------

SoundBuffer::SoundBuffer( StringPtr name, FMOD_SOUND* fmodsound ) : mName( name ), mFmodSound( fmodsound )
{
	//SoundAPI::_FMODSetSoundUserData( mFmodSound, this );

	IncreaseRefCount( );
}

SoundBuffer::~SoundBuffer( )
{
	SoundAPI::_FMODReleaseSound( mFmodSound );
}

StringPtr SoundBuffer::GetName( ) const
{
	return mName;
}

_bool SoundBuffer::EnableLoop( _bool loop )
{
	_dword mode = 0;

	if ( SoundAPI::_FMODGetSoundMode( mFmodSound, &mode ) != 0 )
		return _false;

	if ( loop == _true )
	{
		mode &= ~FMOD_LOOP_OFF;
		mode |=  FMOD_LOOP_NORMAL;
	}
	else
	{
		mode &= ~FMOD_LOOP_NORMAL;
		mode |=  FMOD_LOOP_OFF;
	}

	if ( SoundAPI::_FMODSetSoundMode( mFmodSound, mode ) != 0 )
		return _false;

	return _true;
}

_bool SoundBuffer::GetLength( _dword& millisecond )
{
	return SoundAPI::_FMODGetSoundLength( mFmodSound, &millisecond, FMOD_TIMEUNIT_MS ) == 0;
}

_bool SoundBuffer::Set3DDistance( _float min, _float max )
{
	return SoundAPI::_FMODSetSound3DDistance( mFmodSound, min, max ) == 0;
}

_bool SoundBuffer::Get3DDistance( _float& min, _float& max )
{
	return SoundAPI::_FMODGetSound3DDistance( mFmodSound, &min, &max ) == 0;
}