//============================================================================
// ResourceManager.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// ResourceManager Implementation
//----------------------------------------------------------------------------

ResourceManager::ResourceManager( ) : mEnumIterator( mFileDictionary.GetHeadIterator( ) )
{
	mDefaultArchive			= _null;

	mTotalIOControlTime		= 0;
	mTotalIOReadTime		= 0;
	mTotalIOReadBytes		= 0;
	mTotalIOWriteTime		= 0;
	mTotalIOWriteBytes		= 0;

	mHappenSearchResource	= 0;
	mHappenIOControlTime	= 0;
	mHappenIOReadTime		= 0;
	mHappenIOWriteTime		= 0;

	mAsyncIOManager			= new AsyncIOManager;

	mUseRootArchive			= _true;
	mDebugResource			= _false;
	mDebugLibrary			= _true;
	mDebugPDB				= _true;
	mMultiDebug				= _false;
}

ResourceManager::~ResourceManager( )
{
	delete mAsyncIOManager;
	delete mDefaultArchive;

	for ( _dword i = 0; i < mArchiveArray.Number( ); i ++ )
		mArchiveArray[i]->Release( );

	for ( _dword i = 0; i < mLibraryArray.Number( ); i ++ )
	{
		LibraryDesc& libdesc = mLibraryArray[i];

		delete libdesc.mLibrary;

		// Delete dll file.
		if ( libdesc.mDllFileName != L"" )
			FileSystem::Delete( libdesc.mDllFileName );

		// Delete pdb file.
		if ( libdesc.mPdbFileName != L"" )
			FileSystem::Delete( libdesc.mPdbFileName );
	}

	GetFairyLandGlobal( ).mResourceManager = _null;
}

_bool ResourceManager::OnCheckResource( StringPtr resname, _dword resnumber, _void* parameter1, _void* parameter2 )
{
	IResourceManager* resourcemanager = GetResourceManagerPointer( );
	FL_ASSERT( resourcemanager != _null )

	if ( resourcemanager->SearchResource( resname, _null, _null ) == _true )
	{
		_bool* foundsamefile = (_bool*) parameter1;
		FL_ASSERT( foundsamefile != _null )

		*foundsamefile = _true;

		return _false;
	}

	return _true;
}

_bool ResourceManager::OnCopyResource( StringPtr resname, _dword resnumber, _void* parameter1, _void* parameter2 )
{
	IDataArchive* srcarchive = (IDataArchive*) parameter1;
	FL_ASSERT( srcarchive != _null )

	IDataArchive* desarchive = (IDataArchive*) parameter2;
	FL_ASSERT( desarchive != _null )

	desarchive->DeleteResource( resname );

	CopyResourceHelper( srcarchive, desarchive, resname, resname, _null, _null );

	return _true;
}

_bool ResourceManager::OnMoveResource( StringPtr resname, _dword resnumber, _void* parameter1, _void* parameter2 )
{
	IDataArchive* srcarchive = (IDataArchive*) parameter1;
	FL_ASSERT( srcarchive != _null )

	IDataArchive* desarchive = (IDataArchive*) parameter2;
	FL_ASSERT( desarchive != _null )

	MoveResourceHelper( srcarchive, desarchive, resname, resname, _null, _null );

	return _true;
}

_bool ResourceManager::OnCheckLocalResource( StringPtr realname, StringPtr logicname, _dword size, const Oword& checksum, const Oword& fastchecksum, _dword flag, _bool needed, _void* parameter1, _void* parameter2 )
{
	ResourceManager* resourcemanager = (ResourceManager*) parameter1;
	FL_ASSERT( resourcemanager != _null )

	CheckStruct* checkstruct = (CheckStruct*) parameter2;
	FL_ASSERT( checkstruct != _null )

	// Skip not needed file.
	if ( checkstruct->mOnlyNeededFile && needed == _false )
		return _true;

	// Skip file with same postfix.
	if ( checkstruct->mSkipFile != L"" && logicname.SearchL2RLowercase( checkstruct->mSkipFile ) != -1 )
		return _true;

	// This file is not existing, skip it.
	Oword oldchecksum;
	if ( resourcemanager->SearchResource( logicname, _null, &oldchecksum ) == _false )
	{
		checkstruct->mAllResourceReady = _false;

		return _true;
	}

	if ( checkstruct->mDeleteInvalidFile )
	{
		// Checksum is not same, delete the local file.
		if ( checksum != oldchecksum )
		{
			resourcemanager->DeleteResource( logicname );

			checkstruct->mAllResourceReady = _false;
		}
	}

	return _true;
}

_bool ResourceManager::CopyResourceHelper( IDataArchive* srcarchive, IDataArchive* desarchive, StringPtr srcresname, StringPtr desresname, _bool* filechanged, const Oword* checksum )
{
	_dword copymethod = 0; // 0 - Keep, 1 - Decompress, 2 - Compress.

	// Package/PERes/Mem to file, decompress it.
	if ( desarchive->GetArchiveType( ) == _ARCHIVE_FILE )
	{
		copymethod = 1;
	}
	// File to package, compress it.
	else if ( srcarchive->GetArchiveType( ) == _ARCHIVE_FILE && desarchive->GetArchiveType( ) == _ARCHIVE_PACK )
	{
		copymethod = 2;
	}

	Oword reschecksum;

	if ( checksum != _null )
		reschecksum = *checksum;

	if ( reschecksum == Oword( ) )
	{
		// Get checksum if target archive is package.
		if ( desarchive->GetArchiveType( ) == _ARCHIVE_PACK || desarchive->GetArchiveType( ) == _ARCHIVE_MEM )
		{
			if ( srcarchive->SearchResource( srcresname, _null, &reschecksum ) == _false )
				return _false;
		}
	}

	// Open resource to copy.
	IDataStream* srcstream = srcarchive->OpenResource( srcresname );
	if ( srcstream == _null )
		return _false;

	MemFile memfile;

	// Directly use data stream buffer.
	if ( copymethod == 0 && srcstream->GetBuffer( ) != _null )
	{
		memfile.Open( srcstream->GetBuffer( ), srcstream->GetSize( ), _false );
		memfile.EnableAutoDeleteBuffer( _false );
	}
	// Map into memory file.
	else
	{
		if ( srcstream->Mapping( memfile, copymethod == 1 ) == _false )
		{
			srcstream->Release( );

			return _false;
		}
	}

	// File is decompressed, so means it changed.
	if ( filechanged != _null && copymethod == 1 && srcstream->IsCompressed( ) == _true )
		*filechanged = _true;

	srcstream->Release( );

	// Compress memory file.
	if ( copymethod == 2 )
	{
		String ext = StringFormatter::GetExtention( srcresname ).Lowercase( );
		
		if ( ext != L"ogg" && ext != L"wav" && ext != L"mp3" )
		{
			_bool compressresult = Compression::Compress( memfile, 0.8f );

			if ( filechanged != _null )
				*filechanged = compressresult;
		}
	}

	// Copy resource to target archive.
	if ( desarchive->CreateResource( desresname, memfile.GetBuffer( ), memfile.GetLength( ), &reschecksum ) == _false )
		return _false;

	return _true;
}

_bool ResourceManager::MoveResourceHelper( IDataArchive* srcarchive, IDataArchive* desarchive, StringPtr srcresname, StringPtr desresname, _bool* filechanged, const Oword* checksum )
{
	_dword movemethod = 0; // 0 - copy/delete, 1 - filemove.

	// File system to file system, just file move.
	if ( srcarchive->GetArchiveType( ) == _ARCHIVE_FILE && desarchive->GetArchiveType( ) == _ARCHIVE_FILE )
	{
		movemethod = 1;
	}

	// Copy and delete old resource.
	if ( movemethod == 0 )
	{
		if ( CopyResourceHelper( srcarchive, desarchive, srcresname, desresname, filechanged, checksum ) == _false )
			return _false;

		if ( srcarchive->DeleteResource( srcresname ) == _false )
			return _false;
	}
	// File move.
	else
	{
		_char srcfilename[1024], desfilename[1024];

		( (FileArchive*) srcarchive )->BuildFileName( srcfilename, 1024, srcresname );
		( (FileArchive*) desarchive )->BuildFileName( desfilename, 1024, desresname );

		if ( FileSystem::Move( srcfilename, desfilename ) == _false )
			return _false;

		if ( FileSystem::IsFileCompressed( desfilename ) )
		{
			if ( FileSystem::DecompressFile( desfilename ) == _false )
				return _false;

			// File is decompressed, so means it changed.
			if ( filechanged != _null )
				*filechanged = _true;
		}
	}

	return _true;
}

_bool ResourceManager::InThePath( StringPtr fullpath, StringPtr path )
{
	_dword pathlen = path.Length( ), fulllength = fullpath.Length( );

	if ( pathlen >= fulllength ) 
		return _false;

	if ( fullpath[ pathlen ] != L'\\' )
		return _false;

	FL_ASSERT( fulllength < 1024 );

	_char buffer[ 1024 ];
	Memory::MemCpy( buffer, fullpath, pathlen * sizeof( _char ) );
	buffer[ pathlen ] = 0;

	return path.CompareLowercase( buffer );
}

_void ResourceManager::PrepareArchive( )
{
	if ( mArchiveArray.Number( ) != 0 )
		return;

	if ( mDefaultArchive != _null )
		return;

	mDefaultArchive = new FileArchive( );

	PS_VERIFY( mDefaultArchive->OpenArchive( FileSystem::GetCurrentDir( ), L"." ) )
}

_void ResourceManager::SearchResourceHelper( StringPtr resname, _dword process, _void* parameter, _dword* retval )
{
	PrepareArchive( );

	mHappenSearchResource ++;

	if ( mArchiveArray.Number( ) == 0 )
	{
		ProcessResourceHelper( mDefaultArchive, resname, process, parameter, retval );
		return;
	}

	// Get the archive path of the resource.
	String archivepath = StringFormatter::GetPathName( resname, _false );

	// First try to open resource in archives with same path name, from back to front.
	for ( _dword i = 0; i < mArchiveArray.Number( ); i ++ )
	{
		IDataArchive* dataarchive = mArchiveArray[ mArchiveArray.Number( ) - i - 1 ];

		_bool found = _false;

        if ( InThePath( resname, dataarchive->GetPath( ) ) )
			found = _true;

		if ( found == _false && dataarchive->GetPath( ) == L"." && archivepath == L"" )
			found = _true;

		if ( found == _false && dataarchive->GetPath( ) == L"" && archivepath == L"." )
			found = _true;

		// Skip this archive.
		if ( found == _false )
			continue;

		_dword index = 0;

		if ( archivepath != L"" && archivepath != L"." )
            index = dataarchive->GetPath().Length( ) + 1;

		// Build the file name inside the archive.
		StringPtr filename( (const _char*) resname + index );

		// Call the process function, if it return true, stop search.
		if ( ProcessResourceHelper( dataarchive, filename, process, parameter, retval ) == _true )
			return;
	}

	// Dont use root archive.
	if ( mUseRootArchive == _false )
		return;

	// Then try to open resource in root archives, from back to front.
	for ( _dword i = 0; i < mArchiveArray.Number( ); i ++ )
	{
		IDataArchive* dataarchive = mArchiveArray[ mArchiveArray.Number( ) - i - 1 ];

		// Skip none root archive.
		if ( dataarchive->GetPath( ) != L"." && dataarchive->GetPath( ) != L"" )
			continue;

		// Call the process function, if it return true, stop search.
		if ( ProcessResourceHelper( dataarchive, resname, process, parameter, retval ) == _true )
			return;
	}
}

_bool ResourceManager::ProcessResourceHelper( IDataArchive* dataarchive, StringPtr filename, _dword process, _void* parameter, _dword* retval )
{
	switch ( process )
	{
		case _PROCESS_OPEN:
		{
			// Try to open data stream in archive.
			IDataStream* datastream = dataarchive->OpenResource( filename );

			// Set process return value.
			*retval = (_dword) datastream;

			return datastream != _null;
		}

		case _PROCESS_CREATE:
		{
			CreateStruct* createstruct = (CreateStruct*) parameter;

			// Try to create resource in archive.
			*retval = dataarchive->CreateResource( filename, createstruct->mBuffer, createstruct->mSize, createstruct->mChecksum );

			return *retval == _true;
		}

		case _PROCESS_DELETE:
		{
			// Try to delete resource in archive.
			*retval = dataarchive->DeleteResource( filename );

			return *retval == _true;
		}

		case _PROCESS_SEARCH:
		{
			SearchStruct* searchstruct = (SearchStruct*) parameter;

			// Try to search resource in archive.
			*retval = dataarchive->SearchResource( filename, searchstruct->mSize, searchstruct->mChecksum );

			return *retval == _true;
		}
	}

	return _true;
}

