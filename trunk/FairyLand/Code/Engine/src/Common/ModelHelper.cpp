//============================================================================
// ModelHelper.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// ModelHelper Implementation
//----------------------------------------------------------------------------

String ModelHelper::GetGroupFromName( StringPtr meshname )
{
	String groupname;

	// WallXXX
	_dword index = meshname.SearchL2R( L"wall" );
	if ( index != -1 && meshname.Length( ) >= index + 4 )
		groupname += String( L"wall" ) + Long( (const _char*) meshname + index + 4 ).ToString( );

	// DecoratorXXX
	index = meshname.SearchL2R( L"decorator" );
	if ( index != -1 && meshname.Length( ) >= index + 9 )
		groupname += String( L"decorator" ) + Long( (const _char*) meshname + index + 9 ).ToString( );

	// Floor
	index = meshname.SearchL2R( L"floor" );
	if ( index != -1 )
		groupname += String( L"floor" );

	// Detail
	index = meshname.SearchL2R( L"detail" );
	if ( index != -1 )
		groupname += String( L"detail" );

	// Skip
	index = meshname.SearchL2R( L"skip" );
	if ( index != -1 )
		groupname += String( L"skip" );

	return groupname;
}

_dword ModelHelper::GetRenderLayerFromName( StringPtr meshname )
{
	_dword layer = 0, index = -1;

	if ( meshname.SearchL2R( L"layera" ) != -1 )
		return 1;
	if ( meshname.SearchL2R( L"layerb" ) != -1 )
		return 2;
	if ( meshname.SearchL2R( L"layerc" ) != -1 )
		return 3;
	if ( meshname.SearchL2R( L"layerd" ) != -1 )
		return 4;
	if ( meshname.SearchL2R( L"layere" ) != -1 )
		return 5;
	if ( meshname.SearchL2R( L"layerf" ) != -1 )
		return 6;
	if ( meshname.SearchL2R( L"layerg" ) != -1 )
		return 7;
	if ( meshname.SearchL2R( L"layerh" ) != -1 )
		return 8;
	if ( meshname.SearchL2R( L"layeri" ) != -1 )
		return 9;
	if ( meshname.SearchL2R( L"layerj" ) != -1 )
		return 10;
	if ( meshname.SearchL2R( L"layerk" ) != -1 )
		return 11;
	if ( meshname.SearchL2R( L"layerl" ) != -1 )
		return 12;
	if ( meshname.SearchL2R( L"layerm" ) != -1 )
		return 13;
	if ( meshname.SearchL2R( L"layern" ) != -1 )
		return 14;
	if ( meshname.SearchL2R( L"layero" ) != -1 )
		return 15;
	if ( meshname.SearchL2R( L"layerp" ) != -1 )
		return 16;
	if ( meshname.SearchL2R( L"layerq" ) != -1 )
		return 17;
	if ( meshname.SearchL2R( L"layerr" ) != -1 )
		return 18;
	if ( meshname.SearchL2R( L"layers" ) != -1 )
		return 19;
	if ( meshname.SearchL2R( L"layert" ) != -1 )
		return 20;
	if ( meshname.SearchL2R( L"layeru" ) != -1 )
		return 21;
	if ( meshname.SearchL2R( L"layerv" ) != -1 )
		return 22;
	if ( meshname.SearchL2R( L"layerw" ) != -1 )
		return 23;
	if ( meshname.SearchL2R( L"layerx" ) != -1 )
		return 24;
	if ( meshname.SearchL2R( L"layery" ) != -1 )
		return 25;
	if ( meshname.SearchL2R( L"layerz" ) != -1 )
		return 26;

	index = meshname.SearchL2R( L"alpha_" );
	if ( index != -1 )
	{
		layer = Long( (const _char*) meshname + index + 6 );

		if ( layer > 20 )
			layer = 20;

		return layer;
	}

	index = meshname.SearchL2R( L"alpha" );
	if ( index != -1 )
	{
		layer = Long( (const _char*) meshname + index + 5 );

		if ( layer > 20 )
			layer = 20;

		return layer;
	}

	return layer;
}

