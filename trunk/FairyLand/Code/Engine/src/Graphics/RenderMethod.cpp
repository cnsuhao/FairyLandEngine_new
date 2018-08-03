//============================================================================
// RenderMethod.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// RenderMethod Implementation
//----------------------------------------------------------------------------

RenderMethod::RenderMethod( _bool defaultmethod, _dword renderorder )
{
	mDefaultMethod		= defaultmethod;
	mRenderOrder		= renderorder;

	mCullMode			= _CULLMODE_CCW;
	mLightingEnabled	= _false;

	mVSObject			= _null;
	mVSConst			= _null;

	IncreaseRefCount( );
}

RenderMethod::~RenderMethod( )
{
	GraphicsManager::GetRenderAPIHelper( ).ReleaseVertexShader( mVSObject );
	delete mVSConst;
}

_void RenderMethod::SetRenderState( _dword state, _dword value )
{
	// Find the existing state.
	for ( _dword i = 0; i < mRenderStateArray.Number( ); i ++ )
	{
		// Found the same state.
		if ( mRenderStateArray[i].mState == state )
		{
			// It is default value, just remove it.
			if ( value == TechniqueFactory::GetDefaultRenderState( state ) )
			{
				mRenderStateArray.Remove( i );
			}
			// Change state value.
			else
			{
				mRenderStateArray[i].mValue = value;
			}

			return;
		}
	}

	if ( value == TechniqueFactory::GetDefaultRenderState( state ) )
		return;

	// Not found the same state, add a new one.
	mRenderStateArray.Append( RenderState( state, value ) );
}

_void RenderMethod::SetMethodString( StringPtr method )
{
	mMethodString = method;
}

StringPtr RenderMethod::GetMethodString( ) const
{
	return mMethodString;
}

_void RenderMethod::EnableMethod( ) const
{
	RenderAPI& renderapi = GraphicsManager::GetRenderAPIHelper( );

	for ( _dword i = 0; i < mRenderStateArray.Number( ); i ++ )
	{
		const RenderState& state = mRenderStateArray[i];

		renderapi.EnableRenderState( state.mState, state.mValue );
	}

	if ( mVSObject != _null )
	{
		renderapi.SetVertexShader( mVSObject );

		//if ( mVSConst != _null )
		//	mVSConst->EnableVSConst( );
	}
	else if ( mLightingEnabled )
	{
		renderapi.EnableRenderState( D3DRS_LIGHTING, _true );
		renderapi.EnableRenderState( D3DRS_NORMALIZENORMALS, _true );

		// Enable all light.
		for ( _dword i = 0; i < mLightSlotArray.Number( ); i ++ )
		{
			const LightSlot& lightslot = mLightSlotArray[i];

			renderapi.SetLight( lightslot.mSlot, (_void*) &lightslot.mLight );
		}
	}
}

_void RenderMethod::DisableMethod( ) const
{
	RenderAPI& renderapi = GraphicsManager::GetRenderAPIHelper( );

	for ( _dword i = 0; i < mRenderStateArray.Number( ); i ++ )
	{
		const RenderState& state = mRenderStateArray[i];

		renderapi.DisableRenderState( state.mState );
	}

	if ( mVSObject != _null )
	{
		renderapi.SetVertexShader( _null );
	}
	else if ( mLightingEnabled )
	{
		renderapi.EnableRenderState( D3DRS_LIGHTING, _false );
		renderapi.EnableRenderState( D3DRS_NORMALIZENORMALS, _false );

		// Disable all light.
		for ( _dword i = 0; i < mLightSlotArray.Number( ); i ++ )
		{
			const LightSlot& lightslot = mLightSlotArray[i];

			renderapi.SetLight( lightslot.mSlot, _null );
		}
	}
}

_bool RenderMethod::IsDefaultMethod( ) const
{
	return mDefaultMethod;
}