IDataArchive* ResourceManager::SearchArchiveByName( StringPtr archivename )
{
	for ( _dword i = 0; i < mArchiveArray.Number( ); i ++ )
	{
		IDataArchive* dataarchive = mArchiveArray[i];

		if ( dataarchive->GetName( ).CompareLowercase( archivename ) == _true )
			return dataarchive;
	}

	return _null;
}

IDataArchive* ResourceManager::SearchArchiveByResource( String& resname )
{
	PrepareArchive( );

	if ( mArchiveArray.Number( ) == 0 )
		return mDefaultArchive->SearchResource( resname, _null, _null ) ? mDefaultArchive : _null;

	// Get the archive path of the resource.
	String archivepath = StringFormatter::GetPathName( resname, _false );

	// First try to open resource in archives with same path name, from back to front.
	for ( _dword i = 0; i < mArchiveArray.Number( ); i ++ )
	{
		IDataArchive* dataarchive = mArchiveArray[ mArchiveArray.Number( ) - i - 1 ];

		_bool found = _false;

        if( InThePath(resname, dataarchive->GetPath()) == _true )
			found = _true;

		if ( found == _false && dataarchive->GetPath( ) == L"." && archivepath == L"" )
			found = _true;

		if ( found == _false && dataarchive->GetPath( ) == L"" && archivepath == L"." )
			found = _true;

		// Skip this archive.
		if ( found == _false )
			continue;

		_dword index = 0;

		if ( archivepath != L"" && archivepath != L"." )
            index = dataarchive->GetPath().Length() + 1;

		// Build the file name inside the archive.
		StringPtr filename( (const _char*) resname + index );

		if ( dataarchive->SearchResource( filename, _null, _null ) == _true )
		{
			resname = filename;

			return dataarchive;
		}
	}

	// Then try to open resource in root archives, from back to front.
	for ( _dword j = 0; j < mArchiveArray.Number( ); j ++ )
	{
		IDataArchive* dataarchive = mArchiveArray[ mArchiveArray.Number( ) - j - 1 ];

		// Skip none root archive.
		if ( dataarchive->GetPath( ) != L"." && dataarchive->GetPath( ) != L"" )
			continue;

		if ( dataarchive->SearchResource( resname, _null, _null ) == _true )
			return dataarchive;
	}

	return _null;
}

IDataArchive* ResourceManager::SearchArchiveToCreateResource( String& resname )
{
	PrepareArchive( );

	if ( mArchiveArray.Number( ) == 0 )
		return mDefaultArchive;

	// Get the archive path of the resource.
	String archivepath = StringFormatter::GetPathName( resname, _false );

	// First try to open resource in archives with same path name, from back to front.
	for ( _dword i = 0; i < mArchiveArray.Number( ); i ++ )
	{
		IDataArchive* dataarchive = mArchiveArray[ mArchiveArray.Number( ) - i - 1 ];

		_bool found = _false;

        if( InThePath(resname, dataarchive->GetPath()) == _true)
			found = _true;

		if ( found == _false && dataarchive->GetPath( ) == L"." && archivepath == L"" )
			found = _true;

		if ( found == _false && dataarchive->GetPath( ) == L"" && archivepath == L"." )
			found = _true;

		// Skip this archive.
		if ( found == _false )
			continue;

		_dword index = 0;

		if ( archivepath != L"" && archivepath != L"." )
            index = dataarchive->GetPath().Length( ) + 1;

		// Build the file name inside the archive.
		StringPtr filename( (const _char*) resname + index );

		// Found an archive can be used to create resrouce.
		if ( dataarchive->GetArchiveType( ) != _ARCHIVE_PERES )
		{
			resname = filename;

			return dataarchive;
		}
	}

	// Then try to open resource in root archives, from back to front.
	for ( _dword j = 0; j < mArchiveArray.Number( ); j ++ )
	{
		IDataArchive* dataarchive = mArchiveArray[ mArchiveArray.Number( ) - j - 1 ];

		// Skip none root archive.
		if ( dataarchive->GetPath( ) != L"." && dataarchive->GetPath( ) != L"" )
			continue;

		// Found an archive can be used to create resrouce.
		if ( dataarchive->GetArchiveType( ) != _ARCHIVE_PERES )
			return dataarchive;
	}

	return _null;
}

_dword ResourceManager::CheckResourceDictionary( const _byte* hintbuffer, _dword filesize )
{
	const _byte magiccode[] = { 'P', 'S', 'P', 'D', 'I', 'C', 'T', 0 };

	// Check file dictionary with magic code.
	for ( _dword i = 0; i < sizeof( magiccode ); i ++ )
	{
		if ( hintbuffer[i] != magiccode[i] )
			return 0;
	}

	_dword offset = * (_dword*) ( hintbuffer + sizeof( magiccode ) );

	if ( offset == 0 )
		return 0;

	if ( offset > filesize )
		return 0;

	return offset;
}

_bool ResourceManager::LoadResourceDictionary( const _byte* chunkbuffer, _dword length, OnFoundResource foundresourcefunc, OnFoundChunk foundchunkfunc, _dword userdata )
{
	if ( length <= sizeof( _dword ) * 3 )
		return _false;

	_dword* pointer = (_dword*) chunkbuffer;

	if ( pointer[2] >= 1024 )
		return _false;

	_dword chunkflag	= pointer[0];
	_dword chunksize	= pointer[1];
	_dword namelength	= ( pointer[2] + 1 ) * sizeof( _char );

	pointer += 3;

	_char namebuffer[1024];
	Memory::MemCpy( namebuffer, pointer, namelength );

	if ( StringPtr( namebuffer ) != L"<<PSP_DICTIONARY>>" )
		return _false;

	// Get file size/checksum.
	_dword* sizechecksum = (_dword*)( (_byte*) pointer + namelength );

	MemFile memfile;
	memfile.Open( (const _byte*) ( sizechecksum + 5 ), sizechecksum[0], _false );
	memfile.EnableAutoDeleteBuffer( _false );

	// Try to decompress file dictionary.
	if ( Compression::Decompress( memfile ) == _false )
		return _false;

	// Try to decompress file dictionary.
	if ( ParseResourceDictionary( memfile, foundresourcefunc, foundchunkfunc, userdata ) == _false )
		return _false;

	return _true;
}

_bool ResourceManager::ParseResourceDictionary( MemFile& memfile, OnFoundResource foundresourcefunc, OnFoundChunk foundchunkfunc, _dword userdata )
{
	_dword offset = 0;

	while ( offset < memfile.GetLength( ) )
	{
		_dword* pointer = (_dword*) ( memfile.GetBuffer( ) + offset );

		// Empty chunk.
		if ( pointer[0] == -1 )
		{
			if ( memfile.GetLength( ) - offset < sizeof( _dword ) * 3 )
				return _false;

			// Create empty chunk according to file dictionary.
			if ( foundchunkfunc != _null )
			{
				(*foundchunkfunc)( pointer[1], pointer[2], userdata );
			}

			// Increase offset.
			offset += sizeof( _dword ) * 3;
		}
		// Option chunk.
		else if ( pointer[0] == -2 )
		{
			// Reserved for now.
		}
		// File chunk.
		else
		{
			if ( memfile.GetLength( ) - offset < sizeof( _dword ) * 9 )
				return _false;

			// Create resource according to file dictionary.
			if ( foundresourcefunc != _null )
			{
				(*foundresourcefunc)( (const _char*) ( pointer + 9 ), pointer[1], pointer[2],
					pointer[3], pointer[4], Oword( pointer[5], pointer[6], pointer[7], pointer[8] ), userdata );
			}

			// Increase offset.
			offset += pointer[0] + sizeof( _dword ) * 9;
		}
	}

	return _true;
}

_void ResourceManager::Release( )
{
	delete this;
}

_void ResourceManager::Update( _dword elapse )
{
}

_bool ResourceManager::CreateFileArchive( StringPtr archivename, StringPtr mountpath )
{
	LockOwner lockowner( mResourceLock );

	FileArchive* dataarchive = new FileArchive( );

	// Create file archive and mount it.
	if ( dataarchive->CreateArchive( archivename, mountpath ) == _false )
	{
		dataarchive->Release( );
		dataarchive = _null;
	}
	else
	{
		mArchiveArray.Append( dataarchive );
	}

	if ( dataarchive == _null )
	{
		PS_ERROR2( L"[RM] Create file archive failed : %s => %s\r\n", archivename, mountpath )
	}
	else
	{
		PS_TRACE2( L"[RM] Create file archive successful : %s => %s\r\n", archivename, mountpath )
	}

	return dataarchive != _null;
}

_bool ResourceManager::CreatePackArchive( StringPtr archivename, StringPtr mountpath )
{
	LockOwner lockowner( mResourceLock );

	PackArchive* dataarchive = new PackArchive( );

	// Create package archive and mount it.
	if ( dataarchive->CreateArchive( archivename, mountpath ) == _false )
	{
		dataarchive->Release( );
		dataarchive = _null;
	}
	else
	{
		mArchiveArray.Append( dataarchive );
	}

	if ( dataarchive == _null )
	{
		PS_ERROR2( L"[RM] Create pack archive failed : %s => %s\r\n", archivename, mountpath )
	}
	else
	{
		PS_TRACE2( L"[RM] Create pack archive successful : %s => %s\r\n", archivename, mountpath )
	}

	return dataarchive != _null;
}

_bool ResourceManager::CreateHashArchive( StringPtr archivename, StringPtr mountpath, _dword maxitem )
{
	LockOwner lockowner( mResourceLock );

	HashArchive* dataarchive = new HashArchive( );

	// Create package archive and mount it.
	if ( dataarchive->CreateArchive( archivename, mountpath, maxitem ) == _false )
	{
		dataarchive->Release( );
		dataarchive = _null;
	}
	else
	{
		mArchiveArray.Append( dataarchive );
	}

	if ( dataarchive == _null )
	{
		PS_ERROR2( L"[RM] Create hash archive failed : %s => %s\r\n", archivename, mountpath )
	}
	else
	{
		PS_TRACE2( L"[RM] Create hash archive successful : %s => %s\r\n", archivename, mountpath )
	}

	return dataarchive != _null;
}

_bool ResourceManager::CreatePEResArchive( StringPtr archivename, StringPtr mountpath )
{
	PS_ERROR2( L"[RM] Create PERes archive failed : %s => %s\r\n", archivename, mountpath )

	return _false;
}

_bool ResourceManager::CreateMemArchive( StringPtr archivename, StringPtr mountpath )
{
	LockOwner lockowner( mResourceLock );

	MemArchive* dataarchive = new MemArchive( );

	if ( dataarchive->CreateArchive( archivename, mountpath ) == _false )
	{
		dataarchive->Release( );
		dataarchive = _null;
	}

	if ( dataarchive != _null )
		mArchiveArray.Append( dataarchive );

	if ( dataarchive == _null )
	{
		PS_ERROR2( L"[RM] Create memory archive failed : %s => %s\r\n", archivename, mountpath )
	}
	else
	{
		PS_TRACE2( L"[RM] Create memory archive successful : %s => %s\r\n", archivename, mountpath )
	}

	return dataarchive != _null;
}

_bool ResourceManager::OpenFileArchive( StringPtr archivename, StringPtr mountpath )
{
	LockOwner lockowner( mResourceLock );

	FileArchive* dataarchive = new FileArchive( );

	// Open file archive and mount it.
	if ( dataarchive->OpenArchive( archivename, mountpath ) == _false )
	{
		dataarchive->Release( );
		dataarchive = _null;
	}

	if ( dataarchive != _null )
		mArchiveArray.Append( dataarchive );

	if ( dataarchive == _null )
	{
		PS_ERROR2( L"[RM] Open file archive failed : %s => %s\r\n", archivename, mountpath )
	}
	else
	{
		PS_TRACE2( L"[RM] Open file archive successful : %s => %s\r\n", archivename, mountpath )
	}

	return dataarchive != _null;
}

