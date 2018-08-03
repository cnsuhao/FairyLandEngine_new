//============================================================================
// TechniqueFactory.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// TechniqueFactory Implementation
//----------------------------------------------------------------------------

TechniqueFactory::TechniqueFactory( _dword vsver, _dword psver )
{
	mVertexShaderVersion			= vsver;
	mPixelShaderVersion				= psver;

	mNormalRenderMethod				= _null;
	mGrayRenderMethod				= _null;
	mDecalRenderMethod				= _null;
	mAlphaRenderMethod				= _null;
	mColorkeyRenderMethod			= _null;
	mAdditiveRenderMethod			= _null;
	mSubtractiveRenderMethod		= _null;

	mOverlayRenderMethod			= _null;
	mOverlayBlendRenderMethod		= _null;

	mPrimitiveRenderMethod			= _null;
	mPrimitiveBlendRenderMethod		= _null;

	mBillboardRenderMethod			= _null;
	mBillboardBlendRenderMethod		= _null;

	mProjectRenderMethod			= _null;
	mOnlyWriteDepthRenderMethod		= _null;
	mDepthAsColorRenderMethod		= _null;

	mTextureFactorMethod			= _null;
	mVertexColorMethod				= _null;
	mSilhouetteTextureMethod		= _null;

	for ( _dword i = 0; i <= Geometry::_MAX_TEXTURE_NUMBER; i ++ )
	{
		mWrapSamplerMethod[i]		= _null;
		mClampSamplerMethod[i]		= _null;

		mBlendTextureMethod[i]		= _null;
		mHighlightTextureMethod[i]	= _null;
		mGrayTextureMethod[i]		= _null;
	}
}

TechniqueFactory::~TechniqueFactory( )
{
	delete mNormalRenderMethod;
	delete mGrayRenderMethod;
	delete mDecalRenderMethod;
	delete mAlphaRenderMethod;
	delete mColorkeyRenderMethod;
	delete mAdditiveRenderMethod;
	delete mSubtractiveRenderMethod;

	delete mOverlayRenderMethod;
	delete mOverlayBlendRenderMethod;

	delete mPrimitiveRenderMethod;
	delete mPrimitiveBlendRenderMethod;

	delete mBillboardRenderMethod;
	delete mBillboardBlendRenderMethod;

	delete mProjectRenderMethod;
	delete mOnlyWriteDepthRenderMethod;
	delete mDepthAsColorRenderMethod;

	delete mTextureFactorMethod;
	delete mVertexColorMethod;
	delete mSilhouetteTextureMethod;

	for ( _dword i = 0; i <= Geometry::_MAX_TEXTURE_NUMBER; i ++ )
	{
		delete mWrapSamplerMethod[i];
		delete mClampSamplerMethod[i];

		delete mBlendTextureMethod[i];
		delete mHighlightTextureMethod[i];
	}
}

_dword TechniqueFactory::GetDefaultRenderState( _dword state )
{
	static const _float cFloatZero	= 0.0f;
	static const _float cFloatOne	= 1.0f;
	static const _dword cDwordZero	= *( (_dword*) &cFloatZero );
	static const _dword cDwordOne	= *( (_dword*) &cFloatOne );

	switch ( state )
	{
		case D3DRS_FILLMODE:				return IRenderMethod::_FILLMODE_SOLID;
		case D3DRS_CULLMODE:				return IRenderMethod::_CULLMODE_CCW;
		case D3DRS_SHADEMODE:				return IRenderMethod::_SHADEMODE_SMOOTH;
		case D3DRS_CLIPPING:				return _true;
		case D3DRS_NORMALIZENORMALS:		return _false;
		case D3DRS_LIGHTING:				return _false;
		case D3DRS_SPECULARENABLE:			return _false;
		case D3DRS_AMBIENT:					return 0x00000000;
		case D3DRS_ZWRITEENABLE:			return _true;
		case D3DRS_ZENABLE:					return _true;
		case D3DRS_ZFUNC:					return IRenderMethod::_COMPAREFUNC_LESSEQUAL;
		case D3DRS_STENCILENABLE:			return _false;
		case D3DRS_STENCILFUNC:				return IRenderMethod::_COMPAREFUNC_ALWAYS;
		case D3DRS_STENCILREF:				return 0x00000000;
		case D3DRS_STENCILMASK:				return 0xffffffff;
		case D3DRS_STENCILWRITEMASK:		return 0xffffffff;
		case D3DRS_STENCILPASS:				return IRenderMethod::_STENCILOP_KEEP;
		case D3DRS_STENCILFAIL:				return IRenderMethod::_STENCILOP_KEEP;
		case D3DRS_STENCILZFAIL:			return IRenderMethod::_STENCILOP_KEEP;
		case D3DRS_ALPHATESTENABLE:			return _false;
		case D3DRS_ALPHAFUNC:				return IRenderMethod::_COMPAREFUNC_ALWAYS;
		case D3DRS_ALPHAREF:				return 0x00000000;
		case D3DRS_ALPHABLENDENABLE:		return _false;
		case D3DRS_BLENDOP:					return IRenderMethod::_BLENDOP_ADD;
		case D3DRS_BLENDFACTOR:				return 0xffffffff;
		case D3DRS_SRCBLEND:				return IRenderMethod::_BLENDFACTOR_ONE;
		case D3DRS_DESTBLEND:				return IRenderMethod::_BLENDFACTOR_ZERO;
		case D3DRS_FOGENABLE:				return _false;
		case D3DRS_RANGEFOGENABLE:			return IRenderMethod::_FOGMODE_DEPTHBASED;
		case D3DRS_FOGVERTEXMODE:			return IRenderMethod::_FOGFORMULA_NONE;
		case D3DRS_FOGTABLEMODE:			return IRenderMethod::_FOGFORMULA_NONE;
		case D3DRS_FOGCOLOR:				return 0x00000000;
		case D3DRS_FOGSTART:				return cDwordZero;
		case D3DRS_FOGEND:					return cDwordOne;
		case D3DRS_FOGDENSITY:				return cDwordOne;
		case D3DRS_MULTISAMPLEANTIALIAS:	return _true;
		case D3DRS_MULTISAMPLEMASK:			return 0xffffffff;
		case D3DRS_COLORWRITEENABLE:		return 0x0000000f;
	}

	return 0;
}

