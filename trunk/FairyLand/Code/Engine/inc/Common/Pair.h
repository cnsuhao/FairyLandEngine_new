//============================================================================
// Pair.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Pair
//----------------------------------------------------------------------------

//! This class is template container class, represents a 2-element structure.

//! Template parameter "Type1" defines the first element type.<br>
//! Template parameter "Type2" defines the second element type.<br>
//! A comparision of pair only care about the first element type, the <b>Key<b> type.
template< typename Type1, typename Type2 > class Pair
{
public:
	//! The first element.
	Type1	mObject1;
	//! The second element.
	Type2	mObject2;

	//! Constructor, create a pair without assigning of elements.
	//! @param		none
	inline Pair( );
	//! Constructor, create a pair and assign elements.
	//! @param		none
	inline Pair( const Type1& object1, const Type2& object2 );

	//! Type conversion, get the first object.
	//! @param		none
	//! @return		The const reference of the first object.
	inline operator const Type1& ( ) const;
};

//----------------------------------------------------------------------------
// Pair Implementation
//----------------------------------------------------------------------------

template< typename Type1, typename Type2 >
Pair< Type1, Type2 >::Pair( )
{
}

template< typename Type1, typename Type2 >
Pair< Type1, Type2 >::Pair( const Type1& object1, const Type2& object2 )
	: mObject1( object1 ), mObject2( object2 )
{
}

template< typename Type1, typename Type2 >
Pair< Type1, Type2 >::operator const Type1& ( ) const
{
	return mObject1;
}

};