//============================================================================
// BinarySearch.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// BinarySearch
//----------------------------------------------------------------------------

template< typename Type, typename Key > class BinarySearch
{
public:
	_long SearchAscending( const Type elements[], const Key& key, _long number, _long& left, _long& right );
	_long SearchDescending( const Type elements[], const Key& key, _long number, _long& left, _long& right );
};

//----------------------------------------------------------------------------
// BinarySearch Implementation
//----------------------------------------------------------------------------

template< typename Type, typename Key >
_long BinarySearch< Type, Key >::SearchAscending( const Type elements[], const Key& key, _long number, _long& left, _long& right )
{
	if ( number <= 0 )
		return -1;

	_long i = 0, j = number - 1;

	while ( i < j - 1 )
	{
		_long m = ( i + j ) / 2;

		if ( key >= (const Key&) elements[m] )
			i = m;
		else
			j = m;
	}

	left	= i;
	right	= j;

	return i;
}

template< typename Type, typename Key >
_long BinarySearch< Type, Key >::SearchDescending( const Type elements[], const Key& key, _long number, _long& left, _long& right )
{
	if ( number <= 0 )
		return -1;

	_long i = 0, j = number - 1;

	while ( i < j - 1 )
	{
		_long m = ( i + j ) / 2;

		if ( key <= (const Key&) elements[m] )
			i = m;
		else
			j = m;
	}

	left	= i;
	right	= j;

	return i;
}

};