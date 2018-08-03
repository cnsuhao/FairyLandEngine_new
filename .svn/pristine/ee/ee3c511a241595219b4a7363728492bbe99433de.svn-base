//============================================================================
// TextureFactory.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// TextureFactory Implementation
//----------------------------------------------------------------------------

TextureFactory::TextureFactory( )
{
	mLoadTextureScale	= 1;
	mLoadTextureFormat	= 0;
	mLoadTextureMipmap	= 0;
	mDefaultTextureType	= 0;

	mMissTexture		= _null;
	mChessboard8X8		= _null;
	mChessboard4X4		= _null;
	mChessboard2X2		= _null;
	mGlassboardRed		= _null;
	mGlassboardGreen	= _null;
	mGlassboardBlue		= _null;
}

TextureFactory::~TextureFactory( )
{
	for ( _dword i = 0; i < mRenderTextureArray.Number( ); i ++ )
		delete mRenderTextureArray[i];

	for ( _dword i = 0; i < mDepthTextureArray.Number( ); i ++ )
		delete mDepthTextureArray[i];

	for ( _dword i = 0; i < mRenderSurfaceArray.Number( ); i ++ )
		delete mRenderSurfaceArray[i];

	for ( _dword i = 0; i < mDepthSurfaceArray.Number( ); i ++ )
		delete mDepthSurfaceArray[i];

	for ( _dword i = 0; i < mFontArray.Number( ); i ++ )
		delete mFontArray[i];

	delete mMissTexture;
	delete mChessboard8X8;
	delete mChessboard4X4;
	delete mChessboard2X2;
	delete mGlassboardRed;
	delete mGlassboardGreen;
	delete mGlassboardBlue;
}

_bool TextureFactory::OnReleaseTexture( _void* buffer, _bool uselessonly, _bool deleteobject )
{
	Texture* texture = (Texture*) buffer;

	if ( uselessonly && texture->GetRefCount( ) > 0 )
		return _false;

	// Decrease reference count.
	if ( texture->DecreaseRefCount( ) == 0 && deleteobject )
	{
		TextureFactory& factory = (TextureFactory&) GetTextureFactory( );

		// Remove from render target array.
		for ( _dword i = 0; i < factory.mRenderTextureArray.Number( ); i ++ )
		{
			if ( factory.mRenderTextureArray[i] == texture )
			{
				factory.mRenderTextureArray.Remove( i );
				break;
			}
		}

		// Remove from depth stencil array.
		for ( _dword i = 0; i < factory.mDepthTextureArray.Number( ); i ++ )
		{
			if ( factory.mDepthTextureArray[i] == texture )
			{
				factory.mDepthTextureArray.Remove( i );
				break;
			}
		}

		delete texture;
		return _true;
	}

	return _false;
}

