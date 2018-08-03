//============================================================================
// HashArchive.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// HashArchive
//----------------------------------------------------------------------------

class HashArchive : public IDataArchive
{
private:
	enum
	{
		_ARCHIVE_CODE		= 0x00485350,
		_ARCHIVE_VERSION	= 0x00000001,
		_ARCHIVE_KEY		= 0xAC,

		_MAX_ITEM_NUMBER	= 65536,
		_MIN_ITEM_NUMBER	= 64,

		_ITEM_OFFSET		= 40,
	};

	enum
	{
		_CHUNK_USE_CODE		= 0x00455355,	// USE
		_CHUNK_NOT_CODE		= 0x00544F4E,	// NOT
	};

	struct ChunkHeader
	{
		_dword		mMagicCode;
		_dword		mChunkSize;
		_dword		mReserved1;
		_dword		mReserved2;

		ChunkHeader( )
			: mMagicCode( _CHUNK_USE_CODE ), mChunkSize( 0 ), mReserved1( 0 ), mReserved2( 0 ) { }
	};

	struct PackItem
	{
		_dword		mMagicCode;

		_dword		mChunkOffset;
		_dword		mChunkSize;

		_dword		mResourceOffset;
		_dword		mResourceSize;
		_dword		mResourceFlag;

		_dword		mNameOffset;
		_dword		mNameLength;
		_dword		mNameMask;

		_dword		mHashCode1;
		_dword		mHashCode2;
		_dword		mHashCode3;

		_dword		mReserved1;
		_dword		mReserved2;

		Oword		mResourceChecksum;
	};

	typedef Array< PackItem* > PackItemArray;

	struct PackResource
	{
		PackItem*	mPackItem;
		_dword		mNameOffset;
		String		mResourceName;
	};

	typedef StringHash< PackResource > ResourceHash;

	File			mFile;
	File			mAsyncReadFile;
	File			mAsyncWriteFile;
	String			mName;
	String			mPath;

	_dword			mBasePosition;
	_dword			mRangePosition;
	_dword			mFileSize;

	_dword			mPackItemIndex;
	_dword			mPackItemNumber;
	PackItem*		mPackItemList;
	PackItemArray	mFreeItemArray;

	ResourceHash	mResourceHash;

	_void		CreateResourceHelper( PackItem* item, StringPtr resname );
	_void		DeleteResourceHelper( PackItem* item, PackItem* affectitem[], _bool cutfile );
 
	PackItem*	SearchFreeItem( _dword needsize );
	_dword		GetItemOffset( const PackItem* item );
	_byte		GetItemMask( const PackItem* item );

	_void		LoadResourceName( PackResource& resource );

public:
	HashArchive( );
	~HashArchive( );

	_bool	CreateArchive( StringPtr archivename, StringPtr mountpath, _dword maxitem );
	_bool	OpenArchive( StringPtr archivename, StringPtr mountpath, _dword baseposition, _dword rangeposition );

	virtual _void			Release( );

	virtual StringPtr		GetName( ) const;
	virtual StringPtr		GetPath( ) const;

	virtual _dword			GetArchiveType( ) const;
	virtual _dword			GetResourceNumber( ) const;

	virtual IDataStream*	OpenResource( StringPtr resname );
	virtual _bool			CreateResource( StringPtr resname, const _void* buffer, _dword size, const Oword* checksum, _dword* finish );
	virtual _bool			DeleteResource( StringPtr resname );
	virtual _bool			SearchResource( StringPtr resname, _dword* size, Oword* checksum );

	virtual _dword			EnumArchive( FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2, _float* percentage );
};

};