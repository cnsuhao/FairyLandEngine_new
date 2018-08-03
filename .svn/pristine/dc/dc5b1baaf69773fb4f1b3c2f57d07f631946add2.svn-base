//============================================================================
// Texture.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Texture
//----------------------------------------------------------------------------

class Texture : public ITexture, public ReferencedObject
{
public:
	enum { _MAX_SURFACE_NUMBER = 16 };

protected:
	_void*		mTexObject;

	String		mName;
	String		mResName;
	_dword		mUserData;

	_dword		mSize;
	_dword		mFormat;

	_dword		mMip;
	_dword		mWidth;
	_dword		mHeight;

	Rect		mLockRect;
	_void*		mLockBuffer;

	Surface*	mSurfaceList[ _MAX_SURFACE_NUMBER ];

public:
	Texture( _void* tex, _dword width, _dword height, _dword format, _dword mip );
	Texture( Surface* surface );
	~Texture( );

	inline _void* GetTextureObject( ) const
		{ return mTexObject; }

	_void ReleaseTextureObject( );
	_void ResetTextureObject( _void* tex );

	virtual _void		SetName( StringPtr name );
	virtual StringPtr	GetName( ) const;

	virtual _void		SetResName( StringPtr name );
	virtual StringPtr	GetResName( ) const;

	virtual _void		SetUserData( _dword userdata );
	virtual _dword		GetUserData( ) const;

	virtual _dword		GetSize( ) const;
	virtual _dword		GetFormat( ) const;

	virtual _dword		GetSurfaceNumber( ) const;
	virtual _dword		GetSurfaceWidth( _dword level ) const;
	virtual _dword		GetSurfaceHeight( _dword level ) const;

	virtual ISurface*	GetSurface( _dword level );

	virtual _void*		Lock( _dword level, _dword& pitch, const Rect* lockrect = 0 );
	virtual _void		Unlock( _dword level );

	virtual _void		GenerateMipmaps( );
	virtual _void		SaveToFile( StringPtr filename, _dword format ) const;

	virtual _void		ProcessHSL( _dword hue, _dword saturation, _long lightness, const Rect& rect );
	virtual _void		ProcessRGB( _dword modulate, _dword additive, _dword subtract, const Rect& rect );
	virtual _void		ProcessBorder( _dword bordercolor, _bool blendsource, _dword bordersize, _float blurrate, const Rect& rect );

	virtual _void		ProcessFilter( _dword color, const Rect& rect );
};

};