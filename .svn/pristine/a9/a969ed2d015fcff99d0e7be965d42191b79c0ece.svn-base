//============================================================================
// TextureMethod.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// TextureMethod Implementation
//----------------------------------------------------------------------------

TextureMethod::TextureMethod( _bool defaultmethod )
{
	mDefaultMethod	= defaultmethod;

	mPSObject		= _null;
	mPSConst		= _null;

	IncreaseRefCount( );
}

TextureMethod::~TextureMethod( )
{
	GraphicsManager::GetRenderAPIHelper( ).ReleasePixelShader( mPSObject );
	delete mPSConst;
}

_void TextureMethod::SetTextureState( _dword stage, _dword state, _dword value )
{
	// Find the existing state.
	for ( _dword i = 0; i < mTextureStateArray.Number( ); i ++ )
	{
		// Found the same state.
		if ( mTextureStateArray[i].mStage == stage && mTextureStateArray[i].mState == state )
		{
			// It is default value, just remove it.
			if ( value == TechniqueFactory::GetDefaultTextureState( stage, state ) )
			{
				mTextureStateArray.Remove( i );
			}
			// Change state value.
			else
			{
				mTextureStateArray[i].mValue = value;
			}

			return;
		}
	}

	if ( value == TechniqueFactory::GetDefaultTextureState( stage, state ) )
		return;

	// Not found the same state, add a new one.
	mTextureStateArray.Append( TextureState( stage, state, value ) );
}

_void TextureMethod::EnableMethod( ) const
{
	RenderAPI& renderapi = GraphicsManager::GetRenderAPIHelper( );

	if ( mPSObject != _null )
	{
		renderapi.SetPixelShader( mPSObject );

		//if ( mPSConst != _null )
		//	mPSConst->EnablePSConst( );
	}
	else
	{
		for ( _dword i = 0; i < mTextureStateArray.Number( ); i ++ )
		{
			const TextureState& state = mTextureStateArray[i];

			renderapi.EnableTextureState( state.mStage, state.mState, state.mValue );
		}
	}
}

_void TextureMethod::DisableMethod( ) const
{
	RenderAPI& renderapi = GraphicsManager::GetRenderAPIHelper( );

	if ( mPSObject != _null )
	{
		renderapi.SetPixelShader( _null );
	}
	else
	{
		for ( _dword i = 0; i < mTextureStateArray.Number( ); i ++ )
		{
			const TextureState& state = mTextureStateArray[i];

			renderapi.DisableTextureState( state.mStage, state.mState );
		}
	}
}

_bool TextureMethod::IsDefaultMethod( ) const
{
	return mDefaultMethod;
}

_void TextureMethod::SetTextureColorOperation( _dword stage, _dword value )
{
	SetTextureState( stage, D3DTSS_COLOROP, value );
}

_void TextureMethod::SetTextureColorArgument0( _dword stage, _dword value )
{
	SetTextureState( stage, D3DTSS_COLORARG0, value );
}

_void TextureMethod::SetTextureColorArgument1( _dword stage, _dword value )
{
	SetTextureState( stage, D3DTSS_COLORARG1, value );
}

_void TextureMethod::SetTextureColorArgument2( _dword stage, _dword value )
{
	SetTextureState( stage, D3DTSS_COLORARG2, value );
}

_void TextureMethod::SetTextureAlphaOperation( _dword stage, _dword value )
{
	SetTextureState( stage, D3DTSS_ALPHAOP, value );
}

_void TextureMethod::SetTextureAlphaArgument0( _dword stage, _dword value )
{
	SetTextureState( stage, D3DTSS_ALPHAARG0, value );
}

_void TextureMethod::SetTextureAlphaArgument1( _dword stage, _dword value )
{
	SetTextureState( stage, D3DTSS_ALPHAARG1, value );
}

_void TextureMethod::SetTextureAlphaArgument2( _dword stage, _dword value )
{
	SetTextureState( stage, D3DTSS_ALPHAARG2, value );
}

_void TextureMethod::SetTextureCoordinateIndex( _dword stage, _dword value )
{
	SetTextureState( stage, D3DTSS_TEXCOORDINDEX, value );
}

_void TextureMethod::SetTextureTransformFlags( _dword stage, _dword value )
{
	SetTextureState( stage, D3DTSS_TEXTURETRANSFORMFLAGS, value );
}

