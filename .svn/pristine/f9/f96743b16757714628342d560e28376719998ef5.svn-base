//============================================================================
// String.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// String Implementation
//----------------------------------------------------------------------------

String::String( )
{
	mString = L"";
	mLength	= 0;
	mSize	= 0;
}

String::String( const _char* string )
{
	// Convert to StringPtr to prevent null pointer.
	StringPtr stringptr = string;

	if ( stringptr == L"" )
	{
		mString = L"";
		mLength	= 0;
		mSize	= 0;
	}
	else
	{
		// Create buffer for string.
		mLength	= stringptr.Length( );
		mSize	= mLength + 1;
		mString = (_char*) Memory::HeapAlloc( mSize * sizeof( _char ) );

		// Copy string.
		Memory::MemCpy( mString, (const _char*) stringptr, mLength * sizeof( _char ) );

		// Set null-terminated.
		mString[ mLength ] = 0;
	}
}

String::String( const _char* string, _dword number )
{
	// Convert to StringPtr to prevent null pointer.
	StringPtr stringptr = string;

	if ( number == -1 )
		mLength	= stringptr.Length( );
	else
		mLength	= number;

	if ( mLength == 0 )
	{
		mString = L"";
		mLength	= 0;
		mSize	= 0;
	}
	else
	{
		// Create buffer for string.
		mSize	= mLength + 1;
		mString = (_char*) Memory::HeapAlloc( mSize * sizeof( _char ) );

		// Copy string.
		Memory::MemCpy( mString, (const _char*) stringptr, mLength * sizeof( _char ) );

		// Set null-terminated.
		mString[ mLength ] = 0;
	}
}

String::String( const String& string )
{
	if ( string == L"" )
	{
		mString = L"";
		mLength	= 0;
		mSize	= 0;
	}
	else
	{
		// Create buffer for string.
		mLength	= string.Length( );
		mSize	= mLength + 1;
		mString = (_char*) Memory::HeapAlloc( mSize * sizeof( _char ) );

		// Copy string.
		Memory::MemCpy( mString, (const _char*) string, mLength * sizeof( _char ) );

		// Set null-terminated.
		mString[ mLength ] = 0;
	}
}

String::~String( )
{
	if ( mSize > 0 )
		Memory::HeapFree( mString );
}

String& String::operator = ( const _char* string )
{
	// Convert to StringPtr to prevent null pointer.
	StringPtr stringptr = string;

	mLength = stringptr.Length( );

	// Clear string buffer.
	if ( mLength == 0 )
	{
		if ( mSize > 0 )
			Memory::HeapFree( mString );

		mSize	= 0;
		mString	= L"";

		return *this;
	}

	// Create more memory if necessary.
	if ( mSize < mLength + 1 )
	{
		if ( mSize > 0 )
			Memory::HeapFree( mString );

		mSize	= mLength + 1;
		mString	= (_char*) Memory::HeapAlloc( mSize * sizeof( _char ) );
	}

	// Copy string.
	Memory::MemCpy( mString, (const _char*) stringptr, mLength * sizeof( _char ) );

	// Set null-terminated.
	mString[ mLength ] = 0;

	return *this;
}

String& String::operator = ( const String& string )
{
	mLength = string.Length( );

	// Clear string buffer.
	if ( mLength == 0 )
	{
		if ( mSize > 0 )
			Memory::HeapFree( mString );

		mSize	= 0;
		mString	= L"";

		return *this;
	}

	// Create more memory if necessary.
	if ( mSize < mLength + 1 )
	{
		if ( mSize > 0 )
			Memory::HeapFree( mString );

		mSize	= mLength + 1;
		mString	= (_char*) Memory::HeapAlloc( mSize * sizeof( _char ) );
	}

	// Copy string.
	Memory::MemCpy( mString, (const _char*) string, mLength * sizeof( _char ) );

	// Set null-terminated.
	mString[ mLength ] = 0;

	return *this;
}

String& String::operator += ( const _char* string )
{
	// Convert to StringPtr to prevent null pointer.
	StringPtr stringptr = string;

	_dword length1 = mLength, length2 = stringptr.Length( );

	mLength = length1 + length2;

	// Create more memory if necessary.
	if ( mSize < mLength + 1 )
	{
		_char* oldstring = mString;
		_bool  olddelete = mSize > 0;

		// Create new memory to save both string.
		mSize	= mLength + 1;
		mString = (_char*) Memory::HeapAlloc( mSize * sizeof( _char ) );

		// Copy old string back to buffer.
		Memory::MemCpy( mString, oldstring, length1 * sizeof( _char ) );

		// Delete the old memory.
		if ( olddelete )
			Memory::HeapFree( oldstring );
	}

	// Append new string.
	Memory::MemCpy( mString + length1, (const _char*) stringptr, length2 * sizeof( _char ) );

	// Set null-terminated.
	mString[ mLength ] = 0;

	return *this;
}

