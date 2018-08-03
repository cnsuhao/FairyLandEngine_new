//============================================================================
// SoundAPI.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// SoundAPI
//----------------------------------------------------------------------------

class SoundAPI
{
private:
	typedef _long (_stdcall *_FMODCreateSystemFunc)( FMOD_SYSTEM** fmodsystem );
	typedef _long (_stdcall *_FMODCloseSystemFunc)( FMOD_SYSTEM* fmodsystem );
	typedef _long (_stdcall *_FMODInitSystemFunc)( FMOD_SYSTEM* fmodsystem, _dword maxchannels, _dword flags, _void* driverdata );
	typedef _long (_stdcall *_FMODSetFileSystemFunc)( FMOD_SYSTEM* fmodsystem, FMOD_FILE_OPENCALLBACK useropen, FMOD_FILE_CLOSECALLBACK userclose, FMOD_FILE_READCALLBACK userread, FMOD_FILE_SEEKCALLBACK userseek, _dword blocksize );
	typedef _long (_stdcall *_FMODReleaseSystemFunc)( FMOD_SYSTEM* fmodsystem );
	typedef _long (_stdcall *_FMODUpdateSystemFunc)( FMOD_SYSTEM* fmodsystem );
	typedef _long (_stdcall *_FMODSet3DSettingsFunc)( FMOD_SYSTEM* fmodsystem, _float dopplerscale, _float distancefactor, _float rolloffscale );
	typedef _long (_stdcall *_FMODGet3DSettingsFunc)( FMOD_SYSTEM* fmodsystem, _float* dopplerscale, _float* distancefactor, _float* rolloffscale );
	typedef _long (_stdcall *_FMODSetListenerPositionFunc)( FMOD_SYSTEM* fmodsystem, _dword id, const Vector3* pos, const Vector3* vel, const Vector3* forward, const Vector3* up );
	typedef _long (_stdcall *_FMODGetListenerPositionFunc)( FMOD_SYSTEM* fmodsystem, _dword id, Vector3* pos, Vector3* vel, Vector3* forward, Vector3* up );
	typedef _long (_stdcall *_FMODGetPlayingChannelNumberFunc)( FMOD_SYSTEM* fmodsystem, _dword* number );
	typedef _long (_stdcall *_FMODGetChannelFunc)( FMOD_SYSTEM* fmodsystem, _dword channelid, FMOD_CHANNEL** channel );
	typedef _long (_stdcall *_FMODCreateSoundFunc)( FMOD_SYSTEM* fmodsystem, const _void* buffer, _dword flags, _void* info, FMOD_SOUND** sound );
	typedef _long (_stdcall *_FMODReleaseSoundFunc)( FMOD_SOUND* sound );
	typedef _long (_stdcall *_FMODPlaySoundFunc)( FMOD_SYSTEM* fmodsystem, _dword channelid, FMOD_SOUND* sound, _bool paused, FMOD_CHANNEL** channel );
	typedef _long (_stdcall *_FMODGetSoundLengthFunc)( FMOD_SOUND* sound, _dword* time, _dword unit );
	typedef _long (_stdcall *_FMODSetSoundModeFunc)( FMOD_SOUND* sound, _dword mode );
	typedef _long (_stdcall *_FMODGetSoundModeFunc)( FMOD_SOUND* sound, _dword* mode );
	typedef _long (_stdcall *_FMODSetSound3DDistanceFunc)( FMOD_SOUND* sound, _float min, _float max );
	typedef _long (_stdcall *_FMODGetSound3DDistanceFunc)( FMOD_SOUND* sound, _float* min, _float* max );
	typedef _long (_stdcall *_FMODSetSoundUserDataFunc)( FMOD_SOUND* sound, _void* userdata );
	typedef _long (_stdcall *_FMODGetSoundUserDataFunc)( FMOD_SOUND* sound, _void* userdata );
	typedef _long (_stdcall *_FMODStopChannelFunc)( FMOD_CHANNEL* channel );
	typedef _long (_stdcall *_FMODIsChannelPlayingFunc)( FMOD_CHANNEL* channel, _bool* playing );
	typedef _long (_stdcall *_FMODSetChannelPositionFunc)( FMOD_CHANNEL* channel, _dword position, FMOD_TIMEUNIT unit );
	typedef _long (_stdcall *_FMODGetChannelPositionFunc)( FMOD_CHANNEL* channel, _dword* position, FMOD_TIMEUNIT unit );
	typedef _long (_stdcall *_FMODSetChannelMuteFunc)( FMOD_CHANNEL* channel, _bool mute );
	typedef _long (_stdcall *_FMODGetChannelMuteFunc)( FMOD_CHANNEL* channel, _bool* mute );
	typedef _long (_stdcall *_FMODSetChannelPausedFunc)( FMOD_CHANNEL* channel, _bool paused );
	typedef _long (_stdcall *_FMODGetChannelPausedFunc)( FMOD_CHANNEL* channel, _bool* paused );
	typedef _long (_stdcall *_FMODSetChannelVolumeFunc)( FMOD_CHANNEL* channel, _float volume );
	typedef _long (_stdcall *_FMODGetChannelVolumeFunc)( FMOD_CHANNEL* channel, _float* volume );
	typedef _long (_stdcall *_FMODSetChannelFrequencyFunc)( FMOD_CHANNEL* channel, _float frequency );
	typedef _long (_stdcall *_FMODGetChannelFrequencyFunc)( FMOD_CHANNEL* channel, _float* frequency );
	typedef _long (_stdcall *_FMODSetChannel3DAttributesFunc)( FMOD_CHANNEL* channel, const Vector3* position, const Vector3* velocity );
	typedef _long (_stdcall *_FMODGetChannel3DAttributesFunc)( FMOD_CHANNEL* channel, Vector3* position, Vector3* velocity );
	typedef _long (_stdcall *_FMODSetChannelGroupFunc)( FMOD_CHANNEL* channel, FMOD_CHANNELGROUP* channelgroup );
	typedef _long (_stdcall *_FMODGetChannelGroupFunc)( FMOD_CHANNEL* channel, FMOD_CHANNELGROUP** channelgroup );
	typedef _long (_stdcall *_FMODSetChannelUserDataFunc)( FMOD_CHANNEL* channel, _void* userdata );
	typedef _long (_stdcall *_FMODGetChannelUserDataFunc)( FMOD_CHANNEL* channel, _void** userdata );
	typedef _long (_stdcall *_FMODGetChannelCurrentSoundFunc)( FMOD_CHANNEL* channel, FMOD_SOUND** sound );
	typedef _long (_stdcall *_FMODGetMasterChannelGroupFunc)( FMOD_SYSTEM* system, FMOD_CHANNELGROUP** channelgroup );
	typedef _long (_stdcall *_FMODCreateChannelGroupFunc)( FMOD_SYSTEM* system, const _chara* name, FMOD_CHANNELGROUP** channelgroup );
	typedef _long (_stdcall *_FMODReleaseChannelGroupFunc)( FMOD_CHANNELGROUP* channelgroup );
	typedef _long (_stdcall *_FMODGetChannelGroupNameFunc)( FMOD_CHANNELGROUP* channelgroup, const _chara* name, _dword length );
	typedef _long (_stdcall *_FMODSetChannelGroupVolumeFunc)( FMOD_CHANNELGROUP* channelgroup, _float volume );
	typedef _long (_stdcall *_FMODGetChannelGroupVolumeFunc)( FMOD_CHANNELGROUP* channelgroup, _float* volume );
	typedef _long (_stdcall *_FMODOverrideChannelGroupMuteFunc)( FMOD_CHANNELGROUP* channelgroup, _bool mute );
	typedef _long (_stdcall *_FMODGetChannelNumberInGroupFunc)( FMOD_CHANNELGROUP* channelgroup, _dword* number );
	typedef _long (_stdcall *_FMODGetChannelInGroupFunc)( FMOD_CHANNELGROUP* channelgroup, _dword index, FMOD_CHANNEL** channel );
	typedef _long (_stdcall *_FMODStopChannelGroupFunc)( FMOD_CHANNELGROUP* channelgroup );

