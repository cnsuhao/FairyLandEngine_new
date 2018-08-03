//============================================================================
// FileSystem.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// FileSystem
//----------------------------------------------------------------------------

//! This class represents win32 file system.
class FileSystem
{
public:
	//! Define found a file callback function prototype.
	//! @param		filename		The name of the file (or folder) found.
	//! @param		filenumber		The number of file (or folder) found so far.
	//! @param		parameter1		User defined parameter.
	//! @param		parameter2		User defined parameter.
	//! @return		True indicates continue enumeration, or false indicates stop enumeration.
	typedef _bool (*OnFoundFile)( StringPtr filename, _dword filenumber, _void* parameter1, _void* parameter2 );

	//! Attribute of the file.
	enum _FILE_ATTRIBUTE
	{
		//! The file is read-only.
		_ATTRIBUTE_READONLY		= 0x00000001,
		//! The file is hidden.
		_ATTRIBUTE_HIDDEN		= 0x00000002,
		//! The file is part of, or is used exclusively by OS.
		_ATTRIBUTE_SYSTEM		= 0x00000004,
		//! The object is a directory.
		_ATTRIBUTE_DIRECTORY	= 0x00000010,
		//! The file is an archive file.
		_ATTRIBUTE_ARCHIVE		= 0x00000020,
		//! The file is a normal file, this attribute is valid only if used alone.
		_ATTRIBUTE_NORMAL		= 0x00000080,
		//! The file is temp file.
		_ATTRIBUTE_TEMPORARY	= 0x00000100,
	};

	static const _byte cCompressedFileHeader[16];

	static const _char* sFileNameAddPrefix;
	static const _char* sFileNameDelPrefix;

private:
	static _bool EnumFileHelper( StringPtr enumfilter, StringPtr relativedirectory, _dword& filenumber, OnFoundFile funcpointer, _void* parameter1, _void* parameter2 );
	static _bool EnumFolderHelper( StringPtr enumfilter, StringPtr relativedirectory, _dword& filenumber, OnFoundFile funcpointer, _void* parameter1, _void* parameter2, _bool enumfile, _bool recursive );

	struct BackgroundStruct
	{
		String	mAddress;
		String	mContent;
		_float*	mProgress;
	};

	static _bool BackgroundDownload( _void* parameter1, _void* parameter2 );
	static _bool BackgroundUpload( _void* parameter1, _void* parameter2 );
	static _void BackgroundThreadClose( _void* parameter1, _void* parameter2 );

public:
	//! Check if the specified file exists.
	//! @param		filename		The name of the file.
	//! @return		True if the file exists, false otherwise.
	static _bool DoesFileExist( StringPtr filename );

	//! Check if the specified directory exists.
	//! @param		directory		The name of the directory.
	//! @return		True if the directory exists, false otherwise.
	static _bool DoesDirExist( StringPtr filename );

	//! Check if the specified file is a file ( not a directory ).
	//! @param		filename		The name of the file.
	//! @return		True if is file, false otherwise.
	static _bool IsFile( StringPtr filename );
	//! Check if the specified file is a directory ( not a file ).
	//! @param		filename		The name of the file.
	//! @return		True if is file, false otherwise.
	static _bool IsDirectory( StringPtr filename );
	//! Check if a directory is empty.
	//! @param		directory		The name of the directory.
	//! @return		True if is empty, false otherwise.
	static _bool IsDirectoryEmpty( StringPtr directory );

	//! Retrieves the size of the specified file in bytes.
	//! @param		filename		The name of the file.
	//! @param		bytes			Size of file to get from this function.
	//! @return		True indicates success, false indicates failure.
	static _bool GetFileSize( StringPtr filename, _dword& bytes );
	//! Get attributes of a specified file.
	//! @param		filename		The name of the file.
	//! @param		attributes		Attributes of file to get from this function.
	//! @return		True indicates success, false indicates failure.
	static _bool GetAttributes( StringPtr filename, _dword& attributes );
	//! Set attributes of a specified file.
	//! @param		filename		The name of the file.
	//! @param		attributes		Attributes to set for the file.
	//! @return		True indicates success, false indicates failure.
	static _bool SetAttributes( StringPtr filename, _dword attributes );

