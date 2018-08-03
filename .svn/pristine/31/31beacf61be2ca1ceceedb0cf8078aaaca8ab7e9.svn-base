//============================================================================
// FileFinder.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// FileFinder Implementation
//----------------------------------------------------------------------------

FileFinder::FileFinder( ) : mFinderHandle( INVALID_HANDLE_VALUE )
{
}

FileFinder::~FileFinder( )
{
	Close( );
}

_bool FileFinder::FindFirst( StringPtr filename, FileInfo& fileinfo )
{
	WIN32_FIND_DATA finddata;
	if ( ( mFinderHandle = ::FindFirstFile( filename, &finddata ) ) == INVALID_HANDLE_VALUE )
		return _false;

	fileinfo.mName			= finddata.cFileName;
	fileinfo.mSize			= finddata.nFileSizeLow;
	fileinfo.mAttributes	= finddata.dwFileAttributes;

	return _true;
}

_bool FileFinder::FindNext( FileInfo& fileinfo )
{
	WIN32_FIND_DATA finddata;
	if ( ::FindNextFile( mFinderHandle, &finddata ) == _false )
		return _false;

	fileinfo.mName			= finddata.cFileName;
	fileinfo.mSize			= finddata.nFileSizeLow;
	fileinfo.mAttributes	= finddata.dwFileAttributes;

	return _true;
}

_void FileFinder::Close( )
{
	if ( mFinderHandle != INVALID_HANDLE_VALUE )
		::FindClose( mFinderHandle );

	mFinderHandle = INVALID_HANDLE_VALUE;
}