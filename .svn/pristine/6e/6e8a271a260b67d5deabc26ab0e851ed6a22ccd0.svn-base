//============================================================================
// Model.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// ModelVertexAddon Implementation
//----------------------------------------------------------------------------

ModelVertexAddon::ModelVertexAddon( )
{
	mEnableAdditionalNormal		= _false;
	mDefaultAdditionalNormal	= Vector3::cOrigin;

	mEnableAdditionalDiffuse	= _false;
	mDefaultAdditionalDiffuse	= 0xFFFFFFFF;

	mEnableAdditionalSpecular	= _false;
	mDefaultAdditionalSpecular	= 0x00000000;

	mEnableAdditionalTexcoord	= _false;
	mDefaultAdditionalTexcoord	= Vector2::cOrigin;
}

_bool ModelVertexAddon::IsEnabled( ) const
{
	return	mEnableAdditionalNormal ||
			mEnableAdditionalDiffuse ||
			mEnableAdditionalSpecular ||
			mEnableAdditionalTexcoord;
}

ModelVertexAddon& ModelVertexAddon::EnableAdditionalNormal( const Vector3& normal )
{
	mEnableAdditionalNormal		= _true;
	mDefaultAdditionalNormal	= normal;

	return *this;
}

ModelVertexAddon& ModelVertexAddon::EnableAdditionalDiffuse( _dword diffuse )
{
	mEnableAdditionalDiffuse	= _true;
	mDefaultAdditionalDiffuse	= diffuse;

	return *this;
}

ModelVertexAddon& ModelVertexAddon::EnableAdditionalSpecular( _dword specular )
{
	mEnableAdditionalSpecular	= _true;
	mDefaultAdditionalSpecular	= specular;

	return *this;
}

ModelVertexAddon& ModelVertexAddon::EnableAdditionalTexcoord( const Vector2& texcoord )
{
	mEnableAdditionalTexcoord	= _true;
	mDefaultAdditionalTexcoord	= texcoord;

	return *this;
}

_dword ModelVertexAddon::UpdateFormat( _dword format ) const
{
	if ( mEnableAdditionalNormal )
		format |= Geometry::_FORMATFLAG_NORMAL;

	if ( mEnableAdditionalDiffuse )
		format |= Geometry::_FORMATFLAG_DIFFUSE;

	if ( mEnableAdditionalSpecular )
		format |= Geometry::_FORMATFLAG_SPECULAR;

	if ( mEnableAdditionalTexcoord )
	{
		if ( format & Geometry::_FORMATFLAG_TEXCOORD3 )
		{
			format &= ~Geometry::_FORMATFLAG_TEXCOORD3;
			format |=  Geometry::_FORMATFLAG_TEXCOORD4;
		}
		else if ( format & Geometry::_FORMATFLAG_TEXCOORD2 )
		{
			format &= ~Geometry::_FORMATFLAG_TEXCOORD2;
			format |=  Geometry::_FORMATFLAG_TEXCOORD3;
		}
		else if ( format & Geometry::_FORMATFLAG_TEXCOORD1 )
		{
			format &= ~Geometry::_FORMATFLAG_TEXCOORD1;
			format |=  Geometry::_FORMATFLAG_TEXCOORD2;
		}
		else
		{
			format |=  Geometry::_FORMATFLAG_TEXCOORD1;
		}
	}

	return format;
}