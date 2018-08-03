//============================================================================
// SurfaceProcessor.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// SurfaceProcessor Implementation
//----------------------------------------------------------------------------

_void SurfaceProcessor::MakeGaussianBlurKernel( _float* matrix, _long radius )
{
	_long	rows		 = radius * 2 + 1;
	_float	sigma		 = radius / 1.2f;
	_float	sigma22		 = 2.0f * sigma * sigma;
	_float	sigmaPi2	 = 2.0f * Math::cPi * sigma;
	_float	sqrtSigmaPi2 = Math::Sqrt( sigmaPi2 );
	_float	radius2		 = _float( radius * radius );
	_float	total		 = 0;
	_long	index		 = 0;

	for ( _long row = -radius; row <= radius; row ++ )
	{
		for ( _long col = -radius; col <= radius; col ++ )
		{
			_float distance = _float( row * row + col * col );
			matrix[index] = Math::Exp(-distance/sigma22) / sqrtSigmaPi2;

			total += matrix[index];
			index ++;
		}
	}

	for ( _long i = 0; i < rows * rows; i ++ )
		matrix[i] /= total;
}

_dword SurfaceProcessor::GetDword( _byte* buffer, _dword pitch, const Rect& rect, _long x, _long y )
{
	if ( y < rect.t || y >= rect.b )
		return 0;

	if ( x < rect.l || x >= rect.r )
		return 0;

	_dword* line = (_dword*)( buffer + y * pitch );
	return *( line + x );
}

_void SurfaceProcessor::GetDwordMatrix( _byte* buffer, _dword pitch, const Rect& rect, _long x, _long y, _dword* data, _dword xsize, _dword ysize )
{
	_dword index = 0;

	for ( _long i = y - ysize; i <= _long( y + ysize ); i ++ )
	{
		for ( _long j = x - xsize; j <= _long( x + xsize ); j ++ )
		{
			data[index] = GetDword( buffer, pitch, rect, j, i );
			index ++;
		}
	}
}

_void SurfaceProcessor::ProcessHSL( _byte* buffer, _dword pitch, _dword hue, _dword saturation, _long lightness, const Rect& rect )
{
	for ( _long i = rect.t; i < rect.b; i ++ )
	{
		for ( _long j = rect.l; j < rect.r; j ++ )
		{
			Color color( GetDword( buffer, pitch, rect, j, i ) );

			// Speed up
			if ( color.a == 0.0f )
				continue;

			_float h, s, l;
			color.ToHSL( h, s, l );

			if ( hue != -1 )
				h = hue / 360.0f;

			if ( saturation != -1 )
				s = saturation / 100.0f;

			if ( lightness != 0 )
			{
				l = l + lightness / 100.0f;

				if ( l > 1.0f )
					l = 1.0f;

				if ( l < 0.0f )
					l = 0.0f;
			}

			color.FromHSL( h, s, l );

			*( ( (_dword*)( buffer + i * pitch ) ) + j ) = color;
		}
	}
}

_void SurfaceProcessor::ProcessRGB( _byte* buffer, _dword pitch, _dword modulate, _dword additive, _dword subtract, const Rect& rect )
{
	for ( _long i = rect.t; i < rect.b; i ++ )
	{
		for ( _long j = rect.l; j < rect.r; j ++ )
		{
			Color color( GetDword( buffer, pitch, rect, j, i ) );
			
			if ( color.a == 0.0f )
				continue;

			if ( modulate != 1 )
				color *= Color( modulate );

			if ( additive != 0 )
				color += Color( additive );

			if ( subtract != 0 )
				color -= Color( subtract );

			*( ( (_dword*)( buffer + i * pitch ) ) + j ) = color;
		}
	}
}

