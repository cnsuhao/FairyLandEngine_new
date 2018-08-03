//============================================================================
// IParticleSystem.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// IParticleSystem
//----------------------------------------------------------------------------

class IParticleSystem
{
public:
	enum _PARTICLE_TYPE
	{
		_PARTICLE_PRIMITIVEPOINT				= 1,
		_PARTICLE_PRIMITIVELINE					= 2,
		_PARTICLE_PRIMITIVERECT					= 3,
		_PARTICLE_PRIMITIVEPOINTSTRIP			= 4,
		_PARTICLE_PRIMITIVELINESTRIP			= 5,
		_PARTICLE_BILLBOARDSA					= 6,
		_PARTICLE_BILLBOARDWO					= 7,
		_PARTICLE_BILLBOARDAX					= 8,
		_PARTICLE_MESH							= 9,
		_PARTICLE_PRIMITIVEIMAGESTRIP			= 10,
		_PARTICLE_MESHCOLUMN					= 11,
		_PARTICLE_MESHSPHERE					= 12,
		_PARTICLE_PRIMITIVEQUADRANGLEIMAGE		= 13,
	};

	enum _EMITTER_TYPE
	{
		_EMITTER_POINT					= 1,
		_EMITTER_BOX					= 2,
		_EMITTER_CONE					= 3,
		_EMITTER_CIRCLE					= 4,
		_EMITTER_SPHERE					= 5,
		_EMITTER_HEMISPHERE				= 6,
		_EMITTER_MESH					= 7,
		_EMITTER_STATIC					= 8,
		_EMITTER_USER					= 9,
		_EMITTER_GEOMETRY				= 10,
		_EMITTER_STATICSKIN				= 11,
	};

	enum _DIRECTION_TYPE
	{
		_DIRECTION_SAMEASEMITTER		=  1,
		_DIRECTION_REVERSEFROMEMITTER	=  2,
		_DIRECTION_POINT_CENTRIPETAL	=  3,
		_DIRECTION_POINT_RADIAL			=  4,
		_DIRECTION_POINT_TANGENT		=  5,
		_DIRECTION_POINT_REVERSETANGENT	=  6,
		_DIRECTION_LINE_CENTRIPETAL		=  7,
		_DIRECTION_LINE_RADIAL			=  8,
		_DIRECTION_LINE_TANGENT			=  9,
		_DIRECTION_LINE_REVERSETANGENT	= 10,
		_DIRECTION_RANDOM				= 11,
	};

	enum _NORMAL_TYPE
	{
		_NORMAL_NOTCARE					= 1,
		_NORMAL_FORWARD					= 2,
		_NORMAL_BACKWARD				= 3,
		_NORMAL_OUTSIDE					= 4,
		_NORMAL_INSIDE					= 5,
		_NORMAL_RANDOM					= 6,
		_NORMAL_FIXED					= 7,
	};

	enum _GRAVITY_TYPE
	{
		_GRAVITY_POINT_CENTRIPETAL		=  1,
		_GRAVITY_POINT_RADIAL			=  2,
		_GRAVITY_POINT_CIRCLING			=  3,
		_GRAVITY_LINE_CENTRIPETAL		=  4,
		_GRAVITY_LINE_RADIAL			=  5,
		_GRAVITY_LINE_CIRCLING			=  6,
		_GRAVITY_PLANE_CENTRIPETAL		=  7,
		_GRAVITY_PLANE_RADIAL			=  8,
		_GRAVITY_PLANE_DIRECTIONAL		=  9,
		_GRAVITY_RANDOM					= 10,
	};

	enum _UPDATER_TYPE
	{
		_UPDATER_SPEED					= 1,
		_UPDATER_SPIN					= 2,
		_UPDATER_COLOR_R				= 3,
		_UPDATER_COLOR_G				= 4,
		_UPDATER_COLOR_B				= 5,
		_UPDATER_COLOR_A				= 6,
		_UPDATER_SIZE_X					= 7,
		_UPDATER_SIZE_Y					= 8,
		_UPDATER_SIZE_Z					= 9,
		_UPDATER_SIZE_XYZ				= 10,
		_UPDATER_USER					= 11,
	};

