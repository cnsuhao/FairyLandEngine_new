//============================================================================
// SamplerMethod.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// SamplerMethod
//----------------------------------------------------------------------------

class SamplerMethod : public ISamplerMethod, public ReferencedObject
{
private:
	struct SamplerState
	{
		_dword	mStage;
		_dword	mState;
		_dword	mValue;

		SamplerState( _dword stage = 0, _dword state = 0, _dword value = 0 )
			: mStage( stage ), mState( state ), mValue( value ) { }
	};

	_bool					mDefaultMethod;

	Array< SamplerState >	mSamplerStateArray;

	_void		SetSamplerState( _dword stage, _dword state, _dword value );

public:
	SamplerMethod( _bool defaultmethod );
	~SamplerMethod( );

	_void	EnableMethod( ) const;
	_void	DisableMethod( ) const;

	virtual _bool	IsDefaultMethod( ) const;

	virtual _void	SetSamplerAddressU( _dword stage, _dword value );
	virtual _void	SetSamplerAddressV( _dword stage, _dword value );
	virtual _void	SetSamplerAddressW( _dword stage, _dword value );
	virtual _void	SetSamplerBorderColor( _dword stage, _dword value );

	virtual _void	SetSamplerMagFilter( _dword stage, _dword value );
	virtual _void	SetSamplerMinFilter( _dword stage, _dword value );
	virtual _void	SetSamplerMipFilter( _dword stage, _dword value );

	virtual _void	SetSamplerMipmapBias( _dword stage, _dword value );
	virtual _void	SetSamplerMaxMipmapLevel( _dword stage, _dword value );
	virtual _void	SetSamplerMaxAnisotropy( _dword stage, _dword value );
};

};