	//! Get times of a specified file.
	//! @param		filename		The name of the file.
	//! @param		creation		The date and time the file was created.
	//! @param		lastaccess		The date and time the file was last accessed.
	//! @param		lastwrite		The date and time the file was last written to.
	//! @return		True indicates success, false indicates failure.
	static _bool GetTimes( StringPtr filename, Time* creation, Time* lastaccess, Time* lastwrite );
	//! Set times of a specified file.
	//! @param		filename		The name of the file.
	//! @param		creation		The date and time the file was created.
	//! @param		lastaccess		The date and time the file was last accessed.
	//! @param		lastwrite		The date and time the file was last written to.
	//! @return		True indicates success, false indicates failure.
	static _bool SetTimes( StringPtr filename, Time* creation, Time* lastaccess, Time* lastwrite );

	//! Create an empty file.
	//! @param		filename		The name of the file.
	//! @return		True indicates success, false indicates failure.
	//! @remarks	If the file already exists, it will not be truncated.
	static _bool Create( StringPtr filename );
	//! Create a file with specified buffer and size.
	//! @param		filename		The name of the file.
	//! @param		buffer			The buffer to be written into the file, it can be null, means only set file size.
	//! @param		size			The size of file.
	//! @return		True indicates success, false indicates failure.
	//! @remarks	If the file already exists, it will be overwritten.
	static _bool Create( StringPtr filename, const _void* buffer, _dword size );
	//! Open an existing file, and copy file data into a memory file.
	//! @param		filename		The name of the file.
	//! @param		memfile			Memory file used to store file data.
	//! @return		True indicates success, false indicates failure.
	static _bool Open( StringPtr filename, MemFile& memfile );
	//! Delete an existing file.
	//! @param		filename		The name of the file.
	//! @return		True indicates success, false indicates failure.
	static _bool Delete( StringPtr filename );
	//! Copy an existing file to a new one.
	//! @param		srcfilename		The name of the existing file.
	//! @param		desfilename		The name of the new file.
	//! @return		True indicates success, false indicates failure.
	static _bool Copy( StringPtr srcfilename, StringPtr desfilename );
	//! Renames an existing file or a directory.
	//! @param		srcfilename		The name of the existing file.
	//! @param		desfilename		The name of the new file.
	//! @return		True indicates success, false indicates failure.
	static _bool Move( StringPtr srcfilename, StringPtr desfilename );

	//! Creates a name for a temporary file.
	//! @param		tempfolder		Pointer to a null-terminated string that specifies the directory path for the file name.
	//! @param		prefix			Pointer to a null-terminated prefix string.
	//! @return		The temporary file name.
	static String BuildTempFileName( StringPtr tempfolder, StringPtr prefix );

	//! Get the current directory for the current process.
	//! @param		none
	//! @return		The path string of current directory.
	static String GetCurrentDir( );
	//! Set the current directory for the current process.
	//! @param		directory		The path name of new current directory.
	//! @return		True indicates success, false indicates failure.
	static _bool SetCurrentDir( StringPtr directory );

	//! Create a new directory.
	//! @param		directory		The path name of new directory.
	//! @return		True indicates success, false indicates failure.
	static _bool CreateDir( StringPtr directory );

	//! Delete a directory tree, includes all files and sub directories.
	//! @param		directory		The path name of directory to be deleted.
	//! @return		True indicates success, false indicates failure.
	static _bool DeleteDir( StringPtr directory );

	//! Copy a directory tree, includes all files and sub directories.
	//! @param		srcdirectory	The path name of directory to be copied.
	//! @param		desdirectory	The path name of directory to copy into.
	//! @return		True indicates success, false indicates failure.
	static _bool CopyDir( StringPtr srcdirectory, StringPtr desdirectory );

	//! Move a directory tree, includes all files and sub directories.
	//! @param		srcdirectory	The path name of directory to be moved.
	//! @param		desdirectory	The path name of directory to move into.
	//! @return		True indicates success, false indicates failure.
	static _bool MoveDir( StringPtr srcdirectory, StringPtr desdirectory );

	//! Trim a directory, delete any empty sub directories.
	//! @param		directory		The path name of directory to be trimed.
	//! @return		True if no files inside the directory, otherwise false.
	static _bool TrimDir( StringPtr directory );

	//! Get information about the amount of space on a disk volume.
	//! @param		directory		Pointer to a null-terminated string that specifies a directory on the specified disk.
	//! @param		total			Number of total bytes on a disk get from this function.
	//! @param		available		Number of free bytes on a disk get from this function.
	//! @return		True indicates success, false indicates failure.
	static _bool GetDiskSpace( StringPtr directory, _qword& total, _qword& available );