_dword ModelHelper::GetRenderFlagFromName( StringPtr meshname )
{
	_dword flag = 0;

	if ( meshname.SearchL2R( L"invisible" ) != -1 || meshname.SearchL2R( L"norender" ) != -1 )
		flag |= Geometry::_RENDERFLAG_INVISIBLE;

	if ( meshname.SearchL2R( L"decal" ) != -1 )
		flag |= Geometry::_RENDERFLAG_DECAL;

	if ( meshname.SearchL2R( L"alpha" ) != -1 )
		flag |= Geometry::_RENDERFLAG_ALPHA;
	else if ( meshname.SearchL2R( L"colorkey" ) != -1 )
		flag |= Geometry::_RENDERFLAG_COLORKEY;

	if ( meshname.SearchL2R( L"billboardx" ) != -1 )
		flag |= Geometry::_RENDERFLAG_BILLBOARDX;
	else if ( meshname.SearchL2R( L"billboardy" ) != -1 )
		flag |= Geometry::_RENDERFLAG_BILLBOARDY;
	else if ( meshname.SearchL2R( L"billboardz" ) != -1 )
		flag |= Geometry::_RENDERFLAG_BILLBOARDZ;
	else if ( meshname.SearchL2R( L"billboard" ) != -1 )
		flag |= Geometry::_RENDERFLAG_BILLBOARD;

	if ( meshname.SearchL2R( L"additive" ) != -1 )
		flag |= Geometry::_RENDERFLAG_ADDITIVE;
	else if ( meshname.SearchL2R( L"subtractive" ) != -1 )
		flag |= Geometry::_RENDERFLAG_SUBTRACTIVE;

	return flag;
}

_dword ModelHelper::GetVertexSizeFromFormat( _dword format )
{
	_dword size = 0;

	if ( format & Geometry::_FORMATFLAG_POSITIONT )
		size = sizeof( Vector4 );
	else
		size = sizeof( Vector3 );

	if ( format & Geometry::_FORMATFLAG_NORMAL )
		size += sizeof( Vector3 );

	if ( format & Geometry::_FORMATFLAG_DIFFUSE )
		size += sizeof( _dword );

	if ( format & Geometry::_FORMATFLAG_SPECULAR )
		size += sizeof( _dword );

	if ( format & Geometry::_FORMATFLAG_TEXCOORD4 )
		size += sizeof( Vector2 ) * 4;
	else if ( format & Geometry::_FORMATFLAG_TEXCOORD3 )
		size += sizeof( Vector2 ) * 3;
	else if ( format & Geometry::_FORMATFLAG_TEXCOORD2 )
		size += sizeof( Vector2 ) * 2;
	else if ( format & Geometry::_FORMATFLAG_TEXCOORD1 )
		size += sizeof( Vector2 ) * 1;

	if ( format & Geometry::_FORMATFLAG_BINORMAL )
		size += sizeof( Vector3 );

	if ( format & Geometry::_FORMATFLAG_TANGENT )
		size += sizeof( Vector3 );

	return size;
}

_dword ModelHelper::GetHelperSizeFromFormat( _dword format )
{
	if ( format & Geometry::_FORMATFLAG_INFLUENCE )
	{
		_dword size = sizeof( Vector3 ) + sizeof( ModelInfluenceGroup );

		if ( format & Geometry::_FORMATFLAG_NORMAL )
			size += sizeof( Vector3 );

		return size;
	}
	else
	{
		return 0;
	}
}

_dword ModelHelper::GetHelperRefSizeFromFormat( _dword format )
{
	_dword size = sizeof( Vector3 );

	if ( format & Geometry::_FORMATFLAG_NORMAL )
		size += sizeof( Vector3 );

	return size;
}

_dword ModelHelper::GetMathDataSizeFromFormat( _dword format )
{
	_dword size = 0;

	if ( format & Geometry::_FORMATFLAG_BINORMAL )
		size += sizeof( Vector3 );

	if ( format & Geometry::_FORMATFLAG_TANGENT )
		size += sizeof( Vector3 );

	return size;
}

_dword ModelHelper::GetAnimationSizeFromFormat( _dword format )
{
	_dword size = 0;

	if ( format & Geometry::_FORMATFLAG_POSITION )
		size += sizeof( Vector3 );

	if ( format & Geometry::_FORMATFLAG_NORMAL )
		size += sizeof( Vector3 );

	if ( format & Geometry::_FORMATFLAG_DIFFUSE )
		size += sizeof( _dword );

	if ( format & Geometry::_FORMATFLAG_SPECULAR )
		size += sizeof( _dword );

	if ( format & Geometry::_FORMATFLAG_TEXCOORD1 )
		size += sizeof( Vector2 );

	if ( format & Geometry::_FORMATFLAG_TEXCOORD2 )
		size += sizeof( Vector2 );

	if ( format & Geometry::_FORMATFLAG_TEXCOORD3 )
		size += sizeof( Vector2 );

	if ( format & Geometry::_FORMATFLAG_TEXCOORD4 )
		size += sizeof( Vector2 );

	return size;
}

