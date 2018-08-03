#include "FairyLandSound.h"

//----------------------------------------------------------------------------
// SoundAPI Implementation
//----------------------------------------------------------------------------

SoundAPI::_FMODCreateSystemFunc				SoundAPI::sCreateSystemFunc				= _null;
SoundAPI::_FMODCloseSystemFunc				SoundAPI::sCloseSystemFunc				= _null;
SoundAPI::_FMODInitSystemFunc				SoundAPI::sInitSystemFunc				= _null;
SoundAPI::_FMODSetFileSystemFunc			SoundAPI::sSetFileSystemFunc			= _null;
SoundAPI::_FMODReleaseSystemFunc			SoundAPI::sReleaseSystemFunc			= _null;
SoundAPI::_FMODUpdateSystemFunc				SoundAPI::sUpdateSystemFunc				= _null;
SoundAPI::_FMODSet3DSettingsFunc			SoundAPI::sSet3DSettingsFunc			= _null;
SoundAPI::_FMODGet3DSettingsFunc			SoundAPI::sGet3DSettingsFunc			= _null;
SoundAPI::_FMODSetListenerPositionFunc		SoundAPI::sSetListenerPositionFunc		= _null;
SoundAPI::_FMODGetListenerPositionFunc		SoundAPI::sGetListenerPositionFunc		= _null;
SoundAPI::_FMODGetPlayingChannelNumberFunc	SoundAPI::sGetPlayingChannelNumberFunc	= _null;
SoundAPI::_FMODGetChannelFunc				SoundAPI::sGetChannelFunc				= _null;

SoundAPI::_FMODCreateSoundFunc				SoundAPI::sCreateSoundFunc				= _null;
SoundAPI::_FMODReleaseSoundFunc				SoundAPI::sReleaseSoundFunc				= _null;
SoundAPI::_FMODPlaySoundFunc				SoundAPI::sPlaySoundFunc				= _null;
SoundAPI::_FMODGetSoundLengthFunc			SoundAPI::sGetSoundLengthFunc			= _null;
SoundAPI::_FMODSetSoundModeFunc				SoundAPI::sSetSoundModeFunc				= _null;
SoundAPI::_FMODGetSoundModeFunc				SoundAPI::sGetSoundModeFunc				= _null;
SoundAPI::_FMODSetSound3DDistanceFunc		SoundAPI::sSetSound3DDistanceFunc		= _null;
SoundAPI::_FMODGetSound3DDistanceFunc		SoundAPI::sGetSound3DDistanceFunc		= _null;
SoundAPI::_FMODSetSoundUserDataFunc			SoundAPI::sSetSoundUserDataFunc			= _null;
SoundAPI::_FMODGetSoundUserDataFunc			SoundAPI::sGetSoundUserDataFunc			= _null;