	//! Compare two files.
	//! @param		srcfilename		The name of the source file.
	//! @param		desfilename		The name of the target file.
	//! @return		True if two files are exactly same, otherwise false.
	static _bool CompareFile( StringPtr srcfilename, StringPtr desfilename );

	//! Compare two folders.
	//! @param		srcdirectory	The name of the source directory.
	//! @param		desdirectory	The name of the target directory.
	//! @param		deletesame		If delete same file in target directory.
	//! @return		True if two directories are exactly same, otherwise false.
	static _bool CompareDir( StringPtr srcdirectory, StringPtr desdirectory, _bool deletesame );

	//! Get the md5 checksum of a file.
	//! @param		filename		The name of the file.
	//! @param		checksum		The md5 checksum result of the file.
	//! @param		withfilename	If file name is part of checksum.
	//! @param		fullfiledata	if using full file to make checksum or just using part of it (3K from begin, middle, and end)
	//! @return		True indicates success, false indicates failure.
	static _bool GetFileChecksum( StringPtr filename, Oword& checksum, _bool withfilename = _true, _bool fullfiledata = _true );

	//! Get the md5 checksum of a file.
	//! @param		filename		The name of the file.
	//! @param		withfilename	The md5 checksum result of the file with file name.
	//! @param		withoutfilename	The md5 checksum result of the file without file name.
	//! @return		True indicates success, false indicates failure.
	static _bool GetFileChecksum( StringPtr filename, Oword& withfilename, Oword& withoutfilename );

	//! Get the md5 checksum of a file.
	//! @param		filename		The name of the file.
	//! @param		rename			Rename the file before make checksum.
	//! @param		checksum		The md5 checksum result of the file.
	//! @return		True indicates success, false indicates failure.
	static _bool GetFileChecksum( StringPtr filename, StringPtr rename, Oword& checksum );

	//! Enumerate each file in a directory, includes files in subdirectory, and call a user-defined callback function for each file.
	//! @param		rootfolder		The name of root directory.
	//! @param		recursive		Whether search in subdirectory.
	//! @param		filecallback	The function pointer to a user-defined callback function, can be null.
	//! @param		parameter1		User defined parameter.
	//! @param		parameter2		User defined parameter.
	//! @return		The number of file found.
	static _dword EnumFilesInDirectory( StringPtr rootfolder, _bool recursive, OnFoundFile funcpointer, _void* parameter1, _void* parameter2, StringPtr prefixfolder = L"" );

	//! Enumerate each folder in a directory, includes folders in subdirectory, and call a user-defined callback function for each file.
	//! @param		rootfolder		The name of root directory.
	//! @param		recursive		Whether search in subdirectory.
	//! @param		filecallback	The function pointer to a user-defined callback function, can be null.
	//! @param		parameter1		User defined parameter.
	//! @param		parameter2		User defined parameter.
	//! @return		The number of folder found.
	static _dword EnumFoldersInDirectory( StringPtr rootfolder, _bool recursive, OnFoundFile funcpointer, _void* parameter1, _void* parameter2, StringPtr prefixfolder = L"" );

	static _bool CheckDirectoryAccess( StringPtr directory );

	static _bool IsFileCompressed( StringPtr filename );
	static _bool CompressFile( StringPtr filename );
	static _bool DecompressFile( StringPtr filename );

	static _bool GetFileInfoAfterCompressed( StringPtr filename, _dword& size, Oword& checksum, _float compressratio );
	static _bool WritePEResToFile( _handle module, StringPtr resname, StringPtr filename );

	static _bool DownloadFile( StringPtr address, StringPtr localfilename, _float* progress = _null );
	static _bool UploadString( StringPtr address, StringPtr content );
	static _bool UploadFile( StringPtr address, StringPtr predata, StringPtr postdata, StringPtr localfilename );

	static _bool BackgroundDownloadFile( StringPtr address, StringPtr localfilename, _float* progress = _null );
	static _bool BackgroundUploadString( StringPtr address, StringPtr content );
};

//----------------------------------------------------------------------------
// DirectoryRecorder
//----------------------------------------------------------------------------

//! This class represents a directory recorder, used for reset current directory as an old one.
class DirectoryRecorder
{
private:
	//! The old directory, used for set back as current one.
	String	mOldDirectory;

public:
	//! Constructor, create a directory recorder, save the current directory path.
	//! @param		pathname		The path name of new current directory.
	DirectoryRecorder( StringPtr directory );
	//! Destructor, set back current directory.
	//! @param		none
	~DirectoryRecorder( );
};

};