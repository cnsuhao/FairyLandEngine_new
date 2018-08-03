//============================================================================
// RenderAPI.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// RenderAPI Implementation
//----------------------------------------------------------------------------

RenderAPI::RenderAPI( )
{
	mWindow				= 0;
	mDeviceType			= 0;
	mDeviceWidth		= 0;
	mDeviceHeight		= 0;

	mRGBAFormat			= _false;

	mClearColor			= 0;
	mClearDepth			= 1.0f;
	mClearStencil		= 0;
}

_dword RenderAPI::GetPixelSize( _dword format )
{
	switch ( format )
	{
		case ITextureFactory::_FORMAT_UNKNOWN:
			return 0;

		case ITextureFactory::_FORMAT_R8G8B8:
			return 3;

		case ITextureFactory::_FORMAT_A8R8G8B8:
			return 4;

		case ITextureFactory::_FORMAT_X8R8G8B8:
			return 4;

		case ITextureFactory::_FORMAT_A4R4G4B4:
			return 2;

		case ITextureFactory::_FORMAT_X4R4G4B4:
			return 2;

		case ITextureFactory::_FORMAT_DXT1:
			return 0;

		case ITextureFactory::_FORMAT_DXT2:
			return 0;

		case ITextureFactory::_FORMAT_DXT3:
			return 0;

		case ITextureFactory::_FORMAT_DXT4:
			return 0;

		case ITextureFactory::_FORMAT_DXT5:
			return 0;

		case ITextureFactory::_FORMAT_A8L8:
			return 2;

		case ITextureFactory::_FORMAT_D24S8:
			return 4;
	}

	return 0;
}

_dword RenderAPI::GetSurfaceSize( _dword width, _dword height, _dword format, _dword mip )
{
	_dword pixelsize = GetPixelSize( format );

	if ( pixelsize == 0 )
		return 0;

	if ( mip == 0 )
		mip = -1;

	_dword totalsize = 0;

	for ( _dword i = 0; i < mip; i ++ )
	{
		totalsize += pixelsize * width * height;

		width	/= 2;
		height	/= 2;

		if ( width == 0 || height == 0 )
			break;
	}

	return totalsize;
}