//============================================================================
// GeometryFactory.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// GeometryFactory Implementation
//----------------------------------------------------------------------------

_void GeometryFactory::SetVertexAddon( const ModelVertexAddon& vertexaddon )
{
	mVertexAddon = vertexaddon;
}

const ModelVertexAddon& GeometryFactory::GetVertexAddon( ) const
{
	return mVertexAddon;
}

IVertexDeclaration* GeometryFactory::CreateVertexDeclaration( )
{
	VertexDeclaration* vertexdeclaration = new VertexDeclaration;

	return vertexdeclaration;
}

IVertexDeclaration* GeometryFactory::CloneVertexDeclaration( IVertexDeclaration* vertexdeclaration, _bool reference )
{
	if ( vertexdeclaration == _null )
		return _null;

	// Increase reference count.
	( (VertexDeclaration*) vertexdeclaration )->IncreaseRefCount( );

	return vertexdeclaration;
}

_void GeometryFactory::ReleaseVertexDeclaration( IVertexDeclaration*& vertexdeclaration )
{
	if ( vertexdeclaration == _null )
		return;

	// Decrease reference count.
	if ( ( (VertexDeclaration*) vertexdeclaration )->DecreaseRefCount( ) == 0 )
		delete (VertexDeclaration*) vertexdeclaration;

	vertexdeclaration = _null;
}

IVertexBuffer* GeometryFactory::CreateVertexBuffer( _dword type, const _void* buffer, _dword length )
{
	if ( length == 0 )
		return _null;

	_void* vb = GraphicsManager::GetRenderAPIHelper( ).CreateVertexBuffer( type, length );
	if ( vb == _null )
		return _null;

	VertexBuffer* vertexbuffer = new VertexBuffer( vb, type, length );
	vertexbuffer->Fill( 0, buffer, length );

	return vertexbuffer;
}

IVertexBuffer* GeometryFactory::CreateVertexBuffer( _dword type, _dword length )
{
	if ( length == 0 )
		return _null;

	_void* vb = GraphicsManager::GetRenderAPIHelper( ).CreateVertexBuffer( type, length );
	if ( vb == _null )
		return _null;

	VertexBuffer* vertexbuffer = new VertexBuffer( vb, type, length );

	return vertexbuffer;
}

IVertexBuffer* GeometryFactory::CloneVertexBuffer( IVertexBuffer* vertexbuffer, _bool reference )
{
	if ( vertexbuffer == _null )
		return _null;

	if ( reference )
	{
		// Increase reference count.
		( (VertexBuffer*) vertexbuffer )->IncreaseRefCount( );

		return vertexbuffer;
	}
	else
	{
		IVertexBuffer* newvertexbuffer = CreateVertexBuffer( vertexbuffer->GetType( ), vertexbuffer->GetSize( ) );
		if ( newvertexbuffer == _null )
			return _null;

		if ( vertexbuffer->GetSize( ) > 0 )
		{
			_void* oldbuffer = vertexbuffer->Lock( 0, 0, IGeometryFactory::_FLAG_DISCARD );
			FL_ASSERT( oldbuffer != _null )

			_void* newbuffer = newvertexbuffer->Lock( 0, 0 );
			FL_ASSERT( newbuffer != _null )

			Memory::MemCpy( newbuffer, oldbuffer, vertexbuffer->GetSize( ) );

			vertexbuffer->Unlock( );
			newvertexbuffer->Unlock( );
		}

		newvertexbuffer->SetBoundBox( vertexbuffer->GetBoundBox( ) );

		return newvertexbuffer;
	}
}

_void GeometryFactory::CopyVertexBuffer( IVertexBuffer* desvertexbuffer, IVertexBuffer* srcvertexbuffer )
{
	if ( desvertexbuffer == _null || srcvertexbuffer == _null )
		return;

	_void* desbuffer = desvertexbuffer->Lock( 0, 0 );
	FL_ASSERT( desbuffer != _null )

	_void* srcbuffer = srcvertexbuffer->Lock( 0, 0, IGeometryFactory::_FLAG_DISCARD );
	FL_ASSERT( srcbuffer != _null )

	Memory::MemCpy( desbuffer, srcbuffer, Math::Min( srcvertexbuffer->GetSize( ), desvertexbuffer->GetSize( ) ) );

	srcvertexbuffer->Unlock( );
	desvertexbuffer->Unlock( );

	desvertexbuffer->SetBoundBox( srcvertexbuffer->GetBoundBox( ) );
}

_void GeometryFactory::FillVertexBuffer( IVertexBuffer* vertexbuffer, const _void* buffer, _dword length, _bool bufferowner )
{
	if ( vertexbuffer == _null )
		return;

	_byte* targetbuffer = (_byte*) vertexbuffer->Lock( 0, 0 );

	if ( targetbuffer != _null )
		Memory::MemCpy( targetbuffer, buffer, Math::Min( vertexbuffer->GetSize( ), length ) );

	vertexbuffer->Unlock( );

	if ( bufferowner )
		delete[] (_byte*) buffer;
}

_void GeometryFactory::ReleaseVertexBuffer( IVertexBuffer*& vertexbuffer )
{
	if ( vertexbuffer == _null )
		return;

	// Decrease reference count.
	if ( ( (VertexBuffer*) vertexbuffer )->DecreaseRefCount( ) == 0 )
		delete (VertexBuffer*) vertexbuffer;

	vertexbuffer = _null;
}

IIndexBuffer* GeometryFactory::CreateIndexBuffer( _dword type, _dword length, _dword indexsize )
{
	if ( length == 0 )
		return _null;

	_void* ib = GraphicsManager::GetRenderAPIHelper( ).CreateIndexBuffer( type, length, indexsize );
	if ( ib == _null )
		return _null;

	IndexBuffer* indexbuffer = new IndexBuffer( ib, type, length, indexsize );

	return indexbuffer;
}

