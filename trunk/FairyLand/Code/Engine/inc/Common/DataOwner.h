//============================================================================
// DataOwner.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// DataOwner
//----------------------------------------------------------------------------

template< typename Type > class DataOwner
{
private:
	Type&	mDataRef;
	Type	mDataCopy;

public:
	DataOwner( Type& data );
	~DataOwner( );
};

//----------------------------------------------------------------------------
// DataOwner Implementation
//----------------------------------------------------------------------------

template< typename Type >
DataOwner< Type >::DataOwner( Type& data ) : mDataRef( data ), mDataCopy( data )
{
}

template< typename Type >
DataOwner< Type >::~DataOwner( )
{
	mDataRef = mDataCopy;
}

};