_void SurfaceProcessor::ProcessBorder( _byte* buffer, _dword pitch, _dword bordercolor, _bool blendsource, _dword bordersize, _float blurrate, const Rect& rect )
{
	// Border Parameters
	_dword realbordercolor	= bordercolor & 0x00FFFFFF;

	// Blur Parameters
	_dword blurradius		= bordersize;
	_dword blursize			= 2 * blurradius + 1;
	_dword blursize2		= blursize * blursize;

	_float* blurkernel		= new _float[ blursize2 ];
	_dword* oricolorarray	= new _dword[ blursize2 ];
	_dword* blurimage		= new _dword[ rect.Height( ) * rect.Width( ) ];

	Memory::MemSet( oricolorarray, 0, blursize2 * 4 );
	MakeGaussianBlurKernel( blurkernel, blurradius );

	// Blur Step for speed up
	_dword step = 1;

	// Blur and alpha blend
	for ( _long i = rect.t; i < rect.b; i ++ )
	{
		for ( _long j = rect.l; j < rect.r; j ++ )
		{
			// Ori color
			GetDwordMatrix( buffer, pitch, rect, j, i, oricolorarray, blurradius, blurradius );

			// Speed up
			Color color0( oricolorarray[ ( blursize2 - 1 ) / 2 ] );

			if ( blendsource && color0.a == 1.0f )
			{
				blurimage[ ( i - rect.t ) * rect.Width( ) + ( j - rect.l ) ] = *( ( (_dword*)( buffer + i * pitch ) ) + j );
				continue;
			}

			// Blur
			_float alpha = 0.0f;
			for ( _dword m = 0; m < blursize2; m += step )
			{
				_float aa = ( ( oricolorarray[m] & 0xFF000000 ) >> 24 ) / 255.0f;
				alpha += aa * blurkernel[m];
			}

			alpha *= step;
			alpha *= blurrate;

			if ( alpha > 1.0f )
				alpha = 1.0f;

			_dword aa = (_dword) ( alpha * 255.0f );		
			_dword final = ( aa << 24 ) | realbordercolor;

			if ( blendsource && color0.a > 0.0f )
			{
				Color color( final );
				color.a = Math::Max( color0.a, color.a );
				color.r = color0.r * color0.a + color.r * ( 1.0f - color0.a );
				color.g = color0.g * color0.a + color.g * ( 1.0f - color0.a );
				color.b = color0.b * color0.a + color.b * ( 1.0f - color0.a );
				final = color;
			}

			blurimage[ ( i - rect.t ) * rect.Width( ) + ( j - rect.l ) ] = final;
		}
	}

	for ( _long i = rect.t; i < rect.b; i ++ )
	{
		for ( _long j = rect.l; j < rect.r; j ++ )
		{
			// Set back
			*( ( (_dword*)( buffer + i * pitch ) ) + j ) = blurimage[ ( i - rect.t ) * rect.Width( ) + ( j - rect.l ) ];
		}
	}

	delete [] blurimage;
	delete [] blurkernel;
	delete [] oricolorarray;
}

_void SurfaceProcessor::ProcessFilter( _byte* buffer, _dword pitch, _dword color, const Rect& rect )
{
	_float h = 0.0f, s = 0.0f, l = 0.0f;
	Color( color ).ToHSL( h, s, l );

	//color &= 0x00FFFFFF;

	//_dword r = color >> 16;
	//_dword g = ( color & 0x0000FFFF ) >> 8;
	//_dword b = ( color & 0x000000FF );

	_float hh = 0.0f, ss = 0.0f, ll = 0.0f;

	Color temp;

	for ( _long y = rect.t; y < rect.b; y ++ )
	{
		_dword* line = (_dword*) buffer;

		for ( _long x = rect.l; x < rect.r; x ++ )
		{
			_dword a = line[x] >> 24;

			if ( a == 0 )
				continue;

			_float aa = a / 255.0f;

			Color( line[x] ).ToHSL( hh, ss, ll );
			temp.FromHSL( h, ss, ll );
			temp.a = (_float) a;

			line[x] = temp;

			//line[x] = ( a << 24 ) | ( _dword( r * aa ) << 16 ) | ( _dword( g * aa ) << 8 ) | _dword( b * aa );
		}

		buffer += pitch;
	}
}