IIndexBuffer* GeometryFactory::CreateIndexBuffer( _dword type, const _void* buffer, _dword length, _dword indexsize )
{
	if ( length == 0 )
		return _null;

	_void* ib = GraphicsManager::GetRenderAPIHelper( ).CreateIndexBuffer( type, length, indexsize );
	if ( ib == _null )
		return _null;

	IndexBuffer* indexbuffer = new IndexBuffer( ib, type, length, indexsize );
	indexbuffer->Fill( 0, buffer, length );

	return indexbuffer;
}

IIndexBuffer* GeometryFactory::CloneIndexBuffer( IIndexBuffer* indexbuffer, _bool reference )
{
	if ( indexbuffer == _null )
		return _null;

	if ( reference )
	{
		// Increase reference count.
		( (IndexBuffer*) indexbuffer )->IncreaseRefCount( );

		return indexbuffer;
	}
	else
	{
		IIndexBuffer* newindexbuffer = CreateIndexBuffer( indexbuffer->GetType( ), indexbuffer->GetSize( ), indexbuffer->GetIndexSize( ) );
		if ( newindexbuffer == _null )
			return _null;

		if ( indexbuffer->GetSize( ) > 0 )
		{
			_void* oldbuffer = indexbuffer->Lock( 0, 0, IGeometryFactory::_FLAG_DISCARD );
			FL_ASSERT( oldbuffer != _null )

			_void* newbuffer = newindexbuffer->Lock( 0, 0 );
			FL_ASSERT( newbuffer != _null )

			Memory::MemCpy( newbuffer, oldbuffer, indexbuffer->GetSize( ) );

			indexbuffer->Unlock( );
			newindexbuffer->Unlock( );
		}

		return newindexbuffer;
	}
}

_void GeometryFactory::CopyIndexBuffer( IIndexBuffer* desindexbuffer, IIndexBuffer* srcindexbuffer )
{
	if ( desindexbuffer == _null || srcindexbuffer == _null )
		return;

	_void* desbuffer = desindexbuffer->Lock( 0, 0 );
	FL_ASSERT( desbuffer != _null )

	_void* srcbuffer = srcindexbuffer->Lock( 0, 0, IGeometryFactory::_FLAG_DISCARD );
	FL_ASSERT( srcbuffer != _null )

	Memory::MemCpy( desbuffer, srcbuffer, Math::Min( srcindexbuffer->GetSize( ), desindexbuffer->GetSize( ) ) );

	desindexbuffer->Unlock( );
	srcindexbuffer->Unlock( );
}

_void GeometryFactory::FillIndexBuffer( IIndexBuffer* indexbuffer, const _void* buffer, _dword length, _bool bufferowner )
{
	if ( indexbuffer == _null )
		return;
	_byte* targetbuffer = (_byte*) indexbuffer->Lock( 0, 0 );

	if ( targetbuffer != _null )
		Memory::MemCpy( targetbuffer, buffer, Math::Min( indexbuffer->GetSize( ), length ) );

	indexbuffer->Unlock( );

	if ( bufferowner )
		delete[] (_byte*) buffer;
}

_void GeometryFactory::ReleaseIndexBuffer( IIndexBuffer*& indexbuffer )
{
	if ( indexbuffer == _null )
		return;

	// Decrease reference count.
	if ( ( (IndexBuffer*) indexbuffer )->DecreaseRefCount( ) == 0 )
		delete (IndexBuffer*) indexbuffer;

	indexbuffer = _null;
}

Geometry GeometryFactory::CreateGeometry( _dword format, _dword vertexnumber, _dword indexnumber, _dword geometrytype, _dword geometrynumber )
{
	format = mVertexAddon.UpdateFormat( format );

	Geometry geometry;

	// Set vertex format.
	geometry.mFormatFlag	= format;
	geometry.mVertexNumber	= vertexnumber;
	geometry.mVertexSize	= ModelHelper::GetVertexSizeFromFormat( format );
	geometry.mHelperSize	= ModelHelper::GetHelperSizeFromFormat( format );

	// Create vertex declaration.
	if ( ( geometry.mVertexDeclaration = CreateVertexDeclaration( ) ) == _null )
	{
		ReleaseGeometry( geometry );
		return geometry;
	}

	// Calculate vertex size and vertex declaration.
	_dword vertexoffset = 0;

	if ( format & Geometry::_FORMATFLAG_POSITIONT )
	{
		vertexoffset = sizeof( Vector4 );
		geometry.mVertexDeclaration->AddVertexElement( 0, 0, IVertexDeclaration::_TYPE_FLOAT4, IVertexDeclaration::_USAGE_POSITIONT, 0 );
	}
	else
	{
		vertexoffset = sizeof( Vector3 );
		geometry.mVertexDeclaration->AddVertexElement( 0, 0, IVertexDeclaration::_TYPE_FLOAT3, IVertexDeclaration::_USAGE_POSITION, 0 );
	}

	if ( format & Geometry::_FORMATFLAG_NORMAL )
	{
		geometry.mVertexDeclaration->AddVertexElement( 0, vertexoffset, IVertexDeclaration::_TYPE_FLOAT3, IVertexDeclaration::_USAGE_NORMAL, 0 );
		vertexoffset += sizeof( Vector3 );
	}

	if ( format & Geometry::_FORMATFLAG_DIFFUSE )
	{
		geometry.mVertexDeclaration->AddVertexElement( 0, vertexoffset, IVertexDeclaration::_TYPE_COLOR, IVertexDeclaration::_USAGE_DIFFUSE, 0 );
		vertexoffset += sizeof( _dword );
	}

	if ( format & Geometry::_FORMATFLAG_SPECULAR )
	{
		geometry.mVertexDeclaration->AddVertexElement( 0, vertexoffset, IVertexDeclaration::_TYPE_COLOR, IVertexDeclaration::_USAGE_SPECULAR, 0 );
		vertexoffset += sizeof( _dword );
	}

	_dword texnumber = ModelHelper::GetTexcoordNumberFromFormat( format );

	for ( _dword i = 0; i < texnumber; i ++ )
	{
		geometry.mVertexDeclaration->AddVertexElement( 0, vertexoffset, IVertexDeclaration::_TYPE_FLOAT2, IVertexDeclaration::_USAGE_TEXCOORD, i );
		vertexoffset += sizeof( Vector2 );
	}

	// Create vertex buffer.
	if ( format & Geometry::_FORMATFLAG_INFLUENCE )
	{
		geometry.mVertexBuffer = CreateVertexBuffer( _TYPE_DYNAMIC, geometry.mVertexSize * vertexnumber );
		geometry.mHelperBuffer = CreateVertexBuffer( _TYPE_DYNAMIC, geometry.mHelperSize * vertexnumber );
	}
	else
	{
		geometry.mVertexBuffer = CreateVertexBuffer( _TYPE_STATIC, geometry.mVertexSize * vertexnumber );
	}

	if ( geometry.mVertexBuffer == _null )
	{
		ReleaseGeometry( geometry );
		return geometry;
	}

	// Create index buffer.
	if ( indexnumber > 0 )
	{
		if ( ( geometry.mIndexBuffer = CreateIndexBuffer( _TYPE_STATIC, sizeof( _word ) * indexnumber, sizeof( _word ) ) ) == _null )
			ReleaseGeometry( geometry );
	}

	geometry.mGeometryType		= geometrytype;
	geometry.mGeometryNumber	= geometrynumber;

	return geometry;
}

