//============================================================================
// TechniqueFactory.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// TechniqueFactory
//----------------------------------------------------------------------------

class TechniqueFactory : public ITechniqueFactory
{
private:
	_dword			mVertexShaderVersion;
	_dword			mPixelShaderVersion;

	RenderMethod*	mNormalRenderMethod;
	RenderMethod*	mGrayRenderMethod;
	RenderMethod*	mDecalRenderMethod;
	RenderMethod*	mAlphaRenderMethod;
	RenderMethod*	mColorkeyRenderMethod;
	RenderMethod*	mAdditiveRenderMethod;
	RenderMethod*	mSubtractiveRenderMethod;

	RenderMethod*	mOverlayRenderMethod;
	RenderMethod*	mOverlayBlendRenderMethod;

	RenderMethod*	mPrimitiveRenderMethod;
	RenderMethod*	mPrimitiveBlendRenderMethod;

	RenderMethod*	mBillboardRenderMethod;
	RenderMethod*	mBillboardBlendRenderMethod;

	RenderMethod*	mProjectRenderMethod;
	RenderMethod*	mOnlyWriteDepthRenderMethod;
	RenderMethod*	mDepthAsColorRenderMethod;

	SamplerMethod*	mWrapSamplerMethod[ Geometry::_MAX_TEXTURE_NUMBER + 1 ];
	SamplerMethod*	mClampSamplerMethod[ Geometry::_MAX_TEXTURE_NUMBER + 1 ];

	TextureMethod*	mTextureFactorMethod;
	TextureMethod*	mVertexColorMethod;
	TextureMethod*	mSilhouetteTextureMethod;
	TextureMethod*	mBlendTextureMethod[ Geometry::_MAX_TEXTURE_NUMBER + 1 ];
	TextureMethod*	mHighlightTextureMethod[ Geometry::_MAX_TEXTURE_NUMBER + 1 ];
	TextureMethod*	mGrayTextureMethod[ Geometry::_MAX_TEXTURE_NUMBER + 1 ];

public:
	TechniqueFactory( _dword vsver, _dword psver );
	~TechniqueFactory( );

	static _dword	GetDefaultRenderState( _dword state );
	static _dword	GetDefaultSamplerState( _dword stage, _dword state );
	static _dword	GetDefaultTextureState( _dword stage, _dword state );

	virtual _dword				GetVertexShaderVersion( ) const;
	virtual _dword				GetPixelShaderVersion( ) const;

	virtual IRenderSet*			CreateRenderSet( );
	virtual IRenderSet*			CloneRenderSet( IRenderSet* renderset );
	virtual _void				ReleaseRenderSet( IRenderSet* renderset );

	virtual IShaderConst*		CreateShaderConst( );
	virtual IShaderConst*		CloneShaderConst( IShaderConst* shaderconst, _bool reference );
	virtual _void				ReleaseShaderConst( IShaderConst*& shaderconst );

	virtual IRenderMethod*		CreateRenderMethod( );
	virtual IRenderMethod*		CreateRenderMethod( StringPtr methodstring );
	virtual IRenderMethod*		GetDefaultRenderMethod( const Renderable& renderable, _bool alpha );
	virtual IRenderMethod*		GetDefaultNormalRenderMethod( );
	virtual IRenderMethod*		GetDefaultDecalRenderMethod( );
	virtual IRenderMethod*		GetDefaultAlphaRenderMethod( );
	virtual IRenderMethod*		GetDefaultColorkeyRenderMethod( );
	virtual IRenderMethod*		GetDefaultAdditiveRenderMethod( );
	virtual IRenderMethod*		GetDefaultSubtractiveRenderMethod( );
	virtual IRenderMethod*		GetDefaultOverlayRenderMethod( );
	virtual IRenderMethod*		GetDefaultOverlayBlendRenderMethod( );
	virtual IRenderMethod*		GetDefaultPrimitiveRenderMethod( );
	virtual IRenderMethod*		GetDefaultPrimitiveBlendRenderMethod( );
	virtual IRenderMethod*		GetDefaultBillboardRenderMethod( );
	virtual IRenderMethod*		GetDefaultBillboardBlendRenderMethod( );
	virtual IRenderMethod*		GetDefaultProjectRenderMethod( );
	virtual IRenderMethod*		GetDefaultOnlyWriteDepthRenderMethod( );
	virtual IRenderMethod*		GetDefaultDepthAsColorRenderMethod( );
	virtual IRenderMethod*		CloneRenderMethod( IRenderMethod* rendermethod, _bool reference );
	virtual _void				ReleaseRenderMethod( IRenderMethod*& rendermethod );

	virtual ISamplerMethod*		CreateSamplerMethod( );
	virtual ISamplerMethod*		GetDefaultSamplerMethod( const Renderable& renderable );
	virtual ISamplerMethod*		GetDefaultWrapSamplerMethod( _dword texnumber );
	virtual ISamplerMethod*		GetDefaultClampSamplerMethod( _dword texnumber );
	virtual ISamplerMethod*		CloneSamplerMethod( ISamplerMethod* method, _bool reference );
	virtual _void				ReleaseSamplerMethod( ISamplerMethod*& method );

	virtual ITextureMethod*		CreateTextureMethod( );
	virtual ITextureMethod*		GetDefaultTextureMethod( const Renderable& renderable, _bool alpha );
	virtual ITextureMethod*		GetDefaultTextureFactorMethod( );
	virtual ITextureMethod*		GetDefaultVertexColorMethod( );
	virtual ITextureMethod*		GetDefaultSilhouetteTextureMethod( );
	virtual ITextureMethod*		GetDefaultBlendTextureMethod( _dword texnumber );
	virtual ITextureMethod*		GetDefaultHighlightTextureMethod( _dword texnumber );
	virtual ITextureMethod*		GetDefaultGrayTextureMethod( _dword texnumber );
	virtual ITextureMethod*		CloneTextureMethod( ITextureMethod* method, _bool reference );
	virtual _void				ReleaseTextureMethod( ITextureMethod*& method );
};

};