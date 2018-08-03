//============================================================================
// Array.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Array
//----------------------------------------------------------------------------

//! This class is template container class, represents a array.

//! An array allocates a buffer to store all elements, the buffer will auto enlarge to hold more elements.
//! Each elements are adjacent in memory, can be accessed by index base from zero.<br>
//! Template parameter "Type" defines the element type storaged in the array.<br>
//! Template parameter "Key" defines the comparable type of the element type.<br>
//! For example, Array< ClassA, _dword > defines a array stores ClassA element, and access it
//! by _dword, means ClassA has a overload operator with _dword ( ==, !=, > and < etc ), or it can convert to _dword.
template< typename Type, typename Key = Type > class Array
{
private:
	//! The initial size of the array in number of element.
	_dword	mInitialSize;
	//! The alloced size of the array in number of element.
	_dword	mAllocedSize;
	//! The initial grow size of the array in number of element.
	_dword	mInitialGrow;
	//! The alloced grow size of the array in number of element.
	_dword	mAllocedGrow;
	//! The current number of elements in array.
	_dword	mNumber;
	//! The memory pointer of the array.
	Type*	mElements;

public:
	//! The iterator of array class, provides ++ and -- operator to walk through the array.
	class Iterator
	{
	public:
		//! The array reference.
		Array< Type, Key >*	mArray;
		//! The current index of the element.
		_dword				mIndex;

		//! Constructor, create an iterator of the array.
		//! @param		array		The array reference.
		//! @param		index		The current index of the element.
		Iterator( Array< Type, Key >* array, _dword index )
			: mArray( array ), mIndex( index ) { }

		//! Type conversion, get the element reference.
		//! @param		none
		//! @return		Reference of the element.
		operator Type& ( )
			{ return (*mArray)[ mIndex ]; }

		//! Increase the iterator, point to the next element of the array.
		//! @param		none
		_void operator ++ ( _long )
			{ mIndex ++; }
		//! decrease the iterator, point to the previous element of the array.
		//! @param		none
		_void operator -- ( _long )
			{ mIndex --; }

		//! Check the iterator if it is valid ( points to an available element ).
		//! @param		none
		//! @return		True if the iterator is valid, false otherwise.
		_bool Valid( ) const
			{ return mIndex < mArray->Number( ); }
	};

	//! Constructor, create an empty array.
	//! @param		size		The initial size of the array in number of element.
	//! @param		grow		The initial grow size of the array in number of element.
	Array( _dword size = 20, _dword grow = 10 );
	//! Copy-Constructor, create an array by copy from another one.
	//! @param		array		The other array.
	Array( const Array< Type, Key >& array );
	//! Destructor, delete the array, and release the elements memory.
	//! @param		none
	~Array( );

	//! Get a pointer to array buffer.
	//! @param		none.
	//! @return		A pointer to array buffer.
	inline const Type* GetArrayBuffer( ) const;

	//! Get the size of array buffer used, in number of bytes.
	//! @param		none.
	//! @return		The size of array buffer, in number of bytes.
	inline _dword GetArrayBufferLength( ) const;

	//! Create array from a buffer.
	//! @param		buffer		The buffer of array.
	//! @param		number		The number of element in buffer.
	//! @param		memorycopy	Use memory copy if true, otherwise it will copy element one by one, so "operator =" of element will called.
	//! @return		The number of element copied.
	inline _dword CreateArrayFromBuffer( const Type* buffer, _dword number, _bool memorycopy = _false );
	//! Append array from a buffer.
	//! @param		buffer		The buffer of array.
	//! @param		number		The number of element in buffer.
	//! @param		memorycopy	Use memory copy if true, otherwise it will copy element one by one, so "operator =" of element will called.
	//! @return		The number of element copied.
	inline _dword AppendArrayFromBuffer( const Type* buffer, _dword number, _bool memorycopy = _false );

	//! Copy elements from another array, and recreates memory to hold it.
	//! @param		array		The other array.
	//! @return		The reference of current array.
	Array< Type, Key >& operator = ( const Array< Type, Key >& array );

	//! Get a const element reference by index.
	//! @param		index		The index of the element in array.
	//! @return		The const element reference.
	inline const Type& operator [] ( _dword index ) const;
	//! Get a mutable element reference by index.
	//! @param		index		The index of the element in array.
	//! @return		The mutable element reference.
	inline Type& operator [] ( _dword index );

	//! Get the current size of elements buffer in array.
	//! @param		none
	//! @return		The current size of elements buffer in array.
	inline _dword Size( ) const;

	//! Get the current number of elements in array.
	//! @param		none
	//! @return		The current number of elements in array.
	inline _dword Number( ) const;

	//! Get an element pointer by index.
	//! @param		index		The index of the element in array.
	//! @return		The pointer to a element if the index is valid, null otherwise.
	//! @remarks	This is a safe way to get an element by index from the array.
	inline Type* Index( _dword index );

	//! Get the head iterator.
	//! @param		none
	//! @return		The head iterator.
	inline Iterator GetHeadIterator( ) const;
	//! Get the tail iterator.
	//! @param		none
	//! @return		The tail iterator.
	inline Iterator GetTailIterator( ) const;
	//! Get an iterator by the index of an element.
	//! @param		index		The index of the element in array.
	//! @return		An iterator reference to the element.
	inline Iterator GetIterator( _dword index ) const;

	//! Get a const element from the head of array.
	//! @param		none.
	//! @return		The const head element reference.
	inline const Type& GetHeadElement( ) const;
	//! Get a mutable element from the head of array.
	//! @param		none.
	//! @return		The mutable head element reference.
	inline Type& GetHeadElement( );

	//! Get a const element from the tail of array.
	//! @param		none.
	//! @return		The const tail element reference.
	inline const Type& GetTailElement( ) const;
	//! Get a mutable element from the tail of array.
	//! @param		none.
	//! @return		The mutable tail element reference.
	inline Type& GetTailElement( );

	//! Search an element by the key in the array.
	//! @param		key			The key of element to be searched.
	//! @return		The const iterator points to the element in array.
	Iterator Search( const Key& key ) const;

	//! Append an element to the end of the array.
	//! @param		element		The new element to be appended into.
	_void Append( const Type& element );
	//! Insert an element into the array, the position is specified by an iterator.
	//! @param		element		The new element to be inserted into.
	//! @param		iterator	The iterator specifies a position.
	_void Insert( const Type& element, const Iterator& iterator );
	//! Insert an element into the array, the position is specified by an index.
	//! @param		element		The new element to be inserted into.
	//! @param		index		The index specifies a position.
	_void Insert( const Type& element, _dword index );
	//! Remove an element from the array, the element is specified by an iterator.
	//! @param		iterator	The iterator specifies a position.
	//! @return		True if the element is removed, false otherwise.
	_bool Remove( Iterator& iterator );
	//! Remove an element from the array, the element is specified by an index.
	//! @param		iterator	The iterator specifies a position.
	//! @return		True if the element is removed, false otherwise.
	_bool Remove( _dword index );
	//! Resize the array by create new memory and copy old element into it, the memory is larger than old array by grow size.
	//! @param		none
	_void Resize( );

	//! Clear the array, delete all elements and free the memory.
	//! @param		freememory	If free the memory used by array.
	_void Clear( _bool freememory = _false );
	//! Clear the array, delete all elements and free the memory, set the initial size and the grow size.
	//! @param		size		The initial size of the array in number of element.
	//! @param		grow		The grow size of the array in number of element.
	_void Clear( _dword size, _dword grow );

	//! Cut some elements from array.
	//! @param		number		The number of elements to be cut.
	_void Reduce( _dword number );

	Iterator SearchAscending( const Key& key ) const;
	Iterator SearchDescending( const Key& key ) const;

	_dword InsertAscending( const Type& element );
	_dword InsertDescending( const Type& element );

	_bool RemoveAscending( const Key& key );
	_bool RemoveDescending( const Key& key );

	_void SortAscending( );
	_void SortDescending( );
};

