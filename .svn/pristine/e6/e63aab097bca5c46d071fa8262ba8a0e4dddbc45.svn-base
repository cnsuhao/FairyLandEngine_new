//============================================================================
// ISamplerMethod.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ISamplerMethod
//----------------------------------------------------------------------------

class ISamplerMethod
{
public:
	enum _TEXTUREADDR
	{
		_TEXTUREADDR_WRAP				= 1,
		_TEXTUREADDR_MIRROR				= 2,
		_TEXTUREADDR_CLAMP				= 3,
		_TEXTUREADDR_BORDER				= 4,
		_TEXTUREADDR_MIRRORONCE			= 5,
	};

	enum _TEXTUREFILTER
	{
		_TEXTUREFILTER_NONE				= 0,
		_TEXTUREFILTER_POINT			= 1,
		_TEXTUREFILTER_LINEAR			= 2,
		_TEXTUREFILTER_ANISOTROPIC		= 3,
		_TEXTUREFILTER_PYRAMIDALQUAD	= 6,
		_TEXTUREFILTER_GAUSSIANQUAD		= 7,
	};

public:
	virtual _bool	IsDefaultMethod( ) const = 0;

	virtual _void	SetSamplerAddressU( _dword stage, _dword value ) = 0;
	virtual _void	SetSamplerAddressV( _dword stage, _dword value ) = 0;
	virtual _void	SetSamplerAddressW( _dword stage, _dword value ) = 0;
	virtual _void	SetSamplerBorderColor( _dword stage, _dword value ) = 0;

	virtual _void	SetSamplerMagFilter( _dword stage, _dword value ) = 0;
	virtual _void	SetSamplerMinFilter( _dword stage, _dword value ) = 0;
	virtual _void	SetSamplerMipFilter( _dword stage, _dword value ) = 0;

	virtual _void	SetSamplerMipmapBias( _dword stage, _dword value ) = 0;
	virtual _void	SetSamplerMaxMipmapLevel( _dword stage, _dword value ) = 0;
	virtual _void	SetSamplerMaxAnisotropy( _dword stage, _dword value ) = 0;
};

};