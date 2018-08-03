//============================================================================
// FileStream.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// FileStream Implementation
//----------------------------------------------------------------------------

FileStream::FileStream( StringPtr name, File& file ) : mName( name ), mFile( file )
{
	mName.Lowercase( );
}

FileStream::~FileStream( )
{
}

_void FileStream::Release( )
{
	delete this;
}

_dword FileStream::GetStreamType( ) const
{
	return IResourceManager::_ARCHIVE_FILE;
}

StringPtr FileStream::GetName( ) const
{
	return mName;
}

Oword FileStream::GetChecksum( ) const
{
	return Oword( 0, 0, 0, 0 );
}

_byte* FileStream::GetBuffer( ) const
{
	return _null;
}

_dword FileStream::GetSize( ) const
{
	return mFile.GetSize( );
}

_bool FileStream::Read( _void* buffer, _dword length, _dword offset, _dword* finish )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	if ( mFile.SeekFromBegin( offset ) == _false )
		return _false;

	// Read data.
	if ( mFile.Read( buffer, length ) == _false )
		return _false;

	return _true;
}

_bool FileStream::Write( const _void* buffer, _dword length, _dword offset, _dword* finish )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	if ( mFile.SeekFromBegin( offset ) == _false )
		return _false;

	// Write data.
	if ( mFile.Write( buffer, length ) == _false )
		return _false;

	return _true;
}

_bool FileStream::IsCompressed( )
{
	// Size too small, should not compressed.
	if ( mFile.GetSize( ) < 20 )
		return _false;

	_byte buffer[16];

	// Read header from file.
	if ( Read( buffer, 16, 0 ) == _false )
		return _false;

	// Compare header.
	return Memory::MemCmp( FileSystem::cCompressedFileHeader, buffer, 16 );
}

_bool FileStream::Mapping( MemFile& memfile, _bool decompress )
{
	_dword size = mFile.GetSize( );

	if ( memfile.Create( size ) == _false )
		return _false;

	if ( Read( memfile.GetBuffer( ), size, 0 ) == _false )
		return _false;

	if ( decompress == _true )
	{
		if ( Compression::Decompress( memfile ) == _false )
			return _false;
	}

	return _true;
}

_bool FileStream::Unmapping( MemFile& memfile, _bool compress )
{
	if ( compress == _true )
	{
		if ( Compression::Compress( memfile, 1.0f ) == _false )
			return _false;
	}

	if ( Write( memfile.GetBuffer( ), memfile.GetLength( ), 0 ) == _false )
		return _false;

	return _true;
}

_bool FileStream::Check( const Oword& checksum, _bool withname, _dword* finish )
{
	AsyncIOManager& asynciomanager = ( (ResourceManager*) GetResourceManagerPointer( ) )->GetAsyncIOManager( );

	// Only do sync check.
	_dword result = 0;
	asynciomanager.ProcessCheck( this, checksum, withname, &result );

	if ( finish != _null )
		*finish = result;

	return result == IResourceManager::_ASYNC_DONE_TRUE;
}