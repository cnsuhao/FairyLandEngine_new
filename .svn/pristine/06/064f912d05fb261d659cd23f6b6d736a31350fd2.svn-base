//============================================================================
// GeometryFactory.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// GeometryFactory
//----------------------------------------------------------------------------

class GeometryFactory : public IGeometryFactory
{
private:
	ModelVertexAddon		mVertexAddon;

public:
	virtual _void					SetVertexAddon( const ModelVertexAddon& vertexaddon );
	virtual const ModelVertexAddon&	GetVertexAddon( ) const;

	virtual IVertexDeclaration*		CreateVertexDeclaration( );
	virtual IVertexDeclaration*		CloneVertexDeclaration( IVertexDeclaration* vertexdeclaration, _bool reference );
	virtual _void					ReleaseVertexDeclaration( IVertexDeclaration*& vertexdeclaration );

	virtual IVertexBuffer*			CreateVertexBuffer( _dword type, _dword length );
	virtual IVertexBuffer*			CreateVertexBuffer( _dword type, const _void* buffer, _dword length );
	virtual IVertexBuffer*			CloneVertexBuffer( IVertexBuffer* vertexbuffer, _bool reference );
	virtual _void					CopyVertexBuffer( IVertexBuffer* desvertexbuffer, IVertexBuffer* srcvertexbuffer );
	virtual _void					FillVertexBuffer( IVertexBuffer* vertexbuffer, const _void* buffer, _dword length, _bool bufferowner );
	virtual _void					ReleaseVertexBuffer( IVertexBuffer*& vertexbuffer );

	virtual IIndexBuffer*			CreateIndexBuffer( _dword type, _dword length, _dword indexsize );
	virtual IIndexBuffer*			CreateIndexBuffer( _dword type, const _void* buffer, _dword length, _dword indexsize );
	virtual IIndexBuffer*			CloneIndexBuffer( IIndexBuffer* indexbuffer, _bool reference );
	virtual _void					CopyIndexBuffer( IIndexBuffer* desindexbuffer, IIndexBuffer* srcindexbuffer );
	virtual _void					FillIndexBuffer( IIndexBuffer* indexbuffer, const _void* buffer, _dword length, _bool bufferowner );
	virtual _void					ReleaseIndexBuffer( IIndexBuffer*& indexbuffer );

	virtual Geometry				CreateGeometry( _dword format, _dword vertexnumber, _dword indexnumber, _dword geometrytype, _dword geometrynumber );
	virtual Geometry				CreatePlaneGeometry( _dword format );
	virtual Geometry				CreateCubeGeometry( _dword format );
	virtual Geometry				CreatePyramidGeometry( _dword format );
	virtual Geometry				CreateSphereGeometry( _dword format, _dword ringnumber, _dword segmentnumber );
	virtual Geometry				CreateCylinderGeometry( _dword format, _dword segmentnumber );
	virtual Geometry				CreateTeapotGeometry( _dword format );

	virtual Geometry				CloneGeometry( const Geometry& geometry, _bool reference );

	virtual _void				ReleaseGeometry( Geometry& geometry );
};

};