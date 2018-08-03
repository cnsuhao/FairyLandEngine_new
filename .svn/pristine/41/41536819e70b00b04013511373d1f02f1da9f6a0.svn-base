//============================================================================
// SoundChannel.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// SoundChannel
//----------------------------------------------------------------------------

class SoundChannel : public ISoundChannel
{
private:
	FMOD_CHANNEL*	mFmodChannel;

public:
	SoundChannel( FMOD_CHANNEL* fmodchannel );
	~SoundChannel( );

	virtual _bool Stop( );

	virtual _bool IsPlaying( ) const;

	virtual _bool SetTime( _dword millisecond );
	virtual _bool GetTime( _dword& millisecond ) const;
	virtual _bool SetMute( _bool mute );
	virtual _bool GetMute( _bool& mute ) const;
	virtual _bool SetPaused( _bool paused );
	virtual _bool GetPaused( _bool& paused ) const;
	virtual _bool SetVolume( _float volume );
	virtual _bool GetVolume( _float& volume ) const;
	virtual _bool SetFrequency( _float frequency );
	virtual _bool GetFrequency( _float& frequency ) const;
	virtual _bool Set3DPosition( const Vector3& position );
	virtual _bool Get3DPosition( Vector3& position ) const;

	virtual ISoundBuffer* GetCurrentSound( ) const;
};

};