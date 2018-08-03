//============================================================================
// TextureFactory.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// TextureFactory
//----------------------------------------------------------------------------

class TextureFactory : public ITextureFactory
{
private:
	_dword						mLoadTextureScale;
	_dword						mLoadTextureFormat;
	_dword						mLoadTextureMipmap;
	_dword						mDefaultTextureType;

	Texture*					mMissTexture;
	Texture*					mChessboard8X8;
	Texture*					mChessboard4X4;
	Texture*					mChessboard2X2;
	Texture*					mGlassboardRed;
	Texture*					mGlassboardGreen;
	Texture*					mGlassboardBlue;

	Array< String >				mFontFileArray;
	Array< FontTexture* >		mFontArray;

	Array< Texture* >			mRenderTextureArray;
	Array< Texture* >			mDepthTextureArray;
	Array< Surface* >			mRenderSurfaceArray;
	Array< Surface* >			mDepthSurfaceArray;

	static _bool OnReleaseTexture( _void* buffer, _bool uselessonly = _false, _bool deleteobject = _true );

public:
	TextureFactory( );
	~TextureFactory( );

	_void CreateDefaultTexture( );

	virtual _void			SetLoadTextureScale( _dword scale );
	virtual _dword			GetLoadTextureScale( ) const;
	virtual _void			SetLoadTextureFormat( _dword format );
	virtual _dword			GetLoadTextureFormat( ) const;
	virtual _void			SetLoadTextureMipmap( _dword mipmap );
	virtual _dword			GetLoadTextureMipmap( ) const;
	virtual _void			SetDefaultTexture( _dword type );
	virtual _dword			GetDefaultTexture( ) const;

	virtual _void			ReleaseLostResource( );
	virtual _void			RecreateLostResource( );

	virtual _bool			GetTextureInfo( StringPtr filename, _dword& width, _dword& height, _dword& format, _dword& mip );
	virtual ITexture*		CreateRenderTexture( _dword width, _dword height, _dword format );
	virtual ITexture*		CreateDepthTexture( _dword width, _dword height );
	virtual ITexture*		CreateTexture( _dword width, _dword height, _dword format, _dword mip );
	virtual ITexture*		CreateTexture( StringPtr filename );
	virtual ITexture*		CreateTexture( const _void* buffer, _dword length );
	virtual ITexture*		CloneTexture( ITexture* texture, _bool reference );
	virtual _void			ReleaseTexture( ITexture*& texture );

	virtual ISurface*		CreateRenderTargetSurface( _dword width, _dword height, _dword format );
	virtual ISurface*		CreateDepthStencilSurface( _dword width, _dword height );
	virtual ISurface*		CreateSurface( _dword width, _dword height, _dword format );
	virtual ISurface*		CreateSurface( StringPtr filename );
	virtual ISurface*		CreateSurface( const _void* buffer, _dword length );
	virtual ISurface*		CloneSurface( ISurface* surface, _bool reference );
	virtual _void			ReleaseSurface( ISurface*& surface );

	virtual IFontTexture*	CreateFontTexture( StringPtr fontname, _dword size, _dword border, _bool blod, _bool italic, _bool underline, _dword edgesize );
	virtual IFontTexture*	CreateFontTexture( StringPtr fontname, StringPtr filename, _dword size, _dword border, _bool blod, _bool italic, _bool underline, _dword edgesize );
	virtual IFontTexture*	CloneFontTexture( IFontTexture* fonttexture, _bool reference );
	virtual _void			ReleaseFontTexture( IFontTexture*& fonttexture );
};

};