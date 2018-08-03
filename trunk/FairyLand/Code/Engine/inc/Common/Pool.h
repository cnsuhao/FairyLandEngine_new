//============================================================================
// Pool.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Pool
//----------------------------------------------------------------------------

//! This class is template container class, represents a pool.

//! A pool is an array in fact, and allocates a maximal size buffer to store all elements.
//! But different from array, the buffer will never enlarge, so it can store limited elements.
//! Each elements are not adjacent in a pool, and linked by a chain inside pool.<br>
//! Template parameter "Type" defines the element type storaged in the array.<br>
//! Template parameter "Key" defines the comparable type of the element type.<br>
//! For example, Pool< ClassA, _dword > defines a pool stores ClassA element, and access it
//! by _dword, means ClassA has a overload operator with _dword ( ==, !=, > and < etc ), or it can convert to _dword.
template< typename Type, typename Key = Type > class Pool : public Link< Type, Key >
{
private:
	//! The node in the pool stores an element, and linked as a chain.
	struct PoolNode : public Node
	{
		//! The index of the node in the pool.
		_dword	mIndex;

		//! Constructor, create an empty node, and set the index to -1.
		//! @param		none
		PoolNode( ) : mIndex( -1 ) { }
	};

	//! The memory pointer of the pool.
	PoolNode*	mNodes;
	//! The maximal size of the pool in number of element.
	_dword		mSize;
	//! The next index of the empty node.
	_dword		mNext;

public:
	//! Constructor, create an empty pool.
	//! @param		size		The maximal size of the pool in number of element.
	Pool( _dword size );
	//! Copy-Constructor, create a pool by copy from another one.
	//! @param		pool		The other pool.
	Pool( const Pool< Type, Key >& pool );
	//! Destructor, delete the pool, and release the memory used by nodes.
	//! @param		none
	~Pool( );

	//! Copy elements from another pool.
	//! @param		pool		The other pool.
	//! @return		The reference of current pool.
	Pool< Type, Key >& operator = ( const Pool< Type, Key >& pool );

	//! Get a const element reference by index.
	//! @param		index		The index of the element in pool.
	//! @return		The const element reference.
	inline const Type& operator [] ( _dword index ) const;
	//! Get a mutable element reference by index.
	//! @param		index		The index of the element in pool.
	//! @return		The mutable element reference.
	inline Type& operator [] ( _dword index );

	//! Get an element pointer by index.
	//! @param		index		The index of the element in pool.
	//! @return		The pointer to a element if the index is valid, null otherwise.
	//! @remarks	This is a safe way to get an element by index from the pool.
	inline Type* Index( _dword index ) const;

	//! Get an iterator by the index of an element.
	//! @param		index		The index of the element in pool.
	//! @return		An iterator reference to the element.
	inline Iterator GetIterator( _dword index ) const;

	//! Get the maximal size of the pool in number of element.
	//! @param		none
	//! @return		The size of the pool.
	inline _dword Size( ) const;

	//! Find an available index for the pool.
	//! @return		The available index of the pool.
	_dword GetAvailableIndex( ) const;

	//! Create a node to store element in the pool.
	//! @param		element		The element to be stored in the pool.
	//! @return		The index of the element if it can be stored, or -1 indicates there is no more node to store it.
	_dword Create( const Type& element );
	//! Insert an element into the pool at specified index.
	//! @param		element		The element to be inserted.
	//! @param		index		The index specifies a position.
	_void Insert( const Type& element, _dword index );
	//! Remove an element from the pool, the element is specified by an iterator.
	//! @param		iterator	The iterator specifies a position.
	_void Remove( Iterator& iterator );
	//! Remove an element from the pool, the element is specified by an index.
	//! @param		index		The index specifies a position.
	_void Remove( _dword index );
	//! Resize the pool by create new memory, and clear all the old element.
	//! @param		newsize		The new size of the pool, should be larger than number in the pool.
	//! @remark		After resize, all the old elements will be removed.
	_void Resize( _dword newsize );

	//! Clear pool and delete all memory used by nodes.
	//! @param		freememory	If free the memory used by array.
	_void Clear( _bool freememory = _false );
};

//----------------------------------------------------------------------------
// Pool Implementation
//----------------------------------------------------------------------------

template< typename Type, typename Key >
Pool< Type, Key >::Pool( _dword size )
{
	FL_ASSERT( size > 0 )

	mSize	= size;
	mNext	= 0;
	mNodes	= _null;
}

template< typename Type, typename Key >
Pool< Type, Key >::Pool( const Pool< Type, Key >& pool )
{
	mSize	= pool.mSize;
	mNext	= pool.mNext;
	mNodes	= _null;

	for ( Node* temp = pool.mHead; temp != _null; temp = temp->mNext )
		Insert( temp->mElement, ( (PoolNode*) temp )->mIndex );
}

