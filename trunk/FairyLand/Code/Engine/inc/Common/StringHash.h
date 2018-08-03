//============================================================================
// StringHash.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// StringHash
//----------------------------------------------------------------------------

//! This class is template container class, represents a hash table.
template< typename Type > class StringHash : public Link< Type, StringPtr >
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

		//! The hash code of the node, used for check.
		_dword		mHashCodeEx1;
		//! The hash code of the node, used for check.
		_dword		mHashCodeEx2;

		HashNode( ) : mLinkPrev( _null ), mLinkNext( _null ), mHashCode( -1 ), mHashCodeEx1( -1 ), mHashCodeEx2( -1 ) { }
	};

	//! Copy-Constructor, create a hash table by copy from another one, disabled.
	StringHash( const StringHash< Type >& );
	//! Copy elements from another hash table, disabled.
	StringHash< Type >& operator = ( const StringHash< Type >& );

	//! The memory pointer of the hash buckets.
	HashNode**	mNodes;
	//! The divisor of the hash table, used to find a hash bucket.
	_dword		mDivisor;

public:
	//! Constructor, create an empty hash table.
	//! @param		divisor		The divisor of the hash table.
	StringHash( _dword divisor );
	//! Destructor, delete the hash table, and release the memory used by nodes.
	//! @param		none
	~StringHash( );

	//! Get a const element reference by element key.
	//! @param		key			The key of element, used to calculate hash code.
	//! @return		The const element reference.
	inline const Type& operator [] ( StringPtr key ) const;
	//! Get a mutable element reference by element key.
	//! @param		key			The key of element, used to calculate hash code.
	//! @return		The mutable element reference.
	inline Type& operator [] ( StringPtr key );

	//! Get an iterator by the key of an element.
	//! @param		key			The key of element, used to calculate hash code.
	//! @return		An iterator reference to the element.
	Iterator GetIterator( StringPtr key ) const;
	//! Get an iterator by the key of an element.
	//! @param		hashcode1	The hash code of the element.
	//! @param		hashcode2	The hash code of the element.
	//! @param		hashcode3	The hash code of the element.
	//! @return		An iterator reference to the element.
	Iterator GetIterator( _dword hashcode1, _dword hashcode2, _dword hashcode3 ) const;

	//! Get an element pointer by element key.
	//! @param		key			The key of element, used to calculate hash code.
	//! @return		The pointer to a element if the index is valid, null otherwise.
	//! @remarks	This is a safe way to get an element by index from the hash.
	Type* Index( StringPtr key ) const;
	//! Get an element pointer by element key.
	//! @param		hashcode1	The hash code of the element.
	//! @param		hashcode2	The hash code of the element.
	//! @param		hashcode3	The hash code of the element.
	//! @return		The pointer to a element if the index is valid, null otherwise.
	//! @remarks	This is a safe way to get an element by index from the hash.
	Type* Index( _dword hashcode1, _dword hashcode2, _dword hashcode3 ) const;

	//! Insert an element into the hash table by hash code that calculated from element key.
	//! @param		element		The new element to be inserted into.
	//! @param		key			The key of element, used to calculate hash code.
	//! @return		An iterator reference to the new element.
	Iterator Insert( const Type& element, StringPtr key );
	//! Insert an element into the hash table by hash code that calculated from element key.
	//! @param		element		The new element to be inserted into.
	//! @param		hashcode1	The hash code of the element.
	//! @param		hashcode2	The hash code of the element.
	//! @param		hashcode3	The hash code of the element.
	//! @return		An iterator reference to the new element.
	Iterator Insert( const Type& element, _dword hashcode1, _dword hashcode2, _dword hashcode3 );
	//! Remove an element from the hash table, the element is specified by an iterator.
	//! @param		iterator	The iterator specifies a position.
	_bool Remove( Iterator& iterator );
	//! Remove an element from the hash table, the element is specified by element key.
	//! @param		key			The key of element, used to calculate hash code.
	_bool Remove( StringPtr key );
	//! Remove an element from the hash table, the element is specified by element key.
	//! @param		hashcode1	The hash code of the element.
	//! @param		hashcode2	The hash code of the element.
	//! @param		hashcode3	The hash code of the element.
	_bool Remove( _dword hashcode1, _dword hashcode2, _dword hashcode3 );

	//! Clear hash table and delete all memory used by nodes.
	//! @param		none
	_void Clear( );
};

//----------------------------------------------------------------------------
// StringHash Implementation
//----------------------------------------------------------------------------

template< typename Type >
StringHash< Type >::StringHash( _dword divisor )
{
	FL_ASSERT( divisor > 0 )

	mDivisor	= divisor;
	mNodes		= _null;
}

template< typename Type >
StringHash< Type >::~StringHash( )
{
	Clear( );
}

template< typename Type >
const Type& StringHash< Type >::operator [] ( StringPtr key ) const
{
	Type* element = Index( key );

	FL_ASSERT( element != _null )

	return *element;
}

template< typename Type >
Type& StringHash< Type >::operator [] ( StringPtr key )
{
	Type* element = Index( key );

	FL_ASSERT( element != _null )

	return *element;
}

template< typename Type >
typename StringHash< Type >::Iterator StringHash< Type >::GetIterator( StringPtr key ) const
{
	// Calculate hash code from key.
	_dword hashcode1 = key.HashCode1( );
	_dword hashcode2 = key.HashCode2( );
	_dword hashcode3 = key.HashCode3( );

	return GetIterator( hashcode1, hashcode2, hashcode3 );
}

