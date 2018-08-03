//============================================================================
// MemArchive.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// MemArchive Implementation
//----------------------------------------------------------------------------

MemArchive::MemArchive( )
{
	mBaseBuffer	= _null;
	mBaseLength	= 0;
}

MemArchive::~MemArchive( )
{
	// Release buffer only when these are new created.
	if ( mBaseBuffer == _null )
	{
		for ( _dword i = 0; i < mResourceArray.Number( ); i ++ )
			Memory::ReleaseReferencedBuffer( mResourceArray[i].mBuffer );
	}
}

_void MemArchive::OnFoundMemResource( StringPtr resname, _dword chunkoffset, _dword chunksize, _dword resoffset, _dword ressize, const Oword& reschecksum, _dword userdata )
{
	MemArchive* memarchive = (MemArchive*) userdata;
	FL_ASSERT( memarchive != _null )

	memarchive->CreateResourceHelper( resname, memarchive->mBaseBuffer + resoffset, ressize, reschecksum );
}

_bool MemArchive::CreateResourceHelper( StringPtr resname, const _void* buffer, _dword size, const Oword& checksum )
{
	MemResource memresource;
	memresource.mName		= resname;
	memresource.mChecksum	= checksum;
	memresource.mSize		= size;
	memresource.mBuffer		= _null;

	if ( size > 0 )
	{
		// We create data buffer for this resource.
		if ( mBaseBuffer == _null )
		{
			memresource.mBuffer = (_byte*) Memory::CreateReferencedBuffer( size );

			if ( buffer != _null )
				Memory::MemCpy( memresource.mBuffer, buffer, size );
		}
		// We just use the buffer.
		else
		{
			memresource.mBuffer = (_byte*) buffer;
		}
	}

	memresource.mName.Lowercase( );

	// Remove path prefix for the resource.
	if ( memresource.mName[0] == mPath[0] )
	{
		_dword length = mPath.Length( );

		if ( memresource.mName.SearchL2R( mPath ) == 0 && memresource.mName[ length ] == '\\' )
			memresource.mName.Remove( 0, length + 1 );
	}

	mResourceArray.InsertAscending( memresource );

	return _true;
}

_bool MemArchive::DeleteResourceHelper( _dword index )
{
	if ( index >= mResourceArray.Number( ) )
		return _false;

	// Release buffer only when these are new created.
	if ( mBaseBuffer == _null )
		Memory::ReleaseReferencedBuffer( mResourceArray[ index ].mBuffer );

	mResourceArray.Remove( index );

	return _true;
}

_dword MemArchive::SearchResourceHelper( StringPtr resname )
{
	if ( mResourceArray.Number( ) == 0 )
		return -1;

	String lowercasedname( resname );
	lowercasedname.Lowercase( );

	return mResourceArray.SearchAscending( lowercasedname ).mIndex;
}

_bool MemArchive::CreateArchive( StringPtr archivename, StringPtr mountpath )
{
	mName = archivename;
	mName.Lowercase( );

	mPath = mountpath;
	mPath.Lowercase( );
	mPath.TrimBoth( L"\\/ " );
	mPath.ReplaceAll( '/', '\\' );

	if ( mPath == L"." )
		mPath = L"";

	return _true;
}

_bool MemArchive::OpenArchive( StringPtr archivename, StringPtr mountpath, _byte* buffer, _dword length )
{
	mName = archivename;
	mName.Lowercase( );

	mPath = mountpath;
	mPath.Lowercase( );
	mPath.TrimBoth( L"\\/ " );
	mPath.ReplaceAll( '/', '\\' );

	if ( mPath == L"." )
		mPath = L"";

	mBaseBuffer	= buffer;
	mBaseLength	= length;

	if ( length < sizeof( _dword ) * 8 )
		return _false;

	// Try to load file dictionary.
	_dword dictionaryoffset = ResourceManager::CheckResourceDictionary( buffer + length - sizeof( _dword ) * 4, length );

	if ( dictionaryoffset == 0 )
		return _false;

	return ResourceManager::LoadResourceDictionary( buffer + dictionaryoffset, length - dictionaryoffset, OnFoundMemResource, _null, (_dword) this );
}

_void MemArchive::Release( )
{
	delete this;
}

StringPtr MemArchive::GetName( ) const
{
	return mName;
}

StringPtr MemArchive::GetPath( ) const
{
	return mPath;
}

_dword MemArchive::GetArchiveType( ) const
{
	return IResourceManager::_ARCHIVE_MEM;
}

_dword MemArchive::GetResourceNumber( ) const
{
	return mResourceArray.Number( );
}

IDataStream* MemArchive::OpenResource( StringPtr resname )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Search the resource with its name.
	_dword index = SearchResourceHelper( resname );
	if ( index == -1 )
		return _false;

	String fullname;

	if ( mPath == L"." || mPath == L"" )
		fullname = resname;
	else
		fullname.Format( L"%s\\%s", (const _char*) mPath, (const _char*) resname );

	return new MemStream( fullname, mResourceArray[ index ].mChecksum, mResourceArray[ index ].mBuffer, mResourceArray[ index ].mSize, mBaseBuffer == _null );
}

_bool MemArchive::CreateResource( StringPtr resname, const _void* buffer, _dword size, const Oword* checksum, _dword* finish )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Delete the old resource file with same name.
	DeleteResource( resname );

	if ( checksum == _null )
	{
		if ( CreateResourceHelper( resname, buffer, size, Oword( 0, 0, 0, 0 ) ) == _false )
			return _false;
	}
	else
	{
		if ( CreateResourceHelper( resname, buffer, size, *checksum ) == _false )
			return _false;
	}

	if ( finish != _null )
		*finish = IResourceManager::_ASYNC_DONE_TRUE;

	return _true;
}

_bool MemArchive::DeleteResource( StringPtr resname )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Search the resource with its name.
	_dword index = SearchResourceHelper( resname );
	if ( index == -1 )
		return _false;

	if ( DeleteResourceHelper( index ) == _false )
		return _false;

	return _true;
}

_bool MemArchive::SearchResource( StringPtr resname, _dword* size, Oword* checksum )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Search the resource with its name.
	_dword index = SearchResourceHelper( resname );
	if ( index == -1 )
		return _false;

	if ( size != _null )
		*size = mResourceArray[ index ].mSize;

	if ( checksum != _null )
		*checksum = mResourceArray[ index ].mChecksum;

	return _true;
}

_dword MemArchive::EnumArchive( FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2, _float* percentage )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	_dword resourcenumber = GetResourceNumber( );

	// Set percentage to 0.0 at begin.
	if ( percentage != _null )
		*percentage = 0.0f;

	for ( _dword i = 0; i < resourcenumber; i ++ )
	{
		// Call found resource function.
		if ( funcpointer != _null )
			(*funcpointer)( mResourceArray[i].mName, i + 1, parameter1, parameter2 );

		// Update percentage.
		if ( percentage != _null )
			*percentage = (_float) i / (_float) resourcenumber;
	}

	// Set percentage to 1.0 at end.
	if ( percentage != _null )
		*percentage = 1.0f;

	return resourcenumber;
}