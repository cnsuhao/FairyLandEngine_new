//============================================================================
// Singleton.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Singleton
//----------------------------------------------------------------------------

//! This template class is use for creating global single-instance class.
template < typename Type > class Singleton
{
private:
	//! Copy-Constructor, private access indicates can not copy a object from another one.
	//! @param		singleton		The other single-instance.
	Singleton( const Singleton< Type >& singleton );

	//! Copy-Operator, private access indicates can not copy a object from another one.
	//! @param		singleton		The other single-instance.
	Singleton< Type >& operator = ( const Singleton< Type >& singleton );

protected:
	//! The pointer of the single-instance.
	static Type* sSingleton;

public:
	//! Get the reference of the single-instance.
	//! @param		none
	//! @return		The reference of the single-instance.
	static inline Type& GetSingleton( );
	//! Get the pointer of the single-instance.
	//! @param		none
	//! @return		The pointer of the single-instance.
	static inline Type* GetSingletonPtr( );

	//! Constructor, create an instance, and assign the global single-instance pointer.
	//! @param		none
	inline Singleton( );
	//! Destructor, delete an instance, and clear the global single-instance pointer.
	//! @param		none
    inline ~Singleton( );
};

//----------------------------------------------------------------------------
// Singleton Implementation
//----------------------------------------------------------------------------

template < typename Type >
Type& Singleton< Type >::GetSingleton( )
{
	FL_ASSERT( sSingleton != _null )

	#ifdef _MULTIGLOBAL_ENABLE

	FairyLandGlobal& global = GetFairyLandGlobal( );
	return *( (Type*) (Singleton<Type>*) global.mSingletons[ (_dword) sSingleton - 1 ] );

	#else

	return *sSingleton;

	#endif
}

template < typename Type >
Type* Singleton< Type >::GetSingletonPtr( )
{
	#ifdef _MULTIGLOBAL_ENABLE

	if ( sSingleton == _null )
		return _null;

	FairyLandGlobal& global = GetFairyLandGlobal( );
	return (Type*) (Singleton<Type>*) global.mSingletons[ (_dword) sSingleton - 1 ];

	#else

	return sSingleton;

	#endif
}

template < typename Type >
Singleton< Type >::Singleton( )
{
	#ifdef _MULTIGLOBAL_ENABLE

	if ( sSingleton == _null )
		sSingleton = (Type*) GetFairyLandSingletonToken( );

	FairyLandGlobal& global = GetFairyLandGlobal( );
	FL_ASSERT( global.mSingletons[ (_dword) sSingleton - 1 ] == _null )
	global.mSingletons[ (_dword) sSingleton - 1 ] = this;

	#else

	FL_ASSERT( sSingleton == _null )
	sSingleton = static_cast< Type* >( this );

	#endif
}

template < typename Type >
Singleton< Type >::~Singleton( )
{
	FL_ASSERT( sSingleton != _null )

	#ifdef _MULTIGLOBAL_ENABLE

	FairyLandGlobal& global = GetFairyLandGlobal( );
	global.mSingletons[ (_dword) sSingleton - 1 ] = _null;

	#else

	sSingleton = _null;

	#endif
}

};