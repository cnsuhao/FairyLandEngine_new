//============================================================================
// IRenderMethod.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IRenderMethod
//----------------------------------------------------------------------------

class IRenderMethod
{
public:
	enum _RENDERORDER
	{
		_RENDERORDER_NORMAL				= 0,
		_RENDERORDER_DECAL				= 1,
		_RENDERORDER_COLORKEY			= 2,
		_RENDERORDER_ALPHA				= 3,
	};

	enum _CULLMODE
	{
		_CULLMODE_NONE					= 1,
		_CULLMODE_CW					= 2,
		_CULLMODE_CCW					= 3,
	};

	enum _FILLMODE
	{
		_FILLMODE_POINT					= 1,
		_FILLMODE_WIREFRAME				= 2,
		_FILLMODE_SOLID					= 3,
	};

	enum _SHADEMODE
	{
		_SHADEMODE_FLAT					= 1,
		_SHADEMODE_SMOOTH				= 2,
	};

	enum _COMPAREFUNC
	{
		_COMPAREFUNC_NEVER				= 1,
		_COMPAREFUNC_LESS				= 2,
		_COMPAREFUNC_EQUAL				= 3,
		_COMPAREFUNC_LESSEQUAL			= 4,
		_COMPAREFUNC_GREATER			= 5,
		_COMPAREFUNC_NOTEQUAL			= 6,
		_COMPAREFUNC_GREATEREQUAL 		= 7,
		_COMPAREFUNC_ALWAYS				= 8,
	};

	enum _STENCILOP
	{
		_STENCILOP_KEEP					= 1,
		_STENCILOP_ZERO					= 2,
		_STENCILOP_REPLACE				= 3,
		_STENCILOP_INCREASE_CLAMP		= 4,
		_STENCILOP_DECREASE_CLAMP		= 5,
		_STENCILOP_INVERT				= 6,
		_STENCILOP_INCREASE_WRAP		= 7,
		_STENCILOP_DECREASE_WRAP		= 8,
	};

	enum _BLENDOP
	{
		_BLENDOP_ADD					= 1,
		_BLENDOP_SUBTRACT				= 2,
		_BLENDOP_REVSUBTRACT			= 3,
		_BLENDOP_MIN					= 4,
		_BLENDOP_MAX					= 5,
	};

	enum _BLENDFACTOR
	{
		_BLENDFACTOR_ZERO				= 1,
		_BLENDFACTOR_ONE				= 2,
		_BLENDFACTOR_SRCCOLOR			= 3,
		_BLENDFACTOR_INVSRCCOLOR		= 4,
		_BLENDFACTOR_SRCALPHA			= 5,
		_BLENDFACTOR_INVSRCALPHA		= 6,
		_BLENDFACTOR_DESTALPHA			= 7,
		_BLENDFACTOR_INVDESTALPHA		= 8,
		_BLENDFACTOR_DESTCOLOR			= 9,
		_BLENDFACTOR_INVDESTCOLOR		= 10,
		_BLENDFACTOR_SRCALPHASAT		= 11,
		_BLENDFACTOR_BOTHSRCALPHA		= 12,
		_BLENDFACTOR_BOTHINVSRCALPHA	= 13,
		_BLENDFACTOR_BLENDFACTOR		= 14,
		_BLENDFACTOR_INVBLENDFACTOR		= 15,
	};

	enum _FOGMODE
	{
		_FOGMODE_DEPTHBASED				= 0,
		_FOGMODE_RANGEBASED				= 1,
	};

	enum _FOGFORMULA
	{
		_FOGFORMULA_NONE				= 0,
		_FOGFORMULA_EXP					= 1,
		_FOGFORMULA_EXP2				= 2,
		_FOGFORMULA_LINEAR				= 3,
	};

	enum _PARAMETER_TYPE
	{
		_PARAMETER_NONE					= 0,

		_PARAMETER_TRANSFORM_W			= 1,
		_PARAMETER_TRANSFORM_WV			= 2,
		_PARAMETER_TRANSFORM_WVP		= 3,

		_PARAMETER_TRANSFORM_W_TRP		= 4,
		_PARAMETER_TRANSFORM_WV_TRP		= 5,
		_PARAMETER_TRANSFORM_WVP_TRP	= 6,
	};

public:
	virtual _bool	IsDefaultMethod( ) const = 0;

	virtual _void	SetRenderOrder( _dword order ) = 0;
	virtual _dword	GetRenderOrder( ) const = 0;

