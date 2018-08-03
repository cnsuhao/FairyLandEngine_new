//============================================================================
// KernelObject.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// KernelObject Implementation
//----------------------------------------------------------------------------

KernelObject::KernelObject( ) : mObjectHandle( _null )
{
}

KernelObject::KernelObject( _handle objecthandle ) : mObjectHandle( objecthandle )
{
}

KernelObject::KernelObject( const KernelObject& kernelobject )
{
	::DuplicateHandle( ::GetCurrentProcess( ), (_handle) kernelobject, ::GetCurrentProcess( ),
		&mObjectHandle, 0, 0, DUPLICATE_SAME_ACCESS );
}

KernelObject::~KernelObject( )
{
	Close( );
}

KernelObject& KernelObject::operator = ( const KernelObject& kernelobject )
{
	Close( );

	::DuplicateHandle( ::GetCurrentProcess( ), (_handle) kernelobject, ::GetCurrentProcess( ),
		&mObjectHandle, 0, 0, DUPLICATE_SAME_ACCESS );

	return *this;
}

_void KernelObject::Close( )
{
	if ( mObjectHandle != _null && mObjectHandle != INVALID_HANDLE_VALUE )
		::CloseHandle( mObjectHandle );

	mObjectHandle = _null;
}

_handle KernelObject::CloneHandle( ) const
{
	if ( mObjectHandle == _null || mObjectHandle == INVALID_HANDLE_VALUE )
		return mObjectHandle;

	_handle clonedhandle = _null;

	::DuplicateHandle( ::GetCurrentProcess( ), mObjectHandle, ::GetCurrentProcess( ),
		&clonedhandle, 0, 0, DUPLICATE_SAME_ACCESS );

	return clonedhandle;
}