//----------------------------------------------------------------------------
// Array Implementation
//----------------------------------------------------------------------------

template< typename Type, typename Key >
Array< Type, Key >::Array( _dword size, _dword grow )
{
	FL_ASSERT( size > 0 && grow > 0 )

	mInitialSize	= size;
	mAllocedSize	= 0;
	mInitialGrow	= grow;
	mAllocedGrow	= grow;
	mNumber			= 0;
	mElements		= _null;
}

template< typename Type, typename Key >
Array< Type, Key >::Array( const Array< Type, Key >& array )
{
	mInitialSize	= array.mInitialSize;
	mAllocedSize	= array.mAllocedSize;
	mInitialGrow	= array.mInitialGrow;
	mAllocedGrow	= array.mAllocedGrow;
	mNumber			= array.mNumber;
	mElements		= _null;

	if ( mAllocedSize > 0 )
	{
		mElements	= new Type[ mAllocedSize ];

		// Copy elements into new memory, execute opertor = of Type class.
		for ( _dword i = 0; i < mNumber; i ++ )
			mElements[i] = array.mElements[i];
	}
}

template< typename Type, typename Key >
Array< Type, Key >::~Array( )
{
	// Release array memory.
	delete[] mElements;
}

template< typename Type, typename Key >
const Type* Array< Type, Key >::GetArrayBuffer( ) const
{
	return mElements;
}