_dword ModelHelper::GetTexcoordNumberFromFormat( _dword format )
{
	_dword texnumber = 0;

	if ( format & Geometry::_FORMATFLAG_TEXCOORD4 )
		texnumber = 4;
	else if ( format & Geometry::_FORMATFLAG_TEXCOORD3 )
		texnumber = 3;
	else if ( format & Geometry::_FORMATFLAG_TEXCOORD2 )
		texnumber = 2;
	else if ( format & Geometry::_FORMATFLAG_TEXCOORD1 )
		texnumber = 1;

	return texnumber;
}

_bool ModelHelper::CheckVertexFormatVSAnimationFormat( _dword vertexformat, _dword animationformat )
{
	// Not support binormal now.
	if ( animationformat & Geometry::_FORMATFLAG_BINORMAL )
		return _false;

	// Not support tangent now.
	if ( animationformat & Geometry::_FORMATFLAG_TANGENT )
		return _false;

	// Not support influence now.
	if ( animationformat & Geometry::_FORMATFLAG_INFLUENCE )
		return _false;

	// Check normal.
	if ( animationformat & Geometry::_FORMATFLAG_NORMAL )
	{
		if ( ( vertexformat & Geometry::_FORMATFLAG_NORMAL ) == _false )
			return _false;
	}

	// Check diffuse.
	if ( animationformat & Geometry::_FORMATFLAG_DIFFUSE )
	{
		if ( ( vertexformat & Geometry::_FORMATFLAG_DIFFUSE ) == _false )
			return _false;
	}

	// Check specular.
	if ( animationformat & Geometry::_FORMATFLAG_SPECULAR )
	{
		if ( ( vertexformat & Geometry::_FORMATFLAG_SPECULAR ) == _false )
			return _false;
	}

	_dword texnumber = GetTexcoordNumberFromFormat( vertexformat );

	// Check texcoord1.
	if ( animationformat & Geometry::_FORMATFLAG_TEXCOORD1 )
	{
		if ( texnumber < 1 )
			return _false;
	}

	// Check texcoord2.
	if ( animationformat & Geometry::_FORMATFLAG_TEXCOORD2 )
	{
		if ( texnumber < 2 )
			return _false;
	}

	// Check texcoord3.
	if ( animationformat & Geometry::_FORMATFLAG_TEXCOORD3 )
	{
		if ( texnumber < 3 )
			return _false;
	}

	// Check texcoord4.
	if ( animationformat & Geometry::_FORMATFLAG_TEXCOORD4 )
	{
		if ( texnumber < 4 )
			return _false;
	}

	return _true;
}

_bool ModelHelper::CompareVertex( _dword format, const _byte* vertex1, const _byte* vertex2, _float epsilon )
{
	// Compare position.
	if ( ( (Vector3*) vertex1 )->Equal( *( (Vector3*) vertex2 ), epsilon ) == _false )
		return _false;

	vertex1 += sizeof( Vector3 );
	vertex2 += sizeof( Vector3 );

	// Compare normal.
	if ( format & Geometry::_FORMATFLAG_NORMAL )
	{
		if ( ( (Vector3*) vertex1 )->Equal( *( (Vector3*) vertex2 ), epsilon ) == _false )
			return _false;

		vertex1 += sizeof( Vector3 );
		vertex2 += sizeof( Vector3 );
	}

	// Compare normal.
	if ( format & Geometry::_FORMATFLAG_NORMAL )
	{
		if ( ( (Vector3*) vertex1 )->Equal( *( (Vector3*) vertex2 ), epsilon ) == _false )
			return _false;

		vertex1 += sizeof( Vector3 );
		vertex2 += sizeof( Vector3 );
	}

	// Compare diffuse.
	if ( format & Geometry::_FORMATFLAG_DIFFUSE )
	{
		if ( *( (_dword*) vertex1 ) != *( (_dword*) vertex2 ) )
			return _false;

		vertex1 += sizeof( _dword );
		vertex2 += sizeof( _dword );
	}

	// Compare specular.
	if ( format & Geometry::_FORMATFLAG_SPECULAR )
	{
		if ( *( (_dword*) vertex1 ) != *( (_dword*) vertex2 ) )
			return _false;

		vertex1 += sizeof( _dword );
		vertex2 += sizeof( _dword );
	}

	// Compare texcoord.
	_dword texnumber = 0;

	if ( format & Geometry::_FORMATFLAG_TEXCOORD4 )
		texnumber = 4;
	else if ( format & Geometry::_FORMATFLAG_TEXCOORD3 )
		texnumber = 3;
	else if ( format & Geometry::_FORMATFLAG_TEXCOORD2 )
		texnumber = 2;
	else if ( format & Geometry::_FORMATFLAG_TEXCOORD1 )
		texnumber = 1;

	for ( _dword i = 0; i < texnumber; i ++ )
	{
		if ( ( (Vector2*) vertex1 )->Equal( *( (Vector2*) vertex2 ), epsilon ) == _false )
			return _false;

		vertex1 += sizeof( Vector2 );
		vertex2 += sizeof( Vector2 );
	}

	// Compare binormal.
	if ( format & Geometry::_FORMATFLAG_BINORMAL )
	{
		if ( ( (Vector3*) vertex1 )->Equal( *( (Vector3*) vertex2 ), epsilon ) == _false )
			return _false;

		vertex1 += sizeof( Vector3 );
		vertex2 += sizeof( Vector3 );
	}

	// Compare tangent.
	if ( format & Geometry::_FORMATFLAG_TANGENT )
	{
		if ( ( (Vector3*) vertex1 )->Equal( *( (Vector3*) vertex2 ), epsilon ) == _false )
			return _false;

		vertex1 += sizeof( Vector3 );
		vertex2 += sizeof( Vector3 );
	}

	return _true;
}