String String::operator + ( const _char* string ) const
{
	String returnstring = *this;

	return returnstring += string;
}

String& String::operator += ( _char character )
{
	_dword oldlength = mLength;

	mLength ++;

	// Create more memory if necessary.
	if ( mSize < mLength + 1 )
	{
		_char* oldstring = mString;
		_bool  olddelete = mSize > 0;

		// Create new memory to save both string.
		mSize	= mLength + 1;
		mString = (_char*) Memory::HeapAlloc( mSize * sizeof( _char ) );

		// Copy old string back to buffer.
		Memory::MemCpy( mString, oldstring, oldlength * sizeof( _char ) );

		// Delete the old memory.
		if ( olddelete )
			Memory::HeapFree( oldstring );
	}

	// Append character.
	mString[ oldlength ] = character;

	// Set null-terminated.
	mString[ mLength ] = 0;

	return *this;
}

String String::operator + ( _char character ) const
{
	String returnstring = *this;

	return returnstring += character;
}

#ifdef _TYPECHECK_ENABLE

String& String::FormatHelper( const _char* format, ... )
{
	FL_ASSERT( format != _null )

	_char buffer[4096];
	::_vsnwprintf( buffer, 4096, format, (_chara*)( &format ) + sizeof( format ) );
	buffer[4095] = 0;

	return operator = ( buffer );
}

#else

String& String::Format( const _char* format, ... )
{
	FL_ASSERT( format != _null )

	_char buffer[4096];
	::_vsnwprintf( buffer, 4096, format, (_chara*)( &format ) + sizeof( format ) );
	buffer[4095] = 0;

	return operator = ( buffer );
}

#endif

String& String::CopyString( const _char* string, _dword length )
{
	// Get string length
	if ( length == -1 )
		mLength	= StringFormatter::StringLength( string );
	else
		mLength = length;

	// Clear string buffer.
	if ( mLength == 0 )
	{
		if ( mSize > 0 )
			Memory::HeapFree( mString );

		mSize	= 0;
		mString	= L"";

		return *this;
	}

	// Create more memory if necessary.
	if ( mSize < mLength + 1 )
	{
		if ( mSize > 0 )
			Memory::HeapFree( mString );

		mSize	= mLength + 1;
		mString	= (_char*) Memory::HeapAlloc( mSize * sizeof( _char ) );
	}

	// Copy string.
	Memory::MemCpy( mString, string, mLength * sizeof( _char ) );

	// Set null-terminated.
	mString[ mLength ] = 0;

	return *this;
}

String& String::FormatV( const _char* format, const _void* arguments )
{
	FL_ASSERT( format != _null && arguments != _null )

	_char buffer[ 4096 ];
	::_vsnwprintf( buffer, 4096, format, (_chara*) arguments );
	buffer[ 4095 ] = 0;

	return operator = ( buffer );
}

String& String::Lowercase( )
{
	if ( mLength > 0 )
		::_wcslwr( mString );

	return *this;
}

String& String::Uppercase( )
{
	if ( mLength > 0 )
		::_wcsupr( mString );

	return *this;
}

_void String::Insert( _dword index, _char character )
{
	FL_ASSERT( index <= mLength )

	_dword oldlength = mLength;

	mLength ++;

	// Create more memory if necessary.
	if ( mSize < mLength + 1 )
	{
		_char* oldstring = mString;
		_bool  olddelete = mSize > 0;

		// Create new memory to save both string.
		mSize	= mLength + 1;
		mString = (_char*) Memory::HeapAlloc( mSize * sizeof( _char ) );

		// Copy old string back to buffer.
		Memory::MemCpy( mString, oldstring, index * sizeof( _char ) );
		Memory::MemCpy( mString + index + 1, oldstring + index, ( oldlength - index ) * sizeof( _char ) );

		// Delete the old memory.
		if ( olddelete )
			Memory::HeapFree( oldstring );
	}
	else
	{
		for ( _dword i = index; i < oldlength; i ++ )
			mString[ mLength - i + index - 1 ] = mString[ oldlength - i + index - 1 ];
	}

	// Insert character.
	mString[ index ] = character;

	// Set null-terminated.
	mString[ mLength ] = 0;
}

