//============================================================================
// FontTexture.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// FontTexture Implementation
//----------------------------------------------------------------------------

FontTexture::FontTexture( StringPtr fontname, _dword size, _dword border, _bool blod, _bool italic, _bool underline, _dword edgesize )
{
	// Get device context from desktop window.
	HDC dc = ::GetDC( _null );

	// Create a compatible memory device context.
	mGDIMemDC = ::CreateCompatibleDC( (HDC) dc );

	// Create gui font from system font library.
	mGDIFont = ::CreateFont( - MulDiv( size, 96, 72 ), 0, 0, 0, blod ? FW_BOLD : FW_NORMAL,
		italic, underline, 0, DEFAULT_CHARSET, 0, 0, 5, 0, fontname );

	mOldFont = ::SelectObject( (HDC) mGDIMemDC, (HGDIOBJ) mGDIFont );
	::SetMapMode( (HDC) mGDIMemDC, MM_TEXT );

	// Get font information.
	TEXTMETRIC textmetric;
	::GetTextMetrics( (HDC) mGDIMemDC, &textmetric );

	mFontName	= fontname;
	mFontSize	= size;
	mFontBorder	= border;
	mFontEdge	= edgesize;
	mBlod		= blod;
	mItalic		= italic;
	mUnderline	= underline;

	mFontSpace	= Math::Max( border, edgesize ) * 2 + _LEADING_SPACE;
	mFontWidth	= textmetric.tmMaxCharWidth * 2 + mFontSpace;
	mFontHeight	= textmetric.tmHeight + Math::Max( border, edgesize ) * 2 + _HEIGHT_SPACE;

	// Create a compatible bitmap with one font size, and select it into device.
	mGDIMemBitmap = ::CreateCompatibleBitmap( (HDC) dc, mFontWidth, mFontHeight );
	mOldMemBitmap = ::SelectObject( (HDC) mGDIMemDC, (HGDIOBJ) mGDIMemBitmap );
	::SetTextColor( (HDC) mGDIMemDC, RGB( 0, 0, 0 ) );

	// Release the device context of desktop window.
	::ReleaseDC( _null, (HDC) dc );

	// Initialize font code list.
	for ( _dword i = 0; i < 256; i ++ )
		mCodeList[i] = _null;

	mNextCodeX			= 0;
	mNextCodeY			= 0;

	mFontColor			= Color::cWhite;
	mBorderColor		= Color::cYellow;
	mEdgeColor			= Color::cRed;
	mCodeNumber			= 0;

	mScaleH				= 1.0f;
	mScaleV				= 1.0f;
	mSpaceHorizontal	= 0;
	mSpaceVertical		= 0;

	IncreaseRefCount( );
}

FontTexture::~FontTexture( )
{
	// Reset GDI font.
	if ( mOldFont != _null )
		::SelectObject( (HDC) mGDIMemDC, (HGDIOBJ) mOldFont );

	// Reset GDI bitmap.
	if ( mOldMemBitmap != _null )
		::SelectObject( (HDC) mGDIMemDC, (HGDIOBJ) mOldMemBitmap );

	// Delete bitmap.
	if ( mGDIMemBitmap != _null )
		::DeleteObject( (HGDIOBJ) mGDIMemBitmap );

	// Delete memory device context.
	if ( mGDIMemDC != _null )
		::DeleteDC( (HDC) mGDIMemDC );

	// Delete GDI font.
	if ( mGDIFont != _null )
		::DeleteObject( (HGDIOBJ) mGDIFont );

	// Release all font textures.
	for ( _dword i = 0; i < mFontTextureArray.Number( ); i ++ )
		GetTextureFactory( ).ReleaseTexture( mFontTextureArray[i] );

	// Release all font background textures.
	for ( _dword i = 0; i < mBackTextureArray.Number( ); i ++ )
		GetTextureFactory( ).ReleaseTexture( mBackTextureArray[i] );

	// Release all font edge textures.
	for ( _dword i = 0; i < mEdgeTextureArray.Number( ); i ++ )
		GetTextureFactory( ).ReleaseTexture( mEdgeTextureArray[i] );

	// Initialize font code list.
	for ( _dword i = 0; i < 256; i ++ )
	{
		if ( mCodeList[i] != _null )
			delete mCodeList[i];
	}
}

