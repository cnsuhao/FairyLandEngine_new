//============================================================================
// ShaderConst.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// ShaderConst Implementation
//----------------------------------------------------------------------------

ShaderConst::ShaderConst( )
{
	for ( _dword i = 0; i < _MAX_CONSTANT_NUMBER; i ++ )
	{
		mVSConstantFlag[i] = -1;
		mPSConstantFlag[i] = -1;
	}

	IncreaseRefCount( );
}

_void ShaderConst::EnableVSConst( ) const
{
	for ( _dword i = 0; i < _MAX_CONSTANT_NUMBER; i ++ )
	{
		_dword index = mVSConstantFlag[i];

		if ( index == -1 )
			break;

		if ( index < _MAX_CONSTANT_NUMBER )
		{
			GraphicsManager::GetRenderAPIHelper( ).SetVertexShaderConstant( index, (const _float*) &mVSConstantValue[index], 1 );

			continue;
		}

		_dword flag = ( index & 0xFF000000 ) >> 24;

		index &= 0x000000FF;

		if ( flag == IRenderMethod::_PARAMETER_TRANSFORM_W )
		{
			Matrix4 worldtransform;
			GraphicsManager::GetRenderAPIHelper( ).GetWorldTransform( worldtransform );

			// Set shader parameter as world transform.
			GraphicsManager::GetRenderAPIHelper( ).SetVertexShaderConstant( index, (const _float*) &worldtransform, 4 );
		}
		else if ( flag == IRenderMethod::_PARAMETER_TRANSFORM_WV )
		{
			Matrix4 worldtransform;
			GraphicsManager::GetRenderAPIHelper( ).GetWorldTransform( worldtransform );

			Matrix4 viewtransform;
			GraphicsManager::GetRenderAPIHelper( ).GetViewTransform( viewtransform );

			worldtransform *= viewtransform;

			// Set shader parameter as world transform * view transform.
			GraphicsManager::GetRenderAPIHelper( ).SetVertexShaderConstant( index, (const _float*) &worldtransform, 4 );
		}
		else if ( flag == IRenderMethod::_PARAMETER_TRANSFORM_WVP )
		{
			Matrix4 worldtransform;
			GraphicsManager::GetRenderAPIHelper( ).GetWorldTransform( worldtransform );

			Matrix4 viewtransform;
			GraphicsManager::GetRenderAPIHelper( ).GetViewTransform( viewtransform );

			Matrix4 projtransform;
			GraphicsManager::GetRenderAPIHelper( ).GetProjectionTransform( projtransform );

			worldtransform *= viewtransform;
			worldtransform *= projtransform;

			// Set shader parameter as world transform * view transform * projection transform.
			GraphicsManager::GetRenderAPIHelper( ).SetVertexShaderConstant( index, (const _float*) &worldtransform, 4 );
		}
		else if ( flag == IRenderMethod::_PARAMETER_TRANSFORM_W_TRP )
		{
			Matrix4 worldtransform;
			GraphicsManager::GetRenderAPIHelper( ).GetWorldTransform( worldtransform );

			worldtransform.Transpose( );

			// Set shader parameter as world transform.
			GraphicsManager::GetRenderAPIHelper( ).SetVertexShaderConstant( index, (const _float*) &worldtransform, 4 );
		}
		else if ( flag == IRenderMethod::_PARAMETER_TRANSFORM_WV_TRP )
		{
			Matrix4 worldtransform;
			GraphicsManager::GetRenderAPIHelper( ).GetWorldTransform( worldtransform );

			Matrix4 viewtransform;
			GraphicsManager::GetRenderAPIHelper( ).GetViewTransform( viewtransform );

			worldtransform *= viewtransform;

			worldtransform.Transpose( );

			// Set shader parameter as world transform * view transform.
			GraphicsManager::GetRenderAPIHelper( ).SetVertexShaderConstant( index, (const _float*) &worldtransform, 4 );
		}
		else if ( flag == IRenderMethod::_PARAMETER_TRANSFORM_WVP_TRP )
		{
			Matrix4 worldtransform;
			GraphicsManager::GetRenderAPIHelper( ).GetWorldTransform( worldtransform );

			Matrix4 viewtransform;
			GraphicsManager::GetRenderAPIHelper( ).GetViewTransform( viewtransform );

			Matrix4 projtransform;
			GraphicsManager::GetRenderAPIHelper( ).GetProjectionTransform( projtransform );

			worldtransform *= viewtransform;
			worldtransform *= projtransform;

			worldtransform.Transpose( );

			// Set shader parameter as world transform * view transform * projection transform.
			GraphicsManager::GetRenderAPIHelper( ).SetVertexShaderConstant( index, (const _float*) &worldtransform, 4 );
		}
	}
}