SoundAPI::_FMODStopChannelFunc				SoundAPI::sStopChannelFunc				= _null;
SoundAPI::_FMODIsChannelPlayingFunc			SoundAPI::sIsChannelPlayingFunc			= _null;
SoundAPI::_FMODSetChannelPositionFunc		SoundAPI::sSetChannelPosition			= _null;
SoundAPI::_FMODGetChannelPositionFunc		SoundAPI::sGetChannelPosition			= _null;
SoundAPI::_FMODSetChannelMuteFunc			SoundAPI::sSetChannelMuteFunc			= _null;
SoundAPI::_FMODGetChannelMuteFunc			SoundAPI::sGetChannelMuteFunc			= _null;
SoundAPI::_FMODSetChannelPausedFunc			SoundAPI::sSetChannelPausedFunc			= _null;
SoundAPI::_FMODGetChannelPausedFunc			SoundAPI::sGetChannelPausedFunc			= _null;
SoundAPI::_FMODSetChannelVolumeFunc			SoundAPI::sSetChannelVolumeFunc			= _null;
SoundAPI::_FMODGetChannelVolumeFunc			SoundAPI::sGetChannelVolumeFunc			= _null;
SoundAPI::_FMODSetChannelFrequencyFunc		SoundAPI::sSetChannelFrequencyFunc		= _null;
SoundAPI::_FMODGetChannelFrequencyFunc		SoundAPI::sGetChannelFrequencyFunc		= _null;
SoundAPI::_FMODSetChannel3DAttributesFunc	SoundAPI::sSetChannel3DAttributesFunc	= _null;
SoundAPI::_FMODGetChannel3DAttributesFunc	SoundAPI::sGetChannel3DAttributesFunc	= _null;
SoundAPI::_FMODSetChannelGroupFunc			SoundAPI::sSetChannelGroupFunc			= _null;
SoundAPI::_FMODGetChannelGroupFunc			SoundAPI::sGetChannelGroupFunc			= _null;
SoundAPI::_FMODSetChannelUserDataFunc		SoundAPI::sSetChannelUserDataFunc		= _null;
SoundAPI::_FMODGetChannelUserDataFunc		SoundAPI::sGetChannelUserDataFunc		= _null;
SoundAPI::_FMODGetChannelCurrentSoundFunc	SoundAPI::sGetChannelCurrentSoundFunc	= _null;

SoundAPI::_FMODGetMasterChannelGroupFunc	SoundAPI::sGetMasterChannelGroupFunc	= _null;
SoundAPI::_FMODCreateChannelGroupFunc		SoundAPI::sCreateChannelGroupFunc		= _null;
SoundAPI::_FMODReleaseChannelGroupFunc		SoundAPI::sReleaseChannelGroupFunc		= _null;
SoundAPI::_FMODGetChannelGroupNameFunc		SoundAPI::sGetChannelGroupNameFunc		= _null;
SoundAPI::_FMODSetChannelGroupVolumeFunc	SoundAPI::sSetChannelGroupVolumeFunc	= _null;
SoundAPI::_FMODGetChannelGroupVolumeFunc	SoundAPI::sGetChannelGroupVolumeFunc	= _null;
SoundAPI::_FMODOverrideChannelGroupMuteFunc	SoundAPI::sOverrideChannelGroupMuteFunc	= _null;
SoundAPI::_FMODGetChannelNumberInGroupFunc	SoundAPI::sGetChannelNumberInGroupFunc	= _null;
SoundAPI::_FMODGetChannelInGroupFunc		SoundAPI::sGetChannelInGroupFunc		= _null;
SoundAPI::_FMODStopChannelGroupFunc			SoundAPI::sStopChannelGroupFunc			= _null;

Library	SoundAPI::sFmodLibrary;
Lock	SoundAPI::sFmodLock;