	static _FMODCreateSystemFunc				sCreateSystemFunc;
	static _FMODCloseSystemFunc					sCloseSystemFunc;
	static _FMODInitSystemFunc					sInitSystemFunc;
	static _FMODSetFileSystemFunc				sSetFileSystemFunc;
	static _FMODReleaseSystemFunc				sReleaseSystemFunc;
	static _FMODUpdateSystemFunc				sUpdateSystemFunc;
	static _FMODSet3DSettingsFunc				sSet3DSettingsFunc;
	static _FMODGet3DSettingsFunc				sGet3DSettingsFunc;
	static _FMODSetListenerPositionFunc			sSetListenerPositionFunc;
	static _FMODGetListenerPositionFunc			sGetListenerPositionFunc;
	static _FMODGetPlayingChannelNumberFunc		sGetPlayingChannelNumberFunc;
	static _FMODGetChannelFunc					sGetChannelFunc;
	static _FMODCreateSoundFunc					sCreateSoundFunc;
	static _FMODReleaseSoundFunc				sReleaseSoundFunc;
	static _FMODPlaySoundFunc					sPlaySoundFunc;
	static _FMODGetSoundLengthFunc				sGetSoundLengthFunc;
	static _FMODSetSoundModeFunc				sSetSoundModeFunc;
	static _FMODGetSoundModeFunc				sGetSoundModeFunc;
	static _FMODSetSound3DDistanceFunc			sSetSound3DDistanceFunc;
	static _FMODGetSound3DDistanceFunc			sGetSound3DDistanceFunc;
	static _FMODSetSoundUserDataFunc			sSetSoundUserDataFunc;
	static _FMODGetSoundUserDataFunc			sGetSoundUserDataFunc;
	static _FMODStopChannelFunc					sStopChannelFunc;
	static _FMODIsChannelPlayingFunc			sIsChannelPlayingFunc;
	static _FMODSetChannelPositionFunc			sSetChannelPosition;
	static _FMODGetChannelPositionFunc			sGetChannelPosition;
	static _FMODSetChannelMuteFunc				sSetChannelMuteFunc;
	static _FMODGetChannelMuteFunc				sGetChannelMuteFunc;
	static _FMODSetChannelPausedFunc			sSetChannelPausedFunc;
	static _FMODGetChannelPausedFunc			sGetChannelPausedFunc;
	static _FMODSetChannelVolumeFunc			sSetChannelVolumeFunc;
	static _FMODGetChannelVolumeFunc			sGetChannelVolumeFunc;
	static _FMODSetChannelFrequencyFunc			sSetChannelFrequencyFunc;
	static _FMODGetChannelFrequencyFunc			sGetChannelFrequencyFunc;
	static _FMODSetChannel3DAttributesFunc		sSetChannel3DAttributesFunc;
	static _FMODGetChannel3DAttributesFunc		sGetChannel3DAttributesFunc;
	static _FMODSetChannelGroupFunc				sSetChannelGroupFunc;
	static _FMODGetChannelGroupFunc				sGetChannelGroupFunc;
	static _FMODSetChannelUserDataFunc			sSetChannelUserDataFunc;
	static _FMODGetChannelUserDataFunc			sGetChannelUserDataFunc;
	static _FMODGetChannelCurrentSoundFunc		sGetChannelCurrentSoundFunc;
	static _FMODGetMasterChannelGroupFunc		sGetMasterChannelGroupFunc;
	static _FMODCreateChannelGroupFunc			sCreateChannelGroupFunc;
	static _FMODReleaseChannelGroupFunc			sReleaseChannelGroupFunc;
	static _FMODGetChannelGroupNameFunc			sGetChannelGroupNameFunc;
	static _FMODSetChannelGroupVolumeFunc		sSetChannelGroupVolumeFunc;
	static _FMODGetChannelGroupVolumeFunc		sGetChannelGroupVolumeFunc;
	static _FMODOverrideChannelGroupMuteFunc	sOverrideChannelGroupMuteFunc;
	static _FMODGetChannelNumberInGroupFunc		sGetChannelNumberInGroupFunc;
	static _FMODGetChannelInGroupFunc			sGetChannelInGroupFunc;
	static _FMODStopChannelGroupFunc			sStopChannelGroupFunc;
	
private:
	static Library	sFmodLibrary;
	static Lock		sFmodLock;

public:
	static _bool LoadFmod( _handle module );

