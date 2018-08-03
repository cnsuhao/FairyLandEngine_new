//============================================================================
// Link.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Link
//----------------------------------------------------------------------------

//! This class is template base class of some bidirectional chain structure container class.

//! Template parameter "Type" defines the element type storaged in the queue.<br>
//! Template parameter "Key" defines the comparable type of the element type.<br>
//! For example, Link< ClassA, _dword > defines a link stores ClassA element, and access it
//! by _dword, means ClassA has a overload operator with _dword ( ==, !=, > and < etc ), or it can convert to _dword.
template< typename Type, typename Key = Type > class Link
{
public:
	//! The node in the chain structure stores an element.
	struct Node
	{
		//! The element of the node.
		Type	mElement;
		//! The previous node in the chain structure.
		Node*	mPrev;
		//! The next node in the chain structure.
		Node*	mNext;

		//! Constructor, create an unlinked node.
		//! @param		none
		Node( ) : mPrev( _null ), mNext( _null ) { }

		//! Constructor, create an unlinked node stores an element.
		//! @param		element		The element will be stored in the node.
		Node( const Type& element ) : mElement( element ), mPrev( _null ), mNext( _null ) { }

		//! Type conversion, get the element reference.
		//! @param		none
		//! @return		Reference of the element.
		operator Type& ( )
			{ return mElement; }
	};

	//! The iterator of link class, provides ++ and -- operator to walk through the queue.
	class Iterator
	{
	public:
		//! The current node that iterator points to.
		Node*	mNode;
		//! The previous node in the chain structure.
		Node*	mPrev;
		//! The next node in the chain structure.
		Node*	mNext;

		//! Constructor, create an iterator of the link.
		//! @param		node		The node that iterator points to.
		Iterator( Node* node ) : mNode( node ), mPrev( _null ), mNext( _null )
			{ if ( mNode != _null ) { mPrev = mNode->mPrev; mNext = mNode->mNext; } }

		//! Type conversion, get the element reference.
		//! @param		none
		//! @return		Reference of the element.
		operator Type& ( )
			{ return mNode->mElement; }

		//! Increase the iterator, point to the next element of the array.
		//! @param		none
		_void operator ++ ( _long )
			{ mPrev = mNode; mNode = mNext; mNext = mNode != _null ? mNode->mNext : _null; }
		//! decrease the iterator, point to the previous element of the array.
		//! @param		none
		_void operator -- ( _long )
			{ mNext = mNode; mNode = mPrev; mPrev = mNode != _null ? mNode->mPrev : _null; }

		//! Check the iterator if it is valid ( points to an available element ).
		//! @param		none
		//! @return		True if the iterator is valid, false otherwise.
		_bool Valid( ) const
			{ return mNode != _null; }
	};

protected:
	//! The head node of the link.
	Node*	mHead;
	//! The tail node of the link.
	Node*	mTail;
	//! The current number of elements in the link.
	_dword	mNumber;

public:
	//! Constructor, create an empty link, only can be called by derived classes.
	//! @param		none
	inline Link( );

	//! Insert a node into the link at head.
	//! @param		node		The node to be inserted.
	_void InsertHead( Node* node );
	//! Insert a node into the link at tail.
	//! @param		node		The node to be inserted.
	_void InsertTail( Node* node );
	//! Insert a node into the link before the position specified by an iterator.
	//! @param		node		The node to be inserted.
	//! @param		iterator	The iterator specifies a position.
	_void InsertBefore( Node* node, const Iterator& iterator );
	//! Insert a node into the link after the position specified by an iterator.
	//! @param		node		The node to be inserted.
	//! @param		iterator	The iterator specifies a position.
	_void InsertAfter( Node* node, const Iterator& iterator );
	//! Remove a node from the link.
	//! @param		node		The node to be removed.
	_void Remove( Node* node );

	//! Clear the link, just clean the head and tail pointer, derived classes must release memory used by nodes.
	//! @param		none
	inline _void Clear( );

	//! Get the current number of elements in the link.
	//! @param		none
	//! @return		The current number of elements in link.
	inline _dword Number( ) const;

	//! Get the pointer of first element in the link.
	//! @param		none
	//! @return		The pointer to a element if there is, null otherwise.
	inline Type* FirstElement( ) const;

	//! Get the pointer of last element in the link.
	//! @param		none
	//! @return		The pointer to a element if there is, null otherwise.
	inline Type* LastElement( ) const;

	//! Get the pointer of previous element the in link by current element pointer.
	//! @param		element		The current element pointer.
	//! @return		The pointer to a element if there is element next to current one, null otherwise.
	inline Type* PrevElement( Type* element ) const;

	//! Get the pointer of next element in link by current element pointer.
	//! @param		element		The current element pointer.
	//! @return		The pointer to a element if there is element next to current one, null otherwise.
	inline Type* NextElement( Type* element ) const;

	//! Get the head node pointer.
	//! @param		none
	//! @return		The head node.
	inline Node* GetHeadNode( ) const;

	//! Get the tail node pointer.
	//! @param		none
	//! @return		The tail node.
	inline Node* GetTailNode( ) const;

	//! Get the head iterator.
	//! @param		none
	//! @return		The head iterator.
	inline Iterator GetHeadIterator( ) const;
	//! Get the tail iterator.
	//! @param		none
	//! @return		The tail iterator.
	inline Iterator GetTailIterator( ) const;

	//! Search an element by the key in the link.
	//! @param		key			The key of element to be searched.
	//! @return		The const iterator points to the element in array.
	Iterator Search( const Key& key ) const;
};

