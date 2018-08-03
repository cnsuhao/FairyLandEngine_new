//============================================================================
// KernelObject.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// KernelObject
//----------------------------------------------------------------------------

//! This class is base class of all kernel object.
class KernelObject
{
protected:
	//! KernelObject object handle.
	_handle		mObjectHandle;

public:
	//! Constructor, create a empty kernel object.
	//! @param		none
	KernelObject( );
	//! Constructor, create a kernel object attach to the object handle.
	//! @param		objecthandle	The kernel object handle.
	KernelObject( _handle objecthandle );
	//! Copy-Constructor, copy a kernel object from another one, duplicates object handle.
	//! @param		kernelobject	The other kernel object.
	KernelObject( const KernelObject& kernelobject );
	//! Destructor, delete the object and close the handle.
	//! @param		none
	~KernelObject( );

	//! Copy-Operator, copy a kernel object from another one, duplicates object handle.
	//! @param		kernelobject	The other kernel object.
	KernelObject& operator = ( const KernelObject& kernelobject );

	//! Type conversion, get the kernel object handle.
	//! @param		none
	//! @return		The object handle.
	inline operator _handle ( ) const;

	//! Close the object handle, decrements its reference count, and if the count is 0, then the kernel object is deleted.
	//! @param		none
	_void Close( );

	//! Clone the handle to the object.
	//! @param		none
	//! @return		The handle cloned.
	_handle CloneHandle( ) const;
};

//----------------------------------------------------------------------------
// KernelObject Implementation
//----------------------------------------------------------------------------

KernelObject::operator _handle ( ) const
{
	return mObjectHandle;
}

};