_bool ResourceManager::OpenPackArchive( StringPtr archivename, StringPtr mountpath, _dword baseposition, _dword rangeposition )
{
	LockOwner lockowner( mResourceLock );

	PackArchive* dataarchive = new PackArchive( );

	// Open package archive and mount it.
	if ( dataarchive->OpenArchive( archivename, mountpath, baseposition, rangeposition ) == _false )
	{
		dataarchive->Release( );
		dataarchive = _null;
	}

	if ( dataarchive != _null )
		mArchiveArray.Append( dataarchive );

	if ( dataarchive == _null )
	{
		PS_ERROR2( L"[RM] Open pack archive failed : %s => %s\r\n", archivename, mountpath )
	}
	else
	{
		PS_TRACE2( L"[RM] Open pack archive successful : %s => %s\r\n", archivename, mountpath )
	}

	return dataarchive != _null;
}

_bool ResourceManager::OpenHashArchive( StringPtr archivename, StringPtr mountpath, _dword baseposition, _dword rangeposition )
{
	LockOwner lockowner( mResourceLock );

	HashArchive* dataarchive = new HashArchive( );

	// Open package archive and mount it.
	if ( dataarchive->OpenArchive( archivename, mountpath, baseposition, rangeposition ) == _false )
	{
		dataarchive->Release( );
		dataarchive = _null;
	}

	if ( dataarchive != _null )
		mArchiveArray.Append( dataarchive );

	if ( dataarchive == _null )
	{
		PS_ERROR2( L"[RM] Open hash archive failed : %s => %s\r\n", archivename, mountpath )
	}
	else
	{
		PS_TRACE2( L"[RM] Open hash archive successful : %s => %s\r\n", archivename, mountpath )
	}

	return dataarchive != _null;
}

_bool ResourceManager::OpenPEResArchive( StringPtr archivename, StringPtr mountpath, _handle module )
{
	LockOwner lockowner( mResourceLock );

	PEResArchive* dataarchive = new PEResArchive( );

	// Open pe resouce archive and mount it.
	if ( dataarchive->OpenArchive( archivename, mountpath, module ) == _false )
	{
		dataarchive->Release( );
		dataarchive = _null;
	}
	else
	{
		mArchiveArray.Append( dataarchive );
	}

	if ( dataarchive == _null )
	{
		PS_ERROR2( L"[RM] Open PERes archive failed : %s => %s\r\n", archivename, mountpath )
	}
	else
	{
		PS_TRACE2( L"[RM] Open PERes archive successful : %s => %s\r\n", archivename, mountpath )
	}

	return dataarchive != _null;
}

_bool ResourceManager::OpenMemArchive( StringPtr archivename, StringPtr mountpath, _byte* buffer, _dword length )
{
	LockOwner lockowner( mResourceLock );

	MemArchive* dataarchive = new MemArchive( );

	// Open pe resouce archive and mount it.
	if ( dataarchive->OpenArchive( archivename, mountpath, buffer, length ) == _false )
	{
		dataarchive->Release( );
		dataarchive = _null;
	}
	else
	{
		mArchiveArray.Append( dataarchive );
	}

	if ( dataarchive == _null )
	{
		PS_ERROR2( L"[RM] Open Mem archive failed : %s => %s\r\n", archivename, mountpath )
	}
	else
	{
		PS_TRACE2( L"[RM] Open Mem archive successful : %s => %s\r\n", archivename, mountpath )
	}

	return dataarchive != _null;
}

_bool ResourceManager::CheckFileArchive( StringPtr archivename, StringPtr mountpath )
{
	LockOwner lockowner( mResourceLock );

	FileArchive dataarchive;

	if ( dataarchive.OpenArchive( archivename, mountpath ) == _false )
		return _false;

	_bool foundsamefile = _false;

	dataarchive.EnumArchive( OnCheckResource, &foundsamefile, _null, _null );

	return foundsamefile == _false;
}

_bool ResourceManager::CheckPackArchive( StringPtr archivename, StringPtr mountpath )
{
	LockOwner lockowner( mResourceLock );

	PackArchive dataarchive;

	if ( dataarchive.OpenArchive( archivename, mountpath, 0, 0 ) == _false )
		return _false;

	_bool foundsamefile = _false;

	dataarchive.EnumArchive( OnCheckResource, &foundsamefile, _null, _null );

	return foundsamefile == _false;
}

_bool ResourceManager::CheckHashArchive( StringPtr archivename, StringPtr mountpath )
{
	LockOwner lockowner( mResourceLock );

	HashArchive dataarchive;

	if ( dataarchive.OpenArchive( archivename, mountpath, 0, 0 ) == _false )
		return _false;

	_bool foundsamefile = _false;

	dataarchive.EnumArchive( OnCheckResource, &foundsamefile, _null, _null );

	return foundsamefile == _false;
}

_bool ResourceManager::CheckPEResArchive( StringPtr archivename, StringPtr mountpath )
{
	LockOwner lockowner( mResourceLock );

	PEResArchive dataarchive;

	if ( dataarchive.OpenArchive( archivename, mountpath, _null ) == _false )
		return _false;

	_bool foundsamefile = _false;

	dataarchive.EnumArchive( OnCheckResource, &foundsamefile, _null, _null );

	return foundsamefile == _false;
}

_bool ResourceManager::CheckMemArchive( StringPtr archivename, StringPtr mountpath )
{
	return _true;
}

_dword ResourceManager::GetResourceNumber( ) const
{
	_dword resourcenumber = 0;

	for ( _dword i = 0; i < mArchiveArray.Number( ); i ++ )
	{
		// Skip file archive.
		if ( mArchiveArray[i]->GetArchiveType( ) == _ARCHIVE_FILE )
			continue;

		resourcenumber += mArchiveArray[i]->GetResourceNumber( );
	}

	return resourcenumber;
}

_dword ResourceManager::GetArchiveNumber( ) const
{
	LockOwner lockowner( mResourceLock );

	return mArchiveArray.Number( );
}

IDataArchive* ResourceManager::GetArchive( _dword archiveindex )
{
	LockOwner lockowner( mResourceLock );

	if ( archiveindex >= mArchiveArray.Number( ) )
		return _null;

	return mArchiveArray[ archiveindex ];
}

IDataArchive* ResourceManager::SearchArchive( StringPtr archivepath )
{
	// Search archive by name.
	for ( _dword i = 0; i < mArchiveArray.Number( ); i ++ )
	{
		IDataArchive* dataarchive = mArchiveArray[i];

		if ( dataarchive->GetPath( ).CompareLowercase( archivepath ) == _true )
			return dataarchive;
	}

	return _null;
}

_bool ResourceManager::CloseArchive( _dword archiveindex )
{
	LockOwner lockowner( mResourceLock );

	if ( archiveindex >= mArchiveArray.Number( ) )
		return _false;

	IDataArchive* dataarchive = mArchiveArray[ archiveindex ];
	dataarchive->Release( );

	mArchiveArray.Remove( archiveindex );

	return _true;
}

_bool ResourceManager::CloseArchive( StringPtr archivename )
{
	LockOwner lockowner( mResourceLock );

	// Search archive by name.
	for ( _dword i = 0; i < mArchiveArray.Number( ); i ++ )
	{
		IDataArchive* dataarchive = mArchiveArray[i];

		// Found the archive, release it, and remove it from archive array.
		if ( dataarchive->GetName( ).CompareLowercase( archivename ) )
		{
			dataarchive->Release( );
			mArchiveArray.Remove( i );

			return _true;
		}
	}

	return _false;
}

_bool ResourceManager::CloseAllArchive( )
{
	LockOwner lockowner( mResourceLock );

	for ( _dword i = 0; i < mArchiveArray.Number( ); i ++ )
	{
		IDataArchive* dataarchive = mArchiveArray[i];

		dataarchive->Release( );
	}

	mArchiveArray.Clear( );

	return _true;
}

IDataStream* ResourceManager::CreateMemStream( StringPtr resname, _dword size, const Oword& checksum )
{
	_byte* buffer = _null;

	if ( size != 0 )
		buffer = (_byte*) Memory::CreateReferencedBuffer( size );

	IDataStream* datastream = new MemStream( resname, checksum, buffer, size, _true );

	if ( buffer != _null )
		Memory::ReleaseReferencedBuffer( buffer );

	return datastream;
}

IDataStream* ResourceManager::OpenResource( StringPtr resname )
{
	if ( Trace::IsAssertErrorEnabled( ) )
		PS_TRACE1( L"[RM] OpenResource %s\r\n", resname )

	LockOwner lockowner( mResourceLock );

	IDataStream* datastream = _null;

	// Try to open resource.
	SearchResourceHelper( GetValidResourceName( resname ), _PROCESS_OPEN, _null, (_dword*) &datastream );

	// Try to open resource with deep search.
	if ( datastream == _null )
	{
		// Only deep search when resource has no path.
		if ( resname.SearchL2R( '\\' ) == -1 && resname.SearchL2R( '/' ) == -1 )
		{
			// Try to open in each search path, from back to front.
			for ( _dword i = 0; i < mSearchPathArray.Number( ); i ++ )
			{
				StringPtr searchpath = mSearchPathArray[ mSearchPathArray.Number( ) - i - 1 ].mObject1;

				// Search resource by filename from manifest.
				SearchResourceHelper( GetValidResourceName( searchpath, resname ), _PROCESS_OPEN, _null, (_dword*) &datastream );

				// Found it.
				if ( datastream != _null )
					break;
			}
		}
	}

	if ( datastream == _null )
		PS_ERROR1( L"[RM] Open resource failed : %s\r\n", resname )

	return datastream;
}

_void ResourceManager::CloseResource( IDataStream*& datastream )
{
	if ( datastream == _null )
		return;

	datastream->Release( );
	datastream = _null;
}

_void ResourceManager::EnableRootArchive( _bool enable )
{
	mUseRootArchive = enable;
}

_void ResourceManager::EnableDebugResource( _bool enable )
{
	mDebugResource = enable;
}

_void ResourceManager::EnableDebugLibrary( _bool enable )
{
	mDebugLibrary = enable;
}

_void ResourceManager::EnableDebugPDB( _bool enable, _bool multidebug )
{
	mDebugPDB	= enable;
	mMultiDebug	= multidebug;
}

