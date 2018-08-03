//============================================================================
// IParticlePlayer.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IParticlePlayer
//----------------------------------------------------------------------------

class IParticlePlayer
{
public:
	virtual _dword Play( StringPtr filename, _dword proirity = 0, StringPtr name = _null, _dword delay = 0, _dword lifelimit = -1, 
		ISkeleton* skeleton = _null, IMesh* mesh = _null, const Matrix4& bindmatrix = Matrix4::cIdentity, _float scale = 1.0f ) = 0;
	virtual _dword Play( StringPtr filename, StringPtr name, const Matrix4& bindmatrix = Matrix4::cIdentity, _dword delay = 0 ) = 0;
	virtual _dword Play( StringPtr filename, StringPtr name, const Geometry& geometry ) = 0;

	virtual _void Stop( StringPtr name, _dword duration = 1000, _dword findall = 0 ) = 0;
	virtual _void Stop( _dword id, _dword duration = 1000 ) = 0;
	virtual _void StopAll( _dword duration = 1000 ) = 0;

	virtual _void Update( _dword elapse, const Matrix4& transform ) = 0;
	virtual _void Render( ) = 0;
 
	virtual _bool GetUserColor( Color& normalcolor ) = 0;
	virtual _bool GetUserAdditiveColor( Color& additivecolor ) = 0;
		
	virtual _void AppendResourcePath( StringPtr path ) = 0;
	virtual _void RemoveResourcePath( StringPtr path ) = 0;
	
	virtual _dword GetParticleNumber( ) = 0;

	virtual _dword GetParticleSystemNumber( ) const = 0;
	virtual const IParticleSystem* GetParticleSystem( _dword index ) const = 0;
	virtual const IParticleSystem* GetParticleSystemByID( _dword id ) const = 0;
	virtual StringPtr GetParticleSystemName( _dword index ) const = 0;

	virtual _dword SearchParticleID( StringPtr name, _bool carestopped = _true ) const = 0;
	virtual _void  SetBindMatrixByID( _dword id, const Matrix4& matrix ) = 0;

	virtual _void  SetUseOrtho( _bool enable ) = 0;
	virtual _bool  GetUseOrtho( ) const = 0;

	virtual _void  SetRenderAlpha( _float alpha ) = 0;
};

};