_dword TechniqueFactory::GetDefaultSamplerState( _dword stage, _dword state )
{
	switch ( state )
	{
		case D3DSAMP_ADDRESSU:				return ISamplerMethod::_TEXTUREADDR_WRAP;
		case D3DSAMP_ADDRESSV:				return ISamplerMethod::_TEXTUREADDR_WRAP;
		case D3DSAMP_ADDRESSW:				return ISamplerMethod::_TEXTUREADDR_WRAP;
		case D3DSAMP_BORDERCOLOR:			return 0x00000000;
		case D3DSAMP_MAGFILTER:				return ISamplerMethod::_TEXTUREFILTER_LINEAR;
		case D3DSAMP_MINFILTER:				return ISamplerMethod::_TEXTUREFILTER_LINEAR;
		case D3DSAMP_MIPFILTER:				return ISamplerMethod::_TEXTUREFILTER_LINEAR;
		case D3DSAMP_MIPMAPLODBIAS:			return 0;
		case D3DSAMP_MAXMIPLEVEL:			return 0;
		case D3DSAMP_MAXANISOTROPY:			return 1;
	}

	return 0;
}

_dword TechniqueFactory::GetDefaultTextureState( _dword stage, _dword state )
{
	static const _float cFloatZero	= 0.0f;
	static const _dword cDwordZero	= *( (_dword*) &cFloatZero );

	switch ( state )
	{
		case D3DTSS_COLOROP:				return stage == 0 ? ITextureMethod::_TEXTUREOP_MODULATE : ITextureMethod::_TEXTUREOP_DISABLE;
		case D3DTSS_COLORARG0:				return ITextureMethod::_TEXTUREVALUE_CURRENT;
		case D3DTSS_COLORARG1:				return ITextureMethod::_TEXTUREVALUE_TEXTURE;
		case D3DTSS_COLORARG2:				return ITextureMethod::_TEXTUREVALUE_CURRENT;
		case D3DTSS_ALPHAOP:				return stage == 0 ? ITextureMethod::_TEXTUREOP_SELECTARG1 : ITextureMethod::_TEXTUREOP_DISABLE;
		case D3DTSS_ALPHAARG0:				return ITextureMethod::_TEXTUREVALUE_CURRENT;
		case D3DTSS_ALPHAARG1:				return ITextureMethod::_TEXTUREVALUE_TEXTURE;
		case D3DTSS_ALPHAARG2:				return ITextureMethod::_TEXTUREVALUE_CURRENT;
		case D3DTSS_TEXCOORDINDEX:			return stage;
		case D3DTSS_TEXTURETRANSFORMFLAGS:	return ITextureMethod::_TEXTURETRANSFORM_DISABLE;
		case D3DTSS_RESULTARG:				return ITextureMethod::_TEXTUREVALUE_CURRENT;
		case D3DTSS_CONSTANT:				return 0x00000000;
		case D3DTSS_BUMPENVLSCALE:			return cDwordZero;
		case D3DTSS_BUMPENVLOFFSET:			return cDwordZero;
		case D3DTSS_BUMPENVMAT00:			return cDwordZero;
		case D3DTSS_BUMPENVMAT01:			return cDwordZero;
		case D3DTSS_BUMPENVMAT10:			return cDwordZero;
		case D3DTSS_BUMPENVMAT11:			return cDwordZero;
	}

	return 0;
}

_dword TechniqueFactory::GetVertexShaderVersion( ) const
{
	return mVertexShaderVersion;
}

_dword TechniqueFactory::GetPixelShaderVersion( ) const
{
	return mPixelShaderVersion;
}

