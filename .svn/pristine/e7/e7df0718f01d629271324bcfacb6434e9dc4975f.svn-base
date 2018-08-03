//============================================================================
// ParticleSystem.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// ParticleSystem
//----------------------------------------------------------------------------

class ParticleSystem : public IParticleSystem, public ReferencedObject
{
	friend ParticleEmitter;
	friend ParticleGravity;
	friend ParticleBlocker;

private:
	typedef Array< ParticleEmitter* >	ParticleEmitterArray;
	typedef Array< ParticleGravity* >	ParticleGravityArray;
	typedef Array< ParticleUpdater* >	ParticleUpdaterArray;
	typedef Array< ParticleBlocker* >	ParticleBlockerArray;

	String					mName;

	_dword					mVersion;
	_dword					mEmitterIDCreator;
	_dword					mGravityIDCreator;
	_dword					mUpdaterIDCreator;
	_dword					mBlockerIDCreator;

	ParticleEmitterArray	mEmitterArray;
	ParticleGravityArray	mGravityArray;
	ParticleUpdaterArray	mUpdaterArray;
	ParticleBlockerArray	mBlockerArray;

	ParticleEmitter* SearchEmitter( _dword emitterid ) const;
	ParticleGravity* SearchGravity( _dword gravityid ) const;
	ParticleUpdater* SearchUpdater( _dword updaterid ) const;
	ParticleBlocker* SearchBlocker( _dword blockerid ) const;

	_bool					mUpdateAABB;
	AxisAlignedBox			mBoundBox;

	_dword					mStop;
	
	_dword					mParticleGroup;

	_float					mRenderAlpha;
	// For debug
	_dword					mShowDirection;
	_dword					mUserData;

public:
	ParticleSystem( StringPtr name );
	~ParticleSystem( );

	ParticleSystem* Clone( _dword particlegroup ) const;

	virtual StringPtr GetName( ) const;

	virtual _void SetSpeed( _float speed );
	virtual _void SetScale( _float scale );

	virtual _bool LoadParticleSystem( TxtFile& pfxfile, _dword particlegroup );
	virtual _void Update( _dword elapse );

	virtual _dword GetVersion( ) const;
	virtual _dword GetParticleNumber( ) const;
	virtual _void  ClearAllParticles( );
	virtual _dword GetCurrentTickcount( ) const;
	virtual _dword GetEndTickcount( ) const;
	virtual _bool  IsFinished( ) const;

	virtual _dword CreateEmitter( _dword emitterid );
	virtual _dword CreateGravity( _dword gravityid );
	virtual _dword CreateUpdater( _dword updaterid );
	virtual _dword CreateBlocker( _dword blockerid );

	virtual _void RemoveEmitter( _dword emitterid );
	virtual _void RemoveGravity( _dword gravityid );
	virtual _void RemoveUpdater( _dword updaterid );
	virtual _void RemoveBlocker( _dword blockerid );

	virtual _dword GetEmitterNumber( ) const;
	virtual _dword GetEmitterID( _dword index ) const;
	virtual _dword GetGravityNumber( ) const;
	virtual _dword GetGravityID( _dword index ) const;
	virtual _dword GetUpdaterNumber( ) const;
	virtual _dword GetUpdaterID( _dword index ) const;
	virtual _dword GetBlockerNumber( ) const;
	virtual _dword GetBlockerID( _dword index ) const;

	virtual _bool SetMaxParticleNumber( _dword emitterid, _dword maxparticlenumber );
	virtual _bool GetMaxParticleNumber( _dword emitterid, _dword& maxparticlenumber ) const;
	virtual _bool SetParticleDelay( _dword emitterid, _dword delaymin, _dword delaymax );
	virtual _bool GetParticleDelay( _dword emitterid, _dword& delaymin, _dword& delaymax ) const;
	virtual _bool SetParticleLife( _dword emitterid, _dword lifemin, _dword lifemax );
	virtual _bool GetParticleLife( _dword emitterid, _dword& lifemin, _dword& lifemax ) const;
	virtual _bool SetParticleSpeed( _dword emitterid, _float speedmin, _float speedmax );
	virtual _bool GetParticleSpeed( _dword emitterid, _float& speedmin, _float& speedmax ) const;
	virtual _bool SetParticleSpin( _dword emitterid, _float spinmin, _float spinmax );
	virtual _bool GetParticleSpin( _dword emitterid, _float& spinmin, _float& spinmax ) const;
	virtual _bool SetParticleColor( _dword emitterid, _bool fixrate, const Color& colormin, const Color& colormax );
	virtual _bool GetParticleColor( _dword emitterid, _bool& fixrate, Color& colormin, Color& colormax ) const;
	virtual _bool SetParticleSize( _dword emitterid, _bool fixrate, const Vector3& sizemin, const Vector3& sizemax );
	virtual _bool GetParticleSize( _dword emitterid, _bool& fixrate, Vector3& sizemin, Vector3& sizemax ) const;
	virtual _bool SetParticleBindEmitter( _dword emitterid, _dword bindemitterid );
	virtual _bool GetParticleBindEmitter( _dword emitterid, _dword& bindemitterid ) const;
	virtual _bool SetParticleBindGravity( _dword emitterid, _dword bindgravityid );
	virtual _bool GetParticleBindGravity( _dword emitterid, _dword& bindgravityid ) const;
	virtual	_bool SetParticleRotation( _dword emitterid, const Vector3& rotation, const Vector3& rotatespeed );
	virtual _bool GetParticleRotation( _dword emitterid, Vector3& rotation, Vector3& rotatespeed ) const;

