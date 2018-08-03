//============================================================================
// ITexture.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ITexture
//----------------------------------------------------------------------------

class ITexture
{
public:
	virtual _void		SetName( StringPtr name ) = 0;
	virtual StringPtr	GetName( ) const = 0;

	virtual _void		SetResName( StringPtr name ) = 0;
	virtual StringPtr	GetResName( ) const = 0;

	virtual _void		SetUserData( _dword userdata ) = 0;
	virtual _dword		GetUserData( ) const = 0;

	virtual _dword		GetSize( ) const = 0;
	virtual _dword		GetFormat( ) const = 0;

	virtual _dword		GetSurfaceNumber( ) const = 0;
	virtual _dword		GetSurfaceWidth( _dword level ) const = 0;
	virtual _dword		GetSurfaceHeight( _dword level ) const = 0;

	virtual ISurface*	GetSurface( _dword level ) = 0;

	virtual _void*		Lock( _dword level, _dword& pitch, const Rect* lockrect = _null ) = 0;
	virtual _void		Unlock( _dword level ) = 0;

	virtual _void		GenerateMipmaps( ) = 0;
	virtual _void		SaveToFile( StringPtr filename, _dword format ) const = 0;

	// Adjust with HSL hue ( 0 - 360 ), saturation ( 0 - 100 ), lightness ( -100 - 100 ).
	virtual _void		ProcessHSL( _dword hue = -1, _dword saturation = -1, _long lightness = 0, const Rect& rect = Rect::cNullRect ) = 0;
	// Adjust with RGB modulate -1 no change, additivecolor 0 no change, subtractcolor 0 no change.
	virtual _void		ProcessRGB( _dword modulate = -1, _dword additive = 0, _dword subtract = 0, const Rect& rect = Rect::cNullRect ) = 0;
	// Add border with blur process
	virtual _void		ProcessBorder( _dword bordercolor, _bool blendsource, _dword bordersize = 2, _float blurrate = 1.0f, const Rect& rect = Rect::cNullRect ) = 0;

	virtual _void		ProcessFilter( _dword color, const Rect& rect = Rect::cNullRect ) = 0;
};

};