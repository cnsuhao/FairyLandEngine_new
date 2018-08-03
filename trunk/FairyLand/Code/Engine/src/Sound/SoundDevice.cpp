//============================================================================
// SoundDevice.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandSound.h"

//----------------------------------------------------------------------------
// SoundDevice Implementation
//----------------------------------------------------------------------------

SoundDevice::SoundDevice( )
{
	mFmodSystem				= _null;
	mCurrentGroup			= _null;
	mMasterGroup			= _null;

	mSoundResourceManager	= new SoundResourceManager;

	mMaxChannelNumber		= 0;
	mMinDistance			= 1.0f;
	mMaxDistance			= 10000.0f;

	mGlobalVolume			= 1.0f;
	mAutoMute				= _false;
	mGlobalMute				= _false;

	mAccumulatedElapse		= 0;
	mFadeInTime				= 1000;
	mFadeInDelay			= 0;
	mFadeOutTime			= 1000;
	mFadeOutDelay			= 0;
}

SoundDevice::~SoundDevice( )
{
	mDelaySoundThread.Close( );

	StopAllSoundGroup( );

	SoundAPI::_FMODCloseSystem( mFmodSystem );
	SoundAPI::_FMODReleaseSystem( mFmodSystem );

	delete mSoundResourceManager;

	GetFairyLandGlobal( ).mSoundDevice = _null;
}

_bool SoundDevice::OnLoadSoundThread( _void* parameter1, _void* parameter2 )
{
	SoundDevice* sounddevice = (SoundDevice*) parameter1;
	FL_ASSERT( sounddevice != _null )

	sounddevice->mDelaySoundLock.Enter( );
	_dword number = sounddevice->mDelayLoadQueue.Number( );
	sounddevice->mDelaySoundLock.Leave( );

	// Process all delay sound.
	for ( _dword i = 0; i < number; i ++ )
	{
		// Get a sound to play.
		sounddevice->mDelaySoundLock.Enter( );
		DelaySound delaysound = sounddevice->mDelayLoadQueue.Front( );
		sounddevice->mDelaySoundLock.Leave( );

		// It is too late to play sound now, and it is not loop, just skip it.
		if ( System::GetCurrentTickcount( ) - delaysound.mTickcount > 500 && ( delaysound.mFlag & _SOUND_LOOP_ENABLE ) == 0 )
		{
			sounddevice->mDelaySoundLock.Enter( );
			sounddevice->mDelayLoadQueue.Remove( );
			sounddevice->mDelaySoundLock.Leave( );

			continue;
		}

		if ( GetResourceManagerPointer( ) == _null )
			continue;

		// Try to use existing resource.
		delaysound.mToken = sounddevice->mSoundResourceManager->ObtainResource( delaysound.mFileName );

		if ( delaysound.mToken == 0 )
		{
			MemFile tempfile;

			// Load sound buffer into memory.
			GetResourceManager( ).LoadResource( delaysound.mFileName, tempfile, _false );

			// Load sound failed.
			if ( tempfile.GetLength( ) == 0 )
			{
				sounddevice->mDelaySoundLock.Enter( );
				sounddevice->mDelayLoadQueue.Remove( );
				sounddevice->mDelaySoundLock.Leave( );

				continue;
			}

			IDataStream* datastream = GetResourceManager( ).CreateMemStream( L"", tempfile.GetLength( ), Oword( ) );
			FL_ASSERT( datastream != _null )

			datastream->Write( tempfile.GetBuffer( ), tempfile.GetLength( ), 0 );

			// Create new sound resource.
			delaysound.mToken = sounddevice->mSoundResourceManager->InsertResource( delaysound.mFileName, datastream );
		}

		sounddevice->mDelaySoundLock.Enter( );

		// Put the loaded sound buffer into delay play queue.
		if ( sounddevice->mDelayLoadQueue.Front( ).mTickcount != -1 )
			sounddevice->mDelayPlayQueue.Append( delaysound );

		sounddevice->mDelayLoadQueue.Remove( );

		sounddevice->mDelaySoundLock.Leave( );
	}

	return _true;
}