template< typename Type >
typename StringHash< Type >::Iterator StringHash< Type >::GetIterator( _dword hashcode1, _dword hashcode2, _dword hashcode3 ) const
{
	if ( mNodes == _null )
		return Iterator( _null );

	if ( mNodes[ hashcode1 % mDivisor ] == _null )
		return Iterator( _null );

	HashNode* tempnode = mNodes[ hashcode1 % mDivisor ];

	// Find the node by hash code, nodes are sort ascending.
	while ( tempnode != _null && tempnode->mHashCode < hashcode1 )
		tempnode = tempnode->mLinkNext;

	// Can not find node.
	if ( tempnode == _null || tempnode->mHashCode != hashcode1 )
		return Iterator( _null );

	// There may be many elements has same key, so find the exact one.
	while ( tempnode != _null && tempnode->mHashCode == hashcode1 && ( tempnode->mHashCodeEx1 != hashcode2 || tempnode->mHashCodeEx2 != hashcode3 ) )
		tempnode = tempnode->mLinkNext;

	// There may be many elements has same key, so find the exact one.
	while ( tempnode == _null || tempnode->mHashCode != hashcode1 || tempnode->mHashCodeEx1 != hashcode2 || tempnode->mHashCodeEx2 != hashcode3 )
		return Iterator( _null );

	return Iterator( tempnode );
}

template< typename Type >
Type* StringHash< Type >::Index( StringPtr key ) const
{
	Iterator iterator = GetIterator( key );

	if ( iterator.Valid( ) == _false )
		return _null;

	HashNode* tempnode = (HashNode*) iterator.mNode;

	return &( tempnode->mElement );
}

template< typename Type >
Type* StringHash< Type >::Index( _dword hashcode1, _dword hashcode2, _dword hashcode3 ) const
{
	Iterator iterator = GetIterator( hashcode1, hashcode2, hashcode3 );

	if ( iterator.Valid( ) == _false )
		return _null;

	HashNode* tempnode = (HashNode*) iterator.mNode;

	return &( tempnode->mElement );
}

template< typename Type >
typename StringHash< Type >::Iterator StringHash< Type >::Insert( const Type& element, StringPtr key )
{
	return Insert( element, key.HashCode1( ), key.HashCode2( ), key.HashCode3( ) );
}

template< typename Type >
typename StringHash< Type >::Iterator StringHash< Type >::Insert( const Type& element, _dword hashcode1, _dword hashcode2, _dword hashcode3 )
{
	if ( mNodes == _null )
	{
		mNodes = new HashNode*[ mDivisor ];

		for ( _dword i = 0; i < mDivisor; i ++ )
			mNodes[i] = _null;
	}

	HashNode* temp = new HashNode;

	temp->mElement		= element;
	temp->mHashCode		= hashcode1;
	temp->mHashCodeEx1	= hashcode2;
	temp->mHashCodeEx2	= hashcode3;

	// If the hash bucket has no node, put the new node as head of the hash bucket.
	if ( mNodes[ hashcode1 % mDivisor ] == _null )
	{
		mNodes[ hashcode1 % mDivisor ] = temp;
	}
	// Put the new node into the hash bucket sort ascending
	else
	{
		HashNode* temp1 = _null;
		HashNode* temp2 = mNodes[ hashcode1 % mDivisor ];

		// Skip the less node compare to the new one.
		while ( temp2 != _null && temp2->mHashCode <= hashcode1 )
			{ temp1 = temp2; temp2 = temp2->mLinkNext; }

		// Relink the hash bucket.
		temp->mLinkPrev = temp1;
		temp->mLinkNext = temp2;

		if ( temp2 != _null )
			temp2->mLinkPrev = temp;

		if ( temp1 != _null )
			temp1->mLinkNext = temp;
		else
			mNodes[ hashcode1 % mDivisor ] = temp;
	}

	Link< Type, StringPtr >::InsertTail( temp );

	return Iterator( temp );
}

template< typename Type >
_bool StringHash< Type >::Remove( Iterator& iterator )
{
	if ( iterator.Valid( ) == _false )
		return _false;

	HashNode* tempnode = (HashNode*) iterator.mNode;

	if ( mNodes[ tempnode->mHashCode % mDivisor ] == _null )
		return _false;

	if ( tempnode->mLinkNext != _null )
		tempnode->mLinkNext->mLinkPrev = tempnode->mLinkPrev;

	if ( tempnode->mLinkPrev != _null )
		tempnode->mLinkPrev->mLinkNext = tempnode->mLinkNext;
	else
		mNodes[ tempnode->mHashCode % mDivisor ] = tempnode->mLinkNext;

	// Remove from hash link.
	iterator.mNode = _null;
	Link< Type, StringPtr >::Remove( tempnode );

	delete tempnode;

	return _true;
}

template< typename Type >
_bool StringHash< Type >::Remove( StringPtr key )
{
	Iterator iterator = GetIterator( key );

	if ( iterator.Valid( ) == _false )
		return _false;

	return Remove( iterator );
}

template< typename Type >
_bool StringHash< Type >::Remove( _dword hashcode1, _dword hashcode2, _dword hashcode3 )
{
	Iterator iterator = GetIterator( hashcode1, hashcode2, hashcode3 );

	if ( iterator.Valid( ) == _false )
		return _false;

	return Remove( iterator );
}

template< typename Type >
_void StringHash< Type >::Clear( )
{
	if ( mNodes != _null )
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

	Link< Type, StringPtr >::Clear( );
}

};