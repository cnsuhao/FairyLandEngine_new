//============================================================================
// IShaderConst.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IShaderConst
//----------------------------------------------------------------------------

class IShaderConst
{
public:
	virtual _void	SetVertexShaderConstant( _dword index, _dword parametertype ) = 0;
	virtual _void	SetVertexShaderConstant( _dword index, const Vector3& constant ) = 0;
	virtual _void	SetVertexShaderConstant( _dword index, const Vector4& constant ) = 0;
	virtual _void	SetVertexShaderConstant( _dword index, const Quaternion& constant ) = 0;
	virtual _void	SetVertexShaderConstant( _dword index, const Matrix4& constant ) = 0;

	virtual _void	SetPixelShaderConstant( _dword index, const Vector3& constant ) = 0;
	virtual _void	SetPixelShaderConstant( _dword index, const Vector4& constant ) = 0;
	virtual _void	SetPixelShaderConstant( _dword index, const Color& constant ) = 0;
};

};