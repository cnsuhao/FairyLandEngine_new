//============================================================================
// Queue.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Queue
//----------------------------------------------------------------------------

//! This class is template container class, represents a queue, can insert at front and remove from rear.

//! A queue allocates a buffer to store all elements, the buffer will auto enlarge to hold more elements.
//! Each elements are adjacent in memory as a ring, can be accessed by index base from zero.<br>
//! Template parameter "Type" defines the element type storaged in the queue.<br>
//! Template parameter "Key" defines the comparable type of the element type.<br>
//! For example, Queue< ClassA, _dword > defines a queue stores ClassA element, and access it
//! by _dword, means ClassA has a overload operator with _dword ( ==, !=, > and < etc ), or it can convert to _dword.
template< typename Type, typename Key = Type > class Queue
{
private:
	//! The initial size of the queue in number of element.
	_dword	mInitialSize;
	//! The alloced size of the queue in number of element.
	_dword	mAllocedSize;
	//! The initial grow size of the array in number of element.
	_dword	mInitialGrow;
	//! The alloced grow size of the array in number of element.
	_dword	mAllocedGrow;
	//! The current number of elements in queue.
	_dword	mNumber;
	//! The front index of queue.
	_dword	mFront;
	//! The rear index of queue.
	_dword	mRear;
	//! The memory pointer of the queue.
	Type*	mElements;

public:
	//! The iterator of queue class, provides ++ and -- operator to walk through the queue.
	class Iterator
	{
	public:
		//! The queue reference.
		Queue< Type, Key >*	mQueue;
		//! The current index of the element.
		_dword				mIndex;

		//! Constructor, create an iterator of the queue.
		//! @param		queue	The queue reference.
		//! @param		index		The current index of the element.
		Iterator( Queue< Type, Key >* queue, _dword index )
			: mQueue( queue ), mIndex( index ) { }

		//! Type conversion, get the element reference.
		//! @param		none
		//! @return		Reference of the element.
		operator Type& ( )
			{ return (*mQueue)[ mIndex ]; }

		//! Increase the iterator, point to the next element of the queue.
		//! @param		none
		_void operator ++ ( _long )
			{ mIndex = ( mIndex + 1 ) % ( mQueue->mAllocedSize + 1 ); }
		//! decrease the iterator, point to the previous element of the queue.
		//! @param		none
		_void operator -- ( _long )
			{ mIndex = ( mQueue->mAllocedSize + mIndex ) % ( mQueue->mAllocedSize + 1 ); }

		//! Check the iterator if it is valid ( points to an available element ).
		//! @param		none
		//! @return		True if the iterator is valid, false otherwise.
		_bool Valid( ) const
		{
			if ( mQueue->mFront <= mQueue->mRear )
				return mIndex >= mQueue->mFront && mIndex < mQueue->mRear;
			else
				return ( mIndex >= 0 && mIndex < mQueue->mRear ) || ( mIndex >= mQueue->mFront && mIndex <= mQueue->mAllocedSize );
		}
	};

public:
	//! Constructor, create an empty queue.
	//! @param		size		The initial size of the queue in number of element.
	//! @param		grow		The initial grow size of the queue in number of element.
	Queue( _dword size = 20, _dword grow = 10 );
	//! Copy-Constructor, create a queue by copy from another one.
	//! @param		queue	The other queue.
	Queue( const Queue< Type, Key >& queue );
	//! Destructor, delete the queue, and release the elements memory.
	//! @param		none
	~Queue( );

	//! Copy elements from another queue, and recreates memory to hold it.
	//! @param		queue	The other queue.
	//! @return		The reference of current queue.
	Queue< Type, Key >& operator = ( const Queue< Type, Key >& queue );

	//! Get a const element reference by index.
	//! @param		index		The index of the element in queue.
	//! @return		The const element reference.
	inline const Type& operator [] ( _dword index ) const;
	//! Get a mutable element reference by index.
	//! @param		index		The index of the element in queue.
	//! @return		The mutable element reference.
	inline Type& operator [] ( _dword index );

	//! Get the current number of elements in queue.
	//! @param		none
	//! @return		The current number of elements in queue.
	inline _dword Number( ) const;

	//! Get the front element in queue.
	//! @param		none
	//! @return		The element reference of the front element.
	inline Type& Front( );
	//! Get the rear element in queue.
	//! @param		none
	//! @return		The element reference of the rear element.
	inline Type& Rear( );

	//! Get the head iterator.
	//! @param		none
	//! @return		The head iterator.
	inline Iterator GetHeadIterator( ) const;
	//! Get the tail iterator.
	//! @param		none
	//! @return		The tail iterator.
	inline Iterator GetTailIterator( ) const;

	//! Get an iterator by the index of an element.
	//! @param		index		The index of the element in queue.
	//! @return		An iterator reference to the element.
	inline Iterator GetIterator( _dword index ) const;

	//! Search an element by the key in the queue.
	//! @param		key			The key of element to be searched.
	//! @return		The const iterator points to the element in queue.
	Iterator Search( const Key& key ) const;

	//! Add an element at rear of the queue.
	//! @param		element		The new element to be push into.
	_void Append( const Type& element );
	//! Remove an element at front of the queue.
	//! @param		none
	_void Remove( );
	//! Remove an element by an iterator the queue.
	//! @param		iterator	The iterator specifies a position.
	_void Remove( const Iterator& iterator );
	//! Resize the queue by create new memory and copy old element into it, the memory is larger than older by grow size.
	//! @param		none
	_void Resize( );

	//! Clear the queue, delete all elements and free the memory.
	//! @param		freememory	If free the memory used by array.
	_void Clear( _bool freememory = _false );
	//! Clear the queue, delete all elements and free the memory, set the initial size and the grow size.
	//! @param		size		The initial size of the queue in number of element.
	//! @param		grow		The grow size of the queue in number of element.
	_void Clear( _dword size, _dword grow );
};

