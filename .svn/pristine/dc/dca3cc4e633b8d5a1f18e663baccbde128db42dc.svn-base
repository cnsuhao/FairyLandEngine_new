//============================================================================
// ReferencedObject.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ReferencedObject
//----------------------------------------------------------------------------

class ReferencedObject
{
private:
	_dword	mRefCount;

protected:
	inline ReferencedObject( );
	inline ReferencedObject( const ReferencedObject& object );

public:
	inline _dword GetRefCount( ) const;
	inline _dword IncreaseRefCount( );
	inline _dword DecreaseRefCount( );
};

//----------------------------------------------------------------------------
// ReferencedObject Implementation
//----------------------------------------------------------------------------

ReferencedObject::ReferencedObject( ) : mRefCount( 0 )
{
}

ReferencedObject::ReferencedObject( const ReferencedObject& object ) : mRefCount( 1 )
{
}

_dword ReferencedObject::GetRefCount( ) const
{
	return mRefCount;
}

_dword ReferencedObject::IncreaseRefCount( )
{
	mRefCount ++;

	return mRefCount;
}

_dword ReferencedObject::DecreaseRefCount( )
{
	if ( mRefCount > 0 )
		mRefCount --;

	return mRefCount;
}

};