_void TextureFactory::CreateDefaultTexture( )
{
	mMissTexture		= (Texture*) CreateTexture( 64, 64, ITextureFactory::_FORMAT_X8R8G8B8, 1 );
	mChessboard8X8		= (Texture*) CreateTexture( 64, 64, ITextureFactory::_FORMAT_X8R8G8B8, 1 );
	mChessboard4X4		= (Texture*) CreateTexture( 32, 32, ITextureFactory::_FORMAT_X8R8G8B8, 1 );
	mChessboard2X2		= (Texture*) CreateTexture( 16, 16, ITextureFactory::_FORMAT_X8R8G8B8, 1 );
	mGlassboardRed		= (Texture*) CreateTexture( 16, 16, ITextureFactory::_FORMAT_A8R8G8B8, 1 );
	mGlassboardGreen	= (Texture*) CreateTexture( 16, 16, ITextureFactory::_FORMAT_A8R8G8B8, 1 );
	mGlassboardBlue		= (Texture*) CreateTexture( 16, 16, ITextureFactory::_FORMAT_A8R8G8B8, 1 );

	if ( mMissTexture != _null )
	{
		_dword  pitch  = 0;
		_dword* buffer = (_dword*) mMissTexture->Lock( 0, pitch );

		if ( buffer != _null )
		{
			for ( _dword i = 0; i < 64 * 64; i ++ )
			{
				_dword x = ( i % 64 ) / 8;
				_dword y = ( i / 64 ) / 8;

				buffer[i] = ( x + y ) % 2 == 0 ? 0xFFFF0000 : 0xFFFFFFFF;
			}
		}

		mMissTexture->Unlock( 0 );
	}

	if ( mChessboard8X8 != _null )
	{
		_dword  pitch  = 0;
		_dword* buffer = (_dword*) mChessboard8X8->Lock( 0, pitch );

		if ( buffer != _null )
		{
			for ( _dword i = 0; i < 64 * 64; i ++ )
			{
				_dword x = ( i % 64 ) / 8;
				_dword y = ( i / 64 ) / 8;

				buffer[i] = ( x + y ) % 2 == 0 ? 0xFF000000 : 0xFFFFFFFF;
			}
		}

		mChessboard8X8->Unlock( 0 );
	}

	if ( mChessboard4X4 != _null )
	{
		_dword  pitch  = 0;
		_dword* buffer = (_dword*) mChessboard4X4->Lock( 0, pitch );

		if ( buffer != _null )
		{
			for ( _dword i = 0; i < 32 * 32; i ++ )
			{
				_dword x = ( i % 32 ) / 8;
				_dword y = ( i / 32 ) / 8;

				buffer[i] = ( x + y ) % 2 == 0 ? 0xFF000000 : 0xFFFFFFFF;
			}
		}

		mChessboard4X4->Unlock( 0 );
	}

	if ( mChessboard2X2 != _null )
	{
		_dword  pitch  = 0;
		_dword* buffer = (_dword*) mChessboard2X2->Lock( 0, pitch );

		if ( buffer != _null )
		{
			for ( _dword i = 0; i < 16 * 16; i ++ )
			{
				_dword x = ( i % 16 ) / 8;
				_dword y = ( i / 16 ) / 8;

				buffer[i] = ( x + y ) % 2 == 0 ? 0xFF000000 : 0xFFFFFFFF;
			}
		}

		mChessboard2X2->Unlock( 0 );
	}

	if ( mGlassboardRed != _null )
	{
		_dword  pitch  = 0;
		_dword* buffer = (_dword*) mGlassboardRed->Lock( 0, pitch );

		if ( buffer != _null )
		{
			for ( _dword i = 0; i < 16 * 16; i ++ )
				buffer[i] = 0x40FF0000;
		}

		mGlassboardRed->Unlock( 0 );
	}

	if ( mGlassboardGreen != _null )
	{
		_dword  pitch  = 0;
		_dword* buffer = (_dword*) mGlassboardGreen->Lock( 0, pitch );

		if ( buffer != _null )
		{
			for ( _dword i = 0; i < 16 * 16; i ++ )
				buffer[i] = 0x4000FF00;
		}

		mGlassboardGreen->Unlock( 0 );
	}

	if ( mGlassboardBlue != _null )
	{
		_dword  pitch  = 0;
		_dword* buffer = (_dword*) mGlassboardBlue->Lock( 0, pitch );

		if ( buffer != _null )
		{
			for ( _dword i = 0; i < 16 * 16; i ++ )
				buffer[i] = 0x400000FF;
		}

		mGlassboardBlue->Unlock( 0 );
	}
}

_void TextureFactory::SetLoadTextureScale( _dword scale )
{
	if ( scale > 0 )
		mLoadTextureScale = scale;
}

_dword TextureFactory::GetLoadTextureScale( ) const
{
	return mLoadTextureScale;
}

_void TextureFactory::SetLoadTextureFormat( _dword format )
{
	mLoadTextureFormat = format;
}

_dword TextureFactory::GetLoadTextureFormat( ) const
{
	return mLoadTextureFormat;
}

_void TextureFactory::SetLoadTextureMipmap( _dword mipmap )
{
	mLoadTextureMipmap = mipmap;
}

_dword TextureFactory::GetLoadTextureMipmap( ) const
{
	return mLoadTextureMipmap;
}

_void TextureFactory::SetDefaultTexture( _dword type )
{
	mDefaultTextureType = type;
}

_dword TextureFactory::GetDefaultTexture( ) const
{
	return mDefaultTextureType;
}

_void TextureFactory::ReleaseLostResource( )
{
	for ( _dword i = 0; i < mRenderTextureArray.Number( ); i ++ )
	{
		Texture* texture = mRenderTextureArray[i];

		texture->ReleaseTextureObject( );
	}

	for ( _dword i = 0; i < mDepthTextureArray.Number( ); i ++ )
	{
		Texture* texture = mDepthTextureArray[i];

		texture->ReleaseTextureObject( );
	}
}

