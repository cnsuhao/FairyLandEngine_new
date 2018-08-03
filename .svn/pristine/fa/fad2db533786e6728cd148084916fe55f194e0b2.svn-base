//============================================================================
// ShaderConst.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ShaderConst
//----------------------------------------------------------------------------

class ShaderConst : public IShaderConst, public ReferencedObject
{
private:
	enum { _MAX_CONSTANT_NUMBER = 256 };

	_dword	mVSConstantFlag[ _MAX_CONSTANT_NUMBER ];
	Vector4	mVSConstantValue[ _MAX_CONSTANT_NUMBER ];

	_dword	mPSConstantFlag[ _MAX_CONSTANT_NUMBER ];
	Vector4	mPSConstantValue[ _MAX_CONSTANT_NUMBER ];

public:
	ShaderConst( );

	_void	EnableVSConst( ) const;
	_void	DisableVSConst( ) const;

	_void	EnablePSConst( ) const;
	_void	DisablePSConst( ) const;

	_void	SetVSConstantFlag( _dword index );
	_void	SetPSConstantFlag( _dword index );

	virtual _void	SetVertexShaderConstant( _dword index, _dword parametertype );
	virtual _void	SetVertexShaderConstant( _dword index, const Vector3& constant );
	virtual _void	SetVertexShaderConstant( _dword index, const Vector4& constant );
	virtual _void	SetVertexShaderConstant( _dword index, const Quaternion& constant );
	virtual _void	SetVertexShaderConstant( _dword index, const Matrix4& constant );

	virtual _void	SetPixelShaderConstant( _dword index, const Vector3& constant );
	virtual _void	SetPixelShaderConstant( _dword index, const Vector4& constant );
	virtual _void	SetPixelShaderConstant( _dword index, const Color& constant );
};

};