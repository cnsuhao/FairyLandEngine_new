//============================================================================
// PackArchive.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// PackArchive
//----------------------------------------------------------------------------

class PackArchive : public IDataArchive
{
private:
	enum
	{
		_ARCHIVE_CODE		= 0x00505350,
		_ARCHIVE_VERSION	= 0x00000005,
	};

	struct PackChunk
	{
		enum { _FLAG_USED = 0x00445355 };

		_dword		mChunkOffset;
		_dword		mChunkSize;
	};

	struct PackResource
	{
		String		mName;
		_bool		mWithPrefix;
		_dword		mDictSize;
		_dword		mChunkOffset;
		_dword		mChunkSize;
		_dword		mResourceOffset;
		_dword		mResourceSize;
		Oword		mResourceChecksum;

		operator StringPtr ( ) const
			{ return mName; }
	};

	typedef Array< PackChunk >					ChunkArray;
	typedef Array< PackResource, StringPtr >	ResourceArray;

	File			mFile;
	File			mAsyncReadFile;
	File			mAsyncWriteFile;
	String			mName;
	String			mPath;

	_dword			mBasePosition;
	_dword			mRangePosition;
	_dword			mDictionaryOffset;
	_dword			mFileSize;

	ChunkArray		mUnusedChunkArray;
	ResourceArray	mResourceArray;

	static _void OnFoundPackResource( StringPtr resname, _dword chunkoffset, _dword chunksize, _dword resoffset, _dword ressize, const Oword& reschecksum, _dword userdata );
	static _void OnFoundPackChunk( _dword chunkoffset, _dword chunksize, _dword userdata );

	_bool	CreateResourceHelper( StringPtr resname, _dword chunkoffset, _dword chunksize, _dword resoffset, _dword ressize, const Oword& reschecksum, _bool fromdict = _false );
	_bool	CreateResourceHelper( StringPtr resname, const _void* buffer, _dword size, const Oword& checksum, _bool useemptychunk = _true, _dword* finish = _null );
	_bool	DeleteResourceHelper( _dword index );
	_dword	ParseChunkBuffer( const _byte* chunkheader, _dword chunkoffset, _dword length );
	_dword	SearchResourceHelper( StringPtr resname );
	_bool	SearchEmptyChunkHelper( _dword neededsize, _dword& chunkoffset, _dword& chunksize, _bool splitchunk );
	_void	ProcessArchive( );
	_void	ClearArchive( );

	_bool	ParseResourceDictionary( MemFile& memfile );
	_bool	BuildResourceDictionary( MemFile& memfile ) const;
	_bool	LoadResourceDictionary( const _byte* chunkbuffer, _dword length );
	_bool	SaveResourceDictionary( );

public:
	PackArchive( );
	~PackArchive( );

	_bool	CreateArchive( StringPtr archivename, StringPtr mountpath );
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