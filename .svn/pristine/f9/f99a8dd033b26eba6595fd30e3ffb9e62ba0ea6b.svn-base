//============================================================================
// RawString.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// RawString
//----------------------------------------------------------------------------

template< _dword _MAX_SIZE > class RawString
{
private:
	_char	mString[ _MAX_SIZE ];

public:
	//! Default constructor.
	//! @param		none
	inline RawString( );

	//! Type conversion, get the const string pointer.
	//! @param		none
	//! @return		The const string pointer.
	inline operator const _char* ( ) const;

	//! Get the const string pointer.
	//! @param		none
	//! @return		The const string pointer.
	inline const _char* GetString( ) const;

	//! Type conversion, get the string pointer.
	//! @param		none
	//! @return		The string pointer.
	inline operator _char* ( );

	inline	_char*	GetBuffer( );
	inline	_dword	GetBufferSize( ) const;

	//! Get a const character reference by index.
	//! @param		index		The index of the character in string.
	//! @return		The const element reference.
	inline _char operator [] ( _long index ) const;
	//! Get a const character reference by index.
	//! @param		index		The index of the character in string.
	//! @return		The const element reference.
	inline _char operator [] ( _dword index ) const;

	//! Get a mutable character reference by index.
	//! @param		index		The index of the character in string.
	//! @return		The mutable element reference.
	inline _char& operator [] ( _long index );
	//! Get a mutable character reference by index.
	//! @param		index		The index of the character in string.
	//! @return		The mutable element reference.
	inline _char& operator [] ( _dword index );

	//! Get max size of string.
	//! @param		none
	//! @return		The max size of string.
	inline _dword Size( ) const;

	//! Copy string.
	//! @param		string		The string to copy from.
	//! @return		none.
	inline _void Copy( const _char* string );

	//! Clear string.
	//! @param		none
	inline _void Clear( );
};

template< _dword _MAX_SIZE >
RawString< _MAX_SIZE >::RawString( )
{
	mString[0] = 0;
}

template< _dword _MAX_SIZE >
RawString< _MAX_SIZE >::operator const _char* ( ) const
{
	return mString;
}

template< _dword _MAX_SIZE >
const _char*	RawString< _MAX_SIZE >::GetString( ) const
{
	return	mString;
}

template< _dword _MAX_SIZE >
RawString< _MAX_SIZE >::operator _char* ( )
{
	return mString;
}

template< _dword _MAX_SIZE >
_char*	RawString< _MAX_SIZE >::GetBuffer( )
{
	return	mString;
}

template< _dword _MAX_SIZE >
_dword	RawString< _MAX_SIZE >::GetBufferSize( ) const
{
	return	sizeof( mString );
}

template< _dword _MAX_SIZE >
_char RawString< _MAX_SIZE >::operator [] ( _long index ) const
{
	FL_ASSERT( index >= 0 && index < _MAX_SIZE )

	return mString[ index ];
}

template< _dword _MAX_SIZE >
_char RawString< _MAX_SIZE >::operator [] ( _dword index ) const
{
	FL_ASSERT( index < _MAX_SIZE )

	return mString[ index ];
}

template< _dword _MAX_SIZE >
_char& RawString< _MAX_SIZE >::operator [] ( _long index )
{
	FL_ASSERT( index >= 0 && index < _MAX_SIZE )

	return mString[ index ];
}

template< _dword _MAX_SIZE >
_char& RawString< _MAX_SIZE >::operator [] ( _dword index )
{
	FL_ASSERT( index < _MAX_SIZE )

	return mString[ index ];
}

template< _dword _MAX_SIZE >
_dword RawString< _MAX_SIZE >::Size( ) const
{
	return _MAX_SIZE;
}

template< _dword _MAX_SIZE >
_void RawString< _MAX_SIZE >::Copy( const _char* string )
{
	FL_ASSERT( string != _null )

	for ( _dword i = 0; i < _MAX_SIZE; i ++ )
	{
		mString[i] = string[i];

		if ( string[i] == 0 )
			return;
	}

	mString[ _MAX_SIZE - 1 ] = 0;
}

template< _dword _MAX_SIZE >
_void RawString< _MAX_SIZE >::Clear( )
{
	mString[0] = 0;
}

};