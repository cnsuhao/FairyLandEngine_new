//============================================================================
// Hash.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Hash
//----------------------------------------------------------------------------

//! This class is template container class, represents a hash table.

//! A hash table stores elements in many hash buckets, and use hash code of an element to find which
//! bucket should it stay. A hash bucket is a hash node list that sort ascending, all nodes are
//! allocated from heap. When use elements in a hash table, the element class must overload
//! operator _dword ( ) to calculate hash code.<br>
//! Template parameter "Type" defines the element type storaged in the hash table.<br>
//! Template parameter "Key" defines the comparable type of the element type.<br>
//! For example, Hash< ClassA, _dword > defines a hash table stores ClassA element, and access it
//! by _dword, means ClassA has a overload operator with _dword ( ==, !=, > and < etc ), or it can convert to _dword.
template< typename Type, typename Key = Type > class Hash : public Link< Type, Key >
{
private:
	//! The node in the hash table stores an element, and linked as a chain.
	class HashNode : public Node
	{
	public:
		//! The link node in hash bucket.
		HashNode*	mLinkPrev;
		//! The link node in hash bucket.
		HashNode*	mLinkNext;
		//! The hash code of the node.
		_dword		mHashCode;

		//! Constructor, create an empty node.
		//! @param		none
		HashNode( ) : mLinkPrev( _null ), mLinkNext( _null ), mHashCode( -1 ) { }
	};

	//! The memory pointer of the hash buckets.
	HashNode**	mNodes;
	//! The divisor of the hash table, used to find a hash bucket.
	_dword		mDivisor;

public:
	//! Constructor, create an empty hash table.
	//! @param		divisor		The divisor of the hash table.
	Hash( _dword divisor );
	//! Copy-Constructor, create a hash table by copy from another one.
	Hash( const Hash< Type, Key >& hash );
	//! Destructor, delete the hash table, and release the memory used by nodes.
	//! @param		none
	~Hash( );

	//! Copy elements from another hash table.
	//! @param		hash		The other hash table.
	//! @return		The reference of current hash table.
	Hash< Type, Key >& operator = ( const Hash< Type, Key >& hash );

	//! Get a const element reference by element key.
	//! @param		key			The key of element, used to calculate hash code.
	//! @return		The const element reference.
	inline const Type& operator [] ( const Key& key ) const;
	//! Get a mutable element reference by element key.
	//! @param		key			The key of element, used to calculate hash code.
	//! @return		The mutable element reference.
	inline Type& operator [] ( const Key& key );

	//! Get an iterator by the key of an element.
	//! @param		key			The key of element, used to calculate hash code.
	//! @return		An iterator reference to the element.
	Iterator GetIterator( const Key& key ) const;

	//! Get an element pointer by element key.
	//! @param		key			The key of element, used to calculate hash code.
	//! @return		The pointer to a element if the index is valid, null otherwise.
	//! @remarks	This is a safe way to get an element by index from the hash.
	Type* Index( const Key& key ) const;

	//! Insert an element into the hash table by hash code that calculated from element key.
	//! @param		element		The new element to be inserted into.
	//! @return		An iterator reference to the new element.
	Iterator Insert( const Type& element );
	//! Remove an element from the hash table, the element is specified by an iterator.
	//! @param		iterator	The iterator specifies a position.
	_bool Remove( Iterator& iterator );
	//! Remove an element from the hash table, the element is specified by element key.
	//! @param		key			The key of element, used to calculate hash code.
	_bool Remove( const Key& key );

	//! Clear hash table and delete all memory used by nodes.
	//! @param		none
	_void Clear( );
	//! Clear hash table and delete all memory used by nodes.
	//! @param		divisor		The divisor of the hash table.
	_void Clear( _dword divisor );
};

//----------------------------------------------------------------------------
// Hash Implementation
//----------------------------------------------------------------------------

template< typename Type, typename Key >
Hash< Type, Key >::Hash( _dword divisor )
{
	FL_ASSERT( divisor > 0 )

	mDivisor	= divisor;
	mNodes		= new HashNode*[ mDivisor ];

	for ( _dword i = 0; i < mDivisor; i ++ )
		mNodes[i] = _null;
}

template< typename Type, typename Key >
Hash< Type, Key >::Hash( const Hash< Type, Key >& hash )
{
	mDivisor	= hash.mDivisor;
	mNodes		= new HashNode*[ mDivisor ];

	for ( _dword i = 0; i < mDivisor; i ++ )
		mNodes[i] = _null;

	for ( Node* temp = hash.mHead; temp != _null; temp = temp->mNext )
		Insert( temp->mElement );
}

template< typename Type, typename Key >
Hash< Type, Key >::~Hash( )
{
	// Delete all hash node.
	for ( _dword i = 0; i < mDivisor; i ++ )
	{
		while ( mNodes[i] != _null )
		{
			HashNode* temp = mNodes[i]; mNodes[i] = temp->mLinkNext;

			delete temp;
		}
	}

	delete[] mNodes;
}

template< typename Type, typename Key >
Hash< Type, Key >& Hash< Type, Key >::operator = ( const Hash< Type, Key >& hash )
{
	Clear( hash.mDivisor );

	for ( Node* temp = hash.mHead; temp != _null; temp = temp->mNext )
		Insert( temp->mElement );

	return *this;
}

template< typename Type, typename Key >
const Type& Hash< Type, Key >::operator [] ( const Key& key ) const
{
	Type* element = Index( key );

	FL_ASSERT( element != _null )

	return *element;
}

template< typename Type, typename Key >
Type& Hash< Type, Key >::operator [] ( const Key& key )
{
	Type* element = Index( key );

	FL_ASSERT( element != _null )

	return *element;
}

