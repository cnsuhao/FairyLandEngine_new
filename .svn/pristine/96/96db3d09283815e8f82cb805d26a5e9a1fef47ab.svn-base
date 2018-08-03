//============================================================================
// StringFormatter.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// StringFormatter Implementation
//----------------------------------------------------------------------------

#ifdef _TYPECHECK_ENABLE

String StringFormatter::FormatStringHelper( const _char* format, ... )
{
	FL_ASSERT( format != _null )

	_char buffer[4096];
	::_vsnwprintf( buffer, 4096, format, (_chara*)( &format ) + sizeof( format ) );
	buffer[4095] = 0;

	return String( buffer );
}

StringPtr StringFormatter::FormatBufferHelper( _char* buffer, _dword size, const _char* format, ... )
{
	FL_ASSERT( buffer != _null && size != 0 && format != _null )

	::_vsnwprintf( buffer, size, format, (_chara*)( &format ) + sizeof( format ) );
	buffer[ size - 1 ] = 0;

	return StringPtr( buffer );
}

#else

String StringFormatter::FormatString( const _char* format, ... )
{
	FL_ASSERT( format != _null )

	_char buffer[4096];
	::_vsnwprintf( buffer, 4096, format, (_chara*)( &format ) + sizeof( format ) );
	buffer[4095] = 0;

	return String( buffer );
}

StringPtr StringFormatter::FormatBuffer( _char* buffer, _dword size, const _char* format, ... )
{
	FL_ASSERT( buffer != _null && size != 0 && format != _null )

	::_vsnwprintf( buffer, size, format, (_chara*)( &format ) + sizeof( format ) );
	buffer[ size - 1 ] = 0;

	return StringPtr( buffer );
}

#endif

String StringFormatter::FormatStringV( const _char* format, const _void* arguments )
{
	FL_ASSERT( format != _null && arguments != _null )

	_char buffer[4096];
	::_vsnwprintf( buffer, 4096, format, (_chara*) arguments );
	buffer[4095] = 0;

	return String( buffer );
}

StringPtr StringFormatter::FormatBufferV( _char* buffer, _dword size, const _char* format, const _void* arguments )
{
	FL_ASSERT( buffer != _null && size != 0 && format != _null && arguments != _null )

	::_vsnwprintf( buffer, size, format, (_chara*) arguments );
	buffer[ size - 1 ] = 0;

	return StringPtr( buffer );
}

String StringFormatter::FormatBytesString( _qword bytes )
{
	_char buffer[1024];

	return String( FormatBytesBuffer( buffer, 1024, bytes ) );
}

StringPtr StringFormatter::FormatBytesBuffer( _char* buffer, _dword size, _qword bytes )
{
	if ( bytes >= 1000 * 1024 * 1024 )
		return FormatBuffer( buffer, size, L"%3.2f GB", (_float) bytes / 1024.0f / 1024.0f / 1024.0f );
	else if ( bytes >= 1000 * 1024 )
		return FormatBuffer( buffer, size, L"%3.2f MB", (_float) bytes / 1024.0f / 1024.0f );
	else
		return FormatBuffer( buffer, size, L"%3.2f KB", (_float) bytes / 1024.0f );
}

String StringFormatter::FormatNumberString( _dword number )
{
	_char buffer[1024];

	return String( FormatNumberBuffer( buffer, 1024, number ) );
}

StringPtr StringFormatter::FormatNumberBuffer( _char* buffer, _dword size, _dword number )
{
	// Max value 4294967295 => 4,294,967,295

	_dword a = number / 1000000000;
	_dword b = ( number % 1000000000 ) / 1000000;
	_dword c = ( number % 1000000 ) / 1000;
	_dword d = number % 1000;

	if ( a > 0 )
		return FormatBuffer( buffer, size, L"%d,%.3d,%.3d,%.3d", a, b, c, d );
	else if ( b > 0 )
		return FormatBuffer( buffer, size, L"%d,%.3d,%.3d", b, c, d );
	else if ( c > 0 )
		return FormatBuffer( buffer, size, L"%d,%.3d", c, d );
	else
		return FormatBuffer( buffer, size, L"%d", d );
}

String StringFormatter::FormatTimeString( _dword millisecond )
{
	_char buffer[1024];

	return String( FormatTimeBuffer( buffer, 1024, millisecond ) );
}

