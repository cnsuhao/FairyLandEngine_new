//============================================================================
// SoundResource.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandSound.h"

//----------------------------------------------------------------------------
// SoundResourceManager Implementation
//----------------------------------------------------------------------------

SoundResourceManager::SoundResourceManager( )
{
}

SoundResourceManager::~SoundResourceManager( )
{
	for ( _dword i = 0; i < mResourceArray.Number( ); i ++ )
	{
		SoundResource* soundres = mResourceArray[i].mObject2;

		soundres->mSoundStream->Release( );
		delete soundres;
	}
}

_dword SoundResourceManager::ObtainResource( StringPtr name )
{
	LockOwner lockowner( mResourceLock );

	// Convert logic game to real name.
	_char lowercasedname[1024];
	StringFormatter::CopyString( lowercasedname, name );
	StringFormatter::LowercaseString( lowercasedname );

	_dword index = mResourceArray.SearchAscending( lowercasedname ).mIndex;
	if ( index == -1 )
		return 0;

	SoundResource* soundres = mResourceArray[index].mObject2;
	soundres->IncreaseRefCount( );

	return (_dword) soundres;
}

_dword SoundResourceManager::InsertResource( StringPtr name, IDataStream* stream )
{
	LockOwner lockowner( mResourceLock );

	#ifdef _DEBUG_SOUND
	PS_TRACE1( L"    Insert sound resource %s\r\n", name )
	#endif

	SoundResource* soundres = new SoundResource;
	soundres->IncreaseRefCount( );

	soundres->mSoundStream	= stream;
	soundres->mSoundName	= name;
	soundres->mSoundName.Lowercase( );

	mResourceArray.InsertAscending( ResourcePair( soundres->mSoundName, soundres ) );

	return (_dword) soundres;
}

_void SoundResourceManager::RemoveResource( _dword token )
{
	SoundResource* soundres = (SoundResource*) token;
	soundres->DecreaseRefCount( );

	if ( soundres->GetRefCount( ) > 0 )
		return;

	LockOwner lockowner( mResourceLock );

	#ifdef _DEBUG_SOUND
	PS_TRACE1( L"    Remove sound resource %s\r\n", (const _char*) soundres->mSoundName )
	#endif

	_dword index = mResourceArray.SearchAscending( soundres->mSoundName ).mIndex;
	FL_ASSERT( index != -1 )

	mResourceArray.Remove( index );

	soundres->mSoundStream->Release( );
	delete soundres;
}

IDataStream* SoundResourceManager::ObtainSoundStream( _dword token )
{
	SoundResource* soundres = (SoundResource*) token;

	return soundres->mSoundStream;
}

_dword SoundResourceManager::GetResourceRefcount( _dword token )
{
	SoundResource* soundres = (SoundResource*) token;

	return soundres->GetRefCount( );
}