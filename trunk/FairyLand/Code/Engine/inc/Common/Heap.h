//============================================================================
// Heap.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Heap
//----------------------------------------------------------------------------

//! This class is template container class, represents a heap ( 2D-heap ).

//! A heap is a 2D-heap in fact, allocates a buffer to store pools in 1D, and stores elements in pools.
//! The index of heap is divided into 2 parts by a divisor, the high part and the low part.
//! Elements with same high part index wiil be stroed in heap, and can be accessed by low part index.
//! If there is no element in a heap, the heap will not be created to save memory.
//! Each elements are not adjacent in a heap, and linked by a chain inside heap.<br>
//! Template parameter "Type" defines the element type storaged in the array.<br>
//! Template parameter "Key" defines the comparable type of the element type.<br>
//! For example, Heap< ClassA, _dword > defines a heap stores ClassA element, and access it
//! by _dword, means ClassA has a overload operator with _dword ( ==, !=, > and < etc ), or it can convert to _dword.
template< typename Type, typename Key = Type > class Heap : public Link< Type, Key >
{
private:
	//! The node in the heap stores an element, and linked as a chain.
	struct HeapNode : public Node
	{
		//! The index of the node in the heap.
		_dword	mIndex;

		//! Constructor, create an empty node, and set the index to -1.
		//! @param		none
		HeapNode( ) : mIndex( -1 ) { }
	};

	//! The memory pointer of the pools.
	HeapNode**	mNodes;
	//! The maximal high part index of the heap in number of heap.
	_dword		mHighPart;
	//! The maximal low part index of the heap in number of element.
	_dword		mLowPart;
	//! The next index of the empty node.
	_dword		mNext;

public:
	//! Constructor, create an empty heap.
	//! @param		highpart	The maximal high part index of the heap in number of heap.
	//! @param		lowpart		The maximal low part index of the heap in number of element.
	Heap( _dword highpart, _dword lowpart );
	//! Copy-Constructor, create a heap by copy from another one.
	//! @param		heap		The other heap.
	Heap( const Heap< Type, Key >& heap );
	//! Destructor, delete the heap, and release the memory used by nodes.
	//! @param		none
	~Heap( );

	//! Copy elements from another heap.
	//! @param		heap		The other heap.
	//! @return		The reference of current heap.
	Heap< Type, Key >& operator = ( const Heap< Type, Key >& heap );

	//! Get a const element reference by index.
	//! @param		index		The index of the element in heap.
	//! @return		The const element reference.
	inline const Type& operator [] ( _dword index ) const;
	//! Get a mutable element reference by index.
	//! @param		index		The index of the element in heap.
	//! @return		The mutable element reference.
	inline Type& operator [] ( _dword index );

	//! Get an element pointer by index.
	//! @param		index		The index of the element in heap.
	//! @return		The pointer to a element if the index is valid, null otherwise.
	//! @remarks	This is a safe way to get an element by index from the heap.
	inline Type* Index( _dword index ) const;

	//! Get an iterator by the index of an element.
	//! @param		index		The index of the element in heap.
	//! @return		An iterator reference to the element.
	inline Iterator GetIterator( _dword index ) const;

	//! Get the maximal size of the heap in number of element.
	//! @param		none
	//! @return		The size of the heap.
	inline _dword Size( ) const;

	//! Create a node to store element in the heap.
	//! @param		element		The element to be stored in the heap.
	//! @return		The index of the element if it can be stored, or -1 indicates there is no more node to store it.
	_dword Create( const Type& element );
	//! Insert an element into the heap at specified index.
	//! @param		element		The element to be inserted.
	//! @param		index		The index specifies a position.
	_void Insert( const Type& element, _dword index );
	//! Remove an element from the heap, the element is specified by an iterator.
	//! @param		iterator	The iterator specifies a position.
	_void Remove( Iterator& iterator );
	//! Remove an element from the heap, the element is specified by an index.
	//! @param		index		The index specifies a position.
	_void Remove( _dword index );

	//! Clear heap and delete all memory used by nodes.
	//! @param		freememory	If free the memory used by array.
	_void Clear( _bool freememory = _false );
	//! Clear heap and delete all memory used by nodes.
	//! @param		highpart	The maximal high part index of the heap in number of heap.
	//! @param		lowpart		The maximal low part index of the heap in number of element.
	_void Clear( _dword highpart, _dword lowpart );
};

//----------------------------------------------------------------------------
// Heap Implementation
//----------------------------------------------------------------------------

template< typename Type, typename Key >
Heap< Type, Key >::Heap( _dword highpart, _dword lowpart )
{
	mHighPart	= highpart;
	mLowPart	= lowpart;
	mNext		= 0;
	mNodes		= new HeapNode*[ mHighPart ];

	for ( _dword i = 0; i < mHighPart; i ++ )
		mNodes[i] = _null;
}

template< typename Type, typename Key >
Heap< Type, Key >::Heap( const Heap< Type, Key >& heap )
{
	mHighPart	= heap.mHighPart;
	mLowPart	= heap.mLowPart;
	mNext		= heap.mNext;
	mNodes		= new HeapNode*[ mHighPart ];

	for ( _dword i = 0; i < mHighPart; i ++ )
		mNodes[i] = _null;

	for ( Node* temp = heap.mHead; temp != _null; temp = temp->mNext )
		Insert( temp->mElement, ( (HeapNode*) temp )->mIndex );
}

template< typename Type, typename Key >
Heap< Type, Key >::~Heap( )
{
	for ( _dword i = 0; i < mHighPart; i ++ )
		delete mNodes[i];

	delete[] mNodes;
}

