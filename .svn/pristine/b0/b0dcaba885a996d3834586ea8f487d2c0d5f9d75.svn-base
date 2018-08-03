//============================================================================
// IVertexAnimation.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IVertexAnimation
//----------------------------------------------------------------------------

class IVertexAnimation : public IAnimation
{
public:
	virtual IMesh*		GetAttachedMesh( ) const									= 0;

	virtual _bool		SetPositionKeyframe( _float time, const Vector3& position )	= 0;
	virtual _bool		SetNormalKeyframe( _float time, const Vector3& normal )		= 0;
	virtual _bool		SetDiffuseKeyframe( _float time, _dword diffuse )			= 0;
	virtual _bool		SetSpecularKeyframe( _float time, _dword specular )			= 0;
};

};