//============================================================================
// SoundBuffer.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// SoundBuffer
//----------------------------------------------------------------------------

class SoundBuffer : public ISoundBuffer, public ReferencedObject
{
private:
	String		mName;
	FMOD_SOUND*	mFmodSound;

public:
	SoundBuffer( StringPtr name, FMOD_SOUND* fmodsound );
	~SoundBuffer( );

	inline FMOD_SOUND* GetFmodSound( ) const;

	virtual StringPtr GetName( ) const;

	virtual _bool EnableLoop( _bool loop );

	virtual _bool GetLength( _dword& millisecond );
	virtual _bool Set3DDistance( _float min, _float max );
	virtual _bool Get3DDistance( _float& min, _float& max );
};

//----------------------------------------------------------------------------
// SoundBuffer Implementation
//----------------------------------------------------------------------------

FMOD_SOUND* SoundBuffer::GetFmodSound( ) const
{
	return mFmodSound;
}

};