_bool SoundAPI::LoadFmod( _handle module )
{
	LockOwner lock( sFmodLock );

	if ( sFmodLibrary.GetModule( ) != _null )
		return _true;

	// Find fmod dll resource.
	_handle resinfo = ::FindResource( (HMODULE) module, L"SOUNDMODULE", L"FAIRYLANDRES" );
	if ( resinfo == _null )
		return _false;

	// Get the size of the resource.
	_dword size = ::SizeofResource( (HMODULE) module, (HRSRC) resinfo );
	if ( size == 0 )
		return _false;

	// Load the resource into memory.
	_handle resource = ::LoadResource( (HMODULE) module, (HRSRC) resinfo );
	if ( resource == _null )
		return _false;

	if ( sFmodLibrary.Load( (const _byte*) resource ) == _false )
		return _false;

	sCreateSystemFunc				= (_FMODCreateSystemFunc)				sFmodLibrary.GetFunction( L"FMOD_System_Create" );
	sCloseSystemFunc				= (_FMODCloseSystemFunc)				sFmodLibrary.GetFunction( L"FMOD_System_Close" );
	sInitSystemFunc					= (_FMODInitSystemFunc)					sFmodLibrary.GetFunction( L"FMOD_System_Init" );
	sSetFileSystemFunc				= (_FMODSetFileSystemFunc)				sFmodLibrary.GetFunction( L"FMOD_System_SetFileSystem" );
	sReleaseSystemFunc				= (_FMODReleaseSystemFunc)				sFmodLibrary.GetFunction( L"FMOD_System_Release" );
	sUpdateSystemFunc				= (_FMODUpdateSystemFunc)				sFmodLibrary.GetFunction( L"FMOD_System_Update" );
	sSet3DSettingsFunc				= (_FMODSet3DSettingsFunc)				sFmodLibrary.GetFunction( L"FMOD_System_Set3DSettings" );
	sGet3DSettingsFunc				= (_FMODGet3DSettingsFunc)				sFmodLibrary.GetFunction( L"FMOD_System_Get3DSettings" );
	sSetListenerPositionFunc		= (_FMODSetListenerPositionFunc)		sFmodLibrary.GetFunction( L"FMOD_System_Set3DListenerAttributes" );
	sGetListenerPositionFunc		= (_FMODGetListenerPositionFunc)		sFmodLibrary.GetFunction( L"FMOD_System_Get3DListenerAttributes" );
	sGetPlayingChannelNumberFunc	= (_FMODGetPlayingChannelNumberFunc)	sFmodLibrary.GetFunction( L"FMOD_System_GetChannelsPlaying" );
	sGetChannelFunc					= (_FMODGetChannelFunc)					sFmodLibrary.GetFunction( L"FMOD_System_GetChannel" );
	sCreateSoundFunc				= (_FMODCreateSoundFunc)				sFmodLibrary.GetFunction( L"FMOD_System_CreateSound" );
	sReleaseSoundFunc				= (_FMODReleaseSoundFunc)				sFmodLibrary.GetFunction( L"FMOD_Sound_Release" );
	sPlaySoundFunc					= (_FMODPlaySoundFunc)					sFmodLibrary.GetFunction( L"FMOD_System_PlaySound" );
	sGetSoundLengthFunc				= (_FMODGetSoundLengthFunc)				sFmodLibrary.GetFunction( L"FMOD_Sound_GetLength" );
	sSetSoundModeFunc				= (_FMODSetSoundModeFunc)				sFmodLibrary.GetFunction( L"FMOD_Sound_SetMode" );
	sGetSoundModeFunc				= (_FMODGetSoundModeFunc)				sFmodLibrary.GetFunction( L"FMOD_Sound_GetMode" );
	sSetSound3DDistanceFunc			= (_FMODSetSound3DDistanceFunc)			sFmodLibrary.GetFunction( L"FMOD_Sound_Set3DMinMaxDistance" );
	sGetSound3DDistanceFunc			= (_FMODGetSound3DDistanceFunc)			sFmodLibrary.GetFunction( L"FMOD_Sound_Get3DMinMaxDistance" );
	sSetSoundUserDataFunc			= (_FMODSetSoundUserDataFunc)			sFmodLibrary.GetFunction( L"FMOD_Sound_SetUserData" );
	sGetSoundUserDataFunc			= (_FMODGetSoundUserDataFunc)			sFmodLibrary.GetFunction( L"FMOD_Sound_GetUserData" );
	sStopChannelFunc				= (_FMODStopChannelFunc)				sFmodLibrary.GetFunction( L"FMOD_Channel_Stop" );
	sIsChannelPlayingFunc			= (_FMODIsChannelPlayingFunc)			sFmodLibrary.GetFunction( L"FMOD_Channel_IsPlaying" );
	sSetChannelPosition				= (_FMODSetChannelPositionFunc)			sFmodLibrary.GetFunction( L"FMOD_Channel_SetPosition" );
	sGetChannelPosition				= (_FMODGetChannelPositionFunc)			sFmodLibrary.GetFunction( L"FMOD_Channel_GetPosition" );
	sSetChannelMuteFunc				= (_FMODSetChannelMuteFunc)				sFmodLibrary.GetFunction( L"FMOD_Channel_SetMute" );
	sGetChannelMuteFunc				= (_FMODGetChannelMuteFunc)				sFmodLibrary.GetFunction( L"FMOD_Channel_GetMute" );
	sSetChannelPausedFunc			= (_FMODSetChannelPausedFunc)			sFmodLibrary.GetFunction( L"FMOD_Channel_SetPaused" );
	sGetChannelPausedFunc			= (_FMODGetChannelPausedFunc)			sFmodLibrary.GetFunction( L"FMOD_Channel_GetPaused" );
	sSetChannelVolumeFunc			= (_FMODSetChannelVolumeFunc)			sFmodLibrary.GetFunction( L"FMOD_Channel_SetVolume" );
	sGetChannelVolumeFunc			= (_FMODGetChannelVolumeFunc)			sFmodLibrary.GetFunction( L"FMOD_Channel_GetVolume" );
	sSetChannelFrequencyFunc		= (_FMODSetChannelFrequencyFunc)		sFmodLibrary.GetFunction( L"FMOD_Channel_SetFrequency" );
	sGetChannelFrequencyFunc		= (_FMODGetChannelFrequencyFunc)		sFmodLibrary.GetFunction( L"FMOD_Channel_GetFrequency" );
	sSetChannel3DAttributesFunc		= (_FMODSetChannel3DAttributesFunc)		sFmodLibrary.GetFunction( L"FMOD_Channel_Set3DAttributes" );
	sGetChannel3DAttributesFunc		= (_FMODGetChannel3DAttributesFunc)		sFmodLibrary.GetFunction( L"FMOD_Channel_Get3DAttributes" );
	sSetChannelGroupFunc			= (_FMODSetChannelGroupFunc)			sFmodLibrary.GetFunction( L"FMOD_Channel_SetChannelGroup" );
	sGetChannelGroupFunc			= (_FMODGetChannelGroupFunc)			sFmodLibrary.GetFunction( L"FMOD_Channel_GetChannelGroup" );
	sSetChannelUserDataFunc			= (_FMODSetChannelUserDataFunc)			sFmodLibrary.GetFunction( L"FMOD_Channel_SetUserData" );
	sGetChannelUserDataFunc			= (_FMODGetChannelUserDataFunc)			sFmodLibrary.GetFunction( L"FMOD_Channel_GetUserData" );
	sGetChannelCurrentSoundFunc		= (_FMODGetChannelCurrentSoundFunc)		sFmodLibrary.GetFunction( L"FMOD_Channel_GetCurrentSound" );
	sGetMasterChannelGroupFunc		= (_FMODGetMasterChannelGroupFunc)		sFmodLibrary.GetFunction( L"FMOD_System_GetMasterChannelGroup" );
	sCreateChannelGroupFunc			= (_FMODCreateChannelGroupFunc)			sFmodLibrary.GetFunction( L"FMOD_System_CreateChannelGroup" );
	sReleaseChannelGroupFunc		= (_FMODReleaseChannelGroupFunc)		sFmodLibrary.GetFunction( L"FMOD_ChannelGroup_Release" );
	sGetChannelGroupNameFunc		= (_FMODGetChannelGroupNameFunc)		sFmodLibrary.GetFunction( L"FMOD_ChannelGroup_GetName" );
	sSetChannelGroupVolumeFunc		= (_FMODSetChannelGroupVolumeFunc)		sFmodLibrary.GetFunction( L"FMOD_ChannelGroup_SetVolume" );
	sGetChannelGroupVolumeFunc		= (_FMODGetChannelGroupVolumeFunc)		sFmodLibrary.GetFunction( L"FMOD_ChannelGroup_GetVolume" );
	sOverrideChannelGroupMuteFunc	= (_FMODOverrideChannelGroupMuteFunc)	sFmodLibrary.GetFunction( L"FMOD_ChannelGroup_OverrideMute" );
	sGetChannelNumberInGroupFunc	= (_FMODGetChannelNumberInGroupFunc)	sFmodLibrary.GetFunction( L"FMOD_ChannelGroup_GetNumChannels" );
	sGetChannelInGroupFunc			= (_FMODGetChannelInGroupFunc)			sFmodLibrary.GetFunction( L"FMOD_ChannelGroup_GetChannel" );
	sStopChannelGroupFunc			= (_FMODStopChannelGroupFunc)			sFmodLibrary.GetFunction( L"FMOD_ChannelGroup_Stop" );

	return _true;
}

