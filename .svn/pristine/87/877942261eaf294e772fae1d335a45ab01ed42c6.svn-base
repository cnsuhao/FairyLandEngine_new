//============================================================================
// PEResStream.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// PEResStream Implementation
//----------------------------------------------------------------------------

PEResStream::PEResStream( StringPtr name, _handle resource, _dword size )
	: mName( name ), mResource( resource ), mSize( size )
{
	mName.Lowercase( );

	mBuffer = (_byte*) ::LockResource( resource );
}

PEResStream::~PEResStream( )
{
	::FreeResource( mResource );
}

_void PEResStream::Release( )
{
	delete this;
}

_dword PEResStream::GetStreamType( ) const
{
	return IResourceManager::_ARCHIVE_PERES;
}

StringPtr PEResStream::GetName( ) const
{
	return mName;
}

Oword PEResStream::GetChecksum( ) const
{
	return Oword( 0, 0, 0, 0 );
}

_byte* PEResStream::GetBuffer( ) const
{
	return mBuffer;
}

_dword PEResStream::GetSize( ) const
{
	return mSize;
}

_bool PEResStream::Read( _void* buffer, _dword length, _dword offset, _dword* finish )
{
	// Should not exceed file size.
	if ( offset + length > mSize )
		return _false;

	Memory::MemCpy( buffer, mBuffer + offset, length );

	return _true;
}

_bool PEResStream::Write( const _void* buffer, _dword length, _dword offset, _dword* finish )
{
	// Cant write to PE resource.

	return _false;
}

_bool PEResStream::IsCompressed( )
{
	// Size too small, should not compressed.
	if ( mSize < 20 )
		return _false;

	// Compare header.
	return Memory::MemCmp( FileSystem::cCompressedFileHeader, mBuffer, 16 );
}

_bool PEResStream::Mapping( MemFile& memfile, _bool decompress )
{
	if ( GetResourceManagerPointer( ) != _null )
		GetResourceManagerPointer( )->IncreaseIORead( mSize );

	if ( memfile.Open( mBuffer, mSize ) == _false )
		return _false;

	if ( decompress == _true )
	{
		if ( Compression::Decompress( memfile ) == _false )
			return _false;
	}

	return _true;
}

_bool PEResStream::Unmapping( MemFile& memfile, _bool compress )
{
	// Cant write to PE resource.

	return _false;
}

_bool PEResStream::Check( const Oword& checksum, _bool withname, _dword* finish )
{
	AsyncIOManager& asynciomanager = ( (ResourceManager*) GetResourceManagerPointer( ) )->GetAsyncIOManager( );

	// Only do sync check.
	_dword result = 0;
	asynciomanager.ProcessCheck( this, checksum, withname, &result );

	if ( finish != _null )
		*finish = result;

	return result == IResourceManager::_ASYNC_DONE_TRUE;
}