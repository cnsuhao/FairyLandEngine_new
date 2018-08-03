#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Triple
//----------------------------------------------------------------------------

//! This class is template container class, represents a 3-element structure.

//! Template parameter "Type1" defines the first element type.<br>
//! Template parameter "Type2" defines the second element type.<br>
//! Template parameter "Type3" defines the third element type.<br>
//! A comparision of triple only care about the first element type, the <b>Key<b> type.
template< typename Type1, typename Type2, typename Type3 > class Triple
{
public:
	//! The first element.
	Type1	mObject1;
	//! The second element.
	Type2	mObject2;
	//! The third element.
	Type3	mObject3;

	//! Constructor, create a triple without assigning of elements.
	//! @param		none
	inline Triple( );
	//! Constructor, create a triple and assign elements.
	//! @param		none
	inline Triple( const Type1& object1, const Type2& object2, const Type3& object3 );

	//! Type conversion, get the first object.
	//! @param		none
	//! @return		The const reference of the first object.
	inline operator const Type1& ( ) const;
};

//----------------------------------------------------------------------------
// Triple Implementation
//----------------------------------------------------------------------------

template< typename Type1, typename Type2, typename Type3 >
Triple< Type1, Type2, Type3 >::Triple( )
{
}

template< typename Type1, typename Type2, typename Type3 >
Triple< Type1, Type2, Type3 >::Triple( const Type1& object1, const Type2& object2, const Type3& object3 )
	: mObject1( object1 ), mObject2( object2 ), mObject3( object3 )
{
}

template< typename Type1, typename Type2, typename Type3 >
Triple< Type1, Type2, Type3 >::operator const Type1& ( ) const
{
	return mObject1;
}

};