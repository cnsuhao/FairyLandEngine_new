//============================================================================
// FileFinder.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// FileInfo
//----------------------------------------------------------------------------

struct FileInfo
{
	String	mName;
	_dword	mSize;
	_dword	mAttributes;
};

//----------------------------------------------------------------------------
// FileFinder
//----------------------------------------------------------------------------

class FileFinder
{
private:
	_handle		mFinderHandle;

public:
	FileFinder( );
	~FileFinder( );

	_bool FindFirst( StringPtr filename, FileInfo& fileinfo );
	_bool FindNext( FileInfo& fileinfo );
	_void Close( );
};

};