template< typename Type, typename Key >
_dword Array< Type, Key >::GetArrayBufferLength( ) const
{
	return mNumber * sizeof( Type );
}

template< typename Type, typename Key >
_dword Array< Type, Key >::CreateArrayFromBuffer( const Type* buffer, _dword number, _bool memorycopy )
{
	Clear( number, mInitialGrow );

	if ( number > 0 )
	{
		if ( memorycopy )
		{
			Memory::MemCpy( mElements, buffer, sizeof( Type ) * number );
		}
		else
		{
			for ( _dword i = 0; i < number; i ++ )
				mElements[i] = buffer[i];
		}
	}

	mNumber = number;

	return mNumber;
}

template< typename Type, typename Key >
_dword Array< Type, Key >::AppendArrayFromBuffer( const Type* buffer, _dword number, _bool memorycopy )
{
	if ( mAllocedSize < mNumber + number )
	{
		Type* newelements = new Type[ mNumber + number ];

		if ( memorycopy )
		{
			Memory::MemCpy( newelements, mElements, sizeof( Type ) * mNumber );
		}
		else
		{
			for ( _dword i = 0; i < mNumber; i ++ )
				newelements[i] = mElements[i];
		}

		delete[] mElements;

		mElements		= newelements;
		mAllocedSize	= mNumber + number;
	}

	if ( number > 0 )
	{
		if ( memorycopy )
		{
			Memory::MemCpy( mElements + mNumber, buffer, sizeof( Type ) * number );
		}
		else
		{
			for ( _dword i = 0; i < number; i ++ )
				mElements[ mNumber + i ] = buffer[i];
		}
	}

	mNumber += number;

	return mNumber;
}

template< typename Type, typename Key >
Array< Type, Key >& Array< Type, Key >::operator = ( const Array< Type, Key >& array )
{
	// Release old memory.
	delete[] mElements;

	mInitialSize	= array.mInitialSize;
	mAllocedSize	= array.mAllocedSize;
	mInitialGrow	= array.mInitialGrow;
	mAllocedGrow	= array.mAllocedGrow;
	mNumber			= array.mNumber;
	mElements		= _null;

	if ( mAllocedSize > 0 )
	{
		mElements	= new Type[ mAllocedSize ];

		// Copy elements one by one, execute opertor = of Type class.
		for ( _dword i = 0; i < mNumber; i ++ )
			mElements[i] = array.mElements[i];
	}

	return *this;
}

template< typename Type, typename Key >
const Type& Array< Type, Key >::operator [] ( _dword index ) const
{
	FL_ASSERT( index < mNumber )

	return mElements[ index ];
}

template< typename Type, typename Key >
Type& Array< Type, Key >::operator [] ( _dword index )
{
	FL_ASSERT( index < mNumber )

	return mElements[ index ];
}

template< typename Type, typename Key >
_dword Array< Type, Key >::Size( ) const
{
	return mAllocedSize;
}

template< typename Type, typename Key >
_dword Array< Type, Key >::Number( ) const
{
	return mNumber;
}

template< typename Type, typename Key >
Type* Array< Type, Key >::Index( _dword index )
{
	if ( index >= mNumber )
		return _null;

	return &( mElements[ index ] );
}

template< typename Type, typename Key >
typename Array< Type, Key >::Iterator Array< Type, Key >::GetHeadIterator( ) const
{
	return Iterator( (Array< Type, Key >*) this, 0 );
}

template< typename Type, typename Key >
typename Array< Type, Key >::Iterator Array< Type, Key >::GetTailIterator( ) const
{
	return Iterator( (Array< Type, Key >*) this, mNumber - 1 );
}

template< typename Type, typename Key >
typename Array< Type, Key >::Iterator Array< Type, Key >::GetIterator( _dword index ) const
{
	return Iterator( (Array< Type, Key >*) this, index );
}

template< typename Type, typename Key >
const Type& Array< Type, Key >::GetHeadElement( ) const
{
	FL_ASSERT( mNumber > 0 )

	return mElements[ 0 ];
}

