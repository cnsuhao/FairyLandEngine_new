//============================================================================
// StringPtr.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// StringPtr Implementation
//----------------------------------------------------------------------------

_dword StringPtr::HashCode1( ) const
{
	if ( mString == _null )
		return 0;

	_char* key = mString;

	_dword seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE, ch = 0;

	while ( *key != 0 )
	{
		ch = ::toupper( *key ++ );
		seed1 = ch ^ ( seed1 + seed2 );
		seed2 = ch + seed1 + seed2 + ( seed2 << 5 ) + 3;
	}

	return seed1;
}

_dword StringPtr::HashCode2( ) const
{
	if ( mString == _null )
		return 0;

	_char* key = mString; _dword seed = 0;

	while ( *key != 0 )
	{
		seed = 31 * seed + ::toupper( *key );

		key ++;
	}

	return seed;
}

_dword StringPtr::HashCode3( ) const
{
	if ( mString == _null )
		return 0;

	_dword hashcode[4] = { 0 };

	// Calculate hash code by add each character together.
	for ( _dword i = 0; mString[i] != 0; i ++ )
		hashcode[ i % 4 ] += ( (_byte) ::toupper( mString[i] ) );

	return ( hashcode[0] ) | ( hashcode[1] << 8 ) | ( hashcode[2] << 16 ) | ( hashcode[3] << 24 );
}

_bool StringPtr::operator == ( const _char* string ) const
{
	return ::wcscmp( GetString( ), string ) == 0;
}

_bool StringPtr::operator != ( const _char* string ) const
{
	return ::wcscmp( GetString( ), string ) != 0;
}

_bool StringPtr::operator > ( const _char* string ) const
{
	return ::wcscmp( GetString( ), string ) > 0;
}

_bool StringPtr::operator < ( const _char* string ) const
{
	return ::wcscmp( GetString( ), string ) < 0;
}

_bool StringPtr::operator >= ( const _char* string ) const
{
	return ::wcscmp( GetString( ), string ) >= 0;
}

_bool StringPtr::operator <= ( const _char* string ) const
{
	return ::wcscmp( GetString( ), string ) <= 0;
}

_dword StringPtr::Length( ) const
{
	return (_dword) ::wcslen( GetString( ) );
}

_dword StringPtr::SizeOfBytes( ) const
{
	return ( (_dword) ::wcslen( GetString( ) ) + 1 ) * sizeof( _char );
}

StringPtr StringPtr::SubString( _dword index ) const
{
	if ( mString == _null )
		return StringPtr( L"" );

	return StringPtr( mString + index );
}

_dword StringPtr::SearchL2R( _char character ) const
{
	if ( mString == _null )
		return -1;

	for ( _dword i = 0; mString[i] != 0; i ++ )
	{
		if ( mString[i] == character )
			return i;
	}

	return -1;
}

_dword StringPtr::SearchL2RLowercase( _char character ) const
{
	if ( mString == _null )
		return -1;

	if ( character >= 'A' && character <= 'Z' )
		character = character - 'A' + 'a';

	for ( _dword i = 0; mString[i] != 0; i ++ )
	{
		_char temp = mString[i];

		if ( temp >= 'A' && temp <= 'Z' )
			temp = temp - 'A' + 'a';

		if ( temp == character )
			return i;
	}

	return -1;
}

_dword StringPtr::SearchR2L( _char character ) const
{
	if ( mString == _null )
		return -1;

	_dword length = Length( );

	for ( _dword i = 0; i < length; i ++ )
	{
		if ( mString[ length - i - 1 ] == character )
			return length - i - 1;
	}

	return -1;
}

_dword StringPtr::SearchR2LLowercase( _char character ) const
{
	if ( mString == _null )
		return -1;

	if ( character >= 'A' && character <= 'Z' )
		character = character - 'A' + 'a';

	_dword length = Length( );

	for ( _dword i = 0; i < length; i ++ )
	{
		_char temp = mString[ length - i - 1 ];

		if ( temp >= 'A' && temp <= 'Z' )
			temp = temp - 'A' + 'a';

		if ( temp == character )
			return length - i - 1;
	}

	return -1;
}

_dword StringPtr::SearchL2R( StringPtr string ) const
{
	_dword length1 = Length( ), length2 = string.Length( );

	if ( length1 < length2 )
		return -1;

	for ( _dword i = 0; i <= length1 - length2; i ++ )
	{
		_bool match = _true;

		for ( _dword j = 0; j < length2; j ++ )
		{
			if ( mString[ i + j ] != string[j] )
				{ match = _false; break; }
		}

		if ( match == _true )
			return i;
	}

	return -1;
}

