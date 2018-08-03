//============================================================================
// Viewport.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Viewport
//----------------------------------------------------------------------------

class Viewport
{
public:
	const static Viewport cNull;

public:
	_long	x;
	_long	y;
	_long	w;
	_long	h;
	_float	minz;
	_float	maxz;

public:
	inline Viewport( );
	inline Viewport( _long xx, _long yy, _long ww, _long hh, _float minzz, _float maxzz );

	inline _bool operator == ( const Viewport& viewport ) const;
	inline _bool operator != ( const Viewport& viewport ) const;

	inline _float GetAspectRatio( ) const;
};

//----------------------------------------------------------------------------
// Viewport Implementation
//----------------------------------------------------------------------------

Viewport::Viewport( )
{
}

Viewport::Viewport( _long xx, _long yy, _long ww, _long hh, _float minzz, _float maxzz )
	: x( xx ), y( yy ), w( ww ), h( hh ), minz( minzz ), maxz( maxzz )
{
}

_bool Viewport::operator == ( const Viewport& viewport ) const
{
	return x == viewport.x && y == viewport.y && w == viewport.w && h == viewport.h && minz == viewport.minz && maxz == viewport.maxz;
}

_bool Viewport::operator != ( const Viewport& viewport ) const
{
	return x != viewport.x || y != viewport.y || w != viewport.w || h != viewport.h || minz != viewport.minz || maxz != viewport.maxz;
}

_float Viewport::GetAspectRatio( ) const
{
	return (_float) w / (_float) h;
}

};