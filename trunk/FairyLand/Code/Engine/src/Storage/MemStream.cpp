//============================================================================
// MemStream.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// MemStream Implementation
//----------------------------------------------------------------------------

MemStream::MemStream( StringPtr name, const Oword& checksum, _byte* buffer, _dword size, _bool bufferowner )
	: mName( name ), mChecksum( checksum ), mSize( size ), mBuffer( buffer ), mBufferOwner( bufferowner )
{
	if ( mBufferOwner && mBuffer != _null )
		Memory::ReserveReferencedBuffer( mBuffer );

	mName.Lowercase( );
}

MemStream::~MemStream( )
{
	if ( mBufferOwner && mBuffer != _null )
		Memory::ReleaseReferencedBuffer( mBuffer );
}

_void MemStream::Release( )
{
	delete this;
}

_dword MemStream::GetStreamType( ) const
{
	return IResourceManager::_ARCHIVE_MEM;
}

StringPtr MemStream::GetName( ) const
{
	return mName;
}

Oword MemStream::GetChecksum( ) const
{
	return mChecksum;
}

_byte* MemStream::GetBuffer( ) const
{
	return mBuffer;
}

_dword MemStream::GetSize( ) const
{
	return mSize;
}

_bool MemStream::Read( _void* buffer, _dword length, _dword offset, _dword* finish )
{
	if ( mBuffer == _null )
		return _false;

	// Should not exceed file size.
	if ( offset + length > mSize )
		return _false;

	Memory::MemCpy( buffer, mBuffer + offset, length );

	return _true;
}

_bool MemStream::Write( const _void* buffer, _dword length, _dword offset, _dword* finish )
{
	if ( mBuffer == _null )
		return _false;

	// Should not exceed file size.
	if ( offset + length > mSize )
		return _false;

	Memory::MemCpy( mBuffer + offset, buffer, length );

	return _true;
}

_bool MemStream::IsCompressed( )
{
	// Size too small, should not compressed.
	if ( mSize < 20 )
		return _false;

	_byte buffer[16];

	// Read header from file.
	if ( Read( buffer, 16, 0 ) == _false )
		return _false;

	// Compare header.
	return Memory::MemCmp( FileSystem::cCompressedFileHeader, buffer, 16 );
}

_bool MemStream::Mapping( MemFile& memfile, _bool decompress )
{
	if ( decompress )
	{
		if ( memfile.Create( mSize ) == _false )
			return _false;

		if ( Read( memfile.GetBuffer( ), mSize, 0 ) == _false )
			return _false;

		if ( Compression::Decompress( memfile ) == _false )
			return _false;
	}
	else
	{
		memfile.Open( mBuffer, mSize, _false );
		memfile.EnableAutoDeleteBuffer( _false );
	}

	return _true;
}

_bool MemStream::Unmapping( MemFile& memfile, _bool compress )
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

_bool MemStream::Check( const Oword& checksum, _bool withname, _dword* finish )
{
	AsyncIOManager& asynciomanager = ( (ResourceManager*) GetResourceManagerPointer( ) )->GetAsyncIOManager( );

	if ( finish == _null )
	{
		// Do sync check.
		_dword result = 0;
		asynciomanager.ProcessCheck( this, checksum, withname, &result );

		return result == IResourceManager::_ASYNC_DONE_TRUE;
	}
	else
	{
		// Post async check.
		asynciomanager.PostAsyncCheck( this, checksum, withname, finish );

		return _true;
	}
}