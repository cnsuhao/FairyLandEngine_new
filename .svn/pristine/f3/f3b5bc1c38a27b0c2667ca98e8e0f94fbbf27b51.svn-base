//============================================================================
// SoundResource.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// SoundResource
//----------------------------------------------------------------------------

struct SoundResource : public ReferencedObject
{
	String			mSoundName;
	IDataStream*	mSoundStream;

	SoundResource( ) : mSoundStream( _null ) { }
};

//----------------------------------------------------------------------------
// SoundResourceManager
//----------------------------------------------------------------------------

class SoundResourceManager
{
private:
	typedef Pair< String, SoundResource* >		ResourcePair;
	typedef Array< ResourcePair, StringPtr >	ResourceArray;

	ResourceArray	mResourceArray;
	Lock			mResourceLock;

public:
	SoundResourceManager( );
	~SoundResourceManager( );

	_dword			ObtainResource( StringPtr name );
	_dword			InsertResource( StringPtr name, IDataStream* stream );
	_void			RemoveResource( _dword token );

	IDataStream*	ObtainSoundStream( _dword token );
	_dword			GetResourceRefcount( _dword token );
};

};