_void RenderMethod::SetRenderOrder( _dword order )
{
	if ( mDefaultMethod )
		return;

	mRenderOrder = order;
}

_dword RenderMethod::GetRenderOrder( ) const
{
	return mRenderOrder;
}

_void RenderMethod::SetCullMode( _dword value )
{
	mCullMode = value;
}

_dword RenderMethod::GetCullMode( _bool mirror ) const
{
	if ( mCullMode == _CULLMODE_NONE )
		return _CULLMODE_NONE;

	if ( mCullMode == _CULLMODE_CW )
		return mirror ? _CULLMODE_CCW : _CULLMODE_CW;
	else
		return mirror ? _CULLMODE_CW : _CULLMODE_CCW;
}

_void RenderMethod::SetFillMode( _dword value )
{
	SetRenderState( D3DRS_FILLMODE, value );
}

_void RenderMethod::SetShadeMode( _dword value )
{
	SetRenderState( D3DRS_SHADEMODE, value );
}

_void RenderMethod::EnableClipping( _bool value )
{
	SetRenderState( D3DRS_CLIPPING, value );
}

_void RenderMethod::EnableNormalizeNormals( _bool value )
{
	SetRenderState( D3DRS_NORMALIZENORMALS, value );
}

_bool RenderMethod::IsLightingEnabled( ) const
{
	return mLightingEnabled;
}

_void RenderMethod::EnableLighting( _bool value )
{
	mLightingEnabled = value;

	SetRenderState( D3DRS_LIGHTING, value );
}

_void RenderMethod::EnableSpecular( _bool value )
{
	SetRenderState( D3DRS_SPECULARENABLE, value );
}

_void RenderMethod::SetAmbientLightColor( _dword value )
{
	SetRenderState( D3DRS_AMBIENT, value );
}

_void RenderMethod::SetDirectionalLight( _dword slot, _dword diffuse, _dword specular, _dword ambient, const Vector3& direction )
{
	FL_ASSERT( slot < _MAX_LIGHT_NUMBER )

	CloseLight( slot );

	LightSlot lightslot;
	lightslot.mSlot					= slot;
	lightslot.mLight.mType			= D3DLIGHT_DIRECTIONAL;
	lightslot.mLight.mDiffuse		= Color( diffuse );
	lightslot.mLight.mSpecular		= Color( specular );
	lightslot.mLight.mAmbient		= Color( ambient );
	lightslot.mLight.mDirection		= direction;

	mLightSlotArray.Append( lightslot );
}

_void RenderMethod::SetPointLight( _dword slot, _dword diffuse, _dword specular, _dword ambient, const Vector3& position, _float range )
{
	FL_ASSERT( slot < _MAX_LIGHT_NUMBER )

	CloseLight( slot );

	LightSlot lightslot;
	lightslot.mSlot					= slot;
	lightslot.mLight.mType			= D3DLIGHT_POINT;
	lightslot.mLight.mDiffuse		= Color( diffuse );
	lightslot.mLight.mSpecular		= Color( specular );
	lightslot.mLight.mAmbient		= Color( ambient );
	lightslot.mLight.mPosition		= position;
	lightslot.mLight.mRange			= range;
	lightslot.mLight.mAttenuation0	= 0.8f;
	lightslot.mLight.mAttenuation1	= 0.0f;
	lightslot.mLight.mAttenuation2	= 0.0f;

	mLightSlotArray.Append( lightslot );
}

