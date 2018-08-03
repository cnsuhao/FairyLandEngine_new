//============================================================================
// PhysicsFactory.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// PhysicsFactory
//----------------------------------------------------------------------------

class PhysicsFactory : public IPhysicsFactory
{
public:
	virtual ITagPoint*	CreateTagPoint( );
	virtual ITagPoint*	CloneTagPoint( ITagPoint* tagpoint, _bool reference );
	virtual _void		ReleaseTagPoint( ITagPoint*& tagpoint );
};

};