FMOD_RESULT SoundDevice::OnOpenStream( const _char* filename, _dword unicode, _dword* filesize, _void** handle, _void** userdata )
{
	#ifdef _DEBUG_SOUND
	PS_TRACE1( L"OnOpenStream %s\r\n", (_void*) filename )
	#endif

	SoundDevice* sounddevice = (SoundDevice*) GetSoundDevicePointer( );
	FL_ASSERT( sounddevice != _null )

	_dword token = sounddevice->mSoundResourceManager->ObtainResource( filename );

	IDataStream* datastream = _null;

	if ( token != 0 )
	{
		#ifdef _DEBUG_SOUND
		PS_TRACE1( L"    Found existing token %p\r\n", (_void*) token )
		#endif

		datastream = sounddevice->mSoundResourceManager->ObtainSoundStream( token );
		FL_ASSERT( datastream != _null )
	}
	else
	{
		if ( GetResourceManagerPointer( ) == _null )
			return FMOD_ERR_FILE_NOTFOUND;

		datastream = GetResourceManager( ).OpenResource( filename );

		if ( datastream == _null )
			return FMOD_ERR_FILE_NOTFOUND;

		// Create new sound resource.
		token = sounddevice->mSoundResourceManager->InsertResource( filename, datastream );

		#ifdef _DEBUG_SOUND
		PS_TRACE1( L"    Create new token %p\r\n", (_void*) token )
		#endif
	}

	// Set file size.
	*filesize = datastream->GetSize( );

	// Set file handle.
	*handle = (_void*) token;

	*userdata = new _dword( 0 );

	return FMOD_OK;
}

FMOD_RESULT SoundDevice::OnCloseStream( _void* handle, _void* userdata )
{
	#ifdef _DEBUG_SOUND
	PS_TRACE1( L"OnCloseStream %d\r\n", handle )
	#endif

	SoundDevice* sounddevice = (SoundDevice*) GetSoundDevicePointer( );
	FL_ASSERT( sounddevice != _null )

	sounddevice->mSoundResourceManager->RemoveResource( (_dword) handle );

	delete (_dword*) userdata;

	return FMOD_OK;
}

FMOD_RESULT SoundDevice::OnReadStream( _void* handle, _void* buffer, _dword size, _dword* bytesread, _void* userdata )
{
	#ifdef _DEBUG_SOUND
	PS_TRACE1( L"OnReadStream %d\r\n", handle )
	#endif

	SoundDevice* sounddevice = (SoundDevice*) GetSoundDevicePointer( );
	FL_ASSERT( sounddevice != _null )

	IDataStream* datastream = sounddevice->mSoundResourceManager->ObtainSoundStream( (_dword) handle );
	if ( datastream == _null )
		return FMOD_ERR_INVALID_PARAM;

	_dword offset = *( (_dword*) userdata );

	_dword length = datastream->GetSize( ) - offset;

	if ( length > size )
		length = size;

	if ( datastream->Read( buffer, length, offset ) == _false )
		return FMOD_ERR_INVALID_PARAM;

	*( (_dword*) userdata ) += length;

	// Set bytes read.
	*bytesread = length;

	return length == size ? FMOD_OK : FMOD_ERR_FILE_EOF;
}

FMOD_RESULT SoundDevice::OnSeekStream( _void* handle, _dword offset, _void* userdata )
{
	#ifdef _DEBUG_SOUND
	PS_TRACE1( L"OnSeekStream %d\r\n", handle )
	#endif

	SoundDevice* sounddevice = (SoundDevice*) GetSoundDevicePointer( );
	FL_ASSERT( sounddevice != _null )

	IDataStream* datastream = sounddevice->mSoundResourceManager->ObtainSoundStream( (_dword) handle );
	if ( datastream == _null )
		return FMOD_ERR_INVALID_PARAM;

	if ( offset > datastream->GetSize( ) )
		offset = datastream->GetSize( );

	*( (_dword*) userdata ) = offset;

	return FMOD_OK;
}

FMOD_CHANNELGROUP* SoundDevice::CreateSoundGroup( StringPtr groupname )
{
	// Search an existing group.
	FMOD_CHANNELGROUP* channelgroup = SearchSoundGroup( groupname );

	if ( channelgroup != _null )
		return channelgroup;

	_chara buffer[1024];
	StringFormatter::UnicodeToAnsi( buffer, 1024, groupname );

	// Create an new group.
	if ( SoundAPI::_FMODCreateChannelGroup( mFmodSystem, buffer, &channelgroup ) != 0 )
		return _null;

	if ( mGlobalMute )
		SoundAPI::_FMODOverrideChannelGroupMute( channelgroup, _true );

	mSoundGroupArray.Append( SoundGroup( String( groupname ), channelgroup ) );

	return channelgroup;
}