Library* ResourceManager::CreateLibrary( StringPtr filename )
{
	// Enter resource lock, search existing library.
	{
		LockOwner lockowner( mResourceLock );

		for ( _dword i = 0; i < mLibraryArray.Number( ); i ++ )
		{
			LibraryDesc& libdesc = mLibraryArray[i];

			if ( libdesc.mLibrary == _null )
				continue;

			if ( libdesc.mFileName.CompareLowercase( filename ) )
			{
				libdesc.mRefCount ++;

				return libdesc.mLibrary;
			}
		}
	}

	Library* newlibrary = new Library;

	if ( mDebugLibrary && mDebugPDB )
	{
		// Load from hard disk first.
		if ( newlibrary->Load( FileSystem::GetCurrentDir( ) + L"\\" + filename ) )
		{
			LibraryDesc libdesc;
			libdesc.mLibrary	= newlibrary;
			libdesc.mFileName	= filename;
			libdesc.mRefCount	= 1;

			mResourceLock.Enter( );
			mLibraryArray.Append( libdesc );
			mResourceLock.Leave( );

			PS_TRACE1( L"[RM] Load Library From File %s\r\n", (const _char*) filename )

			return newlibrary;
		}
	}

	MemFile libfile;

	if ( mDebugLibrary )
		libfile.Open( FileSystem::GetCurrentDir( ) + L"\\" + filename );

	if ( libfile.GetLength( ) == 0 )
	{
		// In this function, we may share file via network, so we must not enter resource lock.
		if ( LoadResource( filename, libfile ) == _false )
		{
			delete newlibrary;
			return _null;
		}
	}

	LibraryDesc libdesc;
	libdesc.mLibrary	= newlibrary;
	libdesc.mFileName	= filename;
	libdesc.mRefCount	= 1;

	if ( mDebugPDB )
	{
		MemFile pdbfile;

		String pdbfilename = StringFormatter::GetFileName( filename, _false, _true ).Lowercase( ) + L".pdb";

		// Try to load pdb file also.
		if ( SearchResource( pdbfilename, _false, _false ) )
			LoadResource( pdbfilename, pdbfile );

		String tempname = StringFormatter::GetFileName( filename, _false ).Lowercase( );
		String tempdll  = tempname + L".plb";
		String temppdb  = tempname + L".pdb";

		String temppath;

		if ( mManifestVersion != L"" )
		{
			temppath = FileSystem::GetCurrentDir( );
			temppath += L"\\PSTEMP";
			temppath += mManifestVersion;
		}
		else
		{
			temppath = System::GetSystemTempPath( );
			temppath += L"\\PSTEMP";

			// Add Process ID into temp path, will be used at client side only.
			if ( mMultiDebug )
				temppath += Long( System::GetCurrentProcessID( ) ).ToString( );
		}

		if ( temppath != L"" )
		{
			temppath += L"\\";

			tempdll = temppath + tempdll;
			temppdb = temppath + temppdb;
		}

		if ( temppath != L"" )
		{
			FileSystem::CreateDir( temppath );
			FileSystem::SetAttributes( temppath, FileSystem::_ATTRIBUTE_DIRECTORY | FileSystem::_ATTRIBUTE_HIDDEN );
		}

		// Load library in memory cant use M$ debug function, so we release it to hard disk, and load from hard disk.
		if ( libfile.GetLength( ) > 0 )
		{
			FileSystem::Create( tempdll, libfile.GetBuffer( ), libfile.GetLength( ) );
			FileSystem::SetAttributes( tempdll, FileSystem::_ATTRIBUTE_NORMAL | FileSystem::_ATTRIBUTE_HIDDEN );

			libdesc.mDllFileName = tempdll;

			//File tempdllfile;
			//if ( tempdllfile.Open( tempdll, File::_CREATE_ALWAYS, File::_OPERATION_WRITE, File::_SHARE_READ | File::_SHARE_WRITE | File::_SHARE_DELETEONCLOSE ) )
			//{
			//	tempdllfile.Write( libfile.GetBuffer( ), libfile.GetLength( ) );

			//	// Clone the file handle, to prevent file auto delete.
			//	tempdllfile.CloneHandle( );

			//	libdesc.mDllFileName = tempdll;
			//}
		}

		// Also release pdb file to hard disk.
		if ( pdbfile.GetLength( ) > 0 )
		{
			FileSystem::Create( temppdb, pdbfile.GetBuffer( ), pdbfile.GetLength( ) );
			FileSystem::SetAttributes( temppdb, FileSystem::_ATTRIBUTE_NORMAL | FileSystem::_ATTRIBUTE_HIDDEN );

			libdesc.mPdbFileName = temppdb;

			//File temppdbfile;
			//if ( temppdbfile.Open( temppdb, File::_CREATE_ALWAYS, File::_OPERATION_WRITE, File::_SHARE_READ | File::_SHARE_WRITE | File::_SHARE_DELETEONCLOSE ) )
			//{
			//	temppdbfile.Write( pdbfile.GetBuffer( ), pdbfile.GetLength( ) );

			//	// Clone the file handle, to prevent file auto delete.
			//	temppdbfile.CloneHandle( );

			//	libdesc.mPdbFileName = temppdb;
			//}
		}

		PS_TRACE1( L"[RM] Load Library From File %s\r\n", (const _char*) tempdll )

		if ( newlibrary->Load( tempdll ) == _false )
		{
			delete newlibrary;
			return _null;
		}
	}
	else
	{
		PS_TRACE1( L"[RM] Load Library From Memory %s\r\n", (const _char*) filename )

		if ( newlibrary->Load( libfile.GetBuffer( ) ) == _false )
		{
			delete newlibrary;
			return _null;
		}
	}

	mResourceLock.Enter( );
	mLibraryArray.Append( libdesc );
	mResourceLock.Leave( );

	return newlibrary;
}

_void ResourceManager::ReleaseLibrary( Library* library )
{
	LockOwner lockowner( mResourceLock );

	for ( _dword i = 0; i < mLibraryArray.Number( ); i ++ )
	{
		LibraryDesc& libdesc = mLibraryArray[i];

		if ( libdesc.mLibrary == _null )
			continue;

		if ( libdesc.mLibrary == library )
		{
			if ( libdesc.mRefCount > 0 )
				libdesc.mRefCount --;

			// For now never release library.
			//if ( libdesc.mRefCount == 0 )
			//{
			//	delete libdesc.mLibrary;

			//	// Delete dll file.
			//	if ( libdesc.mDllFileName != L"" )
			//		FileSystem::Delete( libdesc.mDllFileName );

			//	// Delete pdb file.
			//	if ( libdesc.mPdbFileName != L"" )
			//		FileSystem::Delete( libdesc.mPdbFileName );

			//	mLibraryArray.Remove( i );
			//}

			return;
		}
	}
}

_void ResourceManager::InsertAdditionalDebugFile( StringPtr filename )
{
	// Try to load pdb file.
	MemFile pdbfile;
	LoadResource( filename, pdbfile );

	if ( pdbfile.GetLength( ) == 0 )
		return;

	String tempname = StringFormatter::GetFileName( filename, _false ).Lowercase( ) + L".pdb";
	String temppath = System::GetSystemTempPath( );

	if ( temppath != L"" )
		tempname = temppath + L"\\" + tempname;

	FileSystem::Create( tempname, pdbfile.GetBuffer( ), pdbfile.GetLength( ) );
	FileSystem::SetAttributes( tempname, FileSystem::_ATTRIBUTE_NORMAL | FileSystem::_ATTRIBUTE_HIDDEN );

	LibraryDesc debuglibdesc;
	debuglibdesc.mLibrary		= _null;
	debuglibdesc.mRefCount		= 0;
	debuglibdesc.mPdbFileName	= tempname;
	mLibraryArray.Append( debuglibdesc );
}

_void ResourceManager::LoadAllLibraryDebugFile( )
{
	for ( _dword i = 0; i < mLibraryArray.Number( ); i ++ )
	{
		LibraryDesc& libdesc = mLibraryArray[i];

		// Copy pdb file.
		if ( libdesc.mPdbFileName != L"" )
		{
			String tempfilename = StringFormatter::GetFileName( libdesc.mPdbFileName );

			if ( FileSystem::DoesFileExist( tempfilename ) == _false )
			{
				libdesc.mTempFileName = tempfilename;

				FileSystem::Copy( libdesc.mPdbFileName, tempfilename );

				_bool asserterror = Trace::IsAssertErrorEnabled( );

				Trace::EnableAssertError( _false );
				PS_ERROR2( L"LoadAllLibraryDebugFile : Copy %s => %s\r\n", (const _char*) libdesc.mPdbFileName, (const _char*) tempfilename );
				Trace::EnableAssertError( asserterror );
			}
		}
	}
}

_void ResourceManager::FreeAllLibraryDebugFile( )
{
	for ( _dword i = 0; i < mLibraryArray.Number( ); i ++ )
	{
		LibraryDesc& libdesc = mLibraryArray[i];

		// Delete temp file.
		if ( libdesc.mTempFileName != L"" )
		{
			_bool asserterror = Trace::IsAssertErrorEnabled( );

			Trace::EnableAssertError( _false );
			PS_ERROR1( L"FreeAllLibraryDebugFile : Delete %s\r\n", (const _char*) libdesc.mTempFileName );
			Trace::EnableAssertError( asserterror );

			FileSystem::Delete( libdesc.mTempFileName );

			libdesc.mTempFileName = L"";
		}
	}
}

_bool ResourceManager::LoadResource( StringPtr resname, MemFile& memfile, _bool decompress )
{
	if ( resname == L"" )
		return _false;

	LockOwner lockowner( mResourceLock );

	if ( mDebugResource )
	{
		if ( memfile.Open( resname ) )
			return _true;

		// Only deep search when resource has no path.
		if ( resname.SearchL2R( '\\' ) == -1 && resname.SearchL2R( '/' ) == -1 )
		{
			_char buffer[1024];

			for ( _dword i = 0; i < mSearchPathArray.Number( ); i ++ )
			{
				StringPtr searchpath = mSearchPathArray[ mSearchPathArray.Number( ) - i - 1 ].mObject1;

				StringFormatter::FormatBuffer( buffer, 1024, L"%s\\%s", searchpath, resname );
			}

			if ( memfile.Open( buffer ) )
				return _true;
		}
	}

	IDataStream* datastream = OpenResource( resname );

	if ( datastream == _null )
		return _false;

	memfile.SetFileName( datastream->GetName( ) );

	_bool retval = datastream->Mapping( memfile, decompress );

	datastream->Release( );

	return retval;
}

_bool ResourceManager::SaveResource( StringPtr resname, MemFile& memfile )
{
	return CreateResource( resname, memfile.GetBuffer( ), memfile.GetLength( ), _null );
}

_bool ResourceManager::CreateResource( StringPtr resname, const _void* buffer, _dword size, const Oword* checksum )
{
	LockOwner lockowner( mResourceLock );

	CreateStruct createstruct;
	createstruct.mBuffer	= (_void*) buffer;
	createstruct.mSize		= size;
	createstruct.mChecksum	= (Oword*) checksum;

	_bool retval = _false;

	// Try to create resource.
	SearchResourceHelper( GetValidResourceName( resname ), _PROCESS_CREATE, &createstruct, (_dword*) &retval );

	return retval;
}

_bool ResourceManager::CreateResource( StringPtr resname, IDataStream* datastream, _dword* finish )
{
	if ( datastream == _null )
		return _false;

	// Not enter lock for whole function, just for convert resource name.

	// Convert logic game to real name, need use string here, because later it maybe changed.
	mResourceLock.Enter( );
	String lowercasedname = GetValidResourceName( resname );
	mResourceLock.Leave( );

	IDataArchive* desarchive = SearchArchiveToCreateResource( lowercasedname );
	if ( desarchive == _null )
		return _false;

	_dword copymethod = 0; // 0 - Keep, 1 - Decompress, 2 - Compress.

	// Package/PERes/Mem to file, decompress it.
	if ( desarchive->GetArchiveType( ) == _ARCHIVE_FILE )
	{
		copymethod = 1;
	}
	// File to package, compress it.
	else if ( datastream->GetStreamType( ) == _ARCHIVE_FILE && ( desarchive->GetArchiveType( ) == _ARCHIVE_PACK || desarchive->GetArchiveType( ) == _ARCHIVE_HASH ) )
	{
		copymethod = 2;
	}

	MemFile memfile;

	// Directly use data stream buffer.
	if ( copymethod == 0 && datastream->GetBuffer( ) != _null )
	{
		memfile.Open( datastream->GetBuffer( ), datastream->GetSize( ), _false );
		memfile.EnableAutoDeleteBuffer( _false );
	}
	// Map into memory file.
	else
	{
		if ( datastream->Mapping( memfile, copymethod == 1 ) == _false )
			return _false;
	}

	// Compress memory file.
	if ( copymethod == 2 )
		Compression::Compress( memfile, 0.8f );

	Oword reschecksum = datastream->GetChecksum( );

	if ( finish != _null )
		*finish = _ASYNC_PROCESSING;

	// Copy resource to target archive.
	if ( desarchive->CreateResource( lowercasedname, memfile.GetBuffer( ), memfile.GetLength( ), &reschecksum, finish ) == _false )
		return _false;

	return _true;
}

_bool ResourceManager::DeleteResource( StringPtr resname )
{
	LockOwner lockowner( mResourceLock );

	_bool retval = _false;

	// Try to delete resource.
	SearchResourceHelper( GetValidResourceName( resname ), _PROCESS_DELETE, _null, (_dword*) &retval );

	return retval;
}

_bool ResourceManager::SearchResource( StringPtr resname, _dword* size, Oword* checksum )
{
	LockOwner lockowner( mResourceLock );

	SearchStruct searchstruct;
	searchstruct.mSize		= size;
	searchstruct.mChecksum	= checksum;

	_bool retval = _false;

	// Try to search resource.
	SearchResourceHelper( GetValidResourceName( resname ), _PROCESS_SEARCH, &searchstruct, (_dword*) &retval );

	// Try to search resource with deep search.
	if ( retval == _false )
	{
		// Only deep search when resource has no path.
		if ( resname.SearchL2R( '\\' ) == -1 && resname.SearchL2R( '/' ) == -1 )
		{
			// Try to open in each search path, from back to front.
			for ( _dword i = 0; i < mSearchPathArray.Number( ); i ++ )
			{
				StringPtr searchpath = mSearchPathArray[ mSearchPathArray.Number( ) - i - 1 ].mObject1;

				SearchResourceHelper( GetValidResourceName( searchpath, resname ), _PROCESS_SEARCH, &searchstruct, (_dword*) &retval );

				if ( retval != _false )
				{
					mLastSearchResult = GetValidResourceName( searchpath, resname );

					break;
				}
			}
		}
	}
	else
	{
		mLastSearchResult = resname;
	}

	return retval;
}

