//============================================================================
// RenderConfig.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// RenderConfig Implementation
//----------------------------------------------------------------------------

RenderConfig::RenderConfig( )
{
	mEnableRenderInvisibleGeometry	= _false;
	mEnableRenderNormalGeometry		= _true;
	mEnableRenderAlphaGeometry		= _true;

	mEnableViewFrustumClip			= _true;
	mEnableOverlayPixelAlign		= _true;

	mEnableAlphaRefAttenuation		= _false;
	mMinAlphaRefAttenuationValue	= 100;
	mMinAlphaRefAttenuationDistance	= 100.0f;
	mMaxAlphaRefAttenuationDistance	= 500.0f;
}

RenderConfig::~RenderConfig( )
{
}

_bool RenderConfig::TestMultiSampleType( _dword multisampletype )
{
	return _false;
}

_bool RenderConfig::SetMultiSampleType( _dword multisampletype )
{
	return _false;
}

_dword RenderConfig::GetMultiSampleType( ) const
{
	return 0;
}

_void RenderConfig::EnableVSync( _bool enable )
{
}

_bool RenderConfig::IsVSyncEnabled( ) const
{
	return _true;
}

_void RenderConfig::EnableAnisotropicSampler( _bool enable )
{
}

_bool RenderConfig::IsAnisotropicSamplerEnabled( ) const
{
	return _false;
}

_void RenderConfig::EnableRenderInvisibleGeometry( _bool enable )
{
	mEnableRenderInvisibleGeometry = enable;
}

_bool RenderConfig::IsRenderInvisibleGeometryEnabled( ) const
{
	return mEnableRenderInvisibleGeometry;
}

_void RenderConfig::EnableRenderNormalGeometry( _bool enable )
{
	mEnableRenderNormalGeometry = enable;
}

_bool RenderConfig::IsRenderNormalGeometryEnabled( ) const
{
	return mEnableRenderNormalGeometry;
}

_void RenderConfig::EnableRenderAlphaGeometry( _bool enable )
{
	mEnableRenderAlphaGeometry = enable;
}

_bool RenderConfig::IsRenderAlphaGeometryEnabled( ) const
{
	return mEnableRenderAlphaGeometry;
}

_void RenderConfig::EnableViewFrustumClip( _bool enable )
{
	mEnableViewFrustumClip = enable;
}

_bool RenderConfig::IsViewFrustumClipEnabled( ) const
{
	return mEnableViewFrustumClip;
}

_void RenderConfig::EnableOverlayPixelAlign( _bool enable )
{
	mEnableOverlayPixelAlign = enable;
}

_bool RenderConfig::IsOverlayPixelAlignEnabled( ) const
{
	return mEnableOverlayPixelAlign;
}

_void RenderConfig::EnableAlphaRefAttenuation( _bool enable )
{
	mEnableAlphaRefAttenuation = enable;
}

_bool RenderConfig::IsAlphaRefAttenuationEnabled( ) const
{
	return mEnableAlphaRefAttenuation;
}	

_void RenderConfig::SetMinAlphaRefAttenuationValue( _byte value )
{
	mMinAlphaRefAttenuationValue = (_dword) value;		
}

_byte RenderConfig::GetMinAlphaRefAttenuationValue( ) const
{
	return (_byte) mMinAlphaRefAttenuationValue;
}

_void RenderConfig::SetMinAlphaRefAttenuationDistance( _float distance )
{
	mMinAlphaRefAttenuationDistance	= distance;
	mMinAlphaRefAttenuationDistance	= Math::Max( 0.0f, mMinAlphaRefAttenuationDistance );
}

_float RenderConfig::GetMinAlphaRefAttenuationDistance( ) const
{
	return mMinAlphaRefAttenuationDistance;
}

_void RenderConfig::SetMaxAlphaRefAttenuationDistance( _float distance )
{
	mMaxAlphaRefAttenuationDistance	= distance;
	mMaxAlphaRefAttenuationDistance	= Math::Max( 0.0f, mMaxAlphaRefAttenuationDistance );
}

_float RenderConfig::GetMaxAlphaRefAttenuationDistance( ) const
{
	return mMaxAlphaRefAttenuationDistance;
}