FMOD_CHANNELGROUP* SoundDevice::SearchSoundGroup( StringPtr groupname ) const
{
	// Search in existing groups.
	for ( _dword i = 0; i < mSoundGroupArray.Number( ); i ++ )
	{
		// Compare group name.
		if ( mSoundGroupArray[i].mObject1.CompareLowercase( groupname ) == _true )
			return mSoundGroupArray[i].mObject2;
	}

	return _null;
}

_void SoundDevice::StopSoundChannel( FMOD_CHANNEL* channel )
{
	_void* userdata = _null;

	if ( SoundAPI::_FMODGetChannelUserData( channel, &userdata ) != 0 || userdata == _null )
		return;

	SoundAPI::_FMODSetChannelUserData( channel, _null );
	SoundAPI::_FMODStopChannel( channel );

	ISoundBuffer* soundbuffer = (ISoundBuffer*) userdata;

	// Release the sound buffer.
	ReleaseSoundBuffer( soundbuffer );
}

_bool SoundDevice::UpdateFadeChannel( FadeChannel& fadechannel, _dword elapse )
{
	// Update delay.
	if ( fadechannel.mDelay >= elapse )
	{
		fadechannel.mDelay -= elapse;
		elapse = 0;
	}
	else
	{
		elapse -= fadechannel.mDelay;
		fadechannel.mDelay = 0;
	}

	if ( elapse == 0 )
		return _false;

	// Update tickcount.
	fadechannel.mTickcount += elapse;
	if ( fadechannel.mTickcount > fadechannel.mDuration )
		fadechannel.mTickcount = fadechannel.mDuration;

	return _true;
}

_bool SoundDevice::CreateDevice( _handle module, _dword maxchannels )
{
	if ( maxchannels > 64 )
		maxchannels = 64;

	// No sound device.
	if ( ::waveOutGetNumDevs( ) == 0 || ::midiOutGetNumDevs( ) == 0 )
		return _true;

	if ( SoundAPI::LoadFmod( module ) == _false )
		return _false;

	if ( SoundAPI::_FMODCreateSystem( &mFmodSystem ) != 0 )
		return _false;

	if ( SoundAPI::_FMODInitSystem( mFmodSystem, maxchannels, FMOD_INIT_NORMAL, _null ) != 0 )
		return _false;

	if ( SoundAPI::_FMODSetFileSystem( mFmodSystem, (FMOD_FILE_OPENCALLBACK) OnOpenStream, (FMOD_FILE_CLOSECALLBACK) OnCloseStream, (FMOD_FILE_READCALLBACK) OnReadStream, (FMOD_FILE_SEEKCALLBACK) OnSeekStream, 8192 ) != 0 )
		return _false;

	// Get master group from system.
	if ( SoundAPI::_FMODGetMasterChannelGroup( mFmodSystem, &mMasterGroup ) != 0 )
		return _false;

	mMaxChannelNumber = maxchannels;

	return _true;
}

_void SoundDevice::Release( )
{
	delete this;
}

