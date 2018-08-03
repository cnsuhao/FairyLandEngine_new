//============================================================================
// IndexBuffer.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// IndexBuffer Implementation
//----------------------------------------------------------------------------

IndexBuffer::IndexBuffer( _void* ib, _dword type, _dword size, _dword indexsize )
{
	mIBObject	= ib;

	mUserData	= 0;

	mType		= type;
	mSize		= size;

	mIndexSize	= indexsize;

	IncreaseRefCount( );
}

IndexBuffer::~IndexBuffer( )
{
	GraphicsManager::GetRenderAPIHelper( ).ReleaseIndexBuffer( mIBObject );
}

_void IndexBuffer::SetUserData( _dword userdata )
{
	mUserData = userdata;
}

_dword IndexBuffer::GetUserData( ) const
{
	return mUserData;
}

_dword IndexBuffer::GetType( ) const
{
	return mType;
}

_dword IndexBuffer::GetSize( ) const
{
	return mSize;
}

_dword IndexBuffer::GetIndexSize( ) const
{
	return mIndexSize;
}

_void* IndexBuffer::Lock( _dword offset, _dword length, _dword flag )
{
	return GraphicsManager::GetRenderAPIHelper( ).LockIndexBuffer( mIBObject, offset, length, flag );
}

_void IndexBuffer::Unlock( )
{
	GraphicsManager::GetRenderAPIHelper( ).UnlockIndexBuffer( mIBObject );
}

_void IndexBuffer::Fill( _dword offset, const _void* buffer, _dword length )
{
	if ( offset + length > mSize )
		return;

	_void* tempbuffer = GraphicsManager::GetRenderAPIHelper( ).LockIndexBuffer( mIBObject, offset, length, 0 );
	Memory::MemCpy( tempbuffer, buffer, length );
	GraphicsManager::GetRenderAPIHelper( ).UnlockIndexBuffer( mIBObject );
}