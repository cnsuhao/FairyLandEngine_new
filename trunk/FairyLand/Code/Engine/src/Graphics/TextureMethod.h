//============================================================================
// TextureMethod.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// TextureMethod
//----------------------------------------------------------------------------

class TextureMethod : public ITextureMethod, public ReferencedObject
{
private:
	struct TextureState
	{
		_dword	mStage;
		_dword	mState;
		_dword	mValue;

		TextureState( _dword stage = 0, _dword state = 0, _dword value = 0 )
			: mStage( stage ), mState( state ), mValue( value ) { }
	};

	_bool					mDefaultMethod;

	Array< TextureState >	mTextureStateArray;

	_void*					mPSObject;
	ShaderConst*			mPSConst;

	_void	SetTextureState( _dword stage, _dword state, _dword value );

public:
	TextureMethod( _bool defaultmethod );
	~TextureMethod( );

	_void	EnableMethod( ) const;
	_void	DisableMethod( ) const;

	virtual _bool	IsDefaultMethod( ) const;

	virtual _void	SetTextureColorOperation( _dword stage, _dword value );
	virtual _void	SetTextureColorArgument0( _dword stage, _dword value );
	virtual _void	SetTextureColorArgument1( _dword stage, _dword value );
	virtual _void	SetTextureColorArgument2( _dword stage, _dword value );

	virtual _void	SetTextureAlphaOperation( _dword stage, _dword value );
	virtual _void	SetTextureAlphaArgument0( _dword stage, _dword value );
	virtual _void	SetTextureAlphaArgument1( _dword stage, _dword value );
	virtual _void	SetTextureAlphaArgument2( _dword stage, _dword value );

	virtual _void	SetTextureCoordinateIndex( _dword stage, _dword value );
	virtual _void	SetTextureTransformFlags( _dword stage, _dword value );

	virtual _void	SetTextureResultArgument( _dword stage, _dword value );
	virtual _void	SetTextureConstant( _dword stage, _dword value );

	virtual _void	SetTextureBumpScale( _dword stage, _float value );
	virtual _void	SetTextureBumpOffset( _dword stage, _float value );
	virtual _void	SetTextureBumpMatrix( _dword stage, const Matrix2& matrix );

	virtual _bool	IsPixelShaderEnabled( ) const;
	virtual _bool	SetPixelShaderFile( StringPtr filename );
	virtual _bool	SetPixelShaderFile( StringPtr filename, StringPtr function, StringPtr profile );
	virtual _bool	SetPixelShaderCode( StringPtr code );
	virtual _bool	SetPixelShaderCode( StringPtr code, StringPtr function, StringPtr profile );
	virtual _void	SetPixelShaderConstant( _dword index, const Vector3& constant );
	virtual _void	SetPixelShaderConstant( _dword index, const Vector4& constant );
	virtual _void	SetPixelShaderConstant( _dword index, const Color& constant );

	virtual IShaderConst*	GetPixelShaderConstant( );
};

};