_void SoundDevice::Update( _dword elapse )
{
	mAccumulatedElapse += elapse;

	// Play delay sound.
	mDelaySoundLock.Enter( );
	_dword number = mDelayPlayQueue.Number( );
	mDelaySoundLock.Leave( );
	
	_qword startcycle	= System::GetCurrentCycleCount( );
	_qword cyclefreq	= System::GetSystemCycleFrequency( );

	String currentgroup = mCurrentGroupName;

	// Process all delay sound.
	for ( _dword i = 0; i < number; i ++ )
	{
		// Get a sound to play.
		mDelaySoundLock.Enter( );
		DelaySound delaysound = mDelayPlayQueue.Front( );
		mDelayPlayQueue.Remove( );
		mDelaySoundLock.Leave( );

		// Dont need play this sound.
		if ( delaysound.mTickcount == -1 )
		{
			mSoundResourceManager->RemoveResource( delaysound.mToken );
			continue;
		}

		// It is too late to play sound now, and it is not loop, just skip it.
		if ( System::GetCurrentTickcount( ) - delaysound.mTickcount > 500 && ( delaysound.mFlag & _SOUND_LOOP_ENABLE ) == 0 )
		{
			mSoundResourceManager->RemoveResource( delaysound.mToken );
			continue;
		}

		// Check sound channel number.
		if ( GetPlayingChannelNumber( L"" ) < mMaxChannelNumber )
		{
			SetCurrentSoundGroup( delaysound.mSoundGroup );

			// Create sound buffer from memory.
			//BinFile memfile;

			//IDataStream* stream = mSoundResourceManager->ObtainSoundStream( delaysound.mToken );
			//stream->Mapping( memfile );
			//ISoundBuffer* soundbuffer = CreateSoundBuffer( memfile, delaysound.mFlag );

			ISoundBuffer* soundbuffer = CreateSoundBuffer( delaysound.mFileName, delaysound.mFlag | _SOUND_STREAM );

			if ( soundbuffer != _null )
			{
				// Play the sound buffer.
				PlaySoundBuffer( soundbuffer, delaysound.mPosition, _null, delaysound.mFlag );

				// Decrease reference count, means can be auto deleted when play over.
				ReleaseSoundBuffer( soundbuffer );
			}
		}

		mSoundResourceManager->RemoveResource( delaysound.mToken );

		// Get process time, in millisecond.
		_float taketime = (_float) ( System::GetCurrentCycleCount( ) - startcycle ) / (_float) cyclefreq * 1000.0f;

		// Already process enough time.
		if ( taketime > 10.0f )
			break;
	}

	SetCurrentSoundGroup( currentgroup );

	// Process all fading in sound.
	for ( _dword i = 0; i < mFadeInArray.Number( ); i ++ )
	{
		FadeChannel& fadechannel = mFadeInArray[i];

		if ( UpdateFadeChannel( fadechannel, elapse ) == _false )
			continue;

		_float volumn = 1.0f;

		if ( fadechannel.mDuration != 0 )
			volumn = (_float) fadechannel.mTickcount / (_float) fadechannel.mDuration;

		SoundAPI::_FMODSetChannelPaused( fadechannel.mFmodChannel, _false );
		SoundAPI::_FMODSetChannelVolume( fadechannel.mFmodChannel, volumn );

		if ( fadechannel.mTickcount == fadechannel.mDuration )
			mFadeInArray.Remove( i -- );
	}

	// Process all fading out sound.
	for ( _dword i = 0; i < mFadeOutArray.Number( ); i ++ )
	{
		FadeChannel& fadechannel = mFadeOutArray[i];

		if ( UpdateFadeChannel( fadechannel, elapse ) == _false )
			continue;

		_float volumn = 1.0f;

		if ( fadechannel.mDuration != 0 )
			volumn = (_float) fadechannel.mTickcount / (_float) fadechannel.mDuration;

		SoundAPI::_FMODSetChannelVolume( fadechannel.mFmodChannel, 1.0f - volumn );

		if ( fadechannel.mTickcount == fadechannel.mDuration )
		{
			StopSoundChannel( fadechannel.mFmodChannel );

			mFadeOutArray.Remove( i -- );
		}
	}

	// Check cache every 500 millisecond.
	if ( mAccumulatedElapse >= 500 )
	{
		elapse = mAccumulatedElapse;
		mAccumulatedElapse = 0;

		_bool changemute = _false;

		WINDOWPLACEMENT windowplacement = { 0 };
		::GetWindowPlacement( ::GetActiveWindow( ), &windowplacement );

		if ( windowplacement.showCmd == SW_SHOWMINIMIZED || windowplacement.showCmd == SW_HIDE )
		{
			if ( mAutoMute == _false )
			{
				mAutoMute = _true;
				changemute	= _true;
			}
		}
		else
		{
			if ( mAutoMute == _true )
			{
				mAutoMute = _false;
				changemute	= _true;
			}
		}

		SoundAPI::_FMODUpdateSystem( mFmodSystem );

		for ( _dword i = 0; i < mMaxChannelNumber; i ++ )
		{
			FMOD_CHANNEL* fmodchannel = _null;

			if ( SoundAPI::_FMODGetChannel( mFmodSystem, i, &fmodchannel ) != 0 || fmodchannel == _null )
				continue;

			if ( changemute == _true )
				SoundAPI::_FMODSetChannelMute( fmodchannel, mAutoMute );

			_bool playing = _false;

			// Return value is zero means the channel is playing.
			if ( SoundAPI::_FMODIsChannelPlaying( fmodchannel, &playing ) == 0 )
				continue;

			StopSoundChannel( fmodchannel );
		}
	}
}