StringPtr StringFormatter::FormatTimeBuffer( _char* buffer, _dword size, _dword millisecond )
{
	_dword hourunit = 60 * 60 * 1000, minuteunit = 60 * 1000, secondunit = 1000;

	_dword hours	= millisecond / hourunit;
	_dword minutes	= ( millisecond - hours * hourunit ) / minuteunit;
	_dword seconds	= ( millisecond - hours * hourunit - minutes* minuteunit ) / secondunit;

	return FormatBuffer( buffer, size, L"%.3d:%.2d:%.2d", hours, minutes, seconds );
}

_dword StringFormatter::FormatCommandLine( _char* cmdline, _char* commands[], _dword cmdnumber, StringPtr delimiter )
{
	_dword parsednumber = 0;

	_char* lastcmd = _null;

	for ( _dword i = 0; cmdline[i] != 0; i ++ )
	{
		if ( delimiter.SearchL2R( cmdline[i] ) != -1 )
		{
			if ( lastcmd != _null && parsednumber < cmdnumber )
				commands[ parsednumber ++ ] = lastcmd;

			cmdline[i]	= 0;
			lastcmd		= _null;

			continue;
		}

		if ( lastcmd == _null )
			lastcmd = cmdline + i;
	}

	if ( lastcmd != _null && parsednumber < cmdnumber )
		commands[ parsednumber ++ ] = lastcmd;

	return parsednumber;
}

_dword StringFormatter::FormatConfigString( const _char* source, Array< Pair< String, String > >& output, _char keydiv, _char argdiv )
{
	_dword parsednumber = 0;
	_dword strnumber = StringPtr( source ).Length( );

	if ( 0 == strnumber )
		return parsednumber;

	do
	{
		_dword keystart = parsednumber;
		_dword keylen = FormatConfigWord( source + parsednumber, keydiv );
		parsednumber += keylen;

		if ( 0 == keylen )
			break;

		if ( keydiv == source[ keystart + keylen - 1 ] )
			keylen -= 1;

		_dword valuestart = parsednumber;
		_dword valuelen = FormatConfigWord( source + parsednumber, argdiv );
		parsednumber += valuelen;

		if ( 0 == valuelen )
			break;

		if ( argdiv == source[ valuestart + valuelen -1 ] )
			valuelen -= 1;

		output.Append( Pair< String, String>( String( source + keystart, keylen ).TrimBoth( L"\t " ), String( source + valuestart, valuelen ).TrimBoth( L"\t " ) ) );


	} while ( parsednumber < strnumber );

	return parsednumber;
}

_dword StringFormatter::FormatConfigWord( const _char* source, _char endword )
{
	_dword getnumber = 0;

	if ( _null == source )
		return getnumber;

	while ( source[ getnumber ] != 0 )
	{
		_char chr = source[ getnumber ];
		getnumber++;

		if ( endword == chr )
			break;

		if ( L'\"' != endword )
		{
			if ( L' ' == chr )
				continue;

			if ( L'\t' == chr)
				continue;
		}

		if ( L'{' == chr )
		{
			getnumber += FormatConfigWord( source + getnumber, L'}' );
			continue;
		}

		if ( L'\"' == chr )
		{
			getnumber += FormatConfigWord( source + getnumber, L'\"' );
			continue;
		}
	}

	return getnumber;
}

_char* StringFormatter::CopyString( _char* desbuffer, const _char* srcbuffer, _dword number )
{
	if ( number == -1 )
	{
		::wcscpy( desbuffer, srcbuffer );
	}
	else
	{
		::wcsncpy( desbuffer, srcbuffer, number );

		desbuffer[ number ] = 0;
	}

	return desbuffer;
}

_char* StringFormatter::AppendString( _char* desbuffer, const _char* srcbuffer, _dword number )
{
	if ( number == -1 )
	{
		::wcscat( desbuffer, srcbuffer );
	}
	else
	{
		_dword length = StringPtr( desbuffer ).Length( );

		::wcsncat( desbuffer, srcbuffer, number );

		desbuffer[ length + number ] = 0;
	}

	return desbuffer;
}

_char* StringFormatter::LowercaseString( _char* string )
{
	if ( string != _null )
		::_wcslwr( string );

	return string;
}

_char* StringFormatter::UppercaseString( _char* string )
{
	if ( string != _null )
		::_wcsupr( string );

	return string;
}

_dword StringFormatter::StringLength( const _char* string )
{
	if ( string == _null )
		return 0;

	return (_dword) ::wcslen( string );
}