Geometry GeometryFactory::CreatePlaneGeometry( _dword format )
{
	// Create geometry base on format.
	Geometry geometry = CreateGeometry( format, 6, 0, Geometry::_GEOMETRY_TRIANGLELIST, 2 );

	// Create geometry failed.
	if ( geometry.mVertexDeclaration == _null )
		return geometry;

	// Fill vertex buffer.
	_dword vertexbuffersize = geometry.mVertexBuffer->GetSize( );

	_byte* vertexbuffer = (_byte*) geometry.mVertexBuffer->Lock( 0, 0 );

	if ( vertexbuffer == _null )
	{
		ReleaseGeometry( geometry );
		return geometry;
	}

	ModelBuffer modelbuffer;
	modelbuffer.mBuffer = vertexbuffer;
	modelbuffer.mLength = vertexbuffersize;

	const static Vector3 positiondata[] =
	{
		Vector3(  1.0f,  1.0f,  0.0f ),
		Vector3( -1.0f, -1.0f,  0.0f ),
		Vector3(  1.0f, -1.0f,  0.0f ),
		Vector3(  1.0f,  1.0f,  0.0f ),
		Vector3( -1.0f,  1.0f,  0.0f ),
		Vector3( -1.0f, -1.0f,  0.0f ),
	};

	const static Vector3 normaldata[] =
	{
		Vector3( 0.0f, 0.0f, 1.0f ),
		Vector3( 0.0f, 0.0f, 1.0f ),
		Vector3( 0.0f, 0.0f, 1.0f ),
		Vector3( 0.0f, 0.0f, 1.0f ),
		Vector3( 0.0f, 0.0f, 1.0f ),
		Vector3( 0.0f, 0.0f, 1.0f ),
	};

	const static Vector2 texcoorddata[] =
	{
		Vector2( 1.0f, 0.0f ),
		Vector2( 0.0f, 1.0f ),
		Vector2( 1.0f, 1.0f ),
		Vector2( 1.0f, 0.0f ),
		Vector2( 0.0f, 0.0f ),
		Vector2( 0.0f, 1.0f ),
	};

	_dword texnumber = ModelHelper::GetTexcoordNumberFromFormat( format );

	for ( _dword i = 0; i < 6; i ++ )
	{
		_byte* buffer = vertexbuffer;

		// Set vertex position.
		*( (Vector3*) buffer ) = positiondata[i];
		buffer += sizeof( Vector3 );

		// Set vertex normal.
		if ( format & Geometry::_FORMATFLAG_NORMAL )
		{
			*( (Vector3*) buffer ) = normaldata[i];
			buffer += sizeof( Vector3 );
		}

		// Set diffuse color.
		if ( format & Geometry::_FORMATFLAG_DIFFUSE )
		{
			*( (_dword*) buffer ) = -1;
			buffer += sizeof( _dword );
		}

		// Set specular color.
		if ( format & Geometry::_FORMATFLAG_SPECULAR )
		{
			*( (_dword*) buffer ) = 0;
			buffer += sizeof( _dword );
		}

		// Set vertex texcoord.
		for ( _dword t = 0; t < texnumber; t ++ )
		{
			*( (Vector2*) buffer ) = texcoorddata[i];
			buffer += sizeof( Vector2 );
		}

		vertexbuffer += geometry.mVertexSize;
	}

	geometry.mVertexBuffer->Unlock( );

	// Set bound box for this geometry.
	geometry.mVertexBuffer->SetBoundBox( ModelHelper::GetBoundBoxFromVertexBuffer( format, modelbuffer ) );

	return geometry;
}

