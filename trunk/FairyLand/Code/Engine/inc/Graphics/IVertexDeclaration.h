//============================================================================
// IVertexDeclaration.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IVertexDeclaration
//----------------------------------------------------------------------------

class IVertexDeclaration
{
public:
	enum _ELEMENT_TYPE
    {
        _TYPE_FLOAT1			= 1,
        _TYPE_FLOAT2			= 2,
        _TYPE_FLOAT3			= 3,
        _TYPE_FLOAT4			= 4,
        _TYPE_COLOR				= 5,
		_TYPE_SHORT2			= 6,
		_TYPE_SHORT4			= 7,
        _TYPE_UBYTE4			= 8,
		_TYPE_UNUSED			= 9,
    };

	enum _ELEMENT_USAGE
	{
		_USAGE_POSITION			= 1,
		_USAGE_BLEND_WEIGHTS	= 2,
        _USAGE_BLEND_INDICES	= 3,
		_USAGE_NORMAL			= 4,
		_USAGE_DIFFUSE			= 5,
		_USAGE_SPECULAR			= 6,
		_USAGE_TEXCOORD			= 7,
        _USAGE_BINORMAL			= 8,
        _USAGE_TANGENT			= 9,
		_USAGE_POSITIONT		= 10,
	};

public:
	virtual _void		SetUserData( _dword userdata ) = 0;
	virtual _dword		GetUserData( ) const = 0;

	virtual _void		AddVertexElement( _dword source, _dword offset, _dword type, _dword usage, _dword index ) = 0;
	virtual _void		DelVertexElement( _dword elementindex ) = 0;
	virtual _void		ClearVertexElement( ) = 0;
};

};