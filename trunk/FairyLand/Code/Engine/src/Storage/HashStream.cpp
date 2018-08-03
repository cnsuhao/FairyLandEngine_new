//============================================================================
// HashStream.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// HashStream Implementation
//----------------------------------------------------------------------------

HashStream::HashStream( StringPtr name, Oword& checksum, File& file, _dword size, _dword base )
	: mName( name ), mChecksum( checksum ), mFile( file ), mSize( size ), mBase( base )
{
	mName.Lowercase( );
}

HashStream::~HashStream( )
{
}

_void HashStream::Release( )
{
	delete this;
}

_dword HashStream::GetStreamType( ) const
{
	return IResourceManager::_ARCHIVE_HASH;
}

StringPtr HashStream::GetName( ) const
{
	return mName;
}

Oword HashStream::GetChecksum( ) const
{
	return mChecksum;
}

_byte* HashStream::GetBuffer( ) const
{
	return _null;
}

_dword HashStream::GetSize( ) const
{
	return mSize;
}

_bool HashStream::Read( _void* buffer, _dword length, _dword offset, _dword* finish )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Should not exceed file size.
	if ( offset + length > mSize )
		return _false;

	if ( mFile.SeekFromBegin( mBase + offset ) == _false )
		return _false;

	// Read data.
	if ( mFile.Read( buffer, length ) == _false )
		return _false;

	return _true;
}

_bool HashStream::Write( const _void* buffer, _dword length, _dword offset, _dword* finish )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Should not exceed file size.
	if ( offset + length > mSize )
		return _false;

	if ( mFile.SeekFromBegin( mBase + offset ) == _false )
		return _false;

	// Write data.
	if ( mFile.Write( buffer, length ) == _false )
		return _false;

	return _true;
}

_bool HashStream::IsCompressed( )
{
	if ( mSize < 20 )
		return _false;

	_byte buffer[16];

	// Read header from file.
	if ( Read( buffer, 16, 0 ) == _false )
		return _false;

	// Compare header.
	return Memory::MemCmp( FileSystem::cCompressedFileHeader, buffer, 16 );
}

_bool HashStream::Mapping( MemFile& memfile, _bool decompress )
{
	if ( memfile.Create( mSize ) == _false )
		return _false;

	if ( Read( memfile.GetBuffer( ), mSize, 0 ) == _false )
		return _false;

	if ( decompress == _true )
	{
		if ( Compression::Decompress( memfile ) == _false )
			return _false;
	}

	return _true;
}

_bool HashStream::Unmapping( MemFile& memfile, _bool compress )
{
	if ( compress == _true )
	{
		if ( Compression::Compress( memfile, 1.0f ) == _false )
			return _false;
	}

	if ( memfile.GetLength( ) != mSize )
		return _false;

	if ( Write( memfile.GetBuffer( ), mSize, 0 ) == _false )
		return _false;

	return _true;
}

_bool HashStream::Check( const Oword& checksum, _bool withname, _dword* finish )
{
	AsyncIOManager& asynciomanager = ( (ResourceManager*) GetResourceManagerPointer( ) )->GetAsyncIOManager( );

	// Only do sync check.
	_dword result = 0;
	asynciomanager.ProcessCheck( this, checksum, withname, &result );

	if ( finish != _null )
		*finish = result;

	return result == IResourceManager::_ASYNC_DONE_TRUE;
}