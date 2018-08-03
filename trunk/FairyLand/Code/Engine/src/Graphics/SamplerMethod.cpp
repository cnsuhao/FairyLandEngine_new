//============================================================================
// SamplerMethod.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// SamplerMethod Implementation
//----------------------------------------------------------------------------

SamplerMethod::SamplerMethod( _bool defaultmethod )
{
	mDefaultMethod	= defaultmethod;

	IncreaseRefCount( );
}

SamplerMethod::~SamplerMethod( )
{
}

_void SamplerMethod::SetSamplerState( _dword stage, _dword state, _dword value )
{
	// Find the existing state.
	for ( _dword i = 0; i < mSamplerStateArray.Number( ); i ++ )
	{
		// Found the same state.
		if ( mSamplerStateArray[i].mStage == stage && mSamplerStateArray[i].mState == state )
		{
			// It is default value, just remove it.
			if ( value == TechniqueFactory::GetDefaultSamplerState( stage, state ) )
			{
				mSamplerStateArray.Remove( i );
			}
			// Change state value.
			else
			{
				mSamplerStateArray[i].mValue = value;
			}

			return;
		}
	}

	if ( value == TechniqueFactory::GetDefaultSamplerState( stage, state ) )
		return;

	// Not found the same state, add a new one.
	mSamplerStateArray.Append( SamplerState( stage, state, value ) );
}

_void SamplerMethod::EnableMethod( ) const
{
	for ( _dword i = 0; i < mSamplerStateArray.Number( ); i ++ )
	{
		const SamplerState& state = mSamplerStateArray[i];

		GraphicsManager::GetRenderAPIHelper( ).EnableSamplerState( state.mStage, state.mState, state.mValue );
	}
}

_void SamplerMethod::DisableMethod( ) const
{
	for ( _dword i = 0; i < mSamplerStateArray.Number( ); i ++ )
	{
		const SamplerState& state = mSamplerStateArray[i];

		GraphicsManager::GetRenderAPIHelper( ).DisableSamplerState( state.mStage, state.mState );
	}
}

_bool SamplerMethod::IsDefaultMethod( ) const
{
	return mDefaultMethod;
}

_void SamplerMethod::SetSamplerAddressU( _dword stage, _dword value )
{
	SetSamplerState( stage, D3DSAMP_ADDRESSU, value );
}

_void SamplerMethod::SetSamplerAddressV( _dword stage, _dword value )
{
	SetSamplerState( stage, D3DSAMP_ADDRESSV, value );
}

_void SamplerMethod::SetSamplerAddressW( _dword stage, _dword value )
{
	SetSamplerState( stage, D3DSAMP_ADDRESSW, value );
}

_void SamplerMethod::SetSamplerBorderColor( _dword stage, _dword value )
{
	SetSamplerState( stage, D3DSAMP_BORDERCOLOR, value );
}

_void SamplerMethod::SetSamplerMagFilter( _dword stage, _dword value )
{
	SetSamplerState( stage, D3DSAMP_MAGFILTER, value );
}

_void SamplerMethod::SetSamplerMinFilter( _dword stage, _dword value )
{
	SetSamplerState( stage, D3DSAMP_MINFILTER, value );
}

_void SamplerMethod::SetSamplerMipFilter( _dword stage, _dword value )
{
	SetSamplerState( stage, D3DSAMP_MIPFILTER, value );
}

_void SamplerMethod::SetSamplerMipmapBias( _dword stage, _dword value )
{
	SetSamplerState( stage, D3DSAMP_MIPMAPLODBIAS, value );
}

_void SamplerMethod::SetSamplerMaxMipmapLevel( _dword stage, _dword value )
{
	SetSamplerState( stage, D3DSAMP_MAXMIPLEVEL, value );
}

_void SamplerMethod::SetSamplerMaxAnisotropy( _dword stage, _dword value )
{
	SetSamplerState( stage, D3DSAMP_MAXANISOTROPY, value );
}