IRenderSet* TechniqueFactory::CreateRenderSet( )
{
	RenderSet* renderset = new RenderSet( );

	return renderset;
}

IRenderSet* TechniqueFactory::CloneRenderSet( IRenderSet* renderset )
{
	if ( renderset == _null )
		return _null;

	// Increase reference count.
	( (RenderSet*) renderset )->IncreaseRefCount( );

	return renderset;
}

_void TechniqueFactory::ReleaseRenderSet( IRenderSet* renderset )
{
	if ( renderset == _null )
		return;

	// Decrease reference count.
	if ( ( (RenderSet*) renderset )->DecreaseRefCount( ) == 0 )
		delete (RenderSet*) renderset;
}

IShaderConst* TechniqueFactory::CreateShaderConst( )
{
	ShaderConst* shaderconst = new ShaderConst( );

	return shaderconst;
}

IShaderConst* TechniqueFactory::CloneShaderConst( IShaderConst* shaderconst, _bool reference )
{
	if ( shaderconst == _null )
		return _null;

	// Increase reference count.
	( (ShaderConst*) shaderconst )->IncreaseRefCount( );

	return shaderconst;
}

_void TechniqueFactory::ReleaseShaderConst( IShaderConst*& shaderconst )
{
	if ( shaderconst == _null )
		return;

	// Decrease reference count.
	if ( ( (ShaderConst*) shaderconst )->DecreaseRefCount( ) == 0 )
		delete (ShaderConst*) shaderconst;

	shaderconst = _null;
}

IRenderMethod* TechniqueFactory::CreateRenderMethod( )
{
	RenderMethod* method = new RenderMethod( _false, IRenderMethod::_RENDERORDER_NORMAL );

	return method;
}

IRenderMethod* TechniqueFactory::CreateRenderMethod( StringPtr methodstring )
{
	RenderMethod* method = new RenderMethod( _false, IRenderMethod::_RENDERORDER_NORMAL );

	// Set method by string.
	method->SetMethodString( methodstring );

	return method;
}

IRenderMethod* TechniqueFactory::GetDefaultRenderMethod( const Renderable& renderable, _bool alpha )
{
	// Geometry.
	if ( renderable.mType == Renderable::_TYPE_GEOMETRY )
	{
		const Geometry& geometry = (const Geometry&) renderable;

		if ( geometry.mRenderFlag & Geometry::_RENDERFLAG_DECAL )
		{
			// Use decal render method.
			return GetDefaultDecalRenderMethod( );
		}
		else if ( geometry.mRenderFlag & Geometry::_RENDERFLAG_ADDITIVE )
		{
			// Use additive render method.
			return GetDefaultAdditiveRenderMethod( );
		}
		else if ( geometry.mRenderFlag & Geometry::_RENDERFLAG_SUBTRACTIVE )
		{
			// Use subtractive render method.
			return GetDefaultSubtractiveRenderMethod( );
		}
		else if ( geometry.mRenderFlag & Geometry::_RENDERFLAG_ALPHA ) 
		{
			// Use alpha render method.
			return GetDefaultAlphaRenderMethod( );
		}
		else if ( geometry.mRenderFlag & Geometry::_RENDERFLAG_COLORKEY )
		{
			// Use colorkey render method.
			return GetDefaultColorkeyRenderMethod( );
		}
		else
		{
			return alpha ? GetDefaultAlphaRenderMethod( ) : GetDefaultNormalRenderMethod( );
		}
	}
	// Overlay.
	else if ( renderable.mType >= Renderable::_TYPE_OVERLAY_BEGIN &&
			  renderable.mType <= Renderable::_TYPE_OVERLAY_END )
	{
		// Use overlay render method.
		return alpha ? GetDefaultOverlayBlendRenderMethod( ) : GetDefaultOverlayRenderMethod( );
	}
	// Primitive.
	else if ( renderable.mType >= Renderable::_TYPE_PRIMITIVE_BEGIN &&
			  renderable.mType <= Renderable::_TYPE_PRIMITIVE_END )
	{
		// Use primitive render method.
		return alpha ? GetDefaultPrimitiveBlendRenderMethod( ) : GetDefaultPrimitiveRenderMethod( );
	}
	// Billboard.
	else if ( renderable.mType >= Renderable::_TYPE_BILLBOARD_BEGIN &&
			  renderable.mType <= Renderable::_TYPE_BILLBOARD_END )
	{
		// Use billboard render method.
		return alpha ? GetDefaultBillboardBlendRenderMethod( ) : GetDefaultBillboardRenderMethod( );
	}

	return GetDefaultNormalRenderMethod( );
}

IRenderMethod* TechniqueFactory::GetDefaultNormalRenderMethod( )
{
	if ( mNormalRenderMethod != _null )
		return mNormalRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_NORMAL );

	return mNormalRenderMethod = method;
}