//----------------------------------------------------------------------------
// Queue Implementation
//----------------------------------------------------------------------------

template< typename Type, typename Key >
Queue< Type, Key >::Queue( _dword size, _dword grow )
{
	FL_ASSERT( size > 0 && grow > 0 )

	mInitialSize	= size;
	mAllocedSize	= 0;
	mInitialGrow	= grow;
	mAllocedGrow	= grow;
	mNumber			= 0;
	mFront			= 0;
	mRear			= 0;
	mElements		= _null;
}

template< typename Type, typename Key >
Queue< Type, Key >::Queue( const Queue< Type, Key >& queue )
{
	mInitialSize	= queue.mInitialSize;
	mAllocedSize	= queue.mAllocedSize;
	mInitialGrow	= queue.mInitialGrow;
	mAllocedGrow	= queue.mAllocedGrow;
	mNumber			= queue.mNumber;
	mFront			= 0;
	mRear			= queue.mNumber;
	mElements		= _null;

	if ( mAllocedSize > 0 )
	{
		mElements	= new Type[ mAllocedSize + 1 ];

		// Copy elements one by one, execute opertor = of Type class.
		for ( _dword i = 0; i < mNumber; i ++ )
			mElements[i] = queue.mElements[ ( queue.mFront + i ) % ( mAllocedSize + 1 ) ];
	}
}

template< typename Type, typename Key >
Queue< Type, Key >::~Queue( )
{
	// Release queue memory.
	delete[] mElements;
}

template< typename Type, typename Key >
Queue< Type, Key >& Queue< Type, Key >::operator = ( const Queue< Type, Key >& queue )
{
	// Release old memory.
	delete[] mElements;

	mInitialSize	= queue.mInitialSize;
	mAllocedSize	= queue.mAllocedSize;
	mInitialGrow	= queue.mInitialGrow;
	mAllocedGrow	= queue.mAllocedGrow;
	mNumber			= queue.mNumber;
	mFront			= 0;
	mRear			= queue.mNumber;
	mElements		= _null;

	if ( mAllocedSize > 0 )
	{
		mElements	= new Type[ mAllocedSize + 1 ];

		// Copy elements one by one, execute opertor = of Type class.
		for ( _dword i = 0; i < mNumber; i ++ )
			mElements[i] = queue.mElements[ ( queue.mFront + i ) % ( mAllocedSize + 1 ) ];
	}

	return *this;
}

template< typename Type, typename Key >
const Type& Queue< Type, Key >::operator [] ( _dword index ) const
{
	if ( mFront <= mRear )
		FL_ASSERT( index >= mFront && index < mRear )
	else
		FL_ASSERT( index >= 0 && index < mRear ) || ( index >= mFront && index <= mAllocedSize );

	return mElements[ index ];
}

template< typename Type, typename Key >
Type& Queue< Type, Key >::operator [] ( _dword index )
{
	if ( mFront <= mRear )
		FL_ASSERT( index >= mFront && index < mRear )
	else
		FL_ASSERT( ( index >= 0 && index < mRear ) || ( index >= mFront && index <= mAllocedSize ) )

	return mElements[ index ];
}

