//============================================================================
// Stack.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Stack
//----------------------------------------------------------------------------

//! This class is template container class, represents a stack.

//! A stack allocates a buffer to store all elements, the buffer will auto enlarge to hold more elements.
//! Each elements are adjacent in memory, can be accessed by index base from zero.<br>
//! Template parameter "Type" defines the element type storaged in the stack.<br>
//! Template parameter "Key" defines the comparable type of the element type.<br>
//! For example, Stack< ClassA, _dword > defines a stack stores ClassA element, and access it
//! by _dword, means ClassA has a overload operator with _dword ( ==, !=, > and < etc ), or it can convert to _dword.
template< typename Type, typename Key = Type > class Stack
{
private:
	//! The initial size of the stack in number of element.
	_dword	mInitialSize;
	//! The alloced size of the stack in number of element.
	_dword	mAllocedSize;
	//! The initial grow size of the array in number of element.
	_dword	mInitialGrow;
	//! The alloced grow size of the array in number of element.
	_dword	mAllocedGrow;
	//! The current number of elements in stack.
	_dword	mNumber;
	//! The memory pointer of the stack.
	Type*	mElements;

public:
	//! The iterator of stack class, provides ++ and -- operator to walk through the stack.
	class Iterator
	{
	public:
		//! The stack reference.
		Stack< Type, Key >*	mStack;
		//! The current index of the element.
		_dword				mIndex;

		//! Constructor, create an iterator of the stack.
		//! @param		stack	The stack reference.
		//! @param		index		The current index of the element.
		Iterator( Stack< Type, Key >* stack, _dword index )
			: mStack( stack ), mIndex( index ) { }

		//! Type conversion, get the element reference.
		//! @param		none
		//! @return		Reference of the element.
		operator Type& ( )
			{ return (*mStack)[ mIndex ]; }

		//! Increase the iterator, point to the next element of the stack.
		//! @param		none
		_void operator ++ ( _long )
			{ mIndex ++; }
		//! decrease the iterator, point to the previous element of the stack.
		//! @param		none
		_void operator -- ( _long )
			{ mIndex --; }

		//! Check the iterator if it is valid ( points to an available element ).
		//! @param		none
		//! @return		True if the iterator is valid, false otherwise.
		_bool Valid( ) const
			{ return mIndex < mStack->Number( ); }
	};

	//! Constructor, create an empty stack.
	//! @param		size		The initial size of the stack in number of element.
	//! @param		grow		The initial grow size of the stack in number of element.
	Stack( _dword size = 20, _dword grow = 10 );
	//! Copy-Constructor, create a stack by copy from another one.
	//! @param		stack	The other stack.
	Stack( const Stack< Type, Key >& stack );
	//! Destructor, delete the stack, and release the elements memory.
	//! @param		none
	~Stack( );

	//! Copy elements from another stack, and recreates memory to hold it.
	//! @param		stack	The other stack.
	//! @return		The reference of current stack.
	Stack< Type, Key >& operator = ( const Stack< Type, Key >& stack );

	//! Get a const element reference by index.
	//! @param		index		The index of the element in stack.
	//! @return		The const element reference.
	inline const Type& operator [] ( _dword index ) const;
	//! Get a mutable element reference by index.
	//! @param		index		The index of the element in stack.
	//! @return		The mutable element reference.
	inline Type& operator [] ( _dword index );

	//! Get the current number of elements in stack.
	//! @param		none
	//! @return		The current number of elements in stack.
	inline _dword Number( ) const;

	//! Get the top element in stack.
	//! @param		none
	//! @return		The element reference of the top element.
	inline Type& Top( ) const;

	//! Get the head iterator.
	//! @param		none
	//! @return		The head iterator.
	inline Iterator GetHeadIterator( ) const;
	//! Get the tail iterator.
	//! @param		none
	//! @return		The tail iterator.
	inline Iterator GetTailIterator( ) const;

	//! Get an iterator by the index of an element.
	//! @param		index		The index of the element in stack.
	//! @return		An iterator reference to the element.
	inline Iterator GetIterator( _dword index ) const;

	//! Push an element into stack.
	//! @param		element		The new element to be push into.
	_void Push( const Type& element );
	//! Pop an element from stack.
	//! @param		none
	_void Pop( );
	//! Resize the stack by create new memory and copy old element into it, the memory is larger than older by grow size.
	//! @param		none
	_void Resize( );

	//! Clear the stack, delete all elements and free the memory.
	//! @param		freememory	If free the memory used by array.
	_void Clear( _bool freememory = _false );
	//! Clear the stack, delete all elements and free the memory, set the initial size and the grow size.
	//! @param		size		The initial size of the stack in number of element.
	//! @param		grow		The grow size of the stack in number of element.
	_void Clear( _dword size, _dword grow );
};