template< typename Type, typename Key >
Type& Array< Type, Key >::GetHeadElement( )
{
	FL_ASSERT( mNumber > 0 )

	return mElements[ 0 ];
}

template< typename Type, typename Key >
const Type& Array< Type, Key >::GetTailElement( ) const
{
	FL_ASSERT( mNumber > 0 )

	return mElements[ mNumber - 1 ];
}

template< typename Type, typename Key >
Type& Array< Type, Key >::GetTailElement( )
{
	FL_ASSERT( mNumber > 0 )

	return mElements[ mNumber - 1 ];
}

template< typename Type, typename Key >
typename Array< Type, Key >::Iterator Array< Type, Key >::Search( const Key& key ) const
{
	for ( _dword i = 0; i < mNumber; i ++ )
	{
		if ( (const Key&) mElements[i] == key )
			return Iterator( (Array< Type, Key >*) this, i );
	}

	return Iterator( (Array< Type, Key >*) this, -1 );
}

template< typename Type, typename Key >
_void Array< Type, Key >::Append( const Type& element )
{
	// Resize the array when necessary.
	if ( mNumber == mAllocedSize )
		Resize( );

	// Assign the element.
	mElements[ mNumber ] = element;

	// Increase the element number.
	mNumber ++;
}

template< typename Type, typename Key >
_void Array< Type, Key >::Insert( const Type& element, const Iterator& iterator )
{
	Insert( element, iterator.mIndex );
}

template< typename Type, typename Key >
_void Array< Type, Key >::Insert( const Type& element, _dword index )
{
	FL_ASSERT( index <= mNumber )

	// Resize the array when necessary.
	if ( mNumber == mAllocedSize )
		Resize( );

	// Move elements after the index backward, make a place for the new element.
	for ( _dword i = 0; i < mNumber - index; i ++ )
		mElements[ mNumber - i ] = mElements[ mNumber - i - 1 ];

	// Assign the element.
	mElements[ index ] = element;

	// Increase the element number.
	mNumber ++;
}

template< typename Type, typename Key >
_bool Array< Type, Key >::Remove( Iterator& iterator )
{
	_bool retval = Remove( iterator.mIndex );

	iterator.mIndex = -1;

	return retval;
}

template< typename Type, typename Key >
_bool Array< Type, Key >::Remove( _dword index )
{
	if ( index >= mNumber )
		return _false;

	// Move elements after the index forward, replace the deleted one.
	for ( _dword i = index; i < mNumber - 1; i ++ )
		mElements[i] = mElements[ i + 1 ];

	// Decrease the element number.
	mNumber --;

	return _true;
}

template< typename Type, typename Key >
_void Array< Type, Key >::Resize( )
{
	_dword newsize = 0, newgrow = 0;

	// First time to create memory, use initial size.
	if ( mAllocedSize == 0 )
	{
		newsize = mInitialSize;
		newgrow = mInitialGrow;
	}
	// Otherwise, auto grow.
	else
	{
		newsize = mAllocedSize + mAllocedGrow;
		newgrow = mAllocedGrow + mAllocedGrow;
	}

	Type* newelements = new Type[ newsize ];

	// Copy elements into new memory, execute opertor = of Type class.
	for ( _dword i = 0; i < mNumber; i ++ )
		newelements[i] = mElements[i];

	delete[] mElements;

	mElements		= newelements;
	mAllocedSize	= newsize;
	mAllocedGrow	= newgrow;
}

template< typename Type, typename Key >
_void Array< Type, Key >::Clear( _bool freememory )
{
	mNumber = 0;

	if ( freememory == _true )
	{
		delete[] mElements;

		mAllocedSize	= 0;
		mAllocedGrow	= mInitialGrow;
		mElements		= _null;
	}
}

template< typename Type, typename Key >
_void Array< Type, Key >::Clear( _dword size, _dword grow )
{
	delete[] mElements;

	mInitialSize	= size;
	mAllocedSize	= size;
	mInitialGrow	= grow;
	mAllocedGrow	= grow;
	mNumber			= 0;
	mElements		= _null;

	if ( mInitialSize > 0 )
		mElements = new Type[ mInitialSize ];
}

template< typename Type, typename Key >
_void Array< Type, Key >::Reduce( _dword number )
{
	FL_ASSERT( number <= mNumber )

	mNumber -= number;
}

