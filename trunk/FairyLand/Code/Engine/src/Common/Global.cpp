//============================================================================
// Global.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Global Implementation
//----------------------------------------------------------------------------

Global::Global( StringPtr name ) : mName( name ), mValue( 0 ), mNext( _null )
{
	_void* pointer = _null;

	// Get existing pointer from process environment.
	if ( System::GetEnvironment( mName, pointer ) == _false || pointer == _null )
	{
		// Not found, put this global into environment.
		System::SetEnvironment( mName, this );
	}
	else
	{
		// Found, link the global nodes together.
		Global* rootnode = (Global*) pointer;

		mValue			= rootnode->mValue;
		mNext			= rootnode->mNext;
		rootnode->mNext	= this;
	}
}

Global::~Global( )
{
	_void* pointer = _null;

	// Get existing pointer from process environment.
	if ( System::GetEnvironment( mName, pointer ) == _false )
		return;

	Global* prevnode = _null;

	// Found, set all global value in the list.
	for ( Global* node = (Global*) pointer; node != _null; node = node->mNext )
	{
		if ( node == this )
		{
			if ( prevnode != _null )
			{
				prevnode->mNext = node->mNext;
			}
			else
			{
				System::SetEnvironment( mName, node->mNext );
			}

			break;
		}

		// Break dead loop.
		if ( node != _null && node == node->mNext )
			break;

		prevnode = node;
	}
}

_bool Global::SetValue( StringPtr name, _dword value )
{
	_void* pointer = _null;

	// Get existing pointer from process environment.
	if ( System::GetEnvironment( name, pointer ) == _false )
		return _false;

	// Found, set all global value in the list.
	for ( Global* node = (Global*) pointer; node != _null; node = node->mNext )
		node->mValue = value;

	return _true;
}

_dword Global::GetValue( StringPtr name )
{
	_void* pointer = _null;

	// Get existing pointer from process environment.
	if ( System::GetEnvironment( name, pointer ) == _false || pointer == _null )
		return 0;

	return ( (Global*) pointer )->mValue;
}