	static _long _FMODCreateSystem( FMOD_SYSTEM** fmodsystem );
	static _long _FMODCloseSystem( FMOD_SYSTEM* fmodsystem );
	static _long _FMODInitSystem( FMOD_SYSTEM* fmodsystem, _dword maxchannels, _dword flags, _void* driverdata );
	static _long _FMODSetFileSystem( FMOD_SYSTEM* fmodsystem, FMOD_FILE_OPENCALLBACK useropen, FMOD_FILE_CLOSECALLBACK userclose, FMOD_FILE_READCALLBACK userread, FMOD_FILE_SEEKCALLBACK userseek, _dword blocksize );
	static _long _FMODReleaseSystem( FMOD_SYSTEM* fmodsystem );
	static _long _FMODUpdateSystem( FMOD_SYSTEM* fmodsystem );
	static _long _FMODSet3DSettings( FMOD_SYSTEM* fmodsystem, _float dopplerscale, _float distancefactor, _float rolloffscale );
	static _long _FMODGet3DSettings( FMOD_SYSTEM* fmodsystem, _float* dopplerscale, _float* distancefactor, _float* rolloffscale );
	static _long _FMODSetListenerPosition( FMOD_SYSTEM* fmodsystem, _dword id, const Vector3* pos, const Vector3* vel, const Vector3* forward, const Vector3* up );
	static _long _FMODGetListenerPosition( FMOD_SYSTEM* fmodsystem, _dword id, Vector3* pos, Vector3* vel, Vector3* forward, Vector3* up );
	static _long _FMODGetPlayingChannelNumber( FMOD_SYSTEM* fmodsystem, _dword* number );
	static _long _FMODGetChannel( FMOD_SYSTEM* fmodsystem, _dword channelid, FMOD_CHANNEL** channel );
	static _long _FMODCreateSound( FMOD_SYSTEM* fmodsystem, const _void* buffer, _dword flags, _void* info, FMOD_SOUND** sound );
	static _long _FMODReleaseSound( FMOD_SOUND* sound );
	static _long _FMODPlaySound( FMOD_SYSTEM* fmodsystem, _dword channelid, FMOD_SOUND* sound, _bool paused, FMOD_CHANNEL** channel );
	static _long _FMODGetSoundLength( FMOD_SOUND* sound, _dword* time, _dword unit );
	static _long _FMODSetSoundMode( FMOD_SOUND* sound, _dword mode );
	static _long _FMODGetSoundMode( FMOD_SOUND* sound, _dword* mode );
	static _long _FMODSetSound3DDistance( FMOD_SOUND* sound, _float min, _float max );
	static _long _FMODGetSound3DDistance( FMOD_SOUND* sound, _float* min, _float* max );
	static _long _FMODSetSoundUserData( FMOD_SOUND* sound, _void* userdata );
	static _long _FMODGetSoundUserData( FMOD_SOUND* sound, _void** userdata );
	static _long _FMODStopChannel( FMOD_CHANNEL* channel );
	static _long _FMODIsChannelPlaying( FMOD_CHANNEL* channel, _bool* playing );
	static _long _FMODSetChannelPosition( FMOD_CHANNEL* channel, _dword position, FMOD_TIMEUNIT unit );
	static _long _FMODGetChannelPosition( FMOD_CHANNEL* channel, _dword* position, FMOD_TIMEUNIT unit );
	static _long _FMODSetChannelMute( FMOD_CHANNEL* channel, _bool mute );
	static _long _FMODGetChannelMute( FMOD_CHANNEL* channel, _bool* mute );
	static _long _FMODSetChannelPaused( FMOD_CHANNEL* channel, _bool paused );
	static _long _FMODGetChannelPaused( FMOD_CHANNEL* channel, _bool* paused );
	static _long _FMODSetChannelVolume( FMOD_CHANNEL* channel, _float volume );
	static _long _FMODGetChannelVolume( FMOD_CHANNEL* channel, _float* volume );
	static _long _FMODSetChannelFrequency( FMOD_CHANNEL* channel, _float frequency );
	static _long _FMODGetChannelFrequency( FMOD_CHANNEL* channel, _float* frequency );
	static _long _FMODSetChannel3DAttributes( FMOD_CHANNEL* channel, const Vector3* position, const Vector3* velocity );
	static _long _FMODGetChannel3DAttributes( FMOD_CHANNEL* channel, Vector3* position, Vector3* velocity );
	static _long _FMODSetChannelGroup( FMOD_CHANNEL* channel, FMOD_CHANNELGROUP* channelgroup );
	static _long _FMODGetChannelGroup( FMOD_CHANNEL* channel, FMOD_CHANNELGROUP** channelgroup );
	static _long _FMODSetChannelUserData( FMOD_CHANNEL* channel, _void* userdata );
	static _long _FMODGetChannelUserData( FMOD_CHANNEL* channel, _void** userdata );
	static _long _FMODGetChannelCurrentSound( FMOD_CHANNEL* channel, FMOD_SOUND** sound );
	static _long _FMODGetMasterChannelGroup( FMOD_SYSTEM* system, FMOD_CHANNELGROUP** channelgroup );
	static _long _FMODCreateChannelGroup( FMOD_SYSTEM* system, const _chara* name, FMOD_CHANNELGROUP** channelgroup );
	static _long _FMODReleaseChannelGroup( FMOD_CHANNELGROUP* channelgroup );
	static _long _FMODGetChannelGroupName( FMOD_CHANNELGROUP* channelgroup, _chara* name, _dword length );
	static _long _FMODSetChannelGroupVolume( FMOD_CHANNELGROUP* channelgroup, _float volume );
	static _long _FMODGetChannelGroupVolume( FMOD_CHANNELGROUP* channelgroup, _float* volume );
	static _long _FMODOverrideChannelGroupMute( FMOD_CHANNELGROUP* channelgroup, _bool mute );
	static _long _FMODGetChannelNumberInGroup( FMOD_CHANNELGROUP* channelgroup, _dword* number );
	static _long _FMODGetChannelInGroup( FMOD_CHANNELGROUP* channelgroup, _dword index, FMOD_CHANNEL** channel );
	static _long _FMODStopChannelGroup( FMOD_CHANNELGROUP* channelgroup );
};

};