template< typename Type, typename Key >
typename Array< Type, Key >::Iterator Array< Type, Key >::SearchAscending( const Key& key ) const
{
	_long i = 0, j = mNumber - 1;

	while ( i < j - 1 )
	{
		_long m = ( i + j ) / 2;

		if ( (const Key&) mElements[m] == key )
			return Iterator( (Array< Type, Key >*) this, m );

		if ( (const Key&) mElements[m] < key )
			i = m;
		else
			j = m;
	}

	if ( (_dword) i >= mNumber )
		return Iterator( (Array< Type, Key >*) this, -1 );

	if ( (const Key&) mElements[i] == key )
		return Iterator( (Array< Type, Key >*) this, i );

	if ( (_dword) j >= mNumber )
		return Iterator( (Array< Type, Key >*) this, -1 );

	if ( (const Key&) mElements[j] == key )
		return Iterator( (Array< Type, Key >*) this, j );

	return Iterator( (Array< Type, Key >*) this, -1 );
}

template< typename Type, typename Key >
typename Array< Type, Key >::Iterator Array< Type, Key >::SearchDescending( const Key& key ) const
{
	_long i = 0, j = mNumber - 1;

	while ( i < j - 1 )
	{
		_long m = ( i + j ) / 2;

		if ( (const Key&) mElements[m] == key )
			return Iterator( (Array< Type, Key >*) this, m );

		if ( (const Key&) mElements[m] > key )
			i = m;
		else
			j = m;
	}

	if ( (_dword) i >= mNumber )
		return Iterator( (Array< Type, Key >*) this, -1 );

	if ( (const Key&) mElements[i] == key )
		return Iterator( (Array< Type, Key >*) this, i );

	if ( (_dword) j >= mNumber )
		return Iterator( (Array< Type, Key >*) this, -1 );

	if ( (const Key&) mElements[j] == key )
		return Iterator( (Array< Type, Key >*) this, j );

	return Iterator( (Array< Type, Key >*) this, -1 );
}

template< typename Type, typename Key >
_dword Array< Type, Key >::InsertAscending( const Type& element )
{
	if ( mNumber == 0 )
	{
		Append( element );

		return 0;
	}
	else
	{
		_long i = 0, j = mNumber - 1;

		while ( i < j - 1 )
		{
			_long m = ( i + j ) / 2;

			if ( (const Key&) mElements[m] == (const Key&) element )
			{
				j = m;
				break;
			}

			if ( (const Key&) mElements[m] < (const Key&) element )
				i = m;
			else
				j = m;
		}

		_dword index = -1;

		if ( (const Key&) mElements[i] > (const Key&) element )
			index = i;
		else if ( (const Key&) mElements[j] > (const Key&) element )
			index = j;
		else
			index = j + 1;

		Insert( element, index );

		return index;
	}
}

template< typename Type, typename Key >
_dword Array< Type, Key >::InsertDescending( const Type& element )
{
	if ( mNumber == 0 )
	{
		Append( element );

		return 0;
	}
	else
	{
		_long i = 0, j = mNumber - 1;

		while ( i < j - 1 )
		{
			_long m = ( i + j ) / 2;

			if ( (const Key&) mElements[m] == (const Key&) element )
			{
				j = m;
				break;
			}

			if ( (const Key&) mElements[m] > (const Key&) element )
				i = m;
			else
				j = m;
		}

		_dword index = -1;

		if ( (const Key&) mElements[i] < (const Key&) element )
			index = i;
		else if ( (const Key&) mElements[j] < (const Key&) element )
			index = j;
		else
			index = j + 1;

		Insert( element, index );

		return index;
	}
}

template< typename Type, typename Key >
_bool Array< Type, Key >::RemoveAscending( const Key& key )
{
	Iterator it = SearchAscending( key );

	if ( it.Valid( ) == _false )
		return _false;

	Remove( it.mIndex );

	return _true;
}

template< typename Type, typename Key >
_bool Array< Type, Key >::RemoveDescending( const Key& key )
{
	Iterator it = SearchDescending( key );

	if ( it.Valid( ) == _false )
		return _false;

	Remove( it.mIndex );

	return _true;
}

template< typename Type, typename Key >
_void Array< Type, Key >::SortAscending( )
{
	if ( mNumber == 0 )
		return;

	QuickSort< Type > quicksort;
	quicksort.SortAscending( mElements, mNumber );
}

template< typename Type, typename Key >
_void Array< Type, Key >::SortDescending( )
{
	if ( mNumber == 0 )
		return;

	QuickSort< Type > quicksort;
	quicksort.SortDescending( mElements, mNumber );
}

};