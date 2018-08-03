//============================================================================
// FontTexture.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// FontTexture
//----------------------------------------------------------------------------

class FontTexture : public IFontTexture, public ReferencedObject
{
private:
	enum { _TEXTURE_SIZE  = 256 };
	enum { _LEADING_SPACE = 8 };
	enum { _HEIGHT_SPACE  = 4 };

	typedef Array< ITexture* > TextureArray;

	struct FontCode
	{
		_word	mTextureIndex;
		_word	mWidth;
		_word	mStartX;
		_word	mStartY;
	};

	String			mFontName;
	_dword			mFontSize;
	_dword			mFontBorder;
	_dword			mFontEdge;
	_bool			mBlod;
	_bool			mItalic;
	_bool			mUnderline;

	_handle			mOldFont;
	_handle			mGDIFont;
	_handle			mOldMemBitmap;
	_handle			mGDIMemBitmap;
	_handle			mGDIMemDC;

	_dword			mFontSpace;
	_dword			mFontWidth;
	_dword			mFontHeight;

	_dword			mNextCodeX;
	_dword			mNextCodeY;

	_dword			mFontColor;
	_dword			mBorderColor;
	_dword			mEdgeColor;

	_float			mScaleH;
	_float			mScaleV;

	_long			mSpaceHorizontal;
	_long			mSpaceVertical;

	_dword			mCodeNumber;
	FontCode*		mCodeList[256];
	TextureArray	mFontTextureArray;
	TextureArray	mBackTextureArray;
	TextureArray	mEdgeTextureArray;

	_void CreateCode( _dword code );
	_void CreateFontTexture( ITexture* texture, const Rect& rect, const _byte* bitmap, _dword stride );
	_void CreateBackTexture( ITexture* texture, const Rect& rect, const _byte* bitmap, _dword stride );
	_void CreateEdgeTexture( ITexture* texture, const Rect& rect, const _byte* bitmap, _dword stride );

public:
	FontTexture( StringPtr fontname, _dword size, _dword border, _bool blod, _bool italic, _bool underline, _dword edgesize );
	~FontTexture( );

	_float CreateOverlay( OverlayRectangleImage& fontoverlay, _float x, _float y, _dword code, _dword fontcolor );
	_float CreateOverlay( OverlayRectangleImage& fontoverlay, OverlayRectangleImage& backoverlay, _float x, _float y, _dword code, _dword fontcolor, _dword bordercolor );
	_float CreateOverlay( OverlayRectangleImage& fontoverlay, OverlayRectangleImage& backoverlay, OverlayRectangleImage& edgeoverlay, _float x, _float y, _dword code, _dword fontcolor, _dword bordercolor, _dword edgecolor );
	_void Render( _float x, _float y, _dword align, StringPtr string );
	
	_float CreateBillboard( BillboardScreenAligned& billboard, const Vector3& center, const Vector3& rightaxis, const Vector3& upaxis, _float height, _dword code );
	_float CreateBillboard( BillboardScreenAligned& billboard, BillboardScreenAligned& backbillboard, const Vector3& center, const Vector3& rightaxis, const Vector3& upaxis, _float height, _dword code );
	_void Render( const Vector3& center, _float height, StringPtr string );

	_float CreateQuadrangleImage( PrimitiveQuadrangleImage& quadrangle, _float x, _float y, const Vector3& center, const Vector3& directionx, const Vector3& directiony, _float height, _dword code );
	_float CreateQuadrangleImage( PrimitiveQuadrangleImage& quadrangle, PrimitiveQuadrangleImage& backquadrangle, _float x, _float y, const Vector3& center, const Vector3& directionx, const Vector3& directiony, _float height, _dword code );
	_void Render( _float y, const Vector3& center, Vector3 directionx, Vector3 directiony, _float height, StringPtr string );

	virtual StringPtr	GetFontName( ) const;
	virtual _dword		GetFontSize( ) const;
	virtual _dword		GetFontBorder( ) const;
	virtual _dword		GetFontEdge( ) const;
	virtual _bool		IsBlod( ) const;
	virtual _bool		IsItalic( ) const;
	virtual _bool		IsUnderline( ) const;

	virtual _dword GetCharWidth( _char character, _bool scale = _true );
	virtual _dword GetStringWidth( StringPtr string, _bool scale = _true );
	virtual _dword GetFontHeight( );

	virtual _void  SetFontColor( _dword color );
	virtual _dword GetFontColor( ) const;
	virtual _void  SetBorderColor( _dword color );
	virtual _dword GetBorderColor( ) const;
	virtual _void  SetEdgeColor( _dword color );
	virtual _dword GetEdgeColor( ) const;

	virtual _void  SetFontScaleHorizontal( _float scale );
	virtual _float GetFontScaleHorizontal( ) const;
	
	virtual _void  SetFontScaleVertical( _float scale );
	virtual _float GetFontScaleVertical( ) const;

	virtual _void SetFontSpaceHorizontal( _long space );
	virtual _long GetFontSpaceHorizontal( ) const;

	virtual _void SetFontSpaceVertical( _long space );
	virtual _long GetFontSpaceVertical( ) const;

	virtual ITexture* GetFontTexture( _char character, Vector2& uv1, Vector2& uv2 );
};

};