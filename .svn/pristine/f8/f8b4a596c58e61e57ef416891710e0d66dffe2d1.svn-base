//============================================================================
// HashArchive.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandStorage.h"

//----------------------------------------------------------------------------
// HashArchive Structure
//----------------------------------------------------------------------------

// Archive Header	: [MagicCode(4)] [Version(4)] [Reserved(16)]
// Table List 1		: [TableOffset(4)] * _TABLE_NUMBER
// Table List 2		: [TableOffset(4)] * _TABLE_NUMBER
// Chunk Header		: [MagicCode(4)] [ChunkSize(4)] [Reserved(8)]
// File Table		: Chunk Header + [PackItem(72)] * _TABLE_CAPACITY
// Resource			: Chunk Header + [ResourceName(N)] [ResourceData(N)]

//----------------------------------------------------------------------------
// HashArchive Implementation
//----------------------------------------------------------------------------

HashArchive::HashArchive( ) : mResourceHash( 1024 )
{
	mBasePosition		= 0;
	mRangePosition		= 0;
	mFileSize			= 0;

	mPackItemIndex		= 0;
	mPackItemNumber		= 0;
	mPackItemList		= _null;
}

HashArchive::~HashArchive( )
{
	delete[] mPackItemList;
}

_void HashArchive::CreateResourceHelper( PackItem* item, StringPtr resname )
{
	PackResource newresource;
	newresource.mPackItem	= item;
	newresource.mNameOffset	= item->mNameOffset;

	// We know the resource name.
	if ( resname != L"" )
	{
		newresource.mNameOffset		= -1;
		newresource.mResourceName	= resname;
	}

	// Put resource into string hash.
	mResourceHash.Insert( newresource, item->mHashCode1, item->mHashCode2, item->mHashCode3 );
}

_void HashArchive::DeleteResourceHelper( PackItem* item, PackItem* affectitem[], _bool cutfile )
{
	_dword affectnumber = 0;

	_dword chunkoffset	= item->mChunkOffset;
	_dword chunksize	= item->mChunkSize;

	_bool combined = _false;

	// Try to combine with other free chunk.
	for ( _dword i = 0; i < mFreeItemArray.Number( ); i ++ )
	{
		PackItem* freeitem = mFreeItemArray[i];

		// Combine at begin of current chunk.
		if ( freeitem->mChunkOffset == chunkoffset + chunksize )
		{
			freeitem->mChunkOffset = chunkoffset;
			freeitem->mChunkSize  += chunksize;

			// Put into affected item list.
			affectitem[ affectnumber ++ ] = freeitem;

			combined = _true;
			break;
		}

		// Combine at end of current chunk.
		if ( freeitem->mChunkOffset + freeitem->mChunkSize == chunkoffset )
		{
			freeitem->mChunkSize += chunksize;

			// Try to combine with next chunk too.
			if ( i < mFreeItemArray.Number( ) - 1 )
			{
				PackItem* nextitem = mFreeItemArray[i + 1];

				// We can combine, so increase chunk size, and also remove the next chunk.
				if ( freeitem->mChunkOffset + freeitem->mChunkSize == nextitem->mChunkOffset )
				{
					freeitem->mChunkSize += nextitem->mChunkSize;
					mFreeItemArray.Remove( i + 1 );

					// Init file item.
					Memory::MemSet( nextitem, 0, sizeof( PackItem ) );
					nextitem->mMagicCode = _CHUNK_NOT_CODE;

					// Put into affected item list.
					affectitem[ affectnumber ++ ] = nextitem;
				}
			}

			// Put into affected item list.
			affectitem[ affectnumber ++ ] = freeitem;

			combined = _true;
			break;
		}

		// Insert before current chunk.
		if ( freeitem->mChunkOffset > chunkoffset + chunksize )
		{
			mFreeItemArray.Insert( item, i );

			combined = _true;
			break;
		}
	}

	// Init file item.
	Memory::MemSet( item, 0, sizeof( PackItem ) );
	item->mMagicCode = _CHUNK_NOT_CODE;

	if ( combined == _false )
	{
		item->mChunkOffset	= chunkoffset;
		item->mChunkSize	= chunksize;

		mFreeItemArray.Append( item );
	}

	// Put into affected item list.
	affectitem[ affectnumber ++ ] = item;

	if ( cutfile )
	{
		// Get the last empty chunk.
		PackItem* lastitem = mFreeItemArray[ mFreeItemArray.Number( ) - 1 ];
		FL_ASSERT( lastitem->mChunkOffset + lastitem->mChunkSize <= mFileSize )

		// Cut file if the chunk is at file end.
		if ( lastitem->mChunkOffset + lastitem->mChunkSize == mFileSize )
		{
			// Remove the last chunk.
			mFreeItemArray.Remove( mFreeItemArray.Number( ) - 1 );

			// Change file size.
			mFileSize = lastitem->mChunkOffset;

			// Init file item.
			Memory::MemSet( item, 0, sizeof( PackItem ) );
			lastitem->mMagicCode = _CHUNK_NOT_CODE;

			// Put into affected item list.
			for ( _dword i = 0; i < affectnumber; i ++ )
			{
				if ( affectitem[i] == lastitem )
				{
					affectnumber = -1;
					break;
				}
			}

			if ( affectnumber != -1 )
				affectitem[ affectnumber ++ ] = lastitem;
		}
	}
}

