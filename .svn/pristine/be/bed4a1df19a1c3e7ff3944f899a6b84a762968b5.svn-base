//============================================================================
// File.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// File Implementation
//----------------------------------------------------------------------------

_bool File::Open( StringPtr filename, _dword createflag, _dword operateflag, _dword shareflag, _bool overlapped )
{
	if ( createflag == _CREATE_NEW || createflag == _CREATE_ALWAYS || createflag == _OPEN_ALWAYS )
	{
		// Create directory if needed.
		FileSystem::CreateDir( StringFormatter::GetPathName( filename ) );
	}

	if ( ( operateflag & _OPERATION_WRITE ) != 0 || ( operateflag & _OPERATION_ALL ) != 0 )
	{
		// Set file attribute to normal.
		FileSystem::SetAttributes( filename, FileSystem::_ATTRIBUTE_NORMAL );
	}

	_dword fileflag = FILE_ATTRIBUTE_NORMAL;

	if ( shareflag & _SHARE_DELETEONCLOSE )
		fileflag |= FILE_FLAG_DELETE_ON_CLOSE;

	if ( overlapped )
		fileflag |= FILE_FLAG_OVERLAPPED;

	// Create win32 file handle.
	mObjectHandle = ::CreateFile( filename, operateflag, shareflag & _SHARE_MASK, _null, createflag, fileflag, _null );

	return mObjectHandle != INVALID_HANDLE_VALUE;
}

_bool File::Read( _void* buffer, _dword length, _dword* bytesread, Overlapped* overlapped )
{
	if ( bytesread != _null )
		*bytesread = 0;

	if ( length == 0 )
		return _true;

	// Length is not zero, so buffer must be valid too.
	FL_ASSERT( buffer != _null )

	_dword bytes = 0;

	// Set overlapped I/O structure.
	if ( overlapped != _null )
	{
		overlapped->mOffset		= GetOffset( );
		overlapped->mOffsetHigh	= 0;

		SeekFromCurrent( length );
	}

	// Read file by win32 api.
	if ( ::ReadFile( mObjectHandle, buffer, length, &bytes, (OVERLAPPED*) overlapped ) == 0 )
	{
		if ( ERROR_IO_PENDING == ::GetLastError( ) )
			return _true;

		return _false;
	}

	// If dont need bytes read, and the bytes read is less than request, then return false.
	if ( bytesread == _null && bytes < length )
		return _false;

	if ( bytesread != _null )
		*bytesread = bytes;

	return _true;
}

_bool File::Write( const _void* buffer, _dword length, _dword* byteswritten, Overlapped* overlapped )
{
	if ( byteswritten != _null )
		*byteswritten = 0;

	if ( length == 0 )
		return _true;

	// Length is not zero, so buffer must be valid too.
	FL_ASSERT( buffer != _null )

	_dword bytes = 0;

	// Set overlapped I/O structure.
	if ( overlapped != _null )
	{
		overlapped->mOffset		= GetOffset( );
		overlapped->mOffsetHigh	= 0;

		SeekFromCurrent( length );
	}

	// Write file by win32 api.
	if ( ::WriteFile( mObjectHandle, buffer, length, &bytes, (OVERLAPPED*) overlapped ) == 0 )
	{
		if ( ERROR_IO_PENDING == ::GetLastError( ) )
			return _true;

		return _false;
	}

	// If dont need bytes written, and the bytes written is less than request, then return false.
	if ( byteswritten == _null && bytes < length )
		return _false;

	if ( byteswritten != _null )
		*byteswritten = bytes;

	return _true;
}

_bool File::Flush( )
{
	return ::FlushFileBuffers( mObjectHandle ) != 0;
}

_bool File::WriteUnicodeFlag( )
{
	_word unicodeflag = 0xFEFF;
	return Write( &unicodeflag, sizeof( _word ) );
}

_bool File::WriteEndline( _bool unicode )
{
	if ( unicode )
		return Write( L"\r\n", 4 );
	else
		return Write( "\r\n", 2 );
}

_bool File::WriteString( StringPtr string, _bool unicode )
{
	if ( unicode )
	{
		return Write( (const _char*) string, string.Length( ) * sizeof( _char ) );
	}
	else
	{
		_chara* ansistring = StringFormatter::UnicodeToAnsi( _null, 0, string );

		_bool retval = Write( ansistring, StringFormatter::StringLength( ansistring ) );

		delete[] ansistring;

		return retval;
	}
}

#ifdef _TYPECHECK_ENABLE

_bool File::WriteFormatStringHelper( const _char* format, ... )
{
	_char buffer[4096];
	StringFormatter::FormatBufferV( buffer, 4096, format, (_chara*)( &format ) + sizeof( format ) );

	return Write( buffer, StringPtr( buffer ).Length( ) * sizeof( _char ) );
}

#else

_bool File::WriteFormatString( const _char* format, ... )
{
	_char buffer[4096];
	StringFormatter::FormatBufferV( buffer, 4096, format, (_chara*)( &format ) + sizeof( format ) );

	return Write( buffer, StringPtr( buffer ).Length( ) * sizeof( _char ) );
}

#endif

_bool File::SeekFromBegin( _long distance )
{
	return ::SetFilePointer( mObjectHandle, distance, _null, FILE_BEGIN ) != -1;
}

_bool File::SeekFromCurrent( _long distance )
{
	return ::SetFilePointer( mObjectHandle, distance, _null, FILE_CURRENT ) != -1;
}

_bool File::SeekFromEnd( _long distance )
{
	return ::SetFilePointer( mObjectHandle, distance, _null, FILE_END ) != -1;
}

_dword File::GetOffset( ) const
{
	if ( mObjectHandle == _null || mObjectHandle == INVALID_HANDLE_VALUE )
		return 0;

	return ::SetFilePointer( mObjectHandle, 0, _null, FILE_CURRENT );
}

_dword File::GetSize( ) const
{
	if ( mObjectHandle == _null || mObjectHandle == INVALID_HANDLE_VALUE )
		return 0;

	return ::GetFileSize( mObjectHandle, _null );
}

_bool File::SetSize( _dword size )
{
	// Obtain current offset, for later restoration.
	_dword offset = ::SetFilePointer( mObjectHandle, 0, _null, FILE_CURRENT );

	if ( offset == -1 )
		return _false;

	// Move pointer to the end of file.
	if ( ::SetFilePointer( mObjectHandle, (_long) size , _null, FILE_BEGIN ) == -1 )
		return _false;

	// Set it as end of file.
	if ( ::SetEndOfFile( mObjectHandle ) == 0 )
		return _false;

	// Restore current offset.
	if ( ::SetFilePointer( mObjectHandle, (_long) offset , _null, FILE_BEGIN ) == -1 )
		return _false;

	return _true;
}