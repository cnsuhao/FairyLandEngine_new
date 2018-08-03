//============================================================================
// IResourceReplacer.h
//
// Copyright 2006-2007 Possibility Space Incorporated.
//============================================================================

namespace FL
{

//----------------------------------------------------------------------------
// IResourceReplacer
//----------------------------------------------------------------------------

class IResourceReplacer
{
public:
	virtual _void	Release( )								= 0;

	virtual _void	Update( _dword elapse )					= 0;

	virtual _void*	GetTextureReplacer( StringPtr resname )	= 0;
};

};