_void TextureFactory::RecreateLostResource( )
{
	// TODO : release all surface in texture.

	for ( _dword i = 0; i < mRenderTextureArray.Number( ); i ++ )
	{
		Texture* texture = mRenderTextureArray[i];

		_void* tex = GraphicsManager::GetRenderAPIHelper( ).CreateRenderTexture(
			texture->GetSurfaceWidth( 0 ), texture->GetSurfaceHeight( 0 ), texture->GetFormat( ) );

		texture->ResetTextureObject( tex );
	}

	for ( _dword i = 0; i < mDepthTextureArray.Number( ); i ++ )
	{
		Texture* texture = mDepthTextureArray[i];

		_void* tex = GraphicsManager::GetRenderAPIHelper( ).CreateDepthTexture(
			texture->GetSurfaceWidth( 0 ), texture->GetSurfaceHeight( 0 ) );

		texture->ResetTextureObject( tex );
	}
}

_bool TextureFactory::GetTextureInfo( StringPtr filename, _dword& width, _dword& height, _dword& format, _dword& mip )
{
	BinFile texturefile;

	if ( GetResourceManagerPointer( ) != _null )
	{
		if ( GetResourceManagerPointer( )->LoadResource( filename, texturefile ) == _false )
			return _false;
	}
	else
	{
		if ( texturefile.Open( filename ) == _false )
			return _false;
	}

	if ( texturefile.GetLength( ) == 0 )
		return _false;

	return GraphicsManager::GetRenderAPIHelper( ).GetTextureInfo( texturefile.GetBuffer( ), texturefile.GetLength( ), width, height, format, mip );
}

ITexture* TextureFactory::CreateRenderTexture( _dword width, _dword height, _dword format )
{
	if ( format == 0 )
		format = GetRenderDevice( ).GetRenderTargetFormat( );

	_void* tex = GraphicsManager::GetRenderAPIHelper( ).CreateRenderTexture( width, height, format );
	if ( tex == _null )
	{
		Surface* surface = (Surface*) CreateRenderTargetSurface( width, height, format );
		if ( surface == _null )
			return _null;

		return new Texture( surface );
	}

	Texture* texture = new Texture( tex, width, height, format, 1 );

	mRenderTextureArray.Append( texture );

	return texture;
}

ITexture* TextureFactory::CreateDepthTexture( _dword width, _dword height )
{
	_void* tex = GraphicsManager::GetRenderAPIHelper( ).CreateDepthTexture( width, height );
	if ( tex == _null )
	{
		Surface* surface = (Surface*) CreateDepthStencilSurface( width, height );
		if ( surface == _null )
			return _null;

		return new Texture( surface );
	}

	Texture* texture = new Texture( tex, width, height, _FORMAT_D24S8, 1 );

	mDepthTextureArray.Append( texture );

	return texture;
}

ITexture* TextureFactory::CreateTexture( _dword width, _dword height, _dword format, _dword mip )
{
	_void* tex = GraphicsManager::GetRenderAPIHelper( ).CreateTexture( width, height, format, mip );
	if ( tex == _null )
		return _null;

	Texture* texture = new Texture( tex, width, height, format, mip );

	return texture;
}

ITexture* TextureFactory::CreateTexture( StringPtr filename )
{
	// Search in resource cache first.
	if ( GetResourceCachePointer( ) != _null )
	{
		ITexture* cachedtexture = (ITexture*) GetResourceCache( ).ObtainCache( filename, OnReleaseTexture, _true );

		if ( cachedtexture != _null )
			return CloneTexture( cachedtexture, _true );
	}

	BinFile texturefile;

	// Load texture from resource manager.
	if ( GetResourceManagerPointer( ) != _null )
	{
		GetResourceManagerPointer( )->LoadResource( filename, texturefile );
	}
	else
	{
		texturefile.Open( filename );
	}

	if ( texturefile.GetLength( ) == 0 )
	{
		//// Get texture replacer.
		//if ( GetResourceReplacerPointer( ) != _null )
		//{
		//	ITexture* texture = (ITexture*) GetResourceReplacerPointer( )->GetTextureReplacer( filename );
		//	if ( texture != _null )
		//		return texture;
		//}

		if ( mMissTexture != _null )
			mMissTexture->IncreaseRefCount( );

		return mMissTexture;
	}

	//// Create texture sequence.
	//if ( StringFormatter::GetExtention( filename ).CompareLowercase( L"tan" ) )
	//{
	//	TextureSequence* texture = new TextureSequence( );

	//	// Increase reference count.
	//	texture->IncreaseRefCount( );

	//	// Set texture name for later use.
	//	texture->SetName( texturefile.GetFileName( ) );

	//	// Load texture animation.
	//	texture->LoadAnimation( texturefile );

	//	// Insert texture cache.
	//	if ( GetResourceCachePointer( ) != _null )
	//		GetResourceCachePointer( )->CreateCache( texture->GetName( ), texture, OnReleaseTexture );

	//	return texture;
	//}

	Texture* texture = (Texture*) CreateTexture( texturefile.GetBuffer( ), texturefile.GetLength( ) );

	if ( texture == _null )
	{
		if ( mMissTexture != _null )
			mMissTexture->IncreaseRefCount( );

		return mMissTexture;
	}

	// Set texture name for later use.
	texture->SetName( texturefile.GetFileName( ) );

	// Insert texture into cache.
	if ( GetResourceCachePointer( ) != _null )
		GetResourceCachePointer( )->CreateCache( texture->GetName( ), texture, OnReleaseTexture );

	return texture;
}