_long SoundAPI::_FMODCreateSystem( FMOD_SYSTEM** fmodsystem )
{
	if ( sCreateSystemFunc == _null )
		return -1;

	return (*sCreateSystemFunc)( fmodsystem );
}

_long SoundAPI::_FMODCloseSystem( FMOD_SYSTEM* fmodsystem )
{
	if ( sCloseSystemFunc == _null )
		return -1;

	return (*sCloseSystemFunc)( fmodsystem );
}

_long SoundAPI::_FMODInitSystem( FMOD_SYSTEM* fmodsystem, _dword maxchannels, _dword flags, _void* driverdata )
{
	if ( sInitSystemFunc == _null )
		return -1;

	return (*sInitSystemFunc)( fmodsystem, maxchannels, flags, driverdata );
}

_long SoundAPI::_FMODSetFileSystem( FMOD_SYSTEM* fmodsystem, FMOD_FILE_OPENCALLBACK useropen, FMOD_FILE_CLOSECALLBACK userclose, FMOD_FILE_READCALLBACK userread, FMOD_FILE_SEEKCALLBACK userseek, _dword blocksize )
{
	if ( sSetFileSystemFunc == _null )
		return -1;

	return (*sSetFileSystemFunc)( fmodsystem, useropen, userclose, userread, userseek, blocksize );
}

