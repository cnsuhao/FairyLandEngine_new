//============================================================================
// PackArchive.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// PackArchive Implementation
//----------------------------------------------------------------------------

PackArchive::PackArchive( )
{
	mBasePosition		= 0;
	mRangePosition		= 0;
	mDictionaryOffset	= -1;
	mFileSize			= 0;
}

PackArchive::~PackArchive( )
{
	SaveResourceDictionary( );
}

_void PackArchive::OnFoundPackResource( StringPtr resname, _dword chunkoffset, _dword chunksize, _dword resoffset, _dword ressize, const Oword& reschecksum, _dword userdata )
{
	PackArchive* packarchive = (PackArchive*) userdata;
	FL_ASSERT( packarchive != _null )

	packarchive->CreateResourceHelper( resname, chunkoffset, chunksize, resoffset, ressize, reschecksum, _true );
}

_void PackArchive::OnFoundPackChunk( _dword chunkoffset, _dword chunksize, _dword userdata )
{
	PackArchive* packarchive = (PackArchive*) userdata;
	FL_ASSERT( packarchive != _null )

	// Found an unused chunk.
	PackChunk packchunk;
	packchunk.mChunkOffset	= packarchive->mBasePosition + chunkoffset;
	packchunk.mChunkSize	= chunksize;
	packarchive->mUnusedChunkArray.Append( packchunk );
}

_bool PackArchive::CreateResourceHelper( StringPtr resname, _dword chunkoffset, _dword chunksize, _dword resoffset, _dword ressize, const Oword& reschecksum, _bool fromdict )
{
	PackResource newresource;
	newresource.mName				= resname;
	newresource.mWithPrefix			= _false;
	newresource.mDictSize			= resname.SizeOfBytes( ) + sizeof( _dword ) * 9;
	newresource.mChunkOffset		= mBasePosition + chunkoffset;
	newresource.mChunkSize			= chunksize;
	newresource.mResourceOffset		= mBasePosition + resoffset;
	newresource.mResourceSize		= ressize;
	newresource.mResourceChecksum	= reschecksum;

	if ( fromdict == _false )
		newresource.mName.Lowercase( );

	// Remove path prefix for the resource.
	if ( newresource.mName[0] == mPath[0] )
	{
		_dword length = mPath.Length( );

		if ( newresource.mName.SearchL2R( mPath ) == 0 && ( newresource.mName[ length ] == '\\' || newresource.mName[ length ] == '/' ) )
			newresource.mName.Remove( 0, length + 1 );

		newresource.mWithPrefix = _true;
	}

	if ( fromdict )
		mResourceArray.Append( newresource );
	else
		mResourceArray.InsertAscending( newresource );

	return _true;
}

