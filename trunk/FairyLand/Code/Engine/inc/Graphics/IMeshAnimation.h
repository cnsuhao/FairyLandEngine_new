//============================================================================
// IMeshAnimation.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IMeshAnimation
//----------------------------------------------------------------------------

class IMeshAnimation : public IAnimation
{
public:
	virtual IMesh* GetAttachedMesh( ) const = 0;
};

};