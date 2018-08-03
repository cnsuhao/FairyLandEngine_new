//============================================================================
// Tree.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Tree
//----------------------------------------------------------------------------

template< typename Type, typename Key = Type > class Tree
{
private:
	struct TreeNode
	{
		Type		mElement;
		TreeNode*	mParent;
		TreeNode*	mBrother;
		TreeNode*	mChild;

		TreeNode( const Type& element, TreeNode* parent = _null )
			: mElement( element ), mParent( parent ), mBrother( _null ), mChild( _null ) { }
	};

	_dword		mNumber;
	TreeNode*	mRootNode;

	_void ClearHelper( TreeNode* node );

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

		Iterator GetBrother( ) const
		{
			FL_ASSERT( mNode != _null )

			return Iterator( mNode->mBrother );
		}

		Iterator GetChild( ) const
		{
			FL_ASSERT( mNode != _null )

			return Iterator( mNode->mChild );
		}
	};

public:
	Tree( );
	~Tree( );

	inline _dword Number( ) const;

	Iterator GetRootIterator( ) const;
	Iterator SearchInChild( const Iterator& iterator, const Key& key ) const;

	Iterator SetRootNode( const Type& element );
	Iterator Insert( const Type& element, const Iterator& iterator );
	_void Remove( Iterator& iterator );

	_void Clear( );
};

//----------------------------------------------------------------------------
// Tree Implementation
//----------------------------------------------------------------------------

template< typename Type, typename Key >
Tree< Type, Key >::Tree( ) : mNumber( 0 ), mRootNode( _null )
{
}

template< typename Type, typename Key >
Tree< Type, Key >::~Tree( )
{
	Clear( );
}

template< typename Type, typename Key >
_void Tree< Type, Key >::ClearHelper( TreeNode* node )
{
	if ( node == _null )
		return;

	TreeNode* child = node->mChild;

	while ( child != _null )
	{
		TreeNode* temp = child;
		child = child->mBrother;

		// Clear the child node.
		ClearHelper( temp );
	}

	FL_ASSERT( mNumber > 0 )
	mNumber --;

	delete node;
}

template< typename Type, typename Key >
_dword Tree< Type, Key >::Number( ) const
{
	return mNumber;
}

template< typename Type, typename Key >
typename Tree< Type, Key >::Iterator Tree< Type, Key >::GetRootIterator( ) const
{
	return Iterator( mRootNode );
}

template< typename Type, typename Key >
typename Tree< Type, Key >::Iterator Tree< Type, Key >::SearchInChild( const Iterator& iterator, const Key& key ) const
{
	FL_ASSERT( iterator.mNode != _null )

	Iterator child = iterator.GetChild( );

	while ( child.Valid( ) )
	{
		if ( ( (const Key&) (const Type&) child ) == key )
			return child;

		child = child.GetBrother( );
	}

	return Iterator( _null );
}

template< typename Type, typename Key >
typename Tree< Type, Key >::Iterator Tree< Type, Key >::SetRootNode( const Type& element )
{
	FL_ASSERT( mRootNode == _null )

	mRootNode = new TreeNode( element );

	FL_ASSERT( mNumber == 0 )
	mNumber ++;

	return Iterator( mRootNode );
}

template< typename Type, typename Key >
typename Tree< Type, Key >::Iterator Tree< Type, Key >::Insert( const Type& element, const Iterator& iterator )
{
	FL_ASSERT( iterator.mNode != _null )

	TreeNode* node = new TreeNode( element, iterator.mNode );

	if ( iterator.mNode->mChild == _null )
	{
		iterator.mNode->mChild = node;
	}
	else
	{
		TreeNode* child = iterator.mNode->mChild;

		while ( child->mBrother != _null )
			child = child->mBrother;

		child->mBrother = node;
	}

	mNumber ++;

	return Iterator( node );
}

template< typename Type, typename Key >
_void Tree< Type, Key >::Remove( Iterator& iterator )
{
	FL_ASSERT( iterator.mNode != _null )

	TreeNode* parent = iterator.mNode->mParent;

	if ( parent == _null )
	{
		mRootNode = _null;
	}
	else if ( parent->mChild == iterator.mNode )
	{
		parent->mChild = iterator.mNode->mBrother;
	}
	else
	{
		TreeNode* child = parent->mChild;

		while ( child != _null )
		{
			if ( child->mBrother == iterator.mNode )
			{
				child->mBrother = iterator.mNode->mBrother;
				break;
			}

			child = child->mBrother;
		}
	}

	ClearHelper( iterator.mNode );
	iterator.mNode = _null;
}

template< typename Type, typename Key >
_void Tree< Type, Key >::Clear( )
{
	ClearHelper( mRootNode );
	mRootNode = _null;

	FL_ASSERT( mNumber == 0 )
}

};