_bool PackArchive::CreateResourceHelper( StringPtr resname, const _void* buffer, _dword size, const Oword& checksum, _bool useemptychunk, _dword* finish )
{
	if ( mBasePosition != 0 )
		return _false;

	_bool asyncwrite = finish != _null, appendchunk = _false;

	// First clean old file dictionary.
	if ( mDictionaryOffset > 0 && mDictionaryOffset != -1 )
	{
		mFileSize = mDictionaryOffset;
		mFile.SetSize( mFileSize );
	}

	// Clean file dictionary position.
	mDictionaryOffset = 0;

	_dword chunkoffset = 0, chunksize = 0, flagsize[2] = { PackChunk::_FLAG_USED, resname.SizeOfBytes( ) + sizeof( _dword ) * 6 + size };

	// Try to find an empty chunk, enable split chunk if not in async write mode.
	if ( useemptychunk && SearchEmptyChunkHelper( flagsize[1], chunkoffset, chunksize, asyncwrite == _false ) == _true )
	{
		flagsize[1] = chunksize;
	}
	// Not found, so create a new chunk at the end of file.
	else
	{
		chunkoffset	= mFileSize;
		chunksize	= flagsize[1];

		appendchunk = _true;
	}

	// Directly write into file.
	if ( asyncwrite == _false )
	{
		// Set file pointer to chunk begin.
		if ( mFile.SeekFromBegin( chunkoffset ) == _false )
			return _false;

		// Write the chunk header.
		if ( mFile.Write( flagsize, sizeof( flagsize ) ) == _false )
			return _false;
	}

	_byte headerbuffer[1024];

	// Prepare file header.
	_byte* pointer = headerbuffer;

	// Fill resource name length, excluding the terminal null.
	_dword namelength = resname.Length( );
	Memory::MemCpy( pointer, &namelength, sizeof( _dword ) );
	pointer += sizeof( _dword );

	// Fill resource name, including the terminal null.
	Memory::MemCpy( pointer, (const _char*) resname, ( namelength + 1 ) * sizeof( _char ) );
	pointer += ( namelength + 1 ) * sizeof( _char );

	// Fill resource data size and check sum.
	_dword sizechecksum[5] = { size, checksum.Dword1( ), checksum.Dword2( ), checksum.Dword3( ), checksum.Dword4( ) };
	Memory::MemCpy( pointer, sizechecksum, sizeof( sizechecksum ) );
	pointer += sizeof( sizechecksum );

	FL_ASSERT( pointer - headerbuffer < 1024 )

	_dword headerlength = (_dword) ( pointer - headerbuffer );

	// Prepare async header buffer.
	_byte* asyncbuffer = _null;

	// Async write.
	if ( asyncwrite )
	{
		// Create async header buffer, include chuck header and resource header.
		asyncbuffer = (_byte*) Memory::CreateReferencedBuffer( sizeof( flagsize ) + headerlength );

		Memory::MemCpy( asyncbuffer, flagsize, sizeof( flagsize ) );
		Memory::MemCpy( asyncbuffer + sizeof( flagsize ), headerbuffer, headerlength );
	}
	// Directly write.
	else
	{
		// Write resource file header.
		if ( mFile.Write( headerbuffer, headerlength ) == _false )
			return _false;
	}

	_dword fileoffset = chunkoffset + sizeof( flagsize ) + headerlength;

	if ( size > 0 )
	{
		// Async write.
		if ( asyncwrite )
		{
			AsyncIOManager& asynciomanager = ( (ResourceManager*) GetResourceManagerPointer( ) )->GetAsyncIOManager( );

			// Post async write operation.
			asynciomanager.PostAsyncWrite( mAsyncWriteFile, chunkoffset, asyncbuffer, sizeof( flagsize ) + headerlength, (_byte*) buffer, size, finish );

			// Release async header buffer.
			Memory::ReleaseReferencedBuffer( asyncbuffer );
		}
		// Directly write.
		else
		{
			// Write resource data.
			if ( mFile.Write( buffer, size ) == _false )
				return _false;
		}
	}

	if ( appendchunk )
		mFileSize += sizeof( flagsize ) + headerlength + size;

	return CreateResourceHelper( resname, chunkoffset, chunksize, fileoffset, size, checksum );
}