_void FontTexture::CreateCode( _dword code )
{
	StringPtr string = (_char*) &code;

	// Get the size of this font code.
	Point size;
	::GetTextExtentPoint( (HDC) mGDIMemDC, string, 1, (SIZE*) &size );
	//size.x += mFontEdge / 2;

	// Make sure size is 2n, for opengl texture problem.
	size.x = ( size.x + 1 ) / 2 * 2;

	_bool createnewtex = _false;

	// Compute the next position of this font code.
	_dword startx = mNextCodeX;
	_dword starty = mNextCodeY;

	// Font too width, choose next line.
	if ( startx + size.x + mFontSpace > _TEXTURE_SIZE )
		{ startx = 0; starty += mFontHeight; }

	// Overuse current texture, craete a new one.
	if ( starty + mFontHeight > _TEXTURE_SIZE )
		{ starty = 0; createnewtex = _true; }

	FL_ASSERT( startx + size.x + mFontSpace <= _TEXTURE_SIZE )
	FL_ASSERT( starty + mFontHeight <= _TEXTURE_SIZE )

	// Draw a blank rectangle first.
	::Rectangle( (HDC) mGDIMemDC, -1, -1, mFontWidth + 1, mFontHeight + 1 );
	::TextOut( (HDC) mGDIMemDC, _LEADING_SPACE + Math::Max( mFontBorder, mFontEdge ), Math::Max( mFontBorder, mFontEdge ), string, 1 );

	// Create GDI+ graphics object.
	Gdiplus::Graphics graphics( (HDC) mGDIMemDC );

	// Get bitmap from graphics object, used for create texture.
	Gdiplus::Bitmap backbitmap( size.x + mFontSpace, mFontHeight, &graphics );
	Gdiplus::Graphics* backgraphics = Gdiplus::Graphics::FromImage( &backbitmap );

	// Copy font to bitmap.
	HDC graphicsdc = backgraphics->GetHDC( );
	::BitBlt( graphicsdc, 0, 0, size.x + mFontSpace, mFontHeight, (HDC) mGDIMemDC, 0, 0, SRCCOPY );
	backgraphics->ReleaseHDC( graphicsdc );

	// Delete GDI+ graphics.
	delete backgraphics;

	// Get bits from back bitmap.
	Gdiplus::BitmapData bitmapdata;
	backbitmap.LockBits( &Gdiplus::Rect( 0, 0, size.x + mFontSpace, mFontHeight ), Gdiplus::ImageLockModeRead, PixelFormat32bppRGB, &bitmapdata );

	Rect lockrect( startx, starty, startx + size.x + mFontSpace, starty + mFontHeight );

	// Create font texture.
	{
		ITexture* fonttexture = _null;

		// All current textures are used, so create another one.
		if ( createnewtex || mFontTextureArray.Number( ) == 0 )
		{
			fonttexture = GetTextureFactory( ).CreateTexture( _TEXTURE_SIZE, _TEXTURE_SIZE, ITextureFactory::_FORMAT_A4R4G4B4, 1 );
			mFontTextureArray.Append( fonttexture );
		}
		// There is texture space to create a font.
		else
		{
			fonttexture = mFontTextureArray[ mFontTextureArray.Number( ) - 1 ];
		}

		if ( fonttexture != _null )
			CreateFontTexture( fonttexture, lockrect, (const _byte*) bitmapdata.Scan0, bitmapdata.Stride );
	}

	// Create border texture.
	if ( mFontBorder > 0 )
	{
		ITexture* backtexture = _null;

		// All current textures are used, so create another one.
		if ( createnewtex || mBackTextureArray.Number( ) == 0 )
		{
			backtexture = GetTextureFactory( ).CreateTexture( _TEXTURE_SIZE, _TEXTURE_SIZE, ITextureFactory::_FORMAT_A8R8G8B8, 1 );
			mBackTextureArray.Append( backtexture );
		}
		// There is texture space to create a font.
		else
		{
			backtexture = mBackTextureArray[ mBackTextureArray.Number( ) - 1 ];
		}

		if ( backtexture != _null )
			CreateBackTexture( backtexture, lockrect, (const _byte*) bitmapdata.Scan0, bitmapdata.Stride );
	}

	if ( mFontEdge > 0 )
	{
		ITexture* edgetexture = _null;

		// All current textures are used, so create another one.
		if ( createnewtex || mEdgeTextureArray.Number( ) == 0 )
		{
			edgetexture = GetTextureFactory( ).CreateTexture( _TEXTURE_SIZE, _TEXTURE_SIZE, ITextureFactory::_FORMAT_A8R8G8B8, 1 );
			mEdgeTextureArray.Append( edgetexture );
		}
		// There is texture space to create a font.
		else
		{
			edgetexture = mEdgeTextureArray[ mEdgeTextureArray.Number( ) - 1 ];
		}

		if ( edgetexture != _null )
			CreateEdgeTexture( edgetexture, lockrect, (const _byte*) bitmapdata.Scan0, bitmapdata.Stride );
	}

	backbitmap.UnlockBits( &bitmapdata );

	if ( mCodeList[ code / 256 ] == _null )
	{
		FontCode* codelist = new FontCode[ 256 ];

		for ( _dword i = 0; i < 256; i ++ )
			codelist[i].mTextureIndex = 0xFFFF;

		mCodeList[ code / 256 ] = codelist;
	}

	FontCode& fontcode = mCodeList[ code / 256 ][ code % 256 ];

	// Set font code information.
	fontcode.mTextureIndex	= _word( mFontTextureArray.Number( ) - 1 );
	fontcode.mWidth			= _word( size.x );
	fontcode.mStartX		= _word( startx );
	fontcode.mStartY		= _word( starty );

	mNextCodeX = startx + size.x + mFontSpace;
	mNextCodeY = starty;
	mCodeNumber ++;
}