_bool SoundDevice::Set3DSettings( _float dopplerscale, _float distancefactor, _float rolloffscale, _float mindistance, _float maxdistance )
{
	mMinDistance = mindistance;
	mMaxDistance = maxdistance;

	return SoundAPI::_FMODSet3DSettings( mFmodSystem, dopplerscale, distancefactor, rolloffscale ) == 0;
}

_bool SoundDevice::Get3DSettings( _float& dopplerscale, _float& distancefactor, _float& rolloffscale, _float& mindistance, _float& maxdistance )
{
	mindistance = mMinDistance;
	maxdistance = mMaxDistance;

	return SoundAPI::_FMODGet3DSettings( mFmodSystem, &dopplerscale, &distancefactor, &rolloffscale ) == 0;
}

_bool SoundDevice::SetListenerPosition( const Vector3& position, const Vector3& forward, const Vector3& up )
{
	Vector3 velocity = Vector3::cOrigin;

	return SoundAPI::_FMODSetListenerPosition( mFmodSystem, 0, &position, &velocity, &forward, &up ) == 0;
}

_bool SoundDevice::GetListenerPosition( Vector3& position, Vector3& forward, Vector3& up )
{
	Vector3 velocity = Vector3::cOrigin;

	return SoundAPI::_FMODGetListenerPosition( mFmodSystem, 0, &position, &velocity, &forward, &up ) == 0;
}

ISoundBuffer* SoundDevice::CreateSoundBuffer( StringPtr filename, _dword flags )
{
	_dword compressflag = Compression::_COMPRESS_NO;

	if ( GetResourceManagerPointer( ) != _null )
		GetResourceManager( ).GetRealPathFromLogicPath( filename, _null, _null, &compressflag );

	// Create sound buffer.
	if ( ( flags & _SOUND_STREAM ) == 0 || compressflag == Compression::_COMPRESS_YES )
	{
		BinFile soundfile;

		if ( GetResourceManagerPointer( ) != _null )
		{
			if ( GetResourceManager( ).LoadResource( filename, soundfile ) == _false )
				return _null;
		}
		else
		{
			if ( soundfile.Open( filename ) == _false )
				return _null;
		}

		if ( soundfile.GetLength( ) == 0 )
			return _null;

		return CreateSoundBuffer( soundfile, flags );
	}
	// Create sound stream.
	else
	{
		// Set fmod flag.
		_dword fmodflag = 0;

		if ( flags & _SOUND_LOOP_DISABLE )
			fmodflag |= FMOD_LOOP_OFF;
		if ( flags & _SOUND_LOOP_ENABLE )
			fmodflag |= FMOD_LOOP_NORMAL;
		if ( flags & _SOUND_2D )
			fmodflag |= FMOD_2D;
		if ( flags & _SOUND_3D )
			fmodflag |= FMOD_3D;

		FMOD_SOUND* fmodsound = _null;
		if ( SoundAPI::_FMODCreateSound( mFmodSystem, (const _char*) filename, FMOD_HARDWARE | FMOD_UNICODE | FMOD_CREATESTREAM | fmodflag, _null, &fmodsound ) != 0 )
			return _null;

		SoundBuffer* soundbuffer = new SoundBuffer( filename, fmodsound );

		return soundbuffer;
	}
}

ISoundBuffer* SoundDevice::CreateSoundBuffer( const MemFile& soundfile, _dword flags )
{
	// Set fmod flag.
	_dword fmodflag = 0;

	if ( flags & _SOUND_LOOP_DISABLE )
		fmodflag |= FMOD_LOOP_OFF;
	if ( flags & _SOUND_LOOP_ENABLE )
		fmodflag |= FMOD_LOOP_NORMAL;
	if ( flags & _SOUND_2D )
		fmodflag |= FMOD_2D;
	if ( flags & _SOUND_3D )
		fmodflag |= FMOD_3D;

	// Prepare information to create sound in memory.
	FMOD_CREATESOUNDEXINFO createsoundinfo = { sizeof( FMOD_CREATESOUNDEXINFO ), soundfile.GetLength( ) };

	// Create sound buffer.
	FMOD_SOUND* fmodsound = _null;
	if ( SoundAPI::_FMODCreateSound( mFmodSystem, soundfile.GetBuffer( ), FMOD_HARDWARE | FMOD_OPENMEMORY | fmodflag, &createsoundinfo, &fmodsound ) != 0 )
		return _null;

	SoundBuffer* soundbuffer = new SoundBuffer( soundfile.GetFileName( ), fmodsound );

	return soundbuffer;
}

