//============================================================================
// RenderConfig.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// RenderConfig
//----------------------------------------------------------------------------

class RenderConfig : public IRenderConfig
{
private:
	_bool		mEnableRenderInvisibleGeometry;
	_bool		mEnableRenderNormalGeometry;
	_bool		mEnableRenderAlphaGeometry;

	_bool		mEnableViewFrustumClip;
	_bool		mEnableOverlayPixelAlign;

	_bool		mEnableAlphaRefAttenuation;
	_dword		mMinAlphaRefAttenuationValue;
	_float		mMinAlphaRefAttenuationDistance;
	_float		mMaxAlphaRefAttenuationDistance;

public:
	RenderConfig( );
	~RenderConfig( );

	virtual _bool	TestMultiSampleType( _dword multisampletype );
	virtual _bool	SetMultiSampleType( _dword multisampletype );
	virtual _dword	GetMultiSampleType( ) const;

	virtual _void	EnableVSync( _bool enable );
	virtual _bool	IsVSyncEnabled( ) const;

	virtual _void	EnableAnisotropicSampler( _bool enable );
	virtual _bool	IsAnisotropicSamplerEnabled( ) const;

	virtual _void	EnableRenderInvisibleGeometry( _bool enable );
	virtual _bool	IsRenderInvisibleGeometryEnabled( ) const;

	virtual _void	EnableRenderNormalGeometry( _bool enable );
	virtual _bool	IsRenderNormalGeometryEnabled( ) const;

	virtual _void	EnableRenderAlphaGeometry( _bool enable );
	virtual _bool	IsRenderAlphaGeometryEnabled( ) const;

	virtual _void	EnableViewFrustumClip( _bool enable );
	virtual _bool	IsViewFrustumClipEnabled( ) const;

	virtual _void	EnableOverlayPixelAlign( _bool enable );
	virtual _bool	IsOverlayPixelAlignEnabled( ) const;

	virtual	_void	EnableAlphaRefAttenuation( _bool enable );
	virtual	_bool	IsAlphaRefAttenuationEnabled( ) const;

	virtual	_void	SetMinAlphaRefAttenuationValue( _byte value );
	virtual	_byte	GetMinAlphaRefAttenuationValue( ) const;

	virtual	_void	SetMinAlphaRefAttenuationDistance( _float value );
	virtual	_float	GetMinAlphaRefAttenuationDistance( ) const;

	virtual	_void	SetMaxAlphaRefAttenuationDistance( _float distance );
	virtual	_float	GetMaxAlphaRefAttenuationDistance( ) const;
};

};