Geometry GeometryFactory::CreateCubeGeometry( _dword format )
{
	// Create geometry base on format.
	Geometry geometry = CreateGeometry( format, 24, 36, Geometry::_GEOMETRY_TRIANGLELIST, 12 );

	// Create geometry failed.
	if ( geometry.mVertexDeclaration == _null )
		return geometry;

	// Fill vertex buffer.
	_dword vertexbuffersize = geometry.mVertexBuffer->GetSize( );

	_byte* vertexbuffer = (_byte*) geometry.mVertexBuffer->Lock( 0, 0 );

	if ( vertexbuffer == _null )
	{
		ReleaseGeometry( geometry );
		return geometry;
	}

	ModelBuffer modelbuffer;
	modelbuffer.mBuffer = vertexbuffer;
	modelbuffer.mLength = vertexbuffersize;

	const static Vector3 positiondata[] =
	{
		// Front Face
		Vector3(  1.0f,  1.0f,  1.0f ),
		Vector3(  1.0f, -1.0f,  1.0f ),
		Vector3( -1.0f, -1.0f,  1.0f ),
		Vector3( -1.0f,  1.0f,  1.0f ),
		// Back Face
		Vector3(  1.0f,  1.0f, -1.0f ),
		Vector3( -1.0f,  1.0f, -1.0f ),
		Vector3( -1.0f, -1.0f, -1.0f ),
		Vector3(  1.0f, -1.0f, -1.0f ),
		// Top Face
		Vector3(  1.0f,  1.0f,  1.0f ),
		Vector3( -1.0f,  1.0f,  1.0f ),
		Vector3( -1.0f,  1.0f, -1.0f ),
		Vector3(  1.0f,  1.0f, -1.0f ),
		// Bottom Face
		Vector3(  1.0f, -1.0f,  1.0f ),
		Vector3(  1.0f, -1.0f, -1.0f ),
		Vector3( -1.0f, -1.0f, -1.0f ),
		Vector3( -1.0f, -1.0f,  1.0f ),
		// Right face
		Vector3(  1.0f,  1.0f,  1.0f ),
		Vector3(  1.0f,  1.0f, -1.0f ),
		Vector3(  1.0f, -1.0f, -1.0f ),
		Vector3(  1.0f, -1.0f,  1.0f ),
		// Left Face
		Vector3( -1.0f,  1.0f,  1.0f ),
		Vector3( -1.0f, -1.0f,  1.0f ),
		Vector3( -1.0f, -1.0f, -1.0f ),
		Vector3( -1.0f,  1.0f, -1.0f ),
	};

	const static Vector3 normaldata[] =
	{
		// Front Face
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		// Back Face
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		// Top Face
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		// Bottom Face
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		// Right face
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		// Left Face
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
	};

	const static Vector2 texcoorddata[] =
	{
		// Front Face
		Vector2( 1.0f, 0.0f ),
		Vector2( 1.0f, 1.0f ),
		Vector2( 0.0f, 1.0f ),
		Vector2( 0.0f, 0.0f ),
		// Back Face
		Vector2( 0.0f, 0.0f ),
		Vector2( 1.0f, 0.0f ),
		Vector2( 1.0f, 1.0f ),
		Vector2( 0.0f, 1.0f ),
		// Top Face
		Vector2( 1.0f, 1.0f ),
		Vector2( 0.0f, 1.0f ),
		Vector2( 0.0f, 0.0f ),
		Vector2( 1.0f, 0.0f ),
		// Bottom Face
		Vector2( 0.0f, 1.0f ),
		Vector2( 0.0f, 0.0f ),
		Vector2( 1.0f, 0.0f ),
		Vector2( 1.0f, 1.0f ),
		// Right face
		Vector2( 0.0f, 0.0f ),
		Vector2( 1.0f, 0.0f ),
		Vector2( 1.0f, 1.0f ),
		Vector2( 0.0f, 1.0f ),
		// Left Face
		Vector2( 1.0f, 0.0f ),
		Vector2( 1.0f, 1.0f ),
		Vector2( 0.0f, 1.0f ),
		Vector2( 0.0f, 0.0f ),
	};

	_dword texnumber = ModelHelper::GetTexcoordNumberFromFormat( format );

	for ( _dword i = 0; i < 24; i ++ )
	{
		_byte* buffer = vertexbuffer;

		// Set vertex position.
		*( (Vector3*) buffer ) = positiondata[i];
		buffer += sizeof( Vector3 );

		// Set vertex normal.
		if ( format & Geometry::_FORMATFLAG_NORMAL )
		{
			*( (Vector3*) buffer ) = normaldata[i];
			buffer += sizeof( Vector3 );
		}

		// Set diffuse color.
		if ( format & Geometry::_FORMATFLAG_DIFFUSE )
		{
			*( (_dword*) buffer ) = -1;
			buffer += sizeof( _dword );
		}

		// Set specular color.
		if ( format & Geometry::_FORMATFLAG_SPECULAR )
		{
			*( (_dword*) buffer ) = 0;
			buffer += sizeof( _dword );
		}

		// Set vertex texcoord.
		for ( _dword t = 0; t < texnumber; t ++ )
		{
			*( (Vector2*) buffer ) = texcoorddata[i];
			buffer += sizeof( Vector2 );
		}

		vertexbuffer += geometry.mVertexSize;
	}

	geometry.mVertexBuffer->Unlock( );

	// Set bound box for this geometry.
	geometry.mVertexBuffer->SetBoundBox( ModelHelper::GetBoundBoxFromVertexBuffer( format, modelbuffer ) );

	if ( geometry.mIndexBuffer == _null )
		return geometry;

	// Fill vertex buffer.
	_dword indexbuffersize = geometry.mIndexBuffer->GetSize( );

	_word* indexbuffer = (_word*) geometry.mIndexBuffer->Lock( 0, 0 );

	if ( indexbuffer == _null )
	{
		ReleaseGeometry( geometry );
		return geometry;
	}

	// Front Face
	indexbuffer[ 0] = 0;
	indexbuffer[ 1] = 2;
	indexbuffer[ 2] = 1;
	indexbuffer[ 3] = 0;
	indexbuffer[ 4] = 3;
	indexbuffer[ 5] = 2;
	// Back Face
	indexbuffer[ 6] = 4;
	indexbuffer[ 7] = 6;
	indexbuffer[ 8] = 5;
	indexbuffer[ 9] = 4;
	indexbuffer[10] = 7;
	indexbuffer[11] = 6;
	// Top Face
	indexbuffer[12] = 8;
	indexbuffer[13] = 10;
	indexbuffer[14] = 9;
	indexbuffer[15] = 8;
	indexbuffer[16] = 11;
	indexbuffer[17] = 10;
	// Bottom Face
	indexbuffer[18] = 12;
	indexbuffer[19] = 14;
	indexbuffer[20] = 13;
	indexbuffer[21] = 12;
	indexbuffer[22] = 15;
	indexbuffer[23] = 14;
	// Right face
	indexbuffer[24] = 16;
	indexbuffer[25] = 18;
	indexbuffer[26] = 17;
	indexbuffer[27] = 16;
	indexbuffer[28] = 19;
	indexbuffer[29] = 18;
	// Left Face
	indexbuffer[30] = 20;
	indexbuffer[31] = 22;
	indexbuffer[32] = 21;
	indexbuffer[33] = 20;
	indexbuffer[34] = 23;
	indexbuffer[35] = 22;

	geometry.mIndexBuffer->Unlock( );

	return geometry;
}