template< typename Type, typename Key >
typename Hash< Type, Key >::Iterator Hash< Type, Key >::GetIterator( const Key& key ) const
{
	if ( mNodes == _null )
		return Iterator( _null );

	// Calculate hash code from key.
	_dword hashcode = (_dword) key;

	if ( mNodes[ hashcode % mDivisor ] == _null )
		return Iterator( _null );

	HashNode* temp = mNodes[ hashcode % mDivisor ];

	// Find the node by hash code, nodes are sort ascending.
	while ( temp != _null && temp->mHashCode < hashcode )
		temp = temp->mLinkNext;

	// Can not find node.
	if ( temp == _null || temp->mHashCode != hashcode )
		return Iterator( _null );

	// There may be many elements has same key, so find the exact one.
	while ( temp != _null && (const Key&) temp->mElement != key && temp->mHashCode == hashcode )
		temp = temp->mLinkNext;

	// Can not find the exact one.
	if ( temp == _null || (const Key&) temp->mElement != key )
		return Iterator( _null );

	return Iterator( temp );
}

template< typename Type, typename Key >
Type* Hash< Type, Key >::Index( const Key& key ) const
{
	Iterator it = GetIterator( key );

	return it.mNode == _null ? _null : &( it.mNode->mElement );
}

template< typename Type, typename Key >
typename Hash< Type, Key >::Iterator Hash< Type, Key >::Insert( const Type& element )
{
	// Calculate hash code from key.
	_dword hashcode = (_dword) ( (const Key&) element );

	HashNode* temp = new HashNode;

	temp->mElement	= element;
	temp->mHashCode	= hashcode;

	// If the hash bucket has no node, put the new node as head of the hash bucket.
	if ( mNodes[ hashcode % mDivisor ] == _null )
	{
		mNodes[ hashcode % mDivisor ] = temp;
	}
	// Put the new node into the hash bucket sort ascending
	else
	{
		HashNode* temp1 = _null;
		HashNode* temp2 = mNodes[ hashcode % mDivisor ];

		// Skip the less node compare to the new one.
		while ( temp2 != _null && temp2->mHashCode <= hashcode )
			{ temp1 = temp2; temp2 = temp2->mLinkNext; }

		// Relink the hash bucket.
		temp->mLinkPrev = temp1;
		temp->mLinkNext = temp2;

		if ( temp2 != _null )
			temp2->mLinkPrev = temp;

		if ( temp1 != _null )
			temp1->mLinkNext = temp;
		else
			mNodes[ hashcode % mDivisor ] = temp;
	}

	Link< Type, Key >::InsertTail( temp );

	return Iterator( temp );
}

template< typename Type, typename Key >
_bool Hash< Type, Key >::Remove( Iterator& iterator )
{
	if ( iterator.Valid( ) == _false )
		return _false;

	// Calculate hash code from key.
	_dword hashcode = (_dword) (const Key&) ( (const Type&) iterator );

	if ( mNodes[ hashcode % mDivisor ] == _null )
		return _false;

	HashNode* tempnode = (HashNode*) iterator.mNode;

	if ( tempnode->mLinkNext != _null )
		tempnode->mLinkNext->mLinkPrev = tempnode->mLinkPrev;

	if ( tempnode->mLinkPrev != _null )
		tempnode->mLinkPrev->mLinkNext = tempnode->mLinkNext;
	else
		mNodes[ hashcode % mDivisor ] = tempnode->mLinkNext;

	// Remove from hash link.
	iterator.mNode = _null;
	Link< Type, Key >::Remove( tempnode );

	delete tempnode;

	return _true;
}

template< typename Type, typename Key >
_bool Hash< Type, Key >::Remove( const Key& key )
{
	// Calculate hash code from key.
	_dword hashcode = (_dword) key;

	if ( mNodes[ hashcode % mDivisor ] == _null )
		return _false;

	HashNode* temp1 = _null;
	HashNode* temp2 = mNodes[ hashcode % mDivisor ];

	// Find the node by hash code, nodes are sort ascending.
	while ( temp2 != _null && temp2->mHashCode < hashcode )
		{ temp1 = temp2; temp2 = temp2->mLinkNext; }

	// Can not find node.
	if ( temp2 == _null || temp2->mHashCode != hashcode )
		return _false;

	// There may be many elements has same key, so find the exact one.
	while ( temp2 != _null && (const Key&) temp2->mElement != key && temp2->mHashCode == hashcode )
		{ temp1 = temp2; temp2 = temp2->mLinkNext; }

	// Can not find the exact one.
	if ( temp2 == _null || (const Key&) temp2->mElement != key )
		return _false;

	if ( temp2->mLinkNext != _null )
		temp2->mLinkNext->mLinkPrev = temp2->mLinkPrev;

	// Relink the hash bucket.
	if ( temp1 != _null )
		temp1->mLinkNext = temp2->mLinkNext;
	else
		mNodes[ hashcode % mDivisor ] = temp2->mLinkNext;

	// Remove from hash link.
	Link< Type, Key >::Remove( temp2 );

	delete temp2;

	return _true;
}

template< typename Type, typename Key >
_void Hash< Type, Key >::Clear( )
{
	Clear( mDivisor );
}

template< typename Type, typename Key >
_void Hash< Type, Key >::Clear( _dword divisor )
{
	// Delete all hash node.
	for ( _dword i = 0; i < mDivisor; i ++ )
	{
		while ( mNodes[i] != _null )
		{
			HashNode* temp = mNodes[i]; mNodes[i] = temp->mLinkNext;

			delete temp;
		}
	}

	delete[] mNodes;

	mDivisor = divisor;
	mNodes	 = new HashNode*[ mDivisor ];

	for ( _dword j = 0; j < mDivisor; j ++ )
		mNodes[j] = _null;

	Link< Type, Key >::Clear( );
}

};