_void FontTexture::CreateFontTexture( ITexture* texture, const Rect& rect, const _byte* bitmap, _dword stride )
{
	_dword pitch  = 0;
	_word* buffer = (_word*) texture->Lock( 0, pitch, &rect );

	if ( buffer == _null )
		return;

	_dword width = rect.Width( ), height = rect.Height( );

	pitch /= 2;

	for ( _dword y = 0; y < height; y ++ )
	{
		const _dword* scanline = (const _dword*) ( bitmap + stride * y );

		for ( _dword x = 0; x < width; x ++ )
		{
			// Get RGB color from bitmap, 
			_word rgb = GetRValue( scanline[x] ) + GetGValue( scanline[x] ) + GetBValue( scanline[x] );

			// Compute alpha value from average of RGB color.
			_word alpha = ( 0xFF - (_byte) ( rgb / 3 ) ) >> 4;

			// Combine alpha with color.
			_word color = 0x0FFF | ( alpha << 12 );

			buffer[ x + y * pitch ] = color;
		}
	}

	texture->Unlock( 0 );
}

_void FontTexture::CreateBackTexture( ITexture* texture, const Rect& rect, const _byte* bitmap, _dword stride )
{
	_dword pitch  = 0;
	_byte* buffer = (_byte*) texture->Lock( 0, pitch, &rect );

	if ( buffer == _null )
		return;

	_dword width = rect.Width( ), height = rect.Height( );

	for ( _dword y = 0; y < height; y ++ )
	{
		const _dword* scanline = (const _dword*) ( bitmap + stride * y );

		for ( _dword x = 0; x < width; x ++ )
		{
			// Get RGB color from bitmap.
			_dword rgb = GetRValue( scanline[x] ) + GetGValue( scanline[x] ) + GetBValue( scanline[x] );

			// Compute alpha value from average of RGB color.
			_dword alpha = 0xFF - (_byte) ( rgb / 3 );

			// Clear font
			alpha += alpha;
			if ( alpha > 255 )
				alpha = 255;

			// Combine alpha with color.
			_dword color = 0x00FFFFFF | ( alpha << 24 );

			*( ( (_dword*)( buffer + y * pitch ) ) + x ) = color;
		}
	}

	texture->Unlock( 0 );

	ISurface* surface = texture->GetSurface( 0 );
	if ( surface != _null )
		surface->ProcessBorder( 0xFFFFFFFF, _false, mFontBorder, 1.2f, rect );
}

_void FontTexture::CreateEdgeTexture( ITexture* texture, const Rect& rect, const _byte* bitmap, _dword stride )
{
	_dword pitch  = 0;
	_byte* buffer = (_byte*) texture->Lock( 0, pitch, &rect );

	if ( buffer == _null )
		return;

	_dword width = rect.Width( ), height = rect.Height( );

	for ( _dword y = 0; y < height; y ++ )
	{
		const _dword* scanline = (const _dword*) ( bitmap + stride * y );

		for ( _dword x = 0; x < width; x ++ )
		{
			// Get RGB color from bitmap.
			_dword rgb = GetRValue( scanline[x] ) + GetGValue( scanline[x] ) + GetBValue( scanline[x] );

			// Compute alpha value from average of RGB color.
			_dword alpha = 0xFF - (_byte) ( rgb / 3 );

			// Clear font
			alpha += alpha;
			if ( alpha > 255 )
				alpha = 255;

			// Combine alpha with color.
			_dword color = 0x00FFFFFF | ( alpha << 24 );

			*( ( (_dword*)( buffer + y * pitch ) ) + x ) = color;
		}
	}

	texture->Unlock( 0 );

	ISurface* surface = texture->GetSurface( 0 );
	if ( surface != _null )
		surface->ProcessBorder( 0xFFFFFFFF, _false, mFontEdge, 6.0f, rect );
}