ITexture* TextureFactory::CreateTexture( const _void* buffer, _dword length )
{
	_dword width = mLoadTextureScale, height = 0, format = mLoadTextureFormat, mip = mLoadTextureMipmap;

	_void* tex = GraphicsManager::GetRenderAPIHelper( ).CreateTexture( buffer, length, width, height, format, mip );
	if ( tex == _null )
		return _null;

	Texture* texture = new Texture( tex, width, height, format, mip );

	return texture;
}

ITexture* TextureFactory::CloneTexture( ITexture* texture, _bool reference )
{
	if ( texture == _null )
		return _null;

	// Increase reference count.
	if ( reference || ( (Texture*) texture )->GetRefCount( ) == 0 )
	{
		( (Texture*) texture )->IncreaseRefCount( );

		return texture;
	}
	else
	{
		// Get infomation
		_dword format	= texture->GetFormat( );
		_dword mip		= texture->GetSurfaceNumber( );
		_dword width	= texture->GetSurfaceWidth( 0 );
		_dword height	= texture->GetSurfaceHeight( 0 );

		ITexture* newtexture = CreateTexture( width, height, format, mip ); 

		//// Copy surface 
		//for ( _dword i = 0; i < mip; i ++ )
		//	newtexture->GetSurface( i )->FromSurface( texture->GetSurface( i ) ); 

		return newtexture;
	}
}

_void TextureFactory::ReleaseTexture( ITexture*& texture )
{
	if ( texture == _null )
		return;

	if ( ( (Texture*) texture )->DecreaseRefCount( ) == 0 )
	{
		if ( GetResourceCachePointer( ) == _null || GetResourceCache( ).ReleaseCache( texture->GetName( ), texture ) == _false )
			OnReleaseTexture( texture );
	}

	texture = _null;
}

ISurface* TextureFactory::CreateRenderTargetSurface( _dword width, _dword height, _dword format )
{
	if ( format == 0 )
		format = GraphicsManager::GetRenderAPIHelper( ).GetRenderTargetFormat( );

	// Create surface by parameters.
	_void* suf = GraphicsManager::GetRenderAPIHelper( ).CreateRenderTargetSurface( width, height, format );
	if ( suf == _null )
		return _null;

	Surface* surface = new Surface( suf, width, height, format );

	mRenderSurfaceArray.Append( surface );

	return surface;
}

ISurface* TextureFactory::CreateDepthStencilSurface( _dword width, _dword height )
{
	// Create surface by parameters.
	_void* suf = GraphicsManager::GetRenderAPIHelper( ).CreateDepthStencilSurface( width, height );
	if ( suf == _null )
		return _null;

	Surface* surface = new Surface( suf, width, height, ITextureFactory::_FORMAT_D24S8 );

	mDepthSurfaceArray.Append( surface );

	return surface;
}

ISurface* TextureFactory::CreateSurface( _dword width, _dword height, _dword format )
{
	_void* suf = GraphicsManager::GetRenderAPIHelper( ).CreateSurface( width, height, format );
	if ( suf == _null )
		return _null;

	Surface* surface = new Surface( suf, width, height, format );

	return surface;
}