template< typename Type, typename Key >
_dword Queue< Type, Key >::Number( ) const
{
	return mNumber;
}

template< typename Type, typename Key >
Type& Queue< Type, Key >::Front( )
{
	FL_ASSERT( mFront <= mAllocedSize )

	return mElements[ mFront ];
}

template< typename Type, typename Key >
Type& Queue< Type, Key >::Rear( )
{
	FL_ASSERT( mRear <= mAllocedSize )

	return mElements[ ( mRear + mAllocedSize ) % ( mAllocedSize + 1 ) ];
}

template< typename Type, typename Key >
typename Queue< Type, Key >::Iterator Queue< Type, Key >::GetHeadIterator( ) const
{
	return Iterator( (Queue< Type, Key >*) this, mFront );
}

template< typename Type, typename Key >
typename Queue< Type, Key >::Iterator Queue< Type, Key >::GetTailIterator( ) const
{
	return Iterator( (Queue< Type, Key >*) this, mRear - 1 );
}

template< typename Type, typename Key >
typename Queue< Type, Key >::Iterator Queue< Type, Key >::GetIterator( _dword index ) const
{
	return Iterator( (Queue< Type, Key >*) this, ( mFront + index ) % ( mAllocedSize + 1 ) );
}

template< typename Type, typename Key >
typename Queue< Type, Key >::Iterator Queue< Type, Key >::Search( const Key& key ) const
{
	for ( Iterator it = GetHeadIterator( ); it.Valid( ); it ++ )
	{
		if ( ( (const Key&) (const Type&) it ) == key )
			return it;
	}

	return Iterator( (Queue< Type, Key >*) this, -1 );
}

template< typename Type, typename Key >
_void Queue< Type, Key >::Append( const Type& element )
{
	// Resize the queue when necessary.
	if ( mNumber == mAllocedSize )
		Resize( );

	// Assign the element.
	mElements[ mRear ] = element;

	// Move the rear index.
	mRear = ( mRear + 1 ) % ( mAllocedSize + 1 );

	// Increase the element number.
	mNumber ++;
}

template< typename Type, typename Key >
_void Queue< Type, Key >::Remove( )
{
	FL_ASSERT( mNumber > 0 )

	// Move the front index.
	mFront = ( mFront + 1 ) % ( mAllocedSize + 1 );

	// Decrease the element number.
	mNumber --;
}

template< typename Type, typename Key >
_void Queue< Type, Key >::Remove( const Iterator& iterator )
{
	FL_ASSERT( mNumber > 0 )
	FL_ASSERT( iterator.Valid( ) )

	Iterator temp = iterator;

	_dword index1 = 0, index2 = 0;

	do
	{
		index1 = temp.mIndex;
		temp ++;
		index2 = temp.mIndex;

		// Move element forward.
		mElements[ index1 ] = mElements[ index2 ];
	}
	while( temp.Valid( ) );

	// Decrease the element number.
	mNumber --;

	// Set queue rear.
	mRear = index1;
}

template< typename Type, typename Key >
_void Queue< Type, Key >::Resize( )
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

	Type* newelements = new Type[ newsize + 1 ];

	// Copy elements into new memory, execute opertor = of Type class.
	for ( _dword i = 0; i < mNumber; i ++ )
		newelements[i] = mElements[ ( mFront + i ) % ( mAllocedSize + 1 ) ];

	delete[] mElements;

	mElements		= newelements;
	mAllocedSize	= newsize;
	mAllocedGrow	= newgrow;
	mFront			= 0;
	mRear			= mNumber;
}

template< typename Type, typename Key >
_void Queue< Type, Key >::Clear( _bool freememory )
{
	mNumber	= 0;
	mFront	= 0;
	mRear	= 0;

	if ( freememory == _true )
	{
		delete[] mElements;

		mAllocedSize	= 0;
		mAllocedGrow	= mInitialGrow;
		mElements		= _null;
	}
}

template< typename Type, typename Key >
_void Queue< Type, Key >::Clear( _dword size, _dword grow )
{
	delete[] mElements;

	mInitialSize	= size;
	mAllocedSize	= size;
	mInitialGrow	= grow;
	mAllocedGrow	= grow;
	mNumber			= 0;
	mFront			= 0;
	mRear			= 0;
	mElements		= new Type[ mInitialSize + 1 ];
}

};