//============================================================================
// ResourceManager.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ResourceManager
//----------------------------------------------------------------------------

class ResourceManager : public IResourceManager
{
private:
	typedef _void (*OnFoundResource)( StringPtr resname, _dword chunkoffset, _dword chunksize, _dword resoffset, _dword ressize, const Oword& reschecksum, _dword userdata );
	typedef _void (*OnFoundChunk)( _dword chunkoffset, _dword chunksize, _dword userdata );

	enum _PORCESS_RESOURCE
	{
		_PROCESS_OPEN		= 1,
		_PROCESS_CREATE		= 2,
		_PROCESS_DELETE		= 3,
		_PROCESS_SEARCH		= 4,
	};

	struct CreateStruct
	{
		_void*		mBuffer;
		_dword		mSize;
		Oword*		mChecksum;
	};

	struct SearchStruct
	{
		_dword*		mSize;
		Oword*		mChecksum;
	};

	struct CheckStruct
	{
		StringPtr	mSkipFile;
		_bool		mOnlyNeededFile;
		_bool		mDeleteInvalidFile;
		_bool		mAllResourceReady;
	};

	struct FileDesc
	{
		// Logic file name.
		String		mLogicName;
		// Real file name with version.
		String		mRealName;
		// File size after compressed.
		_dword		mFileSize;
		// Compress flag.
		_dword		mCompressFlag;
		// File checksum, with full file name, before compressed.
		Oword		mFullChecksum;
		// Data checksum, only includes data in after compressed.
		Oword		mDataChecksum;
		// Is this file needed before enter game.
		_bool		mNeeded;
		// Is this file has been checked.
		_bool		mChecked;

		// For HTTP download, offset in the big file.
		_dword		mOffset;

		operator StringPtr ( ) const		
			{ return mLogicName; }
	};

	struct LibraryDesc
	{
		Library*	mLibrary;
		String		mFileName;
		String		mDllFileName;
		String		mPdbFileName;
		String		mTempFileName;
		_dword		mRefCount;
	};

	typedef Dictionary< FileDesc, StringPtr, 2 >	FileDictionary;

	typedef Array< String >							StringArray;
	typedef Tree< String >							StringTree;

	typedef Array< IDataArchive* >					ArchiveArray;
	typedef Array< LibraryDesc >					LibraryArray;

	typedef Pair< String, _dword >					SearchPathPair;
	typedef Array< SearchPathPair, StringPtr >		SearchPathArray;

	typedef Pair< String, String >					FileAliasPair;
	typedef Array< FileAliasPair, StringPtr >		FileAliasArray;

	typedef Pair< String, String >					NameVariablePair;
	typedef Array< NameVariablePair, StringPtr >	NameVariableArray;

	Lock						mResourceLock;

	FileDictionary				mFileDictionary;
	FileDictionary::Iterator	mEnumIterator;
	FileArchive*				mDefaultArchive;

	StringArray					mVersionArray;
	StringTree					mFolderTree;

	ArchiveArray				mArchiveArray;
	LibraryArray				mLibraryArray;
	SearchPathArray				mSearchPathArray;
	FileAliasArray				mFileAliasArray;
	NameVariableArray			mNameVariableArray;

	String						mLastSearchResult;
	String						mManifestVersion;
	String						mManifestDate;

	_char						mResourceName[1024];

	_dword						mTotalIOControlTime;
	_dword						mTotalIOReadTime;
	_dword						mTotalIOReadBytes;
	_dword						mTotalIOWriteTime;
	_dword						mTotalIOWriteBytes;

	_dword						mHappenSearchResource;
	_dword						mHappenIOControlTime;
	_dword						mHappenIOReadTime;
	_dword						mHappenIOWriteTime;

	AsyncIOManager*				mAsyncIOManager;

	_bool						mUseRootArchive;
	_bool						mDebugResource;
	_bool						mDebugLibrary;
	_bool						mDebugPDB;
	_bool						mMultiDebug;

	static _bool			OnCheckResource( StringPtr resname, _dword resnumber, _void* parameter1, _void* parameter2 );
	static _bool			OnCopyResource( StringPtr resname, _dword resnumber, _void* parameter1, _void* parameter2 );
	static _bool			OnMoveResource( StringPtr resname, _dword resnumber, _void* parameter1, _void* parameter2 );
	static _bool			OnCheckLocalResource( StringPtr realname, StringPtr logicname, _dword size, const Oword& checksum, const Oword& fastchecksum, _dword flag, _bool needed, _void* parameter1, _void* parameter2 );
	static _bool			CopyResourceHelper( IDataArchive* srcarchive, IDataArchive* desarchive, StringPtr srcresname, StringPtr desresname, _bool* filechanged, const Oword* checksum );
	static _bool			MoveResourceHelper( IDataArchive* srcarchive, IDataArchive* desarchive, StringPtr srcresname, StringPtr desresname, _bool* filechanged, const Oword* checksum );
    static _bool            InThePath(StringPtr fullpath, StringPtr path);