_long SoundAPI::_FMODReleaseSystem( FMOD_SYSTEM* fmodsystem )
{
	if ( sReleaseSystemFunc == _null )
		return -1;

	return (*sReleaseSystemFunc)( fmodsystem );
}

_long SoundAPI::_FMODUpdateSystem( FMOD_SYSTEM* fmodsystem )
{
	if ( sUpdateSystemFunc == _null )
		return -1;

	return (*sUpdateSystemFunc)( fmodsystem );
}

_long SoundAPI::_FMODSet3DSettings( FMOD_SYSTEM* fmodsystem, _float dopplerscale, _float distancefactor, _float rolloffscale )
{
	if ( sSet3DSettingsFunc == _null )
		return -1;

	return (*sSet3DSettingsFunc)( fmodsystem, dopplerscale, distancefactor, rolloffscale );
}

_long SoundAPI::_FMODGet3DSettings( FMOD_SYSTEM* fmodsystem, _float* dopplerscale, _float* distancefactor, _float* rolloffscale )
{
	if ( sGet3DSettingsFunc == _null )
		return -1;

	return (*sGet3DSettingsFunc)( fmodsystem, dopplerscale, distancefactor, rolloffscale );
}

_long SoundAPI::_FMODSetListenerPosition( FMOD_SYSTEM* fmodsystem, _dword id, const Vector3* pos, const Vector3* vel, const Vector3* forward, const Vector3* up )
{
	if ( sSetListenerPositionFunc == _null )
		return -1;

	return (*sSetListenerPositionFunc)( fmodsystem, id, pos, vel, forward, up );
}

