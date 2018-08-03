//============================================================================
// FileArchive.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// FileArchive Implementation
//----------------------------------------------------------------------------

FileArchive::FileArchive( )
{
}

FileArchive::~FileArchive( )
{
}

_bool FileArchive::CreateArchive( StringPtr archivename, StringPtr mountpath )
{
	if ( archivename == L"" )
		archivename = L".";

	// Create the archive directory.
	if ( FileSystem::CreateDir( archivename ) == _false )
		return _false;

	mName = archivename;
	mName.Lowercase( );

	mPath = mountpath;
	mPath.Lowercase( );
	mPath.TrimBoth( L"\\/ " );
	mPath.ReplaceAll( '/', '\\' );

	if ( mPath == L"." )
		mPath = L"";

	// Set current directory as archive.
	DirectoryRecorder dirrecorder( archivename );

	// Get absolute path from file system.
	mAbsolutePath = FileSystem::GetCurrentDir( );

	mAbsolutePath.Lowercase( );
	mAbsolutePath.TrimRight( '\\' );

	return _true;
}

_bool FileArchive::OpenArchive( StringPtr archivename, StringPtr mountpath )
{
	if ( archivename == L"" )
		archivename = L".";

	// Check if directory exists.
	if ( FileSystem::DoesDirExist( archivename ) == _false )
		return _false;

	mName = archivename;
	mName.Lowercase( );

	mPath = mountpath;
	mPath.Lowercase( );
	mPath.TrimBoth( L"\\/ " );
	mPath.ReplaceAll( '/', '\\' );

	if ( mPath == L"." )
		mPath = L"";

	// Set current directory as archive.
	DirectoryRecorder dirrecorder( archivename );

	// Get absolute path from file system.
	mAbsolutePath = FileSystem::GetCurrentDir( );

	mAbsolutePath.Lowercase( );
	mAbsolutePath.TrimRight( '\\' );

	return _true;
}

_void FileArchive::BuildFileName( _char* buffer, _dword length, StringPtr resname )
{
	// If the resource name is full path, use it as file name.
	if ( StringFormatter::IsFullpath( resname ) == _true )
	{
		StringFormatter::FormatBuffer( buffer, length, L"%s", (const _char*) resname );
	}
	// If the resource name is not full path, build file name by add absolute path before resource name.
	else
	{
		StringFormatter::FormatBuffer( buffer, length, L"%s\\%s", (const _char*) mAbsolutePath, (const _char*) resname );
	}

	for ( _dword i = 0; buffer[i] != 0; i ++ )
	{
		if ( buffer[i] == '/' )
			buffer[i] = '\\';
	}
}

_void FileArchive::Release( )
{
	delete this;
}

StringPtr FileArchive::GetName( ) const
{
	return mName;
}

StringPtr FileArchive::GetPath( ) const
{
	return mPath;
}

_dword FileArchive::GetArchiveType( ) const
{
	return IResourceManager::_ARCHIVE_FILE;
}

_dword FileArchive::GetResourceNumber( ) const
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Enum files in directory from file system.
	return FileSystem::EnumFilesInDirectory( mAbsolutePath, _true, _null, 0, 0 );
}

IDataStream* FileArchive::OpenResource( StringPtr resname )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	File file; _char filename[1024];

	// Build file name from resource name.
	BuildFileName( filename, 1024, resname );

	// Set file attribute to normal.
	FileSystem::SetAttributes( filename, FileSystem::_ATTRIBUTE_NORMAL );

	// Open a file for both read and write.
	if ( file.Open( filename, File::_OPEN_EXISTING, File::_OPERATION_READ | File::_OPERATION_WRITE, File::_SHARE_READ ) == _false )
	{
		// If failed, open only for read.
		if ( file.Open( filename, File::_OPEN_EXISTING, File::_OPERATION_READ, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
			return _null;
	}

	// Create a file data stream from this file, return it.
	return new FileStream( resname, file );
}

_bool FileArchive::CreateResource( StringPtr resname, const _void* buffer, _dword size, const Oword* checksum, _dword* finish )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	_char filename[1024];

	// Build file name from resource name.
	BuildFileName( filename, 1024, resname );

	// Create a file in file system, and also fill file data with buffer.
	if ( FileSystem::Create( filename, buffer, size ) == _false )
		return _false;

	if ( finish != _null )
		*finish = IResourceManager::_ASYNC_DONE_TRUE;

	return _true;
}

_bool FileArchive::DeleteResource( StringPtr resname )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	_char filename[1024];

	// Build file name from resource name.
	BuildFileName( filename, 1024, resname );

	// Delete the file in file system.
	return FileSystem::Delete( filename );
}

_bool FileArchive::SearchResource( StringPtr resname, _dword* size, Oword* checksum )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	_char filename[1024];

	// Build file name from resource name.
	BuildFileName( filename, 1024, resname );

	if ( FileSystem::DoesFileExist( filename ) == _false )
		return _false;

	// Get file size.
	if ( size != _null )
	{
		// Get file size from file system.
		if ( FileSystem::GetFileSize( filename, *size ) == _false )
			return _false;
	}

	// Get file checksum.
	if ( checksum != _null )
	{
		String fullpath = filename;

		// File checksum is calculate with file name ( includes path ), so we must remove file path away.
		DirectoryRecorder dirrecorder( fullpath.SubString( 0, fullpath.Length( ) - resname.Length( ) ) );

		// Get file checksum from file system.
		if ( FileSystem::GetFileChecksum( resname, *checksum ) == _false )
			return _false;
	}

	return _true;
}

_dword FileArchive::EnumArchive( FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2, _float* percentage )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Set percentage to 0.0 at begin.
	if ( percentage != _null )
		*percentage = 0.0f;

	// Enum files in directory from file system.
	_dword resourcenumber = FileSystem::EnumFilesInDirectory( mAbsolutePath, _true, funcpointer, parameter1, parameter2 );

	// Set percentage to 1.0 at end.
	if ( percentage != _null )
		*percentage = 1.0f;

	return resourcenumber;
}