HashArchive::PackItem* HashArchive::SearchFreeItem( _dword needsize )
{
	// Search in free item array.
	for ( _dword i = 0; i < mFreeItemArray.Number( ); i ++ )
	{
		if ( mFreeItemArray[i]->mChunkSize < needsize )
			continue;

		// Found an usable item.
		PackItem* freeitem = mFreeItemArray[i];

		// Remove it from free item array.
		mFreeItemArray.Remove( i );

		return freeitem;
	}

	if ( mPackItemList == _null )
		return _null;

	for ( _dword i = 0; i < mPackItemNumber; i ++ )
	{
		if ( mPackItemList[ mPackItemIndex ].mMagicCode != _CHUNK_USE_CODE )
			return &mPackItemList[ mPackItemIndex ];

		mPackItemIndex = ( mPackItemIndex + 1 ) % mPackItemNumber;
	}

	return _null;
}

_dword HashArchive::GetItemOffset( const PackItem* item )
{
	FL_ASSERT( item != _null )

	_dword offset = ( (_dword) item - (_dword) mPackItemList );

	FL_ASSERT( offset < sizeof( PackItem ) * mPackItemNumber )

	return offset + _ITEM_OFFSET;
}

_byte HashArchive::GetItemMask( const PackItem* item )
{
	FL_ASSERT( item != _null )

	_dword offset = ( (_dword) item - (_dword) mPackItemList );

	FL_ASSERT( offset < sizeof( PackItem ) * mPackItemNumber )

	return Dword( offset ).Byte1( ) + Dword( offset ).Byte2( ) + Dword( offset ).Byte3( ) + Dword( offset ).Byte4( ) + (_byte) _ARCHIVE_KEY;
}

_void HashArchive::LoadResourceName( PackResource& resource )
{
	// We already loaded resource name.
	if ( resource.mNameOffset == -1 )
		return;

	// The name is too long, should be some problem.
	if ( resource.mPackItem->mNameLength >= 1024 )
		return;

	// Move to name begin, read name into buffer.
	if ( mFile.SeekFromBegin( resource.mPackItem->mNameOffset ) == _false )
		return;

	_char buffer[1024];
	if ( mFile.Read( buffer, resource.mPackItem->mNameLength ) == _false )
		return;

	// Decrypt resource name with name key.
	Encryption::XORDecrypt( buffer, resource.mPackItem->mNameLength, (_byte) resource.mPackItem->mNameMask );

	// Set resource name.
	buffer[ resource.mPackItem->mNameLength / sizeof( _char ) ] = 0;
	resource.mResourceName = buffer;

	// Set name offset, means we already loaded resource name.
	resource.mNameOffset = -1;
}

_bool HashArchive::CreateArchive( StringPtr archivename, StringPtr mountpath, _dword maxitem )
{
	if ( maxitem < _MIN_ITEM_NUMBER )
		maxitem = _MIN_ITEM_NUMBER;
	if ( maxitem > _MAX_ITEM_NUMBER )
		maxitem = _MAX_ITEM_NUMBER;

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
	_dword reserved[7] = { 0 };
	if ( mFile.Write( reserved, sizeof( reserved ) ) == _false )
		return _false;

	// Write file item number.
	if ( mFile.Write( &maxitem, sizeof( maxitem ) ) == _false )
		return _false;

	mPackItemList = new PackItem[ maxitem ];
	Memory::MemSet( mPackItemList, 0, maxitem * sizeof( PackItem ) );

	// Set file item number.
	mPackItemNumber = maxitem;

	// Process all file items.
	for ( _dword i = 0; i < maxitem; i ++ )
	{
		PackItem* item = &( mPackItemList[i] );

		// Init file item.
		Memory::MemSet( item, 0, sizeof( PackItem ) );
		item->mMagicCode = _CHUNK_NOT_CODE;

		// Encrypt file item.
		Encryption::XOREncrypt( item, sizeof( PackItem ), GetItemMask( item ) );
	}

	// Write file items.
	if ( mFile.Write( mPackItemList, maxitem * sizeof( PackItem ) ) == _false )
		return _false;

	// Decrypt all file items.
	for ( _dword i = 0; i < maxitem; i ++ )
	{
		PackItem* item = &( mPackItemList[i] );

		// Decrypt file item.
		Encryption::XORDecrypt( item, sizeof( PackItem ), GetItemMask( item ) );
	}

	// Set package size.
	mFileSize = mFile.GetSize( );

	return _true;
}

