//============================================================================
// List.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// List
//----------------------------------------------------------------------------

//! This class is template container class, represents a list.

//! A list allocates all nodes from heap to store elements, and link them by a chain<br>
//! Template parameter "Type" defines the element type storaged in the list.<br>
//! Template parameter "Key" defines the comparable type of the element type.<br>
//! For example, List< ClassA, _dword > defines a list stores ClassA element, and access it
//! by _dword, means ClassA has a overload operator with _dword ( ==, !=, > and < etc ), or it can convert to _dword.
template< typename Type, typename Key = Type > class List : public Link< Type, Key >
{
public:
	//! Constructor, create an empty list.
	//! @param		none
	List( );
	//! Copy-Constructor, create a list by copy from another one.
	//! @param		list		The other list.
	List( const List< Type, Key >& list );
	//! Destructor, delete the list, and release the memory used by nodes.
	//! @param		none
	~List( );

	//! Copy elements from another list, and allocates nodes to hold it.
	//! @param		list		The other list.
	//! @return		The reference of current list.
	List< Type, Key >& operator = ( const List< Type, Key >& list );

	//! Insert an element into the list at head.
	//! @param		element		The element to be inserted.
	_void InsertHead( const Type& element );
	//! Insert an element into the list at tail.
	//! @param		element		The element to be inserted.
	_void InsertTail( const Type& element );
	//! Insert an element into the list before the position specified by an iterator.
	//! @param		element		The element to be inserted.
	_void InsertBefore( const Type& element, const Iterator& iterator );
	//! Insert an element into the list after the position specified by an iterator.
	//! @param		element		The element to be inserted.
	_void InsertAfter( const Type& element, const Iterator& iterator );
	//! Remove an element from the list, the element is specified by an iterator.
	//! @param		iterator	The iterator specifies a position.
	//! @return		
	_void Remove( Iterator& iterator );

	//! Clear list and delete all memory used by nodes.
	//! @param		none
	_void Clear( );
};

//----------------------------------------------------------------------------
// List Implementation
//----------------------------------------------------------------------------

template< typename Type, typename Key >
List< Type, Key >::List( )
{
}

template< typename Type, typename Key >
List< Type, Key >::List( const List< Type, Key >& list )
{
	for ( Node* temp = list.mHead; temp != _null; temp = temp->mNext )
		InsertTail( temp->mElement );
}

template< typename Type, typename Key >
List< Type, Key >::~List( )
{
	Clear( );
}

template< typename Type, typename Key >
List< Type, Key >& List< Type, Key >::operator = ( const List< Type, Key >& list )
{
	Clear( );

	for ( Node* temp = list.mHead; temp != _null; temp = temp->mNext )
		InsertTail( temp->mElement );

	return *this;
}

template< typename Type, typename Key >
_void List< Type, Key >::InsertHead( const Type& element )
{
	Link< Type, Key >::InsertHead( new Node( element ) );
}

template< typename Type, typename Key >
_void List< Type, Key >::InsertTail( const Type& element )
{
	Link< Type, Key >::InsertTail( new Node( element ) );
}

template< typename Type, typename Key >
_void List< Type, Key >::InsertBefore( const Type& element, const Iterator& iterator )
{
	Link< Type, Key >::InsertBefore( new Node( element ), iterator );
}

template< typename Type, typename Key >
_void List< Type, Key >::InsertAfter( const Type& element, const Iterator& iterator )
{
	Link< Type, Key >::InsertAfter( new Node( element ), iterator );
}

template< typename Type, typename Key >
_void List< Type, Key >::Remove( Iterator& iterator )
{
	FL_ASSERT( iterator.Valid( ) )

	Link< Type, Key >::Remove( iterator.mNode );

	delete iterator.mNode;
	iterator.mNode = _null;
}

template< typename Type, typename Key >
_void List< Type, Key >::Clear( )
{
	while ( mHead != _null )
	{
		Node* temp = mHead; mHead = temp->mNext;

		delete temp;
	}

	Link< Type, Key >::Clear( );
}

};