IRenderMethod* TechniqueFactory::GetDefaultDecalRenderMethod( )
{
	if ( mDecalRenderMethod != _null )
		return mDecalRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_DECAL );

	// Disable depth write.
	method->EnableDepthWrite( _false );

	// Enable alpha blend.
	method->EnableAlphaBlend( _true );

	// Set source blend factor as source alpha.
	method->SetAlphaBlendSrcFactor( IRenderMethod::_BLENDFACTOR_SRCALPHA );

	// Set destination blend factor as inverse of source alpha.
	method->SetAlphaBlendDesFactor( IRenderMethod::_BLENDFACTOR_INVSRCALPHA );

	return mDecalRenderMethod = method;
}

IRenderMethod* TechniqueFactory::GetDefaultAlphaRenderMethod( )
{
	if ( mAlphaRenderMethod != _null )
		return mAlphaRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_ALPHA );

	// Enable alpha blend.
	method->EnableAlphaBlend( _true );

	// Set source blend factor as source alpha.
	method->SetAlphaBlendSrcFactor( IRenderMethod::_BLENDFACTOR_SRCALPHA );

	// Set destination blend factor as inverse of source alpha.
	method->SetAlphaBlendDesFactor( IRenderMethod::_BLENDFACTOR_INVSRCALPHA );

	// Enable alpha test.
	method->EnableAlphaTest( _true );

	// Enable alpha test function as greater or equal.
	method->SetAlphaTestFunction( IRenderMethod::_COMPAREFUNC_GREATEREQUAL );

	// Set alpha test reference as 1, means all none zero alpha will pass.
	method->SetAlphaTestReference( 1 );

	return mAlphaRenderMethod = method;
}

IRenderMethod* TechniqueFactory::GetDefaultColorkeyRenderMethod( )
{
	if ( mColorkeyRenderMethod != _null )
		return mColorkeyRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_COLORKEY );

	// Enable alpha blend.
	method->EnableAlphaBlend( _true );

	// Set source blend factor as source alpha.
	method->SetAlphaBlendSrcFactor( IRenderMethod::_BLENDFACTOR_SRCALPHA );

	// Set destination blend factor as inverse of source alpha.
	method->SetAlphaBlendDesFactor( IRenderMethod::_BLENDFACTOR_INVSRCALPHA );

	// Enable alpha test.
	method->EnableAlphaTest( _true );

	// Enable alpha test function as greater.
	method->SetAlphaTestFunction( IRenderMethod::_COMPAREFUNC_GREATER );

	// Set alpha test reference as 225, means all alpha greater than 225 will pass.
	//method->SetAlphaTestReference( 225 );

	return mColorkeyRenderMethod = method;
}

IRenderMethod* TechniqueFactory::GetDefaultAdditiveRenderMethod( )
{
	if ( mAdditiveRenderMethod != _null )
		return mAdditiveRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_ALPHA );

	// Enable alpha blend.
	method->EnableAlphaBlend( _true );

	// Set source blend factor as source alpha.
	method->SetAlphaBlendSrcFactor( IRenderMethod::_BLENDFACTOR_SRCALPHA );

	// Set destination blend factor as one.
	method->SetAlphaBlendDesFactor( IRenderMethod::_BLENDFACTOR_ONE );

	// Enable alpha test.
	method->EnableAlphaTest( _true );

	// Enable alpha test function as greater or equal.
	method->SetAlphaTestFunction( IRenderMethod::_COMPAREFUNC_GREATEREQUAL );

	// Set alpha test reference as 1, means all none zero alpha will pass.
	method->SetAlphaTestReference( 1 );

	return mAdditiveRenderMethod = method;
}

IRenderMethod* TechniqueFactory::GetDefaultSubtractiveRenderMethod( )
{
	if ( mSubtractiveRenderMethod != _null )
		return mSubtractiveRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_ALPHA );

	// Enable alpha blend.
	method->EnableAlphaBlend( _true );

	// Set source blend factor as source alpha.
	method->SetAlphaBlendSrcFactor( IRenderMethod::_BLENDFACTOR_SRCALPHA );

	// Set destination blend factor as one.
	method->SetAlphaBlendDesFactor( IRenderMethod::_BLENDFACTOR_ONE );

	// Set blend operation as destination subtract source.
	method->SetAlphaBlendOperation( IRenderMethod::_BLENDOP_REVSUBTRACT );

	// Enable alpha test.
	method->EnableAlphaTest( _true );

	// Enable alpha test function as greater or equal.
	method->SetAlphaTestFunction( IRenderMethod::_COMPAREFUNC_GREATEREQUAL );

	// Set alpha test reference as 1, means all none zero alpha will pass.
	method->SetAlphaTestReference( 1 );

	return mSubtractiveRenderMethod = method;
}

IRenderMethod* TechniqueFactory::GetDefaultOverlayRenderMethod( )
{
	if ( mOverlayRenderMethod != _null )
		return mOverlayRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_NORMAL );

	// Disable cull.
	method->SetCullMode( IRenderMethod::_CULLMODE_NONE );

	// Disable clip.
	method->EnableClipping( _false );

	// Disable depth test.
	method->EnableDepthTest( _false );

	// Disable FSAA.
	method->EnableMultiSampleAntialias( _false );

	return mOverlayRenderMethod = method;
}

