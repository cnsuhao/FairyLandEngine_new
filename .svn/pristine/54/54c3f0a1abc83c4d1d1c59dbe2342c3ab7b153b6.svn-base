//============================================================================
// IResourceManager.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IResourceManager
//----------------------------------------------------------------------------

class IResourceManager
{
public:
	enum _ARCHIVE_TYPE
	{
		_ARCHIVE_FILE		= 0,
		_ARCHIVE_PACK		= 1,
		_ARCHIVE_HASH		= 2,
		_ARCHIVE_PERES		= 3,
		_ARCHIVE_MEM		= 4,
	};

	enum _ASYNC_RESULT
	{
		_ASYNC_PROCESSING	= 0,
		_ASYNC_DONE_TRUE	= 1,
		_ASYNC_DONE_FALSE	= 2,
	};

	typedef _bool (*OnEnumManifest)( StringPtr realname, StringPtr logicname, _dword size, const Oword& fullchecksum, const Oword& datachecksum, _dword flag, _bool needed, _void* parameter1, _void* parameter2 );

public:
	virtual _void			Release( ) = 0;

	virtual _void			Update( _dword elapse ) = 0;

	virtual _bool			CreateFileArchive( StringPtr archivename, StringPtr mountpath ) = 0;
	virtual _bool			CreatePackArchive( StringPtr archivename, StringPtr mountpath ) = 0;
	virtual _bool			CreateHashArchive( StringPtr archivename, StringPtr mountpath, _dword maxitem ) = 0;
	virtual _bool			CreatePEResArchive( StringPtr archivename, StringPtr mountpath ) = 0;
	virtual _bool			CreateMemArchive( StringPtr archivename, StringPtr mountpath ) = 0;

	virtual _bool			OpenFileArchive( StringPtr archivename, StringPtr mountpath ) = 0;
	virtual _bool			OpenPackArchive( StringPtr archivename, StringPtr mountpath, _dword baseposition = 0, _dword rangeposition = 0 ) = 0;
	virtual _bool			OpenHashArchive( StringPtr archivename, StringPtr mountpath, _dword baseposition = 0, _dword rangeposition = 0 ) = 0;
	virtual _bool			OpenPEResArchive( StringPtr archivename, StringPtr mountpath, _handle module = _null ) = 0;
	virtual _bool			OpenMemArchive( StringPtr archivename, StringPtr mountpath, _byte* buffer, _dword length ) = 0;

	virtual _bool			CheckFileArchive( StringPtr archivename, StringPtr mountpath ) = 0;
	virtual _bool			CheckPackArchive( StringPtr archivename, StringPtr mountpath ) = 0;
	virtual _bool			CheckHashArchive( StringPtr archivename, StringPtr mountpath ) = 0;
	virtual _bool			CheckPEResArchive( StringPtr archivename, StringPtr mountpath ) = 0;
	virtual _bool			CheckMemArchive( StringPtr archivename, StringPtr mountpath ) = 0;

	virtual _dword			GetResourceNumber( ) const = 0;
	virtual _dword			GetArchiveNumber( ) const = 0;
	virtual IDataArchive*	GetArchive( _dword archiveindex ) = 0;
	virtual IDataArchive*	SearchArchive( StringPtr archivename ) = 0;
	virtual _bool			CloseArchive( _dword archiveindex ) = 0;
	virtual _bool			CloseArchive( StringPtr archivename ) = 0;
	virtual _bool			CloseAllArchive( ) = 0;

	virtual IDataStream*	CreateMemStream( StringPtr resname, _dword size, const Oword& checksum ) = 0;
	virtual IDataStream*	OpenResource( StringPtr resname ) = 0;
	virtual _void			CloseResource( IDataStream*& datastream ) = 0;

	virtual _void			EnableRootArchive( _bool enable ) = 0;
	virtual _void			EnableDebugResource( _bool enable ) = 0;
	virtual _void			EnableDebugLibrary( _bool enable ) = 0;
	virtual _void			EnableDebugPDB( _bool enable, _bool multidebug = _true ) = 0;
	virtual Library*		CreateLibrary( StringPtr filename ) = 0;
	virtual _void			ReleaseLibrary( Library* library ) = 0;

	// These methods just for debug convenience
	virtual _void			InsertAdditionalDebugFile( StringPtr filename ) = 0;
	virtual _void			LoadAllLibraryDebugFile( ) = 0;
	virtual _void			FreeAllLibraryDebugFile( ) = 0;

	// Load a resource ( file ) in memory
	virtual _bool			LoadResource( StringPtr resname, MemFile& memfile, _bool decompress = _true ) = 0;
	// Save a resource ( file ) from memory
	virtual _bool			SaveResource( StringPtr resname, MemFile& memfile ) = 0;

	// Create a resource ( file ), it will created in corresponding archive.
	virtual _bool			CreateResource( StringPtr resname, const _void* buffer, _dword size, const Oword* checksum = _null ) = 0;
	virtual _bool			CreateResource( StringPtr resname, IDataStream* datastream, _dword* finish = _null ) = 0;
	virtual _bool			DeleteResource( StringPtr resname ) = 0;
	virtual _bool			SearchResource( StringPtr resname, _dword* size = _null, Oword* checksum = _null ) = 0;
	virtual _bool			CopyResource( StringPtr srcresname, StringPtr desresname, _bool* filechanged = _null, const Oword* checksum = _null ) = 0;
	virtual _bool			MoveResource( StringPtr srcresname, StringPtr desresname, _bool* filechanged = _null, const Oword* checksum = _null ) = 0;
	virtual _bool			CheckResource( StringPtr resname, const Oword& checksum, _bool deleteinvalidfile = _false ) = 0;
	virtual _bool			CompareResource( StringPtr srcresname, StringPtr desresname ) = 0;
	virtual _bool			CheckLocalResource( _bool onlyneededfile, _bool deleteinvalidfile, StringPtr skipfile = L"" ) = 0;

