//============================================================================
// FairyLandSound.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandSound.h"

//----------------------------------------------------------------------------
// CreateSoundDevice Implementation
//----------------------------------------------------------------------------

ISoundDevice* CreateSoundDevice( _handle module, _dword maxchannels )
{
	ISoundDevice* sounddevice = GetSoundDevicePointer( );

	if ( sounddevice != _null )
		return sounddevice;

	sounddevice = new SoundDevice;

	if ( ( (SoundDevice*) sounddevice )->CreateDevice( module, maxchannels ) == _false )
	{
		sounddevice->Release( );
		return _null;
	}

	GetFairyLandGlobal( ).mSoundDevice = sounddevice;

	PS_TRACE( L"[KL] SoundDevice Created\r\n" )

	return sounddevice;
}