//----------------------------------------------------------------------------
// Stack Implementation
//----------------------------------------------------------------------------

template< typename Type, typename Key >
Stack< Type, Key >::Stack( _dword size, _dword grow )
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
Stack< Type, Key >::Stack( const Stack< Type, Key >& stack )
{
	mInitialSize	= stack.mInitialSize;
	mAllocedSize	= stack.mAllocedSize;
	mInitialGrow	= stack.mInitialGrow;
	mAllocedGrow	= stack.mAllocedGrow;
	mNumber			= stack.mNumber;
	mElements		= _null;

	if ( mAllocedSize > 0 )
	{
		mElements	= new Type[ mAllocedSize ];

		// Copy elements one by one, execute opertor = of Type class.
		for ( _dword i = 0; i < mNumber; i ++ )
			mElements[i] = stack.mElements[i];
	}
}

template< typename Type, typename Key >
Stack< Type, Key >::~Stack( )
{
	// Release stack memory.
	delete[] mElements;
}

template< typename Type, typename Key >
Stack< Type, Key >& Stack< Type, Key >::operator = ( const Stack< Type, Key >& stack )
{
	// Release old memory.
	PS_DELETEARRAY( mElements );

	mInitialSize	= stack.mInitialSize;
	mAllocedSize	= stack.mAllocedSize;
	mInitialGrow	= stack.mInitialGrow;
	mAllocedGrow	= stack.mAllocedGrow;
	mNumber			= stack.mNumber;

	if ( mAllocedSize > 0 )
		mElements = new Type[ mAllocedSize ];

	// Copy elements one by one, execute opertor = of Type class.
	for ( _dword i = 0; i < mNumber; i ++ )
		mElements[i] = stack.mElements[i];

	return *this;
}

template< typename Type, typename Key >
const Type& Stack< Type, Key >::operator [] ( _dword index ) const
{
	FL_ASSERT( index < mNumber )

	return mElements[ index ];
}

template< typename Type, typename Key >
Type& Stack< Type, Key >::operator [] ( _dword index )
{
	FL_ASSERT( index < mNumber )

	return mElements[ index ];
}

template< typename Type, typename Key >
_dword Stack< Type, Key >::Number( ) const
{
	return mNumber;
}

template< typename Type, typename Key >
Type& Stack< Type, Key >::Top( ) const
{
	FL_ASSERT( mNumber > 0 )

	return mElements[ mNumber - 1 ];
}

template< typename Type, typename Key >
typename Stack< Type, Key >::Iterator Stack< Type, Key >::GetHeadIterator( ) const
{
	return Iterator( (Stack< Type, Key >*) this, 0 );
}

template< typename Type, typename Key >
typename Stack< Type, Key >::Iterator Stack< Type, Key >::GetTailIterator( ) const
{
	return Iterator( (Stack< Type, Key >*) this, mNumber - 1 );
}

template< typename Type, typename Key >
typename Stack< Type, Key >::Iterator Stack< Type, Key >::GetIterator( _dword index ) const
{
	return Iterator( (Stack< Type, Key >*) this, index );
}

template< typename Type, typename Key >
_void Stack< Type, Key >::Push( const Type& element )
{
	// Resize the stack when necessary.
	if ( mNumber == mAllocedSize )
		Resize( );

	// Assign the element.
	mElements[ mNumber ] = element;

	// Increase the element number.
	mNumber ++;
}

template< typename Type, typename Key >
_void Stack< Type, Key >::Pop( )
{
	FL_ASSERT( mNumber > 0 )

	// Decrease the element number.
	mNumber --;
}

template< typename Type, typename Key >
_void Stack< Type, Key >::Resize( )
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
_void Stack< Type, Key >::Clear( _bool freememory )
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
_void Stack< Type, Key >::Clear( _dword size, _dword grow )
{
	delete[] mElements;

	mInitialSize	= size;
	mAllocedSize	= size;
	mInitialGrow	= grow;
	mAllocedGrow	= grow;
	mNumber			= 0;
	mElements		= new Type[ mInitialSize ];
}

};