_bool ResourceManager::CopyResource( StringPtr srcresname, StringPtr desresname, _bool* filechanged, const Oword* checksum )
{
	LockOwner lockowner( mResourceLock );

	if ( filechanged != _null )
		*filechanged = _false;

	if ( mArchiveArray.Number( ) == 0 )
		return FileSystem::Copy( srcresname, desresname );

	String srcfilename = srcresname;
	String desfilename = desresname;

	IDataArchive* srcarchive = SearchArchiveByResource( srcfilename );
	if ( srcarchive == _null )
		return _false;

	IDataArchive* desarchive = SearchArchiveToCreateResource( desfilename );
	if ( desarchive == _null )
		return _false;

	return CopyResourceHelper( srcarchive, desarchive, srcfilename, desfilename, filechanged, checksum );
}

_bool ResourceManager::MoveResource( StringPtr srcresname, StringPtr desresname, _bool* filechanged, const Oword* checksum )
{
	LockOwner lockowner( mResourceLock );

	if ( filechanged != _null )
		*filechanged = _false;

	if ( mArchiveArray.Number( ) == 0 )
		return FileSystem::Move( srcresname, desresname );

	String srcfilename = srcresname;
	String desfilename = desresname;

	IDataArchive* srcarchive = SearchArchiveByResource( srcfilename );
	if ( srcarchive == _null )
		return _false;

	IDataArchive* desarchive = SearchArchiveToCreateResource( desfilename );
	if ( desarchive == _null )
		return _false;

	return MoveResourceHelper( srcarchive, desarchive, srcfilename, desfilename, filechanged, checksum );
}

_bool ResourceManager::CheckResource( StringPtr resname, const Oword& checksum, _bool deleteinvalidfile )
{
	LockOwner lockowner( mResourceLock );

	// Bad check sum, we just check if file existing.
	if ( checksum == Oword( ) )
		return SearchResource( resname, _null, _null );

	// Search file desc in dictionary.
	FileDesc* filedesc = mFileDictionary.Index( GetValidResourceName( resname ), 1 );

	if ( filedesc != _null )
	{
		// This file already checked.
		if ( filedesc->mChecked == _true )
			return _true;

		// This file dont need check.
		if ( filedesc->mFileSize == 0 )
			return _true;
	}

	IDataStream* stream = OpenResource( resname );
	if ( stream == _null )
		return _false;

	_dword checkresult = 0;

	// Great, we can check only against data, much faster.
	if ( filedesc != _null && filedesc->mDataChecksum != Oword( ) )
	{
		// Check resource without name, faster.
		mAsyncIOManager->ProcessCheck( stream, filedesc->mDataChecksum, _false, &checkresult );
	}
	else
	{
		// Check resource with name, slower.
		mAsyncIOManager->ProcessCheck( stream, filedesc->mFullChecksum, _true, &checkresult );
	}

	stream->Release( );

	// Check failed.
	if ( checkresult == _ASYNC_DONE_FALSE )
	{
		// Delete local resource.
		if ( deleteinvalidfile )
			DeleteResource( resname );
	}

	return checkresult == _ASYNC_DONE_TRUE;
}

_bool ResourceManager::CompareResource( StringPtr srcresname, StringPtr desresname )
{
	LockOwner lockowner( mResourceLock );

	MemFile srcfile;
	if ( LoadResource( srcresname, srcfile ) == _false )
		return _false;

	MemFile desfile;
	if ( LoadResource( desresname, desfile ) == _false )
		return _false;

	if ( srcfile.GetLength( ) != desfile.GetLength( ) )
		return _false;

	return Memory::MemCmp( srcfile.GetBuffer( ), desfile.GetBuffer( ), desfile.GetLength( ) );
}

_bool ResourceManager::CheckLocalResource( _bool onlyneededfile, _bool deleteinvalidfile, StringPtr skipfile )
{
	LockOwner lockowner( mResourceLock );

	CheckStruct checkstruct;
	checkstruct.mSkipFile			= skipfile;
	checkstruct.mOnlyNeededFile		= onlyneededfile;
	checkstruct.mDeleteInvalidFile	= deleteinvalidfile;
	checkstruct.mAllResourceReady	= _true;

	EnumManifest( OnCheckLocalResource, this, &checkstruct, _null );

	return checkstruct.mAllResourceReady;
}

_void ResourceManager::ClearSearchPath( )
{
	LockOwner lockowner( mResourceLock );

	mSearchPathArray.Clear( );
	mLastSearchResult = L"";
}

_void ResourceManager::InsertSearchPath( StringPtr searchpath )
{
	LockOwner lockowner( mResourceLock );

	String lowercasedpath = searchpath;
	lowercasedpath.Lowercase( );
	lowercasedpath.TrimRight( L"\\/ " );
	lowercasedpath.ReplaceAll( '/', '\\' );

	for ( _dword i = 0; i < mSearchPathArray.Number( ); i ++ )
	{
		if ( mSearchPathArray[i].mObject1.CompareLowercase( lowercasedpath ) )
		{
			mSearchPathArray[i].mObject2 ++;

			return;
		}
	}

	mSearchPathArray.Append( SearchPathPair( lowercasedpath, 1 ) );
}

_void ResourceManager::RemoveSearchPath( StringPtr searchpath )
{
	LockOwner lockowner( mResourceLock );

	String lowercasedpath = searchpath;
	lowercasedpath.Lowercase( );
	lowercasedpath.TrimRight( L"\\/ " );
	lowercasedpath.ReplaceAll( '/', '\\' );

	for ( _dword i = 0; i < mSearchPathArray.Number( ); i ++ )
	{
		if ( mSearchPathArray[i].mObject1 == lowercasedpath )
		{
			if ( mSearchPathArray[i].mObject2 > 0 )
				mSearchPathArray[i].mObject2 --;

			if ( mSearchPathArray[i].mObject2 == 0 )
				mSearchPathArray.Remove( i );

			break;
		}
	}
}

_dword ResourceManager::GetSearchPathNumber( ) const
{
	return mSearchPathArray.Number( );
}

StringPtr ResourceManager::GetSearchPath( _dword index ) const
{
	return mSearchPathArray[ index ].mObject1;
}

_void ResourceManager::ClearFileAlias( )
{
	LockOwner lockowner( mResourceLock );

	mFileAliasArray.Clear( );
}

_void ResourceManager::InsertFileAlias( StringPtr filename, StringPtr filealias )
{
	LockOwner lockowner( mResourceLock );

	String lowercasedfilename  = filename;
	lowercasedfilename.Lowercase( );
	lowercasedfilename.ReplaceAll( '/', '\\' );

	String lowercasedfilealias = filealias;
	lowercasedfilealias.Lowercase( );
	lowercasedfilealias.ReplaceAll( '/', '\\' );

	for ( _dword i = 0; i < mFileAliasArray.Number( ); i ++ )
	{
		// Found existing file alias.
		if ( mFileAliasArray[i].mObject1 == lowercasedfilename )
		{
			mFileAliasArray[i].mObject2 = lowercasedfilealias;
			return;
		}
	}

	mFileAliasArray.Append( FileAliasPair( lowercasedfilename, lowercasedfilealias ) );
}

_void ResourceManager::RemoveFileAlias( StringPtr filename )
{
	LockOwner lockowner( mResourceLock );

	if ( mFileAliasArray.Number( ) == 0 )
		return;

	String lowercasedfilename  = filename;
	lowercasedfilename.Lowercase( );
	lowercasedfilename.ReplaceAll( '/', '\\' );

	for ( _dword i = 0; i < mFileAliasArray.Number( ); i ++ )
	{
		// Found existing file alias.
		if ( mFileAliasArray[i].mObject1 == lowercasedfilename )
		{
			mFileAliasArray.Remove( i );
			return;
		}
	}
}

StringPtr ResourceManager::GetFileAlias( StringPtr filename ) const
{
	LockOwner lockowner( mResourceLock );

	if ( mFileAliasArray.Number( ) == 0 )
		return filename;

	String lowercasedfilename  = filename;
	lowercasedfilename.Lowercase( );
	lowercasedfilename.ReplaceAll( '/', '\\' );

	for ( _dword i = 0; i < mFileAliasArray.Number( ); i ++ )
	{
		if ( mFileAliasArray[i].mObject1 == lowercasedfilename )
			return mFileAliasArray[i].mObject2;
	}

	return filename;
}

_void ResourceManager::ClearNameVariable( )
{
	LockOwner lockowner( mResourceLock );

	mNameVariableArray.Clear( );
}

_void ResourceManager::InsertNameVariable( StringPtr name, StringPtr value )
{
	LockOwner lockowner( mResourceLock );

	// Convert logic game to real name.
	_char lowercasedname[1024];
	StringFormatter::CopyString( lowercasedname, name );
	StringFormatter::LowercaseString( lowercasedname );

	// Search in name variable array.
	_dword index = mNameVariableArray.SearchAscending( name ).mIndex;

	// Put new variable into array.
	if ( index == -1 )
	{
		mNameVariableArray.InsertAscending( NameVariablePair( String( lowercasedname ), String( value ) ) );
	}
	// Already existing in name variable array, just update value.
	else
	{
		mNameVariableArray[index].mObject2 = value;
	}
}

_void ResourceManager::RemoveNameVariable( StringPtr name )
{
	LockOwner lockowner( mResourceLock );

	// Convert logic game to real name.
	_char lowercasedname[1024];
	StringFormatter::CopyString( lowercasedname, name );
	StringFormatter::LowercaseString( lowercasedname );

	// Search in name variable array.
	_dword index = mNameVariableArray.SearchAscending( name ).mIndex;
	if ( index == -1 )
		return;

	mNameVariableArray.Remove( index );
}

StringPtr ResourceManager::GetNameVariableValue( StringPtr name ) const
{
	LockOwner lockowner( mResourceLock );

	// Convert logic game to real name.
	_char lowercasedname[1024];
	StringFormatter::CopyString( lowercasedname, name );
	StringFormatter::LowercaseString( lowercasedname );

	// Search in name variable array.
	_dword index = mNameVariableArray.SearchAscending( name ).mIndex;
	if ( index == -1 )
		return L"";

	return mNameVariableArray[ index ].mObject2;
}

StringPtr ResourceManager::ParseNameVariable( StringPtr name, _char* buffer, _dword size ) const
{
	if ( name[0] != '$' || mNameVariableArray.Number( ) == 0 )
		return L"";

	FL_ASSERT( buffer != _null && size != 0 )

	LockOwner lockowner( mResourceLock );

	// Prepare for key buffer.
	_char key[1024]; _dword keyindex = -1;

	// Parse variable in name variable.
	_dword length = name.Length( ), index = 0; size --;

	for ( _dword i = 1; i < length; i ++ )
	{
		// Found key starts.
		if ( name[i] == '<' )
		{
			keyindex = 0;
		}
		// Found key end.
		else if ( name[i] == '>' )
		{
			key[ keyindex ] = 0;
			keyindex = -1;

			// Get variable value from key.
			StringPtr value = GetNameVariableValue( key );
			_dword valuelength = Math::Min( size - index, value.Length( ) );

			// Append value into result buffer.
			for ( _dword j = 0; j < valuelength; j ++ )
				buffer[ index ++ ] = value[j];

			if ( index == size )
				break;
		}
		// Found normal char.
		else
		{
			// Write to key.
			if ( keyindex != -1 )
			{
				key[ keyindex ++ ] = name[i];
			}
			// Write to buffer.
			else
			{
				buffer[ index ++ ] = name[i];

				if ( index == size )
					break;
			}
		}
	}

	buffer[index] = 0;

	return StringPtr( buffer );
}

_dword ResourceManager::EnumArchive( StringPtr archivename, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2, _float* percentage )
{
	LockOwner lockowner( mResourceLock );

	IDataArchive* dataarchive = SearchArchiveByName( archivename );

	if ( dataarchive == _null )
		return 0;

	return dataarchive->EnumArchive( funcpointer, parameter1, parameter2, percentage );
}

_bool ResourceManager::CopyArchive( StringPtr srcarchivename, StringPtr desarchivename, _float* percentage )
{
	LockOwner lockowner( mResourceLock );

	IDataArchive* srcarchive = SearchArchiveByName( srcarchivename );
	if ( srcarchive == _null )
		return _false;

	IDataArchive* desarchive = SearchArchiveByName( desarchivename );
	if ( desarchive == _null )
		return _false;

	srcarchive->EnumArchive( OnCopyResource, srcarchive, desarchive, percentage );

	return _true;
}

