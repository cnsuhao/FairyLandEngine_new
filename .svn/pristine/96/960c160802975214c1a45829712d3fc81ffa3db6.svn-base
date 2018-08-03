//============================================================================
// TechniqueUnit.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// TechniqueUnit Implementation
//----------------------------------------------------------------------------

_void TechniqueUnit::Clear( )
{
	GetTechniqueFactory( ).ReleaseRenderMethod( mRenderMethod );
	GetTechniqueFactory( ).ReleaseSamplerMethod( mSamplerMethod );
	GetTechniqueFactory( ).ReleaseTextureMethod( mTextureMethod );
	GetTechniqueFactory( ).ReleaseRenderMethod( mGlobalRenderMethod );
	GetTechniqueFactory( ).ReleaseSamplerMethod( mGlobalSamplerMethod );
	GetTechniqueFactory( ).ReleaseTextureMethod( mGlobalTextureMethod );
	GetTechniqueFactory( ).ReleaseShaderConst( mShaderConst );
}

_void TechniqueUnit::Clone( const TechniqueUnit& technique )
{
	SetRenderMethod( technique.mRenderMethod );
	SetSamplerMethod( technique.mSamplerMethod );
	SetTextureMethod( technique.mTextureMethod );
	SetGlobalRenderMethod( technique.mGlobalRenderMethod );
	SetGlobalSamplerMethod( technique.mGlobalSamplerMethod );
	SetGlobalTextureMethod( technique.mGlobalTextureMethod );
	SetShaderConst( technique.mShaderConst );
}

_void TechniqueUnit::SetRenderMethod( IRenderMethod* method )
{
	GetTechniqueFactory( ).ReleaseRenderMethod( mRenderMethod );
	mRenderMethod = GetTechniqueFactory( ).CloneRenderMethod( method );
}

_void TechniqueUnit::SetSamplerMethod( ISamplerMethod* method )
{
	GetTechniqueFactory( ).ReleaseSamplerMethod( mSamplerMethod );
	mSamplerMethod = GetTechniqueFactory( ).CloneSamplerMethod( method );
}

_void TechniqueUnit::SetTextureMethod( ITextureMethod* method )
{
	GetTechniqueFactory( ).ReleaseTextureMethod( mTextureMethod );
	mTextureMethod = GetTechniqueFactory( ).CloneTextureMethod( method );
}

_void TechniqueUnit::SetGlobalRenderMethod( IRenderMethod* method )
{
	GetTechniqueFactory( ).ReleaseRenderMethod( mGlobalRenderMethod );
	mGlobalRenderMethod = GetTechniqueFactory( ).CloneRenderMethod( method );
}

_void TechniqueUnit::SetGlobalSamplerMethod( ISamplerMethod* method )
{
	GetTechniqueFactory( ).ReleaseSamplerMethod( mGlobalSamplerMethod );
	mGlobalSamplerMethod = GetTechniqueFactory( ).CloneSamplerMethod( method );
}

_void TechniqueUnit::SetGlobalTextureMethod( ITextureMethod* method )
{
	GetTechniqueFactory( ).ReleaseTextureMethod( mGlobalTextureMethod );
	mGlobalTextureMethod = GetTechniqueFactory( ).CloneTextureMethod( method );
}

_void TechniqueUnit::SetShaderConst( IShaderConst* shaderconst )
{
	GetTechniqueFactory( ).ReleaseShaderConst( mShaderConst );
	mShaderConst = GetTechniqueFactory( ).CloneShaderConst( shaderconst );
}

_dword TechniqueUnit::GetRenderOrder( ) const
{
	return mRenderMethod != _null ? mRenderMethod->GetRenderOrder( ) : 0;
}

_dword TechniqueUnit::GetCullMode( _bool mirror ) const
{
	return mRenderMethod != _null ? mRenderMethod->GetCullMode( mirror ) : ( mirror ? IRenderMethod::_CULLMODE_CW : IRenderMethod::_CULLMODE_CCW );
}

_bool TechniqueUnit::IsLightingEnabled( ) const
{
	return mRenderMethod != _null && mRenderMethod->IsLightingEnabled( ) && mRenderMethod->IsVertexShaderEnabled( ) == _false;
}

_bool TechniqueUnit::IsVertexShaderEnabled( ) const
{
	return mRenderMethod != _null && mRenderMethod->IsVertexShaderEnabled( );
}

_bool TechniqueUnit::IsPixelShaderEnabled( ) const
{
	return mTextureMethod != _null && mTextureMethod->IsPixelShaderEnabled( );
}

