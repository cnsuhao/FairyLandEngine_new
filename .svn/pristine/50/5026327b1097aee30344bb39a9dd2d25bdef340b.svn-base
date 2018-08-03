//============================================================================
// SoundDevice.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// SoundDevice
//----------------------------------------------------------------------------

class SoundDevice : public ISoundDevice
{
private:
	typedef Pair< String, FMOD_CHANNELGROUP* >	SoundGroup;
	typedef Array< SoundGroup >					SoundGroupArray;

	struct DelaySound
	{
		String			mSoundGroup;
		String			mFileName;
		Vector3			mPosition;
		_dword			mFlag;
		_dword			mTickcount;
		_dword			mToken;
	};

	struct FadeChannel
	{
		FMOD_CHANNEL*	mFmodChannel;
		_dword			mDelay;
		_dword			mTickcount;
		_dword			mDuration;

		operator FMOD_CHANNEL* ( ) const
			{ return mFmodChannel; }
	};

	typedef Queue< DelaySound > DelaySoundQueue;
	typedef Array< FadeChannel, FMOD_CHANNEL* > FadeChannelArray;

	FMOD_SYSTEM*			mFmodSystem;
	FMOD_CHANNELGROUP*		mMasterGroup;
	FMOD_CHANNELGROUP*		mCurrentGroup;
	String					mCurrentGroupName;
	SoundGroupArray			mSoundGroupArray;

	SoundResourceManager*	mSoundResourceManager;

	Lock					mDelaySoundLock;
	Thread					mDelaySoundThread;

	DelaySoundQueue			mDelayLoadQueue;
	DelaySoundQueue			mDelayPlayQueue;
	FadeChannelArray		mFadeInArray;
	FadeChannelArray		mFadeOutArray;

	_dword					mMaxChannelNumber;
	_float					mMinDistance;
	_float					mMaxDistance;

	_bool					mAutoMute;
	_bool					mGlobalMute;
	_float					mGlobalVolume;

	_dword					mAccumulatedElapse;
	_dword					mFadeInTime;
	_dword					mFadeInDelay;
	_dword					mFadeOutTime;
	_dword					mFadeOutDelay;

	static _bool OnLoadSoundThread( _void* parameter1, _void* parameter2 );

	static FMOD_RESULT _stdcall OnOpenStream( const _char* filename, _dword unicode, _dword* filesize, _void** handle, _void** userdata );
	static FMOD_RESULT _stdcall OnCloseStream( _void* handle, _void* userdata );
	static FMOD_RESULT _stdcall OnReadStream( _void* handle, _void* buffer, _dword size, _dword* bytesread, _void* userdata );
	static FMOD_RESULT _stdcall OnSeekStream( _void* handle, _dword offset, _void* userdata );

	FMOD_CHANNELGROUP* CreateSoundGroup( StringPtr groupname );
	FMOD_CHANNELGROUP* SearchSoundGroup( StringPtr groupname ) const;

	_void StopSoundChannel( FMOD_CHANNEL* channel );
	_bool UpdateFadeChannel( FadeChannel& fadechannel, _dword elapse );

public:
	SoundDevice( );
	~SoundDevice( );

	_bool CreateDevice( _handle module, _dword maxchannels );

	virtual _void Release( );

	virtual _void Update( _dword elapse );

	virtual _bool Set3DSettings( _float dopplerscale, _float distancefactor, _float rolloffscale, _float mindistance, _float maxdistance );
	virtual _bool Get3DSettings( _float& dopplerscale, _float& distancefactor, _float& rolloffscale, _float& mindistance, _float& maxdistance );
	virtual _bool SetListenerPosition( const Vector3& position, const Vector3& forward, const Vector3& up );
	virtual _bool GetListenerPosition( Vector3& position, Vector3& forward, Vector3& up );

	virtual ISoundBuffer* CreateSoundBuffer( StringPtr filename, _dword flags );
	virtual ISoundBuffer* CreateSoundBuffer( const MemFile& soundfile, _dword flags );
	virtual ISoundBuffer* CloneSoundBuffer( ISoundBuffer* soundbuffer );
	virtual _void ReleaseSoundBuffer( ISoundBuffer*& soundbuffer );

	virtual _bool PlaySoundBuffer( ISoundBuffer* soundbuffer, ISoundChannel** soundchannel, _dword flags );
	virtual _bool PlaySoundBuffer( ISoundBuffer* soundbuffer, const Vector3& position, ISoundChannel** soundchannel, _dword flags );
	virtual _void ReleaseSoundChannel( ISoundChannel*& soundchannel );

	virtual _bool PlaySoundBuffer( StringPtr filename, _dword flags );
	virtual _bool PlaySoundBuffer( StringPtr filename, const Vector3& position, _dword flags );

	virtual _bool SetCurrentSoundGroup( StringPtr groupname );
	virtual StringPtr GetCurrentSoundGroup( ) const;

	virtual _bool StopSoundGroup( StringPtr groupname, _dword flag );
	virtual _bool StopAllSoundGroup( );

	virtual _bool SetGlobalMute( _bool mute );
	virtual _bool GetGlobalMute( ) const;
	virtual _bool SetGlobalVolume( _float volume );
	virtual _float GetGlobalVolume( ) const;

	virtual _bool SetSoundGroupMute( StringPtr groupname, _bool mute );
	virtual _bool SetSoundGroupVolume( StringPtr groupname, _float volume );
	virtual _float GetSoundGroupVolume( StringPtr groupname ) const;

	virtual _void SetFadeInTime( _dword time );
	virtual _void SetFadeInDelay( _dword delay );
	virtual _void SetFadeOutTime( _dword time );
	virtual _void SetFadeOutDelay( _dword delay );

	virtual _dword GetPlayingChannelNumber( StringPtr groupname ) const;
	virtual _dword GetMaxChannelNumber( ) const;
};

};