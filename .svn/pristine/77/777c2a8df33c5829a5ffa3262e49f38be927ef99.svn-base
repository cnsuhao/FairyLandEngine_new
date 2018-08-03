//============================================================================
// FileSystem.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// FileSystem Implementation
//----------------------------------------------------------------------------

const _byte FileSystem::cCompressedFileHeader[16] =
	{ 0xF7, 0xF4, 0xE4, 0xC8, 0xCA, 0xD7, 0xD5, 0xC2, 0xD4, 0xD4, 0xC2, 0xC3, 0xE1, 0xCE, 0xCB, 0xC2 };

const _char* FileSystem::sFileNameAddPrefix = _null;
const _char* FileSystem::sFileNameDelPrefix = _null;

_bool FileSystem::EnumFileHelper( StringPtr enumfilter, StringPtr relativedirectory, _dword& filenumber, OnFoundFile funcpointer, _void* parameter1, _void* parameter2 )
{
	if ( enumfilter == L"" )
		return _false;

	_char filename[1024];

	FileFinder	filefinder;
	FileInfo	fileinfo;

	if ( filefinder.FindFirst( enumfilter, fileinfo ) == _true )
	{
		do
		{
			if ( fileinfo.mName == L"." || fileinfo.mName == L".." )
				continue;

			if ( relativedirectory == L"" )
				StringFormatter::FormatBuffer( filename, 1024, L"%s", (const _char*) fileinfo.mName );
			else
				StringFormatter::FormatBuffer( filename, 1024, L"%s\\%s", (const _char*) relativedirectory, (const _char*) fileinfo.mName );

			// Found a directory.
			if ( ( fileinfo.mAttributes & FileSystem::_ATTRIBUTE_DIRECTORY ) != 0 )
			{
				continue;
			}
			// Found a file.
			else
			{
				filenumber ++;

				if ( funcpointer != _null && (*funcpointer)( filename, filenumber, parameter1, parameter2 ) == _false )
					return _false;
			}
		}
		// Find next file.
		while ( filefinder.FindNext( fileinfo ) == _true );
	}

	return _true;
}

_bool FileSystem::EnumFolderHelper( StringPtr enumfilter, StringPtr relativedirectory, _dword& filenumber, OnFoundFile funcpointer, _void* parameter1, _void* parameter2, _bool enumfile, _bool recursive )
{
	if ( enumfilter == L"" )
		return _false;

	_char filename[1024], pathname[1024];

	_char prefix[1024];
	StringFormatter::GetPathName( prefix, 1024, enumfilter );

	_char postfix[1024];
	StringFormatter::GetFileName( postfix, 1024, enumfilter, _true, _false );

	FileFinder	filefinder;
	FileInfo	fileinfo;

	if ( enumfile )
	{
		EnumFileHelper( enumfilter, relativedirectory, filenumber, funcpointer, parameter1, parameter2 );

		StringFormatter::FormatBuffer( pathname, 1024, L"%s\\%s\\*.*", prefix, (const _char*) fileinfo.mName );
	}
	else
	{
		StringFormatter::FormatBuffer( pathname, 1024, L"%s", enumfilter );
	}

	if ( filefinder.FindFirst( pathname, fileinfo ) == _true )
	{
		do
		{
			if ( fileinfo.mName == L"." || fileinfo.mName == L".." )
				continue;

			StringFormatter::FormatBuffer( pathname, 1024, L"%s\\%s\\%s", prefix, (const _char*) fileinfo.mName, postfix );

			if ( relativedirectory == L"" )
				StringFormatter::FormatBuffer( filename, 1024, L"%s", (const _char*) fileinfo.mName );
			else
				StringFormatter::FormatBuffer( filename, 1024, L"%s\\%s", (const _char*) relativedirectory, (const _char*) fileinfo.mName );

			// Found a directory.
			if ( ( fileinfo.mAttributes & FileSystem::_ATTRIBUTE_DIRECTORY ) != 0 )
			{
				if ( enumfile == _false )
				{
					filenumber ++;

					if ( funcpointer != _null && (*funcpointer)( filename, filenumber, parameter1, parameter2 ) == _false )
						return _false;
				}

				if ( recursive )
				{
					if ( EnumFolderHelper( pathname, filename, filenumber, funcpointer, parameter1, parameter2, enumfile, recursive ) == _false )
						return _false;
				}
			}
		}
		// Find next file.
		while ( filefinder.FindNext( fileinfo ) == _true );
	}

	return _true;
}

_bool FileSystem::BackgroundDownload( _void* parameter1, _void* parameter2 )
{
	BackgroundStruct* bs = (BackgroundStruct*) parameter1;

	DownloadFile( bs->mAddress, bs->mContent, bs->mProgress );

	return _false;
}

_bool FileSystem::BackgroundUpload( _void* parameter1, _void* parameter2 )
{
	BackgroundStruct* bs = (BackgroundStruct*) parameter1;

	UploadString( bs->mAddress, bs->mContent );

	return _false;
}

_void FileSystem::BackgroundThreadClose( _void* parameter1, _void* parameter2 )
{
	BackgroundStruct* bs = (BackgroundStruct*) parameter1;

	delete bs;
}

_bool FileSystem::DoesFileExist( StringPtr filename )
{
	_dword attributes = ::GetFileAttributes( filename );

	return attributes != -1 && ( attributes & FILE_ATTRIBUTE_DIRECTORY ) == 0;
}

_bool FileSystem::DoesDirExist( StringPtr filename )
{
	_dword attributes = ::GetFileAttributes( filename );

	return attributes != -1 && ( attributes & FILE_ATTRIBUTE_DIRECTORY ) != 0;
}

_bool FileSystem::IsFile( StringPtr filename )
{
	_dword attributes = ::GetFileAttributes( filename );

	if ( attributes == -1 )
		return _false;

	return ( attributes & _ATTRIBUTE_DIRECTORY ) == 0;
}

_bool FileSystem::IsDirectory( StringPtr filename )
{
	_dword attributes = ::GetFileAttributes( filename );

	if ( attributes == -1 )
		return _false;

	return ( attributes & _ATTRIBUTE_DIRECTORY ) != 0;
}