_void ModelHelper::CombineVertexArray( _dword format, ModelBuffer& vertexbuffer, ModelBuffer& helperbuffer, ModelBuffer& indexbuffer, _dword& vertexnumber )
{
	_dword vertexsize = GetVertexSizeFromFormat( format );
	_dword helpersize = GetHelperSizeFromFormat( format );

	for ( _dword i = 0; i < vertexbuffer.mLength / vertexsize; i ++ )
	{
		_byte* vertex1 = vertexbuffer.mBuffer + i * vertexsize;

		for ( _dword j = i + 1; j < vertexbuffer.mLength / vertexsize; j ++ )
		{
			_byte* vertex2 = vertexbuffer.mBuffer + j * vertexsize;
			_byte* vertex3 = helperbuffer.mBuffer + j * helpersize;

			// Can not combine.
			if ( CompareVertex( format, vertex1, vertex2 ) == _false )
				continue;

			// Move vertex buffer and helper buffer forward, replace the combined vertex.
			if ( j != vertexbuffer.mLength / vertexsize )
			{
				Memory::MemMove( vertex2, vertex2 + vertexsize, (_dword)( ( vertexbuffer.mBuffer + vertexbuffer.mLength ) - ( vertex2 + vertexsize ) ) );

				if ( helperbuffer.mBuffer != _null )
					Memory::MemMove( vertex3, vertex3 + helpersize, (_dword)( ( helperbuffer.mBuffer + helperbuffer.mLength ) - ( vertex3 + helpersize ) ) );
			}

			// Decrease vertex number.
			vertexbuffer.mLength -= vertexsize;
			helperbuffer.mLength -= helpersize;

			// Update index buffer.
			for ( _dword k = 0; k < indexbuffer.mLength / sizeof( _word ); k ++ )
			{
				_word& index = ( (_word*) indexbuffer.mBuffer )[k];

				if ( index == (_word) j )
					index = (_word) i;
				else if ( index > (_word ) j )
					index --;
			}

			j --;
		}
	}

	vertexnumber = vertexbuffer.mLength / vertexsize;
}