	_void					PrepareArchive( );
	_void					SearchResourceHelper( StringPtr resname, _dword process, _void* parameter, _dword* retval );
	_bool					ProcessResourceHelper( IDataArchive* dataarchive, StringPtr filename, _dword process, _void* parameter, _dword* retval );

	IDataArchive*			SearchArchiveByName( StringPtr archivename );
	IDataArchive*			SearchArchiveByResource( String& resname );
	IDataArchive*			SearchArchiveToCreateResource( String& resname );

public:
	ResourceManager( );
	~ResourceManager( );

	inline Lock&			GetResourceLock( );
	inline AsyncIOManager&	GetAsyncIOManager( );

	static _dword 			CheckResourceDictionary( const _byte* hintbuffer, _dword filesize );
	static _bool  			LoadResourceDictionary( const _byte* chunkbuffer, _dword length, OnFoundResource foundresourcefunc, OnFoundChunk foundchunkfunc, _dword userdata );
	static _bool  			ParseResourceDictionary( MemFile& memfile, OnFoundResource foundresourcefunc, OnFoundChunk foundchunkfunc, _dword userdata );

	virtual _void			Release( );

	virtual _void			Update( _dword elapse );

	virtual _bool			CreateFileArchive( StringPtr archivename, StringPtr mountpath );
	virtual _bool			CreatePackArchive( StringPtr archivename, StringPtr mountpath );
	virtual _bool			CreateHashArchive( StringPtr archivename, StringPtr mountpath, _dword maxitem );
	virtual _bool			CreatePEResArchive( StringPtr archivename, StringPtr mountpath );
	virtual _bool			CreateMemArchive( StringPtr archivename, StringPtr mountpath );

	virtual _bool			OpenFileArchive( StringPtr archivename, StringPtr mountpath );
	virtual _bool			OpenPackArchive( StringPtr archivename, StringPtr mountpath, _dword baseposition, _dword rangeposition );
	virtual _bool			OpenHashArchive( StringPtr archivename, StringPtr mountpath, _dword baseposition, _dword rangeposition );
	virtual	_bool			OpenPEResArchive( StringPtr archivename, StringPtr mountpath, _handle module );
	virtual _bool			OpenMemArchive( StringPtr archivename, StringPtr mountpath, _byte* buffer, _dword length );

	virtual _bool			CheckFileArchive( StringPtr archivename, StringPtr mountpath );
	virtual _bool			CheckPackArchive( StringPtr archivename, StringPtr mountpath );
	virtual _bool			CheckHashArchive( StringPtr archivename, StringPtr mountpath );
	virtual _bool			CheckPEResArchive( StringPtr archivename, StringPtr mountpath );
	virtual _bool			CheckMemArchive( StringPtr archivename, StringPtr mountpath );

	virtual _dword			GetResourceNumber( ) const;
	virtual _dword			GetArchiveNumber( ) const;
	virtual IDataArchive*	GetArchive( _dword archiveindex );
	virtual IDataArchive*	SearchArchive( StringPtr archivename );
	virtual _bool			CloseArchive( _dword archiveindex );
	virtual _bool			CloseArchive( StringPtr archivename );
	virtual _bool			CloseAllArchive( );

	virtual IDataStream*	CreateMemStream( StringPtr resname, _dword size, const Oword& checksum );
	virtual IDataStream*	OpenResource( StringPtr resname );
	virtual _void			CloseResource( IDataStream*& datastream );

	virtual _void			EnableRootArchive( _bool enable );
	virtual _void			EnableDebugResource( _bool enable );
	virtual _void			EnableDebugLibrary( _bool enable );
	virtual _void			EnableDebugPDB( _bool enable, _bool multidebug );
	virtual Library*		CreateLibrary( StringPtr filename );
	virtual _void			ReleaseLibrary( Library* library );

	virtual _void			InsertAdditionalDebugFile( StringPtr filename );
	virtual _void			LoadAllLibraryDebugFile( );
	virtual _void			FreeAllLibraryDebugFile( );

	virtual _bool			LoadResource( StringPtr resname, MemFile& memfile, _bool decompress = _true );
	virtual _bool			SaveResource( StringPtr resname, MemFile& memfile );

	virtual _bool			CreateResource( StringPtr resname, const _void* buffer, _dword size, const Oword* checksum );
	virtual _bool			CreateResource( StringPtr resname, IDataStream* datastream, _dword* finish );
	virtual _bool			DeleteResource( StringPtr resname );
	virtual _bool			SearchResource( StringPtr resname, _dword* size, Oword* checksum );
	virtual _bool			CopyResource( StringPtr srcresname, StringPtr desresname, _bool* filechanged, const Oword* checksum );
	virtual _bool			MoveResource( StringPtr srcresname, StringPtr desresname, _bool* filechanged, const Oword* checksum );
	virtual _bool			CheckResource( StringPtr resname, const Oword& checksum, _bool deleteinvalidfile );
	virtual _bool			CompareResource( StringPtr srcresname, StringPtr desresname );
	virtual _bool			CheckLocalResource( _bool onlyneededfile, _bool deleteinvalidfile, StringPtr skipfile );

