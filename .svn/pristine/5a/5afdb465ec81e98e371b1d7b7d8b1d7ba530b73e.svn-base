//============================================================================
// FixedAllocater.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// FixedAllocater
//----------------------------------------------------------------------------

template< typename Type, _dword _MAX_POOL_SIZE > class FixedAllocater
{
private:
	struct AllocUnit
	{
		_void*	mPoolPointer;
		_dword	mPoolIndex;
		Type	mElement;
	};

	typedef Pool< AllocUnit >		AllocatePool;
	typedef Array< AllocatePool* >	AllocatePoolArray;

	AllocatePoolArray	mFullPoolArray;
	AllocatePoolArray	mFreePoolArray;

	AllocatePool* GetAvailablePool( );

public:
	FixedAllocater( );
	~FixedAllocater( );

	Type* Allocate( );
	_void Free( Type* object );
};

//----------------------------------------------------------------------------
// FixedTree Implementation
//----------------------------------------------------------------------------

template< typename Type, _dword _MAX_POOL_SIZE >
FixedAllocater< Type, _MAX_POOL_SIZE >::FixedAllocater( )
{
}

template< typename Type, _dword _MAX_POOL_SIZE >
FixedAllocater< Type, _MAX_POOL_SIZE >::~FixedAllocater( )
{
	for ( _dword i = 0; i < mFullPoolArray.Number( ); i ++ )
		delete mFullPoolArray[i];

	for ( _dword i = 0; i < mFreePoolArray.Number( ); i ++ )
		delete mFreePoolArray[i];
}

template< typename Type, _dword _MAX_POOL_SIZE >
typename FixedAllocater< Type, _MAX_POOL_SIZE >::AllocatePool* FixedAllocater< Type, _MAX_POOL_SIZE >::GetAvailablePool( )
{
	if ( mFreePoolArray.Number( ) > 0 )
		return mFreePoolArray[0];

	AllocatePool* newpool = new AllocatePool( _MAX_POOL_SIZE );
	mFreePoolArray.Append( newpool );

	return newpool;
}

template< typename Type, _dword _MAX_POOL_SIZE >
Type* FixedAllocater< Type, _MAX_POOL_SIZE >::Allocate( )
{
	AllocatePool* pool = GetAvailablePool( );
	FL_ASSERT( pool != _null )

	_dword index = pool->Create( AllocUnit( ) );
	FL_ASSERT( index != -1 )

	if ( pool->Size( ) == pool->Number( ) )
	{
		mFreePoolArray.Remove( mFreePoolArray.Search( pool ).mIndex );
		mFullPoolArray.Append( pool );
	}

	AllocUnit& allocunit = (*pool)[ index ];
	allocunit.mPoolPointer	= pool;
	allocunit.mPoolIndex	= index;

	return &allocunit.mElement;
}

template< typename Type, _dword _MAX_POOL_SIZE >
_void FixedAllocater< Type, _MAX_POOL_SIZE >::Free( Type* object )
{
	AllocUnit* allocunit = (AllocUnit*) ( (_byte*) object + sizeof( Type ) - sizeof( AllocUnit ) );

	AllocatePool* pool = (AllocatePool*) allocunit->mPoolPointer;
	FL_ASSERT( pool != _null )

	if ( pool->Size( ) == pool->Number( ) )
	{
		mFullPoolArray.Remove( mFullPoolArray.Search( pool ).mIndex );
		mFreePoolArray.Append( pool );
	}

	pool->Remove( allocunit->mPoolIndex );
}

};