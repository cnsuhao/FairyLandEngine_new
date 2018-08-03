//============================================================================
// IFontTexture.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IFontTexture
//----------------------------------------------------------------------------

class IFontTexture
{
public:
	enum _FONT_ALIGN
	{
		_HALIGN_LEFT	= 0x00,
		_HALIGN_CENTER	= 0x01,
		_HALIGN_RIGHT	= 0x02,

		_VALIGN_TOP		= 0x00,
		_VALIGN_CENTER	= 0x10,
		_VALIGN_BOTTOM	= 0x20,
	};

	enum _FONT_CONTROLLER
	{
		_FONT_CONTROLLER_COLOR_CODE		= 127,
		_FONT_CONTROLLER_COLOR_LENGHT	= 9,
	};

public:
	virtual StringPtr	GetFontName( ) const = 0;
	virtual _dword		GetFontSize( ) const = 0;
	virtual _dword		GetFontBorder( ) const = 0;
	virtual _dword		GetFontEdge( ) const = 0;
	virtual _bool		IsBlod( ) const = 0;
	virtual _bool		IsItalic( ) const = 0;
	virtual _bool		IsUnderline( ) const = 0;

	virtual _dword		GetCharWidth( _char character, _bool scale = _true ) = 0;
	virtual _dword		GetStringWidth( StringPtr string, _bool scale = _true ) = 0;
	virtual _dword		GetFontHeight( ) = 0;

	virtual _void		SetFontColor( _dword color ) = 0;
	virtual _dword		GetFontColor( ) const = 0;
	virtual _void		SetBorderColor( _dword color ) = 0;
	virtual _dword		GetBorderColor( ) const = 0;
	virtual _void		SetEdgeColor( _dword color ) = 0;
	virtual _dword		GetEdgeColor( ) const = 0;
	
	virtual _void		SetFontScaleHorizontal( _float scale ) = 0;
	virtual _float		GetFontScaleHorizontal( ) const = 0;
	
	virtual _void		SetFontScaleVertical( _float scale ) = 0;
	virtual _float		GetFontScaleVertical( ) const = 0;

	virtual _void		SetFontSpaceHorizontal( _long space ) = 0;
	virtual _long		GetFontSpaceHorizontal( ) const = 0;

	virtual _void		SetFontSpaceVertical( _long space ) = 0;
	virtual _long		GetFontSpaceVertical( ) const = 0;

	virtual ITexture*	GetFontTexture( _char character, Vector2& uv1, Vector2& uv2 ) = 0;
};

};