	virtual _bool SetParticleTexture( _dword emitterid, StringPtr name );
	virtual _bool GetParticleTexture( _dword emitterid, String& name, ITexture*& texture ) const;

	virtual _bool SetParticleSkeleton( _dword emitterid, StringPtr name );
	virtual _bool GetParticleSkeleton( _dword emitterid, String& name, ISkeleton*& skeleton ) const;
	virtual _bool SetParticleMesh( _dword emitterid, StringPtr name );
	virtual _bool GetParticleMesh( _dword emitterid, String& name, IMesh*& mesh ) const;
	virtual _bool SetParticleSkeletonAnimation( _dword emitterid, StringPtr name );
	virtual _bool GetParticleSkeletonAnimation( _dword emitterid, String& name, ISkeletonAnimation*& animation ) const;
	
	virtual _bool SetEmitterName( _dword emitterid, StringPtr name );		
	virtual StringPtr GetEmitterName( _dword emitterid ) const;
	virtual _bool SetEmitterEndTickcount( _dword emitterid, _dword endtick );
	virtual _bool GetEmitterEndTickcount( _dword emitterid, _dword& endtick ) const;
	virtual _bool SetEmitterType( _dword emitterid, _dword emittertype, _dword directiontype, _dword normaltype );
	virtual _bool GetEmitterType( _dword emitterid, _dword& emittertype, _dword& directiontype, _dword& normaltype ) const;
	virtual _bool SetEmitterParticleType( _dword emitterid, _dword type );
	virtual _bool GetEmitterParticleType( _dword emitterid, _dword& type ) const;
	virtual _bool SetEmitterEBD( _dword emitterid, _float ebd );
	virtual _bool GetEmitterEBD( _dword emitterid, _float& ebd ) const;
	virtual _bool SetEmitterRate( _dword emitterid, _dword emitrate, _dword emitdelay, _dword emitnumber );
	virtual _bool GetEmitterRate( _dword emitterid, _dword& emitrate, _dword& emitdelay, _dword& emitnumber ) const;
	virtual _bool SetEmitterSize( _dword emitterid, const Vector3& sizeinner, const Vector3& sizeouter );
	virtual _bool GetEmitterSize( _dword emitterid, Vector3& sizeinner, Vector3& sizeouter ) const;
	virtual _bool SetEmitterPosition( _dword emitterid, const Vector3& position );
	virtual _bool GetEmitterPosition( _dword emitterid, Vector3& position ) const;
	virtual _bool SetEmitterDirection( _dword emitterid, const EulerAngles& rotation );
	virtual _bool GetEmitterDirection( _dword emitterid, EulerAngles& rotation ) const;
	virtual _bool SetEmitterTextureFrame( _dword emitterid, _dword column, _dword row, _dword time, _dword random );		
	virtual _bool GetEmitterTextureFrame( _dword emitterid, _dword& column, _dword& row, _dword& time, _dword& random ) const;
	virtual _bool SetEmitterMesh( _dword emitterid, IMesh* mesh );
	virtual _bool GetEmitterMesh( _dword emitterid, IMesh*& mesh ) const;
	virtual _bool SetEmitterGeometry( _dword emitterid, const Geometry& geometry );
	virtual _bool GetEmitterGeometry( _dword emitterid, Geometry& geometry ) const;
	virtual _bool SetEmitterSkeleton( _dword emitterid, ISkeleton* skeleton );
	virtual _bool GetEmitterSkeleton( _dword emitterid, ISkeleton*& skeleton ) const;
	virtual _bool SetEmitterUVInvert( _dword emitterid, _dword invert );
	virtual _bool GetEmitterUVInvert( _dword emitterid, _dword& invert ) const;

	virtual _float GetEmitterLength( _dword emitterid );

	virtual _bool SetEmitterRenderMethod( _dword emitterid, IRenderMethod* rendermethod );
	virtual IRenderMethod* GetEmitterRenderMethod( _dword emitterid );

	virtual _dword GetEmitterParticleNumber( _dword emitterid ) const;
	virtual _void  ClearEmitterParticles( _dword emitterid );

	virtual _bool EnableEmitter( _dword emitterid );
	virtual _bool DisableEmitter( _dword emitterid );
	virtual _bool IsEmitterEnabled( _dword emitterid );

