//============================================================================
// IRenderQueue.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

namespace FL
{

//----------------------------------------------------------------------------
// IRenderConfig
//----------------------------------------------------------------------------

class IRenderConfig
{
public:
	virtual _bool	TestMultiSampleType( _dword multisampletype ) = 0;
	virtual _bool	SetMultiSampleType( _dword multisampletype ) = 0;
	virtual _dword	GetMultiSampleType( ) const = 0;

	virtual _void	EnableVSync( _bool enable ) = 0;
	virtual _bool	IsVSyncEnabled( ) const = 0;

	virtual _void	EnableAnisotropicSampler( _bool enable ) = 0;
	virtual _bool	IsAnisotropicSamplerEnabled( ) const = 0;

	virtual _void	EnableRenderInvisibleGeometry( _bool enable ) = 0;
	virtual _bool	IsRenderInvisibleGeometryEnabled( ) const = 0;

	virtual _void	EnableRenderNormalGeometry( _bool enable ) = 0;
	virtual _bool	IsRenderNormalGeometryEnabled( ) const = 0;

	virtual _void	EnableRenderAlphaGeometry( _bool enable ) = 0;
	virtual _bool	IsRenderAlphaGeometryEnabled( ) const = 0;

	virtual _void	EnableViewFrustumClip( _bool enable ) = 0;
	virtual _bool	IsViewFrustumClipEnabled( ) const = 0;

	virtual _void	EnableOverlayPixelAlign( _bool enable ) = 0;
	virtual _bool	IsOverlayPixelAlignEnabled( ) const = 0;

	virtual	_void	EnableAlphaRefAttenuation( _bool enable ) = 0;
	virtual	_bool	IsAlphaRefAttenuationEnabled( ) const = 0;

	virtual	_void	SetMinAlphaRefAttenuationValue( _byte value ) = 0;
	virtual	_byte	GetMinAlphaRefAttenuationValue( ) const = 0;

	virtual	_void	SetMinAlphaRefAttenuationDistance( _float value ) = 0;
	virtual	_float	GetMinAlphaRefAttenuationDistance( ) const = 0;

	virtual	_void	SetMaxAlphaRefAttenuationDistance( _float distance ) = 0;
	virtual	_float	GetMaxAlphaRefAttenuationDistance( ) const = 0;
};

};