_void TechniqueUnit::Enable( TechniqueUnit& oldtechnique ) const
{
	if ( mRenderMethod != oldtechnique.mRenderMethod )
	{
		if ( oldtechnique.mRenderMethod != _null )
			( (RenderMethod*) oldtechnique.mRenderMethod )->DisableMethod( );

		if ( mRenderMethod != _null )
			( (RenderMethod*) mRenderMethod )->EnableMethod( );
	}

	if ( mSamplerMethod != oldtechnique.mSamplerMethod )
	{
		if ( oldtechnique.mSamplerMethod != _null )
			( (SamplerMethod*) oldtechnique.mSamplerMethod )->DisableMethod( );

		if ( mSamplerMethod != _null )
			( (SamplerMethod*) mSamplerMethod )->EnableMethod( );
	}

	if ( mTextureMethod != oldtechnique.mTextureMethod )
	{
		if ( oldtechnique.mTextureMethod != _null )
			( (TextureMethod*) oldtechnique.mTextureMethod )->DisableMethod( );

		if ( mTextureMethod != _null )
			( (TextureMethod*) mTextureMethod )->EnableMethod( );
	}

	if ( mGlobalRenderMethod != oldtechnique.mGlobalRenderMethod )
	{
		if ( oldtechnique.mGlobalRenderMethod != _null )
			( (RenderMethod*) oldtechnique.mGlobalRenderMethod )->DisableMethod( );

		if ( mGlobalRenderMethod != _null )
			( (RenderMethod*) mGlobalRenderMethod )->EnableMethod( );
	}

	if ( mGlobalSamplerMethod != oldtechnique.mGlobalSamplerMethod )
	{
		if ( oldtechnique.mGlobalSamplerMethod != _null )
			( (SamplerMethod*) oldtechnique.mGlobalSamplerMethod )->DisableMethod( );

		if ( mGlobalSamplerMethod != _null )
			( (SamplerMethod*) mGlobalSamplerMethod )->EnableMethod( );
	}

	if ( mGlobalTextureMethod != oldtechnique.mGlobalTextureMethod )
	{
		if ( oldtechnique.mGlobalTextureMethod != _null )
			( (TextureMethod*) oldtechnique.mGlobalTextureMethod )->DisableMethod( );

		if ( mGlobalTextureMethod != _null )
			( (TextureMethod*) mGlobalTextureMethod )->EnableMethod( );
	}

	if ( IsVertexShaderEnabled( ) )
	{
		IShaderConst* vsconst = mShaderConst;
		if ( vsconst == _null )
			vsconst = mRenderMethod->GetVertexShaderConstant( );

		if ( vsconst != _null )
			( (ShaderConst*) vsconst )->EnableVSConst( );
	}

	if ( IsPixelShaderEnabled( ) )
	{
		IShaderConst* psconst = mShaderConst;
		if ( psconst == _null )
			psconst = mTextureMethod->GetPixelShaderConstant( );

		if ( psconst != _null )
			( (ShaderConst*) psconst )->EnablePSConst( );
	}

	//// Set shader const.
	//if ( mShaderConst != oldtechnique.mShaderConst )
	//{
	//	if ( IsVertexShaderEnabled( ) )
	//	{
	//		if ( oldtechnique.mShaderConst != _null )
	//			( (ShaderConst*) oldtechnique.mShaderConst )->DisableVSConst( );

	//		if ( mShaderConst != _null )
	//			( (ShaderConst*) mShaderConst )->EnableVSConst( );
	//	}

	//	if ( IsPixelShaderEnabled( ) )
	//	{
	//		if ( oldtechnique.mShaderConst != _null )
	//			( (ShaderConst*) oldtechnique.mShaderConst )->DisablePSConst( );

	//		if ( mShaderConst != _null )
	//			( (ShaderConst*) mShaderConst )->EnablePSConst( );
	//	}
	//}
}

_void TechniqueUnit::Disable( ) const
{
	if ( mRenderMethod != _null )
	{
		if ( mRenderMethod != _null )
			( (RenderMethod*) mRenderMethod )->DisableMethod( );
	}

	if ( mSamplerMethod != _null )
	{
		if ( mSamplerMethod != _null )
			( (SamplerMethod*) mSamplerMethod )->DisableMethod( );
	}

	if ( mTextureMethod != _null )
	{
		if ( mTextureMethod != _null )
			( (TextureMethod*) mTextureMethod )->DisableMethod( );
	}

	if ( mGlobalRenderMethod != _null )
	{
		if ( mGlobalRenderMethod != _null )
			( (RenderMethod*) mGlobalRenderMethod )->DisableMethod( );
	}

	if ( mGlobalSamplerMethod != _null )
	{
		if ( mGlobalSamplerMethod != _null )
			( (SamplerMethod*) mGlobalSamplerMethod )->DisableMethod( );
	}

	if ( mGlobalTextureMethod != _null )
	{
		if ( mGlobalTextureMethod != _null )
			( (TextureMethod*) mGlobalTextureMethod )->DisableMethod( );
	}

	//if ( mShaderConst != _null )
	//{
	//	if ( IsVertexShaderEnabled( ) )
	//		( (ShaderConst*) mShaderConst )->DisableVSConst( );

	//	if ( IsPixelShaderEnabled( ) )
	//		( (ShaderConst*) mShaderConst )->DisablePSConst( );
	//}
}