_void RenderMethod::SetSpotLight( _dword slot, _dword diffuse, _dword specular, _dword ambient, const Vector3& position,
	const Vector3& direction, _float range, _float falloff, _float theta, _float phi )
{
	FL_ASSERT( slot < _MAX_LIGHT_NUMBER )

	CloseLight( slot );

	LightSlot lightslot;
	lightslot.mSlot					= slot;
	lightslot.mLight.mType			= D3DLIGHT_SPOT;
	lightslot.mLight.mDiffuse		= Color( diffuse );
	lightslot.mLight.mSpecular		= Color( specular );
	lightslot.mLight.mAmbient		= Color( ambient );
	lightslot.mLight.mPosition		= position;
	lightslot.mLight.mDirection		= direction;
	lightslot.mLight.mRange			= range;
	lightslot.mLight.mFalloff		= falloff;
	lightslot.mLight.mTheta			= theta;
	lightslot.mLight.mPhi			= phi;
	lightslot.mLight.mAttenuation0	= 0.8f;
	lightslot.mLight.mAttenuation1	= 0.0f;
	lightslot.mLight.mAttenuation2	= 0.0f;

	mLightSlotArray.Append( lightslot );
}

_void RenderMethod::CloseLight( _dword slot )
{
	for ( _dword i = 0; i < mLightSlotArray.Number( ); i ++ )
	{
		if ( mLightSlotArray[i].mSlot != slot )
			continue;

		mLightSlotArray.Remove( i );
		break;
	}
}

_void RenderMethod::CloseAllLight( )
{
	mLightSlotArray.Clear( );
}

_void RenderMethod::EnableDepthWrite( _bool value )
{
	SetRenderState( D3DRS_ZWRITEENABLE, value );
}

_void RenderMethod::EnableDepthTest( _bool value )
{
	SetRenderState( D3DRS_ZENABLE, value );
}

_void RenderMethod::SetDepthTestFunction( _dword value )
{
	SetRenderState( D3DRS_ZFUNC, value );
}

_void RenderMethod::EnableStencilTest( _bool value )
{
	SetRenderState( D3DRS_STENCILENABLE, value );
}

_void RenderMethod::SetStencilFunction( _dword value )
{
	SetRenderState( D3DRS_STENCILFUNC, value );
}

_void RenderMethod::SetStencilReference( _dword value )
{
	SetRenderState( D3DRS_STENCILREF, value );
}

_void RenderMethod::SetStencilTestMask( _dword value )
{
	SetRenderState( D3DRS_STENCILMASK, value );
}

_void RenderMethod::SetStencilWriteMask( _dword value )
{
	SetRenderState( D3DRS_STENCILWRITEMASK, value );
}

_void RenderMethod::SetStencilPassoperation( _dword value )
{
	SetRenderState( D3DRS_STENCILPASS, value );
}

_void RenderMethod::SetStencilFailoperation( _dword value )
{
	SetRenderState( D3DRS_STENCILFAIL, value );
}

_void RenderMethod::SetStencilDepthFailoperation( _dword value )
{
	SetRenderState( D3DRS_STENCILZFAIL, value );
}

_void RenderMethod::EnableAlphaTest( _bool value )
{
	SetRenderState( D3DRS_ALPHATESTENABLE, value );
}

_void RenderMethod::SetAlphaTestFunction( _dword value )
{
	SetRenderState( D3DRS_ALPHAFUNC, value );
}

_void RenderMethod::SetAlphaTestReference( _dword value )
{
	SetRenderState( D3DRS_ALPHAREF, value );
}

_void RenderMethod::EnableAlphaBlend( _bool value )
{
	SetRenderState( D3DRS_ALPHABLENDENABLE, value );
}

_void RenderMethod::SetAlphaBlendOperation( _dword value )
{
	SetRenderState( D3DRS_BLENDOP, value );
}

_void RenderMethod::SetAlphaBlendFactor( _dword value )
{
	SetRenderState( D3DRS_BLENDFACTOR, value );
}

_void RenderMethod::SetAlphaBlendSrcFactor( _dword value )
{
	SetRenderState( D3DRS_SRCBLEND, value );
}

_void RenderMethod::SetAlphaBlendDesFactor( _dword value )
{
	SetRenderState( D3DRS_DESTBLEND, value );
}

_void RenderMethod::SetColorWriteMask( _dword value )
{
	SetRenderState( D3DRS_COLORWRITEENABLE, value );
}