Geometry GeometryFactory::CreatePyramidGeometry( _dword format )
{
	// Create geometry base on format.
	Geometry geometry = CreateGeometry( format, 12, 12, Geometry::_GEOMETRY_TRIANGLELIST, 4 );

	// Create geometry failed.
	if ( geometry.mVertexDeclaration == _null )
		return geometry;

	// Fill vertex buffer.
	_dword vertexbuffersize = geometry.mVertexBuffer->GetSize( );

	_byte* vertexbuffer = (_byte*) geometry.mVertexBuffer->Lock( 0, 0 );

	if ( vertexbuffer == _null )
	{
		ReleaseGeometry( geometry );
		return geometry;
	}

	ModelBuffer modelbuffer;
	modelbuffer.mBuffer = vertexbuffer;
	modelbuffer.mLength = vertexbuffersize;

	static Vector3 positiondata[] =
	{
		// Side Face 1
		Vector3(  0.0f,  1.0f,  0.0f ),
		Vector3(  1.0f, -1.0f,  1.0f ),
		Vector3( -1.0f, -1.0f,  1.0f ),
		// Side Face 2
		Vector3(  0.0f,  1.0f,  0.0f ),
		Vector3(  1.0f, -1.0f, -1.0f ),
		Vector3(  1.0f, -1.0f,  1.0f ),
		// Side Face 3
		Vector3(  0.0f,  1.0f,  0.0f ),
		Vector3( -1.0f, -1.0f, -1.0f ),
		Vector3(  1.0f, -1.0f, -1.0f ),
		// Side Face 4
		Vector3(  0.0f,  1.0f,  0.0f ),
		Vector3( -1.0f, -1.0f,  1.0f ),
		Vector3( -1.0f, -1.0f, -1.0f ),
	};

	static Vector3 normaldata[] =
	{
		// Side Face 1
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		// Side Face 2
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		// Side Face 3
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		// Side Face 4
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
		Vector3( 0.0f, 0.0f, 0.0f ),
	};

	static Vector2 texcoorddata[] =
	{
		// Side Face 1
		Vector2( 0.5f, 0.0f ),
		Vector2( 0.0f, 1.0f ),
		Vector2( 1.0f, 1.0f ),
		// Side Face 2
		Vector2( 0.5f, 0.0f ),
		Vector2( 0.0f, 1.0f ),
		Vector2( 1.0f, 1.0f ),
		// Side Face 3
		Vector2( 0.5f, 0.0f ),
		Vector2( 0.0f, 1.0f ),
		Vector2( 1.0f, 1.0f ),
		// Side Face 4
		Vector2( 0.5f, 0.0f ),
		Vector2( 0.0f, 1.0f ),
		Vector2( 1.0f, 1.0f ),
	};

	_dword texnumber = ModelHelper::GetTexcoordNumberFromFormat( format );

	for ( _dword i = 0; i < 12; i ++ )
	{
		_byte* buffer = vertexbuffer;

		// Set vertex position.
		*( (Vector3*) buffer ) = positiondata[i];
		buffer += sizeof( Vector3 );

		// Set vertex normal.
		if ( format & Geometry::_FORMATFLAG_NORMAL )
		{
			*( (Vector3*) buffer ) = normaldata[i];
			buffer += sizeof( Vector3 );
		}

		// Set diffuse color.
		if ( format & Geometry::_FORMATFLAG_DIFFUSE )
		{
			*( (_dword*) buffer ) = -1;
			buffer += sizeof( _dword );
		}

		// Set specular color.
		if ( format & Geometry::_FORMATFLAG_SPECULAR )
		{
			*( (_dword*) buffer ) = 0;
			buffer += sizeof( _dword );
		}

		// Set vertex texcoord.
		for ( _dword t = 0; t < texnumber; t ++ )
		{
			*( (Vector2*) buffer ) = texcoorddata[i];
			buffer += sizeof( Vector2 );
		}

		vertexbuffer += geometry.mVertexSize;
	}

	geometry.mVertexBuffer->Unlock( );

	// Set bound box for this geometry.
	geometry.mVertexBuffer->SetBoundBox( ModelHelper::GetBoundBoxFromVertexBuffer( format, modelbuffer ) );

	if ( geometry.mIndexBuffer == _null )
		return geometry;

	// Fill vertex buffer.
	_dword indexbuffersize = geometry.mIndexBuffer->GetSize( );

	_word* indexbuffer = (_word*) geometry.mIndexBuffer->Lock( 0, 0 );

	if ( indexbuffer == _null )
	{
		ReleaseGeometry( geometry );
		return geometry;
	}

	for ( _dword i = 0; i < 12; i ++ )
		indexbuffer[i] = (_word) i;

	geometry.mIndexBuffer->Unlock( );

	return geometry;
}