_bool PackArchive::DeleteResourceHelper( _dword index )
{
	if ( index >= mResourceArray.Number( ) )
		return _false;

	// First clean old file dictionary.
	if ( mDictionaryOffset > 0 && mDictionaryOffset != -1 )
	{
		mFileSize = mDictionaryOffset;
		mFile.SetSize( mFileSize );
	}

	// Clean file dictionary position.
	mDictionaryOffset = 0;

	PackChunk packchunk;
	packchunk.mChunkOffset	= mResourceArray[ index ].mChunkOffset;
	packchunk.mChunkSize	= mResourceArray[ index ].mChunkSize;

	mResourceArray.Remove( index );

	// There is no usable resource in the package, just clear it.
	if ( mResourceArray.Number( ) == 0 )
	{
		ClearArchive( );
	}
	else
	{
		_dword i = 0;

		// Now the chuck becomes unused, we try to find a position to insert into unused chunk array.
		for ( i = 0; i < mUnusedChunkArray.Number( ); i ++ )
		{
			if ( mUnusedChunkArray[i].mChunkOffset > packchunk.mChunkOffset )
				break;
		}

		mUnusedChunkArray.Insert( packchunk, i );

		// Write chunk flag as unused.
		if ( mFile.SeekFromBegin( packchunk.mChunkOffset ) == _true )
		{
			_dword unused = 0;
			mFile.Write( &unused, sizeof( _dword ) );
		}

		// Try to combine with next chunk.
		if ( i < mUnusedChunkArray.Number( ) - 1 )
		{
			_dword nextchunkoffset	= mUnusedChunkArray[ i + 1 ].mChunkOffset;
			_dword nextchunksize	= mUnusedChunkArray[ i + 1 ].mChunkSize;

			// Can be combined.
			if ( packchunk.mChunkOffset + packchunk.mChunkSize + sizeof( _dword ) * 2 == nextchunkoffset )
			{
				// Increase chunk size.
				packchunk.mChunkSize += sizeof( _dword ) * 2 + nextchunksize;

				// Write new chunk size to file.
				if ( mFile.SeekFromBegin( packchunk.mChunkOffset + sizeof( _dword ) ) == _true )
					mFile.Write( &( packchunk.mChunkSize ), sizeof( _dword ) );

				// Update chunk array.
				mUnusedChunkArray[i].mChunkSize = packchunk.mChunkSize;
				mUnusedChunkArray.Remove( i + 1 );
			}
		}

		// Try to combine with previous chunk.
		if ( i > 0 )
		{
			_dword prevchunkoffset	= mUnusedChunkArray[ i - 1 ].mChunkOffset;
			_dword prevchunksize	= mUnusedChunkArray[ i - 1 ].mChunkSize;

			// Can be combined.
			if ( prevchunkoffset + prevchunksize + sizeof( _dword ) * 2 == packchunk.mChunkOffset )
			{
				// Increase chunk size.
				prevchunksize += sizeof( _dword ) * 2 + packchunk.mChunkSize;

				// Write new chunk size to file.
				if ( mFile.SeekFromBegin( prevchunkoffset + sizeof( _dword ) ) == _true )
					mFile.Write( &prevchunksize, sizeof( _dword ) );

				// Update chunk array.
				mUnusedChunkArray[ i - 1 ].mChunkSize = prevchunksize;
				mUnusedChunkArray.Remove( i );
			}
		}
	}

	return _true;
}

_dword PackArchive::ParseChunkBuffer( const _byte* chunkheader, _dword chunkoffset, _dword length )
{
	_dword* pointer = (_dword*) chunkheader;
	_dword chunksize = pointer[1];

	// If the chunk is not used.
	if ( pointer[0] != PackChunk::_FLAG_USED )
	{
		// Found an unused chunk.
		PackChunk packchunk;
		packchunk.mChunkOffset	= mBasePosition + chunkoffset;
		packchunk.mChunkSize	= chunksize;
		mUnusedChunkArray.Append( packchunk );
	}
	else
	{
		_char namebuffer[1024];

		// Read resource name length.
		pointer += 2;
		_dword namelength = ( pointer[0] + 1 ) * sizeof( _char );

		// Read resource name.
		pointer += 1;
		Memory::MemCpy( namebuffer, pointer, namelength );

		// Get file size/checksum.
		_dword* sizechecksum = (_dword*)( (_byte*) pointer + namelength );

		// Get file pointer offset.
		_dword fileoffset = chunkoffset + (_dword) ( ( (_byte*) pointer + namelength + sizeof( _dword ) * 5 ) - chunkheader );

		// Found a resource.
		CreateResourceHelper( namebuffer, chunkoffset, chunksize, fileoffset, sizechecksum[0], Oword( sizechecksum[1], sizechecksum[2], sizechecksum[3], sizechecksum[4] ) );
	}

	return chunksize + sizeof( _dword ) * 2;
}

_dword PackArchive::SearchResourceHelper( StringPtr resname )
{
	if ( mResourceArray.Number( ) == 0 )
		return -1;

	_char lowercasedname[1024];
	StringFormatter::CopyString( lowercasedname, resname );
	StringFormatter::LowercaseString( lowercasedname );

	return mResourceArray.SearchAscending( lowercasedname ).mIndex;
}

