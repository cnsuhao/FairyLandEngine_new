//============================================================================
// IGeometryFactory.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IGeometryFactory
//----------------------------------------------------------------------------

class IGeometryFactory
{
public:
	enum _BUFFER_TYPE
	{
		_TYPE_STATIC	= 0x00,
		_TYPE_DYNAMIC	= 0x01,
	};

	enum _BUFFER_MEMORY
	{
		_TYPE_MANAGED	= 0x00,
		_TYPE_VIDEOCARD	= 0x10,
	};

	enum _LOCK_FLAG
	{
		_FLAG_DISCARD	= 1,
		_FLAG_READONLY	= 2,
	};

	virtual _void					SetVertexAddon( const ModelVertexAddon& vertexaddon ) = 0;
	virtual const ModelVertexAddon&	GetVertexAddon( ) const = 0;

	virtual IVertexDeclaration*		CreateVertexDeclaration( ) = 0;
	virtual IVertexDeclaration*		CloneVertexDeclaration( IVertexDeclaration* vertexdeclaration, _bool reference = _true ) = 0;
	virtual _void					ReleaseVertexDeclaration( IVertexDeclaration*& vertexdeclaration ) = 0;

	virtual IVertexBuffer*			CreateVertexBuffer( _dword type, _dword length ) = 0;
	virtual IVertexBuffer*			CreateVertexBuffer( _dword type, const _void* buffer, _dword length ) = 0;
	virtual IVertexBuffer*			CloneVertexBuffer( IVertexBuffer* vertexbuffer, _bool reference = _true ) = 0;
	virtual _void					CopyVertexBuffer( IVertexBuffer* desvertexbuffer, IVertexBuffer* srcvertexbuffer ) = 0;
	virtual _void					FillVertexBuffer( IVertexBuffer* vertexbuffer, const _void* buffer, _dword length, _bool bufferowner ) = 0;
	virtual _void					ReleaseVertexBuffer( IVertexBuffer*& vertexbuffer ) = 0;

	virtual IIndexBuffer*			CreateIndexBuffer( _dword type, _dword length, _dword indexsize ) = 0;
	virtual IIndexBuffer*			CreateIndexBuffer( _dword type, const _void* buffer, _dword length, _dword indexsize ) = 0;
	virtual IIndexBuffer*			CloneIndexBuffer( IIndexBuffer* indexbuffer, _bool reference = _true ) = 0;
	virtual _void					CopyIndexBuffer( IIndexBuffer* desindexbuffer, IIndexBuffer* srcindexbuffer ) = 0;
	virtual _void					FillIndexBuffer( IIndexBuffer* indexbuffer, const _void* buffer, _dword length, _bool bufferowner ) = 0;
	virtual _void					ReleaseIndexBuffer( IIndexBuffer*& indexbuffer ) = 0;

	virtual Geometry				CreateGeometry( _dword format, _dword vertexnumber, _dword indexnumber, _dword geometrytype, _dword geometrynumber ) = 0;

	virtual Geometry				CreatePlaneGeometry( _dword format ) = 0;
	virtual Geometry				CreateCubeGeometry( _dword format ) = 0;
	virtual Geometry				CreatePyramidGeometry( _dword format ) = 0;
	virtual Geometry				CreateSphereGeometry( _dword format, _dword ringnumber = 15, _dword segmentnumber = 20 ) = 0;
	virtual Geometry				CreateCylinderGeometry( _dword format, _dword segmentnumber = 20 ) = 0;
	virtual Geometry				CreateTeapotGeometry( _dword format ) = 0;

	virtual Geometry				CloneGeometry( const Geometry& geometry, _bool reference = _true ) = 0;

	virtual _void					ReleaseGeometry( Geometry& geometry ) = 0;
};

};