Geometry GeometryFactory::CreateSphereGeometry( _dword format, _dword ringnumber, _dword segmentnumber )
{
	if ( ringnumber == 0 && segmentnumber == 0 )
		return Geometry( );

	_dword vertexnumber = ringnumber * ( segmentnumber + 1 ) * 2;

	// Create geometry base on format.
	Geometry geometry = CreateGeometry( format, vertexnumber, 0, Geometry::_GEOMETRY_TRIANGLESTRIP, vertexnumber - 2 );

	// Create geometry failed.
	if ( geometry.mVertexDeclaration == _null )
		return geometry;

	// Fill vertex buffer.
	_dword vertexbuffersize = geometry.mVertexBuffer->GetSize( );

	_byte* vertexbuffer = (_byte*) geometry.mVertexBuffer->Lock( 0, 0 );

	if ( vertexbuffer == _null )
	{
		ReleaseGeometry( geometry );
		return geometry;
	}

	ModelBuffer modelbuffer;
	modelbuffer.mBuffer = vertexbuffer;
	modelbuffer.mLength = vertexbuffersize;

	_dword texnumber = ModelHelper::GetTexcoordNumberFromFormat( format );

	_float ringangle = Math::cPi / ringnumber, segmentangle = Math::cPi / segmentnumber * 2.0f;

	for ( _dword i = 0; i < ringnumber; i ++ )
	{
		_float r0 = Math::Sin( ( i + 0 ) * ringangle );
		_float r1 = Math::Sin( ( i + 1 ) * ringangle );
		_float y0 = Math::Cos( ( i + 0 ) * ringangle );
		_float y1 = Math::Cos( ( i + 1 ) * ringangle );

		for ( _dword j = 0; j <= segmentnumber; j ++ )
		{
			_float x0 = r0 * Math::Sin( j * segmentangle );
			_float z0 = r0 * Math::Cos( j * segmentangle );
			_float x1 = r1 * Math::Sin( j * segmentangle );
			_float z1 = r1 * Math::Cos( j * segmentangle );

			_byte* buffer1 = vertexbuffer;
			_byte* buffer2 = vertexbuffer + geometry.mVertexSize;

			// Set vertex position.
			*( (Vector3*) buffer1 ) = Vector3( z0, x0, y0 );
			buffer1 += sizeof( Vector3 );
			*( (Vector3*) buffer2 ) = Vector3( z1, x1, y1 );
			buffer2 += sizeof( Vector3 );

			// Set vertex normal.
			if ( format & Geometry::_FORMATFLAG_NORMAL )
			{
				*( (Vector3*) buffer1 ) = Vector3( z0, x0, y0 );
				buffer1 += sizeof( Vector3 );
				*( (Vector3*) buffer2 ) = Vector3( z1, x1, y1 );
				buffer2 += sizeof( Vector3 );
			}

			// Set diffuse color.
			if ( format & Geometry::_FORMATFLAG_DIFFUSE )
			{
				*( (_dword*) buffer1 ) = -1;
				buffer1 += sizeof( _dword );
				*( (_dword*) buffer2 ) = -1;
				buffer2 += sizeof( _dword );
			}

			// Set specular color.
			if ( format & Geometry::_FORMATFLAG_SPECULAR )
			{
				*( (_dword*) buffer1 ) = 0;
				buffer1 += sizeof( _dword );
				*( (_dword*) buffer2 ) = 0;
				buffer2 += sizeof( _dword );
			}

			// Set vertex texcoord.
			for ( _dword t = 0; t < texnumber; t ++ )
			{
				*( (Vector2*) buffer1 ) = Vector2( 1.0f - (_float) j / segmentnumber, (_float) ( i + 0 ) / ringnumber );
				buffer1 += sizeof( Vector2 );
				*( (Vector2*) buffer2 ) = Vector2( 1.0f - (_float) j / segmentnumber, (_float) ( i + 1 ) / ringnumber );
				buffer2 += sizeof( Vector2 );
			}

			vertexbuffer += geometry.mVertexSize * 2;
		}
	}

	geometry.mVertexBuffer->Unlock( );

	// Set bound box for this geometry.
	geometry.mVertexBuffer->SetBoundBox( ModelHelper::GetBoundBoxFromVertexBuffer( format, modelbuffer ) );

	return geometry;
}