_chara* StringFormatter::CopyString( _chara* desbuffer, const _chara* srcbuffer, _dword number )
{
	if ( number == -1 )
	{
		::strcpy( desbuffer, srcbuffer );
	}
	else
	{
		::strncpy( desbuffer, srcbuffer, number );

		desbuffer[ number ] = 0;
	}

	return desbuffer;
}

_chara* StringFormatter::LowercaseString( _chara* string )
{
	if ( string != _null )
		::_strlwr( string );

	return string;
}

_chara* StringFormatter::UppercaseString( _chara* string )
{
	if ( string != _null )
		::_strupr( string );

	return string;
}

_dword StringFormatter::StringLength( const _chara* string )
{
	if ( string == _null )
		return 0;

	return (_dword) ::strlen( string );
}

_void StringFormatter::SplitString( StringPtr source, Array< String >& output, StringPtr division, StringPtr trim )
{
	FL_ASSERT( division.Length( ) > 0 )

	_dword length = source.Length( ), start = -1;

	for ( _dword i = 0; i < length; i ++ )
	{
		if ( division.SearchL2R( source[i] ) == -1 )
		{
			// Start of a string.
			if ( start == -1 )
				start = i;
		}
		else
		{
			// End of a string.
			if ( start != -1 )
			{
				// Build candidate string.
				String candidate( (const _char*) source + start, i - start );

				// Trim candidate string.
				candidate.TrimBoth( trim );

				// It is valid string, can be part of output.
				if ( candidate.Length( ) > 0 )
					output.Append( candidate );

				start = -1;
			}
		}
	}

	// End of a string.
	if ( start != -1 )
	{
		// Build candidate string.
		String candidate( (const _char*) source + start );

		// Trim candidate string.
		candidate.TrimBoth( trim );

		// It is valid string, can be part of output.
		if ( candidate.Length( ) > 0 )
			output.Append( candidate );

		start = -1;
	}
}

_void	StringFormatter::Replace( _char* buffer, _dword bufferlength, StringPtr srcstring, StringPtr desstring )
{
	if ( bufferlength == 0 )
		return;

	_dword	srcsize	= srcstring.Length( );
	_dword	dessize	= desstring.Length( );
	_dword	size	= StringPtr( buffer ).Length( );	
	_dword	index	= StringPtr( buffer ).SearchL2R( srcstring );

	while ( index != -1 )
	{
		FL_ASSERT( index < size );

		// Move right
		if ( srcsize < dessize )
		{
			_dword	moveoffset	= dessize - srcsize;
			_dword	moveend		= index + srcsize;			
			for ( _dword i = size + 1; i >= moveend; -- i )
			{
				if ( i + moveoffset < bufferlength )
				{
					buffer[ i + moveoffset ]	= buffer[ i ];					
				}
			}

			size	= size + moveoffset < bufferlength ? size + moveoffset : bufferlength - 1;
		}
		// Move left
		else if ( srcsize > dessize )
		{
			_dword	moveoffset	= srcsize - dessize;			
			for ( _dword i = index + srcsize; i <= size; ++ i  )
			{
				buffer[ i - moveoffset ]	= buffer[ i ];
			}

			size	-= moveoffset;
		}

		// Replace character.
		for ( _dword i = 0; i < dessize; ++ i )
		{
			if ( index + i >= bufferlength )
				break;

			buffer[ index + i ]	= desstring[ i ];
		}

		buffer[ bufferlength - 1 ]	= 0;

		// Continue searching
		_dword	replaceend	= index + dessize < size ? index + dessize : size;		
		_dword	newindex	= StringPtr( buffer + replaceend ).SearchL2R( srcstring );

		if ( newindex != -1 )
			newindex	+= replaceend;

		index	= newindex;
	}	
}

_charw* StringFormatter::AnsiToUnicode( _charw* buffer, _dword size, const _chara* string, _dword* charcopy )
{
	FL_ASSERT( string != _null )

	// See how big we need the unicode buffer to be.
	_dword length = ::MultiByteToWideChar( CP_ACP, 0, string, -1, _null, 0 );

	if ( buffer == _null )
	{
		// Create the buffer and fill it.
		buffer = new _charw[ length + 1 ];
	}
	else
	{
		length = Math::Min( length, size - 1 );
	}

	::MultiByteToWideChar( CP_ACP, 0, string, -1, buffer, length );
	buffer[ length ] = 0;

	if ( charcopy != _null )
	{
		if ( length > 0 && buffer[ length - 1 ] == 0 ) length --;
		*charcopy = length;
	}

	return buffer;
}