template< typename Type, typename Key >
Heap< Type, Key >& Heap< Type, Key >::operator = ( const Heap< Type, Key >& heap )
{
	Clear( );

	for ( Node* temp = heap.mHead; temp != _null; temp = temp->mNext )
		Insert( temp->mElement, ( (HeapNode*) temp )->mIndex );

	return *this;
}

template< typename Type, typename Key >
const Type& Heap< Type, Key >::operator [] ( _dword index ) const
{
	FL_ASSERT( index < mHighPart * mLowPart )
	FL_ASSERT( mNodes[ index / mLowPart ] != _null )
	FL_ASSERT( mNodes[ index / mLowPart ][ index % mLowPart ].mIndex != -1 )

	return mNodes[ index / mLowPart ][ index % mLowPart ].mElement;
}

template< typename Type, typename Key >
Type& Heap< Type, Key >::operator [] ( _dword index )
{
	FL_ASSERT( index < mHighPart * mLowPart )
	FL_ASSERT( mNodes[ index / mLowPart ] != _null )
	FL_ASSERT( mNodes[ index / mLowPart ][ index % mLowPart ].mIndex != -1 )

	return mNodes[ index / mLowPart ][ index % mLowPart ].mElement;
}

template< typename Type, typename Key >
Type* Heap< Type, Key >::Index( _dword index ) const
{
	if ( index >= mHighPart * mLowPart )
		return _null;

	if ( mNodes[ index / mLowPart ] == _null )
		return _null;

	if ( mNodes[ index / mLowPart ][ index % mLowPart ].mIndex == -1 )
		return _null;

	return &( mNodes[ index / mLowPart ][ index % mLowPart ].mElement );
}

template< typename Type, typename Key >
typename Heap< Type, Key >::Iterator Heap< Type, Key >::GetIterator( _dword index ) const
{
	if ( index >= mHighPart * mLowPart )
		return Iterator( _null );

	if ( mNodes[ index / mLowPart ] == _null )
		return Iterator( _null );

	if ( mNodes[ index / mLowPart ][ index % mLowPart ].mIndex == -1 )
		return Iterator( _null );

	return Iterator( &( mNodes[ index / mLowPart ][ index % mLowPart ] ) );
}

template< typename Type, typename Key >
_dword Heap< Type, Key >::Size( ) const
{
	return mSize;
}

template< typename Type, typename Key >
_dword Heap< Type, Key >::Create( const Type& element )
{
	if ( mNumber == mHighPart * mLowPart )
		return -1;

	// Find a empty node to place the new element.
	while ( mNodes[ mNext / mLowPart ] != _null && mNodes[ mNext / mLowPart ][ mNext % mLowPart ].mIndex != -1 )
		mNext = ( mNext + 1 ) % ( mHighPart * mLowPart );

	Insert( element, mNext );

	_dword index = mNext;

	// Update next index.
	mNext = ( mNext + 1 ) % ( mHighPart * mLowPart );

	return index;
}

template< typename Type, typename Key >
_void Heap< Type, Key >::Insert( const Type& element, _dword index )
{
	FL_ASSERT( index < mHighPart * mLowPart )

	// If the pool is not created, first create it.
	if ( mNodes[ index / mLowPart ] == _null )
	{
		HeapNode* pool = new HeapNode[ mLowPart ];

		mNodes[ index / mLowPart ] = pool;
	}

	HeapNode& node = mNodes[ index / mLowPart ][ index % mLowPart ];

	FL_ASSERT( node.mIndex == -1 )

	node.mElement	= element;
	node.mIndex		= index;

	Link< Type, Key >::InsertTail( &node );
}

template< typename Type, typename Key >
_void Heap< Type, Key >::Remove( Iterator& iterator )
{
	FL_ASSERT( iterator.Valid( ) )

	Remove( ( (HeapNode*) iterator.mNode )->mIndex );

	( (HeapNode*) iterator.mNode )->mIndex = -1;

	iterator.mNode = _null;
}

template< typename Type, typename Key >
_void Heap< Type, Key >::Remove( _dword index )
{
	FL_ASSERT( index < mHighPart * mLowPart )
	FL_ASSERT( mNodes[ index / mLowPart ] != _null )

	HeapNode& node = mNodes[ index / mLowPart ][ index % mLowPart ];

	FL_ASSERT( node.mIndex != -1 )

	node.mIndex = -1;

	Link< Type, Key >::Remove( &node );
}

template< typename Type, typename Key >
_void Heap< Type, Key >::Clear( _bool freememory )
{
	mNext = 0;

	if ( freememory == _true )
	{
		for ( _dword i = 0; i < mHighPart; i ++ )
			delete mNodes[i];

		delete[] mNodes;

		mNodes = new HeapNode*[ mHighPart ];

		for ( _dword j = 0; j < mHighPart; j ++ )
			mNodes[j] = _null;
	}
	else
	{
		for ( Node* node = mHead; node != _null; node = node->mNext )
		{
			node->mPrev = _null;
			node->mNext = _null;

			( (HeapNode*) node )->mIndex = -1;
		}
	}

	Link< Type, Key >::Clear( );
}

template< typename Type, typename Key >
_void Heap< Type, Key >::Clear( _dword highpart, _dword lowpart )
{
	for ( _dword i = 0; i < mHighPart; i ++ )
		delete mNodes[i];

	delete[] mNodes;

	mHighPart	= highpart;
	mLowPart	= lowpart;
	mNext		= 0;
	mNodes		= new HeapNode*[ mHighPart ];

	for ( _dword j = 0; j < mHighPart; j ++ )
		mNodes[j] = _null;

	Link< Type, Key >::Clear( );
}

};