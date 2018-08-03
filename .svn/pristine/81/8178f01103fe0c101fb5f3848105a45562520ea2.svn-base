//============================================================================
// IVertexBuffer.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IVertexBuffer
//----------------------------------------------------------------------------

class IVertexBuffer
{
public:
	virtual _void		SetUserData( _dword userdata ) = 0;
	virtual _dword		GetUserData( ) const = 0;

	virtual _dword		GetType( ) const = 0;
	virtual _dword		GetSize( ) const = 0;

	virtual _void*		Lock( _dword offset, _dword length, _dword flag = 0 ) = 0;
	virtual _void		Unlock( ) = 0;
	virtual _void		Fill( _dword offset, const _void* buffer, _dword length ) = 0;

	virtual _void					SetBoundBox( const AxisAlignedBox& boundbox ) = 0;
	virtual const AxisAlignedBox&	GetBoundBox( ) const = 0;
};

};