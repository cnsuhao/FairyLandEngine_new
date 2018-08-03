//============================================================================
// IRenderSet.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IRenderSet
//----------------------------------------------------------------------------

class IRenderSet
{
public:
	virtual _void	BeginRecord( )	= 0;
	virtual _void	EndRecord( )	= 0;
	virtual _void	ClearRecord( )	= 0;
};

};