IRenderMethod* TechniqueFactory::GetDefaultOverlayBlendRenderMethod( )
{
	if ( mOverlayBlendRenderMethod != _null )
		return mOverlayBlendRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_ALPHA );

	// Disable cull.
	method->SetCullMode( IRenderMethod::_CULLMODE_NONE );

	// Disable clip.
	method->EnableClipping( _false );

	// Disable depth test.
	method->EnableDepthTest( _false );

	// Disable FSAA.
	method->EnableMultiSampleAntialias( _false );

	// Enable alpha blend.
	method->EnableAlphaBlend( _true );

	// Set source blend factor as source alpha.
	method->SetAlphaBlendSrcFactor( IRenderMethod::_BLENDFACTOR_SRCALPHA );

	// Set destination blend factor as inverse of source alpha.
	method->SetAlphaBlendDesFactor( IRenderMethod::_BLENDFACTOR_INVSRCALPHA );

	// Enable alpha test.
	method->EnableAlphaTest( _true );

	// Enable alpha test function as greater or equal.
	method->SetAlphaTestFunction( IRenderMethod::_COMPAREFUNC_GREATEREQUAL );

	// Set alpha test reference as 1, means all none zero alpha will pass.
	method->SetAlphaTestReference( 1 );

	return mOverlayBlendRenderMethod = method;
}

IRenderMethod* TechniqueFactory::GetDefaultPrimitiveRenderMethod( )
{
	if ( mPrimitiveRenderMethod != _null )
		return mPrimitiveRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_NORMAL );

	// Disalbe back face cull.
	method->SetCullMode( IRenderMethod::_CULLMODE_NONE );

	return mPrimitiveRenderMethod = method;
}

IRenderMethod* TechniqueFactory::GetDefaultPrimitiveBlendRenderMethod( )
{
	if ( mPrimitiveBlendRenderMethod != _null )
		return mPrimitiveBlendRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_ALPHA );

	// Disalbe back face cull.
	method->SetCullMode( IRenderMethod::_CULLMODE_NONE );

	// Disable depth write.
	method->EnableDepthWrite( _false );

	// Enable alpha blend.
	method->EnableAlphaBlend( _true );

	// Set source blend factor as source alpha.
	method->SetAlphaBlendSrcFactor( IRenderMethod::_BLENDFACTOR_SRCALPHA );

	// Set destination blend factor as inverse of source alpha.
	method->SetAlphaBlendDesFactor( IRenderMethod::_BLENDFACTOR_INVSRCALPHA );

	// Enable alpha test.
	method->EnableAlphaTest( _true );

	// Enable alpha test function as greater or equal.
	method->SetAlphaTestFunction( IRenderMethod::_COMPAREFUNC_GREATEREQUAL );

	// Set alpha test reference as 1, means all none zero alpha will pass.
	method->SetAlphaTestReference( 1 );

	return mPrimitiveBlendRenderMethod = method;
}

IRenderMethod* TechniqueFactory::GetDefaultBillboardRenderMethod( )
{
	if ( mBillboardRenderMethod != _null )
		return mBillboardRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_NORMAL );

	return mBillboardRenderMethod = method;
}

IRenderMethod* TechniqueFactory::GetDefaultBillboardBlendRenderMethod( )
{
	if ( mBillboardBlendRenderMethod != _null )
		return mBillboardBlendRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_ALPHA );

	// Disable depth write.
	method->EnableDepthWrite( _false );

	// Enable alpha blend.
	method->EnableAlphaBlend( _true );

	// Set source blend factor as source alpha.
	method->SetAlphaBlendSrcFactor( IRenderMethod::_BLENDFACTOR_SRCALPHA );

	// Set destination blend factor as inverse of source alpha.
	method->SetAlphaBlendDesFactor( IRenderMethod::_BLENDFACTOR_INVSRCALPHA );

	// Enable alpha test.
	method->EnableAlphaTest( _true );

	// Enable alpha test function as greater or equal.
	method->SetAlphaTestFunction( IRenderMethod::_COMPAREFUNC_GREATEREQUAL );

	// Set alpha test reference as 1, means all none zero alpha will pass.
	method->SetAlphaTestReference( 1 );

	return mBillboardBlendRenderMethod = method;
}