//----------------------------------------------------------------------------
// Link Implementation
//----------------------------------------------------------------------------

template< typename Type, typename Key >
Link< Type, Key >::Link( ) : mHead( _null ), mTail( _null ), mNumber( 0 )
{
}

template< typename Type, typename Key >
_void Link< Type, Key >::InsertHead( Node* node )
{
	InsertBefore( node, Iterator( mHead ) );
}

template< typename Type, typename Key >
_void Link< Type, Key >::InsertTail( Node* node )
{
	InsertAfter( node, Iterator( mTail ) );
}

template< typename Type, typename Key >
_void Link< Type, Key >::InsertBefore( Node* node, const Iterator& iterator )
{
	FL_ASSERT( node != _null )

	if ( iterator.Valid( ) == _false )
	{
		mHead = mTail = node;
	}
	else
	{
		node->mPrev = iterator.mNode->mPrev;
		node->mNext = iterator.mNode;

		// Relink the previous one and next one both to this node.
		if ( node->mPrev != _null )
			node->mPrev->mNext = node;
		if ( node->mNext != _null )
			node->mNext->mPrev = node;

		// Adjuset head and tail pointer.
		if ( node->mPrev == _null )
			mHead = node;
		if ( node->mNext == _null )
			mTail = node;
	}

	// Increase the element number.
	mNumber ++;
}

template< typename Type, typename Key >
_void Link< Type, Key >::InsertAfter( Node* node, const Iterator& iterator )
{
	FL_ASSERT( node != _null )

	if ( iterator.Valid( ) == _false )
	{
		mHead = mTail = node;
	}
	else
	{
		node->mPrev = iterator.mNode;
		node->mNext = iterator.mNode->mNext;

		// Relink the previous one and next one both to this node.
		if ( node->mPrev != _null )
			node->mPrev->mNext = node;
		if ( node->mNext != _null )
			node->mNext->mPrev = node;

		// Adjuset head and tail pointer.
		if ( node->mPrev == _null )
			mHead = node;
		if ( node->mNext == _null )
			mTail = node;
	}

	// Increase the element number.
	mNumber ++;
}

template< typename Type, typename Key >
_void Link< Type, Key >::Remove( Node* node )
{
	FL_ASSERT( node != _null )

	// Relink the previous one to next one of this node.
	if ( node->mPrev != _null )
		node->mPrev->mNext = node->mNext;
	if ( node->mNext != _null )
		node->mNext->mPrev = node->mPrev;

	// Adjuset head and tail pointer.
	if ( mHead == node )
		mHead = node->mNext;
	if ( mTail == node )
		mTail = node->mPrev;

	// Remove the link from the node.
	node->mPrev = _null;
	node->mNext = _null;

	// Decrease the element number.
	mNumber --;
}

template< typename Type, typename Key >
_void Link< Type, Key >::Clear( )
{
	mHead	= _null;
	mTail	= _null;
	mNumber	= 0;
}

template< typename Type, typename Key >
_dword Link< Type, Key >::Number( ) const
{
	return mNumber;
}

template< typename Type, typename Key >
Type* Link< Type, Key >::FirstElement( ) const
{
	if ( mHead == _null )
		return _null;

	return &( mHead->mElement );
}

template< typename Type, typename Key >
Type* Link< Type, Key >::LastElement( ) const
{
	if ( mTail == _null )
		return _null;

	return &( mTail->mElement );
}

template< typename Type, typename Key >
Type* Link< Type, Key >::PrevElement( Type* element ) const
{
	if ( element == _null )
		return _null;

	Node* node = (Node*) element;

	if ( node->mPrev == _null )
		return _null;

	return &( node->mPrev->mElement );
}

template< typename Type, typename Key >
Type* Link< Type, Key >::NextElement( Type* element ) const
{
	if ( element == _null )
		return _null;

	Node* node = (Node*) element;

	if ( node->mNext == _null )
		return _null;

	return &( node->mNext->mElement );
}

template< typename Type, typename Key >
typename Link< Type, Key >::Node* Link< Type, Key >::GetHeadNode( ) const
{
	return mHead;
}

template< typename Type, typename Key >
typename Link< Type, Key >::Node* Link< Type, Key >::GetTailNode( ) const
{
	return mTail;
}

template< typename Type, typename Key >
typename Link< Type, Key >::Iterator Link< Type, Key >::GetHeadIterator( ) const
{
	return Iterator( mHead );
}

template< typename Type, typename Key >
typename Link< Type, Key >::Iterator Link< Type, Key >::GetTailIterator( ) const
{
	return Iterator( mTail );
}

template< typename Type, typename Key >
typename Link< Type, Key >::Iterator Link< Type, Key >::Search( const Key& key ) const
{
	for ( Node* temp = mHead; temp != _null; temp = temp->mNext )
	{
		if ( temp->mElement == key )
			return Iterator( temp );
	}

	return Iterator( _null );
}

};