ISoundBuffer* SoundDevice::CloneSoundBuffer( ISoundBuffer* soundbuffer )
{
	if ( soundbuffer == _null )
		return _null;

	// Increase reference count.
	( (SoundBuffer*) soundbuffer )->IncreaseRefCount( );

	return soundbuffer;
}

_void SoundDevice::ReleaseSoundBuffer( ISoundBuffer*& soundbuffer )
{
	if ( soundbuffer == _null )
		return;

	if ( ( (SoundBuffer*) soundbuffer )->DecreaseRefCount( ) == 0 )
		delete (SoundBuffer*) soundbuffer;

	soundbuffer = _null;
}

_bool SoundDevice::PlaySoundBuffer( ISoundBuffer* soundbuffer, ISoundChannel** soundchannel, _dword flags )
{
	return PlaySoundBuffer( soundbuffer, Vector3::cMaxVector, soundchannel, flags );
}

_bool SoundDevice::PlaySoundBuffer( ISoundBuffer* soundbuffer, const Vector3& position, ISoundChannel** soundchannel, _dword flags )
{
	if ( soundbuffer == _null )
		return _false;

	_bool fadein = ( flags & _SOUND_FADEIN ) != 0;
	_bool paused = fadein || mAutoMute || mGlobalMute;

	// Play sound in a free channel.
	FMOD_CHANNEL* fmodchannel = _null;
	if ( SoundAPI::_FMODPlaySound( mFmodSystem, FMOD_CHANNEL_FREE, ( (SoundBuffer*) soundbuffer )->GetFmodSound( ), paused, &fmodchannel ) != 0 )
		return _false;

	if ( mAutoMute || mGlobalMute )
		SoundAPI::_FMODSetChannelMute( fmodchannel, _true );

	if ( fadein == _false )
		SoundAPI::_FMODSetChannelPaused( fmodchannel, _false );

	// Increase reference count.
	( (SoundBuffer*) soundbuffer )->IncreaseRefCount( );

	if ( fmodchannel != _null )
	{
		if ( mCurrentGroup != _null )
			SoundAPI::_FMODSetChannelGroup( fmodchannel, mCurrentGroup );

		SoundAPI::_FMODSetChannelUserData( fmodchannel, soundbuffer );
		SoundAPI::_FMODSetSoundUserData( ( (SoundBuffer*) soundbuffer )->GetFmodSound( ), mCurrentGroup );

		// Sound 
		if ( position != Vector3::cMaxVector )
		{
			Vector3 velocity = Vector3::cOrigin;

			SoundAPI::_FMODSetChannel3DAttributes( fmodchannel, &position, &velocity );
		}

		// We should fade in this sound.
		if ( flags & _SOUND_FADEIN )
		{
			SoundAPI::_FMODSetChannelVolume( fmodchannel, 0.0f );

			FadeChannel fadechannel;
			fadechannel.mFmodChannel	= fmodchannel;
			fadechannel.mDelay			= mFadeInDelay;
			fadechannel.mTickcount		= 0;
			fadechannel.mDuration		= mFadeInTime;

			mFadeInArray.Append( fadechannel );
		}
	}

	if ( soundchannel != _null )
		*soundchannel = new SoundChannel( fmodchannel );

	return _true;
}

_void SoundDevice::ReleaseSoundChannel( ISoundChannel*& soundchannel )
{
	delete (SoundChannel*) soundchannel;
	soundchannel = _null;
}

_bool SoundDevice::PlaySoundBuffer( StringPtr filename, _dword flags )
{
	return PlaySoundBuffer( filename, Vector3::cMaxVector, flags );
}