	enum _BLOCKER_TYPE
	{
		_BLOCKER_PLANE					= 1,
		_BLOCKER_BOX					= 2,
		_BLOCKER_SPHERE					= 3,
	};

	enum _BLOCK_FUNC
	{
		_BLOCK_NONE						= 1,
		_BLOCK_VANISH					= 2,
		_BLOCK_STOP						= 3,
		_BLOCK_REBOUND					= 4,
	};

public:
	virtual StringPtr GetName( ) const																							= 0;

	virtual _void SetSpeed( _float speed )																						= 0;
	virtual _void SetScale( _float scale )																						= 0;

	virtual _bool LoadParticleSystem( TxtFile& pfxfile, _dword particlegroup )													= 0;
	virtual _void Update( _dword elapse )																						= 0;

	virtual _dword GetVersion( ) const																							= 0;
	virtual _dword GetParticleNumber( ) const																					= 0;
	virtual _void  ClearAllParticles( )																							= 0;
	virtual _dword GetEndTickcount( ) const																						= 0;
	virtual _dword GetCurrentTickcount( ) const																					= 0;
	virtual _bool  IsFinished( ) const																							= 0;

	virtual _dword CreateEmitter( _dword emitterid = 0 )																		= 0;
	virtual _dword CreateGravity( _dword gravityid = 0 )																		= 0;
	virtual _dword CreateUpdater( _dword updaterid = 0 )																		= 0;
	virtual _dword CreateBlocker( _dword blockerid = 0 )																		= 0;

	virtual _void RemoveEmitter( _dword emitterid )																				= 0;
	virtual _void RemoveGravity( _dword gravityid )																				= 0;
	virtual _void RemoveUpdater( _dword updaterid )																				= 0;
	virtual _void RemoveBlocker( _dword blockerid )																				= 0;

	virtual _dword GetEmitterNumber( ) const																					= 0;
	virtual _dword GetEmitterID( _dword index ) const																			= 0;
	virtual _dword GetGravityNumber( ) const																					= 0;
	virtual _dword GetGravityID( _dword index ) const																			= 0;
	virtual _dword GetUpdaterNumber( ) const																					= 0;
	virtual _dword GetUpdaterID( _dword index ) const																			= 0;
	virtual _dword GetBlockerNumber( ) const																					= 0;
	virtual _dword GetBlockerID( _dword index ) const																			= 0;

	virtual _bool SetMaxParticleNumber( _dword emitterid, _dword maxparticlenumber )											= 0;
	virtual _bool GetMaxParticleNumber( _dword emitterid, _dword& maxparticlenumber ) const										= 0;
	virtual _bool SetParticleDelay( _dword emitterid, _dword delaymin, _dword delaymax )										= 0;
	virtual _bool GetParticleDelay( _dword emitterid, _dword& delaymin, _dword& delaymax ) const								= 0;
	virtual _bool SetParticleLife( _dword emitterid, _dword lifemin, _dword lifemax )											= 0;
	virtual _bool GetParticleLife( _dword emitterid, _dword& lifemin, _dword& lifemax ) const									= 0;
	virtual _bool SetParticleSpeed( _dword emitterid, _float speedmin, _float speedmax )										= 0;
	virtual _bool GetParticleSpeed( _dword emitterid, _float& speedmin, _float& speedmax ) const								= 0;
	virtual _bool SetParticleSpin( _dword emitterid, _float spinmin, _float spinmax )											= 0;
	virtual _bool GetParticleSpin( _dword emitterid, _float& spinmin, _float& spinmax ) const									= 0;
	virtual _bool SetParticleColor( _dword emitterid, _bool fixrate, const Color& colormin, const Color& colormax )				= 0;
	virtual _bool GetParticleColor( _dword emitterid, _bool& fixrate, Color& colormin, Color& colormax ) const					= 0;
	virtual _bool SetParticleSize( _dword emitterid, _bool fixrate, const Vector3& sizemin, const Vector3& sizemax )			= 0;
	virtual _bool GetParticleSize( _dword emitterid, _bool& fixrate, Vector3& sizemin, Vector3& sizemax ) const					= 0;
	virtual _bool SetParticleBindEmitter( _dword emitterid, _dword bindemitterid )												= 0;
	virtual _bool GetParticleBindEmitter( _dword emitterid, _dword& bindemitterid ) const										= 0;
	virtual _bool SetParticleBindGravity( _dword emitterid, _dword bindgravityid )												= 0;
	virtual _bool GetParticleBindGravity( _dword emitterid, _dword& bindgravityid ) const										= 0;
	virtual _bool SetParticleRotation( _dword emitterid, const Vector3& rotation, const Vector3& rotatespeed )					= 0;
	virtual _bool GetParticleRotation( _dword emitterid, Vector3& rotation, Vector3& rotatespeed ) const						= 0;