	virtual _void			ClearSearchPath( );
	virtual _void			InsertSearchPath( StringPtr searchpath );
	virtual _void			RemoveSearchPath( StringPtr searchpath );
	virtual _dword			GetSearchPathNumber( ) const;
	virtual StringPtr		GetSearchPath( _dword index ) const;

	virtual _void			ClearFileAlias( );
	virtual _void			InsertFileAlias( StringPtr filename, StringPtr filealias );
	virtual _void			RemoveFileAlias( StringPtr filename );
	virtual StringPtr		GetFileAlias( StringPtr filename ) const;

	virtual _void			ClearNameVariable( );
	virtual _void			InsertNameVariable( StringPtr name, StringPtr value );
	virtual _void			RemoveNameVariable( StringPtr name );
	virtual StringPtr		GetNameVariableValue( StringPtr name ) const;
	virtual StringPtr		ParseNameVariable( StringPtr name, _char* buffer, _dword size ) const;

	virtual _dword			EnumArchive( StringPtr archivename, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2, _float* percentage );
	virtual _bool			CopyArchive( StringPtr srcarchivename, StringPtr desarchivename, _float* percentage );
	virtual _bool			MoveArchive( StringPtr srcarchivename, StringPtr desarchivename, _float* percentage );

	virtual _bool			LoadManifest( StringPtr manifestname, _bool* binformat, _bool merge );
	virtual _bool			LoadManifest( TxtFile& manifestfile, _bool merge );
	virtual _bool			LoadManifest( BinFile& manifestfile, _bool merge );
	virtual _bool			SaveManifest( StringPtr manifestname, _bool binformat );
	virtual _bool			InsertManifest( StringPtr filename, _dword size, const Oword& fullchecksum, const Oword& datachecksum, _dword flag, _bool needed );
	virtual _bool			RemoveManifest( StringPtr filename );
	virtual _void			ClearManifest( );

	virtual _dword			GetVersionNumber( ) const;
	virtual StringPtr		GetVersionString( _dword index ) const;

	virtual _dword			EnumManifest( OnEnumManifest funcpointer, _void* parameter1, _void* parameter2, _float* percentage ) const;

	virtual StringPtr		GetFirstFileInManifest( _dword* size, Oword* checksum );
	virtual StringPtr		GetNextFileInManifest( _dword* size, Oword* checksum );

	virtual StringPtr		GetManifestVersion( ) const;
	virtual _void			SetManifestVersion( StringPtr version );
	virtual StringPtr		GetManifestDate( ) const;
	virtual _void			SetManifestDate( StringPtr date );

	virtual StringPtr		GetLogicPathFromRealPath( StringPtr filename, _dword* size = _null, Oword* checksum = _null, _dword* flag = _null, _bool* needed = _null );
	virtual StringPtr		GetRealPathFromLogicPath( StringPtr filename, _dword* size = _null, Oword* checksum = _null, _dword* flag = _null, _bool* needed = _null );
	virtual StringPtr		GetFullPathFromDeepSearch( StringPtr filename );
	virtual StringPtr		GetValidResourceName( StringPtr filename );
	virtual StringPtr		GetValidResourceName( StringPtr filepath, StringPtr filename );
	virtual _bool			GetResourceInformation( StringPtr filename, _dword& size, Oword& fullchecksum, Oword& datachecksum, _dword* offset = _null );

	virtual _void			BuildFoldersInManifest( );
	virtual _dword			EnumFilesInArchive( StringPtr filename, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2 );
	virtual _dword			EnumFoldersInArchive( StringPtr rootpath, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2 );
	virtual _dword			EnumFilesInManifest( StringPtr filename, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2 );
	virtual _dword			EnumFoldersInManifest( StringPtr rootpath, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2 );

	virtual _void			IncreaseIOControl( );
	virtual _void			IncreaseIORead( _dword bytes );
	virtual _void			IncreaseIOWrite( _dword bytes );

	virtual _dword			GetRecentIOControlTime( _dword minutes ) const;
	virtual _dword			GetRecentIOReadTime( _dword minutes ) const;
	virtual _dword			GetRecentIOReadBytes( _dword minutes ) const;
	virtual _dword			GetRecentIOWriteTime( _dword minutes ) const;
	virtual _dword			GetRecentIOWriteBytes( _dword minutes ) const;

	virtual _dword			GetTotalIOControlTime( ) const;
	virtual _dword			GetTotalIOReadTime( ) const;
	virtual _dword			GetTotalIOReadBytes( ) const;
	virtual _dword			GetTotalIOWriteTime( ) const;
	virtual _dword			GetTotalIOWriteBytes( ) const;
};

//----------------------------------------------------------------------------
// ResourceManager Implementation
//----------------------------------------------------------------------------

Lock& ResourceManager::GetResourceLock( )
{
	return mResourceLock;
}

AsyncIOManager& ResourceManager::GetAsyncIOManager( )
{
	return *mAsyncIOManager;
}

};