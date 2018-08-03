//============================================================================
// PhysicsFactory.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// PhysicsFactory Implementation
//----------------------------------------------------------------------------

ITagPoint* PhysicsFactory::CreateTagPoint( )
{
	TagPoint* tagpoint = new TagPoint;

	return tagpoint;
}

ITagPoint* PhysicsFactory::CloneTagPoint( ITagPoint* tagpoint, _bool reference )
{
	if ( tagpoint == _null )
		return _null;

	// Increase reference count.
	( (TagPoint*) tagpoint )->IncreaseRefCount( );

	return tagpoint;
}

_void PhysicsFactory::ReleaseTagPoint( ITagPoint*& tagpoint )
{
	if ( tagpoint == _null )
		return;

	// Decrease reference count.
	if ( ( (TagPoint*) tagpoint )->DecreaseRefCount( ) == 0 )
		delete (TagPoint*) tagpoint;

	tagpoint = _null;
}