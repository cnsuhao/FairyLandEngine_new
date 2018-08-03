//============================================================================
// VertexBuffer.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// VertexBuffer Implementation
//----------------------------------------------------------------------------

VertexBuffer::VertexBuffer( _void* vb, _dword type, _dword size ) : mVBObject( _null )
{
	mVBObject	= vb;

	mUserData	= 0;

	mType		= type;
	mSize		= size;

	mBoundBox	= AxisAlignedBox::cNullBox;

	IncreaseRefCount( );
}

VertexBuffer::~VertexBuffer( )
{
	GraphicsManager::GetRenderAPIHelper( ).ReleaseVertexBuffer( mVBObject );
}

_void VertexBuffer::SetUserData( _dword userdata )
{
	mUserData = userdata;
}

_dword VertexBuffer::GetUserData( ) const
{
	return mUserData;
}

_dword VertexBuffer::GetType( ) const
{
	return mType;
}

_dword VertexBuffer::GetSize( ) const
{
	return mSize;
}

_void* VertexBuffer::Lock( _dword offset, _dword length, _dword flag )
{
	return GraphicsManager::GetRenderAPIHelper( ).LockVertexBuffer( mVBObject, offset, length, flag );
}

_void VertexBuffer::Unlock( )
{
	GraphicsManager::GetRenderAPIHelper( ).UnlockVertexBuffer( mVBObject );
}

_void VertexBuffer::Fill( _dword offset, const _void* buffer, _dword length )
{
	if ( offset + length > mSize )
		return;

	_void* tempbuffer = GraphicsManager::GetRenderAPIHelper( ).LockVertexBuffer( mVBObject, offset, length, 0 );
	Memory::MemCpy( tempbuffer, buffer, length );
	GraphicsManager::GetRenderAPIHelper( ).UnlockVertexBuffer( mVBObject );
}

_void VertexBuffer::SetBoundBox( const AxisAlignedBox& boundbox )
{
	mBoundBox = boundbox;
}

const AxisAlignedBox& VertexBuffer::GetBoundBox( ) const
{
	return mBoundBox;
}