	virtual _bool SetParticleTexture( _dword emitterid, StringPtr name )														= 0;
	virtual _bool GetParticleTexture( _dword emitterid, String& name, ITexture*& texture ) const								= 0;

	virtual _bool SetParticleSkeleton( _dword emitterid, StringPtr name )														= 0;
	virtual _bool GetParticleSkeleton( _dword emitterid, String& name, ISkeleton*& skeleton ) const								= 0;
	virtual _bool SetParticleMesh( _dword emitterid, StringPtr name )															= 0;
	virtual _bool GetParticleMesh( _dword emitterid, String& name, IMesh*& mesh ) const											= 0;
	virtual _bool SetParticleSkeletonAnimation( _dword emitterid, StringPtr name )												= 0;
	virtual _bool GetParticleSkeletonAnimation( _dword emitterid, String& name, ISkeletonAnimation*& animation ) const			= 0;

	virtual _bool SetEmitterName( _dword emitterid, StringPtr name )															= 0;
	virtual StringPtr GetEmitterName( _dword emitterid ) const																	= 0;
	virtual _bool SetEmitterEndTickcount( _dword emitterid, _dword endtick )													= 0;
	virtual _bool GetEmitterEndTickcount( _dword emitterid, _dword& endtick ) const												= 0;
	virtual _bool SetEmitterType( _dword emitterid, _dword emittertype, _dword directiontype, _dword normaltype )				= 0;
	virtual _bool GetEmitterType( _dword emitterid, _dword& emittertype, _dword& directiontype, _dword& normaltype ) const		= 0;
	virtual _bool SetEmitterParticleType( _dword emitterid, _dword type )														= 0;
	virtual _bool GetEmitterParticleType( _dword emitterid, _dword& type ) const												= 0;
	virtual _bool SetEmitterEBD( _dword emitterid, _float ebd )																	= 0;
	virtual _bool GetEmitterEBD( _dword emitterid, _float& ebd ) const															= 0;
	virtual _bool SetEmitterRate( _dword emitterid, _dword emitrate, _dword emitdelay, _dword emitnumber )						= 0;
	virtual _bool GetEmitterRate( _dword emitterid, _dword& emitrate, _dword& emitdelay, _dword& emitnumber ) const				= 0;
	virtual _bool SetEmitterSize( _dword emitterid, const Vector3& sizeinner, const Vector3& sizeouter )						= 0;
	virtual _bool GetEmitterSize( _dword emitterid, Vector3& sizeinner, Vector3& sizeouter ) const								= 0;
	virtual _bool SetEmitterPosition( _dword emitterid, const Vector3& position )												= 0;
	virtual _bool GetEmitterPosition( _dword emitterid, Vector3& position ) const												= 0;
	virtual _bool SetEmitterDirection( _dword emitterid, const EulerAngles& rotation )											= 0;
	virtual _bool GetEmitterDirection( _dword emitterid, EulerAngles& rotation ) const											= 0;
	virtual _bool SetEmitterMesh( _dword emitterid, IMesh* mesh )																= 0;
	virtual _bool GetEmitterMesh( _dword emitterid, IMesh*& mesh ) const														= 0;
	virtual _bool SetEmitterGeometry( _dword emitterid, const Geometry& geometry )												= 0;
	virtual _bool GetEmitterGeometry( _dword emitterid, Geometry& geometry ) const												= 0;
	virtual _bool SetEmitterSkeleton( _dword emitterid, ISkeleton* skeleton )													= 0;
	virtual _bool GetEmitterSkeleton( _dword emitterid, ISkeleton*& skeleton ) const											= 0;
	virtual _bool SetEmitterTextureFrame( _dword emitterid, _dword column, _dword row, _dword time, _dword random )				= 0;
	virtual _bool GetEmitterTextureFrame( _dword emitterid, _dword& column, _dword& row, _dword& time, _dword& random ) const	= 0;
	virtual _bool SetEmitterUVInvert( _dword emitterid, _dword invert )															= 0;
	virtual _bool GetEmitterUVInvert( _dword emitterid, _dword& invert ) const													= 0;