_void RenderMethod::EnableFog( _bool value )
{
	SetRenderState( D3DRS_FOGENABLE, value );
}

_void RenderMethod::SetFogMode( _dword value )
{
	SetRenderState( D3DRS_RANGEFOGENABLE, value );
}

_void RenderMethod::SetFogVertexFormula( _dword value )
{
	SetRenderState( D3DRS_FOGVERTEXMODE, value );
}

_void RenderMethod::SetFogPixelFormula( _dword value )
{
	SetRenderState( D3DRS_FOGTABLEMODE, value );
}

_void RenderMethod::SetFogColor( _dword value )
{
	SetRenderState( D3DRS_FOGCOLOR, value );
}

_void RenderMethod::SetFogStart( _float value )
{
	SetRenderState( D3DRS_FOGSTART, *( (_dword*) &value ) );
}

_void RenderMethod::SetFogEnd( _float value )
{
	SetRenderState( D3DRS_FOGEND, *( (_dword*) &value ) );
}

_void RenderMethod::SetFogDensity( _float value )
{
	SetRenderState( D3DRS_FOGDENSITY, *( (_dword*) &value ) );
}

_void RenderMethod::EnableMultiSampleAntialias( _bool value )
{
	SetRenderState( D3DRS_MULTISAMPLEANTIALIAS,  value );
}

_void RenderMethod::SetMultiSampleMask( _dword value )
{
	SetRenderState( D3DRS_MULTISAMPLEMASK,  value );
}

_bool RenderMethod::IsVertexShaderEnabled( ) const
{
	return mVSObject != _null;
}

_bool RenderMethod::SetVertexShaderFile( StringPtr filename )
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

	return SetVertexShaderCode( shadercode );
}

_bool RenderMethod::SetVertexShaderFile( StringPtr filename, StringPtr function, StringPtr profile )
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

	return SetVertexShaderCode( shadercode, function, profile );
}

_bool RenderMethod::SetVertexShaderCode( StringPtr code )
{
	GraphicsManager::GetRenderAPIHelper( ).ReleaseVertexShader( mVSObject );
	mVSObject = GraphicsManager::GetRenderAPIHelper( ).CreateVertexShader( code );

	return mVSObject != _null;
}

_bool RenderMethod::SetVertexShaderCode( StringPtr code, StringPtr function, StringPtr profile )
{
	GraphicsManager::GetRenderAPIHelper( ).ReleaseVertexShader( mVSObject );
	mVSObject = GraphicsManager::GetRenderAPIHelper( ).CreateVertexShader( code, function, profile );

	return mVSObject != _null;
}

_void RenderMethod::SetVertexShaderConstant( _dword index, _dword parametertype )
{
	if ( mVSConst == _null )
		mVSConst = new ShaderConst;

	mVSConst->SetVertexShaderConstant( index, parametertype );
}

_void RenderMethod::SetVertexShaderConstant( _dword index, const Vector3& constant )
{
	if ( mVSConst == _null )
		mVSConst = new ShaderConst;

	mVSConst->SetVertexShaderConstant( index, constant );
}

_void RenderMethod::SetVertexShaderConstant( _dword index, const Vector4& constant )
{
	if ( mVSConst == _null )
		mVSConst = new ShaderConst;

	mVSConst->SetVertexShaderConstant( index, constant );
}

_void RenderMethod::SetVertexShaderConstant( _dword index, const Quaternion& constant )
{
	if ( mVSConst == _null )
		mVSConst = new ShaderConst;

	mVSConst->SetVertexShaderConstant( index, constant );
}

_void RenderMethod::SetVertexShaderConstant( _dword index, const Matrix4& constant )
{
	if ( mVSConst == _null )
		mVSConst = new ShaderConst;

	mVSConst->SetVertexShaderConstant( index, constant );
}

IShaderConst* RenderMethod::GetVertexShaderConstant( )
{
	return mVSConst;
}