_dword StringPtr::SearchL2RLowercase( StringPtr string ) const
{
	_dword length1 = Length( ), length2 = string.Length( );

	if ( length1 < length2 )
		return -1;

	for ( _dword i = 0; i <= length1 - length2; i ++ )
	{
		_bool match = _true;

		for ( _dword j = 0; j < length2; j ++ )
		{
			_char temp1 = mString[ i + j ];
			_char temp2 = string[j];

			if ( temp1 >= 'A' && temp1 <= 'Z' )
				temp1 = temp1 - 'A' + 'a';
			if ( temp2 >= 'A' && temp2 <= 'Z' )
				temp2 = temp2 - 'A' + 'a';

			if ( temp1 != temp2 )
				{ match = _false; break; }
		}

		if ( match == _true )
			return i;
	}

	return -1;
}

_dword StringPtr::SearchR2L( StringPtr string ) const
{
	_dword length1 = Length( ), length2 = string.Length( );

	if ( length1 < length2 )
		return -1;

	for ( _dword i = 0; i <= length1 - length2; i ++ )
	{
		_bool match = _true;

		for ( _dword j = 0; j < length2; j ++ )
		{
			if ( mString[ length1 - length2 - i + j ] != string[j] )
				{ match = _false; break; }
		}

		if ( match == _true )
			return length1 - length2 - i;
	}

	return -1;
}

_dword StringPtr::SearchR2LLowercase( StringPtr string ) const
{
	_dword length1 = Length( ), length2 = string.Length( );

	if ( length1 < length2 )
		return -1;

	for ( _dword i = 0; i <= length1 - length2; i ++ )
	{
		_bool match = _true;

		for ( _dword j = 0; j < length2; j ++ )
		{
			_char temp1 = mString[ length1 - length2 - i + j ];
			_char temp2 = string[j];

			if ( temp1 >= 'A' && temp1 <= 'Z' )
				temp1 = temp1 - 'A' + 'a';
			if ( temp2 >= 'A' && temp2 <= 'Z' )
				temp2 = temp2 - 'A' + 'a';

			if ( temp1 != temp2 )
				{ match = _false; break; }
		}

		if ( match == _true )
			return length1 - length2 - i;
	}

	return -1;
}

_long StringPtr::Compare( StringPtr string ) const
{
	return ::wcscmp( GetString( ), string );
}

_long StringPtr::CompareCaseInsensitive( StringPtr string ) const
{
	return ::wcsicmp( GetString( ), string );
}

_bool StringPtr::IsBlank( ) const
{
	for ( _dword i = 0; mString[i] != 0; i ++ )
	{
		if ( mString[i] != ' ' && mString[i] != '\t' && mString[i] != '\r' && mString[i] != '\n' )
			return _false;
	}

	return _true;
}

_bool StringPtr::CompareLowercase( StringPtr string ) const
{
	return ::_wcsicmp( GetString( ), string ) == 0;
}

_bool StringPtr::CompareWildcard( StringPtr string ) const
{
	_dword i = 0, j = 0, length1 = Length( ), length2 = string.Length( );

	for ( i = 0, j = 0; i < length1 && j < length2; i ++ )
	{
		if ( string[j] == '*' )
		{
			if ( string[ j + 1 ] == 0 )
				return _true;

			if ( string[ j + 1 ] == mString[i] )
				j += 2;
		}
		else if ( string[j] == '?' || mString[i] == string[j] )
		{
			j ++;
		}
		else
		{
			return _false;
		}
	}

	// Trim the '*' wildcard.
	while ( j < length2 )
	{
		if ( string[j] == '*' )
			j ++;
		else
			break;
	}

	return i == length1 && j == length2;
}

_long StringPtr::Parse( const _char* format, ... ) const
{
	const _char* buffer = GetString( );

	_long paramnumber = 0, retval = 0;

	// Find how many parameters we need.
	for ( _long i = 0; format[i] != 0; i ++ )
	{
		if ( format[i] == '%' && format[ i + 1 ] != '%' && format[ i + 1 ] != '*' )
			paramnumber ++;
	}

	paramnumber *= sizeof( _void* );

	_chara* parameters = (_chara*)( &format ) + sizeof( format );

	// Make a local stack, and fill local stack the way swscanf likes it.
	__asm sub esp, paramnumber;
	__asm mov ecx, paramnumber;
	__asm mov esi, parameters;
	__asm mov edi, esp;
	__asm rep movsb;

	retval = ::swscanf( buffer, format );

	__asm add esp, paramnumber;

	return retval;
}