	virtual _void	SetCullMode( _dword value ) = 0;
	virtual _dword	GetCullMode( _bool mirror ) const = 0;

	virtual _void	SetFillMode( _dword value ) = 0;
	virtual _void	SetShadeMode( _dword value ) = 0;

	virtual _void	EnableClipping( _bool value ) = 0;
	virtual _void	EnableNormalizeNormals( _bool value ) = 0;

	virtual _bool	IsLightingEnabled( ) const = 0;
	virtual _void	EnableLighting( _bool value ) = 0;
	virtual _void	EnableSpecular( _bool value ) = 0;
	virtual _void	SetAmbientLightColor( _dword value ) = 0;

	virtual _void	SetDirectionalLight( _dword slot, _dword diffuse, _dword specular, _dword ambient, const Vector3& direction ) = 0;
	virtual _void	SetPointLight( _dword slot, _dword diffuse, _dword specular, _dword ambient, const Vector3& position, _float range ) = 0;
	virtual _void	SetSpotLight( _dword slot, _dword diffuse, _dword specular, _dword ambient, const Vector3& position, const Vector3& direction, _float range, _float falloff, _float theta, _float phi ) = 0;

	virtual _void	CloseLight( _dword slot ) = 0;
	virtual _void	CloseAllLight( ) = 0;

	virtual _void	EnableDepthWrite( _bool value ) = 0;
	virtual _void	EnableDepthTest( _bool value ) = 0;
	virtual _void	SetDepthTestFunction( _dword value ) = 0;

	virtual _void	EnableStencilTest( _bool value ) = 0;
	virtual _void	SetStencilFunction( _dword value ) = 0;
	virtual _void	SetStencilReference( _dword value ) = 0;
	virtual _void	SetStencilTestMask( _dword value ) = 0;
	virtual _void	SetStencilWriteMask( _dword value ) = 0;
	virtual _void	SetStencilPassoperation( _dword value ) = 0;
	virtual _void	SetStencilFailoperation( _dword value ) = 0;
	virtual _void	SetStencilDepthFailoperation( _dword value ) = 0;

	virtual _void	EnableAlphaTest( _bool value ) = 0;
	virtual _void	SetAlphaTestFunction( _dword value ) = 0;
	virtual _void	SetAlphaTestReference( _dword value ) = 0;

	virtual _void	EnableAlphaBlend( _bool value ) = 0;
	virtual _void	SetAlphaBlendOperation( _dword value ) = 0;
	virtual _void	SetAlphaBlendFactor( _dword value ) = 0;
	virtual _void	SetAlphaBlendSrcFactor( _dword value ) = 0;
	virtual _void	SetAlphaBlendDesFactor( _dword value ) = 0;

	virtual _void	SetColorWriteMask( _dword value ) = 0;

	virtual _void	EnableFog( _bool value ) = 0;
	virtual _void	SetFogMode( _dword value ) = 0;
	virtual _void	SetFogVertexFormula( _dword value ) = 0;
	virtual _void	SetFogPixelFormula( _dword value ) = 0;
	virtual _void	SetFogColor( _dword value ) = 0;
	virtual _void	SetFogStart( _float value ) = 0;
	virtual _void	SetFogEnd( _float value ) = 0;
	virtual _void	SetFogDensity( _float value ) = 0;

	virtual _void	EnableMultiSampleAntialias( _bool value ) = 0;
	virtual _void	SetMultiSampleMask( _dword value ) = 0;

	virtual _bool	IsVertexShaderEnabled( ) const = 0;
	virtual _bool	SetVertexShaderFile( StringPtr filename ) = 0;
	virtual _bool	SetVertexShaderFile( StringPtr filename, StringPtr function, StringPtr profile ) = 0;
	virtual _bool	SetVertexShaderCode( StringPtr code ) = 0;
	virtual _bool	SetVertexShaderCode( StringPtr code, StringPtr function, StringPtr profile ) = 0;
	virtual _void	SetVertexShaderConstant( _dword index, _dword parametertype ) = 0;
	virtual _void	SetVertexShaderConstant( _dword index, const Vector3& constant ) = 0;
	virtual _void	SetVertexShaderConstant( _dword index, const Vector4& constant ) = 0;
	virtual _void	SetVertexShaderConstant( _dword index, const Quaternion& constant ) = 0;
	virtual _void	SetVertexShaderConstant( _dword index, const Matrix4& constant ) = 0;

	virtual IShaderConst*	GetVertexShaderConstant( ) = 0;
};

};