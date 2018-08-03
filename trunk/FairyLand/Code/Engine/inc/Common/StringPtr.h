//============================================================================
// StringPtr.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// StringPtr
//----------------------------------------------------------------------------

//! This class represents a const null-terminated unicode string.

//! This class only has character pointer points to the string, not holds the memory storage of it,
//!	so functions of this class are const, means the string cant be changed.
class StringPtr
{
protected:
	//!	The null-terminated unicode string.
	_char*	mString;

public:
	//! Constructor, create a string points to "".
	//! @param		none
	inline StringPtr( );
	//! Constructor, create a string points to the specified string.
	//! @param		string		The string pointer.
	inline StringPtr( const _char* string );

	//! Type conversion, get the const string pointer.
	//! @param		none
	//! @return		The const string pointer.
	inline operator const _char* ( ) const;

	//! Get the const string pointer.
	//! @param		none
	//! @return		The const string pointer.
	inline const _char* GetString( ) const;

	//! Get a character from string by index.
	//! @param		index		The index of the character in string.
	//! @return		A character specified by index
	inline _char operator [] ( _long index ) const;
	//! Get a character from string by index.
	//! @param		index		The index of the character in string.
	//! @return		A character specified by index
	inline _char operator [] ( _dword index ) const;

	//! Type conversion, convert the string to a hash code.
	//! @param		none
	//! @return		The hash code of the string.
	inline operator _dword ( ) const;

	//! Get hash code of the string. using algorithm 1.
	//! @param		none
	//! @return		The hash code of the string.
	_dword HashCode1( ) const;
	//! Get hash code of the string. using algorithm 2.
	//! @param		none
	//! @return		The hash code of the string.
	_dword HashCode2( ) const;
	//! Get hash code of the string. using algorithm 3.
	//! @param		none
	//! @return		The hash code of the string.
	_dword HashCode3( ) const;

	//! Compare two strings with case sensitive.
	//! @param		string		The second string.
	//! @return		True if two strings are same, false otherwise.
	_bool operator == ( const _char* string ) const;
	//! Compare two strings with case sensitive.
	//! @param		string		The second string.
	//! @return		True if two strings are not same, false otherwise.
	_bool operator != ( const _char* string ) const;
	//! Compare two strings with case sensitive.
	//! @param		string		The second string.
	//! @return		True if the first string is greater than the second one, false otherwise.
	_bool operator > ( const _char* string ) const;
	//! Compare two strings with case sensitive.
	//! @param		string		The second string.
	//! @return		True if the first string is less than the second one, false otherwise.
	_bool operator < ( const _char* string ) const;
	//! Compare two strings with case sensitive.
	//! @param		string		The second string.
	//! @return		True if the first string is greater or equal than the second one, false otherwise.
	_bool operator >= ( const _char* string ) const;
	//! Compare two strings with case sensitive.
	//! @param		string		The second string.
	//! @return		True if the first string is less or equal than the second one, false otherwise.
	_bool operator <= ( const _char* string ) const;

	//! Get the Length of the string, excluding the terminal null.
	//! @param		none
	//! @return		The Length of the string.
	_dword Length( ) const;

	//! Get the number of bytes used by string, include the terminal null.
	//! @param		none
	//! @return		The number of bytes used by string.
	_dword SizeOfBytes( ) const;

	//! Get the substring start from a index.
	//! @param		index		The index of the string, must be less or equal than the length of the string.
	//! @return		The substring from the index.
	StringPtr SubString( _dword index ) const;

	//! Search a character in the string from left to right.
	//! @param		character	The character to be searched.
	//! @return		The index of the first occurrence of the character or -1 indicates cant find.
	_dword SearchL2R( _char character ) const;
	//! Lowercased version of SearchL2R.
	_dword SearchL2RLowercase( _char character ) const;
	//! Search a character in the string from right to left.
	//! @param		character	The character to be searched.
	//! @return		The index of the first occurrence of the character or -1 indicates cant find.
	_dword SearchR2L( _char character ) const;
	_dword SearchR2LLowercase( _char character ) const;
	//! Search a substring in the string from left to right.
	//! @param		string		The substring to be searched.
	//! @return		The index of the first occurrence of the substring or -1 indicates cant find.
	_dword SearchL2R( StringPtr string ) const;
	_dword SearchL2RLowercase( StringPtr string ) const;
	//! Search a substring in the string from right to left.
	//! @param		string		The substring to be searched.
	//! @return		The index of the first occurrence of the substring or -1 indicates cant find.
	_dword SearchR2L( StringPtr string ) const;
	_dword SearchR2LLowercase( StringPtr string ) const;

	//! Compare two strings.
	//! @param		string		The second string object.
	//! @return		The lexicographic relation of two strings, -1, 0, 1.
	_long Compare( StringPtr string ) const;
	//! Compare two strings.
	//! @param		string		The second string object.
	//! @return		The lexicographic relation of two strings, -1, 0, 1.
	_long CompareCaseInsensitive( StringPtr string ) const;

	//! Determine if string is empty, or looks like blank.
	//! @param		none
	//! @return		True if blank, false otherwise.
	_bool IsBlank( ) const;

	//! Compare two strings with lowercase( case insensitive ).
	//! @param		string		The second string object.
	//! @return		True if two strings are same, false otherwise.
	_bool CompareLowercase( StringPtr string ) const;
	//! Compare		two strings with wildcards.
	//! @param		string		The second string with wildcards.
	//! @return		True if the string meets the wildcard, false otherwise.
	//!	@remarks	Wildcards must in the second string, otherwise it will be normal character.<br>
	//!				<b>'*'</b> meets zero or more character.<br>
	//!				<b>'?'</b> meets exactly one character.<br>
	//!				'*' must not adjacent with other wildcards together, otherwise will get wrong result.
	//!				For example, "Hello" will meets "He??o" or "H*o" or "*Hello".
	_bool CompareWildcard( StringPtr string ) const;

	//! Read formatted data from the string, same as sscanf.
	//! @param		format		Format-control string.
	//! @param		...			Optional arguments, addresses of datas to be assigned.
	//! @return		The number of fields successfully parsed, or a negative value if an error occurs.
	_long Parse( const _char* format, ... ) const;
};

//----------------------------------------------------------------------------
// StringPtr Implementation
//----------------------------------------------------------------------------

StringPtr::StringPtr( ) : mString( L"" )
{
}

StringPtr::StringPtr( const _char* string ) : mString( string != _null ? string : L"" )
{
}

StringPtr::operator const _char* ( ) const
{
	if ( mString == _null )
		return L"";

	return mString;
}

const _char* StringPtr::GetString( ) const
{
	if ( mString == _null )
		return L"";

	return mString;
}

_char StringPtr::operator [] ( _long index ) const
{
	if ( mString == _null )
		return 0;

	return mString[ index ];
}

_char StringPtr::operator [] ( _dword index ) const
{
	if ( mString == _null )
		return 0;

	return mString[ index ];
}

StringPtr::operator _dword ( ) const
{
	return HashCode1( );
}

};