_long SoundAPI::_FMODGetListenerPosition( FMOD_SYSTEM* fmodsystem, _dword id, Vector3* pos, Vector3* vel, Vector3* forward, Vector3* up )
{
	if ( sGetListenerPositionFunc == _null )
		return -1;

	return (*sGetListenerPositionFunc)( fmodsystem, id, pos, vel, forward, up );
}

_long SoundAPI::_FMODGetPlayingChannelNumber( FMOD_SYSTEM* fmodsystem, _dword* number )
{
	if ( sGetPlayingChannelNumberFunc == _null )
		return -1;

	return (*sGetPlayingChannelNumberFunc)( fmodsystem, number );
}

_long SoundAPI::_FMODGetChannel( FMOD_SYSTEM* fmodsystem, _dword channelid, FMOD_CHANNEL** channel )
{
	if ( sGetChannelFunc == _null )
		return -1;

	return (*sGetChannelFunc)( fmodsystem, channelid, channel );
}

_long SoundAPI::_FMODCreateSound( FMOD_SYSTEM* fmodsystem, const _void* buffer, _dword flags, _void* info, FMOD_SOUND** sound )
{
	if ( sCreateSoundFunc == _null )
		return -1;

	return (*sCreateSoundFunc)( fmodsystem, buffer, flags, info, sound );
}

_long SoundAPI::_FMODReleaseSound( FMOD_SOUND* sound )
{
	if ( sReleaseSoundFunc == _null )
		return -1;

	return (*sReleaseSoundFunc)( sound );
}

_long SoundAPI::_FMODPlaySound( FMOD_SYSTEM* fmodsystem, _dword channelid, FMOD_SOUND* sound, _bool paused, FMOD_CHANNEL** channel )
{
	if ( sPlaySoundFunc == _null )
		return -1;

	return (*sPlaySoundFunc)( fmodsystem, channelid, sound, paused, channel );
}

_long SoundAPI::_FMODGetSoundLength( FMOD_SOUND* sound, _dword* time, _dword unit )
{
	if ( sGetSoundLengthFunc == _null )
		return -1;

	return (*sGetSoundLengthFunc)( sound, time, unit );
}

_long SoundAPI::_FMODSetSoundMode( FMOD_SOUND* sound, _dword mode )
{
	if ( sSetSoundModeFunc == _null )
		return -1;

	return (*sSetSoundModeFunc)( sound, mode );
}

_long SoundAPI::_FMODGetSoundMode( FMOD_SOUND* sound, _dword* mode )
{
	if ( sGetSoundModeFunc == _null )
		return -1;

	return (*sGetSoundModeFunc)( sound, mode );
}

_long SoundAPI::_FMODSetSound3DDistance( FMOD_SOUND* sound, _float min, _float max )
{
	if ( sSetSound3DDistanceFunc == _null )
		return -1;

	return (*sSetSound3DDistanceFunc)( sound, min, max );
}

_long SoundAPI::_FMODGetSound3DDistance( FMOD_SOUND* sound, _float* min, _float* max )
{
	if ( sGetSound3DDistanceFunc == _null )
		return -1;

	return (*sGetSound3DDistanceFunc)( sound, min, max );
}

_long SoundAPI::_FMODSetSoundUserData( FMOD_SOUND* sound, _void* userdata )
{
	if ( sSetSoundUserDataFunc == _null )
		return -1;

	return (*sSetSoundUserDataFunc)( sound, userdata );
}

_long SoundAPI::_FMODGetSoundUserData( FMOD_SOUND* sound, _void** userdata )
{
	if ( sGetSoundUserDataFunc == _null )
		return -1;

	return (*sGetSoundUserDataFunc)( sound, userdata );
}

_long SoundAPI::_FMODStopChannel( FMOD_CHANNEL* channel )
{
	if ( sStopChannelFunc == _null )
		return -1;

	return (*sStopChannelFunc)( channel );
}

_long SoundAPI::_FMODIsChannelPlaying( FMOD_CHANNEL* channel, _bool* playing )
{
	if ( sIsChannelPlayingFunc == _null )
		return -1;

	return (*sIsChannelPlayingFunc)( channel, playing );
}