_charw* StringFormatter::AnsiToUnicode( _charw* buffer, _dword size, const _chara* string, _dword number, _dword* charcopy )
{
	FL_ASSERT( string != _null )

	// See how big we need the unicode buffer to be.
	_dword length = ::MultiByteToWideChar( CP_ACP, 0, string, number, _null, 0 );

	if ( buffer == _null )
	{
		// Create the buffer and fill it.
		buffer = new _charw[ length + 1 ];
	}
	else
	{
		length = Math::Min( length, size - 1 );
	}

	::MultiByteToWideChar( CP_ACP, 0, string, number, buffer, length );
	buffer[ length ] = 0;

	if ( charcopy != _null )
	{
		if ( length > 0 && buffer[ length - 1 ] == 0 ) length --;
		*charcopy = length;
	}

	return buffer;
}

_chara* StringFormatter::UnicodeToAnsi( _chara* buffer, _dword size, const _charw* string, _dword* charcopy )
{
	FL_ASSERT( string != _null )

	// See how big we need the ansi buffer to be.
	_dword length = ::WideCharToMultiByte( CP_ACP, 0, string, -1, _null, 0, _null, _null );

	if ( buffer == _null )
	{
		// Create the buffer and fill it.
		buffer = new _chara[ length + 1 ];
	}
	else
	{
		length = Math::Min( length, size - 1 );
	}

	::WideCharToMultiByte( CP_ACP, 0, string, -1, buffer, length, _null, _null );
	buffer[ length ] = 0;

	if ( charcopy != _null )
	{
		if ( length > 0 && buffer[ length - 1 ] == 0 ) length --;
		*charcopy = length;
	}

	return buffer;
}

_chara* StringFormatter::UnicodeToAnsi( _chara* buffer, _dword size, const _charw* string, _dword number, _dword* charcopy )
{
	FL_ASSERT( string != _null )

	// See how big we need the ansi buffer to be.
	_dword length = ::WideCharToMultiByte( CP_ACP, 0, string, number, _null, 0, _null, _null );

	if ( buffer == _null )
	{
		// Create the buffer and fill it.
		buffer = new _chara[ length + 1 ];
	}
	else
	{
		length = Math::Min( length, size - 1 );
	}

	::WideCharToMultiByte( CP_ACP, 0, string, number, buffer, length, _null, _null );
	buffer[ length ] = 0;

	if ( charcopy != _null )
	{
		if ( length > 0 && buffer[ length - 1 ] == 0 ) length --;
		*charcopy = length;
	}

	return buffer;
}

_charw* StringFormatter::UTF8ToUnicode( _charw* buffer, _dword size, const _chara* string, _dword* charcopy )
{
	FL_ASSERT( string != _null )

	// See how big we need the unicode buffer to be.
	_dword length = ::MultiByteToWideChar( CP_UTF8, 0, string, -1, _null, 0 );

	if ( buffer == _null )
	{
		// Create the buffer and fill it.
		buffer = new _charw[ length + 1 ];
	}
	else
	{
		length = Math::Min( length, size - 1 );
	}

	::MultiByteToWideChar( CP_UTF8, 0, string, -1, buffer, length );
	buffer[ length ] = 0;

	if ( charcopy != _null )
	{
		if ( length > 0 && buffer[ length - 1 ] == 0 ) length --;
		*charcopy = length;
	}

	return buffer;
}

_charw* StringFormatter::UTF8ToUnicode( _charw* buffer, _dword size, const _chara* string, _dword number, _dword* charcopy )
{
	FL_ASSERT( string != _null )

	// See how big we need the unicode buffer to be.
	_dword length = ::MultiByteToWideChar( CP_UTF8, 0, string, number, _null, 0 );

	if ( buffer == _null )
	{
		// Create the buffer and fill it.
		buffer = new _charw[ length + 1 ];
	}
	else
	{
		length = Math::Min( length, size - 1 );
	}

	::MultiByteToWideChar( CP_UTF8, 0, string, number, buffer, length );
	buffer[ length ] = 0;

	if ( charcopy != _null )
	{
		if ( length > 0 && buffer[ length - 1 ] == 0 ) length --;
		*charcopy = length;
	}

	return buffer;
}