_float FontTexture::CreateOverlay( OverlayRectangleImage& fontoverlay, _float x, _float y, _dword code, _dword fontcolor )
{
	if ( mCodeList[ code / 256 ] == _null || mCodeList[ code / 256 ][ code % 256 ].mTextureIndex == 0xFFFF )
		CreateCode( code );

	FontCode& fontcode = mCodeList[ code / 256 ][ code % 256 ];
	if ( fontcode.mTextureIndex == 0xFFFF )
		return 0.0f;

	// Build uv for the font.
	_float u1 = ( fontcode.mStartX + 0.5f ) / _TEXTURE_SIZE;
	_float v1 = ( fontcode.mStartY + 0.5f ) / _TEXTURE_SIZE;
	_float u2 = ( fontcode.mStartX + fontcode.mWidth + mFontSpace + 0.5f ) / _TEXTURE_SIZE;
	_float v2 = ( fontcode.mStartY + mFontHeight + 0.5f ) / _TEXTURE_SIZE;

	x -= _LEADING_SPACE * mScaleH;
	y += _HEIGHT_SPACE / 2;

	fontoverlay.mPositionLT.x	= x;
	fontoverlay.mPositionLT.y	= y;
	fontoverlay.mPositionRB.x	= x + (_float) ( fontcode.mWidth + mFontSpace ) * mScaleH;
	fontoverlay.mPositionRB.y	= y + (_float) mFontHeight * mScaleV;
	fontoverlay.mTexcoordLT.x	= u1;
	fontoverlay.mTexcoordLT.y	= v1;
	fontoverlay.mTexcoordRB.x	= u2;
	fontoverlay.mTexcoordRB.y	= v2;
	fontoverlay.mColor			= fontcolor;
	fontoverlay.mTexture		= mFontTextureArray[ fontcode.mTextureIndex ];

	return fontcode.mWidth == 0 ? 0.0f : (_float) ( fontcode.mWidth + mSpaceHorizontal ) * mScaleH;
}

_float FontTexture::CreateOverlay( OverlayRectangleImage& fontoverlay, OverlayRectangleImage& backoverlay, _float x, _float y, _dword code, _dword fontcolor, _dword bordercolor )
{
	_float width = CreateOverlay( fontoverlay, x, y, code, fontcolor );

	FontCode& fontcode = mCodeList[ code / 256 ][ code % 256 ];
	if ( fontcode.mTextureIndex == 0xFFFF )
		return width;

	if ( mFontBorder != 0 )
	{
		backoverlay = fontoverlay;

		backoverlay.mColor   = bordercolor;
		backoverlay.mTexture = mBackTextureArray[ fontcode.mTextureIndex ];
	}

	return width;
}

_float FontTexture::CreateOverlay( OverlayRectangleImage& fontoverlay, OverlayRectangleImage& backoverlay, OverlayRectangleImage& edgeoverlay, _float x, _float y, _dword code, _dword fontcolor, _dword bordercolor, _dword edgecolor )
{
	_float width = CreateOverlay( fontoverlay, x, y, code, fontcolor );

	FontCode& fontcode = mCodeList[ code / 256 ][ code % 256 ];
	if ( fontcode.mTextureIndex == 0xFFFF )
		return width;

	if ( mFontBorder != 0 )
	{
		backoverlay = fontoverlay;

		backoverlay.mColor   = bordercolor;
		backoverlay.mTexture = mBackTextureArray[ fontcode.mTextureIndex ];
	}

	if ( mFontEdge != 0 )
	{
		edgeoverlay = fontoverlay;

		edgeoverlay.mColor   = edgecolor;
		edgeoverlay.mTexture = mEdgeTextureArray[ fontcode.mTextureIndex ];
	}

	return width;
}

