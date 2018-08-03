//============================================================================
// Surface.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Surface
//----------------------------------------------------------------------------

class Surface : public ISurface, public ReferencedObject
{
private:
	_void*		mSufObject;

	_dword		mUserData;

	_dword		mSize;
	_dword		mFormat;

	_dword		mWidth;
	_dword		mHeight;

	Rect		mLockRect;
	_void*		mLockBuffer;

public:
	Surface( _void* suf, _dword width, _dword height, _dword format );
	~Surface( );

	inline _void* GetSurfaceObject( ) const
		{ return mSufObject; }

	_void ReleaseSurfaceObject( );
	_void ResetSurfaceObject( _void* suf );

	virtual _void		SetUserData( _dword userdata );
	virtual _dword		GetUserData( ) const;

	virtual _dword		GetSize( ) const;
	virtual _dword		GetFormat( ) const;

	virtual _dword		GetWidth( ) const;
	virtual _dword		GetHeight( ) const;

	virtual _void*		Lock( _dword& pitch, const Rect* lockrect = 0 );
	virtual _void		Unlock( );

	virtual _void		ProcessHSL( _dword hue, _dword saturation, _long lightness, const Rect& rect );
	virtual _void		ProcessRGB( _dword modulate, _dword additive, _dword subtract, const Rect& rect );
	virtual _void		ProcessBorder( _dword bordercolor, _bool blendsource, _dword bordersize, _float blurrate, const Rect& rect );

	virtual _void		ProcessFilter( _dword color, const Rect& rect );
};

};