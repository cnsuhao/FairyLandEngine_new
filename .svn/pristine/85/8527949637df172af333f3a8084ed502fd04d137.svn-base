//============================================================================
// Surface.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// Surface Implementation
//----------------------------------------------------------------------------

Surface::Surface( _void* suf, _dword width, _dword height, _dword format )
{
	mSufObject	= suf;

	mUserData	= 0;

	mSize		= 0;
	mFormat		= format;

	mWidth		= width;
	mHeight		= height;

	mLockRect	= Rect::cNullRect;
	mLockBuffer	= _null;

	IncreaseRefCount( );
}

Surface::~Surface( )
{
	GraphicsManager::GetRenderAPIHelper( ).ReleaseSurface( mSufObject );
}

_void Surface::ReleaseSurfaceObject( )
{
	GraphicsManager::GetRenderAPIHelper( ).ReleaseSurface( mSufObject );

	mSufObject = _null;
}

_void Surface::ResetSurfaceObject( _void* suf )
{
	mSufObject = suf;
}

_void Surface::SetUserData( _dword userdata )
{
	mUserData = userdata;
}

_dword Surface::GetUserData( ) const
{
	return mUserData;
}

_dword Surface::GetSize( ) const
{
	return mSize;
}

_dword Surface::GetFormat( ) const
{
	return mFormat;
}

_dword Surface::GetWidth( ) const
{
	return mWidth;
}

_dword Surface::GetHeight( ) const
{
	return mHeight;
}

_void* Surface::Lock( _dword& pitch, const Rect* lockrect )
{
	if ( mSufObject == _null )
		return _null;

	// Build default lock rect as whole texture.
	mLockRect = lockrect == _null ? Rect( 0, 0, mWidth, mHeight ) : *lockrect;

	// Lock surface.
	return mLockBuffer = GraphicsManager::GetRenderAPIHelper( ).LockSurface( mSufObject, mFormat, pitch, mLockRect );
}

_void Surface::Unlock( )
{
	GraphicsManager::GetRenderAPIHelper( ).UnlockSurface( mSufObject, mFormat, mLockBuffer, mLockRect );
}

_void Surface::ProcessHSL( _dword hue, _dword saturation, _long lightness, const Rect& rect )
{
	if ( mFormat != ITextureFactory::_FORMAT_A8R8G8B8 && mFormat != ITextureFactory::_FORMAT_X8R8G8B8 )
		return;

	_dword pitch  = 0;
	_byte* buffer = (_byte*) Lock( pitch, _null );

	if ( buffer == _null )
		return;

	Rect fixedrect = rect;
	if ( fixedrect == Rect::cNullRect )
		fixedrect = Rect( 0, 0, mWidth, mHeight );

	SurfaceProcessor::ProcessHSL( buffer, pitch, hue, saturation, lightness, fixedrect );

	Unlock( );
}

_void Surface::ProcessRGB( _dword modulate, _dword additive, _dword subtract, const Rect& rect )
{
	if ( mFormat != ITextureFactory::_FORMAT_A8R8G8B8 && mFormat != ITextureFactory::_FORMAT_X8R8G8B8 )
		return;

	_dword pitch  = 0;
	_byte* buffer = (_byte*) Lock( pitch, _null );

	if ( buffer == _null )
		return;

	Rect fixedrect = rect;
	if ( fixedrect == Rect::cNullRect )
		fixedrect = Rect( 0, 0, mWidth, mHeight );

	SurfaceProcessor::ProcessRGB( buffer, pitch, modulate, additive, subtract, fixedrect );

	Unlock( );
}

_void Surface::ProcessBorder( _dword bordercolor, _bool blendsource, _dword bordersize, _float blurrate, const Rect& rect )
{
	if ( mFormat != ITextureFactory::_FORMAT_A8R8G8B8 && mFormat != ITextureFactory::_FORMAT_X8R8G8B8 )
		return;

	_dword pitch  = 0;
	_byte* buffer = (_byte*) Lock( pitch, _null );

	if ( buffer == _null )
		return;

	Rect fixedrect = rect;
	if ( fixedrect == Rect::cNullRect )
		fixedrect = Rect( 0, 0, mWidth, mHeight );

	SurfaceProcessor::ProcessBorder( buffer, pitch, bordercolor, blendsource, bordersize, blurrate, fixedrect );

	Unlock( );
}

_void Surface::ProcessFilter( _dword color, const Rect& rect )
{
	if ( mFormat != ITextureFactory::_FORMAT_A8R8G8B8 && mFormat != ITextureFactory::_FORMAT_X8R8G8B8 )
		return;

	_dword pitch  = 0;
	_byte* buffer = (_byte*) Lock( pitch, _null );

	if ( buffer == _null )
		return;

	Rect fixedrect = rect;
	if ( fixedrect == Rect::cNullRect )
		fixedrect = Rect( 0, 0, mWidth, mHeight );

	SurfaceProcessor::ProcessFilter( buffer, pitch, color, fixedrect );

	Unlock( );
}