_bool ResourceManager::MoveArchive( StringPtr srcarchivename, StringPtr desarchivename, _float* percentage )
{
	LockOwner lockowner( mResourceLock );

	IDataArchive* srcarchive = SearchArchiveByName( srcarchivename );
	if ( srcarchive == _null )
		return _false;

	IDataArchive* desarchive = SearchArchiveByName( desarchivename );
	if ( desarchive == _null )
		return _false;

	srcarchive->EnumArchive( OnMoveResource, srcarchive, desarchive, percentage );

	return _true;
}

_bool ResourceManager::LoadManifest( StringPtr manifestname, _bool* binformat, _bool merge )
{
	LockOwner lockowner( mResourceLock );

	if ( binformat != _null )
		*binformat = _false;

	TxtFile manifestfile;

	if ( LoadResource( manifestname, manifestfile ) == _false )
		return _false;

	_dword magictokenversion[2] = { 0x464D5350, 0x00000001 };

	// Load as bin format.
	if ( manifestfile.GetLength( ) > sizeof( magictokenversion ) && Memory::MemCmp( manifestfile.GetBuffer( ), magictokenversion, sizeof( magictokenversion ) ) )
	{
		if ( binformat != _null )
			*binformat = _true;

		return LoadManifest( (BinFile&) (MemFile&) manifestfile, merge );
	}
	// Load as txt format.
	else
	{
		return LoadManifest( manifestfile, merge );
	}
}

_bool ResourceManager::LoadManifest( TxtFile& manifestfile, _bool merge )
{
	LockOwner lockowner( mResourceLock );

	_dword magictokenversion[2] = { 0x464D5350, 0x00000001 };

	// Load as bin format.
	if ( manifestfile.GetLength( ) > sizeof( magictokenversion ) && Memory::MemCmp( manifestfile.GetBuffer( ), magictokenversion, sizeof( magictokenversion ) ) )
	{
		return LoadManifest( (BinFile&) (MemFile&) manifestfile, merge );
	}

	// First clear old manifest information.
	if ( merge == _false )
		ClearManifest( );

	manifestfile.SeekToBegin( );
	manifestfile.SetDivision( L"|" );

	String pathname, filename, filechecksum, datachecksum, filesize, version, needed;

	// Parse manifest file.
	while ( manifestfile.ReachEnd( ) == _false )
	{
		_dword offset = -1;

		// Read file name from manifest file.
		manifestfile.ReadWord( filename );
		// Read file checuk sum from manifest file.
		manifestfile.ReadWordInLine( filechecksum );
		// Read data checksum from manifest file.
		manifestfile.ReadWordInLine( datachecksum );

		// For compatible with old version.
		if ( datachecksum[0] != '@' && datachecksum[0] != '#' )
		{
			filesize = datachecksum;
			datachecksum = L"";
		}
		// This is file offset.
		else if ( datachecksum[0] == '#' )
		{
			datachecksum.Remove( 0, 1 );

			offset = Long( datachecksum );
			datachecksum = filechecksum;

			// Read file size from manifest file.
			manifestfile.ReadWordInLine( filesize );
		}
		// This is new version.
		else if ( datachecksum != L"" )
		{
			datachecksum.Remove( 0, 1 );

			// Read file size from manifest file.
			manifestfile.ReadWordInLine( filesize );
		}

		// Read version from manifest file.
		manifestfile.ReadWordInLine( version );
		// Read need flag from manifest file.
		manifestfile.ReadWordInLine( needed );

		filename.Lowercase( );
		version.Lowercase( );

		if ( filename == L"*" )
			filename = L"";

		if ( filechecksum == L"*" )
			continue;

		// Found manifest date.
		if ( filechecksum == L"$" )
		{
			if ( merge == _false )
			{
				mManifestDate = filename;
			}

			continue;
		}

		// Found manifest version.
		if ( filechecksum == L"#" )
		{
			if ( merge == _false )
			{
				mManifestVersion = filename;
				mManifestVersion.Lowercase( );
				mManifestVersion.TrimLeft( 'v' );
			}

			continue;
		}

		// Found current path name.
		if ( filechecksum == L"[]" )
		{
			pathname = filename;

			continue;
		}

		// Just for backwards compatible.
		if ( version == L"needed" )
		{
			version = L""; 
			needed  = L"needed";
		}

		// Build full path name by version + path name + file name.

		_char realfilename[1024], logicfilename[1024];

		// Build logic full name.
		if ( pathname == L"" )
			StringFormatter::FormatBuffer( logicfilename, 1024, L"%s", (const _char*) filename );
		else
			StringFormatter::FormatBuffer( logicfilename, 1024, L"%s\\%s", (const _char*) pathname, (const _char*) filename );

		// Build real full name.
		if ( version == L"" || version == L"*" )
		{
			StringFormatter::FormatBuffer( realfilename, 1024, L"%s", (const _char*) logicfilename );
		}
        else
		{
			StringFormatter::FormatBuffer( realfilename, 1024, L"%s\\%s", (const _char*) version, (const _char*) logicfilename );
		}

		FileDesc newfiledesc;
		newfiledesc.mLogicName		= logicfilename;
		newfiledesc.mRealName		= realfilename;
		newfiledesc.mFileSize		= Long( filesize );
		newfiledesc.mCompressFlag	= Compression::_COMPRESS_UNKNOWN;
		newfiledesc.mFullChecksum	= Oword( filechecksum );
		newfiledesc.mDataChecksum	= Oword( datachecksum );
		newfiledesc.mNeeded			= needed.CompareLowercase( L"needed" );
		newfiledesc.mChecked		= _false;
		newfiledesc.mOffset			= offset;

		_dword filesizelength = filesize.Length( );

		// Check compress status. 
		if ( filesizelength > 0 )
		{
			// Compressed.
			if ( filesize[ filesizelength - 1 ] == '*' )
				newfiledesc.mCompressFlag = Compression::_COMPRESS_YES;
			// Not compressed.
			else if ( filesize[ filesizelength - 1 ] == '#' )
				newfiledesc.mCompressFlag = Compression::_COMPRESS_NO;
		}

		if ( merge )
		{
			// We already have this file.
			if ( mFileDictionary.Index( newfiledesc.mRealName, 1 ) != _null )
				continue;

			// In merge mode, set logic name same as real name.
			newfiledesc.mLogicName = newfiledesc.mRealName;
		}

		// Put file desc into dictionary.
		FileDictionary::Iterator it = mFileDictionary.Insert( newfiledesc );

		// Make lookup table using logic name.
		mFileDictionary.InsertLookup( it, ( (FileDesc&) it ).mLogicName, 0 );
		// Make lookup table using real name.
		mFileDictionary.InsertLookup( it, ( (FileDesc&) it ).mRealName, 1 );

		// We found a version.
		if ( version != L"" && mVersionArray.Search( version ).Valid( ) == _false )
			mVersionArray.Append( version );
	}

	mVersionArray.SortAscending( );

	PS_TRACE2( L"[RM] %s manifest (txt), Total %d\r\n", merge == _false ? L"Loaded" : L"Merged", mFileDictionary.Number( ) )
	PS_TRACE2( L"[RM] Logic Name Lookup Table %d / %d\r\n", mFileDictionary.AvgTableLength( 0 ), mFileDictionary.MaxTableLength( 0 ) )
	PS_TRACE2( L"[RM] Real Name Lookup Table %d / %d\r\n", mFileDictionary.AvgTableLength( 1 ), mFileDictionary.MaxTableLength( 1 ) )

	return _true;
}

_bool ResourceManager::LoadManifest( BinFile& manifestfile, _bool merge )
{
	LockOwner lockowner( mResourceLock );

	// First clear old manifest information.
	if ( merge == _false )
		ClearManifest( );

	manifestfile.SeekToBegin( );

	String version; _dword flag = 0, magictokenversion[2];

	// Read file header.
	if ( manifestfile.ReadBuffer( magictokenversion, sizeof( magictokenversion ) ) == _false )
		return _false;

	if ( magictokenversion[0] != 0x464D5350 || magictokenversion[1] != 0x00000001 )
		return _false;

	String manifestdate, manifestversion;

	// Read version date.
	if ( manifestfile.ReadString( manifestdate ) == _false )
		return _false;

	// Read version string.
	if ( manifestfile.ReadString( manifestversion ) == _false )
		return _false;

	if ( merge == _false )
	{
		mManifestDate	 = manifestdate;
		mManifestVersion = manifestversion;
	}

	// Read file dictionary format.
	_dword format = 0;
	if ( manifestfile.ReadDword( format ) == _false )
		return _false;

	// Read file version number.
	_dword versionnumber = 0;
	if ( manifestfile.ReadDword( versionnumber ) == _false )
		return _false;

	// Read all file version.
	for ( _dword i = 0; i < versionnumber; i ++ )
	{
		if ( manifestfile.ReadString( version ) == _false )
			return _false;

		// We found a version.
		if ( version != L"" && mVersionArray.Search( version ).Valid( ) == _false )
			mVersionArray.Append( version );
	}

	// Read all file desc.
	while ( manifestfile.ReachEnd( ) == _false )
	{
		FileDesc newfiledesc;

		// Load file logic name, with full path.
		if ( manifestfile.ReadString( newfiledesc.mLogicName ) == _false )
			return _false;

		// Load file version.
		if ( manifestfile.ReadString( version ) == _false )
			return _false;

		// Build file real name.
		newfiledesc.mRealName.Format( L"%s\\%s", (const _char*) version, (const _char*) newfiledesc.mLogicName );

		// Load file full checksum.
		if ( manifestfile.ReadBuffer( &newfiledesc.mFullChecksum, sizeof( Oword ) ) == _false )
			return _false;

		// Load file data checksum.
		if ( manifestfile.ReadBuffer( &newfiledesc.mDataChecksum, sizeof( Oword ) ) == _false )
			return _false;

		// Load file size.
		if ( manifestfile.ReadDword( newfiledesc.mFileSize ) == _false )
			return _false;

		// Load file flag.
		if ( manifestfile.ReadDword( flag ) == _false )
			return _false;

		if ( merge )
		{
			// We already have this file.
			if ( mFileDictionary.Index( newfiledesc.mRealName, 1 ) != _null )
				continue;

			// In merge mode, set logic name same as real name.
			newfiledesc.mLogicName = newfiledesc.mRealName;
		}

		// Set file flag.
		newfiledesc.mCompressFlag	= ( flag & 0x00FF0000 ) >> 16;
		newfiledesc.mNeeded			= ( flag & 0x000000FF );
		newfiledesc.mChecked		= _false;
		newfiledesc.mOffset			= -1;

		// Put file desc into dictionary.
		FileDictionary::Iterator it = mFileDictionary.Insert( newfiledesc );

		// Make lookup table using logic name.
		mFileDictionary.InsertLookup( it, ( (FileDesc&) it ).mLogicName, 0 );
		// Make lookup table using real name.
		mFileDictionary.InsertLookup( it, ( (FileDesc&) it ).mRealName, 1 );
	}

	PS_TRACE2( L"[RM] %s manifest (bin), Total %d\r\n", merge == _false ? L"Loaded" : L"Merged", mFileDictionary.Number( ) )
	PS_TRACE2( L"[RM] Logic Name Lookup Table %d / %d\r\n", mFileDictionary.AvgTableLength( 0 ), mFileDictionary.MaxTableLength( 0 ) )
	PS_TRACE2( L"[RM] Real Name Lookup Table %d / %d\r\n", mFileDictionary.AvgTableLength( 1 ), mFileDictionary.MaxTableLength( 1 ) )

	return _true;
}