_void FontTexture::Render( _float x, _float y, _dword align, StringPtr string )
{
	if ( align & IFontTexture::_HALIGN_CENTER )
	{
		x = x - (_float) GetStringWidth( string ) / 2.0f;
	}
	else if ( align & IFontTexture::_HALIGN_RIGHT )
	{
		x = x - (_float) GetStringWidth( string );
	}

	if ( align & IFontTexture::_VALIGN_CENTER )
	{
		y = y - (_float) GetFontHeight( ) / 2.0f;
	}
	else if ( align & IFontTexture::_VALIGN_BOTTOM )
	{
		y = y - (_float) GetFontHeight( );
	}

	_dword length = string.Length( ), fontcolor = mFontColor;

	for ( _dword i = 0; string[i] != 0; i ++ )
	{
		// Parse font controller.
		if ( string[i] == _FONT_CONTROLLER_COLOR_CODE )
		{
			_char tempbuffer[ _FONT_CONTROLLER_COLOR_LENGHT ];
			StringFormatter::CopyString( tempbuffer, (const _char*) string + i + 1, _FONT_CONTROLLER_COLOR_LENGHT - 1 );

			fontcolor = Dword( tempbuffer );

			if ( fontcolor == 0 )
			{
				fontcolor = mFontColor;
			}
			else
			{
				fontcolor = ( fontcolor & 0x00FFFFFF ) | ( mFontColor & 0xFF000000 );
			}

			i = i + _FONT_CONTROLLER_COLOR_LENGHT < length ? i + _FONT_CONTROLLER_COLOR_LENGHT : length;

			i --;
		}
		// Skip \r and \n
		else if ( string[i] != L'\r' && string[i] != L'\n' )
		{
			if ( mFontEdge == 0 )
			{
				if ( mFontBorder == 0 )
				{
					OverlayRectangleImage fontoverlay;

					// Create font image to render.
					x += CreateOverlay( fontoverlay, x, y, string[i], fontcolor );

					// Render font image.
					GetRenderDevice( ).Render( fontoverlay );
				}
				else
				{
					OverlayRectangleImage fontoverlay, backoverlay;

					// Create font image and background image to render.
					x += CreateOverlay( fontoverlay, backoverlay, x, y, string[i], fontcolor, mBorderColor );

					// Render background image.
					GetRenderDevice( ).Render( backoverlay );

					// Render font image.
					GetRenderDevice( ).Render( fontoverlay );
				}
			}
			else
			{
				OverlayRectangleImage fontoverlay, backoverlay, edgeoverlay;

				// Create font image and background image to render.
				x += CreateOverlay( fontoverlay, backoverlay, edgeoverlay, x, y, string[i], fontcolor, mBorderColor, mEdgeColor );

				// Render background image.
				if ( mFontBorder != 0 )
					GetRenderDevice( ).Render( backoverlay );

				// Render edge image.
				if ( mFontEdge != 0 )
					GetRenderDevice( ).Render( edgeoverlay );

				// Render font image.
				GetRenderDevice( ).Render( fontoverlay );
			}
		}		
	}
}

_float FontTexture::CreateBillboard( BillboardScreenAligned& billboard, const Vector3& center, const Vector3& rightaxis, const Vector3& upaxis, _float height, _dword code )
{
	if ( mCodeList[ code / 256 ] == _null || mCodeList[ code / 256 ][ code % 256 ].mTextureIndex == 0xFFFF )
		CreateCode( code );

	FontCode& fontcode = mCodeList[ code / 256 ][ code % 256 ];
	if ( fontcode.mTextureIndex == 0xFFFF )
		return 0.0f;

	// Build uv for the font.
	_float u1 = ( fontcode.mStartX + 0.5f ) / _TEXTURE_SIZE;
	_float v1 = ( fontcode.mStartY + 0.5f ) / _TEXTURE_SIZE;
	_float u2 = ( fontcode.mStartX + fontcode.mWidth + mFontSpace + 0.5f ) / _TEXTURE_SIZE;
	_float v2 = ( fontcode.mStartY + mFontHeight + 0.5f ) / _TEXTURE_SIZE;

	_float rate = height / mFontHeight;

	billboard.mType			= BillboardScreenAligned::_TYPE_BILLBOARD_SCREENALIGNED;
	billboard.mPosition		= center + rightaxis * ( ( fontcode.mWidth - _LEADING_SPACE ) * rate * 0.5f ) - upaxis * ( _HEIGHT_SPACE * rate * 0.5f );
	billboard.mSize			= Vector2( mFontHeight * 0.5f, ( fontcode.mWidth + mFontSpace ) * 0.5f ) * rate;
	billboard.mTexture		= mFontTextureArray[ fontcode.mTextureIndex ];
	billboard.mTexcoordLT	= Vector2( u1, v1 );
	billboard.mTexcoordLB	= Vector2( u1, v2 );
	billboard.mTexcoordRT	= Vector2( u2, v1 );
	billboard.mTexcoordRB 	= Vector2( u2, v2 );
	billboard.mColorLT		= mFontColor;
	billboard.mColorLB		= mFontColor;
	billboard.mColorRB		= mFontColor;
	billboard.mColorRT		= mFontColor;
	billboard.mRotation		= 0.0f;

	return fontcode.mWidth * rate;
}