_void ShaderConst::DisableVSConst( ) const
{
	for ( _dword i = 0; i < _MAX_CONSTANT_NUMBER; i ++ )
	{
		_dword index = mVSConstantFlag[i];

		if ( index == -1 )
			break;

		GraphicsManager::GetRenderAPIHelper( ).SetVertexShaderConstant( index, (const _float*) &Vector4::cIdentity, 1 );
	}
}

_void ShaderConst::EnablePSConst( ) const
{
	for ( _dword i = 0; i < _MAX_CONSTANT_NUMBER; i ++ )
	{
		_dword index = mPSConstantFlag[i];

		if ( index == -1 )
			break;

		GraphicsManager::GetRenderAPIHelper( ).SetPixelShaderConstant( index, (const _float*) &mPSConstantValue[index], 1 );
	}
}

_void ShaderConst::DisablePSConst( ) const
{
	for ( _dword i = 0; i < _MAX_CONSTANT_NUMBER; i ++ )
	{
		_dword index = mPSConstantFlag[i];

		if ( index == -1 )
			break;

		GraphicsManager::GetRenderAPIHelper( ).SetPixelShaderConstant( index, (const _float*) &Color::cWhite, 1 );
	}
}

_void ShaderConst::SetVSConstantFlag( _dword index )
{
	for ( _dword i = 0; i < _MAX_CONSTANT_NUMBER; i ++ )
	{
		if ( mVSConstantFlag[i] == index )
			return;

		if ( mVSConstantFlag[i] == -1 )
		{
			mVSConstantFlag[i] = index;
			return;
		}
	}
}

_void ShaderConst::SetPSConstantFlag( _dword index )
{
	for ( _dword i = 0; i < _MAX_CONSTANT_NUMBER; i ++ )
	{
		if ( mPSConstantFlag[i] == index )
			return;

		if ( mPSConstantFlag[i] == -1 )
		{
			mPSConstantFlag[i] = index;
			return;
		}
	}
}

_void ShaderConst::SetVertexShaderConstant( _dword index, _dword parametertype )
{
	if ( index >= _MAX_CONSTANT_NUMBER )
		return;

	if ( index + 3 >= _MAX_CONSTANT_NUMBER )
		return;

	SetVSConstantFlag( ( parametertype << 24 ) | index );
}

_void ShaderConst::SetVertexShaderConstant( _dword index, const Vector3& constant )
{
	if ( index >= _MAX_CONSTANT_NUMBER )
		return;

	SetVSConstantFlag( index );

	mVSConstantValue[ index ] = Vector4( constant, 1.0f );
}

_void ShaderConst::SetVertexShaderConstant( _dword index, const Vector4& constant )
{
	if ( index >= _MAX_CONSTANT_NUMBER )
		return;

	SetVSConstantFlag( index );

	mVSConstantValue[ index ] = constant;
}

_void ShaderConst::SetVertexShaderConstant( _dword index, const Quaternion& constant )
{
	if ( index >= _MAX_CONSTANT_NUMBER )
		return;

	SetVSConstantFlag( index );

	mVSConstantValue[ index ] = Vector4( constant.x, constant.y, constant.z, constant.w );
}

_void ShaderConst::SetVertexShaderConstant( _dword index, const Matrix4& constant )
{
	if ( index >= _MAX_CONSTANT_NUMBER )
		return;

	if ( index + 3 >= _MAX_CONSTANT_NUMBER )
		return;

	SetVSConstantFlag( index );
	SetVSConstantFlag( index + 1 );
	SetVSConstantFlag( index + 2 );
	SetVSConstantFlag( index + 3 );

	mVSConstantValue[ index     ] = constant.GetRow( 0 );
	mVSConstantValue[ index + 1 ] = constant.GetRow( 1 );
	mVSConstantValue[ index + 2 ] = constant.GetRow( 2 );
	mVSConstantValue[ index + 3 ] = constant.GetRow( 3 );
}

_void ShaderConst::SetPixelShaderConstant( _dword index, const Vector3& constant )
{
	if ( index >= _MAX_CONSTANT_NUMBER )
		return;

	SetPSConstantFlag( index );

	mPSConstantValue[ index ] = Vector4( constant, 1.0f );
}

_void ShaderConst::SetPixelShaderConstant( _dword index, const Vector4& constant )
{
	if ( index >= _MAX_CONSTANT_NUMBER )
		return;

	SetPSConstantFlag( index );

	mPSConstantValue[ index ] = constant;
}

_void ShaderConst::SetPixelShaderConstant( _dword index, const Color& constant )
{
	if ( index >= _MAX_CONSTANT_NUMBER )
		return;

	SetPSConstantFlag( index );

	mPSConstantValue[ index ] = Vector4( constant.r, constant.g, constant.b, constant.a );
}