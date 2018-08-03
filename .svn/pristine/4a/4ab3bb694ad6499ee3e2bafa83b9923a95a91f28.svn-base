//============================================================================
// ITechniqueFactory.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

namespace FL
{

//----------------------------------------------------------------------------
// ITechniqueFactory
//----------------------------------------------------------------------------

class ITechniqueFactory
{
public:
	virtual _dword				GetVertexShaderVersion( ) const = 0;
	virtual _dword				GetPixelShaderVersion( ) const = 0;

	virtual IRenderSet*			CreateRenderSet( ) = 0;
	virtual IRenderSet*			CloneRenderSet( IRenderSet* renderset ) = 0;
	virtual _void				ReleaseRenderSet( IRenderSet* renderset ) = 0;

	virtual IShaderConst*		CreateShaderConst( ) = 0;
	virtual IShaderConst*		CloneShaderConst( IShaderConst* renderparameter, _bool reference = _true ) = 0;
	virtual _void				ReleaseShaderConst( IShaderConst*& renderparameter ) = 0;

	virtual IRenderMethod*		CreateRenderMethod( ) = 0;
	virtual IRenderMethod*		CreateRenderMethod( StringPtr methodstring ) = 0;
	virtual IRenderMethod*		GetDefaultRenderMethod( const Renderable& renderable, _bool alpha ) = 0;
	virtual IRenderMethod*		GetDefaultNormalRenderMethod( ) = 0;
	virtual IRenderMethod*		GetDefaultDecalRenderMethod( ) = 0;
	virtual IRenderMethod*		GetDefaultAlphaRenderMethod( ) = 0;
	virtual IRenderMethod*		GetDefaultColorkeyRenderMethod( ) = 0;
	virtual IRenderMethod*		GetDefaultAdditiveRenderMethod( ) = 0;
	virtual IRenderMethod*		GetDefaultSubtractiveRenderMethod( ) = 0;
	virtual IRenderMethod*		GetDefaultOverlayRenderMethod( ) = 0;
	virtual IRenderMethod*		GetDefaultOverlayBlendRenderMethod( ) = 0;
	virtual IRenderMethod*		GetDefaultPrimitiveRenderMethod( ) = 0;
	virtual IRenderMethod*		GetDefaultPrimitiveBlendRenderMethod( ) = 0;
	virtual IRenderMethod*		GetDefaultBillboardRenderMethod( ) = 0;
	virtual IRenderMethod*		GetDefaultBillboardBlendRenderMethod( ) = 0;
	virtual IRenderMethod*		GetDefaultProjectRenderMethod( ) = 0;
	virtual IRenderMethod*		GetDefaultOnlyWriteDepthRenderMethod( ) = 0;
	virtual IRenderMethod*		GetDefaultDepthAsColorRenderMethod( ) = 0;
	virtual IRenderMethod*		CloneRenderMethod( IRenderMethod* method, _bool reference = _true ) = 0;
	virtual _void				ReleaseRenderMethod( IRenderMethod*& method ) = 0;

	virtual ISamplerMethod*		CreateSamplerMethod( ) = 0;
	virtual ISamplerMethod*		GetDefaultSamplerMethod( const Renderable& renderable ) = 0;
	virtual ISamplerMethod*		GetDefaultWrapSamplerMethod( _dword texnumber ) = 0;
	virtual ISamplerMethod*		GetDefaultClampSamplerMethod( _dword texnumber ) = 0;
	virtual ISamplerMethod*		CloneSamplerMethod( ISamplerMethod* method, _bool reference = _true ) = 0;
	virtual _void				ReleaseSamplerMethod( ISamplerMethod*& method ) = 0;

	virtual ITextureMethod*		CreateTextureMethod( ) = 0;
	virtual ITextureMethod*		GetDefaultTextureMethod( const Renderable& renderable, _bool alpha ) = 0;
	virtual ITextureMethod*		GetDefaultTextureFactorMethod( ) = 0;
	virtual ITextureMethod*		GetDefaultVertexColorMethod( ) = 0;
	virtual ITextureMethod*		GetDefaultSilhouetteTextureMethod( ) = 0;
	virtual ITextureMethod*		GetDefaultBlendTextureMethod( _dword texnumber ) = 0;
	virtual ITextureMethod*		GetDefaultHighlightTextureMethod( _dword texnumber ) = 0;
	virtual ITextureMethod*		GetDefaultGrayTextureMethod( _dword texnumber ) = 0;
	virtual ITextureMethod*		CloneTextureMethod( ITextureMethod* method, _bool reference = _true ) = 0;
	virtual _void				ReleaseTextureMethod( ITextureMethod*& method ) = 0;
};

};