ISurface* TextureFactory::CreateSurface( StringPtr filename )
{
	BinFile surfacefile;

	if ( GetResourceManagerPointer( ) != _null )
	{
		if ( GetResourceManagerPointer( )->LoadResource( filename, surfacefile ) == _false )
			return _null;
	}
	else
	{
		if ( surfacefile.Open( filename ) == _false )
			return _null;
	}

	if ( surfacefile.GetLength( ) == 0 )
		return _null;

	// Create surface by buffer & size.
	return CreateSurface( surfacefile.GetBuffer( ), surfacefile.GetLength( ) );
}

ISurface* TextureFactory::CreateSurface( const _void* buffer, _dword length )
{
	_dword width = 0, height = 0, format = 0;

	_void* suf = GraphicsManager::GetRenderAPIHelper( ).CreateSurface( buffer, length, width, height, format );
	if ( suf == _null )
		return _null;

	Surface* surface = new Surface( suf, width, height, format );

	return surface;
}

ISurface* TextureFactory::CloneSurface( ISurface* surface, _bool reference )
{
	if ( surface == _null )
		return _null;

	// Increase reference count.
	( (Surface*) surface )->IncreaseRefCount( );

	return surface;
}

_void TextureFactory::ReleaseSurface( ISurface*& surface )
{
	if ( surface == _null )
		return;

	if ( ( (Surface*) surface )->DecreaseRefCount( ) == 0 )
		delete (Surface*) surface;

	surface = _null;
}

IFontTexture* TextureFactory::CreateFontTexture( StringPtr fontname, _dword size, _dword border, _bool blod, _bool italic, _bool underline, _dword edgesize )
{
	if ( size == 0 )
		return _null;

	for ( _dword i = 0; i < mFontArray.Number( ); i ++ )
	{
		if ( mFontArray[i]->GetFontName( ).CompareLowercase( fontname ) == _false )
			continue;

		if ( mFontArray[i]->GetFontSize( ) != size )
			continue;

		if ( mFontArray[i]->GetFontEdge( ) != edgesize )
			continue;

		if ( mFontArray[i]->GetFontBorder( ) != border )
			continue;

		if ( mFontArray[i]->IsBlod( ) != blod )
			continue;

		if ( mFontArray[i]->IsItalic( ) != italic )
			continue;

		if ( mFontArray[i]->IsUnderline( ) != underline )
			continue;

		// Increase reference count.
		mFontArray[i]->IncreaseRefCount( );

		return mFontArray[i];
	}

	FontTexture* fonttexture = new FontTexture( fontname, size, border, blod, italic, underline, edgesize );

	mFontArray.Append( fonttexture );

	return fonttexture;
}

IFontTexture* TextureFactory::CreateFontTexture( StringPtr fontname, StringPtr filename, _dword size, _dword border, _bool blod, _bool italic, _bool underline, _dword edgesize )
{
	// Check if this font file has been registed.
	for ( _dword i = 0; i < mFontFileArray.Number( ); i ++ )
	{
		if ( mFontFileArray[i].CompareLowercase( fontname ) )
			return CreateFontTexture( fontname, size, border, blod, italic, underline, edgesize );
	}

	// Create gui font from font file, register it into system font library.
	if ( filename != L"" )
	{
		BinFile fontfile;

		if ( GetResourceManagerPointer( ) != _null )
		{
			GetResourceManagerPointer( )->LoadResource( filename, fontfile );
		}
		else
		{
			fontfile.Open( filename );
		}

		if ( fontfile.GetLength( ) != 0 )
		{
			_dword fontnumber = 0;
			::AddFontMemResourceEx( fontfile.GetBuffer( ), fontfile.GetLength( ), 0, &fontnumber );
		}

		mFontFileArray.Append( String( fontname ) );
	}

	return CreateFontTexture( fontname, size, border, blod, italic, underline, edgesize );
}

IFontTexture* TextureFactory::CloneFontTexture( IFontTexture* fonttexture, _bool reference )
{
	if ( fonttexture == _null )
		return _null;

	// Increase reference count.
	( (FontTexture*) fonttexture )->IncreaseRefCount( );

	return fonttexture;
}

_void TextureFactory::ReleaseFontTexture( IFontTexture*& fonttexture )
{
	if ( fonttexture == _null )
		return;

	// Decrease reference count.
	if ( ( (FontTexture*) fonttexture )->DecreaseRefCount( ) == 0 )
	{
		// Remove it from font array.
		for ( _dword i = 0; i < mFontArray.Number( ); i ++ )
		{
			if ( mFontArray[i] == fonttexture )
			{
				mFontArray.Remove( i );
				break;
			}
		}

		delete (FontTexture*) fonttexture;
	}

	fonttexture = _null;
}