template< typename Type, typename Key >
Pool< Type, Key >::~Pool( )
{
	delete[] mNodes;
}

template< typename Type, typename Key >
Pool< Type, Key >& Pool< Type, Key >::operator = ( const Pool< Type, Key >& pool )
{
	Clear( );

	for ( Node* temp = pool.mHead; temp != _null; temp = temp->mNext )
		Insert( temp->mElement, ( (PoolNode*) temp )->mIndex );

	return *this;
}

template< typename Type, typename Key >
const Type& Pool< Type, Key >::operator [] ( _dword index ) const
{
	FL_ASSERT( index < mSize )
	FL_ASSERT( mNodes != _null )
	FL_ASSERT( mNodes[ index ].mIndex != -1 )

	return mNodes[ index ].mElement;
}

template< typename Type, typename Key >
Type& Pool< Type, Key >::operator [] ( _dword index )
{
	FL_ASSERT( index < mSize )
	FL_ASSERT( mNodes != _null )
	FL_ASSERT( mNodes[ index ].mIndex != -1 )

	return mNodes[ index ].mElement;
}

template< typename Type, typename Key >
Type* Pool< Type, Key >::Index( _dword index ) const
{
	if ( index >= mSize || mNodes == _null )
		return _null;

	if ( mNodes[ index ].mIndex == -1 )
		return _null;

	return &( mNodes[ index ].mElement );
}

template< typename Type, typename Key >
typename Pool< Type, Key >::Iterator Pool< Type, Key >::GetIterator( _dword index ) const
{
	if ( index >= mSize || mNodes == _null )
		return Iterator( _null );

	if ( mNodes[ index ].mIndex == -1 )
		return Iterator( _null );

	return Iterator( &( mNodes[ index ] ) );
}

template< typename Type, typename Key >
_dword Pool< Type, Key >::Size( ) const
{
	return mSize;
}

template< typename Type, typename Key >
_dword Pool< Type, Key >::GetAvailableIndex( ) const
{
	if ( mNumber == mSize )
		return -1;

	_dword index = mNext;

	// Find a empty node to place the new element.
	while ( mNodes[ index ].mIndex != -1 )
		index = ( index + 1 ) % mSize;

	return index;
}

template< typename Type, typename Key >
_dword Pool< Type, Key >::Create( const Type& element )
{
	if ( mNumber == mSize )
		return -1;

	if ( mNodes == _null )
		mNodes = new PoolNode[ mSize ];

	// Find a empty node to place the new element.
	while ( mNodes[ mNext ].mIndex != -1 )
		mNext = ( mNext + 1 ) % mSize;

	Insert( element, mNext );

	_dword index = mNext;

	// Update next index.
	mNext = ( mNext + 1 ) % mSize;

	return index;
}

template< typename Type, typename Key >
_void Pool< Type, Key >::Insert( const Type& element, _dword index )
{
	FL_ASSERT( index < mSize )

	if ( mNodes == _null )
		mNodes = new PoolNode[ mSize ];

	FL_ASSERT( mNodes[ index ].mIndex == -1 )

	mNodes[ index ].mElement	= element;
	mNodes[ index ].mIndex		= index;

	Link< Type, Key >::InsertTail( &( mNodes[ index ] ) );
}

template< typename Type, typename Key >
_void Pool< Type, Key >::Remove( Iterator& iterator )
{
	FL_ASSERT( iterator.Valid( ) )

	Remove( ( (PoolNode*) iterator.mNode )->mIndex );

	( (PoolNode*) iterator.mNode )->mIndex = -1;

	iterator.mNode = _null;
}

template< typename Type, typename Key >
_void Pool< Type, Key >::Remove( _dword index )
{
	FL_ASSERT( index < mSize )
	FL_ASSERT( mNodes != _null )
	FL_ASSERT( mNodes[ index ].mIndex != -1 )

	mNodes[ index ].mIndex = -1;

	Link< Type, Key >::Remove( &( mNodes[ index ] ) );
}

template< typename Type, typename Key >
_void Pool< Type, Key >::Resize( _dword newsize )
{
	delete[] mNodes;

	mSize	= newsize;
	mNext	= 0;
	mNodes	= _null;

	Link< Type, Key >::Clear( );
}

template< typename Type, typename Key >
_void Pool< Type, Key >::Clear( _bool freememory )
{
	mNext = 0;

	if ( freememory == _true )
	{
		delete[] mNodes;
		mNodes = _null;
	}
	else
	{
		for ( Node* node = mHead; node != _null; )
		{
			Node* temp = node->mNext;

			node->mPrev = _null;
			node->mNext = _null;

			( (PoolNode*) node )->mIndex = -1;

			node = temp;
		}
	}

	Link< Type, Key >::Clear( );
}

};