_bool ModelHelper::CompareMesh( const ModelMesh& mesh1, const ModelMesh& mesh2 )
{
	// Texture not same, can not combine.
	if ( mesh1.mTexName1 != mesh2.mTexName1 )
		return _false;
	if ( mesh1.mTexName2 != mesh2.mTexName2 )
		return _false;
	if ( mesh1.mTexName3 != mesh2.mTexName3 )
		return _false;
	if ( mesh1.mTexName4 != mesh2.mTexName4 )
		return _false;

	// Group not same, can not combine.
	if ( mesh1.mGroupName != mesh2.mGroupName )
		return _false;

	// Format flag not same, can not combine.
	if ( mesh1.mFormatFlag != mesh2.mFormatFlag )
		return _false;

	// Render flag not same, can not combine.
	if ( mesh1.mRenderFlag != mesh2.mRenderFlag )
		return _false;

	// One of mesh is invisible, and another is not, can not combine.
	if ( ( mesh1.mRenderFlag & Geometry::_RENDERFLAG_INVISIBLE ) != ( mesh2.mRenderFlag & Geometry::_RENDERFLAG_INVISIBLE ) )
		return _false;

	// One of mesh is decal, and another is not, can not combine.
	if ( ( mesh1.mRenderFlag & Geometry::_RENDERFLAG_DECAL ) != ( mesh2.mRenderFlag & Geometry::_RENDERFLAG_DECAL ) )
		return _false;

	// One of mesh is alpha, can not combine.
	if ( ( mesh1.mRenderFlag & Geometry::_RENDERFLAG_ALPHA ) || ( mesh2.mRenderFlag & Geometry::_RENDERFLAG_ALPHA ) )
		return _false;

	// One of mesh is billboard, can not combine.
	if ( ( mesh1.mRenderFlag & Geometry::_RENDERFLAG_BILLBOARD ) || ( mesh2.mRenderFlag & Geometry::_RENDERFLAG_BILLBOARD ) )
		return _false;

	// One of mesh is billboardx, can not combine.
	if ( ( mesh1.mRenderFlag & Geometry::_RENDERFLAG_BILLBOARDX ) || ( mesh2.mRenderFlag & Geometry::_RENDERFLAG_BILLBOARDX ) )
		return _false;

	// One of mesh is billboardy, can not combine.
	if ( ( mesh1.mRenderFlag & Geometry::_RENDERFLAG_BILLBOARDY ) || ( mesh2.mRenderFlag & Geometry::_RENDERFLAG_BILLBOARDY ) )
		return _false;

	// One of mesh is billboardz, can not combine.
	if ( ( mesh1.mRenderFlag & Geometry::_RENDERFLAG_BILLBOARDZ ) || ( mesh2.mRenderFlag & Geometry::_RENDERFLAG_BILLBOARDZ ) )
		return _false;

	// One of mesh is additive, can not combine.
	if ( ( mesh1.mRenderFlag & Geometry::_RENDERFLAG_ADDITIVE ) || ( mesh2.mRenderFlag & Geometry::_RENDERFLAG_ADDITIVE ) )
		return _false;

	// One of mesh is subtractive, can not combine.
	if ( ( mesh1.mRenderFlag & Geometry::_RENDERFLAG_SUBTRACTIVE ) || ( mesh2.mRenderFlag & Geometry::_RENDERFLAG_SUBTRACTIVE ) )
		return _false;

	return _true;
}

_void ModelHelper::CombineMeshArray( Array< ModelMesh >& mesharray )
{
	// Try to combine meshes.
	for ( _dword i = 0; i < mesharray.Number( ); i ++ )
	{
		ModelMesh& mesh1 = mesharray[i];

		for ( _dword j = i + 1; j < mesharray.Number( ); j ++ )
		{
			ModelMesh& mesh2 = mesharray[j];

			// Can not combine, skip it.
			if ( CompareMesh( mesh1, mesh2 ) == _false )
				continue;

			// Reassign index buffer.
			for ( _dword k = 0; k < mesh2.mIndexBuffer.mLength / sizeof( _word ); k ++ )
				( (_word*) mesh2.mIndexBuffer.mBuffer )[k] += (_word) mesh1.mVertexNumber;

			mesh1.mVertexNumber	+= mesh2.mVertexNumber;
			mesh1.mFaceNumber	+= mesh2.mFaceNumber;

			// Combine vertex buffer.
			_byte* vertexbuffer = new _byte[ mesh1.mVertexBuffer.mLength + mesh2.mVertexBuffer.mLength ];

			Memory::MemCpy( vertexbuffer, mesh1.mVertexBuffer.mBuffer, mesh1.mVertexBuffer.mLength );
			Memory::MemCpy( vertexbuffer + mesh1.mVertexBuffer.mLength, mesh2.mVertexBuffer.mBuffer, mesh2.mVertexBuffer.mLength );

			delete[] mesh1.mVertexBuffer.mBuffer;
			mesh1.mVertexBuffer.mBuffer = _null;
			delete[] mesh2.mVertexBuffer.mBuffer;
			mesh2.mVertexBuffer.mBuffer = _null;

			mesh1.mVertexBuffer.mBuffer = vertexbuffer;
			mesh1.mVertexBuffer.mLength = mesh1.mVertexBuffer.mLength + mesh2.mVertexBuffer.mLength;

			// Combine helper buffer.
			if ( mesh1.mHelperBuffer.mLength != 0 && mesh2.mHelperBuffer.mLength != 0 )
			{
				_byte* helperbuffer = new _byte[ mesh1.mHelperBuffer.mLength + mesh2.mHelperBuffer.mLength ];

				Memory::MemCpy( helperbuffer, mesh1.mHelperBuffer.mBuffer, mesh1.mHelperBuffer.mLength );
				Memory::MemCpy( helperbuffer + mesh1.mHelperBuffer.mLength, mesh2.mHelperBuffer.mBuffer, mesh2.mHelperBuffer.mLength );

				delete[] mesh1.mHelperBuffer.mBuffer;
				mesh1.mHelperBuffer.mBuffer = _null;
				delete[] mesh2.mHelperBuffer.mBuffer;
				mesh2.mHelperBuffer.mBuffer = _null;

				mesh1.mHelperBuffer.mBuffer = helperbuffer;
				mesh1.mHelperBuffer.mLength = mesh1.mHelperBuffer.mLength + mesh2.mHelperBuffer.mLength;
			}

			// Combine index buffer.
			_byte* indexbuffer = new _byte[ mesh1.mIndexBuffer.mLength + mesh2.mIndexBuffer.mLength ];

			Memory::MemCpy( indexbuffer, mesh1.mIndexBuffer.mBuffer, mesh1.mIndexBuffer.mLength );
			Memory::MemCpy( indexbuffer + mesh1.mIndexBuffer.mLength, mesh2.mIndexBuffer.mBuffer, mesh2.mIndexBuffer.mLength );

			delete[] mesh1.mIndexBuffer.mBuffer;
			mesh1.mIndexBuffer.mBuffer = _null;
			delete[] mesh2.mIndexBuffer.mBuffer;
			mesh2.mIndexBuffer.mBuffer = _null;

			mesh1.mIndexBuffer.mBuffer = indexbuffer;
			mesh1.mIndexBuffer.mLength = mesh1.mIndexBuffer.mLength + mesh2.mIndexBuffer.mLength;

			// Remove the combined mesh.
			mesharray.Remove( j );

			j --;
		}
	}
}

