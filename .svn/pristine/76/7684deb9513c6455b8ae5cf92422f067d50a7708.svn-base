//============================================================================
// VertexBuffer.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// VertexBuffer
//----------------------------------------------------------------------------

class VertexBuffer : public IVertexBuffer, public ReferencedObject
{
private:
	_void*			mVBObject;

	_dword			mUserData;

	_dword			mType;
	_dword			mSize;

	AxisAlignedBox	mBoundBox;

public:
	VertexBuffer( _void* vb, _dword type, _dword size );
	~VertexBuffer( );

	inline _void* GetBufferObject( ) const
		{ return mVBObject; }

	virtual _void		SetUserData( _dword userdata );
	virtual _dword		GetUserData( ) const;

	virtual _dword		GetType( ) const;
	virtual _dword		GetSize( ) const;

	virtual _void*		Lock( _dword offset, _dword length, _dword flag );
	virtual _void		Unlock( );
	virtual _void		Fill( _dword offset, const _void* buffer, _dword length );

	virtual _void					SetBoundBox( const AxisAlignedBox& boundbox );
	virtual const AxisAlignedBox&	GetBoundBox( ) const;
};

};