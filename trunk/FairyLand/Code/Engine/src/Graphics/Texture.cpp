//============================================================================
// Texture.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// Texture Implementation
//----------------------------------------------------------------------------

Texture::Texture( _void* tex, _dword width, _dword height, _dword format, _dword mip )	
{
	mTexObject	= tex;

	mUserData	= 0;

	mSize		= 0;
	mFormat		= format;

	mMip		= mip;
	mWidth		= width;
	mHeight		= height;

	mLockRect	= Rect::cNullRect;
	mLockBuffer	= _null;

	for ( _dword i = 0; i < _MAX_SURFACE_NUMBER; i ++ )
		mSurfaceList[i] = _null;

	IncreaseRefCount( );
}

Texture::Texture( Surface* surface )
{
	mTexObject	= _null;

	mUserData	= 0;

	mSize		= 0;
	mFormat		= surface->GetFormat( );

	mMip		= 1;
	mWidth		= surface->GetWidth( );
	mHeight		= surface->GetHeight( );

	mLockRect	= Rect::cNullRect;
	mLockBuffer	= _null;

	for ( _dword i = 0; i < _MAX_SURFACE_NUMBER; i ++ )
		mSurfaceList[i] = _null;

	mSurfaceList[0] = surface;

	IncreaseRefCount( );
}

Texture::~Texture( )
{
	for ( _dword i = 0; i < _MAX_SURFACE_NUMBER; i ++ )
	{
		Surface* surface = mSurfaceList[i];

		if ( surface == _null )
			continue;

		if ( surface->DecreaseRefCount( ) == 0 )
			delete surface;
	}

	GraphicsManager::GetRenderAPIHelper( ).ReleaseTexture( mTexObject );
}

_void Texture::ReleaseTextureObject( )
{
	for ( _dword i = 0; i < _MAX_SURFACE_NUMBER; i ++ )
	{
		Surface* surface = mSurfaceList[i];

		if ( surface == _null )
			continue;

		surface->ReleaseSurfaceObject( );
	}

	GraphicsManager::GetRenderAPIHelper( ).ReleaseTexture( mTexObject );

	mTexObject = _null;
}

_void Texture::ResetTextureObject( _void* tex )
{
	mTexObject = tex;

	for ( _dword i = 0; i < _MAX_SURFACE_NUMBER; i ++ )
	{
		Surface* surface = mSurfaceList[i];

		if ( surface == _null )
			continue;

		_void* suf = GraphicsManager::GetRenderAPIHelper( ).GetSurface( mTexObject, i );
		if ( suf == _null )
			continue;

		surface->ResetSurfaceObject( suf );
	}
}

_void Texture::SetName( StringPtr name )
{
	mName = name;
}

StringPtr Texture::GetName( ) const
{
	return mName;
}

_void Texture::SetResName( StringPtr name )
{
	mResName = name;
}

StringPtr Texture::GetResName( ) const
{
	return mResName;
}

_void Texture::SetUserData( _dword userdata )
{
	mUserData = userdata;
}

_dword Texture::GetUserData( ) const
{
	return mUserData;
}

_dword Texture::GetSize( ) const
{
	return mSize;
}

_dword Texture::GetFormat( ) const
{
	return mFormat;
}

_dword Texture::GetSurfaceNumber( ) const
{
	return mMip;
}

_dword Texture::GetSurfaceWidth( _dword level ) const
{
	return mWidth / ( level + 1 );
}

_dword Texture::GetSurfaceHeight( _dword level ) const
{
	return mHeight / ( level + 1 );
}

_void* Texture::Lock( _dword level, _dword& pitch, const Rect* lockrect )
{
	// Build default lock rect as whole texture.
	mLockRect = lockrect == _null ? Rect( 0, 0, mWidth, mHeight ) : *lockrect;

	// Lock texture.
	return mLockBuffer = GraphicsManager::GetRenderAPIHelper( ).LockTexture( mTexObject, mFormat, level, pitch, mLockRect );
}

_void Texture::Unlock( _dword level )
{
	GraphicsManager::GetRenderAPIHelper( ).UnlockTexture( mTexObject, mFormat, level, mLockBuffer, mLockRect );
}

ISurface* Texture::GetSurface( _dword level )
{
	if ( level >= _MAX_SURFACE_NUMBER )
		return _null;

	if ( mSurfaceList[ level ] != _null )
		return mSurfaceList[ level ];

	if ( mTexObject == _null )
		return _null;

	_void* suf = GraphicsManager::GetRenderAPIHelper( ).GetSurface( mTexObject, level );
	if ( suf == _null )
		return _null;

	Surface* surface = new Surface( suf, GetSurfaceWidth( level ), GetSurfaceHeight( level ), mFormat );

	mSurfaceList[ level ] = surface;

	return surface;
}

_void Texture::GenerateMipmaps( )
{
	GraphicsManager::GetRenderAPIHelper( ).GenerateMipmaps( mTexObject );
}

_void Texture::SaveToFile( StringPtr filename, _dword format ) const
{
	String filepath = StringFormatter::GetPathName( filename );

	if ( filepath != L"" )
		FileSystem::CreateDir( filepath );

	GraphicsManager::GetRenderAPIHelper( ).SaveTexture( mTexObject, filename, format );
}

_void Texture::ProcessHSL( _dword hue, _dword saturation, _long lightness, const Rect& rect )
{
	_dword level = mMip == 0 ? 1 : mMip / 2;

	for ( _dword i = 0; i < level; i ++ )
		GetSurface( i )->ProcessHSL( hue, saturation, lightness, rect / ( i + 1 ) );
}	

_void Texture::ProcessRGB( _dword modulate, _dword additive, _dword subtract, const Rect& rect )
{
	_dword level = mMip == 0 ? 1 : mMip / 2;

	for ( _dword i = 0; i < level; i ++ )
		GetSurface( i )->ProcessRGB( modulate, additive, subtract, rect / ( i + 1 ) );
}

_void Texture::ProcessBorder( _dword bordercolor, _bool blendsource, _dword bordersize, _float blurrate, const Rect& rect )
{
	_dword level = mMip == 0 ? 1 : mMip / 2;

	for ( _dword i = 0; i < level; i ++ )
		GetSurface( i )->ProcessBorder( bordercolor, blendsource, bordersize, blurrate, rect / ( i + 1 ) );
}

_void Texture::ProcessFilter( _dword color, const Rect& rect )
{
	//_dword level = mMip == 0 ? 1 : mMip / 2;

	_dword level = 1;

	for ( _dword i = 0; i < level; i ++ )
		GetSurface( i )->ProcessFilter( color, rect / ( i + 1 ) );
}