Geometry GeometryFactory::CreateCylinderGeometry( _dword format, _dword segmentnumber )
{
	if ( segmentnumber == 0 )
		return Geometry( );

	_dword vertexnumber	= ( segmentnumber + 1 ) * 4 + 2;
	_dword indexnumber	= segmentnumber * 12;

	// Create geometry base on format.
	Geometry geometry = CreateGeometry( format, vertexnumber, indexnumber, Geometry::_GEOMETRY_TRIANGLELIST, segmentnumber * 4 );

	// Create geometry failed.
	if ( geometry.mVertexDeclaration == _null )
		return geometry;

	// Fill vertex buffer.
	_dword vertexbuffersize = geometry.mVertexBuffer->GetSize( );

	_byte* vertexbuffer = (_byte*) geometry.mVertexBuffer->Lock( 0, 0 );

	if ( vertexbuffer == _null )
	{
		ReleaseGeometry( geometry );
		return geometry;
	}

	ModelBuffer modelbuffer;
	modelbuffer.mBuffer = vertexbuffer;
	modelbuffer.mLength = vertexbuffersize;

	// Fill vertex buffer.
	_dword indexbuffersize = geometry.mIndexBuffer->GetSize( );

	_word* indexbuffer = (_word*) geometry.mIndexBuffer->Lock( 0, 0 );

	if ( indexbuffer == _null )
	{
		ReleaseGeometry( geometry );
		return geometry;
	}

	_dword texnumber = ModelHelper::GetTexcoordNumberFromFormat( format );

	// 1. Set top center vector.

	// Set vertex position.
	*( (Vector3*) vertexbuffer ) = Vector3( 0.0f, 0.0f, 0.5f );
	vertexbuffer += sizeof( Vector3 );

	// Set vertex normal.
	if ( format & Geometry::_FORMATFLAG_NORMAL )
	{
		*( (Vector3*) vertexbuffer ) = Vector3( 0.0f, 0.0f, 1.0f );
		vertexbuffer += sizeof( Vector3 );
	}

	// Set diffuse color.
	if ( format & Geometry::_FORMATFLAG_DIFFUSE )
	{
		*( (_dword*) vertexbuffer ) = -1;
		vertexbuffer += sizeof( _dword );
	}

	// Set specular color.
	if ( format & Geometry::_FORMATFLAG_SPECULAR )
	{
		*( (_dword*) vertexbuffer ) = 0;
		vertexbuffer += sizeof( _dword );
	}

	// Set vertex texcoord.
	for ( _dword t = 0; t < texnumber; t ++ )
	{
		*( (Vector2*) vertexbuffer ) = Vector2( 0.5f, 1.0f );
		vertexbuffer += sizeof( Vector2 );
	}

	// 2. Set bottom center vector.

	// Set vertex position.
	*( (Vector3*) vertexbuffer ) = Vector3( 0.0f, 0.0f, -0.5f );
	vertexbuffer += sizeof( Vector3 );

	// Set vertex normal.
	if ( format & Geometry::_FORMATFLAG_NORMAL )
	{
		*( (Vector3*) vertexbuffer ) = Vector3( 0.0f, 0.0f, -1.0f );
		vertexbuffer += sizeof( Vector3 );
	}

	// Set diffuse color.
	if ( format & Geometry::_FORMATFLAG_DIFFUSE )
	{
		*( (_dword*) vertexbuffer ) = -1;
		vertexbuffer += sizeof( _dword );
	}

	// Set specular color.
	if ( format & Geometry::_FORMATFLAG_SPECULAR )
	{
		*( (_dword*) vertexbuffer ) = 0;
		vertexbuffer += sizeof( _dword );
	}

	// Set vertex texcoord.
	for ( _dword t = 0; t < texnumber; t ++ )
	{
		*( (Vector2*) vertexbuffer ) = Vector2( 0.5f, 0.0f );
		vertexbuffer += sizeof( Vector2 );
	}

	_float segmentangle = Math::cPi / segmentnumber * 2.0f;

	// 3. Set ring vector.
	for ( _dword i = 0; i <= segmentnumber; i ++ )
	{
		_float x = Math::Sin( i * segmentangle );
		_float y = Math::Cos( i * segmentangle );
		_float u = (_float) i / (_float) segmentnumber;

		_byte* buffer1 = vertexbuffer;
		_byte* buffer2 = vertexbuffer + geometry.mVertexSize * ( segmentnumber + 1 );
		_byte* buffer3 = vertexbuffer + geometry.mVertexSize * ( segmentnumber + 1 ) * 2;
		_byte* buffer4 = vertexbuffer + geometry.mVertexSize * ( segmentnumber + 1 ) * 3;

		// Set vertex position.
		*( (Vector3*) buffer1 ) = Vector3( x, y,  0.5f );
		buffer1 += sizeof( Vector3 );
		*( (Vector3*) buffer2 ) = Vector3( x, y, -0.5f );
		buffer2 += sizeof( Vector3 );
		*( (Vector3*) buffer3 ) = Vector3( x, y,  0.5f );
		buffer3 += sizeof( Vector3 );
		*( (Vector3*) buffer4 ) = Vector3( x, y, -0.5f );
		buffer4 += sizeof( Vector4 );

		// Set vertex normal.
		if ( format & Geometry::_FORMATFLAG_NORMAL )
		{
			*( (Vector3*) buffer1 ) = Vector3( 0.0f, 0.0f,  1.0f );
			buffer1 += sizeof( Vector3 );
			*( (Vector3*) buffer2 ) = Vector3( 0.0f, 0.0f, -1.0f );
			buffer2 += sizeof( Vector3 );
			*( (Vector3*) buffer3 ) = Vector3( x, y, 0.0f );
			buffer3 += sizeof( Vector3 );
			*( (Vector3*) buffer4 ) = Vector3( x, y, 0.0f );
			buffer4 += sizeof( Vector3 );
		}

		// Set diffuse color.
		if ( format & Geometry::_FORMATFLAG_DIFFUSE )
		{
			*( (_dword*) buffer1 ) = -1;
			buffer1 += sizeof( _dword );
			*( (_dword*) buffer2 ) = -1;
			buffer2 += sizeof( _dword );
			*( (_dword*) buffer3 ) = -1;
			buffer3 += sizeof( _dword );
			*( (_dword*) buffer4 ) = -1;
			buffer4 += sizeof( _dword );
		}

		// Set specular color.
		if ( format & Geometry::_FORMATFLAG_SPECULAR )
		{
			*( (_dword*) buffer1 ) = 0;
			buffer1 += sizeof( _dword );
			*( (_dword*) buffer2 ) = 0;
			buffer2 += sizeof( _dword );
			*( (_dword*) buffer3 ) = 0;
			buffer3 += sizeof( _dword );
			*( (_dword*) buffer4 ) = 0;
			buffer4 += sizeof( _dword );
		}

		// Set vertex texcoord.
		for ( _dword t = 0; t < texnumber; t ++ )
		{
			*( (Vector2*) buffer1 ) = Vector2( u, 0.0f );
			buffer1 += sizeof( Vector2 );
			*( (Vector2*) buffer2 ) = Vector2( u, 1.0f );
			buffer2 += sizeof( Vector2 );
			*( (Vector2*) buffer3 ) = Vector2( u, 0.0f );
			buffer3 += sizeof( Vector2 );
			*( (Vector2*) buffer4 ) = Vector2( u, 1.0f );
			buffer4 += sizeof( Vector2 );
		}

		if ( i < segmentnumber )
		{
			// 1. Top face.
			indexbuffer[ 0] = (_word) 0;
			indexbuffer[ 1] = (_word) ( i + 3 );
			indexbuffer[ 2] = (_word) ( i + 2 );

			// 2. Bottom face.
			indexbuffer[ 3] = (_word) 1;
			indexbuffer[ 4] = (_word) ( i + 2 + segmentnumber + 1 );
			indexbuffer[ 5] = (_word) ( i + 3 + segmentnumber + 1 );

			// 3. Side face A.
			indexbuffer[ 6] = (_word) ( i + 2 );
			indexbuffer[ 7] = (_word) ( i + 3 );
			indexbuffer[ 8] = (_word) ( i + 2 + segmentnumber + 1 );

			// 4. Side face B.
			indexbuffer[ 9] = (_word) ( i + 3 );
			indexbuffer[10] = (_word) ( i + 3 + segmentnumber + 1 );
			indexbuffer[11] = (_word) ( i + 2 + segmentnumber + 1 );

			indexbuffer  += 12;
		}

		vertexbuffer += geometry.mVertexSize;
	}

	geometry.mVertexBuffer->Unlock( );
	geometry.mIndexBuffer->Unlock( );

	// Set bound box for this geometry.
	geometry.mVertexBuffer->SetBoundBox( ModelHelper::GetBoundBoxFromVertexBuffer( format, modelbuffer ) );

	return geometry;
}

