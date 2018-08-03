//============================================================================
// ISoundDevice.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ISoundDevice
//----------------------------------------------------------------------------

class ISoundDevice
{
public:
	enum _SOUND_FLAG
	{
		_SOUND_LOOP_DISABLE	= 0x00000001,
		_SOUND_LOOP_ENABLE	= 0x00000002,
		_SOUND_2D			= 0x00000008,
		_SOUND_3D			= 0x00000010,
		_SOUND_STREAM		= 0x00000020,
		_SOUND_FADEIN		= 0x00000100,
		_SOUND_FADEOUT		= 0x00000200,
		_SOUND_PLAYNOW		= 0x80000000,
	};

	typedef _void (*OnPlaySoundEnd)( StringPtr groupname, ISoundBuffer* soundbuffer );

public:
	virtual _void Release( )																													= 0;

	virtual _void Update( _dword elapse )																										= 0;

	virtual _bool Set3DSettings( _float dopplerscale, _float distancefactor, _float rolloffscale, _float mindistance, _float maxdistance )		= 0;
	virtual _bool Get3DSettings( _float& dopplerscale, _float& distancefactor, _float& rolloffscale, _float& mindistance, _float& maxdistance )	= 0;
	virtual _bool SetListenerPosition( const Vector3& position, const Vector3& forward, const Vector3& up )										= 0;
	virtual _bool GetListenerPosition( Vector3& position, Vector3& forward, Vector3& up )														= 0;

	virtual ISoundBuffer* CreateSoundBuffer( StringPtr filename, _dword flags = 0 )																= 0;
	virtual ISoundBuffer* CreateSoundBuffer( const MemFile& soundfile, _dword flags = 0 )														= 0;
	virtual ISoundBuffer* CloneSoundBuffer( ISoundBuffer* soundbuffer )																			= 0;
	virtual _void ReleaseSoundBuffer( ISoundBuffer*& soundbuffer )																				= 0;

	virtual _bool PlaySoundBuffer( ISoundBuffer* soundbuffer, ISoundChannel** soundchannel = _null, _dword flags = 0 )							= 0;
	virtual _bool PlaySoundBuffer( ISoundBuffer* soundbuffer, const Vector3& position, ISoundChannel** soundchannel = _null, _dword flags = 0 )	= 0;
	virtual _void ReleaseSoundChannel( ISoundChannel*& soundchannel )																			= 0;

	virtual _bool PlaySoundBuffer( StringPtr filename, _dword flags = 0 )																		= 0;
	virtual _bool PlaySoundBuffer( StringPtr filename, const Vector3& position, _dword flags = 0 )												= 0;

	virtual _bool SetCurrentSoundGroup( StringPtr groupname )																					= 0;
	virtual StringPtr GetCurrentSoundGroup( ) const																								= 0;

	virtual _bool StopSoundGroup( StringPtr groupname, _dword flag = 0 )																		= 0;
	virtual _bool StopAllSoundGroup( )																											= 0;

	virtual _bool SetGlobalMute( _bool mute )																									= 0;
	virtual _bool GetGlobalMute( ) const																										= 0;
	virtual _bool SetGlobalVolume( _float volume )																								= 0;
	virtual _float GetGlobalVolume( ) const																										= 0;

	virtual _bool SetSoundGroupMute( StringPtr groupname, _bool mute )																			= 0;
	virtual _bool SetSoundGroupVolume( StringPtr groupname, _float volume )																		= 0;
	virtual _float GetSoundGroupVolume( StringPtr groupname ) const																				= 0;

	virtual _void SetFadeInTime( _dword time )																									= 0;
	virtual _void SetFadeInDelay( _dword delay )																								= 0;
	virtual _void SetFadeOutTime( _dword time )																									= 0;
	virtual _void SetFadeOutDelay( _dword delay )																								= 0;

	virtual _dword GetPlayingChannelNumber( StringPtr groupname = L"" ) const																	= 0;
	virtual _dword GetMaxChannelNumber( ) const																									= 0;
};

};