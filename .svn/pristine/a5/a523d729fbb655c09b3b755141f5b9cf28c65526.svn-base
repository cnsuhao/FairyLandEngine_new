//============================================================================
// VertexDeclaration.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// VertexDeclaration
//----------------------------------------------------------------------------

class VertexDeclaration : public IVertexDeclaration, public ReferencedObject
{
private:
	_void*					mVDObject;

	_dword					mUserData;

	Array< VertexElement >	mVertexElementArray;

public:
	VertexDeclaration( );
	~VertexDeclaration( );

	_void EnableDeclaration( );

	virtual _void		SetUserData( _dword userdata );
	virtual _dword		GetUserData( ) const;

	virtual _void		AddVertexElement( _dword source, _dword offset, _dword type, _dword usage, _dword index );
	virtual _void		DelVertexElement( _dword elementindex );
	virtual _void		ClearVertexElement( );
};

};