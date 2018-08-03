//============================================================================
// ITextureMethod.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ITextureMethod
//----------------------------------------------------------------------------

class ITextureMethod
{
public:
	enum _TEXTUREOP
	{
		_TEXTUREOP_DISABLE				= 1,
		_TEXTUREOP_SELECTARG1			= 2,
		_TEXTUREOP_SELECTARG2			= 3,
		_TEXTUREOP_MODULATE				= 4,
		_TEXTUREOP_MODULATE2X			= 5,
		_TEXTUREOP_MODULATE4X			= 6,
		_TEXTUREOP_ADD					= 7,
		_TEXTUREOP_ADDSIGNED			= 8,
		_TEXTUREOP_ADDSIGNED2X			= 9,
		_TEXTUREOP_SUBTRACT				= 10,
		_TEXTUREOP_ADDSMOOTH			= 11,
		_TEXTUREOP_BLENDDIFFUSEALPHA	= 12,
		_TEXTUREOP_BLENDTEXTUREALPHA	= 13,
		_TEXTUREOP_BLENDFACTORALPHA		= 14,
		_TEXTUREOP_BLENDTEXTUREALPHAPM	= 15,
		_TEXTUREOP_BLENDCURRENTALPHA	= 16,
		_TEXTUREOP_PREMODULATE			= 17,
		_TEXTUREOP_MULALPHA_ADDCOLOR	= 18,
		_TEXTUREOP_MULCOLOR_ADDALPHA	= 19,
		_TEXTUREOP_MULINVALPHA_ADDCOLOR	= 20,
		_TEXTUREOP_MULINVCOLOR_ADDALPHA	= 21,
		_TEXTUREOP_BUMPENVMAP			= 22,
		_TEXTUREOP_BUMPENVMAPLUMINANCE	= 23,
		_TEXTUREOP_DOTPRODUCT3			= 24,
		_TEXTUREOP_MULTIPLYADD			= 25,
		_TEXTUREOP_LERP					= 26,
	};

	enum _TEXTUREVALUE
	{
		_TEXTUREVALUE_DIFFUSE			= 0,
		_TEXTUREVALUE_CURRENT			= 1,
		_TEXTUREVALUE_TEXTURE			= 2,
		_TEXTUREVALUE_TFACTOR			= 3,
		_TEXTUREVALUE_SPECULAR			= 4,
		_TEXTUREVALUE_TEMP				= 5,
		_TEXTUREVALUE_CONSTANT			= 6,
		_TEXTUREVALUE_SELECTMASK		= 15,
		_TEXTUREVALUE_COMPLEMENT		= 16,
		_TEXTUREVALUE_ALPHAREPLICATE	= 32,
	};

	enum _TEXTURETRANSFORM
	{
		_TEXTURETRANSFORM_DISABLE		= 0,
		_TEXTURETRANSFORM_COUNT1		= 1,
		_TEXTURETRANSFORM_COUNT2		= 2,
		_TEXTURETRANSFORM_COUNT3		= 3,
		_TEXTURETRANSFORM_COUNT4		= 4,
		_TEXTURETRANSFORM_PROJECTED		= 256,
	};

	enum _TEXCOORDINDEX
	{
		_TEXCOORDINDEX_PASSTHRU						= 0x00000000,
		_TEXCOORDINDEX_CAMERASPACENORMAL			= 0x00010000,
		_TEXCOORDINDEX_CAMERASPACEPOSITION			= 0x00020000,
		_TEXCOORDINDEX_CAMERASPACEREFLECTIONVECTOR	= 0x00030000,
		_TEXCOORDINDEX_SPHEREMAP					= 0x00040000,
	};

public:
	virtual _bool	IsDefaultMethod( ) const = 0;

	virtual _void	SetTextureColorOperation( _dword stage, _dword value ) = 0;
	virtual _void	SetTextureColorArgument0( _dword stage, _dword value ) = 0;
	virtual _void	SetTextureColorArgument1( _dword stage, _dword value ) = 0;
	virtual _void	SetTextureColorArgument2( _dword stage, _dword value ) = 0;

	virtual _void	SetTextureAlphaOperation( _dword stage, _dword value ) = 0;
	virtual _void	SetTextureAlphaArgument0( _dword stage, _dword value ) = 0;
	virtual _void	SetTextureAlphaArgument1( _dword stage, _dword value ) = 0;
	virtual _void	SetTextureAlphaArgument2( _dword stage, _dword value ) = 0;

	virtual _void	SetTextureCoordinateIndex( _dword stage, _dword value ) = 0;
	virtual _void	SetTextureTransformFlags( _dword stage, _dword value ) = 0;

	virtual _void	SetTextureResultArgument( _dword stage, _dword value ) = 0;
	virtual _void	SetTextureConstant( _dword stage, _dword value ) = 0;

	virtual _void	SetTextureBumpScale( _dword stage, _float value ) = 0;
	virtual _void	SetTextureBumpOffset( _dword stage, _float value ) = 0;
	virtual _void	SetTextureBumpMatrix( _dword stage, const Matrix2& matrix ) = 0;

	virtual _bool	IsPixelShaderEnabled( ) const = 0;
	virtual _bool	SetPixelShaderFile( StringPtr filename ) = 0;
	virtual _bool	SetPixelShaderFile( StringPtr filename, StringPtr function, StringPtr profile ) = 0;
	virtual _bool	SetPixelShaderCode( StringPtr code ) = 0;
	virtual _bool	SetPixelShaderCode( StringPtr code, StringPtr function, StringPtr profile ) = 0;
	virtual _void	SetPixelShaderConstant( _dword index, const Vector3& constant ) = 0;
	virtual _void	SetPixelShaderConstant( _dword index, const Vector4& constant ) = 0;
	virtual _void	SetPixelShaderConstant( _dword index, const Color& constant ) = 0;

	virtual IShaderConst*	GetPixelShaderConstant( ) = 0;
};

};