IRenderMethod* TechniqueFactory::GetDefaultProjectRenderMethod( )
{
	if ( mProjectRenderMethod != _null )
		return mProjectRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_NORMAL );

	// Enable alpha blend.
	method->EnableAlphaBlend( _true );

	// Set source blend factor as source alpha.
	method->SetAlphaBlendSrcFactor( IRenderMethod::_BLENDFACTOR_SRCALPHA );

	// Set destination blend factor as inverse of source alpha.
	method->SetAlphaBlendDesFactor( IRenderMethod::_BLENDFACTOR_INVSRCALPHA );

	method->SetRenderOrder( IRenderMethod::_RENDERORDER_ALPHA );

	//// Set texture address mode.
	//method->SetSamplerAddressU( 0, IRenderMethod::_TEXTUREADDR_CLAMP );
	//method->SetSamplerAddressV( 0, IRenderMethod::_TEXTUREADDR_CLAMP );

	//// Set texture transform.
	//method->SetTextureTransformFlags( 0, IRenderMethod::_TEXTURETRANSFORM_COUNT3 | IRenderMethod::_TEXTURETRANSFORM_PROJECTED );
	//method->SetTextureCoordinateIndex( 0, IRenderMethod::_TEXCOORDINDEX_CAMERASPACEPOSITION );

	return mProjectRenderMethod = method;
}

IRenderMethod* TechniqueFactory::GetDefaultOnlyWriteDepthRenderMethod( )
{
	if ( mOnlyWriteDepthRenderMethod != _null )
		return mOnlyWriteDepthRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_NORMAL );

	//if ( ( mDeviceCaps.mPrimitiveMiscCaps & D3DPMISCCAPS_COLORWRITEENABLE ) != 0 )
	//{
	//	method->SetColorWriteMask( 0 );
	//}
	//else
	//{
	//	method->EnableAlphaBlend( _true );
	//	method->SetAlphaBlendSrcFactor( IRenderMethod::_BLENDFACTOR_ZERO );
	//	method->SetAlphaBlendDesFactor( IRenderMethod::_BLENDFACTOR_ONE );
	//}

	method->EnableAlphaTest( _true );
	method->SetAlphaTestFunction( IRenderMethod::_COMPAREFUNC_GREATEREQUAL );
	method->SetAlphaTestReference( 1 );

	return mOnlyWriteDepthRenderMethod = method;
}

IRenderMethod* TechniqueFactory::GetDefaultDepthAsColorRenderMethod( )
{
	if ( mDepthAsColorRenderMethod != _null )
		return mDepthAsColorRenderMethod;

	RenderMethod* method = new RenderMethod( _true, IRenderMethod::_RENDERORDER_NORMAL );

	_bool retval = method->SetVertexShaderCode( DefaultShader::s_HLSL_VS_DepthAsColor, L"vs_main", L"vs_1_1" );

	method->SetVertexShaderConstant( 0, IRenderMethod::_PARAMETER_TRANSFORM_WVP );
	method->SetVertexShaderConstant( 4, Vector3( 500.0f, 0.0f, 0.0f ) );

	FL_ASSERT( retval )

	return mDepthAsColorRenderMethod = method;
}

IRenderMethod* TechniqueFactory::CloneRenderMethod( IRenderMethod* method, _bool reference )
{
	if ( method == _null )
		return _null;

	if ( method->IsDefaultMethod( ) )
		return method;

	// Increase reference count.
	( (RenderMethod*) method )->IncreaseRefCount( );

	return method;
}

_void TechniqueFactory::ReleaseRenderMethod( IRenderMethod*& method )
{
	if ( method == _null )
		return;

	if ( method->IsDefaultMethod( ) )
		return;

	// Decrease reference count.
	if ( ( (RenderMethod*) method )->DecreaseRefCount( ) == 0 )
		delete (RenderMethod*) method;

	method = _null;
}

ISamplerMethod* TechniqueFactory::CreateSamplerMethod( )
{
	SamplerMethod* method = new SamplerMethod( _false );

	return method;
}

ISamplerMethod* TechniqueFactory::GetDefaultSamplerMethod( const Renderable& renderable )
{
	// Overlay.
	if ( renderable.mType >= Renderable::_TYPE_OVERLAY_BEGIN &&
		 renderable.mType <= Renderable::_TYPE_OVERLAY_END )
	{
		// Use default blend method.
		return GetDefaultClampSamplerMethod( 1 );
	}

	return _null;
}

ISamplerMethod* TechniqueFactory::GetDefaultWrapSamplerMethod( _dword texnumber )
{
	return _null;
}

ISamplerMethod* TechniqueFactory::GetDefaultClampSamplerMethod( _dword texnumber )
{
	if ( texnumber >= Geometry::_MAX_TEXTURE_NUMBER + 1 )
		return _null;

	if ( mClampSamplerMethod[ texnumber ] != _null )
		return mClampSamplerMethod[ texnumber ];

	SamplerMethod* method = new SamplerMethod( _true );

	for ( _dword i = 0; i < texnumber; i ++ )
	{
		// Set texture address mode.
		method->SetSamplerAddressU( i, ISamplerMethod::_TEXTUREADDR_CLAMP );
		method->SetSamplerAddressV( i, ISamplerMethod::_TEXTUREADDR_CLAMP );

		// Set mipmap filter.
		method->SetSamplerMipFilter( i, ISamplerMethod::_TEXTUREFILTER_NONE );
	}

	return mClampSamplerMethod[ texnumber ] = method;
}