_bool PackArchive::SearchEmptyChunkHelper( _dword neededsize, _dword& chunkoffset, _dword& chunksize, _bool splitchunk )
{
	for ( _dword i = 0; i < mUnusedChunkArray.Number( ); i ++ )
	{
		if ( mUnusedChunkArray[i].mChunkSize < neededsize )
			continue;

		// Found empty chunk.
		chunkoffset = mUnusedChunkArray[i].mChunkOffset;
		chunksize	= mUnusedChunkArray[i].mChunkSize;

		// If the chunk is too larger than needed, we can split it.
		if ( splitchunk && mUnusedChunkArray[i].mChunkSize - neededsize >= 100 * 1024 )
		{
			chunksize = neededsize;

			mUnusedChunkArray[i].mChunkOffset += neededsize + sizeof( _dword ) * 2;
			mUnusedChunkArray[i].mChunkSize	  -= neededsize + sizeof( _dword ) * 2;

			_dword flagsize[2] = { 0, mUnusedChunkArray[i].mChunkSize };

			// Write chunk header for the new unused chunk.
			if ( mFile.SeekFromBegin( mUnusedChunkArray[i].mChunkOffset ) == _true )
				mFile.Write( flagsize, sizeof( flagsize ) );
		}
		// Use whole chunk, so remove it from unused chunk array.
		else
		{
			mUnusedChunkArray.Remove( i );
		}

		return _true;
	}

	return _false;
}

_void PackArchive::ProcessArchive( )
{
	_dword filesize = mFile.GetSize( ), chunkoffset = mFile.GetOffset( );

	// Create file mapping of the package file.
	_handle filemapping = ::CreateFileMapping( (_handle) mFile, _null, PAGE_READONLY, 0, filesize, _null );

	// Adjust file size.
	if ( mRangePosition != 0 )
		filesize = mRangePosition - mBasePosition;

	 _byte* filebuffer = _null;

	// Map whole file into virtual memory.
	if ( filemapping != _null )
		filebuffer = (_byte*) ::MapViewOfFile( filemapping, FILE_MAP_READ, 0, mBasePosition, mRangePosition - mBasePosition );

	_bool dictionaryexisting = _false; _dword dictionaryoffset = 0;

	// Read file using File Mapping API.
	if ( filebuffer != _null )
	{
		// Try to load file dictionary.
		if ( filesize > sizeof( _dword ) * 8 )
		{
			dictionaryoffset = ResourceManager::CheckResourceDictionary( filebuffer + filesize - sizeof( _dword ) * 4, filesize );

			if ( dictionaryoffset != 0 )
				dictionaryexisting = ResourceManager::LoadResourceDictionary( filebuffer + dictionaryoffset, filesize - dictionaryoffset, OnFoundPackResource, OnFoundPackChunk, (_dword) this );
		}

		// Cant find file dictionary, parse whole package instead.
		if ( dictionaryexisting == _false )
		{
			mUnusedChunkArray.Clear( );
			mResourceArray.Clear( );

			_dword length		= filesize - chunkoffset;
			_byte* chunkbuffer	= filebuffer + chunkoffset;

			while ( length > 0 )
			{
				_dword offset = ParseChunkBuffer( chunkbuffer, chunkoffset - mBasePosition, length );

				chunkbuffer	+= offset;
				chunkoffset += offset;

				if ( length > offset )
					length -= offset;
				else
					length = 0;
			}
		}

		::UnmapViewOfFile( filebuffer );
	}
	// Read file using File IO API.
	else
	{
		// Try to load file dictionary.
		if ( filesize > sizeof( _dword ) * 8 )
		{
			_byte hintbuffer[ sizeof( _dword ) * 4 ] = { 0 };

			mFile.SeekFromBegin( mBasePosition + filesize - sizeof( _dword ) * 4 );
			mFile.Read( hintbuffer, sizeof( _dword ) * 4 );

			dictionaryoffset = ResourceManager::CheckResourceDictionary( hintbuffer, filesize );

			if ( dictionaryoffset != 0 )
			{
				_byte* dictbuffer = new _byte[ filesize - dictionaryoffset ];

				mFile.SeekFromBegin( mBasePosition + dictionaryoffset );

				if ( mFile.Read( dictbuffer, filesize - dictionaryoffset ) )
					dictionaryexisting = ResourceManager::LoadResourceDictionary( dictbuffer, filesize - dictionaryoffset, OnFoundPackResource, OnFoundPackChunk, (_dword) this );

				delete[] dictbuffer;
			}
		}

		// Cant find file dictionary, parse whole package instead.
		if ( dictionaryexisting == _false )
		{
			mUnusedChunkArray.Clear( );
			mResourceArray.Clear( );

			// Skip package header.
			mFile.SeekFromBegin( chunkoffset );

			_byte* chunkheader = new byte[ 1024 * 1024 ];

			// Read all existing chunks.
			while ( _true )
			{
				// Get file pointer offset.
				_dword chunkoffset = mFile.GetOffset( ), length = 0;

				// Get chunk header from the offset.
				if ( mFile.Read( chunkheader, 1024 * 1024, &length ) == _false )
					break;

				if ( length == 0 )
					break;

				_byte* chunkbuffer = chunkheader;

				while ( length >= 1024 )
				{
					_dword offset = ParseChunkBuffer( chunkbuffer, chunkoffset - mBasePosition, length );

					chunkbuffer	+= offset;
					chunkoffset += offset;

					if ( length > offset )
						length -= offset;
					else
						length = 0;
				}

				// Skip chunk data.
				if ( mFile.SeekFromBegin( chunkoffset ) == _false )
					break;
			}

			delete[] chunkheader;
		}
	}

	if ( filemapping != _null )
		::CloseHandle( filemapping );

	// There is no usable resource in the package, just clear it.
	if ( mResourceArray.Number( ) == 0 )
	{
		ClearArchive( );
	}
	else
	{
		if ( dictionaryexisting )
			mDictionaryOffset = dictionaryoffset;
	}
}