_chara* StringFormatter::UnicodeToUTF8( _chara* buffer, _dword size, const _charw* string, _dword* charcopy )
{
	FL_ASSERT( string != _null )

	// See how big we need the ansi buffer to be.
	_dword length = ::WideCharToMultiByte( CP_UTF8, 0, string, -1, _null, 0, _null, _null );

	if ( buffer == _null )
	{
		// Create the buffer and fill it.
		buffer = new _chara[ length + 1 ];
	}
	else
	{
		length = Math::Min( length, size - 1 );
	}

	::WideCharToMultiByte( CP_UTF8, 0, string, -1, buffer, length, _null, _null );
	buffer[ length ] = 0;

	if ( charcopy != _null )
	{
		if ( length > 0 && buffer[ length - 1 ] == 0 ) length --;
		*charcopy = length;
	}

	return buffer;
}

_chara* StringFormatter::UnicodeToUTF8( _chara* buffer, _dword size, const _charw* string, _dword number, _dword* charcopy )
{
	FL_ASSERT( string != _null )

	// See how big we need the ansi buffer to be.
	_dword length = ::WideCharToMultiByte( CP_UTF8, 0, string, number, _null, 0, _null, _null );

	if ( buffer == _null )
	{
		// Create the buffer and fill it.
		buffer = new _chara[ length + 1 ];
	}
	else
	{
		length = Math::Min( length, size - 1 );
	}

	::WideCharToMultiByte( CP_UTF8, 0, string, number, buffer, length, _null, _null );
	buffer[ length ] = 0;

	if ( charcopy != _null )
	{
		if ( length > 0 && buffer[ length - 1 ] == 0 ) length --;
		*charcopy = length;
	}

	return buffer;
}

_bool StringFormatter::IsFullpath( StringPtr fullpath )
{
	if ( fullpath.Length( ) < 2 )
		return _false;

	// \\fileserver\test\abc.txt, that is a full path.
	if ( fullpath[0] == L'\\' && fullpath[1] == L'\\' )
		return _true;

	if ( fullpath[1] != L':' )
		return _false;

	// D:\Project\abc.txt, that is a full path too.
	if ( fullpath[0] >= L'A' && fullpath[0] <= L'Z' )
		return _true;

	// d:\Project\abc.txt, that is a full path too.
	if ( fullpath[0] >= L'a' && fullpath[0] <= L'z' )
		return _true;

	return _false;
}

String StringFormatter::GetFileName( StringPtr fullpath, _bool extention, _bool wholepath )
{
	_char buffer[1024];

	return String( GetFileName( buffer, 1024, fullpath, extention, wholepath ) );
}

StringPtr StringFormatter::GetFileName( _char* buffer, _dword size, StringPtr fullpath, _bool extention, _bool wholepath )
{
	FL_ASSERT( buffer != _null && size != 0 )

	_dword i = -1, j = -1;

	if ( wholepath == _false )
	{
		i = fullpath.SearchR2L( '\\' );

		if ( i == -1 )
			i = fullpath.SearchR2L( '/' );

		if ( i == -1 )
			i = fullpath.SearchR2L( ':' );
	}

	if ( extention == _false )
	{
		j = fullpath.SearchR2L( '.' );

		if ( i != -1 && j < i )
			j = -1;
	}

	// Calculate the size of string to be copy.
	_dword length = j == -1 ? fullpath.Length( ) - i : j - i - 1;

	if ( length >= size )
		length = size - 1;

	Memory::MemCpy( buffer, (const _char*) fullpath + i + 1, length * sizeof( _char ) );
	buffer[ length ] = 0;

	return StringPtr( buffer );
}

String StringFormatter::GetPathName( StringPtr fullpath, _bool wholepath )
{
	_char buffer[1024];

	return String( GetPathName( buffer, 1024, fullpath, wholepath ) );
}

StringPtr StringFormatter::GetPathName( _char* buffer, _dword size, StringPtr fullpath, _bool wholepath )
{
	FL_ASSERT( buffer != _null && size != 0 )

	_dword i = -1;

	if ( wholepath == _true )
	{
		i = fullpath.SearchR2L( '\\' );

		if ( i == -1 )
			i = fullpath.SearchR2L( '/' );

		if ( i == -1 )
			i = fullpath.SearchR2L( ':' );
	}
	else
	{
		i = fullpath.SearchL2R( '\\' );

		if ( i == -1 )
			i = fullpath.SearchL2R( '/' );

		if ( i == -1 )
			i = fullpath.SearchL2R( ':' );
	}

	// Calculate the size of string to be copy.
	_dword length = i == -1 ? 0 : i;

	if ( length >= size )
		length = size - 1;

	Memory::MemCpy( buffer, (const _char*) fullpath, length * sizeof( _char ) );
	buffer[ length ] = 0;

	return StringPtr( buffer );
}

