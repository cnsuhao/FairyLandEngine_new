//============================================================================
// ISoundChannel.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ISoundChannel
//----------------------------------------------------------------------------

class ISoundChannel
{
public:
	virtual _bool Stop( )										= 0;

	virtual _bool IsPlaying( ) const							= 0;

	virtual _bool SetTime( _dword millisecond )					= 0;
	virtual _bool GetTime( _dword& millisecond ) const			= 0;
	virtual _bool SetMute( _bool mute )							= 0;
	virtual _bool GetMute( _bool& mute ) const					= 0;
	virtual _bool SetPaused( _bool paused )						= 0;
	virtual _bool GetPaused( _bool& paused ) const				= 0;
	virtual _bool SetVolume( _float volume )					= 0;
	virtual _bool GetVolume( _float& volume ) const				= 0;
	virtual _bool SetFrequency( _float frequency )				= 0;
	virtual _bool GetFrequency( _float& frequency ) const		= 0;
	virtual _bool Set3DPosition( const Vector3& position )		= 0;
	virtual _bool Get3DPosition( Vector3& position ) const		= 0;

	virtual ISoundBuffer* GetCurrentSound( ) const				= 0;
};

};