AxisAlignedBox ModelHelper::GetBoundBoxFromVertexBuffer( _dword format, const ModelBuffer& vertexbuffer )
{
	_dword vertexsize = GetVertexSizeFromFormat( format );

	AxisAlignedBox boundbox = AxisAlignedBox::cInitBox;

	for ( _dword i = 0; i < vertexbuffer.mLength / vertexsize; i ++ )
		boundbox += *( (Vector3*) ( vertexbuffer.mBuffer + i * vertexsize ) );

	return boundbox;
}

_void ModelHelper::CopyVertexBufferIntoMemory( _dword format, _dword vertexnumber, _byte* vertexbuffer, _byte* helperbuffer, const _byte* srcbuffer, const ModelVertexAddon& vertexaddon )
{
	_dword newformat		= vertexaddon.UpdateFormat( format );

	_dword srcvertexsize	= GetVertexSizeFromFormat( format );
	_dword srctexcoordsize	= GetTexcoordNumberFromFormat( format ) * sizeof( Vector2 );

	_dword desvertexsize	= GetVertexSizeFromFormat( newformat );
	_dword destexcoordsize	= GetTexcoordNumberFromFormat( newformat ) * sizeof( Vector2 );

	_dword helperrefsize	= GetHelperRefSizeFromFormat( format );
	_dword mathdatasize		= GetMathDataSizeFromFormat( format );

	FL_ASSERT( vertexbuffer != _null )
	FL_ASSERT( helperbuffer != _null )

	//_dword* influenceweightsnum	= (_dword*) helperbuffer;
	//_float* influenceweights	= (_float*) helperbuffer;
	//_byte*  influenceboneids	= (_byte*) helperbuffer;

	//if ( newformat & Geometry::_FORMATFLAG_NORMAL )
	//{
	//	influenceweightsnum = (_dword*) ( (_byte*) helperbuffer + sizeof( Vector3 ) * 2 * vertexnumber );
	//}
	//else
	//{
	//	influenceweightsnum = (_dword*) ( (_byte*) helperbuffer + sizeof( Vector3 ) * vertexnumber );
	//}

	//influenceweights = (_float*) ( (_byte*) influenceweightsnum + sizeof( _dword ) * vertexnumber );
	//influenceboneids = (_byte*) influenceweights + sizeof( _float ) * ModelInfluenceGroup::_MAX_INFLUENCE_NUMBER * vertexnumber;

	for ( _dword i = 0; i < vertexnumber; i ++ )
	{
		//// Copy vertex directly.
		if ( vertexaddon.IsEnabled( ) == _false )
		{
			// Copy vertex Position.
			Memory::MemCpy( vertexbuffer, srcbuffer, srcvertexsize ); vertexbuffer += srcvertexsize;
			Memory::MemCpy( helperbuffer, srcbuffer, helperrefsize ); helperbuffer += helperrefsize;

			srcbuffer += srcvertexsize;
		}
		// Copy vertex partly.
		else
		{
			// Copy vertex Position.
			Memory::MemCpy( vertexbuffer, srcbuffer, sizeof( Vector3 ) ); vertexbuffer += sizeof( Vector3 );
			Memory::MemCpy( helperbuffer, srcbuffer, sizeof( Vector3 ) ); helperbuffer += sizeof( Vector3 );
			srcbuffer += sizeof( Vector3 );

			// Copy vertex normal.
			if ( newformat & Geometry::_FORMATFLAG_NORMAL )
			{
				const _byte* tempbuffer = vertexaddon.mEnableAdditionalNormal ? (const _byte*) &vertexaddon.mDefaultAdditionalNormal : srcbuffer;

				Memory::MemCpy( vertexbuffer, tempbuffer, sizeof( Vector3 ) ); vertexbuffer += sizeof( Vector3 );
				Memory::MemCpy( helperbuffer, tempbuffer, sizeof( Vector3 ) ); helperbuffer += sizeof( Vector3 );

				if ( format & Geometry::_FORMATFLAG_NORMAL )
					srcbuffer += sizeof( Vector3 );
			}

			// Copy vertex diffuse.
			if ( newformat & Geometry::_FORMATFLAG_DIFFUSE )
			{
				const _byte* tempbuffer = vertexaddon.mEnableAdditionalDiffuse ? (const _byte*) &vertexaddon.mDefaultAdditionalDiffuse : srcbuffer;

				Memory::MemCpy( vertexbuffer, tempbuffer, sizeof( _dword ) ); vertexbuffer += sizeof( _dword );

				if ( format & Geometry::_FORMATFLAG_DIFFUSE )
					srcbuffer += sizeof( _dword );
			}

			// Copy vertex specular.
			if ( newformat & Geometry::_FORMATFLAG_SPECULAR )
			{
				const _byte* tempbuffer = vertexaddon.mEnableAdditionalSpecular ? (const _byte*) &vertexaddon.mDefaultAdditionalSpecular : srcbuffer;

				Memory::MemCpy( vertexbuffer, tempbuffer, sizeof( _dword ) ); vertexbuffer += sizeof( _dword );

				if ( format & Geometry::_FORMATFLAG_SPECULAR )
					srcbuffer += sizeof( _dword );
			}

			// Copy vertex texcoord.
			if ( srctexcoordsize > 0 )
			{
				Memory::MemCpy( vertexbuffer, srcbuffer, srctexcoordsize ); vertexbuffer += srctexcoordsize;

				srcbuffer += srctexcoordsize;
			}

			// Append additional texcoord.
			if ( srctexcoordsize != destexcoordsize )
			{
				Memory::MemCpy( vertexbuffer, &vertexaddon.mDefaultAdditionalTexcoord, sizeof( Vector2 ) ); vertexbuffer += sizeof( Vector2 );
			}

			// Copy math data size.
			if ( mathdatasize > 0 )
			{
				Memory::MemCpy( vertexbuffer, srcbuffer, mathdatasize ); vertexbuffer += mathdatasize;

				srcbuffer += mathdatasize;
			}
		}

		// Get influence number.
		_dword influencenumber = *( (_dword*) srcbuffer ); srcbuffer += sizeof( _dword );

		// Get influence list pointer.
		//ModelInfluence* srcinfluencelist = (ModelInfluence*) srcbuffer;

		//// Copy each influence.
		//_dword k = 0;
		//while ( k < influencenumber )
		//{
		//	influenceweights[ i * ModelInfluenceGroup::_MAX_INFLUENCE_NUMBER + k ] = srcinfluencelist[k].mWeight;
		//	influenceboneids[ i * ModelInfluenceGroup::_MAX_INFLUENCE_NUMBER + k ] = (_byte) srcinfluencelist[k].mBoneID;

		//	k ++;
		//}

		//influenceweightsnum[i] = influencenumber;

		//while ( k < ModelInfluenceGroup::_MAX_INFLUENCE_NUMBER )
		//{
		//	influenceweights[ i * ModelInfluenceGroup::_MAX_INFLUENCE_NUMBER + k ] = 0.0f;
		//	influenceboneids[ i * ModelInfluenceGroup::_MAX_INFLUENCE_NUMBER + k ] = (_byte) 0xFF;

		//	k ++;
		//}

		//srcbuffer += sizeof( ModelInfluence ) * influencenumber;

		// Get influence list pointer.
		ModelInfluence* srcinfluencelist = (ModelInfluence*) srcbuffer;
		ModelInfluenceGroup* desinfluencegroup = (ModelInfluenceGroup*) helperbuffer;

		// Copy each influence.
		for ( _dword k = 0; k < influencenumber; k ++ )
		{
			desinfluencegroup->mBoneIDList[k] = (_byte) srcinfluencelist[k].mBoneID;
			desinfluencegroup->mWeightList[k] = srcinfluencelist[k].mWeight;
		}

		if ( influencenumber < ModelInfluenceGroup::_MAX_INFLUENCE_NUMBER )
			desinfluencegroup->mBoneIDList[ influencenumber ] = 0xFF;

		srcbuffer	 += sizeof( ModelInfluence ) * influencenumber;
		helperbuffer += sizeof( ModelInfluenceGroup );
	}
}