	virtual ITagPoint* CreateTagPoint( _dword gravityid );
	virtual _bool SetEmitterTagPoint( _dword emitterid, _dword tagpointindex, ITagPoint* tagpoint );

	virtual _bool SetGravityType( _dword gravityid, _dword type );
	virtual _bool GetGravityType( _dword gravityid, _dword& type ) const;
	virtual _bool SetGravityPower( _dword gravityid, _float power, _float distance );
	virtual _bool GetGravityPower( _dword gravityid, _float& power, _float& distance ) const;
	virtual _bool SetGravityPosition( _dword gravityid, const Vector3& position );
	virtual _bool GetGravityPosition( _dword gravityid, Vector3& position ) const;
	virtual _bool SetGravityDirection( _dword gravityid, const Vector3& direction );
	virtual _bool SetGravityDirection( _dword gravityid, const EulerAngles& rotation );
	virtual _bool GetGravityDirection( _dword gravityid, Vector3& direction ) const;

	virtual _bool EnableGravity( _dword gravityid );
	virtual _bool DisableGravity( _dword gravityid );
	virtual _bool IsGravityEnabled( _dword gravityid );

	virtual _bool EnableUpdater( _dword updaterid );	
	virtual _bool DisableUpdater( _dword updaterid );
	virtual _bool IsUpdaterEnabled( _dword updaterid );
	virtual _bool SetUpdaterType( _dword updaterid, _dword type );
	virtual _bool GetUpdaterType( _dword updaterid, _dword& type ) const;
	virtual _bool InsertUpdaterKeyframe( _dword updaterid, _float lifealpha, _float keyframe );
	virtual _bool RemoveUpdaterKeyframe( _dword updaterid, _float lifealpha );
	virtual _bool ClearUpdaterKeyframe( _dword updaterid );

	virtual _dword GetUpdaterKeyframeNumber( _dword updaterid ) const;
	virtual _bool  GetUpdaterKeyframe( _dword updaterid, _dword index, _float& lifealpha, _float& keyframe ) const;

	virtual _bool SetBlockerType( _dword blockerid, _dword type );
	virtual _bool GetBlockerType( _dword blockerid, _dword& type ) const;
	virtual _bool SetBlockerPlane( _dword blockerid, const Plane& plane );
	virtual _bool GetBlockerPlane( _dword blockerid, Plane& plane ) const;
	virtual _bool SetBlockerFunction( _dword blockerid, _dword function, _float parameter );
	virtual _bool GetBlockerFunction( _dword blockerid, _dword& function, _float& parameter ) const;

	virtual _bool AttachGravity( _dword emitterid, _dword gravityid );
	virtual _bool DetachGravity( _dword emitterid, _dword gravityid );
	virtual _bool AttachUpdater( _dword emitterid, _dword updaterid );
	virtual _bool DetachUpdater( _dword emitterid, _dword updaterid );
	virtual _bool AttachBlocker( _dword emitterid, _dword blockerid );
	virtual _bool DetachBlocker( _dword emitterid, _dword blockerid );

	virtual _dword GetAttachedGravityNumber( _dword emitterid ) const;
	virtual _dword GetAttachedGravityID( _dword emitterid, _dword index ) const;
	virtual _dword GetAttachedUpdaterNumber( _dword emitterid ) const;
	virtual _dword GetAttachedUpdaterID( _dword emitterid, _dword index ) const;
	virtual _dword GetAttachedBlockerNumber( _dword emitterid ) const;
	virtual _dword GetAttachedBlockerID( _dword emitterid, _dword index ) const;

	virtual _void SetBindBoneName( _dword emitterid, _dword bindindex, StringPtr name );
	virtual StringPtr GetBindBoneName( _dword emitterid, _dword bindindex ) const;
	virtual _bool DoAttachEmitterToTagPoint( _dword emitterid, ISkeleton* skeleton );

	virtual _bool GetUpdaterCurrentData( _dword updaterid, _float& data ) const;

	virtual _void RenderParticles( );

	virtual _void Stop( _dword duration );
	virtual _void Reset( );
	virtual _dword IsStop( ) const;

	virtual _bool SetTransOnEmit( _dword emitterid, _dword transonemit );
	virtual _bool GetTransOnEmit( _dword emitterid, _dword& transonemit ) const;
	virtual _bool SetTransOnMarker( _dword emitterid, _dword transonmarker );
	virtual _bool GetTransOnMarker( _dword emitterid, _dword& transonmarker ) const;

	virtual _void SetShowDirection( _dword showdirection );
	virtual _void GetShowDirection( _dword& showdirection ) const;

	virtual _void  SetUserData( _dword userdata );
	virtual _dword GetUserData( ) const;

	virtual _void  EnableUpdateAABB( _bool enable );
	virtual const AxisAlignedBox& GetBoundBox( ) const;

	virtual _void  SetRenderAlpha( _float alpha );

private:
	_bool SetParticleMeshRenderFlag( _dword emitterid, _dword renderflag );
};

};