ISamplerMethod* TechniqueFactory::CloneSamplerMethod( ISamplerMethod* method, _bool reference )
{
	if ( method == _null )
		return _null;

	if ( method->IsDefaultMethod( ) )
		return method;

	// Increase reference count.
	( (SamplerMethod*) method )->IncreaseRefCount( );

	return method;
}

_void TechniqueFactory::ReleaseSamplerMethod( ISamplerMethod*& method )
{
	if ( method == _null )
		return;

	if ( method->IsDefaultMethod( ) )
		return;

	// Decrease reference count.
	if ( ( (SamplerMethod*) method )->DecreaseRefCount( ) == 0 )
		delete (SamplerMethod*) method;

	method = _null;
}

ITextureMethod* TechniqueFactory::CreateTextureMethod( )
{
	TextureMethod* method = new TextureMethod( _false );

	return method;
}

ITextureMethod* TechniqueFactory::GetDefaultTextureMethod( const Renderable& renderable, _bool alpha )
{
	// Geometry.
	if ( renderable.mType == Renderable::_TYPE_GEOMETRY )
	{
		const Geometry& geometry = (const Geometry&) renderable;

		// Use default blend method.
		if ( alpha )
			return GetDefaultBlendTextureMethod( geometry.GetTextureNumber( ) );
	}
	// Overlay.
	else if ( renderable.mType >= Renderable::_TYPE_OVERLAY_BEGIN &&
			  renderable.mType <= Renderable::_TYPE_OVERLAY_END )
	{
		// Use default blend method.
		return GetDefaultBlendTextureMethod( 1 );
	}
	// Primitive.
	else if ( renderable.mType >= Renderable::_TYPE_PRIMITIVE_BEGIN &&
			  renderable.mType <= Renderable::_TYPE_PRIMITIVE_END )
	{
		// Use default blend method.
		return GetDefaultBlendTextureMethod( 1 );
	}
	// Billboard.
	else if ( renderable.mType >= Renderable::_TYPE_BILLBOARD_BEGIN &&
			  renderable.mType <= Renderable::_TYPE_BILLBOARD_END )
	{
		// Use default blend method.
		return GetDefaultBlendTextureMethod( 1 );
	}

	return _null;
}

ITextureMethod*	TechniqueFactory::GetDefaultTextureFactorMethod( )
{
	if ( mTextureFactorMethod != _null )
		return mTextureFactorMethod;

	TextureMethod* method = new TextureMethod( _true );

	method->SetTextureColorOperation( 0, ITextureMethod::_TEXTUREOP_SELECTARG1 );
	method->SetTextureColorArgument1( 0, ITextureMethod::_TEXTUREVALUE_TFACTOR );

	method->SetTextureAlphaOperation( 0, ITextureMethod::_TEXTUREOP_SELECTARG1 );
	method->SetTextureAlphaArgument1( 0, ITextureMethod::_TEXTUREVALUE_TFACTOR );

	return mTextureFactorMethod = method;
}

ITextureMethod* TechniqueFactory::GetDefaultVertexColorMethod( )
{
	if ( mVertexColorMethod != _null )
		return mVertexColorMethod;

	TextureMethod* method = new TextureMethod( _true );

	method->SetTextureColorOperation( 0, ITextureMethod::_TEXTUREOP_SELECTARG1 );
	method->SetTextureColorArgument1( 0, ITextureMethod::_TEXTUREVALUE_DIFFUSE );

	method->SetTextureAlphaOperation( 0, ITextureMethod::_TEXTUREOP_SELECTARG1 );
	method->SetTextureAlphaArgument1( 0, ITextureMethod::_TEXTUREVALUE_DIFFUSE );

	return mVertexColorMethod = method;
}

ITextureMethod* TechniqueFactory::GetDefaultSilhouetteTextureMethod( )
{
	if ( mSilhouetteTextureMethod != _null )
		return mSilhouetteTextureMethod;

	TextureMethod* method = new TextureMethod( _true );

	_bool retval = method->SetPixelShaderCode( DefaultShader::s_HLSL_PS_Silhouette, L"ps_main", L"ps_2_0" );

	method->SetPixelShaderConstant( 0, Color::cBlack );
	method->SetPixelShaderConstant( 1, Vector3( 0.01f * 0.01f, 0.0f, 0.0f ) );

	FL_ASSERT( retval )

	return mSilhouetteTextureMethod = method;
}