_bool HashArchive::OpenArchive( StringPtr archivename, StringPtr mountpath, _dword baseposition, _dword rangeposition )
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

		PS_TRACE1( L"[RM] Open hash archive with read only mode : %s\r\n", (const _char*) archivename )
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
	_dword reserved[7] = { 0 };
	if ( mFile.Read( reserved, sizeof( reserved ) ) == _false )
		return _false;

	// Read file item number.
	_dword maxitem = 0;
	if ( mFile.Read( &maxitem, sizeof( maxitem ) ) == _false )
		return _false;

	if ( maxitem < _MIN_ITEM_NUMBER || maxitem > _MAX_ITEM_NUMBER )
		return _false;

	// Read all file items.
	mPackItemList = new PackItem[ maxitem ];

	// Read file items.
	if ( mFile.Read( mPackItemList, maxitem * sizeof( PackItem ) ) == _false )
		return _false;

	// Set file item number.
	mPackItemNumber = maxitem;

	// Get package size.
	mFileSize = mFile.GetSize( );

	PackItem* affectitem[64] = { 0 };

	// Process all file items.
	for ( _dword i = 0; i < maxitem; i ++ )
	{
		PackItem* item = &( mPackItemList[i] );

		// Decrypt file item.
		Encryption::XORDecrypt( item, sizeof( PackItem ), GetItemMask( item ) );

		// Found an using item.
		if ( item->mMagicCode == _CHUNK_USE_CODE )
		{
			CreateResourceHelper( item, L"" );
		}
		// Found an item point to empty chunk.
		else if ( item->mChunkOffset != 0 )
		{
			DeleteResourceHelper( item, affectitem, _false );
		}
	}

	return _true;
}

_void HashArchive::Release( )
{
	delete this;
}

StringPtr HashArchive::GetName( ) const
{
	return mName;
}

StringPtr HashArchive::GetPath( ) const
{
	return mPath;
}

_dword HashArchive::GetArchiveType( ) const
{
	return IResourceManager::_ARCHIVE_HASH;
}

_dword HashArchive::GetResourceNumber( ) const
{
	return mResourceHash.Number( );
}

IDataStream* HashArchive::OpenResource( StringPtr resname )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Search resource in hash table.
	PackResource* resource = mResourceHash.Index( resname );

	// Can not find any resource.
	if ( resource == _null )
		return _null;

	return new HashStream( resname, resource->mPackItem->mResourceChecksum, mFile, resource->mPackItem->mResourceSize, resource->mPackItem->mResourceOffset );
}