Geometry GeometryFactory::CreateTeapotGeometry( _dword format )
{
	_dword vertexnumber	= 1178, indexnumber = 6768;

	// Create geometry base on format.
	Geometry geometry = CreateGeometry( format, vertexnumber, indexnumber, Geometry::_GEOMETRY_TRIANGLELIST, indexnumber / 3 );

	// Create geometry failed.
	if ( geometry.mVertexDeclaration == _null )
		return geometry;

	const _byte* teapotbuffer = GeometryData::cTeapotVertices;

	// Fill vertex buffer.
	_byte* vertexbuffer = (_byte*) geometry.mVertexBuffer->Lock( 0, 0 );

	if ( vertexbuffer != _null )
	{
		for ( _dword i = 0; i < vertexnumber; i ++ )
		{
			Vector3* v1 = (Vector3*) teapotbuffer;
			Vector3* v2 = (Vector3*) vertexbuffer;

			// Copy vertex position.
			v2->x = v1->x; v2->z = v1->y; v2->y = -v1->z;

			v1 ++; v2 ++;

			// Copy vertex normal.
			if ( format & Geometry::_FORMATFLAG_NORMAL )
			{
				v2->x = v1->x; v2->z = v1->y; v2->y = -v1->z;

				v1 ++; v2 ++;
			}

			teapotbuffer += 24;
			vertexbuffer += geometry.mVertexSize;
		}
	}

	geometry.mVertexBuffer->Unlock( );

	// Fill index buffer.
	_byte* indexbuffer = (_byte*) geometry.mIndexBuffer->Lock( 0, 0 );

	if ( indexbuffer != _null )
		Memory::MemCpy( indexbuffer, GeometryData::cTeapotIndices, indexnumber * sizeof( _word ) );

	geometry.mIndexBuffer->Unlock( );

	return geometry;
}

Geometry GeometryFactory::CloneGeometry( const Geometry& geometry, _bool reference )
{
	Geometry newgeometry = geometry;

	// Increase vertex declaration reference.
	if ( newgeometry.mVertexDeclaration != _null )
		( (VertexDeclaration*) newgeometry.mVertexDeclaration )->IncreaseRefCount( );

	if ( reference )
	{
		// Increase vertex buffer reference.
		if ( newgeometry.mVertexBuffer != _null )
			( (VertexBuffer*) newgeometry.mVertexBuffer )->IncreaseRefCount( );

		// Increase vertex buffer reference.
		if ( newgeometry.mBackupBuffer != _null )
			( (VertexBuffer*) newgeometry.mBackupBuffer )->IncreaseRefCount( );
	}
	else
	{
		newgeometry.mVertexBuffer = CloneVertexBuffer( geometry.mVertexBuffer, _false );
		newgeometry.mBackupBuffer = CloneVertexBuffer( geometry.mBackupBuffer, _false );
	}

	// Increase helper buffer reference.
	if ( newgeometry.mHelperBuffer != _null )
		( (VertexBuffer*) newgeometry.mHelperBuffer )->IncreaseRefCount( );

	// Increase index buffer reference.
	if ( newgeometry.mIndexBuffer != _null )
		( (IndexBuffer*) newgeometry.mIndexBuffer )->IncreaseRefCount( );

	// Increase texture reference.
	for ( _dword i = 0; i < Geometry::_MAX_TEXTURE_NUMBER; i ++ )
	{
		if ( newgeometry.mTexture[i] != _null )
			( (Texture*) newgeometry.mTexture[i] )->IncreaseRefCount( );
	}

	return newgeometry;
}

_void GeometryFactory::ReleaseGeometry( Geometry& geometry )
{
	// Release vertex declaration.
	if ( geometry.mVertexDeclaration != _null )
		ReleaseVertexDeclaration( geometry.mVertexDeclaration );

	// Release vertex buffer.
	if ( geometry.mVertexBuffer != _null )
		ReleaseVertexBuffer( geometry.mVertexBuffer );

	// Release backup buffer.
	if ( geometry.mBackupBuffer != _null )
		ReleaseVertexBuffer( geometry.mBackupBuffer );

	// Release helper buffer.
	if ( geometry.mHelperBuffer != _null )
		ReleaseVertexBuffer( geometry.mHelperBuffer );

	// Release index buffer.
	if ( geometry.mIndexBuffer != _null )
		ReleaseIndexBuffer( geometry.mIndexBuffer );

	// Release textures.
	for ( _dword i = 0; i < Geometry::_MAX_TEXTURE_NUMBER; i ++ )
	{
		if ( geometry.mTexture[i] != _null )
			GetTextureFactory( ).ReleaseTexture( geometry.mTexture[i] );
	}

	// Reset render primitive type.
	geometry.mGeometryType		= 0;
	geometry.mGeometryNumber	= 0;
	geometry.mVertexSize		= 0;
	geometry.mVertexNumber		= 0;
	geometry.mStartVertex		= 0;
	geometry.mStartIndex		= 0;
}