_bool FileSystem::IsDirectoryEmpty( StringPtr directory )
{
	FileFinder finder; FileInfo fileinfo;
	finder.FindFirst( StringFormatter::FormatString( L"%s\\*.*", directory ), fileinfo );

	do
	{
		if ( fileinfo.mName != L"." && fileinfo.mName != L".." )
			return _false;
	}
	while ( finder.FindNext( fileinfo ) );

	return _true;
}

_bool FileSystem::GetFileSize( StringPtr filename, _dword& bytes )
{
	File file;
	if ( file.Open( filename, File::_OPEN_EXISTING, 0, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
		return _false;

	bytes = file.GetSize( );

	return bytes != -1;
}

_bool FileSystem::GetAttributes( StringPtr filename, _dword& attributes )
{
	attributes = ::GetFileAttributes( filename );

	return attributes != -1;
}

_bool FileSystem::SetAttributes( StringPtr filename, _dword attributes )
{
	return ::SetFileAttributes( filename, attributes );
}

_bool FileSystem::GetTimes( StringPtr filename, Time* creation, Time* lastaccess, Time* lastwrite )
{
	File file;
	if ( file.Open( filename, File::_OPEN_EXISTING, File::_OPERATION_READ, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
		return _false;

	FILETIME creationtime, lastaccesstime, lastwritetime, localcreationtime, locallastaccesstime, locallastwritetime;

	FILETIME* pct = creation   != _null ? &creationtime   : _null;
	FILETIME* pat = lastaccess != _null ? &lastaccesstime : _null;
	FILETIME* pwt = lastwrite  != _null ? &lastwritetime  : _null;

	// Get file time by win32 api.
	if ( ::GetFileTime( (_handle) file, pct, pat, pwt ) == _false )
		return _false;

	if ( pct != _null )
		::FileTimeToLocalFileTime( pct, &localcreationtime );

	if ( pat != _null )
		::FileTimeToLocalFileTime( pat, &locallastaccesstime );

	if ( pwt != _null )
		::FileTimeToLocalFileTime( pwt, &locallastwritetime );

	if ( creation != _null )
		::FileTimeToSystemTime( &localcreationtime, (SYSTEMTIME*) creation );

	if ( lastaccess != _null )
		::FileTimeToSystemTime( &locallastaccesstime, (SYSTEMTIME*) lastaccess );

	if ( lastwrite != _null )
		::FileTimeToSystemTime( &locallastwritetime, (SYSTEMTIME*) lastwrite );

	return _true;
}

_bool FileSystem::SetTimes( StringPtr filename, Time* creation, Time* lastaccess, Time* lastwrite )
{
	File file;
	if ( file.Open( filename, File::_OPEN_EXISTING, File::_OPERATION_WRITE, File::_SHARE_READ ) == _false )
		return _false;

	FILETIME creationtime, lastaccesstime, lastwritetime, localcreationtime, locallastaccesstime, locallastwritetime;

	FILETIME* pct = creation   != _null ? &creationtime   : _null;
	FILETIME* pat = lastaccess != _null ? &lastaccesstime : _null;
	FILETIME* pwt = lastwrite  != _null ? &lastwritetime  : _null;

	if ( creation != _null )
		::SystemTimeToFileTime( (SYSTEMTIME*) creation, &localcreationtime );

	if ( lastaccess != _null )
		::SystemTimeToFileTime( (SYSTEMTIME*) lastaccess, &locallastaccesstime );

	if ( lastwrite != _null )
		::SystemTimeToFileTime( (SYSTEMTIME*) lastwrite, &locallastwritetime );

	if ( pct != _null )
		::LocalFileTimeToFileTime( &localcreationtime, pct );

	if ( pat != _null )
		::LocalFileTimeToFileTime( &locallastaccesstime, pat );

	if ( pwt != _null )
		::LocalFileTimeToFileTime( &locallastwritetime, pwt );

	// Set file time by win32 api.
	if ( ::SetFileTime( (_handle) file, pct, pat, pwt ) == _false )
		return _false;

	return _true;
}

_bool FileSystem::Create( StringPtr filename )
{
	File file;
	return file.Open( filename, File::_OPEN_ALWAYS, File::_OPERATION_WRITE, File::_SHARE_READ );
}

_bool FileSystem::Create( StringPtr filename, const _void* buffer, _dword size )
{
	File file;
	if ( file.Open( filename, File::_CREATE_ALWAYS, File::_OPERATION_WRITE, File::_SHARE_READ ) == _false )
		return _false;

	if ( buffer == _null )
	{
		file.SetSize( size );
	}
	else
	{
		if ( file.Write( buffer, size ) == _false )
			return _false;
	}

	return _true;
}

_bool FileSystem::Open( StringPtr filename, MemFile& memfile )
{
	File file;
	if ( file.Open( filename, File::_OPEN_EXISTING, File::_OPERATION_READ, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
		return _false;

	_dword filesize = file.GetSize( );
	if ( filesize == -1 )
		return _false;

	if ( memfile.Create( filesize ) == _false )
		return _false;

	if ( file.Read( memfile.GetBuffer( ), filesize ) == _false )
		return _false;

	return _true;
}

_bool FileSystem::Delete( StringPtr filename )
{
	// First set file attribute to normal.
	SetAttributes( filename, _ATTRIBUTE_NORMAL );

	// Delete the file.
	return ::DeleteFile( filename );
}

_bool FileSystem::Copy( StringPtr srcfilename, StringPtr desfilename )
{
	if ( ::GetFileAttributes( srcfilename ) == -1 )
		return _false;

	// Create directory if needed.
	CreateDir( StringFormatter::GetPathName( desfilename ) );

	return ::CopyFile( srcfilename, desfilename, _false );
}

_bool FileSystem::Move( StringPtr srcfilename, StringPtr desfilename )
{
	if ( ::GetFileAttributes( srcfilename ) == -1 )
		return _false;

	// Create directory if needed.
	CreateDir( StringFormatter::GetPathName( desfilename ) );

	return ::MoveFileEx( srcfilename, desfilename, MOVEFILE_REPLACE_EXISTING );
}

String FileSystem::BuildTempFileName( StringPtr tempfolder, StringPtr prefix )
{
	_char buffer[1024];
	if ( ::GetTempFileName( tempfolder, prefix, 0, buffer ) == 0 )
		return String( L"" );

	String filename = buffer;
	filename.TrimLeft( '\\' );

	if ( filename == L"" )
		filename.Format( L"PSTEMP%.3d.tmp", Math::Random( 1000 ) );

	return filename;
}

String FileSystem::GetCurrentDir( )
{
	_char buffer[1024];
	::GetCurrentDirectory( 1024, buffer );

	return String( buffer );
}

_bool FileSystem::SetCurrentDir( StringPtr directory )
{
	// Create directory as needed.
	CreateDir( directory );

	return ::SetCurrentDirectory( directory );
}

_bool FileSystem::CreateDir( StringPtr directory )
{
	if ( directory == L"" )
		return _false;

	if ( DoesDirExist( directory ) == _true )
		return _true;

	if ( directory[ directory.Length( ) - 1 ] == ':' )
		return _true;

	// Auto create middle path.
	for ( _dword i = 0; directory[i] != 0; i ++ )
	{
		if ( directory[i] != '\\' && directory[i] != '/' )
			continue;

		if ( i == 0 )
			continue;

		if ( directory[ i - 1 ] == ':' )
			continue;

		String middlepath( directory, i );

		if ( ::GetFileAttributes( middlepath ) != -1 )
			continue;

		if ( ::CreateDirectory( middlepath, _null ) == _false )
			return _false;
	}

	return ::CreateDirectory( directory, _null );
}

_bool FileSystem::DeleteDir( StringPtr directory )
{
	if ( directory == L"" )
		return _false;

	_char filename[1024];
	StringFormatter::FormatBuffer( filename, 1024, L"%s\\*.*", (const _char*) directory );

	FileFinder	filefinder;
	FileInfo	fileinfo;

	if ( filefinder.FindFirst( filename, fileinfo ) == _true )
	{
		do
		{
			if ( fileinfo.mName == L"." || fileinfo.mName == L".." )
				continue;

			StringFormatter::FormatBuffer( filename, 1024, L"%s\\%s", (const _char*) directory, (const _char*) fileinfo.mName );

			// Found a sub directory.
			if ( ( fileinfo.mAttributes & _ATTRIBUTE_DIRECTORY ) != 0 )
			{
				// Delete sub directory recursively.
				DeleteDir( filename );

				// Delete sub directory itself.
				::SetFileAttributes( filename, _ATTRIBUTE_NORMAL | _ATTRIBUTE_DIRECTORY );
				::RemoveDirectory( filename );
			}
			// Found a file.
			else
			{
				// Delete a file.
				::SetFileAttributes( filename, _ATTRIBUTE_NORMAL );
				::DeleteFile( filename );
			}
		}
		// Find next file.
		while ( filefinder.FindNext( fileinfo ) == _true );
	}

	filefinder.Close( );

	// Delete root directory.
	::SetFileAttributes( directory, _ATTRIBUTE_NORMAL | _ATTRIBUTE_DIRECTORY );
	::RemoveDirectory( directory );

	return _true;
}

_bool FileSystem::CopyDir( StringPtr srcdirectory, StringPtr desdirectory )
{
	if ( srcdirectory == L"" || desdirectory == L"" )
		return _false;

	_char srcfilename[1024], desfilename[1024];
	StringFormatter::FormatBuffer( srcfilename, 1024, L"%s\\*.*", (const _char*) srcdirectory );

	FileFinder	filefinder;
	FileInfo	fileinfo;

	if ( filefinder.FindFirst( srcfilename, fileinfo ) == _true )
	{
		do
		{
			if ( fileinfo.mName == L"." || fileinfo.mName == L".." )
				continue;

			StringFormatter::FormatBuffer( srcfilename, 1024, L"%s\\%s", (const _char*) srcdirectory, (const _char*) fileinfo.mName );
			StringFormatter::FormatBuffer( desfilename, 1024, L"%s\\%s", (const _char*) desdirectory, (const _char*) fileinfo.mName );

			// Found a sub directory.
			if ( ( fileinfo.mAttributes & _ATTRIBUTE_DIRECTORY ) != 0 )
			{
				// Copy sub directory itself.
				CreateDir( desfilename );

				// Copy sub directory recursively.
				CopyDir( srcfilename, desfilename );
			}
			// Found a file.
			else
			{
				// Copy a file.
				Copy( srcfilename, desfilename );
			}
		}
		// Find next file.
		while ( filefinder.FindNext( fileinfo ) == _true );
	}

	filefinder.Close( );

	return _true;
}

_bool FileSystem::MoveDir( StringPtr srcdirectory, StringPtr desdirectory )
{
	if ( srcdirectory == L"" || desdirectory == L"" )
		return _false;

	_char srcfilename[1024], desfilename[1024];
	StringFormatter::FormatBuffer( srcfilename, 1024, L"%s\\*.*", (const _char*) srcdirectory );

	FileFinder	filefinder;
	FileInfo	fileinfo;

	if ( filefinder.FindFirst( srcfilename, fileinfo ) == _true )
	{
		do
		{
			if ( fileinfo.mName == L"." || fileinfo.mName == L".." )
				continue;

			StringFormatter::FormatBuffer( srcfilename, 1024, L"%s\\%s", (const _char*) srcdirectory, (const _char*) fileinfo.mName );
			StringFormatter::FormatBuffer( desfilename, 1024, L"%s\\%s", (const _char*) desdirectory, (const _char*) fileinfo.mName );

			// Found a sub directory.
			if ( ( fileinfo.mAttributes & _ATTRIBUTE_DIRECTORY ) != 0 )
			{
				// Copy sub directory itself.
				CreateDir( desfilename );

				// Move sub directory recursively.
				MoveDir( srcfilename, desfilename );

				// Delete sub directory itself.
				::SetFileAttributes( srcfilename, _ATTRIBUTE_NORMAL | _ATTRIBUTE_DIRECTORY );
				::RemoveDirectory( srcfilename );
			}
			// Found a file.
			else
			{
				// Move a file.
				Move( srcfilename, desfilename );
			}
		}
		// Find next file.
		while ( filefinder.FindNext( fileinfo ) == _true );
	}

	filefinder.Close( );

	// Delete root directory.
	::SetFileAttributes( srcdirectory, _ATTRIBUTE_NORMAL | _ATTRIBUTE_DIRECTORY );
	::RemoveDirectory( srcdirectory );

	return _true;
}

_bool FileSystem::TrimDir( StringPtr directory )
{
	if ( directory == L"" )
		return _false;

	_char filename[1024];
	StringFormatter::FormatBuffer( filename, 1024, L"%s\\*.*", (const _char*) directory );

	FileFinder	filefinder;
	FileInfo	fileinfo;

	_bool nofile = _true;

	if ( filefinder.FindFirst( filename, fileinfo ) == _true )
	{
		do
		{
			if ( fileinfo.mName == L"." || fileinfo.mName == L".." )
				continue;

			StringFormatter::FormatBuffer( filename, 1024, L"%s\\%s", (const _char*) directory, (const _char*) fileinfo.mName );

			// Found a sub directory.
			if ( ( fileinfo.mAttributes & _ATTRIBUTE_DIRECTORY ) != 0 )
			{
				// Trim sub directory recursively.
				if ( TrimDir( filename ) == _true )
				{
					// Delete sub directory itself.
					::SetFileAttributes( filename, _ATTRIBUTE_NORMAL | _ATTRIBUTE_DIRECTORY );
					::RemoveDirectory( filename );
				}
				else
				{
					nofile = _false;
				}
			}
			// Found a file.
			else
			{
				nofile = _false;
			}
		}
		// Find next file.
		while ( filefinder.FindNext( fileinfo ) == _true );
	}

	return nofile;
}

_bool FileSystem::GetDiskSpace( StringPtr directory, _qword& total, _qword& available )
{
	_bool autocreate = _false;

	if ( DoesFileExist( directory ) == _false && DoesDirExist( directory ) == _false )
	{
		CreateDir( directory );

		autocreate = _true;
	}

	_bool retval = ::GetDiskFreeSpaceEx( directory, (ULARGE_INTEGER*) &available, (ULARGE_INTEGER*) &total, _null );

	if ( autocreate == _true )
	{
		DeleteDir( directory );
	}

	return retval;
}

_bool FileSystem::CompareFile( StringPtr srcfilename, StringPtr desfilename )
{
	File srcfile, desfile;

	// Open source file.
	if ( srcfile.Open( srcfilename, File::_OPEN_EXISTING, File::_OPERATION_READ, File::_SHARE_READ ) == _false )
		return _false;

	// Open target file.
	if ( desfile.Open( desfilename, File::_OPEN_EXISTING, File::_OPERATION_READ, File::_SHARE_READ ) == _false )
		return _false;

	if ( srcfile.GetSize( ) != desfile.GetSize( ) )
		return _false;

	_byte buffer1[65536], buffer2[65536];

	_dword length1 = 0, length2 = 0;

	while ( _true )
	{
		if ( srcfile.Read( buffer1, 65536, &length1 ) == _false )
			return _false;

		if ( desfile.Read( buffer2, 65536, &length2 ) == _false )
			return _false;

		if ( length1 != length2 )
			return _false;

		if ( length1 == 0 )
			break;

		// Compare data read from files.
		if ( Memory::MemCmp( buffer1, buffer2, length1 ) == _false )
			return _false;
	}

	return _true;
}

_bool FileSystem::CompareDir( StringPtr srcdirectory, StringPtr desdirectory, _bool deletesame )
{
	if ( srcdirectory == L"" || desdirectory == L"" )
		return _false;

	_bool compareresult = _true;

	_char srcfilename[1024], desfilename[1024];
	StringFormatter::FormatBuffer( srcfilename, 1024, L"%s\\*.*", (const _char*) srcdirectory );

	FileFinder	filefinder;
	FileInfo	fileinfo;

	if ( filefinder.FindFirst( srcfilename, fileinfo ) == _true )
	{
		do
		{
			if ( fileinfo.mName == L"." || fileinfo.mName == L".." )
				continue;

			StringFormatter::FormatBuffer( srcfilename, 1024, L"%s\\%s", (const _char*) srcdirectory, (const _char*) fileinfo.mName );
			StringFormatter::FormatBuffer( desfilename, 1024, L"%s\\%s", (const _char*) desdirectory, (const _char*) fileinfo.mName );

			// Found a sub directory.
			if ( ( fileinfo.mAttributes & _ATTRIBUTE_DIRECTORY ) != 0 )
			{
				// Compare sub directory recursively.
				if ( CompareDir( srcfilename, desfilename, deletesame ) == _false )
					compareresult = _false;

				if ( deletesame == _false && compareresult == _false )
					return _false;
			}
			// Found a file.
			else
			{
				// Compare files.
				if ( CompareFile( srcfilename, desfilename ) == _false )
				{
					compareresult = _false;
				}
				else if ( deletesame )
				{
					Delete( desfilename );
				}

				if ( deletesame == _false && compareresult == _false )
					return _false;
			}
		}
		// Find next file.
		while ( filefinder.FindNext( fileinfo ) == _true );
	}

	return compareresult;
}

_bool FileSystem::GetFileChecksum( StringPtr filename, Oword& checksum, _bool withfilename, _bool fullfiledata )
{
	File file;

	// Open file to read some bytes to compute file check sum.
	if ( file.Open( filename, File::_OPEN_EXISTING, File::_OPERATION_READ, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
		return _false;

	// Remove file name prefix.
	if ( sFileNameDelPrefix != _null )
	{
		if ( String( filename ).Lowercase( ).SearchL2R( String( sFileNameDelPrefix ).Lowercase( ) ) == 0 )
			filename = (const _char*) filename + StringPtr( sFileNameDelPrefix ).Length( );
	}

	_char tempbuffer[1024];

	// Add file name prefix.
	if ( sFileNameAddPrefix != _null )
	{
		StringFormatter::CopyString( tempbuffer, sFileNameAddPrefix );
		StringFormatter::CopyString( tempbuffer + StringPtr( sFileNameAddPrefix ).Length( ), filename );

		filename = tempbuffer;
	}

	_dword filesize = file.GetSize( ), namelength = filename.SizeOfBytes( );

	// Using part of file data.
	if ( fullfiledata == _false )
	{
		// Prepare buffer to read data from file.
		_byte buffer[4096]; _dword length = 0;

		if ( withfilename == _true )
		{
			length = Math::Min( filename.SizeOfBytes( ), (_dword) 1024 );

			// Copy lowercased file name into this buffer.
			Memory::MemCpy( buffer, (const _char*) String( filename ).Lowercase( ), length );
		}

		// Read some bytes from front of the file.
		_dword frontbytes = 1024;
		if ( file.Read( buffer + length, frontbytes, &frontbytes ) == _true )
			length += frontbytes;

		file.SeekFromBegin( (_long) filesize / 2 );

		// Read some bytes from middle of the file.
		_dword middlebytes = 1024;
		if ( file.Read( buffer + length, middlebytes, &middlebytes ) == _true )
			length += middlebytes;

		file.SeekFromEnd( -1024 );

		// Read some bytes from back of the file.
		_dword backbytes = 1024;
		if ( file.Read( buffer + length, backbytes, &backbytes ) == _true )
			length += backbytes;

		FL_ASSERT( length <= 4096 )

		Encryption::MD5Checksum( buffer, length, checksum );
	}
	// Using full file data.
	else
	{
		_dword buffersize = filesize;

		if ( withfilename == _true )
			buffersize += namelength;

		if ( buffersize == 0 )
			return _false;

		_byte* buffer = new _byte[ buffersize ];

		if ( buffer == _null )
			return _false;

		// Copy lowercased file name into this buffer.
		if ( withfilename == _true )
			StringFormatter::CopyString( (_char*) buffer, String( filename ).Lowercase( ) );
		else
			namelength = 0;

		// Read file data into buffer.
		if ( file.Read( buffer + namelength, filesize ) == _false )
		{
			delete[] buffer;
			return _false;
		}

		Encryption::MD5Checksum( buffer, buffersize, checksum );

		delete[] buffer;
	}

	return _true;
}

_bool FileSystem::GetFileChecksum( StringPtr filename, Oword& withfilename, Oword& withoutfilename )
{
	File file;

	// Open file to read some bytes to compute file check sum.
	if ( file.Open( filename, File::_OPEN_EXISTING, File::_OPERATION_READ, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
		return _false;

	_dword filesize = file.GetSize( ), namelength = filename.SizeOfBytes( ), buffersize = filesize + namelength;

	if ( buffersize == 0 )
		return _false;

	_byte* buffer = new _byte[ buffersize ];

	if ( buffer == _null )
		return _false;

	// Copy lowercased file name into this buffer.
	StringFormatter::CopyString( (_char*) buffer, String( filename ).Lowercase( ) );

	// Read file data into buffer.
	if ( file.Read( buffer + namelength, filesize ) == _false )
	{
		delete[] buffer;
		return _false;
	}

	Encryption::MD5Checksum( buffer, buffersize, withfilename );
	Encryption::MD5Checksum( buffer + namelength, filesize, withoutfilename );

	delete[] buffer;

	return _true;
}

_bool FileSystem::GetFileChecksum( StringPtr filename, StringPtr rename, Oword& checksum )
{
	File file;

	// Open file to read some bytes to compute file check sum.
	if ( file.Open( filename, File::_OPEN_EXISTING, File::_OPERATION_READ, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
		return _false;

	_dword filesize = file.GetSize( ), namelength = rename.SizeOfBytes( ), buffersize = filesize + namelength;

	if ( buffersize == 0 )
		return _false;

	_byte* buffer = new _byte[ buffersize ];

	if ( buffer == _null )
		return _false;

	// Copy lowercased file name into this buffer.
	StringFormatter::CopyString( (_char*) buffer, String( rename ).Lowercase( ) );

	// Read file data into buffer.
	if ( file.Read( buffer + namelength, filesize ) == _false )
	{
		delete[] buffer;
		return _false;
	}

	Encryption::MD5Checksum( buffer, buffersize, checksum );

	delete[] buffer;

	return _true;
}

_dword FileSystem::EnumFilesInDirectory( StringPtr rootfolder, _bool recursive, OnFoundFile funcpointer, _void* parameter1, _void* parameter2, StringPtr prefixfolder )
{
	String folder = rootfolder;
	folder.TrimRight( '\\' );
	folder.TrimRight( '/' );

	String prefix = prefixfolder;
	prefix.TrimRight( '\\' );
	prefix.TrimRight( '/' );

	_dword filenumber = 0; _char enumfilter[1024];

	if ( folder.SearchL2R( '*' ) == -1 && folder.SearchL2R( '?' ) == -1 && DoesFileExist( rootfolder ) == _false )
		StringFormatter::FormatBuffer( enumfilter, 1024, L"%s\\*.*", (const _char*) folder );
	else
		StringFormatter::FormatBuffer( enumfilter, 1024, L"%s", (const _char*) folder );

	if ( recursive )
	{
		EnumFolderHelper( enumfilter, prefix, filenumber, funcpointer, parameter1, parameter2, _true, _true );
	}
	else
	{
		EnumFileHelper( enumfilter, prefix, filenumber, funcpointer, parameter1, parameter2 );
	}

	return filenumber;
}

_dword FileSystem::EnumFoldersInDirectory( StringPtr rootfolder, _bool recursive, OnFoundFile funcpointer, _void* parameter1, _void* parameter2, StringPtr prefixfolder )
{
	String folder = rootfolder;
	folder.TrimRight( '\\' );
	folder.TrimRight( '/' );

	String prefix = prefixfolder;
	prefix.TrimRight( '\\' );
	prefix.TrimRight( '/' );

	_dword foldernumber = 0; _char enumfilter[1024];

	if ( folder.SearchL2R( '*' ) == -1 && folder.SearchL2R( '?' ) == -1 )
		StringFormatter::FormatBuffer( enumfilter, 1024, L"%s\\*.*", (const _char*) folder );
	else
		StringFormatter::FormatBuffer( enumfilter, 1024, L"%s", (const _char*) folder );

	EnumFolderHelper( enumfilter, prefix, foldernumber, funcpointer, parameter1, parameter2, _false, recursive );

	return foldernumber;
}

_bool FileSystem::CheckDirectoryAccess( StringPtr directory )
{
	_bool deldir = FileSystem::IsDirectory( directory ) == _false;

	String temp = L"temp";

	temp += Dword( Math::Random( ) + 1 ).ToString( );
	temp += L"_";
	temp += Dword( System::GetCurrentProcessID( ) ).ToString( );

	String file = directory;
	file += L"\\";
	file += temp;

	if ( FileSystem::Create( file, temp, temp.SizeOfBytes( ) ) == _false )
		return _false;

	MemFile load;
	if ( FileSystem::Open( file, load ) == _false )
		return _false;

	FileSystem::Delete( file );

	if ( deldir )
		FileSystem::DeleteDir( directory );

	if ( load.GetLength( ) != temp.SizeOfBytes( ) )
		return _false;

	return Memory::MemCmp( temp.GetString( ), load.GetBuffer( ), load.GetLength( ) );
}

_bool FileSystem::IsFileCompressed( StringPtr filename )
{
	File srcfile;

	// Open source file.
	if ( srcfile.Open( filename, File::_OPEN_EXISTING, File::_OPERATION_READ, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
		return _false;

	if ( srcfile.GetSize( ) < 20 )
		return _false;

	_byte buffer[16];

	// Read header from file.
	if ( srcfile.Read( buffer, 16 ) == _false )
		return _false;

	// Compare header.
	return Memory::MemCmp( cCompressedFileHeader, buffer, 16 );
}

_bool FileSystem::CompressFile( StringPtr filename )
{
	if ( IsFileCompressed( filename ) == _true )
		return _true;

	String tempfilename = StringFormatter::FormatString( L"%s.tmp", (const _char*) filename );

	File srcfile, desfile;

	// Open source file.
	if ( srcfile.Open( filename, File::_OPEN_EXISTING, File::_OPERATION_READ, File::_SHARE_READ | File::_SHARE_WRITE ) == _false )
		return _false;

	_dword filesize = srcfile.GetSize( );

	if ( filesize == -1 )
		return _false;

	// Open temporary file.
	if ( desfile.Open( tempfilename, File::_CREATE_ALWAYS, File::_OPERATION_WRITE ) == _false )
		return _false;

	// Write compressed file header.
	if ( desfile.Write( cCompressedFileHeader, 16 ) == _false )
		return _false;

	// Write original file size.
	if ( desfile.Write( &filesize, sizeof( _dword ) ) == _false )
		return _false;

	CompressionStream stream;
	if ( Compression::BeginCompress( stream ) == _false )
		return _false;

	_byte srcbuffer[65536]; _dword srclength = 0;
	_byte desbuffer[65536]; _dword deslength = 0;

	// Compress until end of file.
	do
	{
		if ( srcfile.Read( srcbuffer, 65536, &srclength ) == _false )
		{
			Compression::FinishCompress( stream );
			return _false;
		}

		FL_ASSERT( filesize >= srclength )

		filesize -= srclength;

		Compression::PrepareCompress( stream, srcbuffer, srclength );

		do
		{
			deslength = 65536;
			Compression::ProcessCompress( stream, desbuffer, deslength,
				filesize == 0 ? Compression::_FLUSH_FINISH : Compression::_FLUSH_NO );

			if ( desfile.Write( desbuffer, deslength ) == _false )
			{
				Compression::FinishCompress( stream );
				return _false;
			}
		}
		while ( deslength == 65536 );
	}
	while ( filesize != 0 );

	// Clean up stream.
	Compression::FinishCompress( stream );

	srcfile.Close( );
	desfile.Close( );

	FileSystem::Delete( filename );
	FileSystem::Move( tempfilename, filename );

	return _true;
}

_bool FileSystem::DecompressFile( StringPtr filename )
{
	if ( IsFileCompressed( filename ) == _false )
		return _true;

	String tempfilename = StringFormatter::FormatString( L"%s.tmp", (const _char*) filename );

	File srcfile, desfile;

	// Open source file.
	if ( srcfile.Open( filename, File::_OPEN_EXISTING, File::_OPERATION_READ ) == _false )
		return _false;

	_dword filesize = srcfile.GetSize( );

	if ( filesize == -1 || filesize < 20 )
		return _false;

	// Skip file header and original size.
	srcfile.SeekFromBegin( 20 );
	filesize -= 20;

	// Open temporary file.
	if ( desfile.Open( tempfilename, File::_CREATE_ALWAYS, File::_OPERATION_WRITE ) == _false )
		return _false;

	CompressionStream stream;
	if ( Compression::BeginDecompress( stream ) == _false )
		return _false;

	_byte srcbuffer[65536]; _dword srclength = 0;
	_byte desbuffer[65536]; _dword deslength = 0;

	_bool finish = _false;

	// Decompress until end of file.
	do
	{
		if ( srcfile.Read( srcbuffer, 65536, &srclength ) == _false )
		{
			Compression::FinishDecompress( stream );
			return _false;
		}

		if ( srclength == 0 )
			break;

		Compression::PrepareDecompress( stream, srcbuffer, srclength );

		do
		{
			deslength = 65536;
			finish = Compression::ProcessDecompress( stream, desbuffer, deslength );

			if ( desfile.Write( desbuffer,deslength ) == _false )
			{
				Compression::FinishDecompress( stream );
				return _false;
			}
		}
		while ( deslength == 65536 );
	}
	while ( finish == _false );

	// Clean up stream.
	Compression::FinishDecompress( stream );

	srcfile.Close( );
	desfile.Close( );

	FileSystem::Delete( filename );
	FileSystem::Move( tempfilename, filename );

	return _true;
}

_bool FileSystem::GetFileInfoAfterCompressed( StringPtr filename, _dword& size, Oword& checksum, _float compressratio )
{
	if ( IsFileCompressed( filename ) == _true )
	{
		GetFileSize( filename, size );
		GetFileChecksum( filename, checksum );

		return _true;
	}

	// Open this file.
	MemFile memfile;
	FileSystem::Open( filename, memfile );

	// Get original file size.
	size = memfile.GetLength( );

	String ext = StringFormatter::GetExtention( filename ).Lowercase( );

	_bool cancompress = ext != L"ogg" && ext != L"wav" && ext != L"mp3";

	// Try to compress this file, if failed, means we cant compress it in this ratio.
	if ( cancompress == _false || Compression::Compress( memfile, compressratio ) == _false )
	{
		Encryption::MD5Checksum( memfile.GetBuffer( ), memfile.GetLength( ), checksum );

		return _false;
	}
	else
	{
		// We can compress it, get file size after compress.
		size = memfile.GetLength( );

		Encryption::MD5Checksum( memfile.GetBuffer( ), memfile.GetLength( ), checksum );

		return _true;
	}
}

_bool FileSystem::WritePEResToFile( _handle module, StringPtr resname, StringPtr filename )
{
	// Find resource by name & type.
	_handle resinfo = ::FindResource( (HMODULE) module, resname, L"FAIRYLANDRES" );
	if ( resinfo == _null )
		return _false;

	// Get the size of the resource.
	_dword size = ::SizeofResource( (HMODULE) module, (HRSRC) resinfo );
	if ( size == 0 )
		return _false;

	// Load the resource into memory.
	_handle resource = ::LoadResource( (HMODULE) module, (HRSRC) resinfo );
	if ( resource == _null )
		return _false;

	return Create( filename, resource, size );
}

_bool FileSystem::DownloadFile( StringPtr address, StringPtr localfilename, _float* progress )
{
	if ( progress != _null )
		*progress = 0.0f;

	// Attempts to make a connection to the internet.
	if ( ::InternetAttemptConnect( 0 ) != 0 )
		return _false;

	// Open an internet handle.
	_handle internet = ::InternetOpen( L"Mozilla/4.0 (compatible; )", INTERNET_OPEN_TYPE_PRECONFIG, _null, _null, 0 );
	if ( internet == _null )
		return _false;

	String lowercaseaddress( address );
	lowercaseaddress.Lowercase( );

	// Remove protocol name.
	lowercaseaddress.Replace( L"http://", L"" );

	_dword index = lowercaseaddress.SearchL2R( '/' );

	String hostname, filename; 

	// Split host name and file name.
	if ( index != -1 )
	{
		hostname = String( (const _char*) lowercaseaddress, index );
		filename = String( (const _char*) lowercaseaddress + index + 1 );
	}
	else
	{
		hostname = lowercaseaddress;
	}

	// Connect to the address, open a session.
	_handle session = ::InternetConnect( internet, hostname, INTERNET_DEFAULT_HTTP_PORT, L"", L"", INTERNET_SERVICE_HTTP, 0, _null );
	if ( session == _null )
	{
		::InternetCloseHandle( internet );
		return _false;
	}

	const _char* accepttype[] = { L"*/*", _null };

	// Creates a HTTP GET request handle.
	_handle request = ::HttpOpenRequest( session, L"GET", filename, L"HTTP/1.1", L"", accepttype, INTERNET_FLAG_KEEP_CONNECTION, _null );
	if ( request == _null )
	{
		::InternetCloseHandle( session );
		::InternetCloseHandle( internet );
		return _false;
	}

	const _char* requestheader = L"Accept: */*\r\n";

	// Add request headers.
	if ( ::HttpAddRequestHeaders( request, requestheader, -1, HTTP_ADDREQ_FLAG_ADD ) == 0 )
	{
		::InternetCloseHandle( request );
		::InternetCloseHandle( session );
		::InternetCloseHandle( internet );
		return _false;
	}

	// Send Request.
	if ( ::HttpSendRequest( request, _null, 0, _null, 0 ) == 0 )
	{
		::InternetCloseHandle( request );
		::InternetCloseHandle( session );
		::InternetCloseHandle( internet );
		return _false;
	}

	File localfile;

	if ( localfile.Open( localfilename, File::_CREATE_ALWAYS, File::_OPERATION_WRITE ) == _false )
	{
		::InternetCloseHandle( request );
		::InternetCloseHandle( session );
		::InternetCloseHandle( internet );
		return _false;
	}

	_dword totalsize = -1, dlsize = 0;
	if ( progress != _null )
	{
		_char size[1024]; _dword length = 1024, index = 0;
		if ( ::HttpQueryInfo( request, HTTP_QUERY_CONTENT_LENGTH, size, &length, &index ) )
		{
			if ( length < 1024 )
				size[ length ] = 0;

			totalsize = Long( size );
		}
	}

	_byte buffer[4096]; _dword bytes = 0;

	// Read response from connection.
	while ( ::InternetReadFile( request, buffer, 4096, &bytes ) != 0 )
	{
		if ( bytes == 0 )
			break;

		if ( localfile.Write( buffer, bytes ) == _false )
			break;

		if ( totalsize != -1 )
		{
			dlsize += bytes;

			*progress = (_float) dlsize / (_float) totalsize;

			if ( *progress == 1.0f )
				*progress = 0.99f;
		}
	}

	::InternetCloseHandle( request );
	::InternetCloseHandle( session );
	::InternetCloseHandle( internet );

	localfile.Close( );

	// Decompress the file.
	if ( FileSystem::IsFileCompressed( localfilename ) == _true )
		FileSystem::DecompressFile( localfilename );

	if ( progress != _null )
		*progress = 1.0f;

	return _true;
}

_bool FileSystem::UploadString( StringPtr address, StringPtr content )
{
	// Attempts to make a connection to the internet.
	if ( ::InternetAttemptConnect( 0 ) != 0 )
		return _false;

	// Open an internet handle.
	_handle internet = ::InternetOpen( L"Mozilla/4.0 (compatible; )", INTERNET_OPEN_TYPE_PRECONFIG, _null, _null, 0 );
	if ( internet == _null )
		return _false;

	// Remove protocol name.
	if ( address.SearchL2R( L"http://" ) != -1 )
		address = StringPtr( (const _char*) address + 7 );

	_dword index = address.SearchL2R( '/' );

	_char hostname[2048];
	_char filename[2048];

	// Split host name and file name.
	if ( index != -1 )
	{
		StringFormatter::CopyString( hostname, address, index );
		StringFormatter::CopyString( filename, (const _char*) address + index + 1 );
	}
	else
	{
		StringFormatter::CopyString( hostname, address );
	}

	// Connect to the address, open a session.
	_handle session = ::InternetConnect( internet, hostname, INTERNET_DEFAULT_HTTP_PORT, L"", L"", INTERNET_SERVICE_HTTP, 0, _null );
	if ( session == _null )
	{
		::InternetCloseHandle( internet );
		return _false;
	}

	const _char* accepttype[] = { L"*/*", _null };

	// Creates a HTTP GET request handle.
	_handle request = ::HttpOpenRequest( session, L"POST", filename, L"HTTP/1.1", L"", accepttype, INTERNET_FLAG_KEEP_CONNECTION, _null );
	if ( request == _null )
	{
		::InternetCloseHandle( session );
		::InternetCloseHandle( internet );
		return _false;
	}

	const _char* requestheader = L"Accept: */*\r\n";

	// Add request headers.
	if ( ::HttpAddRequestHeaders( request, requestheader, -1, HTTP_ADDREQ_FLAG_ADD ) == 0 )
	{
		::InternetCloseHandle( request );
		::InternetCloseHandle( session );
		::InternetCloseHandle( internet );
		return _false;
	}

	const _char* contenttype = L"Content-Type: application/x-www-form-urlencoded";

	_chara ansicontent[4096];
	StringFormatter::UnicodeToAnsi( ansicontent, 4096, content );

	// Send Request.
	_bool retval = ::HttpSendRequest( request, contenttype, StringPtr( contenttype ).Length( ), ansicontent, content.Length( ) ) == 0;

	::InternetCloseHandle( request );
	::InternetCloseHandle( session );
	::InternetCloseHandle( internet );

	return retval;
}

_bool FileSystem::UploadFile( StringPtr address, StringPtr predata, StringPtr postdata, StringPtr localfilename )
{
	// Attempts to make a connection to the internet.
	if ( ::InternetAttemptConnect( 0 ) != 0 )
		return _false;

	// Open an internet handle.
	_handle internet = ::InternetOpen( L"Mozilla/4.0 (compatible; )", INTERNET_OPEN_TYPE_PRECONFIG, _null, _null, 0 );
	if ( internet == _null )
		return _false;

	// Remove protocol name.
	if ( address.SearchL2R( L"http://" ) != -1 )
		address = StringPtr( (const _char*) address + 7 );

	_dword index = address.SearchL2R( '/' );

	_char hostname[2048];
	_char filename[2048];

	// Split host name and file name.
	if ( index != -1 )
	{
		StringFormatter::CopyString( hostname, address, index );
		StringFormatter::CopyString( filename, (const _char*) address + index + 1 );
	}
	else
	{
		StringFormatter::CopyString( hostname, address );
	}

	// Connect to the address, open a session.
	_handle session = ::InternetConnect( internet, hostname, INTERNET_DEFAULT_HTTP_PORT, L"", L"", INTERNET_SERVICE_HTTP, 0, _null );
	if ( session == _null )
	{
		::InternetCloseHandle( internet );
		return _false;
	}

	const _char* accepttype[] = { L"*/*", _null };

	// Creates a HTTP GET request handle.
	_handle request = ::HttpOpenRequest( session, L"POST", filename, L"HTTP/1.1", L"", accepttype, INTERNET_FLAG_KEEP_CONNECTION, _null );
	if ( request == _null )
	{
		::InternetCloseHandle( session );
		::InternetCloseHandle( internet );
		return _false;
	}

	const _char* requestheader = L"Accept: */*\r\n";

	// Add request headers.
	if ( ::HttpAddRequestHeaders( request, requestheader, -1, HTTP_ADDREQ_FLAG_ADD ) == 0 )
	{
		::InternetCloseHandle( request );
		::InternetCloseHandle( session );
		::InternetCloseHandle( internet );
		return _false;
	}

	const _char* contenttype = L"Content-Type: application/x-www-form-urlencoded";

	::InternetCloseHandle( request );
	::InternetCloseHandle( session );
	::InternetCloseHandle( internet );

	return _false;
}

_bool FileSystem::BackgroundDownloadFile( StringPtr address, StringPtr localfilename, _float* progress )
{
	BackgroundStruct* bs = new BackgroundStruct;
	bs->mAddress  = address;
	bs->mContent  = localfilename;
	bs->mProgress = progress;

	return Thread::CallFunctionUsingBackgroundThread( BackgroundDownload, BackgroundThreadClose, bs, _null );
}

_bool FileSystem::BackgroundUploadString( StringPtr address, StringPtr content )
{
	BackgroundStruct* bs = new BackgroundStruct;
	bs->mAddress  = address;
	bs->mContent  = content;
	bs->mProgress = _null;

	return Thread::CallFunctionUsingBackgroundThread( BackgroundUpload, BackgroundThreadClose, bs, _null );
}

//----------------------------------------------------------------------------
// DirectoryRecorder
//----------------------------------------------------------------------------

DirectoryRecorder::DirectoryRecorder( StringPtr directory )
	: mOldDirectory( FileSystem::GetCurrentDir( ) )
{
	FileSystem::SetCurrentDir( directory );
}

DirectoryRecorder::~DirectoryRecorder( )
{
	FileSystem::SetCurrentDir( mOldDirectory );
}