ITextureMethod* TechniqueFactory::GetDefaultBlendTextureMethod( _dword texnumber )
{
	if ( texnumber >= Geometry::_MAX_TEXTURE_NUMBER + 1 )
		return _null;

	if ( mBlendTextureMethod[ texnumber ] != _null )
		return mBlendTextureMethod[ texnumber ];

	TextureMethod* method = new TextureMethod( _true );

	for ( _dword i = 0; i < texnumber; i ++ )
	{
		// Set texture color stage.
		method->SetTextureColorOperation( i, ITextureMethod::_TEXTUREOP_MODULATE );
		method->SetTextureColorArgument1( i, ITextureMethod::_TEXTUREVALUE_TEXTURE );
		method->SetTextureColorArgument2( i, ITextureMethod::_TEXTUREVALUE_CURRENT );

		// Set texture alpha stage.
		method->SetTextureAlphaOperation( i, ITextureMethod::_TEXTUREOP_MODULATE );
		method->SetTextureAlphaArgument1( i, ITextureMethod::_TEXTUREVALUE_TEXTURE );
		method->SetTextureAlphaArgument2( i, ITextureMethod::_TEXTUREVALUE_CURRENT );
	}

	method->SetTextureColorOperation( texnumber, ITextureMethod::_TEXTUREOP_MODULATE );
	method->SetTextureColorArgument1( texnumber, ITextureMethod::_TEXTUREVALUE_TFACTOR );
	method->SetTextureColorArgument2( texnumber, ITextureMethod::_TEXTUREVALUE_CURRENT );

	method->SetTextureAlphaOperation( texnumber, ITextureMethod::_TEXTUREOP_MODULATE );
	method->SetTextureAlphaArgument1( texnumber, ITextureMethod::_TEXTUREVALUE_TFACTOR );
	method->SetTextureAlphaArgument2( texnumber, ITextureMethod::_TEXTUREVALUE_CURRENT );

	return mBlendTextureMethod[ texnumber ] = method;
}

ITextureMethod* TechniqueFactory::GetDefaultHighlightTextureMethod( _dword texnumber )
{
	if ( texnumber >= Geometry::_MAX_TEXTURE_NUMBER + 1 )
		return _null;

	if ( mHighlightTextureMethod[ texnumber ] != _null )
		return mHighlightTextureMethod[ texnumber ];

	TextureMethod* method = new TextureMethod( _true );

	for ( _dword i = 0; i < texnumber; i ++ )
	{
		// Set texture color stage.
		method->SetTextureColorOperation( i, ITextureMethod::_TEXTUREOP_MODULATE );
		method->SetTextureColorArgument1( i, ITextureMethod::_TEXTUREVALUE_TEXTURE );
		method->SetTextureColorArgument2( i, ITextureMethod::_TEXTUREVALUE_CURRENT );

		// Set texture alpha stage.
		method->SetTextureAlphaOperation( i, ITextureMethod::_TEXTUREOP_MODULATE );
		method->SetTextureAlphaArgument1( i, ITextureMethod::_TEXTUREVALUE_TEXTURE );
		method->SetTextureAlphaArgument2( i, ITextureMethod::_TEXTUREVALUE_CURRENT );
	}

	method->SetTextureColorOperation( texnumber, ITextureMethod::_TEXTUREOP_ADD );
	method->SetTextureColorArgument1( texnumber, ITextureMethod::_TEXTUREVALUE_TFACTOR );
	method->SetTextureColorArgument2( texnumber, ITextureMethod::_TEXTUREVALUE_CURRENT );

	method->SetTextureAlphaOperation( texnumber, ITextureMethod::_TEXTUREOP_MODULATE );
	method->SetTextureAlphaArgument1( texnumber, ITextureMethod::_TEXTUREVALUE_TFACTOR );
	method->SetTextureAlphaArgument2( texnumber, ITextureMethod::_TEXTUREVALUE_CURRENT );

	return mHighlightTextureMethod[ texnumber ] = method;
}

ITextureMethod* TechniqueFactory::GetDefaultGrayTextureMethod( _dword texnumber )
{
	if ( texnumber >= Geometry::_MAX_TEXTURE_NUMBER + 1 )
		return _null;

	if ( mGrayTextureMethod[ texnumber ] != _null )
		return mGrayTextureMethod[ texnumber ];

	TextureMethod* method = new TextureMethod( _true );

	method->SetPixelShaderConstant( 0, Color( 0.3f, 0.59f, 0.11f ) );
	method->SetPixelShaderConstant( 1, Color::cWhite );

	_bool retval = method->SetPixelShaderCode( DefaultShader::s_ASM_PS_Gray );

	FL_ASSERT( retval )

	return mGrayTextureMethod[ texnumber ] = method;
}

ITextureMethod* TechniqueFactory::CloneTextureMethod( ITextureMethod* method, _bool reference )
{
	if ( method == _null )
		return _null;

	if ( method->IsDefaultMethod( ) )
		return method;

	// Increase reference count.
	( (TextureMethod*) method )->IncreaseRefCount( );

	return method;
}

_void TechniqueFactory::ReleaseTextureMethod( ITextureMethod*& method )
{
	if ( method == _null )
		return;

	if ( method->IsDefaultMethod( ) )
		return;

	// Decrease reference count.
	if ( ( (TextureMethod*) method )->DecreaseRefCount( ) == 0 )
		delete (TextureMethod*) method;

	method = _null;
}