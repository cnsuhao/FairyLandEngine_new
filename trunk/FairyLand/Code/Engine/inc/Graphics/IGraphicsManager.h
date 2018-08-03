//============================================================================
// IGraphicsManager.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IGraphicsManager
//----------------------------------------------------------------------------

class IGraphicsManager
{
public:
	virtual _void	Release( ) = 0;
	virtual _void	Update( _dword elapse ) = 0;

	virtual _void	EnableAsyncLoader( _bool enable ) = 0;
	virtual _bool	IsAsyncLoaderEnabled( ) const = 0;

	virtual _bool	CreateRenderDevice( _handle window, _dword devicetype ) = 0;
};

};