	virtual _float GetEmitterLength( _dword emitterid )																			= 0;

	virtual _bool SetEmitterRenderMethod( _dword emitterid, IRenderMethod* rendermethod )										= 0;
	virtual IRenderMethod* GetEmitterRenderMethod( _dword emitterid )															= 0;

	virtual _dword GetEmitterParticleNumber( _dword emitterid ) const															= 0;
	virtual _void  ClearEmitterParticles( _dword emitterid )																	= 0;

	virtual _bool EnableEmitter( _dword emitterid )																				= 0;
	virtual _bool DisableEmitter( _dword emitterid )																			= 0;
	virtual _bool IsEmitterEnabled( _dword emitterid )																			= 0;

	virtual ITagPoint* CreateTagPoint( _dword gravityid )																		= 0;
	virtual _bool SetEmitterTagPoint( _dword emitterid, _dword tagpointindex, ITagPoint* tagpoint )								= 0;

	virtual _bool SetGravityType( _dword gravityid, _dword type )																= 0;
	virtual _bool GetGravityType( _dword gravityid, _dword& type ) const														= 0;
	virtual _bool SetGravityPower( _dword gravityid, _float power, _float distance )											= 0;
	virtual _bool GetGravityPower( _dword gravityid, _float& power, _float& distance ) const									= 0;
	virtual _bool SetGravityPosition( _dword gravityid, const Vector3& position )												= 0;
	virtual _bool GetGravityPosition( _dword gravityid, Vector3& position ) const												= 0;
	virtual _bool SetGravityDirection( _dword gravityid, const Vector3& direction )												= 0;
	virtual _bool SetGravityDirection( _dword gravityid, const EulerAngles& rotation )											= 0;
	virtual _bool GetGravityDirection( _dword gravityid, Vector3& direction ) const												= 0;

	virtual _bool EnableGravity( _dword gravityid )																				= 0;
	virtual _bool DisableGravity( _dword gravityid )																			= 0;
	virtual _bool IsGravityEnabled( _dword gravityid )																			= 0;

	virtual _bool EnableUpdater( _dword updaterid )																				= 0;
	virtual _bool DisableUpdater( _dword updaterid )																			= 0;
	virtual _bool IsUpdaterEnabled( _dword updaterid )																			= 0;
	virtual _bool SetUpdaterType( _dword updaterid, _dword type )																= 0;
	virtual _bool GetUpdaterType( _dword updaterid, _dword& type ) const														= 0;
	virtual _bool InsertUpdaterKeyframe( _dword updaterid, _float lifealpha, _float keyframe )									= 0;
	virtual _bool RemoveUpdaterKeyframe( _dword updaterid, _float lifealpha )													= 0;
	virtual _bool ClearUpdaterKeyframe( _dword updaterid )																		= 0;

