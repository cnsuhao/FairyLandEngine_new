//============================================================================
// IndexBuffer.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IndexBuffer
//----------------------------------------------------------------------------

class IndexBuffer : public IIndexBuffer, public ReferencedObject
{
private:
	_void*		mIBObject;

	_dword		mUserData;

	_dword		mType;
	_dword		mSize;

	_dword		mIndexSize;

public:
	IndexBuffer( _void* ib, _dword type, _dword size, _dword indexsize );
	~IndexBuffer( );

	inline _void* GetBufferObject( ) const
		{ return mIBObject; }

	virtual _void		SetUserData( _dword userdata );
	virtual _dword		GetUserData( ) const;

	virtual _dword		GetType( ) const;
	virtual _dword		GetSize( ) const;

	virtual _dword		GetIndexSize( ) const;

	virtual _void*		Lock( _dword offset, _dword length, _dword flag );
	virtual _void		Unlock( );
	virtual _void		Fill( _dword offset, const _void* buffer, _dword length );
};

};