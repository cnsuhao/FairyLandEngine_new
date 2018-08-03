//============================================================================
// ParticlePlayer.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ParticlePlayer
//----------------------------------------------------------------------------

class ParticlePlayer : public IParticlePlayer, public ReferencedObject
{
private:
	struct ParticleDesc
	{
		IParticleSystem*	mParticleSystem;
		String				mParticleName;
		_dword				mParticleID;
		_dword				mColorPriority;
		Matrix4				mBindMatrix;
		_dword				mLifeLimit;
		_dword				mDelay;
		_float				mScale;
	};

	typedef Array< ParticleDesc >	ParticleArray;
	typedef Array< String >			StringArray;

	ParticleArray					mParticleArray;
	StringArray						mSearchResourcePath;
	Matrix4							mTransform;

	_dword							mUniqueID;

	_float							mScale;

	_bool							mUseOrtho;

	_dword							mTotalElapse;
public:
	ParticlePlayer( );
	~ParticlePlayer( );

public:
	virtual _dword Play( StringPtr filename, _dword priority, StringPtr name, _dword delay, 
		_dword lifelimit, ISkeleton* skeleton, IMesh* mesh, const Matrix4& bindmatrix, _float scale );
	virtual _dword Play( StringPtr filename, StringPtr name, const Matrix4& bindmatrix, _dword delay );
	virtual _dword Play( StringPtr filename, StringPtr name, const Geometry& geometry );

	virtual _void Stop( StringPtr name, _dword duration, _dword findall );
	virtual _void Stop( _dword id, _dword duration );
	virtual _void StopAll( _dword duration );

	virtual _void Update( _dword elapse, const Matrix4& transform );
	virtual _void Render( );

	virtual _bool  GetUserColor( Color& normalcolor );
	virtual _bool  GetUserAdditiveColor( Color& additivecolor );
	
	virtual _void  AppendResourcePath( StringPtr path );
	virtual _void  RemoveResourcePath( StringPtr path );
	
	virtual _dword GetParticleNumber( );

	virtual _dword GetParticleSystemNumber( ) const;
	virtual const IParticleSystem* GetParticleSystem( _dword index ) const;
	virtual const IParticleSystem* GetParticleSystemByID( _dword id ) const;
	virtual StringPtr GetParticleSystemName( _dword index ) const;

	virtual _dword SearchParticleID( StringPtr name, _bool carestopped = _true ) const;
	virtual _void  SetBindMatrixByID( _dword id, const Matrix4& matrix );

	virtual _void  SetUseOrtho( _bool enable );
	virtual _bool  GetUseOrtho( ) const;

	virtual _void  SetRenderAlpha( _float alpha );
};

};