_void PackArchive::ClearArchive( )
{
	mUnusedChunkArray.Clear( _true );
	mResourceArray.Clear( _true );

	if ( mBasePosition == 0 )
	{
		mFileSize = sizeof( _dword ) * 4;
		mFile.SetSize( mFileSize );
	}
}

_bool PackArchive::BuildResourceDictionary( MemFile& memfile ) const
{
	_dword totalsize = sizeof( _dword ) * mUnusedChunkArray.Number( ) * 3;

	// Calculate total size to be used 
	for ( _dword i = 0; i < mResourceArray.Number( ); i ++ )
		totalsize += mResourceArray[i].mDictSize;

	memfile.Create( totalsize );

	_byte* pointer = memfile.GetBuffer( );

	// Write each file resource.
	for ( _dword i = 0; i < mResourceArray.Number( ); i ++ )
	{
		const PackResource& packresource = mResourceArray[i];

		_dword header[9];
		header[0] = packresource.mDictSize - sizeof( header );
		header[1] = packresource.mChunkOffset - mBasePosition;
		header[2] = packresource.mChunkSize;
		header[3] = packresource.mResourceOffset - mBasePosition;
		header[4] = packresource.mResourceSize;
		header[5] = packresource.mResourceChecksum.Dword1( );
		header[6] = packresource.mResourceChecksum.Dword2( );
		header[7] = packresource.mResourceChecksum.Dword3( );
		header[8] = packresource.mResourceChecksum.Dword4( );

		// Copy header.
		Memory::MemCpy( pointer, header, sizeof( header ) );

		// Copy file name.
		Memory::MemCpy( pointer + sizeof( header ), (const _char*) packresource.mName, packresource.mDictSize - sizeof( header ) );

		pointer += packresource.mDictSize;
	}

	// Write each empty chunk.
	for ( _dword i = 0; i < mUnusedChunkArray.Number( ); i ++ )
	{
		const PackChunk& packchunk = mUnusedChunkArray[i];

		_dword header[3];
		header[0] = -1;
		header[1] = packchunk.mChunkOffset - mBasePosition;
		header[2] = packchunk.mChunkSize;

		// Copy header.
		Memory::MemCpy( pointer, header, sizeof( header ) );

		pointer += sizeof( header );
	}

	Compression::Compress( memfile );

	PS_TRACE1( L"[RM] Build Resource Dictionary OK : %s\r\n", (const _char*) mName )

	return _true;
}

