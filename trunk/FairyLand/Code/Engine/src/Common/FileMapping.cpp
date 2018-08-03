//============================================================================
// FileMapping.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// FileMapping Implementation
//----------------------------------------------------------------------------

FileMapping::FileMapping( ) : mFileHandle( INVALID_HANDLE_VALUE ), mBuffer( _null ), mLength( 0 )
{
}

FileMapping::~FileMapping( )
{
	Close( );
}

_bool FileMapping::Open( StringPtr filename )
{
	Close( );

	// Open the file.
	mFileHandle = ::CreateFile( filename, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, _null, OPEN_EXISTING, 0, _null );
	if ( mFileHandle == INVALID_HANDLE_VALUE )
		return _false;

	// Get file mapping size.
	mLength = ::GetFileSize( mFileHandle, _null );

	// Create file mapping.
	mObjectHandle = ::CreateFileMapping( mFileHandle, _null, PAGE_READONLY, 0, mLength, _null );
	if ( mObjectHandle == _null )
		return _false;

	// Map whole file.
	mBuffer = (_byte*) ::MapViewOfFile( mObjectHandle, FILE_MAP_READ, 0, 0, 0 );

	return mBuffer != _null;
}

_bool FileMapping::Open( const File& file )
{
	// Duplicate the file handle.
	::DuplicateHandle( ::GetCurrentProcess( ), (_handle) file, ::GetCurrentProcess( ), &mFileHandle, 0, 0, DUPLICATE_SAME_ACCESS );
	if ( mFileHandle == INVALID_HANDLE_VALUE )
		return _false;

	// Get file mapping size.
	mLength = ::GetFileSize( mFileHandle, _null );

	// Create file mapping.
	mObjectHandle = ::CreateFileMapping( mFileHandle, _null, PAGE_READONLY, 0, mLength, _null );
	if ( mObjectHandle == _null )
		return _false;

	// Map whole file.
	mBuffer = (_byte*) ::MapViewOfFile( mObjectHandle, FILE_MAP_READ, 0, 0, 0 );

	return mBuffer != _null;
}

_void FileMapping::Close( )
{
	if ( mBuffer != _null )
		::UnmapViewOfFile( mBuffer );

	if ( mObjectHandle != _null )
		::CloseHandle( mObjectHandle );

	if ( mFileHandle != INVALID_HANDLE_VALUE )
		::CloseHandle( mFileHandle );

	mObjectHandle	= INVALID_HANDLE_VALUE;
	mFileHandle		= _null;
	mBuffer			= _null;
	mLength			= 0;
}