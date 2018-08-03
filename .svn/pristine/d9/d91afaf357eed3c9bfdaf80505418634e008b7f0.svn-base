//============================================================================
// ISoundBuffer.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ISoundBuffer
//----------------------------------------------------------------------------

class ISoundBuffer
{
public:
	virtual StringPtr GetName( ) const						= 0;

	virtual _bool EnableLoop( _bool loop )					= 0;

	virtual _bool GetLength( _dword& millisecond )			= 0;
	virtual _bool Set3DDistance( _float min, _float max )	= 0;
	virtual _bool Get3DDistance( _float& min, _float& max )	= 0;
};

};