_void ModelHelper::CopyVertexBufferIntoMemory( _dword format, _dword vertexnumber, _byte* vertexbuffer, const _byte* srcbuffer, const ModelVertexAddon& vertexaddon )
{
	_dword newformat		= vertexaddon.UpdateFormat( format );

	_dword srcvertexsize	= GetVertexSizeFromFormat( format );
	_dword srctexcoordsize	= GetTexcoordNumberFromFormat( format ) * sizeof( Vector2 );

	_dword desvertexsize	= GetVertexSizeFromFormat( newformat );
	_dword destexcoordsize	= GetTexcoordNumberFromFormat( newformat ) * sizeof( Vector2 );

	_dword mathdatasize		= GetMathDataSizeFromFormat( format );

	FL_ASSERT( vertexbuffer != _null )

	// Copy whole buffer directly.
	if ( vertexaddon.IsEnabled( ) == _false )
	{
		Memory::MemCpy( vertexbuffer, srcbuffer, vertexnumber * srcvertexsize );

		return;
	}

	// Copy vertex one by one.
	for ( _dword i = 0; i < vertexnumber; i ++ )
	{
		// Copy vertex position.
		Memory::MemCpy( vertexbuffer, srcbuffer, sizeof( Vector3 ) ); vertexbuffer += sizeof( Vector3 );
		srcbuffer += sizeof( Vector3 );

		// Copy vertex normal.
		if ( newformat & Geometry::_FORMATFLAG_NORMAL )
		{
			const _byte* tempbuffer = vertexaddon.mEnableAdditionalNormal ? (const _byte*) &vertexaddon.mDefaultAdditionalNormal : srcbuffer;

			Memory::MemCpy( vertexbuffer, tempbuffer, sizeof( Vector3 ) ); vertexbuffer += sizeof( Vector3 );

			if ( format & Geometry::_FORMATFLAG_NORMAL )
				srcbuffer += sizeof( Vector3 );
		}

		// Copy vertex diffuse.
		if ( newformat & Geometry::_FORMATFLAG_DIFFUSE )
		{
			const _byte* tempbuffer = vertexaddon.mEnableAdditionalDiffuse ? (const _byte*) &vertexaddon.mDefaultAdditionalDiffuse : srcbuffer;

			Memory::MemCpy( vertexbuffer, tempbuffer, sizeof( _dword ) ); vertexbuffer += sizeof( _dword );

			if ( format & Geometry::_FORMATFLAG_DIFFUSE )
				srcbuffer += sizeof( _dword );
		}

		// Copy vertex specular.
		if ( newformat & Geometry::_FORMATFLAG_SPECULAR )
		{
			const _byte* tempbuffer = vertexaddon.mEnableAdditionalSpecular ? (const _byte*) &vertexaddon.mDefaultAdditionalSpecular : srcbuffer;

			Memory::MemCpy( vertexbuffer, tempbuffer, sizeof( _dword ) ); vertexbuffer += sizeof( _dword );

			if ( format & Geometry::_FORMATFLAG_SPECULAR )
				srcbuffer += sizeof( _dword );
		}

		// Copy vertex texcoord.
		if ( srctexcoordsize > 0 )
		{
			Memory::MemCpy( vertexbuffer, srcbuffer, srctexcoordsize ); vertexbuffer += srctexcoordsize;

			srcbuffer += srctexcoordsize;
		}

		// Append additional texcoord.
		if ( srctexcoordsize != destexcoordsize )
		{
			Memory::MemCpy( vertexbuffer, &vertexaddon.mDefaultAdditionalTexcoord, sizeof( Vector2 ) ); vertexbuffer += sizeof( Vector2 );
		}
		
		// Copy math data size.
		if ( mathdatasize > 0 )
		{
			Memory::MemCpy( vertexbuffer, srcbuffer, mathdatasize ); vertexbuffer += mathdatasize;

			srcbuffer += mathdatasize;
		}
	}
}

_void ModelHelper::CopyIndexBufferIntoMemory( _dword indexnumber, _byte* indexbuffer, const _byte* srcbuffer )
{
	Memory::MemCpy( indexbuffer, srcbuffer, sizeof( _word ) * indexnumber );
}