_bool SoundDevice::PlaySoundBuffer( StringPtr filename, const Vector3& position, _dword flags )
{
	if ( mGlobalMute && ( flags & _SOUND_LOOP_ENABLE ) == 0 )
		return _false;

	if ( GetPlayingChannelNumber( L"" ) == GetMaxChannelNumber( ) )
		return _false;

	if ( position != Vector3::cMaxVector )
	{
		Vector3 listener, forward, up;
		GetListenerPosition( listener, forward, up );

		// The distance is too far, we can not hear the sound, so we just skip it.
		if ( Vector3( listener - position ).Magnitude( ) > mMaxDistance * 0.8f )
			return _false;
	}

	// Play the sound right now.
	if ( ( flags & _SOUND_PLAYNOW ) || ( flags & _SOUND_STREAM ) )
	{
		ISoundBuffer* soundbuffer = CreateSoundBuffer( filename, flags );
		if ( soundbuffer == _null )
			return _false;

		_bool retval = PlaySoundBuffer( soundbuffer, position, _null, flags );

		// Decrease reference count, means can be auto deleted when play over.
		ReleaseSoundBuffer( soundbuffer );

		return retval;
	}
	// Delay the sound, process at background thread.
	else
	{
		// Create play thead as needed.
		if ( mDelaySoundThread.GetThreadID( ) == 0 )
		{
			mDelaySoundThread.Create( 1 );

			// Post load request to the thread.
			if ( mDelaySoundThread.GetThreadID( ) != 0 )
				mDelaySoundThread.RegisterUpdateFunc( OnLoadSoundThread, (_void*) this, _null );
		}

		DelaySound delaysound;
		delaysound.mSoundGroup	= mCurrentGroupName;
		delaysound.mFileName	= filename;
		delaysound.mPosition	= position;
		delaysound.mFlag		= flags;
		delaysound.mTickcount	= System::GetCurrentTickcount( );
		delaysound.mToken		= 0;

		mDelaySoundLock.Enter( );
		mDelayLoadQueue.Append( delaysound );
		mDelaySoundLock.Leave( );
	}

	return _true;
}

_bool SoundDevice::SetCurrentSoundGroup( StringPtr groupname )
{
	if ( mCurrentGroupName == groupname )
		return _true;

	mCurrentGroupName = groupname;

	if ( groupname == L"" )
	{
		mCurrentGroup = _null;
		return _true;
	}

	mCurrentGroup = CreateSoundGroup( groupname );

	return mCurrentGroup != _null;
}

StringPtr SoundDevice::GetCurrentSoundGroup( ) const
{
	return mCurrentGroupName;
}

_bool SoundDevice::StopSoundGroup( StringPtr groupname, _dword flag )
{
	mDelaySoundLock.Enter( );

	// Stop delay sound.
	for ( DelaySoundQueue::Iterator it = mDelayLoadQueue.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		DelaySound& delaysound = it;

		if ( delaysound.mSoundGroup.CompareLowercase( groupname ) )
			delaysound.mTickcount = -1;
	}

	// Stop delay sound.
	for ( DelaySoundQueue::Iterator it = mDelayPlayQueue.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		DelaySound& delaysound = it;

		if ( delaysound.mSoundGroup.CompareLowercase( groupname ) )
			delaysound.mTickcount = -1;
	}

	mDelaySoundLock.Leave( );

	FMOD_CHANNELGROUP* currentgroup = mMasterGroup;

	if ( groupname != L"" )
		currentgroup = SearchSoundGroup( groupname );

	if ( currentgroup == _null )
		return _false;

	// Stop all playing channel.
	for ( _dword i = 0; i < mMaxChannelNumber; i ++ )
	{
		FMOD_CHANNEL* fmodchannel = _null;
		if ( SoundAPI::_FMODGetChannel( mFmodSystem, i, &fmodchannel ) != 0 || fmodchannel == _null )
			continue;

		FMOD_CHANNELGROUP* parentgroup = _null;
		if ( SoundAPI::_FMODGetChannelGroup( fmodchannel, &parentgroup ) != 0 )
			continue;

		if ( parentgroup != currentgroup )
			continue;

		#ifdef _DEBUG_SOUND
		PS_TRACE1( L"    Stop channel %p\r\n", (_void*) fmodchannel )
		#endif

		StopSoundChannel( fmodchannel );
	}

	SoundAPI::_FMODStopChannelGroup( currentgroup );

	return _true;
}