_float FontTexture::CreateBillboard( BillboardScreenAligned& billboard, BillboardScreenAligned& backbillboard, const Vector3& center, const Vector3& rightaxis, const Vector3& upaxis, _float height, _dword code )
{
	_float xoffset = CreateBillboard( billboard, center, rightaxis, upaxis, height, code );

	FontCode& fontcode = mCodeList[ code / 256 ][ code % 256 ];
	if ( fontcode.mTextureIndex == 0xFFFF )
		return 0.0f;

	backbillboard.mType			= billboard.mType;
	backbillboard.mSize			= billboard.mSize;
	backbillboard.mTexture		= mEdgeTextureArray[ fontcode.mTextureIndex ];
	backbillboard.mTexcoordLT	= billboard.mTexcoordLT;
	backbillboard.mTexcoordLB	= billboard.mTexcoordLB;
	backbillboard.mTexcoordRB 	= billboard.mTexcoordRB;
	backbillboard.mTexcoordRT	= billboard.mTexcoordRT;
	backbillboard.mColorLT		= mEdgeColor;
	backbillboard.mColorLB		= mEdgeColor;
	backbillboard.mColorRB		= mEdgeColor;
	backbillboard.mColorRT		= mEdgeColor;
	backbillboard.mRotation		= 0.0f;

	return xoffset;
}

_void FontTexture::Render( const Vector3& center, _float height, StringPtr string )
{
	Matrix4 worldtransforminverse = GetRenderDevice( ).GetWorldTransform( );
	Vector3 newcenter = center * worldtransforminverse;
	worldtransforminverse.Inverse( );

	_float scale = ( Vector3::cZAxis * GetRenderDevice( ).GetNormalTransform( ) ).Magnitude( );
	height *= scale;

	_float rate = height / mFontHeight;

	// Always center.
	_float x = GetStringWidth( string ) * rate * -0.5f;

	const Matrix4& viewtransform = GetRenderDevice( ).GetViewTransform( );

	Vector3 upaxis		= Vector3( viewtransform( 0, 1 ), viewtransform( 1, 1 ), viewtransform( 2, 1 ) ).Normalize( );
	Vector3 rightaxis	= Vector3( viewtransform( 0, 0 ), viewtransform( 1, 0 ), viewtransform( 2, 0 ) ).Normalize( );
	Vector3 lookaxis	= Vector3::Cross( upaxis, rightaxis ).Normalize( );

	for ( _dword i = 0; string[i] != 0; i ++ )
	{
		if ( mFontEdge == 0 )
		{
			BillboardScreenAligned fontbillboard;

			// Create font image to render.
			x += CreateBillboard( fontbillboard, newcenter + rightaxis * x, rightaxis, upaxis, height, string[i] );

			// Render font image.
			fontbillboard.mPosition *= worldtransforminverse;
			GetRenderDevice( ).Render( fontbillboard );
		}
		else
		{
			BillboardScreenAligned fontbillboard, backbillboard;

			// Create font image and background image to render.
			x += CreateBillboard( fontbillboard, backbillboard, newcenter + rightaxis * x, rightaxis, upaxis, height, string[i] );

			backbillboard.mPosition = fontbillboard.mPosition - lookaxis * ( height * 0.001f );

			// Render background image.
			backbillboard.mPosition *= worldtransforminverse;
			GetRenderDevice( ).Render( backbillboard );

			// Render font image.
			fontbillboard.mPosition *= worldtransforminverse;
			GetRenderDevice( ).Render( fontbillboard );
		}
	}
}

