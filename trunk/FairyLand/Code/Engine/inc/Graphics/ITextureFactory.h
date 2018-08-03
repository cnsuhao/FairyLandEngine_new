//============================================================================
// ITextureFactory.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// SurfaceProcess
//----------------------------------------------------------------------------

struct SurfaceProcess
{
	enum { _MAX_PROCESS_NUMBER = 3 };

	enum
	{
		_PROCESS_NONE	= 0,
		_PROCESS_HSL	= 1,
		_PROCESS_RGB	= 2,
		_PROCESS_BORDER	= 3,
	};

	struct Process
	{
		_dword	mType;

		union
		{
			struct
			{
				_dword	mHue;
				_dword	mSaturation;
				_long	mLightness;
			};

			struct
			{
				_dword	mModulate;
				_dword	mAdditive;
				_dword	mSubtract;
			};

			struct
			{
				_dword	mBorderColor;
				_dword	mBorderSize;
				_bool	mBlendSource;
				_float	mBlurRate;
			};
		};

		Rect	mTargetRect;
	};

	Process		mProcess[ _MAX_PROCESS_NUMBER ];

	SurfaceProcess( )
	{
		mProcess[0].mType		= _PROCESS_NONE;
		mProcess[0].mTargetRect	= Rect::cNullRect;

		mProcess[1].mType		= _PROCESS_NONE;
		mProcess[1].mTargetRect	= Rect::cNullRect;

		mProcess[2].mType		= _PROCESS_NONE;
		mProcess[2].mTargetRect	= Rect::cNullRect;
	}
};

//----------------------------------------------------------------------------
// ITextureFactory
//----------------------------------------------------------------------------

class ITextureFactory
{
public:
	enum _DEFAULT_TEXTURE
	{
		_TEXTURE_NONE				= 0,
		_TEXTURE_CHESSBOARD_8X8		= 1,
		_TEXTURE_CHESSBOARD_4X4		= 2,
		_TEXTURE_CHESSBOARD_2X2		= 3,
		_TEXTURE_GLASSBOARD_RED		= 4,
		_TEXTURE_GLASSBOARD_GREEN	= 5,
		_TEXTURE_GLASSBOARD_BLUE	= 6,
	};

	enum _IMAGE_FORMAT
	{
		_FORMAT_BMP = 1,
		_FORMAT_JPG = 2,
		_FORMAT_PNG = 3,
		_FORMAT_DDS = 4,
		_FORMAT_DIB = 5,
	};

	enum _SURFACE_FORMAT
	{
		_FORMAT_UNKNOWN		=  0,

		_FORMAT_R8G8B8		=  1,
		_FORMAT_A8R8G8B8	=  2,
		_FORMAT_X8R8G8B8	=  3,
		_FORMAT_A4R4G4B4	=  4,
		_FORMAT_X4R4G4B4	=  5,
		_FORMAT_R5G6B5		=  6,
		_FORMAT_A1R5G5B5	=  7,

		_FORMAT_DXT1		=  8,
		_FORMAT_DXT2		=  9,
		_FORMAT_DXT3		= 10,
		_FORMAT_DXT4		= 11,
		_FORMAT_DXT5		= 12,

		_FORMAT_A8L8		= 13,

		_FORMAT_D24S8		= 14,
	};

	virtual _void			SetLoadTextureScale( _dword scale ) = 0;
	virtual _dword			GetLoadTextureScale( ) const = 0;
	virtual _void			SetLoadTextureFormat( _dword format ) = 0;
	virtual _dword			GetLoadTextureFormat( ) const = 0;
	virtual _void			SetLoadTextureMipmap( _dword mipmap ) = 0;
	virtual _dword			GetLoadTextureMipmap( ) const = 0;
	virtual _void			SetDefaultTexture( _dword type ) = 0;
	virtual _dword			GetDefaultTexture( ) const = 0;

	virtual _void			ReleaseLostResource( ) = 0;
	virtual _void			RecreateLostResource( ) = 0;

	virtual _bool			GetTextureInfo( StringPtr filename, _dword& width, _dword& height, _dword& format, _dword& mip ) = 0;
	virtual ITexture*		CreateRenderTexture( _dword width, _dword height, _dword format = 0 ) = 0;
	virtual ITexture*		CreateDepthTexture( _dword width, _dword height ) = 0;
	virtual ITexture*		CreateTexture( _dword width, _dword height, _dword format, _dword mip = 1 ) = 0;
	virtual ITexture*		CreateTexture( StringPtr filename ) = 0;
	virtual ITexture*		CreateTexture( const _void* buffer, _dword length ) = 0;
	virtual ITexture*		CloneTexture( ITexture* texture, _bool reference = _true ) = 0;
	virtual _void			ReleaseTexture( ITexture*& texture ) = 0;

	virtual ISurface*		CreateRenderTargetSurface( _dword width, _dword height, _dword format = 0 ) = 0;
	virtual ISurface*		CreateDepthStencilSurface( _dword width, _dword height ) = 0;
	virtual ISurface*		CreateSurface( _dword width, _dword height, _dword format ) = 0;
	virtual ISurface*		CreateSurface( StringPtr filename ) = 0;
	virtual ISurface*		CreateSurface( const _void* buffer, _dword length ) = 0;
	virtual ISurface*		CloneSurface( ISurface* surface, _bool reference = _true ) = 0;
	virtual _void			ReleaseSurface( ISurface*& surface ) = 0;

	virtual IFontTexture*	CreateFontTexture( StringPtr fontname, _dword size, _dword border, _bool blod, _bool italic, _bool underline, _dword edgesize = 0 ) = 0;
	virtual IFontTexture*	CreateFontTexture( StringPtr fontname, StringPtr filename, _dword size, _dword border, _bool blod, _bool italic, _bool underline, _dword edgesize = 0 ) = 0;
	virtual IFontTexture*	CloneFontTexture( IFontTexture* fonttexture, _bool reference = _true ) = 0;
	virtual _void			ReleaseFontTexture( IFontTexture*& fonttexture ) = 0;
};

};