_bool SoundDevice::StopAllSoundGroup( )
{
	// Stop all playing channel.
	for ( _dword i = 0; i < mMaxChannelNumber; i ++ )
	{
		FMOD_CHANNEL* fmodchannel = _null;

		if ( SoundAPI::_FMODGetChannel( mFmodSystem, i, &fmodchannel ) != 0 || fmodchannel == _null )
			continue;

		StopSoundChannel( fmodchannel );
	}

	for ( _dword i = 0; i < mSoundGroupArray.Number( ); i ++ )
	{
		FMOD_CHANNELGROUP* channelgroup = mSoundGroupArray[i].mObject2;

		// Release the group.
		SoundAPI::_FMODReleaseChannelGroup( channelgroup );
	}

	mCurrentGroup = _null;

	// Clear sound group array.
	mSoundGroupArray.Clear( );

	return _true;
}

_bool SoundDevice::SetGlobalMute( _bool mute )
{
	mGlobalMute = mute;

	if ( SoundAPI::_FMODOverrideChannelGroupMute( mMasterGroup, mute ) != 0 )
		return _false;

	for ( _dword i = 0; i < mSoundGroupArray.Number( ); i ++ )
		SoundAPI::_FMODOverrideChannelGroupMute( mSoundGroupArray[i].mObject2, mute );

	return _true;
}

_bool SoundDevice::GetGlobalMute( ) const
{
	return mGlobalMute;
}

_bool SoundDevice::SetGlobalVolume( _float volume )
{
	if ( volume < 0.0f )
		volume = 0.0f;

	mGlobalVolume = volume;

	if ( SoundAPI::_FMODSetChannelGroupVolume( mMasterGroup, volume ) != 0 )
		return _false;

	for ( _dword i = 0; i < mSoundGroupArray.Number( ); i ++ )
		SoundAPI::_FMODSetChannelGroupVolume( mSoundGroupArray[i].mObject2, volume );

	return _true;
}

_float SoundDevice::GetGlobalVolume( ) const
{
	return mGlobalVolume;
}

_bool SoundDevice::SetSoundGroupMute( StringPtr groupname, _bool mute )
{
	FMOD_CHANNELGROUP* group = SearchSoundGroup( groupname );

	if ( group == _null )
		return _false;

	return SoundAPI::_FMODOverrideChannelGroupMute( group, mute ) == 0;
}

_bool SoundDevice::SetSoundGroupVolume( StringPtr groupname, _float volume )
{
	FMOD_CHANNELGROUP* group = SearchSoundGroup( groupname );

	if ( group == _null )
		return _false;

	if ( volume < 0.0f )
		volume = 0.0f;

	return SoundAPI::_FMODSetChannelGroupVolume( group, volume * mGlobalVolume ) == 0;
}

_float SoundDevice::GetSoundGroupVolume( StringPtr groupname ) const
{
	FMOD_CHANNELGROUP* group = SearchSoundGroup( groupname );

	if ( group == _null )
		return _false;

	_float volume = 0.0f;

	if ( SoundAPI::_FMODGetChannelGroupVolume( group, &volume ) != 0 )
		return 0.0f;

	return volume;
}

_void SoundDevice::SetFadeInTime( _dword time )
{
	mFadeInTime = time;
}

_void SoundDevice::SetFadeInDelay( _dword delay )
{
	mFadeInDelay = delay;
}

_void SoundDevice::SetFadeOutTime( _dword time )
{
	mFadeOutTime = time;
}

_void SoundDevice::SetFadeOutDelay( _dword delay )
{
	mFadeOutDelay = delay;
}

_dword SoundDevice::GetPlayingChannelNumber( StringPtr groupname ) const
{
	_dword number = 0;

	if ( groupname == L"" )
	{
		SoundAPI::_FMODGetPlayingChannelNumber( mFmodSystem, &number );
	}
	else
	{
		FMOD_CHANNELGROUP* channelgroup = SearchSoundGroup( groupname );

		if ( channelgroup != _null )
			SoundAPI::_FMODGetChannelNumberInGroup( channelgroup, &number );
	}

	return number;
}

_dword SoundDevice::GetMaxChannelNumber( ) const
{
	return mMaxChannelNumber;
}