_void String::Insert( _dword index, StringPtr string )
{
	FL_ASSERT( index <= mLength )

	_dword length1 = mLength, length2 = string.Length( );

	mLength = length1 + length2;

	// Create more memory if necessary.
	if ( mSize < mLength + 1 )
	{
		_char* oldstring = mString;
		_bool  olddelete = mSize > 0;

		// Create new memory to save both string.
		mSize	= mLength + 1;
		mString = (_char*) Memory::HeapAlloc( mSize * sizeof( _char ) );

		// Copy old string back to buffer.
		Memory::MemCpy( mString, oldstring, index * sizeof( _char ) );
		Memory::MemCpy( mString + index + length2, oldstring + index, ( length1 - index ) * sizeof( _char ) );

		// Delete the old memory.
		if ( olddelete )
			Memory::HeapFree( oldstring );
	}
	else
	{
		for ( _dword i = index; i < length1; i ++ )
			mString[ mLength - i + index - 1 ] = mString[ length1 - i + index - 1 ];
	}

	// Insert string.
	Memory::MemCpy( mString + index, (const _char*) string, length2 * sizeof( _char ) );

	// Set null-terminated.
	mString[ mLength ] = 0;
}

_void String::Remove( _dword index, _dword number )
{
	if ( number == 0 )
		return;

	FL_ASSERT( index + number <= mLength )

	for ( _dword i = index + number; i <= mLength; i ++ )
		mString[ i - number ] = mString[i];

	mLength -= number;
}

_bool String::Replace( StringPtr srcstring, StringPtr desstring )
{
	_dword index = SearchL2R( srcstring );

	// Cant find the substring, return false.
	if ( index == -1 )
		return _false;

	// Remove the old substring, and insert the new one.
	Remove( index, srcstring.Length( ) );
	Insert( index, desstring );

	return _true;
}

_bool String::Replace( _dword index, _char character )
{
	if ( index >= mLength )
		return _false;

	mString[ index ] = character;

	return _true;
}

_bool String::Replace( _dword index, _dword number, _char character )
{
	_dword end = index + number;

	if ( end > mLength )
		return _false;

	for ( _dword i = index; i < end; i ++ )
		mString[i] = character;

	return _true;
}

_void String::ReplaceAll( _char oldchar, _char newchar )
{
	for ( _dword i = 0; mString[i] != 0; i ++ )
	{
		if ( mString[i] == oldchar )
			mString[i] = newchar;
	}
}

String& String::TrimLeft( _char character )
{
	_dword number = 0;

	for ( number = 0; number < mLength; number ++ )
	{
		if ( mString[ number ] != character )
			break;
	}

	if ( number > 0 )
		Remove( 0, number );

	return *this;
}

String& String::TrimRight( _char character )
{
	_dword number = 0;

	for ( number = 0; number < mLength; number ++ )
	{
		if ( mString[ mLength - number - 1 ] != character )
			break;
	}

	if ( number > 0 )
		Remove( mLength - number, number );

	return *this;
}

String& String::TrimBoth( _char character )
{
	TrimLeft( character );
	TrimRight( character );

	return *this;
}

String& String::TrimLeft( StringPtr charset )
{
	_dword number = 0;
	_dword length = charset.Length( );

	for ( number = 0; number < mLength; number ++ )
	{
		_dword i = 0;

		for ( i = 0; i < length; i ++ )
		{
			if ( mString[ number ] == charset[i] )
				break;
		}

		if ( i == length )
			break;
	}

	if ( number > 0 )
		Remove( 0, number );

	return *this;
}

String& String::TrimRight( StringPtr charset )
{
	_dword number = 0;
	_dword length = charset.Length( );

	for ( number = 0; number < mLength; number ++ )
	{
		_dword i = 0;

		for ( i = 0; i < length; i ++ )
		{
			if ( mString[ mLength - number - 1 ] == charset[i] )
				break;
		}

		if ( i == length )
			break;
	}

	if ( number > 0 )
		Remove( mLength - number, number );

	return *this;
}

String& String::TrimBoth( StringPtr charset )
{
	TrimLeft( charset );
	TrimRight( charset );

	return *this;
}

String String::SubString( _dword start, _dword length ) const
{
	if ( start >= mLength )
		return String( L"" );

	if ( start + length > mLength )
		length = -1;

	return String( mString + start, length );
}

String String::LeftSubString( _char delimiter ) const
{
	_dword index = SearchL2R( delimiter );

	if ( index == -1 )
		return String( *this );

	return String( mString, index );
}

String String::LeftSubString( StringPtr delimiter ) const
{
	_dword index = SearchL2R( delimiter );

	if ( index == -1 )
		return String( *this );

	return String( mString, index );
}

String String::RightSubString( _char delimiter ) const
{
	_dword index = SearchR2L( delimiter );

	if ( index == -1 )
		return String( *this );

	return String( mString + index + 1, mLength - index - 1 );
}

String String::RightSubString( StringPtr delimiter ) const
{
	_dword index = SearchR2L( delimiter );

	if ( index == -1 )
		return String( *this );

	return String( mString + index + 1, mLength - index - 1 );
}