_bool PackArchive::SaveResourceDictionary( )
{
	if ( mBasePosition != 0 )
		return _true;

	if ( mResourceArray.Number( ) == 0 )
	{
		ClearArchive( );

		return _true;
	}

	// There is file dictionary already.
	if ( mDictionaryOffset != 0 )
		return _true;

	MemFile memfile;
	if ( BuildResourceDictionary( memfile ) == _false )
		return _false;

	_dword chunkoffset = mFileSize;

	// Set resource dictionary label.
	if ( CreateResourceHelper( L"<<PSP_DICTIONARY>>", memfile.GetBuffer( ), memfile.GetLength( ), Oword( 0, 0, 0, 0 ), _false ) )
	{
		const _byte magiccode[] = { 'P', 'S', 'P', 'D', 'I', 'C', 'T', 0 };

		// Set magic code.
		mFile.Write( magiccode, sizeof( magiccode ) );

		// Set chunk flag.
		_dword chunkflag[2] = { chunkoffset, 0 };
		mFile.Write( chunkflag, sizeof( chunkflag ) );
	}

	return _false;
}

_bool PackArchive::CreateArchive( StringPtr archivename, StringPtr mountpath )
{
	mBasePosition = 0;

	mName = archivename;
	mName.Lowercase( );

	mPath = mountpath;
	mPath.Lowercase( );
	mPath.TrimBoth( L"\\/ " );
	mPath.ReplaceAll( '/', '\\' );

	if ( mPath == L"." )
		mPath = L"";

	// Set file attribute to normal.
	FileSystem::SetAttributes( archivename, FileSystem::_ATTRIBUTE_NORMAL );

	// Create package file.
	if ( mFile.Open( archivename, File::_CREATE_ALWAYS, File::_OPERATION_READ | File::_OPERATION_WRITE, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
		return _false;

	// Create another file handle, just for ansync read.
	if ( mAsyncReadFile.Open( archivename, File::_OPEN_EXISTING, File::_OPERATION_READ, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
		return _false;

	// Create another file handle, just for ansync write.
	if ( mAsyncWriteFile.Open( archivename, File::_OPEN_EXISTING, File::_OPERATION_WRITE, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
		return _false;

	// Write the magic token and version.
	_dword magictokenversion[2] = { _ARCHIVE_CODE, _ARCHIVE_VERSION };

	// Write file header.
	if ( mFile.Write( &magictokenversion, sizeof( magictokenversion ) ) == _false )
		return _false;

	// Write reserved data.
	_dword reserved[2] = { 0 };
	if ( mFile.Write( reserved, sizeof( reserved ) ) == _false )
		return _false;

	// Set package size.
	mFileSize = mFile.GetSize( );

	return _true;
}

_bool PackArchive::OpenArchive( StringPtr archivename, StringPtr mountpath, _dword baseposition, _dword rangeposition )
{
	mName = archivename;
	mName.Lowercase( );

	mPath = mountpath;
	mPath.Lowercase( );
	mPath.TrimBoth( L"\\/ " );
	mPath.ReplaceAll( '/', '\\' );

	if ( mPath == L"." )
		mPath = L"";

	// Set file attribute to normal.
	FileSystem::SetAttributes( archivename, FileSystem::_ATTRIBUTE_NORMAL );

	// Open package file for both read and write.
	if ( mFile.Open( archivename, File::_OPEN_EXISTING, File::_OPERATION_READ | File::_OPERATION_WRITE, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
	{
		// If failed, open only for read.
		if ( mFile.Open( archivename, File::_OPEN_EXISTING, File::_OPERATION_READ, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
			return _false;

		PS_TRACE1( L"[RM] Open pack archive with read only mode : %s\r\n", (const _char*) archivename )
	}
	else
	{
		// Create another file handle, just for ansync write.
		if ( mAsyncReadFile.Open( archivename, File::_OPEN_EXISTING, File::_OPERATION_READ, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
			return _false;

		// Create another file handle, just for ansync write.
		if ( mAsyncWriteFile.Open( archivename, File::_OPEN_EXISTING, File::_OPERATION_WRITE, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
			return _false;
	}

	mBasePosition	= baseposition;
	mRangePosition	= rangeposition;

	if ( mBasePosition != 0 )
		mFile.SeekFromBegin( mBasePosition );

	_dword magictokenversion[2];

	// Read file header.
	if ( mFile.Read( &magictokenversion, sizeof( magictokenversion ) ) == _false )
		return _false;

	// Check the magic token.
	if ( magictokenversion[0] != _ARCHIVE_CODE )
		return _false;

	// Check the package version.
	if ( magictokenversion[1] != _ARCHIVE_VERSION )
		return _false;

	// Read reserved data.
	_dword reserved[2] = { 0 };
	if ( mFile.Read( reserved, sizeof( reserved ) ) == _false )
		return _false;

	ProcessArchive( );

	// Set package size.
	mFileSize = mFile.GetSize( );

	return _true;
}

_void PackArchive::Release( )
{
	delete this;
}

StringPtr PackArchive::GetName( ) const
{
	return mName;
}

StringPtr PackArchive::GetPath( ) const
{
	return mPath;
}

_dword PackArchive::GetArchiveType( ) const
{
	return IResourceManager::_ARCHIVE_PACK;
}

_dword PackArchive::GetResourceNumber( ) const
{
	return mResourceArray.Number( );
}

IDataStream* PackArchive::OpenResource( StringPtr resname )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Search the resource with its name.
	_dword index = SearchResourceHelper( resname );
	if ( index == -1 )
		return _false;

	_char fullname[1024];

	if ( mPath == L"." || mPath == L"" )
		StringFormatter::CopyString( fullname, resname );
	else
		StringFormatter::FormatBuffer( fullname, 1024, L"%s\\%s", (const _char*) mPath, (const _char*) resname );

	return new PackStream( fullname, mResourceArray[ index ].mResourceChecksum, mFile, mAsyncReadFile, mAsyncWriteFile,
		mResourceArray[ index ].mResourceSize, mResourceArray[ index ].mResourceOffset );
}

_bool PackArchive::CreateResource( StringPtr resname, const _void* buffer, _dword size, const Oword* checksum, _dword* finish )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	String newresname = resname;

	// Search the resource with its name.
	_dword index = SearchResourceHelper( resname );
	if ( index != -1 )
	{
		if ( mResourceArray[ index ].mWithPrefix )
			newresname = mPath + L"\\" + newresname;

		DeleteResourceHelper( index );
	}

	MemFile tempbuffer;

	if ( buffer != _null && size != 0 )
	{
		tempbuffer.Open( (_byte*) buffer, size, _false );
		tempbuffer.EnableAutoDeleteBuffer( _false );
	}
	else
	{
		tempbuffer.Create( size );
		Memory::MemSet( tempbuffer.GetBuffer( ), 0, size );
	}

	Oword reschecksum = checksum != _null ? *checksum : Oword( 0, 0, 0, 0 );

	return CreateResourceHelper( newresname, tempbuffer.GetBuffer( ), tempbuffer.GetLength( ), reschecksum, _true, finish );
}

_bool PackArchive::DeleteResource( StringPtr resname )
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

_bool PackArchive::SearchResource( StringPtr resname, _dword* size, Oword* checksum )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Search the resource with its name.
	_dword index = SearchResourceHelper( resname );
	if ( index == -1 )
		return _false;

	if ( size != _null )
		*size = mResourceArray[ index ].mResourceSize;

	if ( checksum != _null )
		*checksum = mResourceArray[ index ].mResourceChecksum;

	return _true;
}

_dword PackArchive::EnumArchive( FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2, _float* percentage )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Set percentage to 0.0 at begin.
	if ( percentage != _null )
		*percentage = 0.0f;

	_dword resourcenumber = GetResourceNumber( );

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