_long SoundAPI::_FMODSetChannelPosition( FMOD_CHANNEL* channel, _dword position, FMOD_TIMEUNIT unit )
{
	if ( sSetChannelPosition == _null )
		return -1;

	return (*sSetChannelPosition)( channel, position, unit );
}

_long SoundAPI::_FMODGetChannelPosition( FMOD_CHANNEL* channel, _dword* position, FMOD_TIMEUNIT unit )
{
	if ( sGetChannelPosition == _null )
		return -1;

	return (*sGetChannelPosition)( channel, position, unit );
}

_long SoundAPI::_FMODSetChannelMute( FMOD_CHANNEL* channel, _bool mute )
{
	if ( sSetChannelMuteFunc == _null )
		return -1;

	return (*sSetChannelMuteFunc)( channel, mute );
}

_long SoundAPI::_FMODGetChannelMute( FMOD_CHANNEL* channel, _bool* mute )
{
	if ( sGetChannelMuteFunc == _null )
		return -1;

	return (*sGetChannelMuteFunc)( channel, mute );
}

_long SoundAPI::_FMODSetChannelPaused( FMOD_CHANNEL* channel, _bool paused )
{
	if ( sSetChannelPausedFunc == _null )
		return -1;

	return (*sSetChannelPausedFunc)( channel, paused );
}

_long SoundAPI::_FMODGetChannelPaused( FMOD_CHANNEL* channel, _bool* paused )
{
	if ( sGetChannelPausedFunc == _null )
		return -1;

	return (*sGetChannelPausedFunc)( channel, paused );
}

_long SoundAPI::_FMODSetChannelVolume( FMOD_CHANNEL* channel, _float volume )
{
	if ( sSetChannelVolumeFunc == _null )
		return -1;

	return (*sSetChannelVolumeFunc)( channel, volume );
}

_long SoundAPI::_FMODGetChannelVolume( FMOD_CHANNEL* channel, _float* volume )
{
	if ( sGetChannelVolumeFunc == _null )
		return -1;

	return (*sGetChannelVolumeFunc)( channel, volume );
}

_long SoundAPI::_FMODSetChannelFrequency( FMOD_CHANNEL* channel, _float frequency )
{
	if ( sSetChannelFrequencyFunc == _null )
		return -1;

	return (*sSetChannelFrequencyFunc)( channel, frequency );
}

_long SoundAPI::_FMODGetChannelFrequency( FMOD_CHANNEL* channel, _float* frequency )
{
	if ( sGetChannelFrequencyFunc == _null )
		return -1;

	return (*sGetChannelFrequencyFunc)( channel, frequency );
}

_long SoundAPI::_FMODSetChannel3DAttributes( FMOD_CHANNEL* channel, const Vector3* position, const Vector3* velocity )
{
	if ( sSetChannel3DAttributesFunc == _null )
		return -1;

	return (*sSetChannel3DAttributesFunc)( channel, position, velocity );
}

_long SoundAPI::_FMODGetChannel3DAttributes( FMOD_CHANNEL* channel, Vector3* position, Vector3* velocity )
{
	if ( sGetChannel3DAttributesFunc == _null )
		return -1;

	return (*sGetChannel3DAttributesFunc)( channel, position, velocity );
}

_long SoundAPI::_FMODSetChannelGroup( FMOD_CHANNEL* channel, FMOD_CHANNELGROUP* channelgroup )
{
	if ( sSetChannelGroupFunc == _null )
		return -1;

	return (*sSetChannelGroupFunc)( channel, channelgroup );
}

_long SoundAPI::_FMODGetChannelGroup( FMOD_CHANNEL* channel, FMOD_CHANNELGROUP** channelgroup )
{
	if ( sGetChannelGroupFunc == _null )
		return -1;

	return (*sGetChannelGroupFunc)( channel, channelgroup );
}

