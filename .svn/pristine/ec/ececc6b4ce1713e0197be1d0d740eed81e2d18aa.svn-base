//============================================================================
// VertexDeclaration.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// VertexDeclaration Implementation
//----------------------------------------------------------------------------

VertexDeclaration::VertexDeclaration( )
{
	mVDObject		= _null;

	mUserData		= 0;

	IncreaseRefCount( );
}

VertexDeclaration::~VertexDeclaration( )
{
	GraphicsManager::GetRenderAPIHelper( ).ReleaseVertexDeclaration( mVDObject );
}

_void VertexDeclaration::EnableDeclaration( )
{
	// Create vertex declaration object.
	mVDObject = GraphicsManager::GetRenderAPIHelper( ).CreateVertexDeclaration( mVDObject, mVertexElementArray );
}

_void VertexDeclaration::SetUserData( _dword userdata )
{
	mUserData = userdata;
}

_dword VertexDeclaration::GetUserData( ) const
{
	return mUserData;
}

_void VertexDeclaration::AddVertexElement( _dword source, _dword offset, _dword type, _dword usage, _dword index )
{
	VertexElement vertexelement;
	vertexelement.mSource	= source;
	vertexelement.mOffset	= offset;
	vertexelement.mType		= type;
	vertexelement.mUsage	= usage;
	vertexelement.mIndex	= index;

	mVertexElementArray.Append( vertexelement );

	// When vertex declaration changed, release the old one.
	GraphicsManager::GetRenderAPIHelper( ).ReleaseVertexDeclaration( mVDObject );
	mVDObject = _null;
}

_void VertexDeclaration::DelVertexElement( _dword elementindex )
{
	mVertexElementArray.Remove( elementindex );

	// When vertex declaration changed, release the old one.
	GraphicsManager::GetRenderAPIHelper( ).ReleaseVertexDeclaration( mVDObject );
	mVDObject = _null;
}

_void VertexDeclaration::ClearVertexElement( )
{
	mVertexElementArray.Clear( );
}