	virtual _dword GetUpdaterKeyframeNumber( _dword updaterid ) const															= 0;
	virtual _bool  GetUpdaterKeyframe( _dword updaterid, _dword index, _float& lifealpha, _float& keyframe ) const				= 0;

	virtual _bool SetBlockerType( _dword blockerid, _dword type )																= 0;
	virtual _bool GetBlockerType( _dword blockerid, _dword& type ) const														= 0;
	virtual _bool SetBlockerPlane( _dword blockerid, const Plane& plane )														= 0;
	virtual _bool GetBlockerPlane( _dword blockerid, Plane& plane ) const														= 0;
	virtual _bool SetBlockerFunction( _dword blockerid, _dword function, _float parameter )										= 0;
	virtual _bool GetBlockerFunction( _dword blockerid, _dword& function, _float& parameter ) const								= 0;

	virtual _bool AttachGravity( _dword emitterid, _dword gravityid )															= 0;
	virtual _bool DetachGravity( _dword emitterid, _dword gravityid )															= 0;
	virtual _bool AttachUpdater( _dword emitterid, _dword updaterid )															= 0;
	virtual _bool DetachUpdater( _dword emitterid, _dword updaterid )															= 0;
	virtual _bool AttachBlocker( _dword emitterid, _dword blockerid )															= 0;
	virtual _bool DetachBlocker( _dword emitterid, _dword blockerid )															= 0;

	virtual _dword GetAttachedGravityNumber( _dword emitterid ) const															= 0;
	virtual _dword GetAttachedGravityID( _dword emitterid, _dword index ) const													= 0;
	virtual _dword GetAttachedUpdaterNumber( _dword emitterid ) const															= 0;
	virtual _dword GetAttachedUpdaterID( _dword emitterid, _dword index ) const													= 0;
	virtual _dword GetAttachedBlockerNumber( _dword emitterid ) const															= 0;
	virtual _dword GetAttachedBlockerID( _dword emitterid, _dword index ) const													= 0;

	virtual _void SetBindBoneName( _dword emitterid, _dword bindindex, StringPtr name )											= 0;
	virtual StringPtr GetBindBoneName( _dword emitterid, _dword bindindex ) const												= 0;
	virtual _bool DoAttachEmitterToTagPoint( _dword emitterid, ISkeleton* skeleton )											= 0;
	
	virtual _bool GetUpdaterCurrentData( _dword updaterid, _float& data ) const													= 0;

	virtual _void RenderParticles( )																							= 0;
	virtual _void Stop( _dword duration = 1000 )																				= 0;
	virtual _void Reset( )																										= 0;
	virtual _dword IsStop( ) const																								= 0;

	virtual _bool SetTransOnEmit( _dword emitterid, _dword transonemit )														= 0;
	virtual _bool GetTransOnEmit( _dword emitterid, _dword& transonemit ) const													= 0;
	virtual _bool SetTransOnMarker( _dword emitterid, _dword transonmarker )													= 0;
	virtual _bool GetTransOnMarker( _dword emitterid, _dword& transonmarker ) const												= 0;

	virtual _void SetShowDirection( _dword showdirection )																		= 0;
	virtual _void GetShowDirection( _dword& showdirection ) const																= 0;

	virtual _void  SetUserData( _dword userdata )																				= 0;
	virtual _dword GetUserData( ) const																							= 0;

	virtual _void  EnableUpdateAABB( _bool enable )																				= 0;
	virtual const AxisAlignedBox& GetBoundBox( ) const																			= 0;

	virtual _void  SetRenderAlpha( _float alpha )																				= 0;
};

};