_float FontTexture::CreateQuadrangleImage( PrimitiveQuadrangleImage& quadrangle, _float x, _float y, const Vector3& center, const Vector3& directionx, const Vector3& directiony, _float height, _dword code )
{
	if ( mCodeList[ code / 256 ] == _null || mCodeList[ code / 256 ][ code % 256 ].mTextureIndex == 0xFFFF )
		CreateCode( code );

	FontCode& fontcode = mCodeList[ code / 256 ][ code % 256 ];
	if ( fontcode.mTextureIndex == 0xFFFF )
		return 0.0f;

	// Build uv for the font.
	_float u1 = ( fontcode.mStartX + 0.5f ) / _TEXTURE_SIZE;
	_float v1 = ( fontcode.mStartY + 0.5f ) / _TEXTURE_SIZE;
	_float u2 = ( fontcode.mStartX + fontcode.mWidth + mFontSpace + 0.5f ) / _TEXTURE_SIZE;
	_float v2 = ( fontcode.mStartY + mFontHeight + 0.5f ) / _TEXTURE_SIZE;

	// Scale character to fit line height
	Vector2 size( fontcode.mWidth / 2.0f, mFontHeight / 2.0f );

	_float rate = height / 2.0f / size.y;

	size *= rate; x *= rate; y *= rate;

	quadrangle.mCenter		= center - directionx * ( x + size.x ) - directiony * y;
	quadrangle.mHeight		= directiony * -size.y;
	quadrangle.mWidth		= directionx * -size.x;
	quadrangle.mColor		= mFontColor;
	quadrangle.mTexture		= mFontTextureArray[ fontcode.mTextureIndex ];
	quadrangle.mTexcoordLT	= Vector2( u1, v1 );
	quadrangle.mTexcoordLB	= Vector2( u1, v2 );
	quadrangle.mTexcoordRT	= Vector2( u2, v1 );
	quadrangle.mTexcoordRB 	= Vector2( u2, v2 );

	return (_float) fontcode.mWidth;
}

_float FontTexture::CreateQuadrangleImage( PrimitiveQuadrangleImage& quadrangle, PrimitiveQuadrangleImage& backquadrangle, _float x, _float y, const Vector3& center, const Vector3& directionx, const Vector3& directiony, _float height, _dword code )
{
	_float xoffset = CreateQuadrangleImage( quadrangle, x, y, center, directionx, directiony, height, code );

	FontCode& fontcode = mCodeList[ code / 256 ][ code % 256 ];
	if ( fontcode.mTextureIndex == 0xFFFF )
		return 0.0f;

	Vector3 lookaxis = Vector3::Cross( directionx, directiony ).Normalize( );

	backquadrangle.mCenter		= quadrangle.mCenter;
	quadrangle.mCenter			= backquadrangle.mCenter + lookaxis * 0.01f;
	backquadrangle.mHeight		= quadrangle.mHeight;
	backquadrangle.mWidth		= quadrangle.mWidth;	
	backquadrangle.mColor		= mBorderColor;
	backquadrangle.mTexture		= mBackTextureArray[ fontcode.mTextureIndex ];
	backquadrangle.mTexcoordLT	= quadrangle.mTexcoordLT;
	backquadrangle.mTexcoordLB	= quadrangle.mTexcoordLB;
	backquadrangle.mTexcoordRB 	= quadrangle.mTexcoordRB;
	backquadrangle.mTexcoordRT	= quadrangle.mTexcoordRT;
	
	return xoffset;
}

_void FontTexture::Render( _float y, const Vector3& center, Vector3 directionx, Vector3 directiony, _float height, StringPtr string )
{
	directionx.Normalize( );
	directiony.Normalize( );

	// Always center
	_float x = GetStringWidth( string ) / -2.0f;
	
	for ( _dword i = 0; string[i] != 0; i ++ )
	{
		if ( mFontBorder == 0 )
		{
			PrimitiveQuadrangleImage quadrangle;
						
			// Create font image to render.
			x += CreateQuadrangleImage( quadrangle, x, y, center, directionx, directiony, height, string[i] );

			// Render font image.
			GetRenderDevice( ).Render( quadrangle );
		}
		else
		{
			PrimitiveQuadrangleImage fontquadrangle, backquadrangle;

			// Create font image and background image to render.
			x += CreateQuadrangleImage( fontquadrangle, backquadrangle, x, y, center, directionx, directiony, height, string[i] );

			// Render background image.
			GetRenderDevice( ).Render( backquadrangle );

			// Render font image.
			GetRenderDevice( ).Render( fontquadrangle );
		}
	}
}

StringPtr FontTexture::GetFontName( ) const
{
	return mFontName;
}

_dword FontTexture::GetFontSize( ) const
{
	return mFontSize;
}

_dword FontTexture::GetFontBorder( ) const
{
	return mFontBorder;
}

_dword FontTexture::GetFontEdge( ) const
{
	return mFontEdge;
}

_bool FontTexture::IsBlod( ) const
{
	return mBlod;
}

