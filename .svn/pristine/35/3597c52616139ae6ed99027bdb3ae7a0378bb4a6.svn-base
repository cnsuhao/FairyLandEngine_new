//============================================================================
// ModelHelper.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ModelHelper
//----------------------------------------------------------------------------

class ModelHelper
{
public:
	static String			GetGroupFromName( StringPtr meshname );

	static _dword			GetRenderFlagFromName( StringPtr meshname );
	static _dword			GetRenderLayerFromName( StringPtr meshname );
	static _dword			GetVertexSizeFromFormat( _dword format );
	static _dword			GetHelperSizeFromFormat( _dword format );
	static _dword			GetHelperRefSizeFromFormat( _dword format );
	static _dword			GetMathDataSizeFromFormat( _dword format );
	static _dword			GetAnimationSizeFromFormat( _dword format );
	static _dword			GetTexcoordNumberFromFormat( _dword format );

	static _bool			CheckVertexFormatVSAnimationFormat( _dword vertexformat, _dword animationformat );

	static _bool			CompareVertex( _dword format, const _byte* vertex1, const _byte* vertex2, _float epsilon = Math::cEpsilon );
	static _void			CombineVertexArray( _dword format, ModelBuffer& vertexbuffer, ModelBuffer& helperbuffer, ModelBuffer& indexbuffer, _dword& vertexnumber ); 

	static _bool			CompareMesh( const ModelMesh& mesh1, const ModelMesh& mesh2 );
	static _void			CombineMeshArray( Array< ModelMesh >& mesharray );

	static AxisAlignedBox	GetBoundBoxFromVertexBuffer( _dword format, const ModelBuffer& vertexbuffer );

	static _void			CopyVertexBufferIntoMemory( _dword format, _dword vertexnumber, _byte* vertexbuffer, _byte* helperbuffer, const _byte* srcbuffer, const ModelVertexAddon& vertexaddon );
	static _void			CopyVertexBufferIntoMemory( _dword format, _dword vertexnumber, _byte* vertexbuffer, const _byte* srcbuffer, const ModelVertexAddon& vertexaddon );
	static _void			CopyIndexBufferIntoMemory( _dword indexnumber, _byte* indexbuffer, const _byte* srcbuffer );
};

};