_long SoundAPI::_FMODSetChannelUserData( FMOD_CHANNEL* channel, _void* userdata )
{
	if ( sSetChannelUserDataFunc == _null )
		return -1;

	return (*sSetChannelUserDataFunc)( channel, userdata );
}

_long SoundAPI::_FMODGetChannelUserData( FMOD_CHANNEL* channel, _void** userdata )
{
	if ( sGetChannelUserDataFunc == _null )
		return -1;

	return (*sGetChannelUserDataFunc)( channel, userdata );
}

_long SoundAPI::_FMODGetChannelCurrentSound( FMOD_CHANNEL* channel, FMOD_SOUND** sound )
{
	if ( sGetChannelCurrentSoundFunc == _null )
		return -1;

	return (*sGetChannelCurrentSoundFunc)( channel, sound );
}

_long SoundAPI::_FMODGetMasterChannelGroup( FMOD_SYSTEM* system, FMOD_CHANNELGROUP** channelgroup )
{
	if ( sGetMasterChannelGroupFunc == _null )
		return -1;

	return (*sGetMasterChannelGroupFunc)( system, channelgroup );
}

_long SoundAPI::_FMODCreateChannelGroup( FMOD_SYSTEM* system, const _chara* name, FMOD_CHANNELGROUP** channelgroup )
{
	if ( sCreateChannelGroupFunc == _null )
		return -1;

	return (*sCreateChannelGroupFunc)( system, name, channelgroup );
}

_long SoundAPI::_FMODReleaseChannelGroup( FMOD_CHANNELGROUP* channelgroup )
{
	if ( sReleaseChannelGroupFunc == _null )
		return -1;

	return (*sReleaseChannelGroupFunc)( channelgroup );
}

_long SoundAPI::_FMODGetChannelGroupName( FMOD_CHANNELGROUP* channelgroup, _chara* name, _dword length )
{
	if ( sGetChannelGroupNameFunc == _null )
		return -1;

	return (*sGetChannelGroupNameFunc)( channelgroup, name, length );
}

_long SoundAPI::_FMODSetChannelGroupVolume( FMOD_CHANNELGROUP* channelgroup, _float volume )
{
	if ( sSetChannelGroupVolumeFunc == _null )
		return -1;

	return (*sSetChannelGroupVolumeFunc)( channelgroup, volume );
}

_long SoundAPI::_FMODGetChannelGroupVolume( FMOD_CHANNELGROUP* channelgroup, _float* volume )
{
	if ( sGetChannelGroupVolumeFunc == _null )
		return -1;

	return (*sGetChannelGroupVolumeFunc)( channelgroup, volume );
}

_long SoundAPI::_FMODOverrideChannelGroupMute( FMOD_CHANNELGROUP* channelgroup, _bool mute )
{
	if ( sOverrideChannelGroupMuteFunc == _null )
		return -1;

	return (*sOverrideChannelGroupMuteFunc)( channelgroup, mute );
}

_long SoundAPI::_FMODGetChannelNumberInGroup( FMOD_CHANNELGROUP* channelgroup, _dword* number )
{
	if ( sGetChannelNumberInGroupFunc == _null )
		return -1;

	return (*sGetChannelNumberInGroupFunc)( channelgroup, number );
}

_long SoundAPI::_FMODGetChannelInGroup( FMOD_CHANNELGROUP* channelgroup, _dword index, FMOD_CHANNEL** channel )
{
	if ( sGetChannelInGroupFunc == _null )
		return -1;

	return (*sGetChannelInGroupFunc)( channelgroup, index, channel );
}

_long SoundAPI::_FMODStopChannelGroup( FMOD_CHANNELGROUP* channelgroup )
{
	if ( sStopChannelGroupFunc == _null )
		return -1;

	return (*sStopChannelGroupFunc)( channelgroup );
}