	virtual _void			ClearSearchPath( ) = 0;
	virtual _void			InsertSearchPath( StringPtr searchpath ) = 0;
	virtual _void			RemoveSearchPath( StringPtr searchpath ) = 0;
	virtual _dword			GetSearchPathNumber( ) const = 0;
	virtual StringPtr		GetSearchPath( _dword index ) const = 0;

	virtual _void			ClearFileAlias( ) = 0;
	virtual _void			InsertFileAlias( StringPtr filename, StringPtr filealias ) = 0;
	virtual _void			RemoveFileAlias( StringPtr filename ) = 0;
	virtual StringPtr		GetFileAlias( StringPtr filename ) const = 0;

	virtual _void			ClearNameVariable( ) = 0;
	virtual _void			InsertNameVariable( StringPtr name, StringPtr value ) = 0;
	virtual _void			RemoveNameVariable( StringPtr name ) = 0;
	virtual StringPtr		GetNameVariableValue( StringPtr name ) const = 0;
	virtual StringPtr		ParseNameVariable( StringPtr name, _char* buffer, _dword size ) const = 0;

	// Enum all resource in a archive.
	virtual _dword			EnumArchive( StringPtr archivename, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2, _float* percentage = _null ) = 0;
	virtual _bool			CopyArchive( StringPtr srcarchivename, StringPtr desarchivename, _float* percentage = _null ) = 0;
	virtual _bool			MoveArchive( StringPtr srcarchivename, StringPtr desarchivename, _float* percentage = _null ) = 0;

	virtual _bool			LoadManifest( StringPtr manifestname, _bool* binformat = _null, _bool merge = _false ) = 0;
	virtual _bool			LoadManifest( TxtFile& manifestfile, _bool merge = _false ) = 0;
	virtual _bool			LoadManifest( BinFile& manifestfile, _bool merge = _false ) = 0;
	virtual _bool			SaveManifest( StringPtr manifestname, _bool binformat ) = 0;
	virtual _bool			InsertManifest( StringPtr filename, _dword size, const Oword& fullchecksum, const Oword& datachecksum, _dword flag, _bool needed ) = 0;
	virtual _bool			RemoveManifest( StringPtr filename ) = 0;
	virtual _void			ClearManifest( ) = 0;

	virtual _dword			GetVersionNumber( ) const = 0;
	virtual StringPtr		GetVersionString( _dword index ) const = 0;

	virtual _dword			EnumManifest( OnEnumManifest funcpointer, _void* parameter1, _void* parameter2, _float* percentage = _null ) const = 0;

	virtual StringPtr		GetFirstFileInManifest( _dword* size, Oword* checksum ) = 0;
	virtual StringPtr		GetNextFileInManifest( _dword* size, Oword* checksum ) = 0;

	virtual StringPtr		GetManifestVersion( ) const = 0;
	virtual _void			SetManifestVersion( StringPtr version ) = 0;
	virtual StringPtr		GetManifestDate( ) const = 0;
	virtual _void			SetManifestDate( StringPtr date ) = 0;
	
	// Because a file maybe has different version, the logic path did not care the version, real path point to the correct version of the file.
	virtual StringPtr		GetLogicPathFromRealPath( StringPtr filename, _dword* size = _null, Oword* checksum = _null, _dword* flag = _null, _bool* needed = _null ) = 0;
	virtual StringPtr		GetRealPathFromLogicPath( StringPtr filename, _dword* size = _null, Oword* checksum = _null, _dword* flag = _null, _bool* needed = _null ) = 0;
	virtual StringPtr		GetFullPathFromDeepSearch( StringPtr filename ) = 0;
	virtual StringPtr		GetValidResourceName( StringPtr filename ) = 0;
	virtual StringPtr		GetValidResourceName( StringPtr filepath, StringPtr filename ) = 0;
	virtual _bool			GetResourceInformation( StringPtr filename, _dword& size, Oword& fullchecksum, Oword& datachecksum, _dword* offset = _null ) = 0;

	virtual _void			BuildFoldersInManifest( ) = 0;
	virtual _dword			EnumFilesInArchive( StringPtr filename, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2 ) = 0;
	virtual _dword			EnumFoldersInArchive( StringPtr rootpath, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2 ) = 0;
	virtual _dword			EnumFilesInManifest( StringPtr filename, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2 ) = 0;
	virtual _dword			EnumFoldersInManifest( StringPtr rootpath, FileSystem::OnFoundFile funcpointer, _void* parameter1, _void* parameter2 ) = 0;

	virtual _void			IncreaseIOControl( ) = 0;
	virtual _void			IncreaseIORead( _dword bytes ) = 0;
	virtual _void			IncreaseIOWrite( _dword bytes ) = 0;

	virtual _dword			GetRecentIOControlTime( _dword minutes ) const = 0;
	virtual _dword			GetRecentIOReadTime( _dword minutes ) const = 0;
	virtual _dword			GetRecentIOReadBytes( _dword minutes ) const = 0;
	virtual _dword			GetRecentIOWriteTime( _dword minutes ) const = 0;
	virtual _dword			GetRecentIOWriteBytes( _dword minutes ) const = 0;

	virtual _dword			GetTotalIOControlTime( ) const = 0;
	virtual _dword			GetTotalIOReadTime( ) const = 0;
	virtual _dword			GetTotalIOReadBytes( ) const = 0;
	virtual _dword			GetTotalIOWriteTime( ) const = 0;
	virtual _dword			GetTotalIOWriteBytes( ) const = 0;
};

};