String StringFormatter::GetExtention( StringPtr fullpath )
{
	_char buffer[1024];

	return String( GetExtention( buffer, 1024, fullpath ) );
}

StringPtr StringFormatter::GetExtention( _char* buffer, _dword size, StringPtr fullpath )
{
	FL_ASSERT( buffer != _null && size != 0 )

	_dword i = fullpath.SearchR2L( '.' );

	// Calculate the size of string to be copy.
	_dword length = i == -1 ? 0 : fullpath.Length( ) - i;

	if ( length >= size )
		length = size - 1;

	Memory::MemCpy( buffer, (const _char*) fullpath + i + 1, length * sizeof( _char ) );
	buffer[ length ] = 0;

	return StringPtr( buffer );
}

String StringFormatter::RemoveExtention( StringPtr fullpath )
{
	_char buffer[1024];

	return String( RemoveExtention( buffer, 1024, fullpath ) );
}

StringPtr StringFormatter::RemoveExtention( _char* buffer, _dword size, StringPtr fullpath )
{
	FL_ASSERT( buffer != _null && size != 0 )

	_dword i = fullpath.SearchR2L( '.' );

	// Calculate the size of string to be copy.
	_dword length = i == -1 ? fullpath.Length( ) : i;

	if ( length >= size )
		length = size - 1;

	Memory::MemCpy( buffer, (const _char*) fullpath, length * sizeof( _char ) );
	buffer[ length ] = 0;

	return StringPtr( buffer );
}

_dword StringFormatter::StringPtrToBuffer( _char* buffer, _dword size, StringPtr srcstring, _dword offset )
{
	FL_ASSERT( buffer != _null && size != 0 )

	_dword number = 0;

	//calculate offset
	if ( -1 == offset )
	{
		for ( _dword i = 0; i < size; i++ )
		{
			if ( buffer[ i ] != '\0' )
				continue;

			offset = i + 1;
			break;
		}
	}

	if ( offset >= size )
		return number;

	_dword length = Math::Min( srcstring.Length( ), size - offset - 1 );

	CopyString( buffer + offset, srcstring, length );

	buffer[ offset + length ] = '\0';

	return length + 1;
}

_char* StringFormatter::BufferToStringPtr( _char* buffer, _dword size, _dword& offset )
{
	FL_ASSERT( buffer != _null && size != 0 )

	_char* ret = _null;

	if ( offset >= size )
		return ret;

	ret = buffer + offset;

	for ( _dword i = offset; i < size; i++ )
	{
		if ( buffer[ i ] == '\0' )
		{
			offset = i;
			break;
		}
	}

	offset++;

	return ret;
}

_bool StringFormatter::FilterProfanityFromString( String& string, const Array< String >& profanity, _char replacer )
{
	Array< RegularExpression* > regexps;

	for ( _dword i = 0; i < profanity.Number( ); i ++ )
	{
		RegularExpression* reg = new RegularExpression;

		reg->SetRegularExpression( profanity[i],
			RegularExpression::_MATCH_PRE_NONEWORD |
			RegularExpression::_MATCH_POST_NONEWORD |
			RegularExpression::_MATCH_INNER_NONEWORD | 
			RegularExpression::_MATCH_SEQUENCE );

		regexps.Append( reg );
	}

	_bool retval = FilterProfanityFromString( string, regexps, replacer );

	for ( _dword i = 0; i < regexps.Number( ); i ++ )
		delete regexps[i];

	return retval;
}

_bool StringFormatter::FilterProfanityFromString( String& string, const Array< RegularExpression* >& profanity, _char replacer )
{
	_bool retval = _false;

	// Convert subject string to UTF-8.
	_chara tempbuffer[4096];
	StringFormatter::UnicodeToUTF8( tempbuffer, 4096, string );

	_dword startindex[1024], endindex[1024];

	for ( _dword i = 0; i < profanity.Number( ); i ++ )
	{
		_dword matchs = profanity[i]->Match( tempbuffer, startindex, endindex, 1024 );

		if ( matchs == 0 )
			continue;

		for ( _dword m = 0; m < matchs; m ++ )
			string.Replace( startindex[m], endindex[m] - startindex[m], replacer );

		retval = _true;
	}

	return retval;
}