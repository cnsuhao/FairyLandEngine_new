//============================================================================
// RenderMethod.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// RenderMethod
//----------------------------------------------------------------------------

class RenderMethod : public IRenderMethod, public ReferencedObject
{
private:
	enum { _MAX_LIGHT_NUMBER = 8 };

	struct RenderState
	{
		_dword	mState;
		_dword	mValue;

		RenderState( _dword state = 0, _dword value = 0 )
			: mState( state ), mValue( value ) { }
	};

	struct Light
	{
		_dword	mType;
		Color	mDiffuse;
		Color	mSpecular;
		Color	mAmbient;
		Vector3	mPosition;
		Vector3	mDirection;
		_float	mRange;
		_float	mFalloff;
		_float	mAttenuation0;
		_float	mAttenuation1;
		_float	mAttenuation2;
		_float	mTheta;
		_float	mPhi;
	};

	struct LightSlot
	{
		_dword	mSlot;
		Light	mLight;
	};

	_bool					mDefaultMethod;
	_dword					mRenderOrder;
	String					mMethodString;

	Array< RenderState >	mRenderStateArray;
	Array< LightSlot >		mLightSlotArray;

	_dword					mCullMode;
	_bool					mLightingEnabled;

	_void*					mVSObject;
	ShaderConst*			mVSConst;

	_void		SetRenderState( _dword state, _dword value );

public:
	RenderMethod( _bool defaultmethod, _dword renderorder );
	~RenderMethod( );

	_void		SetMethodString( StringPtr method );
	StringPtr	GetMethodString( ) const;

	_void		EnableMethod( ) const;
	_void		DisableMethod( ) const;

	virtual _bool	IsDefaultMethod( ) const;

	virtual _void	SetRenderOrder( _dword order );
	virtual _dword	GetRenderOrder( ) const;

	virtual _void	SetCullMode( _dword value );
	virtual _dword	GetCullMode( _bool mirror ) const;

	virtual _void	SetFillMode( _dword value );
	virtual _void	SetShadeMode( _dword value );

	virtual _void	EnableClipping( _bool value );
	virtual _void	EnableNormalizeNormals( _bool value );

	virtual _bool	IsLightingEnabled( ) const;
	virtual _void	EnableLighting( _bool value );
	virtual _void	EnableSpecular( _bool value );
	virtual _void	SetAmbientLightColor( _dword value );

	virtual _void	SetDirectionalLight( _dword slot, _dword diffuse, _dword specular, _dword ambient, const Vector3& direction );
	virtual _void	SetPointLight( _dword slot, _dword diffuse, _dword specular, _dword ambient, const Vector3& position, _float range );
	virtual _void	SetSpotLight( _dword slot, _dword diffuse, _dword specular, _dword ambient, const Vector3& position, const Vector3& direction, _float range, _float falloff, _float theta, _float phi );

	virtual _void	CloseLight( _dword slot );
	virtual _void	CloseAllLight( );

	virtual _void	EnableDepthWrite( _bool value );
	virtual _void	EnableDepthTest( _bool value );
	virtual _void	SetDepthTestFunction( _dword value );

	virtual _void	EnableStencilTest( _bool value );
	virtual _void	SetStencilFunction( _dword value );
	virtual _void	SetStencilReference( _dword value );
	virtual _void	SetStencilTestMask( _dword value );
	virtual _void	SetStencilWriteMask( _dword value );
	virtual _void	SetStencilPassoperation( _dword value );
	virtual _void	SetStencilFailoperation( _dword value );
	virtual _void	SetStencilDepthFailoperation( _dword value );

	virtual _void	EnableAlphaTest( _bool value );
	virtual _void	SetAlphaTestFunction( _dword value );
	virtual _void	SetAlphaTestReference( _dword value );

	virtual _void	EnableAlphaBlend( _bool value );
	virtual _void	SetAlphaBlendOperation( _dword value );
	virtual _void	SetAlphaBlendFactor( _dword value );
	virtual _void	SetAlphaBlendSrcFactor( _dword value );
	virtual _void	SetAlphaBlendDesFactor( _dword value );

	virtual _void	SetColorWriteMask( _dword value );

	virtual _void	EnableFog( _bool value );
	virtual _void	SetFogMode( _dword value );
	virtual _void	SetFogVertexFormula( _dword value );
	virtual _void	SetFogPixelFormula( _dword value );
	virtual _void	SetFogColor( _dword value );
	virtual _void	SetFogStart( _float value );
	virtual _void	SetFogEnd( _float value );
	virtual _void	SetFogDensity( _float value );

	virtual _void	EnableMultiSampleAntialias( _bool value );
	virtual _void	SetMultiSampleMask( _dword value );

	virtual _bool	IsVertexShaderEnabled( ) const;
	virtual _bool	SetVertexShaderFile( StringPtr filename );
	virtual _bool	SetVertexShaderFile( StringPtr filename, StringPtr function, StringPtr profile );
	virtual _bool	SetVertexShaderCode( StringPtr code );
	virtual _bool	SetVertexShaderCode( StringPtr code, StringPtr function, StringPtr profile );
	virtual _void	SetVertexShaderConstant( _dword index, _dword parametertype );
	virtual _void	SetVertexShaderConstant( _dword index, const Vector3& constant );
	virtual _void	SetVertexShaderConstant( _dword index, const Vector4& constant );
	virtual _void	SetVertexShaderConstant( _dword index, const Quaternion& constant );
	virtual _void	SetVertexShaderConstant( _dword index, const Matrix4& constant );

	virtual IShaderConst*	GetVertexShaderConstant( );
};

};