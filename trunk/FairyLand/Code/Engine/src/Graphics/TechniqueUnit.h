//============================================================================
// TechniqueUnit.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// TechniqueUnit
//----------------------------------------------------------------------------

struct TechniqueUnit
{
	IRenderMethod*		mRenderMethod;
	ISamplerMethod*		mSamplerMethod;
	ITextureMethod*		mTextureMethod;

	IRenderMethod*		mGlobalRenderMethod;
	ISamplerMethod*		mGlobalSamplerMethod;
	ITextureMethod*		mGlobalTextureMethod;

	IShaderConst*		mShaderConst;

	inline TechniqueUnit( )
	{
		mRenderMethod			= _null;
		mSamplerMethod			= _null;
		mTextureMethod			= _null;

		mGlobalRenderMethod		= _null;
		mGlobalSamplerMethod	= _null;
		mGlobalTextureMethod	= _null;

		mShaderConst			= _null;
	}

	inline _bool operator == ( const TechniqueUnit& technique ) const
	{
		return	mRenderMethod == technique.mRenderMethod &&
				mSamplerMethod == technique.mSamplerMethod &&
				mTextureMethod == technique.mTextureMethod &&
				mGlobalRenderMethod == technique.mGlobalRenderMethod &&
				mGlobalSamplerMethod == technique.mGlobalSamplerMethod &&
				mGlobalTextureMethod == technique.mGlobalTextureMethod &&
				mShaderConst == technique.mShaderConst;
	}

	inline _bool operator != ( const TechniqueUnit& technique ) const
	{
		return	mRenderMethod != technique.mRenderMethod ||
				mSamplerMethod != technique.mSamplerMethod ||
				mTextureMethod != technique.mTextureMethod ||
				mGlobalRenderMethod != technique.mGlobalRenderMethod ||
				mGlobalSamplerMethod != technique.mGlobalSamplerMethod ||
				mGlobalTextureMethod != technique.mGlobalTextureMethod ||
				mShaderConst != technique.mShaderConst;
	}

	_void	Clear( );
	_void	Clone( const TechniqueUnit& technique );

	_void	SetRenderMethod( IRenderMethod* method );
	_void	SetSamplerMethod( ISamplerMethod* method );
	_void	SetTextureMethod( ITextureMethod* method );

	_void	SetGlobalRenderMethod( IRenderMethod* method );
	_void	SetGlobalSamplerMethod( ISamplerMethod* method );
	_void	SetGlobalTextureMethod( ITextureMethod* method );

	_void	SetShaderConst( IShaderConst* shaderconst );

	_dword	GetRenderOrder( ) const;
	_dword	GetCullMode( _bool mirror ) const;
	_bool	IsLightingEnabled( ) const;
	_bool	IsVertexShaderEnabled( ) const;
	_bool	IsPixelShaderEnabled( ) const;

	_void	Enable( TechniqueUnit& oldtechnique ) const;
	_void	Disable( ) const;
};

};