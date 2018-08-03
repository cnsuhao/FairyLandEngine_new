//============================================================================
// SurfaceProcessor.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// SurfaceProcessor
//----------------------------------------------------------------------------

class SurfaceProcessor
{
private:
	static _void	MakeGaussianBlurKernel( _float* matrix, _long radius );

	static _dword	GetDword( _byte* buffer, _dword pitch, const Rect& rect, _long x, _long y );
	static _void	GetDwordMatrix( _byte* buffer, _dword pitch, const Rect& rect, _long x, _long y, _dword* data, _dword xsize, _dword ysize );

public:
	static _void	ProcessHSL( _byte* buffer, _dword pitch, _dword hue, _dword saturation, _long lightness, const Rect& rect );
	static _void	ProcessRGB( _byte* buffer, _dword pitch, _dword modulate, _dword additive, _dword subtract, const Rect& rect );
	static _void	ProcessBorder( _byte* buffer, _dword pitch, _dword bordercolor, _bool blendsource, _dword bordersize, _float blurrate, const Rect& rect );
	static _void	ProcessFilter( _byte* buffer, _dword pitch, _dword color, const Rect& fixedrect );
};

};