_bool HashArchive::CreateResource( StringPtr resname, const _void* buffer, _dword size, const Oword* checksum, _dword* finish )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// First we try to delete old resource.
	DeleteResource( resname );

	_dword namesize = resname.SizeOfBytes( ) - sizeof( _char );

	// Find a free item.
	PackItem* freeitem = SearchFreeItem( sizeof( ChunkHeader ) + namesize + size );

	// No free item for this resource, create failed.
	if ( freeitem == _null )
		return _false;

	// This is new item, so extend file size.
	if ( freeitem->mChunkOffset == 0 )
	{
		freeitem->mChunkOffset	= mFileSize;
		freeitem->mChunkSize	= sizeof( ChunkHeader ) + namesize + size;
	}

	// Random a key to encrypt resource name.
	_byte encrykey = (_byte) Math::Random( (_dword) 0x01, (_dword) 0xFF );

	// Prepare file item.
	freeitem->mMagicCode		= _CHUNK_USE_CODE;
	freeitem->mResourceOffset	= freeitem->mChunkOffset + sizeof( ChunkHeader ) + namesize;
	freeitem->mResourceSize		= size;
	freeitem->mResourceFlag		= Compression::IsCompressed( buffer, size );
	freeitem->mNameOffset		= freeitem->mChunkOffset + sizeof( ChunkHeader );
	freeitem->mNameLength		= namesize;
	freeitem->mNameMask			= encrykey;
	freeitem->mHashCode1		= resname.HashCode1( );
	freeitem->mHashCode2		= resname.HashCode2( );
	freeitem->mHashCode3		= resname.HashCode3( );
	freeitem->mReserved1		= 0;
	freeitem->mReserved2		= 0;
	freeitem->mResourceChecksum	= checksum != _null ? *checksum : Oword( );

	_byte chunkbuffer[2048];

	// Prepare chunk buffer, includes chunk header, and resource name.
	ChunkHeader* header = (ChunkHeader*) chunkbuffer;

	// Set chunk magic code.
	header->mMagicCode = _CHUNK_USE_CODE;
	// Set chunk size.
	header->mChunkSize = freeitem->mChunkSize;
	// Set chunk flag, reserved for now.
	header->mReserved1 = 0;
	// Set name size.
	header->mReserved2 = namesize;

	// Copy resource name.
	Memory::MemCpy( chunkbuffer + sizeof( ChunkHeader ), (const _char*) resname, namesize );

	// Encrypt resource name with random key.
	Encryption::XOREncrypt( chunkbuffer + sizeof( ChunkHeader ), namesize, encrykey );

	// Write chunk header.
	if ( mFile.SeekFromBegin( freeitem->mChunkOffset ) == _false || mFile.Write( chunkbuffer, sizeof( ChunkHeader ) + namesize ) == _false )
		return _false;

	// Write resource data.
	if ( mFile.Write( buffer, size ) == _false )
		return _false;

	// Get item offset.
	_dword itemoffset = GetItemOffset( freeitem );

	// Encrypt file item.
	PackItem encrypteditem = *freeitem;
	Encryption::XOREncrypt( &encrypteditem, sizeof( PackItem ), GetItemMask( freeitem ) );

	// Write file item.
	if ( mFile.SeekFromBegin( itemoffset ) == _false || mFile.Write( &encrypteditem, sizeof( PackItem ) ) == _false )
		return _false;

	CreateResourceHelper( freeitem, resname );

	// Set new file size.
	mFileSize = mFile.GetSize( );

	return _true;
}

_bool HashArchive::DeleteResource( StringPtr resname )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Search resource in hash table.
	ResourceHash::Iterator it = mResourceHash.GetIterator( resname );

	// Not found resource.
	if ( it.Valid( ) == _false )
		return _false;

	_dword oldfilesize = mFileSize;

	PackResource& resource = it;

	// Delete resource, and combine empty chunks.
	PackItem* affectitem[64] = { 0 };
	DeleteResourceHelper( resource.mPackItem, affectitem, _true );

	// Remove from resource hash table.
	mResourceHash.Remove( it );

	// Update file items.
	for ( _dword i = 0; i < 64; i ++ )
	{
		PackItem* item = affectitem[i];

		if ( item == _null )
			break;

		// Get item offset.
		_dword itemoffset = GetItemOffset( item );

		// Encrypt file item.
		PackItem encrypteditem = *item;
		Encryption::XOREncrypt( &encrypteditem, sizeof( PackItem ), GetItemMask( item ) );

		// Update file item.
		if ( mFile.SeekFromBegin( itemoffset ) == _false || mFile.Write( &encrypteditem, sizeof( PackItem ) ) == _false )
			return _false;
	}

	// Cut file if the chunk is at file end.
	if ( oldfilesize != mFileSize )
		mFile.SetSize( mFileSize );

	return _true;
}

_bool HashArchive::SearchResource( StringPtr resname, _dword* size, Oword* checksum )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Search resource in hash table.
	PackResource* resource = mResourceHash.Index( resname );

	// Can not find any resource.
	if ( resource == _null )
		return _false;

	if ( size != _null )
		*size = resource->mPackItem->mResourceSize;

	if ( checksum != _null )
		*checksum = resource->mPackItem->mResourceChecksum;

	return _true;
}

_dword HashArchive::EnumArchive( FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2, _float* percentage )
{
	ResourceManager* manager = (ResourceManager*) GetResourceManagerPointer( );
	FL_ASSERT( manager != _null )

	LockOwner lockowner( manager->GetResourceLock( ) );

	// Set percentage to 0.0 at begin.
	if ( percentage != _null )
		*percentage = 0.0f;

	_dword i = 0, resourcenumber = GetResourceNumber( );

	for ( ResourceHash::Iterator it = mResourceHash.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		PackResource& resource = it;

		// Load resource name if needed.
		LoadResourceName( resource );

		// Call found resource function.
		if ( funcpointer != _null )
			(*funcpointer)( resource.mResourceName, i + 1, parameter1, parameter2 );

		// Update percentage.
		if ( percentage != _null )
			*percentage = (_float) i / (_float) resourcenumber;

		i ++;
	}

	// Set percentage to 1.0 at end.
	if ( percentage != _null )
		*percentage = 1.0f;

	return resourcenumber;
}