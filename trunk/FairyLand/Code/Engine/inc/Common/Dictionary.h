//============================================================================
// Dictionary.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Dictionary
//----------------------------------------------------------------------------

template< typename Type, typename Key = Type, _dword _TABLE = 1, _dword _DIVISOR = 3907 > class Dictionary : public List< Type, Key >
{
private:
	typedef List< Type, Key >::Node		ElementNode;
	typedef Pair< Key, ElementNode* >	LookupPair;
	typedef Array< LookupPair, Key >	LookupArray;

	// Look up table.
	struct LookupTable
	{
		_dword		mMaxTableLength;
		_dword		mTotalTableLength;
		LookupArray	mLookupArray[ _DIVISOR ];
	};

	// Look up table array.
	LookupTable		mLookupTable[ _TABLE ];

	Dictionary( const Dictionary< Type, Key, _TABLE, _DIVISOR >& dictionary );
	Dictionary< Type, Key, _TABLE, _DIVISOR >& operator = ( const Dictionary< Type, Key, _TABLE, _DIVISOR >& dictionary );

public:
	Dictionary( );
	~Dictionary( );

	inline _dword Table( ) const;
	inline _dword Divisor( ) const;
	inline _dword MaxTableLength( _dword tableid ) const;
	inline _dword AvgTableLength( _dword tableid ) const;

	Type* Index( const Key& key, _dword tableid ) const;

	Iterator Search( const Key& key, _dword tableid ) const;

	Iterator Insert( const Type& element );

	_void InsertLookup( const Iterator& it, const Key& key, _dword tableid );
	_void RemoveLookup( const Key& key, _dword tableid );

	_void Clear( );
};

//----------------------------------------------------------------------------
// Dictionary Implementation
//----------------------------------------------------------------------------

template< typename Type, typename Key, _dword _TABLE, _dword _DIVISOR >
Dictionary< Type, Key, _TABLE, _DIVISOR >::Dictionary( )
{
	for ( _dword i = 0; i < _TABLE; i ++ )
	{
		mLookupTable[i].mMaxTableLength		= 0;
		mLookupTable[i].mTotalTableLength	= 0;
	}
}

template< typename Type, typename Key, _dword _TABLE, _dword _DIVISOR >
Dictionary< Type, Key, _TABLE, _DIVISOR >::~Dictionary( )
{
}

template< typename Type, typename Key, _dword _TABLE, _dword _DIVISOR >
_dword Dictionary< Type, Key, _TABLE, _DIVISOR >::Table( ) const
{
	return _TABLE;
}

template< typename Type, typename Key, _dword _TABLE, _dword _DIVISOR >
_dword Dictionary< Type, Key, _TABLE, _DIVISOR >::Divisor( ) const
{
	return _DIVISOR;
}

template< typename Type, typename Key, _dword _TABLE, _dword _DIVISOR >
_dword Dictionary< Type, Key, _TABLE, _DIVISOR >::MaxTableLength( _dword tableid ) const
{
	FL_ASSERT( tableid < _TABLE )

	return mLookupTable[ tableid ].mMaxTableLength;
}

template< typename Type, typename Key, _dword _TABLE, _dword _DIVISOR >
_dword Dictionary< Type, Key, _TABLE, _DIVISOR >::AvgTableLength( _dword tableid ) const
{
	FL_ASSERT( tableid < _TABLE )

	return mLookupTable[ tableid ].mTotalTableLength / _DIVISOR;
}

template< typename Type, typename Key, _dword _TABLE, _dword _DIVISOR >
Type* Dictionary< Type, Key, _TABLE, _DIVISOR >::Index( const Key& key, _dword tableid ) const
{
	FL_ASSERT( tableid < _TABLE )

	// Get sort from key.
	_dword sort = (_dword) key % _DIVISOR;

	// Get lookup table array.
	const LookupArray& lookuparray = mLookupTable[ tableid ].mLookupArray[ sort ];

	// Search ascending in lookup table array.
	LookupArray::Iterator it = lookuparray.SearchAscending( key );

	// Not found.
	if ( it.Valid( ) == _false )
		return _null;

	ElementNode* node = lookuparray[ it.mIndex ].mObject2;

	return &( node->mElement );
}

template< typename Type, typename Key, _dword _TABLE, _dword _DIVISOR >
typename Dictionary< Type, Key, _TABLE, _DIVISOR >::Iterator Dictionary< Type, Key, _TABLE, _DIVISOR >::Search( const Key& key, _dword tableid ) const
{
	FL_ASSERT( tableid < _TABLE )

	// Get sort from key.
	_dword sort = (_dword) key % _DIVISOR;

	// Get lookup table array.
	const LookupArray& lookuparray = mLookupTable[ tableid ].mLookupArray[ sort ];

	// Search ascending in lookup table array.
	LookupArray::Iterator it = lookuparray.SearchAscending( key );

	// Not found.
	if ( it.Valid( ) == _false )
		return Link< Type, Key >::Iterator( _null );

	ElementNode* node = lookuparray[ it.mIndex ].mObject2;

	return Link< Type, Key >::Iterator( node );
}

template< typename Type, typename Key, _dword _TABLE, _dword _DIVISOR >
typename Dictionary< Type, Key, _TABLE, _DIVISOR >::Iterator Dictionary< Type, Key, _TABLE, _DIVISOR >::Insert( const Type& element )
{
	List< Type, Key >::InsertTail( element );

	return Link< Type, Key >::GetTailIterator( );
}

template< typename Type, typename Key, _dword _TABLE, _dword _DIVISOR >
_void Dictionary< Type, Key, _TABLE, _DIVISOR >::InsertLookup( const Iterator& it, const Key& key, _dword tableid )
{
	FL_ASSERT( tableid < _TABLE )

	// Get sort from key.
	_dword sort = (_dword) key % _DIVISOR;

	// Get lookup table array.
	LookupArray& lookuparray = mLookupTable[ tableid ].mLookupArray[ sort ];

	// Insert into lookup table array.
	lookuparray.InsertAscending( LookupPair( key, it.mNode ) );

	// Lookup table statistic.
	if ( mLookupTable[ tableid ].mMaxTableLength < lookuparray.Number( ) )
		mLookupTable[ tableid ].mMaxTableLength = lookuparray.Number( );

	mLookupTable[ tableid ].mTotalTableLength ++;
}

template< typename Type, typename Key, _dword _TABLE, _dword _DIVISOR >
_void Dictionary< Type, Key, _TABLE, _DIVISOR >::RemoveLookup( const Key& key, _dword tableid )
{
	FL_ASSERT( tableid < _TABLE )

	// Get sort from key.
	_dword sort = (_dword) key % _DIVISOR;

	// Get lookup table array.
	LookupArray& lookuparray = mLookupTable[ tableid ].mLookupArray[ sort ];

	// Search ascending in lookup table array.
	LookupArray::Iterator it = lookuparray.SearchAscending( key );

	if ( it.Valid( ) )
		lookuparray.Remove( it.mIndex );
}

template< typename Type, typename Key, _dword _TABLE, _dword _DIVISOR >
_void Dictionary< Type, Key, _TABLE, _DIVISOR >::Clear( )
{
	for ( _dword i = 0; i < _TABLE; i ++ )
	{
		for ( _dword j = 0; j < _DIVISOR; j ++ )
			mLookupTable[i].mLookupArray[j].Clear( );

		mLookupTable[i].mMaxTableLength		= 0;
		mLookupTable[i].mTotalTableLength	= 0;
	}

	List< Type, Key >::Clear( );
}

};