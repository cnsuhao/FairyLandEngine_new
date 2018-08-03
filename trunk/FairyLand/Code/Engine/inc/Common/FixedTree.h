//============================================================================
// FixedTree.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// FixedTree
//----------------------------------------------------------------------------

template< typename Type, _dword _MAX_CHILD_NUMBER > class FixedTree
{
private:
	struct TreeNode
	{
		Type		mElement;
		TreeNode*	mParent;
		TreeNode*	mChildList[ _MAX_CHILD_NUMBER ];

		TreeNode( const Type& element, TreeNode* parent = _null ) : mElement( element ), mParent( parent )
		{
			for ( _dword i = 0; i < _MAX_CHILD_NUMBER; i ++ )
				mChildList[i] = _null;
		}
	};

	_dword		mNumber;
	TreeNode*	mRootNode;

public:
	class Iterator
	{
	public:
		TreeNode*	mNode;

		Iterator( TreeNode* node ) : mNode( node ) { }

		_bool Valid( ) const
			{ return mNode != _null; }

		operator Type& ( )
			{ return mNode->mElement; }

		Iterator GetParent( ) const
		{
			FL_ASSERT( mNode != _null )

			return Iterator( mNode->mParent );
		}

		Iterator GetChild( _dword index ) const
		{
			FL_ASSERT( mNode != _null )
			FL_ASSERT( index < _MAX_CHILD_NUMBER )

			return Iterator( mNode->mChildList[ index ] );
		}
	};

	_void ClearHelper( TreeNode*& node );

public:
	FixedTree( );
	~FixedTree( );

	inline _dword Number( ) const;
	inline Iterator GetRootIterator( ) const;

	_void SetRoot( const Type& element );
	_void Insert( const Type& element, const Iterator& iterator, _dword index );
	_void Remove( Iterator& iterator );

	_void Clear( );
};

//----------------------------------------------------------------------------
// FixedTree Implementation
//----------------------------------------------------------------------------

template< typename Type, _dword _MAX_CHILD_NUMBER >
FixedTree< Type, _MAX_CHILD_NUMBER >::FixedTree( ) : mNumber( 0 ), mRootNode( _null )
{
}

template< typename Type, _dword _MAX_CHILD_NUMBER >
FixedTree< Type, _MAX_CHILD_NUMBER >::~FixedTree( )
{
	Clear( );
}

template< typename Type, _dword _MAX_CHILD_NUMBER >
_void FixedTree< Type, _MAX_CHILD_NUMBER >::ClearHelper( TreeNode*& node )
{
	if ( node == _null )
		return;

	for ( _dword i = 0; i < _MAX_CHILD_NUMBER; i ++ )
		ClearHelper( node->mChildList[i] );

	FL_ASSERT( mNumber > 0 )

	delete node;
	node = _null;

	mNumber --;
}

template< typename Type, _dword _MAX_CHILD_NUMBER >
_dword FixedTree< Type, _MAX_CHILD_NUMBER >::Number( ) const
{
	return mNumber;
}

template< typename Type, _dword _MAX_CHILD_NUMBER >
typename FixedTree< Type, _MAX_CHILD_NUMBER >::Iterator FixedTree< Type, _MAX_CHILD_NUMBER >::GetRootIterator( ) const
{
	return Iterator( mRootNode );
}

template< typename Type, _dword _MAX_CHILD_NUMBER >
_void FixedTree< Type, _MAX_CHILD_NUMBER >::SetRoot( const Type& element )
{
	FL_ASSERT( mRootNode == _null )

	mRootNode = new TreeNode( element );

	mNumber ++;
}

template< typename Type, _dword _MAX_CHILD_NUMBER >
_void FixedTree< Type, _MAX_CHILD_NUMBER >::Insert( const Type& element, const Iterator& iterator, _dword index )
{
	FL_ASSERT( iterator.mNode != _null )
	FL_ASSERT( index < _MAX_CHILD_NUMBER )

	if ( iterator.mNode->mChildList[ index ] != _null )
		ClearHelper( iterator.mNode->mChildList[ index ] );

	iterator.mNode->mChildList[ index ] = new TreeNode( element, iterator.mNode );

	mNumber ++;
}

template< typename Type, _dword _MAX_CHILD_NUMBER >
_void FixedTree< Type, _MAX_CHILD_NUMBER >::Remove( Iterator& iterator )
{
	FL_ASSERT( iterator.mNode != _null )

	TreeNode* parent = iterator.mNode->mParent;

	if ( parent == _null )
	{
		mRootNode = _null;
	}
	else
	{
		for ( _dword i = 0; i < _MAX_CHILD_NUMBER; i ++ )
		{
			if ( parent->mChildList[i] == iterator.mNode )
			{
				parent->mChildList[i] = _null;
				break;
			}
		}
	}

	ClearHelper( iterator.mNode );
}

template< typename Type, _dword _MAX_CHILD_NUMBER >
_void FixedTree< Type, _MAX_CHILD_NUMBER >::Clear( )
{
	ClearHelper( mRootNode );
}

};