_void TextureMethod::SetTextureResultArgument( _dword stage, _dword value )
{
	SetTextureState( stage, D3DTSS_RESULTARG, value );
}

_void TextureMethod::SetTextureConstant( _dword stage, _dword value )
{
	SetTextureState( stage, D3DTSS_CONSTANT, value );
}

_void TextureMethod::SetTextureBumpScale( _dword stage, _float value )
{
	SetTextureState( stage, D3DTSS_BUMPENVLSCALE, *( (_dword*) &value ) );
}

_void TextureMethod::SetTextureBumpOffset( _dword stage, _float value )
{
	SetTextureState( stage, D3DTSS_BUMPENVLOFFSET, *( (_dword*) &value ) );
}

_void TextureMethod::SetTextureBumpMatrix( _dword stage, const Matrix2& matrix )
{
	_float m00 = matrix( 0, 0 );
	_float m01 = matrix( 0, 1 );
	_float m10 = matrix( 1, 0 );
	_float m11 = matrix( 1, 1 );

	SetTextureState( stage, D3DTSS_BUMPENVMAT00, *( (_dword*) &m00 ) );
	SetTextureState( stage, D3DTSS_BUMPENVMAT01, *( (_dword*) &m01 ) );
	SetTextureState( stage, D3DTSS_BUMPENVMAT10, *( (_dword*) &m10 ) );
	SetTextureState( stage, D3DTSS_BUMPENVMAT11, *( (_dword*) &m11 ) );
}

_bool TextureMethod::IsPixelShaderEnabled( ) const
{
	return mPSObject != _null;
}

_bool TextureMethod::SetPixelShaderFile( StringPtr filename )
{
	TxtFile shaderfile;

	if ( GetResourceManagerPointer( ) != _null )
	{
		GetResourceManagerPointer( )->LoadResource( filename, shaderfile );
	}
	else
	{
		shaderfile.Open( filename );
	}

	if ( shaderfile.GetLength( ) == 0 )
		return _false;

	String shadercode;
	shaderfile.ReadText( shadercode );

	return SetPixelShaderCode( shadercode );
}

_bool TextureMethod::SetPixelShaderFile( StringPtr filename, StringPtr function, StringPtr profile )
{
	TxtFile shaderfile;

	if ( GetResourceManagerPointer( ) != _null )
	{
		GetResourceManagerPointer( )->LoadResource( filename, shaderfile );
	}
	else
	{
		shaderfile.Open( filename );
	}

	if ( shaderfile.GetLength( ) == 0 )
		return _false;

	String shadercode;
	shaderfile.ReadText( shadercode );

	return SetPixelShaderCode( shadercode, function, profile );
}

_bool TextureMethod::SetPixelShaderCode( StringPtr code )
{
	GraphicsManager::GetRenderAPIHelper( ).ReleasePixelShader( mPSObject );
	mPSObject = GraphicsManager::GetRenderAPIHelper( ).CreatePixelShader( code );

	return mPSObject != _null;
}

_bool TextureMethod::SetPixelShaderCode( StringPtr code, StringPtr function, StringPtr profile )
{
	GraphicsManager::GetRenderAPIHelper( ).ReleasePixelShader( mPSObject );
	mPSObject = GraphicsManager::GetRenderAPIHelper( ).CreatePixelShader( code, function, profile );

	return mPSObject != _null;
}

_void TextureMethod::SetPixelShaderConstant( _dword index, const Vector3& constant )
{
	if ( mPSConst == _null )
		mPSConst = new ShaderConst;

	mPSConst->SetPixelShaderConstant( index, constant );
}

_void TextureMethod::SetPixelShaderConstant( _dword index, const Vector4& constant )
{
	if ( mPSConst == _null )
		mPSConst = new ShaderConst;

	mPSConst->SetPixelShaderConstant( index, constant );
}

_void TextureMethod::SetPixelShaderConstant( _dword index, const Color& constant )
{
	if ( mPSConst == _null )
		mPSConst = new ShaderConst;

	mPSConst->SetPixelShaderConstant( index, constant );
}

IShaderConst* TextureMethod::GetPixelShaderConstant( )
{
	return mPSConst;
}