_bool FontTexture::IsItalic( ) const
{
	return mItalic;
}

_bool FontTexture::IsUnderline( ) const
{
	return mUnderline;
}

_dword FontTexture::GetCharWidth( _char character, _bool scale )
{
	// Create font texture if needed.
	if ( mCodeList[ character / 256 ] == _null || mCodeList[ character / 256 ][ character % 256 ].mTextureIndex == 0xFFFF )
		CreateCode( character );

	FontCode& fontcode = mCodeList[ character / 256 ][ character % 256 ];

	_dword width = _dword( fontcode.mWidth + mSpaceHorizontal );

	if ( scale )
		width = _dword( width * mScaleH + 0.5f );

	return width;
}

_dword FontTexture::GetStringWidth( StringPtr string, _bool scale )
{
	_dword length = 0; 

	for ( _char* buffer = (_char*) (const _char*) string; *buffer != 0; buffer ++ )
	{		
		// Parse font controller.
		if ( *buffer == _FONT_CONTROLLER_COLOR_CODE )
		{
			for ( _dword i = 0; *buffer != 0 && i < _FONT_CONTROLLER_COLOR_LENGHT; i ++, buffer ++ );

			-- buffer;
		}
		else
		{
			_dword character = *( (_word*) buffer );

			// Create font texture if needed.
			if ( mCodeList[ character / 256 ] == _null || mCodeList[ character / 256 ][ character % 256 ].mTextureIndex == 0xFFFF )
				CreateCode( character );

			FontCode& fontcode = mCodeList[ character / 256 ][ character % 256 ];

			// Increase string length by add width of the font code.
			length += fontcode.mWidth + mSpaceHorizontal;
		}
	}

	_dword width = _dword( length + Math::Max( mFontBorder, mFontEdge ) * 2 );

	if ( scale )
		width = _dword( width * mScaleH + 0.5f );

	return width;
}

_dword FontTexture::GetFontHeight( )
{
	return _dword( ( mFontHeight + (_float)mSpaceVertical ) * mScaleV );
}

_void FontTexture::SetFontColor( _dword color )
{
	mFontColor = color;
}

_dword FontTexture::GetFontColor( ) const
{
	return mFontColor;
}

_void FontTexture::SetBorderColor( _dword color )
{
	mBorderColor = color;
}

_dword FontTexture::GetBorderColor( ) const
{
	return mBorderColor;
}

_void FontTexture::SetEdgeColor( _dword color )
{
	mEdgeColor = color;
}

_dword FontTexture::GetEdgeColor( ) const
{
	return mEdgeColor;
}

_void FontTexture::SetFontScaleHorizontal( _float scale )
{
	mScaleH = scale;
}

_float FontTexture::GetFontScaleHorizontal( ) const
{
	return mScaleH;
}

_void FontTexture::SetFontScaleVertical( _float scale )
{
	mScaleV = scale;
}

_float FontTexture::GetFontScaleVertical( ) const
{
	return mScaleV;
}

_void FontTexture::SetFontSpaceHorizontal( _long space )
{
	mSpaceHorizontal = space;
}

_long FontTexture::GetFontSpaceHorizontal( ) const
{
	return mSpaceHorizontal;
}

_void FontTexture::SetFontSpaceVertical( _long space )
{
	mSpaceVertical = space;
}

_long FontTexture::GetFontSpaceVertical( ) const
{
	return mSpaceVertical;
}

ITexture* FontTexture::GetFontTexture( _char character, Vector2& uv1, Vector2& uv2 )
{
	if ( mCodeList[ character / 256 ] == _null || mCodeList[ character / 256 ][ character % 256 ].mTextureIndex == 0xFFFF )
		CreateCode( character );

	FontCode& fontcode = mCodeList[ character / 256 ][ character % 256 ];
	if ( fontcode.mTextureIndex == 0xFFFF )
		return _null;

	// Build uv for the font.
	_float u1 = ( fontcode.mStartX + 0.5f ) / _TEXTURE_SIZE;
	_float v1 = ( fontcode.mStartY + 0.5f ) / _TEXTURE_SIZE;
	_float u2 = ( fontcode.mStartX + fontcode.mWidth + mFontSpace + 0.5f ) / _TEXTURE_SIZE;
	_float v2 = ( fontcode.mStartY + mFontHeight + 0.5f ) / _TEXTURE_SIZE;

	uv1 = Vector2( u1, v1 );
	uv2 = Vector2( u2, v2 );

	return mFontTextureArray[ fontcode.mTextureIndex ];
}
