//============================================================================
// PackStream.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// PackStream Implementation
//----------------------------------------------------------------------------

PackStream::PackStream( StringPtr name, Oword& checksum, File& file, File& asyncreadfile, File& asyncwritefile, _dword size, _dword base )
	: mName( name ), mChecksum( checksum ), mFile( file ), mAsyncReadFile( asyncreadfile ), mAsyncWriteFile( asyncwritefile ), mSize( size ), mBase( base )
{
	mName.Lowercase( );
}

PackStream::~PackStream( )
{
}

_void PackStream::Release( )
{
	delete this;
}

_dword PackStream::GetStreamType( ) const
{
	return IResourceManager::_ARCHIVE_PACK;
}

StringPtr PackStream::GetName( ) const
{
	return mName;
}

Oword PackStream::GetChecksum( ) const
{
	return mChecksum;
}

_byte* PackStream::GetBuffer( ) const
{
	return _null;
}

_dword PackStream::GetSize( ) const
{
	return mSize;
}

_bool PackStream::Read( _void* buffer, _dword length, _dword offset, _dword* finish )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	if ( finish != _null )
	{
		manager->GetAsyncIOManager( ).PostAsyncRead( mAsyncReadFile, mBase + offset, (_byte*) buffer, length, finish );
	}
	else
	{
		LockOwner lockowner( manager->GetResourceLock( ) );

		// Should not exceed file size.
		if ( offset + length > mSize )
			return _false;

		if ( mFile.SeekFromBegin( mBase + offset ) == _false )
			return _false;

		// Read data.
		if ( mFile.Read( buffer, length ) == _false )
			return _false;
	}

	return _true;
}

_bool PackStream::Write( const _void* buffer, _dword length, _dword offset, _dword* finish )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	if ( finish != _null )
	{
		manager->GetAsyncIOManager( ).PostAsyncWrite( mAsyncWriteFile, mBase + offset, _null, 0, (_byte*) buffer, length, finish );
	}
	else
	{
		LockOwner lockowner( manager->GetResourceLock( ) );

		// Should not exceed file size.
		if ( offset + length > mSize )
			return _false;

		if ( mFile.SeekFromBegin( mBase + offset ) == _false )
			return _false;

		// Write data.
		if ( mFile.Write( buffer, length ) == _false )
			return _false;
	}

	return _true;
}

_bool PackStream::IsCompressed( )
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

_bool PackStream::Mapping( MemFile& memfile, _bool decompress )
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

_bool PackStream::Unmapping( MemFile& memfile, _bool compress )
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

_bool PackStream::Check( const Oword& checksum, _bool withname, _dword* finish )
{
	AsyncIOManager& asynciomanager = ( (ResourceManager*) GetResourceManagerPointer( ) )->GetAsyncIOManager( );

	// Only do sync check.
	_dword result = 0;
	asynciomanager.ProcessCheck( this, checksum, withname, &result );

	if ( finish != _null )
		*finish = result;

	return result == IResourceManager::_ASYNC_DONE_TRUE;
}