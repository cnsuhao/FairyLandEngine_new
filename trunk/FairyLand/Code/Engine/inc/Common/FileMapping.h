//============================================================================
// FileMapping.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// FileMapping
//----------------------------------------------------------------------------

class FileMapping : public KernelObject
{
private:
	_handle	mFileHandle;
	_byte*	mBuffer;
	_dword	mLength;

public:
	FileMapping( );
	~FileMapping( );

	inline _byte* GetBuffer( ) const;
	inline _dword GetLength( ) const;

	_bool Open( StringPtr filename );
	_bool Open( const File& file );

	_void Close( );
};

_byte* FileMapping::GetBuffer( ) const
{
	return mBuffer;
}

_dword FileMapping::GetLength( ) const
{
	return mLength;
}

};