//============================================================================
// SoundChannel.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandSound.h"

//----------------------------------------------------------------------------
// SoundChannel Implementation
//----------------------------------------------------------------------------

SoundChannel::SoundChannel( FMOD_CHANNEL* fmodchannel ) : mFmodChannel( fmodchannel )
{
}

SoundChannel::~SoundChannel( )
{
}

_bool SoundChannel::Stop( )
{
	_void* userdata = _null;

	if ( SoundAPI::_FMODGetChannelUserData( mFmodChannel, &userdata ) == 0 && userdata != _null )
	{
		ISoundBuffer* soundbuffer = (ISoundBuffer*) userdata;

		// Release the sound buffer.
		if ( soundbuffer != _null )
			GetSoundDevicePointer( )->ReleaseSoundBuffer( soundbuffer );

		SoundAPI::_FMODSetChannelUserData( mFmodChannel, _null );
	}

	return SoundAPI::_FMODStopChannel( mFmodChannel ) == 0;
}

_bool SoundChannel::IsPlaying( ) const
{
	_bool playing = _false;

	SoundAPI::_FMODIsChannelPlaying( mFmodChannel, &playing );

	return playing;
}

_bool SoundChannel::SetTime( _dword millisecond )
{
	return SoundAPI::_FMODSetChannelPosition( mFmodChannel, millisecond, FMOD_TIMEUNIT_MS ) == 0;
}

_bool SoundChannel::GetTime( _dword& millisecond ) const
{
	return SoundAPI::_FMODGetChannelPosition( mFmodChannel, &millisecond, FMOD_TIMEUNIT_MS ) == 0;
}

_bool SoundChannel::SetMute( _bool mute )
{
	return SoundAPI::_FMODSetChannelMute( mFmodChannel, mute ) == 0;
}

_bool SoundChannel::GetMute( _bool& mute ) const
{
	return SoundAPI::_FMODGetChannelMute( mFmodChannel, &mute ) == 0;
}

_bool SoundChannel::SetPaused( _bool paused )
{
	return SoundAPI::_FMODSetChannelPaused( mFmodChannel, paused ) == 0;
}

_bool SoundChannel::GetPaused( _bool& paused ) const
{
	return SoundAPI::_FMODGetChannelPaused( mFmodChannel, &paused ) == 0;
}

_bool SoundChannel::SetVolume( _float volume )
{
	if ( volume < 0.0f )
		volume = 0.0f;

	return SoundAPI::_FMODSetChannelVolume( mFmodChannel, volume ) == 0;
}

_bool SoundChannel::GetVolume( _float& volume ) const
{
	return SoundAPI::_FMODGetChannelVolume( mFmodChannel, &volume ) == 0;
}

_bool SoundChannel::SetFrequency( _float frequency )
{
	if ( frequency < 0.0f )
		frequency = 0.0f;

	return SoundAPI::_FMODSetChannelFrequency( mFmodChannel, frequency ) == 0;
}

_bool SoundChannel::GetFrequency( _float& frequency ) const
{
	return SoundAPI::_FMODGetChannelFrequency( mFmodChannel, &frequency ) == 0;
}

_bool SoundChannel::Set3DPosition( const Vector3& position )
{
	Vector3 velocity = Vector3::cOrigin;

	return SoundAPI::_FMODSetChannel3DAttributes( mFmodChannel, &position, &velocity ) == 0;
}

_bool SoundChannel::Get3DPosition( Vector3& position ) const
{
	Vector3 velocity = Vector3::cOrigin;

	return SoundAPI::_FMODGetChannel3DAttributes( mFmodChannel, &position, &velocity ) == 0;
}

ISoundBuffer* SoundChannel::GetCurrentSound( ) const
{
	return _null;

	//FMOD_SOUND* sound = _null;

	//if ( SoundAPI::_FMODGetChannelCurrentSound( mFmodChannel, &sound ) != 0 )
	//	return _null;

	//ISoundBuffer* soundbuffer = _null;

	//if ( SoundAPI::_FMODGetSoundUserData( sound, (_void**) &soundbuffer ) != 0 )
	//	return _null;

	//return soundbuffer;
}