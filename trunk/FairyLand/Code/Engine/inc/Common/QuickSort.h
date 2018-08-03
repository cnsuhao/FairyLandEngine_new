//============================================================================
// QuickSort.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// QuickSort
//----------------------------------------------------------------------------

template< typename Type > class QuickSort
{
private:
	_void SortAscendingHelper( Type elements[], _long l, _long r );
	_void SortDescendingHelper( Type elements[], _long l, _long r );

public:
	_void SortAscending( Type elements[], _long number );
	_void SortDescending( Type elements[], _long number );
};

//----------------------------------------------------------------------------
// QuickSort Implementation
//----------------------------------------------------------------------------

template< typename Type >
_void QuickSort< Type >::SortAscendingHelper( Type elements[], _long l, _long r )
{
	if ( l >= r )
		return;

	_long i = l - 1, j = r + 1, c = ( l + r ) / 2;

	Type center = elements[c];

	while ( _true )
	{
		// Find the element that less than center from left to right.
		while ( i < r && elements[ ++ i ] < center );
		// Find the element that greater than center from right to left.
		while ( j > l && elements[ -- j ] > center );

		if ( i >= j )
			break;

		// Swap two elements.
		Type temp	= elements[i];
		elements[i]	= elements[j];
		elements[j]	= temp;
	}

	SortAscendingHelper( elements, l, i - 1 );
	SortAscendingHelper( elements, j + 1, r );
}

template< typename Type >
_void QuickSort< Type >::SortDescendingHelper( Type elements[], _long l, _long r )
{
	if ( l >= r )
		return;

	_long i = l - 1, j = r + 1, c = ( l + r ) / 2;

	Type center = elements[c];

	while ( _true )
	{
		// Find the element that greater than center from left to right.
		while ( i < r && elements[ ++ i ] > center );
		// Find the element that less than center from right to left.
		while ( j > l && elements[ -- j ] < center );

		if ( i >= j )
			break;

		// Swap two elements.
		Type temp	= elements[i];
		elements[i]	= elements[j];
		elements[j]	= temp;
	}

	SortDescendingHelper( elements, l, i - 1 );
	SortDescendingHelper( elements, j + 1, r );
}

template< typename Type >
_void QuickSort< Type >::SortAscending( Type elements[], _long n )
{
	SortAscendingHelper( elements, 0, n - 1 );
}

template< typename Type >
_void QuickSort< Type >::SortDescending( Type elements[], _long n )
{
	SortDescendingHelper( elements, 0, n - 1 );
}

};