_bool ResourceManager::SaveManifest( StringPtr manifestname, _bool binformat )
{
	LockOwner lockowner( mResourceLock );

	File manifestfile;
	manifestfile.Open( manifestname, File::_CREATE_ALWAYS, File::_OPERATION_WRITE );

	// Prepare buffer.
	String oldfilepath, newfilepath, newfilename, writebuffer, fileversion;

	// Write as bin format.
	if ( binformat )
	{
		// Write the magic token and version.
		_dword magictokenversion[2] = { 0x464D5350, 0x00000001 };

		// Write file header.
		if ( manifestfile.Write( magictokenversion, sizeof( magictokenversion ) ) == _false )
			return _false;

		// Write version date.
		if ( manifestfile.Write( (const _char*) mManifestDate, mManifestDate.SizeOfBytes( ) ) == _false )
			return _false;

		// Write version string.
		if ( manifestfile.Write( (const _char*) mManifestVersion, mManifestVersion.SizeOfBytes( ) ) == _false )
			return _false;

		// Write file dictionary format.
		_dword format = 0;
		if ( manifestfile.Write( &format, sizeof( format ) ) == _false )
			return _false;

		// Write file version number.
		_dword versionnumber = mVersionArray.Number( );
		if ( manifestfile.Write( &versionnumber, sizeof( versionnumber ) ) == _false )
			return _false;

		// Write all file version.
		for ( _dword i = 0; i < versionnumber; i ++ )
		{
			StringPtr version = mVersionArray[i];

			if ( manifestfile.Write( (const _char*) version, version.SizeOfBytes( ) ) == _false )
				return _false;
		}
	}
	// Write as txt format.
	else
	{
		// Write version date.
		if ( manifestfile.WriteString( mManifestDate + String( L"|$\r\n" ), _false ) == _false )
			return _false;

		// Write version string.
		if ( manifestfile.WriteString( String( L"v" ) + mManifestVersion + String( L"|#\r\n" ), _false ) == _false )
			return _false;
	}

	// Enum all file desc in file dictionary.
	for ( FileDictionary::Iterator it = mFileDictionary.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		const FileDesc& filedesc = it;

		// Get file version from real name and logic name.
		fileversion = String( filedesc.mRealName, filedesc.mRealName.Length( ) - filedesc.mLogicName.Length( ) - 1 );

		// Write as bin format.
		if ( binformat )
		{
			// Wirte file logicname, with full path.
			if ( manifestfile.Write( (const _char*) filedesc.mLogicName, filedesc.mLogicName.SizeOfBytes( ) ) == _false )
				return _false;

			// Wirte file version.
			if ( manifestfile.Write( (const _char*) fileversion, fileversion.SizeOfBytes( ) ) == _false )
				return _false;

			// Wirte file full checksum.
			if ( manifestfile.Write( &filedesc.mFullChecksum, sizeof( Oword ) ) == _false )
				return _false;

			// Wirte file data checksum.
			if ( manifestfile.Write( &filedesc.mDataChecksum, sizeof( Oword ) ) == _false )
				return _false;

			// Wirte file size.
			if ( manifestfile.Write( &filedesc.mFileSize, sizeof( _dword ) ) == _false )
				return _false;

			_dword flag = ( filedesc.mCompressFlag << 16 ) | filedesc.mNeeded;

			// Wirte file flag.
			if ( manifestfile.Write( &flag, sizeof( _dword ) ) == _false )
				return _false;
		}
		// Write as txt format.
		else
		{
			// Write the file manifest down.
			newfilepath = StringFormatter::GetPathName( filedesc.mLogicName );
			newfilename = StringFormatter::GetFileName( filedesc.mLogicName );

			// Write file path.
			if ( oldfilepath != newfilepath )
			{
				writebuffer = StringFormatter::FormatString( L"%s|[]\r\n", newfilepath == L"" ? L"*" : (const _char*) newfilepath );

				// Write into temp file.
				if ( manifestfile.WriteString( writebuffer, _false ) == _false )
					return _false;

				// Set new file path.
				oldfilepath = newfilepath;
			}

			StringPtr compressflag = L"";

			// Get compress flag.
			if ( filedesc.mCompressFlag == Compression::_COMPRESS_YES )
				compressflag = L"*";
			else if ( filedesc.mCompressFlag == Compression::_COMPRESS_NO )
				compressflag = L"#";

			if ( filedesc.mDataChecksum != Oword( ) )
			{
				// Build manifest string.
				writebuffer = StringFormatter::FormatString( L"%s|%s|@%s|%d%s|%s%s\r\n", (const _char*) newfilename,
					(const _char*) filedesc.mFullChecksum.ToString( ), (const _char*) filedesc.mDataChecksum.ToString( ),
					filedesc.mFileSize, compressflag, (const _char*) fileversion, filedesc.mNeeded ? L"|needed" : L"" );
			}
			else
			{
				// Build manifest string.
				writebuffer = StringFormatter::FormatString( L"%s|%s|%d%s|%s%s\r\n", (const _char*) newfilename,
					(const _char*) filedesc.mFullChecksum.ToString( ),
					filedesc.mFileSize, compressflag, (const _char*) fileversion, filedesc.mNeeded ? L"|needed" : L"" );
			}

			// Write into temp file.
			if ( manifestfile.WriteString( writebuffer, _false ) == _false )
				return _false;
		}
	}

	return _true;
}

_bool ResourceManager::InsertManifest( StringPtr filename, _dword size, const Oword& fullchecksum, const Oword& datachecksum, _dword flag, _bool needed )
{
	LockOwner lockowner( mResourceLock );

	// Make file name lowercased.
	_char lowercasedfilename[1024];
	StringFormatter::CopyString( lowercasedfilename, filename );
	StringFormatter::LowercaseString( lowercasedfilename );

	// Search file desc in dictionary.
	FileDesc* filedesc = mFileDictionary.Index( lowercasedfilename, 0 );

	// Not found file desc.
	if ( filedesc == _null )
	{
		FileDesc newfiledesc;
		newfiledesc.mLogicName		= lowercasedfilename;
		newfiledesc.mRealName		= String( L"v" ) + mManifestVersion + String( L"\\" ) + String( lowercasedfilename );
		newfiledesc.mFileSize		= size;
		newfiledesc.mCompressFlag	= flag;
		newfiledesc.mFullChecksum	= fullchecksum;
		newfiledesc.mDataChecksum	= datachecksum;
		newfiledesc.mNeeded			= needed;
		newfiledesc.mChecked		= _false;
		newfiledesc.mOffset			= -1;

		// Put file desc into dictionary.
		FileDictionary::Iterator it = mFileDictionary.Insert( newfiledesc );

		// Make lookup table using logic name.
		mFileDictionary.InsertLookup( it, ( (FileDesc&) it ).mLogicName, 0 );
		// Make lookup table using real name.
		mFileDictionary.InsertLookup( it, ( (FileDesc&) it ).mRealName, 1 );
	}
	else
	{
		filedesc->mFileSize		= size;
		filedesc->mCompressFlag	= flag;
		filedesc->mFullChecksum = fullchecksum;
		filedesc->mDataChecksum = datachecksum;
	}

	return _true;
}

_bool ResourceManager::RemoveManifest( StringPtr filename )
{
	LockOwner lockowner( mResourceLock );

	// Make file name lowercased.
	_char lowercasedfilename[1024];
	StringFormatter::CopyString( lowercasedfilename, filename );
	StringFormatter::LowercaseString( lowercasedfilename );

	// Search file desc in dictionary.
	FileDictionary::Iterator it = mFileDictionary.Search( lowercasedfilename, 0 );

	// Not found file desc.
	if ( it.Valid( ) == _false )
		return _false;

	FileDesc& filedesc = it;

	mFileDictionary.RemoveLookup( filedesc.mLogicName, 0 );
	mFileDictionary.RemoveLookup( filedesc.mRealName, 1 );
	mFileDictionary.Remove( it );

	return _true;
}

_void ResourceManager::ClearManifest( )
{
	LockOwner lockowner( mResourceLock );

	// Clear version array.
	mVersionArray.Clear( );

	// Clear file dictionary.
	mFileDictionary.Clear( );

	// Clear file enum iterator.
	mEnumIterator = mFileDictionary.GetHeadIterator( );

	// Clear manifest information.
	mManifestVersion	= L"";
	mManifestDate		= L"";
}

_dword ResourceManager::GetVersionNumber( ) const
{
	LockOwner lockowner( mResourceLock );

	return mVersionArray.Number( );
}

StringPtr ResourceManager::GetVersionString( _dword index ) const
{
	LockOwner lockowner( mResourceLock );

	return mVersionArray[ index ];
}

_dword ResourceManager::EnumManifest( OnEnumManifest funcpointer, _void* parameter1, _void* parameter2, _float* percentage ) const
{
	LockOwner lockowner( mResourceLock );

	if ( funcpointer == _null )
		return mFileDictionary.Number( );

	// Set percentage to 0.0 at begin.
	if ( percentage != _null )
		*percentage = 0.0f;

	_dword filenumber = 0;

	// Enum all file desc in file dictionary.
	for ( FileDictionary::Iterator it = mFileDictionary.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		const FileDesc& filedesc = it;

		filenumber ++;

		if ( (*funcpointer)( filedesc.mRealName, filedesc.mLogicName, filedesc.mFileSize, filedesc.mFullChecksum, filedesc.mDataChecksum, filedesc.mCompressFlag, filedesc.mNeeded, parameter1, parameter2 ) == _false )
			break;

		// Update percentage.
		if ( percentage != _null )
			*percentage = (_float) filenumber / (_float) mFileDictionary.Number( );
	}

	// Set percentage to 1.0 at end.
	if ( percentage != _null )
		*percentage = 1.0f;

	return filenumber;
}

StringPtr ResourceManager::GetFirstFileInManifest( _dword* size, Oword* checksum )
{
	LockOwner lockowner( mResourceLock );

	// Get head iterator from file dictionary.
	mEnumIterator = mFileDictionary.GetHeadIterator( );

	if ( mEnumIterator.Valid( ) == _false )
		return L"";

	// Get file desc from iterator.
	const FileDesc& filedesc = mEnumIterator;

	if ( size != _null )
		*size = filedesc.mFileSize;

	if ( checksum != _null )
		*checksum = filedesc.mFullChecksum;

	return filedesc.mLogicName;
}

StringPtr ResourceManager::GetNextFileInManifest( _dword* size, Oword* checksum )
{
	LockOwner lockowner( mResourceLock );

	if ( mEnumIterator.Valid( ) == _false )
		return L"";

	// Get next iterator from file dictionary.
	mEnumIterator ++;

	if ( mEnumIterator.Valid( ) == _false )
		return L"";

	// Get file desc from iterator.
	const FileDesc& filedesc = mEnumIterator;

	if ( size != _null )
		*size = filedesc.mFileSize;

	if ( checksum != _null )
		*checksum = filedesc.mFullChecksum;

	return filedesc.mLogicName;
}

StringPtr ResourceManager::GetManifestVersion( ) const
{
	LockOwner lockowner( mResourceLock );

	return mManifestVersion;
}

_void ResourceManager::SetManifestVersion( StringPtr version )
{
	LockOwner lockowner( mResourceLock );

	mManifestVersion = version;
	mManifestVersion.TrimLeft( 'v' );
}

StringPtr ResourceManager::GetManifestDate( ) const
{
	LockOwner lockowner( mResourceLock );

	return mManifestDate;
}

_void ResourceManager::SetManifestDate( StringPtr date )
{
	LockOwner lockowner( mResourceLock );

	mManifestDate = date;
}

StringPtr ResourceManager::GetLogicPathFromRealPath( StringPtr filename, _dword* size, Oword* checksum, _dword* flag, _bool* needed )
{
	if ( size != _null )
		*size = 0;

	if ( checksum != _null )
		*checksum = Oword( );

	if ( flag != _null )
		*flag = 0;

	// Convert file alias.
	if ( mFileAliasArray.Number( ) > 0 )
		filename = GetFileAlias( filename );

	// Make file name lowercased.
	_char lowercasedfilename[1024];
	StringFormatter::CopyString( lowercasedfilename, filename );
	StringFormatter::LowercaseString( lowercasedfilename );

	// Search file desc in dictionary.
	FileDesc* filedesc = mFileDictionary.Index( lowercasedfilename, 1 );

	// Not found any file desc.
	if ( filedesc == _null )
		return filename;

	// Get file size.
	if ( size != _null )
		*size = filedesc->mFileSize;

	// Get file check sum.
	if ( checksum != _null )
		*checksum = filedesc->mFullChecksum;

	// Get compress flag.
	if ( flag != _null )
		*flag = filedesc->mCompressFlag;

	// Get needed flag.
	if ( needed != _null )
		*needed = filedesc->mNeeded;

	// Convert file alias.
	if ( mFileAliasArray.Number( ) > 0 )
		return GetFileAlias( filedesc->mLogicName );

	// Return logic file name.
	return filedesc->mLogicName;
}

