//============================================================================
// ISurface.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ISurface
//----------------------------------------------------------------------------

class ISurface
{
public:
	virtual _void		SetUserData( _dword userdata ) = 0;
	virtual _dword		GetUserData( ) const = 0;

	virtual _dword		GetSize( ) const = 0;
	virtual _dword		GetFormat( ) const = 0;

	virtual _dword		GetWidth( ) const = 0;
	virtual _dword		GetHeight( ) const = 0;

	virtual _void*		Lock( _dword& pitch, const Rect* lockrect = _null ) = 0;
	virtual _void		Unlock( ) = 0;

	// Adjust with HSL hue ( 0 - 360 ), saturation ( 0 - 100 ), lightness ( -100 - 100 ).
	virtual _void		ProcessHSL( _dword hue = -1, _dword saturation = -1, _long lightness = 0, const Rect& rect = Rect::cNullRect ) = 0;
	// Adjust with RGB modulate -1 no change, additivecolor 0 no change, subtractcolor 0 no change.
	virtual _void		ProcessRGB( _dword modulate = -1, _dword additive = 0, _dword subtract = 0, const Rect& rect = Rect::cNullRect ) = 0;
	// Add border with blur process
	virtual _void		ProcessBorder( _dword bordercolor, _bool blendsource, _dword bordersize = 2, _float blurrate = 1.0f, const Rect& rect = Rect::cNullRect ) = 0;

	virtual _void		ProcessFilter( _dword color, const Rect& rect = Rect::cNullRect ) = 0;
};

};