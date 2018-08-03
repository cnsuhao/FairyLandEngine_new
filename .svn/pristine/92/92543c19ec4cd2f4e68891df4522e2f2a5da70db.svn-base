//============================================================================
// RawArray.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// RawArray
//----------------------------------------------------------------------------

template< typename Type, _dword _MAX_SIZE > class RawArray
{
private:
	Type	mElements[ _MAX_SIZE ];

	RawArray( const RawArray< Type, _MAX_SIZE >& );
	RawArray< Type, _MAX_SIZE >& operator = ( const RawArray< Type, _MAX_SIZE >& );

public:
	//! Default constructor.
	//! @param		none
	inline RawArray( );

	//! Constructor, set all elements to same value.
	//! @param		value		The value to set to all elements.
	inline RawArray( const Type& value );

	//! Get a const element reference by index.
	//! @param		index		The index of the element in array.
	//! @return		The const element reference.
	inline const Type& operator [] ( _dword index ) const;
	//! Get a mutable element reference by index.
	//! @param		index		The index of the element in array.
	//! @return		The mutable element reference.
	inline Type& operator [] ( _dword index );

	//! Get max size of array.
	//! @param		none
	//! @return		The max size of array.
	inline _dword Size( ) const;

	//! Clear array, memory set to 0.
	//! @param		none
	inline _void Clear( );

	//! Clear array, set all elements to same value.
	//! @param		value		The value to set to all elements.
	inline _void Clear( const Type& value );
};

template< typename Type, _dword _MAX_SIZE >
RawArray< Type, _MAX_SIZE >::RawArray( )
{
}

template< typename Type, _dword _MAX_SIZE >
RawArray< Type, _MAX_SIZE >::RawArray( const Type& value )
{
	for ( _dword i = 0; i < _MAX_SIZE; i ++ )
		mElements[i] = value;
}

template< typename Type, _dword _MAX_SIZE >
const Type& RawArray< Type, _MAX_SIZE >::operator [] ( _dword index ) const
{
	FL_ASSERT( index < _MAX_SIZE )

	return mElements[ index ];
}

template< typename Type, _dword _MAX_SIZE >
Type& RawArray< Type, _MAX_SIZE >::operator [] ( _dword index )
{
	FL_ASSERT( index < _MAX_SIZE )

	return mElements[ index ];
}

template< typename Type, _dword _MAX_SIZE >
_dword RawArray< Type, _MAX_SIZE >::Size( ) const
{
	return _MAX_SIZE;
}

template< typename Type, _dword _MAX_SIZE >
_void RawArray< Type, _MAX_SIZE >::Clear( )
{
	Memory::MemSet( mElements, 0, sizeof( mElements ) );
}

template< typename Type, _dword _MAX_SIZE >
_void RawArray< Type, _MAX_SIZE >::Clear( const Type& value )
{
	for ( _dword i = 0; i < _MAX_SIZE; i ++ )
		mElements[i] = value;
}

};