StringPtr ResourceManager::GetRealPathFromLogicPath( StringPtr filename, _dword* size, Oword* checksum, _dword* flag, _bool* needed )
{
	if ( flag != _null )
		*flag = 0;

	// Convert file alias.
	if ( mFileAliasArray.Number( ) > 0 )
		filename = GetFileAlias( filename );

	// Make file name lowercased.
	_char lowercasedfilename[1024];
	StringFormatter::CopyString( lowercasedfilename, filename );
	StringFormatter::LowercaseString( lowercasedfilename );

	// Search file desc in dictionary.
	FileDesc* filedesc = mFileDictionary.Index( lowercasedfilename, 0 );

	// Not found any file desc.
	if ( filedesc == _null )
		return filename;

	// Get file size.
	if ( size != _null )
		*size = filedesc->mFileSize;

	// Get file check sum.
	if ( checksum != _null )
		*checksum = filedesc->mFullChecksum;

	// Get compress flag.
	if ( flag != _null )
		*flag = filedesc->mCompressFlag;

	// Get needed flag.
	if ( needed != _null )
		*needed = filedesc->mNeeded;

	// Convert file alias.
	if ( mFileAliasArray.Number( ) > 0 )
		return GetFileAlias( filedesc->mRealName );

	// Return real file name.
	return filedesc->mRealName;
}

StringPtr ResourceManager::GetFullPathFromDeepSearch( StringPtr filename )
{
	if ( SearchResource( filename, _null, _null ) == _false )
		return L"";

	return mLastSearchResult;
}

StringPtr ResourceManager::GetValidResourceName( StringPtr filename )
{
	if ( filename[0] == '$' )
	{
		ParseNameVariable( filename, mResourceName, 1024 );
	}
	else
	{
		StringFormatter::CopyString( mResourceName, filename );
	}

	StringFormatter::LowercaseString( mResourceName );

	for ( _dword i = 0; mResourceName[i] != 0; i ++ )
	{
		if ( mResourceName[i] == '/' )
			mResourceName[i] = '\\';
	}

	FileDesc* filedesc = mFileDictionary.Index( mResourceName, 0 );
	if ( filedesc != _null )
		StringFormatter::CopyString( mResourceName, filedesc->mRealName );

	if ( mFileAliasArray.Number( ) > 0 )
	{
		for ( _dword i = 0; i < mFileAliasArray.Number( ); i ++ )
		{
			if ( mFileAliasArray[i].mObject1 == StringPtr( mResourceName ) )
			{
				StringFormatter::CopyString( mResourceName, mFileAliasArray[i].mObject2 );
				break;
			}
		}
	}

	return StringPtr( mResourceName );
}

StringPtr ResourceManager::GetValidResourceName( StringPtr filepath, StringPtr filename )
{
	StringFormatter::FormatBuffer( mResourceName, 1024, L"%s\\%s", filepath, filename );
	StringFormatter::LowercaseString( mResourceName );

	for ( _dword i = 0; mResourceName[i] != 0; i ++ )
	{
		if ( mResourceName[i] == '/' )
			mResourceName[i] = '\\';
	}

	FileDesc* filedesc = mFileDictionary.Index( mResourceName, 0 );
	if ( filedesc != _null )
		StringFormatter::CopyString( mResourceName, filedesc->mRealName );

	if ( mFileAliasArray.Number( ) > 0 )
	{
		for ( _dword i = 0; i < mFileAliasArray.Number( ); i ++ )
		{
			if ( mFileAliasArray[i].mObject1 == StringPtr( mResourceName ) )
			{
				StringFormatter::CopyString( mResourceName, mFileAliasArray[i].mObject2 );
				break;
			}
		}
	}

	return StringPtr( mResourceName );
}

_bool ResourceManager::GetResourceInformation( StringPtr filename, _dword& size, Oword& fullchecksum, Oword& datachecksum, _dword* offset )
{
	size = 0;
	fullchecksum = Oword( );
	datachecksum = Oword( );

	// Search file desc in dictionary.
	FileDesc* filedesc = mFileDictionary.Index( GetValidResourceName( filename ), 1 );

	// Not found any file desc.
	if ( filedesc == _null )
		return _false;

	// Get file size.
	size = filedesc->mFileSize;

	// Get file full check sum.
	fullchecksum = filedesc->mFullChecksum;

	// Get file data check sum.
	datachecksum = filedesc->mDataChecksum;

	// Get file offset.
	if ( offset != _null )
		*offset = filedesc->mOffset;

	return _true;
}

_void ResourceManager::BuildFoldersInManifest( )
{
	mFolderTree.Clear( );
	mFolderTree.SetRootNode( L"" );

	// Enum all file desc in file dictionary.
	for ( FileDictionary::Iterator it = mFileDictionary.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		const _char* logicname = (const _char*) ( (const FileDesc&) it ).mLogicName;

		// Get root iterator.
		StringTree::Iterator root = mFolderTree.GetRootIterator( );

		while ( *logicname != 0 )
		{
			// Get folder prefix of root path.
			String folderprefix = StringFormatter::GetPathName( logicname, _false );

			if ( folderprefix == L"" )
				break;

			// Get child iterator.
			StringTree::Iterator child = mFolderTree.SearchInChild( root, folderprefix );

			if ( child.Valid( ) == _false )
				root = mFolderTree.Insert( folderprefix, root );
			else
				root = child;

			logicname += folderprefix.Length( );
			if ( *logicname == '\\' )
				logicname ++;
		}
	}
}

_dword ResourceManager::EnumFilesInArchive( StringPtr filename, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2 )
{
	String pathname = StringFormatter::GetPathName( filename );

	if ( pathname != L"" )
		pathname += L"\\";

	FileFinder	filefinder;
	FileInfo	fileinfo;

	_dword filenumber = 0;

	if ( filefinder.FindFirst( filename, fileinfo ) == _true )
	{
		do
		{
			if ( fileinfo.mName == L"." || fileinfo.mName == L".." )
				continue;

			// Found a file.
			if ( ( fileinfo.mAttributes & FileSystem::_ATTRIBUTE_DIRECTORY ) == 0 )
			{
				filenumber ++;

				if ( funcpointer == _null )
					continue;

				(*funcpointer)( pathname + fileinfo.mName, filenumber, parameter1, parameter2 );
			}
		}
		// Find next file.
		while ( filefinder.FindNext( fileinfo ) == _true );
	}

	return filenumber;
}

_dword ResourceManager::EnumFoldersInArchive( StringPtr rootpath, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2 )
{
	String pathname = rootpath;

	if ( pathname != L"" )
		pathname += L"\\";

	_char filename[1024];
	StringFormatter::FormatBuffer( filename, 1024, L"%s\\*.*", (const _char*) rootpath );

	FileFinder	filefinder;
	FileInfo	fileinfo;

	_dword foldernumber = 0;

	if ( filefinder.FindFirst( filename, fileinfo ) == _true )
	{
		do
		{
			if ( fileinfo.mName == L"." || fileinfo.mName == L".." )
				continue;

			// Found a directory.
			if ( ( fileinfo.mAttributes & FileSystem::_ATTRIBUTE_DIRECTORY ) != 0 )
			{
				foldernumber ++;

				if ( funcpointer == _null )
					continue;

				(*funcpointer)( pathname + fileinfo.mName, foldernumber, parameter1, parameter2 );
			}
		}
		// Find next file.
		while ( filefinder.FindNext( fileinfo ) == _true );
	}

	return foldernumber;
}

_dword ResourceManager::EnumFilesInManifest( StringPtr filename, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2 )
{
	// Make file name lowercased.
	_char lowercasedfilename[1024];
	StringFormatter::CopyString( lowercasedfilename, filename );
	StringFormatter::LowercaseString( lowercasedfilename );

	_dword filenumber = 0;

	// Enum all file desc in file dictionary.
	for ( FileDictionary::Iterator it = mFileDictionary.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		StringPtr logicname = ( (const FileDesc&) it ).mLogicName;

		if ( logicname.CompareWildcard( lowercasedfilename ) == _false )
			continue;

		filenumber ++;

		if ( funcpointer != _null )
			(*funcpointer)( logicname, filenumber, parameter1, parameter2 );
	}

	return filenumber;
}

_dword ResourceManager::EnumFoldersInManifest( StringPtr rootpath, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2 )
{
	// Make root path lowercased.
	_char lowercasedrootpath[1024];
	StringFormatter::CopyString( lowercasedrootpath, rootpath );
	StringFormatter::LowercaseString( lowercasedrootpath );

	_dword pathlength = rootpath.Length( ), foldernumber = 0;

	// Enum using built folder array.
	if ( mFolderTree.Number( ) > 0 )
	{
		const _char* folderbuffer = lowercasedrootpath;

		_char folderprefix[1024];

		// Get root iterator.
		StringTree::Iterator root = mFolderTree.GetRootIterator( );

		// Go through folder structure.
		while ( *folderbuffer != 0 )
		{
			// Get folder prefix of root path.
			StringFormatter::GetPathName( folderprefix, 1024, folderbuffer, _false );

			if ( *folderprefix == 0 )
				StringFormatter::CopyString( folderprefix, folderbuffer );

			// Get child iterator.
			StringTree::Iterator child = mFolderTree.SearchInChild( root, folderprefix );

			if ( child.Valid( ) == _false )
				return 0;

			// Check next folder.
			folderbuffer = folderbuffer + StringPtr( folderprefix ).Length( );

			if ( *folderbuffer == '\\' )
				folderbuffer ++;

			// Found the path node.
			if ( *folderbuffer == 0 )
			{
				child = child.GetChild( );

				while ( child.Valid( ) )
				{
					foldernumber ++;

					if ( funcpointer != _null )
					{
						StringFormatter::FormatBuffer( folderprefix, 1024, L"%s\\%s",
							lowercasedrootpath, (const _char*) (const String&) child );

						(*funcpointer)( folderprefix, foldernumber, parameter1, parameter2 );
					}

					child = child.GetBrother( );
				}

				return foldernumber;
			}

			root = child;
		}
	}
	// Enum using file list.
	else
	{
		Array< String > founddirectory;

		// Enum all file desc in file dictionary.
		for ( FileDictionary::Iterator it = mFileDictionary.GetHeadIterator( ); it.Valid( ); it ++ )
		{
			const String& logicname = ( (const FileDesc&) it ).mLogicName;

			if ( logicname.Length( ) < pathlength )
				continue;

			if ( logicname.SearchL2R( lowercasedrootpath ) != 0 )
				continue;

			StringPtr inner( (const _char*) logicname + pathlength + 1 );

			_dword index = inner.SearchL2R( '\\' );
			if ( index == -1 )
				continue;

			String directory( (const _char*) logicname, index + pathlength + 1 );

			// This folder already found, skip it.
			if ( founddirectory.Search( directory ).Valid( ) == _true )
				continue;

			// Put the found folder into array.
			founddirectory.Append( directory );

			foldernumber ++;

			if ( funcpointer != _null )
				(*funcpointer)( directory, foldernumber, parameter1, parameter2 );
		}
	}

	return foldernumber;
}

_void ResourceManager::IncreaseIOControl( )
{
	mTotalIOControlTime	 ++;
	mHappenIOControlTime ++;
}

_void ResourceManager::IncreaseIORead( _dword bytes )
{
	mTotalIOReadTime	++;
	mHappenIOReadTime	++;
	mTotalIOReadBytes	+= bytes;
}

_void ResourceManager::IncreaseIOWrite( _dword bytes )
{
	mTotalIOWriteTime	++;
	mHappenIOWriteTime	++;
	mTotalIOWriteBytes	+= bytes;
}

_dword ResourceManager::GetRecentIOControlTime( _dword minutes ) const
{
	return mTotalIOControlTime;
}

_dword ResourceManager::GetRecentIOReadTime( _dword minutes ) const
{
	return mTotalIOReadTime;
}

_dword ResourceManager::GetRecentIOReadBytes( _dword minutes ) const
{
	return mTotalIOReadBytes;
}

_dword ResourceManager::GetRecentIOWriteTime( _dword minutes ) const
{
	return mTotalIOWriteTime;
}

_dword ResourceManager::GetRecentIOWriteBytes( _dword minutes ) const
{
	return mTotalIOWriteBytes;
}

_dword ResourceManager::GetTotalIOControlTime( ) const
{
	return mTotalIOControlTime;
}

_dword ResourceManager::GetTotalIOReadTime( ) const
{
	return mTotalIOReadTime;
}

_dword ResourceManager::GetTotalIOReadBytes( ) const
{
	return mTotalIOReadBytes;
}

_dword ResourceManager::GetTotalIOWriteTime( ) const
{
	return mTotalIOWriteTime;
}

_dword ResourceManager::GetTotalIOWriteBytes( ) const
{
	return mTotalIOWriteBytes;
}