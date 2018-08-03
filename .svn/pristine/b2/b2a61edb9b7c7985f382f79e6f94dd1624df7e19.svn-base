//============================================================================
// ParticleSystem.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// ParticleSystem Implementation
//----------------------------------------------------------------------------

ParticleSystem::ParticleSystem( StringPtr name ) : mName( name )
{
	mVersion			= 0;
	mEmitterIDCreator	= 1;
	mGravityIDCreator	= 1;
	mUpdaterIDCreator	= 1;
	mBlockerIDCreator	= 1;
	mStop				= 0;
	mShowDirection		= 0;
	mUserData			= 0;

	mUpdateAABB			= _false;
	mBoundBox			= AxisAlignedBox::cInitBox;

	mRenderAlpha		= 1.0f;

	IncreaseRefCount( );
}

ParticleSystem::~ParticleSystem( )
{
	for ( _dword i = 0; i < mEmitterArray.Number( ); i ++ )
		delete mEmitterArray[i];

	for ( _dword j = 0; j < mGravityArray.Number( ); j ++ )
		delete mGravityArray[j];

	for ( _dword k = 0; k < mUpdaterArray.Number( ); k ++ )
		delete mUpdaterArray[k];

	for ( _dword l = 0; l < mBlockerArray.Number( ); l ++ )
		delete mBlockerArray[l];
}

ParticleSystem* ParticleSystem::Clone( _dword particlegroup ) const
{
	ParticleSystem* newparticlesystem = new ParticleSystem( mName );

	newparticlesystem->mVersion				= mVersion;
	newparticlesystem->mEmitterIDCreator	= mEmitterIDCreator;
	newparticlesystem->mGravityIDCreator	= mGravityIDCreator;
	newparticlesystem->mUpdaterIDCreator	= mUpdaterIDCreator;
	newparticlesystem->mBlockerIDCreator	= mBlockerIDCreator;

	newparticlesystem->mParticleGroup		= particlegroup;

	// Clone emitter.
	for ( _dword i = 0; i < mEmitterArray.Number( ); i ++ )
	{
		ParticleEmitter* newemitter = new ParticleEmitter( *mEmitterArray[i] );

		GetTechniqueFactory( ).CloneRenderMethod( newemitter->mRenderMethod, _true );
		
		newemitter->mAttachedTagPoint[0] = _null;
		newemitter->mAttachedTagPoint[1] = _null;

		GetModelFactory( ).CloneMesh( newemitter->mAttachedMesh, _true, _false, _null );
		GetModelFactory( ).CloneSkeleton( newemitter->mAttachedSkeleton, _true );
		
		newemitter->mAttachedGeometry = GetGeometryFactory( ).CloneGeometry( newemitter->mGeometry, _true );

		GetGeometryFactory( ).CloneGeometry( newemitter->mGeometry, _true );
		GetTextureFactory( ).CloneTexture( newemitter->mTexture, _true );

		// Copy all
		newemitter->mSkeleton = GetModelFactory( ).CloneSkeleton( newemitter->mSkeleton, _false );
		newemitter->mMesh = GetModelFactory( ).CloneMesh( newemitter->mMesh, _false, _false, newemitter->mSkeleton );
		newemitter->mSkeletonAnimation = GetModelFactory( ).CloneSkeletonAnimation( newemitter->mSkeletonAnimation, _false, newemitter->mSkeleton );

		newemitter->mParticleSystem = newparticlesystem;
		newparticlesystem->mEmitterArray.Append( newemitter );
	}

	// Clone gravity.
	for ( _dword j = 0; j < mGravityArray.Number( ); j ++ )
	{
		ParticleGravity* newgravity = new ParticleGravity( *mGravityArray[j] );

		newgravity->mAttachedTagPoint = _null;

		newgravity->mParticleSystem = newparticlesystem;
		newparticlesystem->mGravityArray.Append( newgravity );
	}

	// Clone updater.
	for ( _dword k = 0; k < mUpdaterArray.Number( ); k ++ )
	{
		ParticleUpdater* newupdater = new ParticleUpdater( *mUpdaterArray[k] );

		newparticlesystem->mUpdaterArray.Append( newupdater );
	}

	// Clone blocker.
	for ( _dword l = 0; l < mBlockerArray.Number( ); l ++ )
	{
		ParticleBlocker* newblocker = new ParticleBlocker( *mBlockerArray[l] );

		newblocker->mParticleSystem = newparticlesystem;
		newparticlesystem->mBlockerArray.Append( newblocker );
	}

	newparticlesystem->ClearAllParticles( );

	return newparticlesystem;
}

StringPtr ParticleSystem::GetName( ) const
{
	return mName;
}

_void ParticleSystem::SetSpeed( _float speed )
{
	for ( _dword i = 0; i < mEmitterArray.Number( ); i ++ )
		mEmitterArray[i]->mSpeed = speed;
}

_void ParticleSystem::SetScale( _float scale )
{
	for ( _dword i = 0; i < mEmitterArray.Number( ); i ++ )
		mEmitterArray[i]->mScale = scale;
}

_bool ParticleSystem::IsFinished( ) const
{
	if ( mStop )
		return _true;

	_dword currenttickcount = 0;
	_dword endtickcount = GetEndTickcount( );
	for ( _dword i = 0; i < mEmitterArray.Number( ); i ++ )
	{
		if ( mEmitterArray[i]->mEnabled )
			currenttickcount = mEmitterArray[i]->mCurrTickcount;
	}
	if ( currenttickcount > endtickcount )
		return _true;
	else
		return _false;
}

_dword ParticleSystem::GetCurrentTickcount( ) const
{
	_dword maxcurrenttickcount = 0;
	for ( _dword i = 0; i < mEmitterArray.Number( ); i ++ )
	{
		if ( mEmitterArray[i]->mCurrTickcount > maxcurrenttickcount )
			maxcurrenttickcount = mEmitterArray[i]->mCurrTickcount;
	}
	return maxcurrenttickcount;
}

_dword ParticleSystem::GetEndTickcount( ) const
{
	_dword maxendtickcount = 0;
	for ( _dword i = 0; i < mEmitterArray.Number( ); i ++ )
	{
		_dword currentmax = mEmitterArray[i]->mEndTickcount;
		// No next time
		if ( mEmitterArray[i]->mEmitRate == -1 )
		{
			currentmax = mEmitterArray[i]->mEmitDelay + Math::Max( mEmitterArray[i]->mMinParticleDelay, mEmitterArray[i]->mMaxParticleDelay ) + Math::Max( mEmitterArray[i]->mMinParticleLife, mEmitterArray[i]->mMaxParticleLife );
			currentmax += currentmax;
		}
		if ( currentmax > maxendtickcount )
			maxendtickcount = currentmax;
	}
	return maxendtickcount;
}

_bool ParticleSystem::GetUpdaterCurrentData( _dword updaterid, _float& data ) const
{
	data = 1.0f;

	ParticleUpdater* updater = SearchUpdater( updaterid );
	if ( updater == _null )
		return _false;

	if ( updater->mEnabled == _false )
		return _false;

	_float alpha = 0.0f;
	_bool hasuseremitter = _false;
	for ( _dword m = 0; m < mEmitterArray.Number( ); m ++ )
	{
		ParticleEmitter* emitter = mEmitterArray[m];

		if ( emitter->mEmitterType == IParticleSystem::_EMITTER_USER )
		{
			hasuseremitter = _true;
			if ( emitter->mCurrTickcount < emitter->mEmitDelay )
			{
				alpha = 0.0f;
				break;
			}
			
			alpha = ( emitter->mCurrTickcount - emitter->mEmitDelay ) 
				/ _float( emitter->mEndTickcount - emitter->mEmitDelay );
			
			if ( alpha > 1.0f )
				alpha = 1.0f;

			break;
		}
	}
	
	// No user emitter
	if ( !hasuseremitter )
	{
		for ( _dword m = 0; m < mEmitterArray.Number( ); m ++ )
		{
			ParticleEmitter* emitter = mEmitterArray[m];

			if ( emitter->mEnabled == _false )
				continue;
			else
			{
				alpha = emitter->mCurrTickcount / _float( GetEndTickcount( ) );
				if ( alpha > 1.0f )
					alpha = 1.0f;
				break;
			}
		}
		return _false;
	}

	if ( updater->mKeyframeArray.Number( ) < 2 )
		return _false;

	_dword i = 0, j = updater->mKeyframeArray.Number( ) - 1;

	if ( updater->mKeyframeArray[i].mObject1 != 0.0f )
		return _false;

	if ( updater->mKeyframeArray[j].mObject1 != 1.0f )
		return _false;

	// Find the keyframe by alpha.
	while ( i < j - 1 )
	{
		_dword k = ( i + j ) / 2;

		if ( alpha >= updater->mKeyframeArray[k].mObject1 )
			i = k;
		else
			j = k;
	}

	_float alpha1 = updater->mKeyframeArray[i].mObject1;
	_float value1 = updater->mKeyframeArray[i].mObject2;
	_float alpha2 = updater->mKeyframeArray[j].mObject1;
	_float value2 = updater->mKeyframeArray[j].mObject2;

	_float factor = ( alpha - alpha1 ) / ( alpha2 - alpha1 );
	data = value1 + ( value2 - value1 ) * factor;
	return _true;
}

ParticleEmitter* ParticleSystem::SearchEmitter( _dword emitterid ) const
{
	for ( _dword i = 0; i < mEmitterArray.Number( ); i ++ )
	{
		if ( mEmitterArray[i]->mEmitterID == emitterid )
			return mEmitterArray[i];
	}

	return _null;
}

ParticleGravity* ParticleSystem::SearchGravity( _dword gravityid ) const
{
	for ( _dword i = 0; i < mGravityArray.Number( ); i ++ )
	{
		if ( mGravityArray[i]->mGravityID == gravityid )
			return mGravityArray[i];
	}

	return _null;
}

ParticleUpdater* ParticleSystem::SearchUpdater( _dword updaterid ) const
{
	for ( _dword i = 0; i < mUpdaterArray.Number( ); i ++ )
	{
		if ( mUpdaterArray[i]->mUpdaterID == updaterid )
			return mUpdaterArray[i];
	}

	return _null;
}

ParticleBlocker* ParticleSystem::SearchBlocker( _dword blockerid ) const
{
	for ( _dword i = 0; i < mBlockerArray.Number( ); i ++ )
	{
		if ( mBlockerArray[i]->mBlockerID == blockerid )
			return mBlockerArray[i];
	}

	return _null;
}

_bool ParticleSystem::LoadParticleSystem( TxtFile& pfxfile, _dword particlegroup )
{
	mParticleGroup = particlegroup;

	pfxfile.SetDivision( L" " );

	String keystring, valuestring1, valuestring2, valuestring3, 
		valuestring4, valuestring5, valuestring6, valuestring7;

	// current loading
	_dword emitterid = -1;
	_dword updaterid = -1;
	_dword gravityid = -1;
	_dword blockerid = -1;
	_dword renderflag = -1;

	while ( pfxfile.ReadWord( keystring ) )
	{
		// read infomation
		pfxfile.ReadWordInLine( valuestring1 );
		valuestring1.Lowercase( );
		pfxfile.ReadWordInLine( valuestring2 );
		valuestring2.Lowercase( );
		pfxfile.ReadWordInLine( valuestring3 );
		valuestring3.Lowercase( );
		pfxfile.ReadWordInLine( valuestring4 );
		valuestring4.Lowercase( );
		pfxfile.ReadWordInLine( valuestring5 );
		valuestring5.Lowercase( );
		pfxfile.ReadWordInLine( valuestring6 );
		valuestring6.Lowercase( );
		pfxfile.ReadWordInLine( valuestring7 );
		valuestring7.Lowercase( );

		if ( keystring == L"[UserUpdater]" )
		{
			updaterid = CreateUpdater( Long( valuestring1 ) );
			SetUpdaterType( updaterid, IParticleSystem::_UPDATER_USER );
		}
		else if ( keystring == L"UserUpdaterKey" ) 
		{
			// limit to 20 keyframes
			_float key =  Float( valuestring1 ) + 0.005f;

			key = _long( key / 0.05f ) * 0.05f;

			InsertUpdaterKeyframe( updaterid, key, Float( valuestring2 ) );
		}
		else if ( keystring == L"[EmitterBegin]" )
		{
			if ( SearchEmitter( Long( valuestring1 ) ) == _null )
				emitterid = CreateEmitter( Long( valuestring1 ) );
			else
				emitterid = CreateEmitter( 0 );

			renderflag = -1;
		}
		// Load Emitter
		else if ( keystring == L"EmitterName" )
		{
			SetEmitterName( emitterid, valuestring1 );
		}
		else if ( keystring == L"EmitterType" )
		{
			SetEmitterType( emitterid, Long( valuestring1 ), Long( valuestring2 ), Long( valuestring3 ) );
		}
		else if ( keystring == L"EmitterEndTick" )
		{
			SetEmitterEndTickcount( emitterid, Long( valuestring1 ) );
		}
		else if ( keystring == L"EmitterEmitByDistance" )
		{
			SetEmitterEBD( emitterid, Float( valuestring1 ) );
		}
		else if ( keystring == L"EmitterRate" )
		{
			SetEmitterRate( emitterid, Long( valuestring1 ), Long( valuestring2 ), Long( valuestring3 ) );
		}
		else if ( keystring == L"EmitterMaxNumber" )
		{
			SetMaxParticleNumber( emitterid, Long( valuestring1 ) );
		}
		else if ( keystring == L"EmitterSize" )
		{
			SetEmitterSize( emitterid, 
				Vector3( Float( valuestring1 ), Float( valuestring2 ), Float( valuestring3 ) ),
				Vector3( Float( valuestring4 ), Float( valuestring5 ), Float( valuestring6 ) ) );
		}
		else if ( keystring == L"EmitterPosition" )
		{
			SetEmitterPosition( emitterid, Vector3( Float( valuestring1 ), Float( valuestring2 ), Float( valuestring3 ) ) );
		}
		else if ( keystring == L"EmitterDirection" )
		{
			SetEmitterDirection( emitterid, EulerAngles( Float( valuestring1 ), Float( valuestring2 ), Float( valuestring3 ) ) );
		}
		else if ( keystring == L"EmitTransformOnEmit" )
		{
			SetTransOnEmit( emitterid, Long( valuestring1 ) );
		}
		else if ( keystring == L"EmitUseMarkerTransform" )
		{
			SetTransOnMarker( emitterid, Long( valuestring1 ) );
		}
		else if ( keystring == L"EmitterUVInvert" )
		{
			SetEmitterUVInvert( emitterid, Long( valuestring1 ) );
		}
		// Load Paritcle
		else if ( keystring == L"EmitterRenderMethod" )
		{
			if ( valuestring1 == L"alpha" )
			{
				SetEmitterRenderMethod( emitterid, GetTechniqueFactory( ).CreateRenderMethod( L"cullmode:none|depthwrite:false|alphablend:normal" ) );
				if ( !SetParticleMeshRenderFlag( emitterid, Geometry::_RENDERFLAG_ALPHA ) )
					renderflag = Geometry::_RENDERFLAG_ALPHA;
			}
			else if ( valuestring1 == L"additive" )
			{
				SetEmitterRenderMethod( emitterid, GetTechniqueFactory( ).CreateRenderMethod( L"cullmode:none|depthwrite:false|alphablend:additive" ) );
				if ( !SetParticleMeshRenderFlag( emitterid, Geometry::_RENDERFLAG_ALPHA ) )
					renderflag = Geometry::_RENDERFLAG_ALPHA;
			}
			else if ( valuestring1 == L"null" )
			{
				SetEmitterRenderMethod( emitterid, _null );
			}
			else
			{
				SetEmitterRenderMethod( emitterid, GetTechniqueFactory( ).CreateRenderMethod( valuestring1 ) );
			}
		}
		else if ( keystring == L"ParticleRes" )
		{
			// Old version file
			//SetParticleResource( emitterid, Long( valuestring1 ), valuestring2 );
			SetEmitterParticleType( emitterid, Long( valuestring1 ) );
			String ext = StringFormatter::GetExtention( valuestring2 );
			if ( ext.CompareLowercase( L"msh" ) )
			{
				SetParticleMesh( emitterid, valuestring2 );
			}
			else
			{
				SetParticleTexture( emitterid, valuestring2 );
			}
		}
		else if ( keystring == L"EmitterParticleType" )
		{
			SetEmitterParticleType( emitterid, Long( valuestring1 ) );
		}
		else if ( keystring == L"ParticleResource" )
		{
			String ext = StringFormatter::GetExtention( valuestring1 );
			if ( ext.CompareLowercase( L"msh" ) || ext.CompareLowercase( L"skn" ) )
			{
				SetParticleMesh( emitterid, valuestring1 );
				if ( renderflag != -1 )
				{
					SetParticleMeshRenderFlag( emitterid, renderflag );
					renderflag = -1;
				}
			}
			else if ( ext.CompareLowercase( L"skl" ) )
			{
				SetParticleSkeleton( emitterid, valuestring1 );
			}
			else if ( ext.CompareLowercase( L"san" ) )
			{
				SetParticleSkeletonAnimation( emitterid, valuestring1 );
			}
			else
			{
				SetParticleTexture( emitterid, valuestring1 );
			}
		}
		else if ( keystring == L"ParticleFrame" )
		{
			SetEmitterTextureFrame( emitterid, Long( valuestring1 ), Long( valuestring2 ), Long( valuestring3 ), _bool( Dword( valuestring4 ) ) );
		}
		else if ( keystring == L"ParticleDelay" )
		{
			SetParticleDelay( emitterid, Long( valuestring1 ), Long( valuestring2 ) );
		}
		else if ( keystring == L"ParticleLife" )
		{
			SetParticleLife( emitterid, Long( valuestring1 ), Long( valuestring2 ) );
		}
		else if ( keystring == L"ParticleSpeed" )
		{
			SetParticleSpeed( emitterid, Float( valuestring1 ), Float( valuestring2 ) );
		}
		else if ( keystring == L"ParticleSpin" )
		{
			SetParticleSpin( emitterid, Float( valuestring1 ), Float( valuestring2 ) );
		}
		else if ( keystring == L"ParticleSize" )
		{
			SetParticleSize( emitterid, Long( valuestring1 ),
				Vector3( Float( valuestring2 ), Float( valuestring3 ), Float( valuestring4 ) ),
				Vector3( Float( valuestring5 ), Float( valuestring6 ), Float( valuestring7 ) ) );
		}
		else if ( keystring == L"ParticleColor" )
		{
			SetParticleColor( emitterid, Long( valuestring1 ),
				Color( Float( valuestring2 ), Float( valuestring3 ), Float( valuestring4 ) ),
				Color( Float( valuestring5 ), Float( valuestring6 ), Float( valuestring7 ) ) );
		}
		else if ( keystring == L"ParticleUserRotation" )
		{
			SetParticleRotation( emitterid, 
				Vector3( Float( valuestring1 ), Float( valuestring2 ), Float( valuestring3 ) ),
				Vector3( Float( valuestring4 ), Float( valuestring5 ), Float( valuestring6 ) ) );
		}
		// Load updater 
		else if ( keystring == L"[UpdaterBegin]" )
		{
			updaterid = CreateUpdater( 0 );
			AttachUpdater( emitterid, updaterid );
		}
		else if ( keystring == L"UpdaterType" )
		{
			SetUpdaterType( updaterid, Long( valuestring1 ) );
		}
		else if ( keystring == L"UpdaterKey" )
		{
			// limit to 20 keyframes
			_float key =  Float( valuestring1 ) + 0.005f;

			key = _long( key / 0.05f ) * 0.05f;

			InsertUpdaterKeyframe( updaterid, key, Float( valuestring2 ) );
		}
		// Load gravity
		else if ( keystring == L"[GravityBegin]" )
		{
			gravityid = CreateGravity( 0 );
			AttachGravity( emitterid, gravityid );
		}
		else if ( keystring == L"GravityType" )
		{
			SetGravityType( gravityid, Long( valuestring1 ) );
		}
		else if ( keystring == L"GravityPower" )
		{
			SetGravityPower( gravityid, Float( valuestring1 ), Float( valuestring2 ) );
		}
		else if ( keystring == L"GravityPosition" )
		{
			SetGravityPosition( gravityid, Vector3( Float( valuestring1 ), Float( valuestring2 ), Float( valuestring3 ) ) );
		}
		else if ( keystring == L"GravityDirection" )
		{
			SetGravityDirection( gravityid, Vector3( Float( valuestring1 ), Float( valuestring2 ), Float( valuestring3 ) ) );
		}
		// Load Blocker
		else if ( keystring == L"[BlockerBegin]" )
		{
			blockerid = CreateBlocker( 0 );
			AttachBlocker( emitterid, blockerid );
		}
		else if ( keystring == L"BlockerType" )
		{
			SetBlockerType( blockerid, Long( valuestring1 ) );
		}
		else if ( keystring == L"BlockerFunction" )
		{
			SetBlockerFunction( blockerid, Long( valuestring1 ), Float( valuestring2 ) );
		}
		else if ( keystring == L"BlockerPlane" )
		{
			SetBlockerPlane( blockerid, Plane( Vector3( 0.0f, 0.0f, Float( valuestring1 ) ), Vector3( 0.0f, 0.0f, 1.0f ) ) );
		}
		// Load Bind
		else if ( keystring == L"BindBoneName" )
		{
			SetBindBoneName( emitterid, Long( valuestring1 ), valuestring2 );
		}
		else if ( keystring == L"BindEmitterId" )
		{
			SetParticleBindEmitter( emitterid, Long( valuestring1 ) );
		}
	}

	return emitterid != -1;
}

_void ParticleSystem::Update( _dword elapse )
{
	if ( elapse == 0 )
		return;

	_float floatelapse = elapse / 1000.0f;

	// Affect tag point by gravities.
	for ( _dword i = 0; i < mGravityArray.Number( ); i ++ )
	{
		ParticleGravity* gravity = mGravityArray[i];

		if ( gravity->mAttachedTagPoint == _null )
			continue;

		// TODO : NOT CLEAR HOW TO USE TRANSFORM 
		gravity->AffectTagPoint( elapse, 0 );
	}

	mBoundBox = AxisAlignedBox::cInitBox;

	// Porcess all global emitters.
	for ( _dword i = 0; i < mEmitterArray.Number( ); i ++ )
	{
		ParticleEmitter* emitter = mEmitterArray[i];

		// If move fast we need reduce rate
		_dword backuprate = emitter->mEmitRate;
		if ( emitter->mEmitterType == IParticleSystem::_EMITTER_GEOMETRY )
		{
			if ( emitter->mAttachedGeometry.mVertexBuffer != _null && emitter->mAttachedGeometry.mVertexSize != 0 )
			{	
				
				Vector3* currentposition = (Vector3*) emitter->mAttachedGeometry.mVertexBuffer->Lock( emitter->mAttachedGeometry.mVertexSize, emitter->mAttachedGeometry.mVertexSize );
				if ( emitter->mLastEmitPosition != Vector3::cMaxVector )
				{
					_float emittermovespeed = ( emitter->mLastEmitPosition - *currentposition ).Magnitude( ) / elapse;
					if ( emittermovespeed > 0.05f )
						emitter->mEmitRate = emitter->mEmitRate / 3;
					emitter->mLastEmitPosition = *currentposition;
				}
				else
				{
					emitter->mLastEmitPosition = *currentposition;
				}
				emitter->mAttachedGeometry.mVertexBuffer->Unlock( );
			}
		}

		// Get direction and distance
		Vector3 EBDdirection = Vector3::cZAxis;
		_float	EBDdistance  = 0.0f; 
		if ( emitter->mEmitByDistance != 0.0f )
		{
			Particle newparticle;
			emitter->CreateParticle( newparticle );

			if ( emitter->mLastEmitPosition == Vector3::cMaxVector )
				emitter->mLastEmitPosition = newparticle.mPosition;

			EBDdirection = newparticle.mPosition - emitter->mLastEmitPosition;
			EBDdistance  = EBDdirection.Magnitude( );
			EBDdirection.Normalize( );
		}
								
		// Update emitter tickcount.
		emitter->mLastTickcount = emitter->mCurrTickcount;
		_dword elapsespeed = _dword( elapse * emitter->mSpeed + 0.5f );
		emitter->mCurrTickcount = emitter->mCurrTickcount + elapsespeed;

		_dword number = emitter->GetAvailableParticleNumber( );

		// Resume delay
		emitter->mEmitRate = backuprate;

		// Create new particles.
		for ( _dword j = 0; j < number; j ++ )
		{
			Particle newparticle;
			emitter->CreateParticle( newparticle );

			// On track resetposition
			if ( emitter->mEmitByDistance != 0.0f )
			{
				// Last no need to set, only record last position
				if ( j == number - 1 )
				{
					emitter->mLastEmitPosition = newparticle.mPosition;
				}
				else
				{
					_float alpha = _float( j + 1 ) / _float( number );
					_float distance = EBDdistance * alpha;
										
					newparticle.mPosition = emitter->mLastEmitPosition + EBDdirection * distance;
				}
			}

			_dword particleid = emitter->mParticlePool.Create( newparticle ) + 1;

			Particle* lastparticle = emitter->mParticlePool.Index( emitter->mLastParticleID - 1 );
			if ( lastparticle != _null )
				lastparticle->mPrevParticleID = particleid;

			emitter->mLastParticleID = particleid;
		}

		// Update emitter mesh
		if ( emitter->mParticlePool.Number( ) != 0 )
		{
			if ( emitter->mSkeletonAnimation != _null && emitter->mSkeleton != _null && emitter->mMesh != _null )
			{
				emitter->mSkeletonAnimation->Update( elapse );
				emitter->mSkeleton->Update( );
				emitter->mMesh->Update( );
			}
			else if ( emitter->mMesh != _null && emitter->mEmitterType == IParticleSystem::_EMITTER_STATICSKIN )
			{
				emitter->mMesh->Update( );
			}
		}

		// Update emitter 2d frame for image strip
		if ( emitter->mColumn != 1 || emitter->mRow != 1 )
		{
			if ( emitter->mCurrTickcount > emitter->mLastFrameTickcount + emitter->mTime ) 
			{
				emitter->mLastFrameTickcount = emitter->mCurrTickcount;
				emitter->mCurrRow ++;
				// End a row
				if ( emitter->mCurrRow >= emitter->mRow )
				{
					emitter->mCurrRow = 0;
					emitter->mCurrColumn ++;
					// Again
					if ( emitter->mCurrColumn >= emitter->mColumn )
						emitter->mCurrColumn = 0;
				}
			}
		}

		// Update existing particle.
		for ( ParticleEmitter::ParticlePool::Iterator it = emitter->mParticlePool.GetHeadIterator( ); it.Valid( ); it ++ )
		{
			Particle& particle = it;

			// Some delay particle
			if ( particle.mDelay > elapse )
			{
				particle.mDelay -= elapse;
				continue;
			}
			else
			{
				particle.mDelay = 0;
			}

			if ( particle.mParticleType == IParticleSystem::_PARTICLE_PRIMITIVEIMAGESTRIP )
				emitter->mLength = -1;
			
			// Update particle tickcount.
			particle.mLastTickcount = particle.mCurrTickcount;
			particle.mCurrTickcount = particle.mCurrTickcount + elapse;

			// Particle died.
			if ( particle.mCurrTickcount >= particle.mLife )
			{
				emitter->mParticlePool.Remove( it );

				continue;
			}

			// Update the particle.
			for ( _dword i = 0; i < emitter->mUpdaterArray.Number( ); i ++ )
			{
				ParticleUpdater* updater = SearchUpdater( emitter->mUpdaterArray[i] );
				if ( updater == _null )
					continue;

				_float alpha = (_float) particle.mCurrTickcount / (_float) particle.mLife;

				updater->UpdateParticle( particle, alpha );
			}

			if ( mStop && particle.mStopTickCount != 0 )
			{
				Color stopcolor( particle.mColor );
				stopcolor.a *= ( 1.0f - _float( particle.mCurrTickcount - particle.mStopTickCount ) / _float( particle.mLife - particle.mStopTickCount ) );
				particle.mColor = stopcolor;
			}

			Vector3 oldposition = particle.mPosition;

			// Compute current column and row
			if ( particle.mParticleType != IParticleSystem::_PARTICLE_PRIMITIVEIMAGESTRIP )
			{
				if ( emitter->mColumn != 1 || emitter->mRow != 1 )
				{
					if ( particle.mCurrTickcount > particle.mLastFrameTickcount + emitter->mTime ) 
					{
						particle.mLastFrameTickcount = particle.mCurrTickcount;
						particle.mCurrRow ++;
						// End a row
						if ( particle.mCurrRow >= emitter->mRow )
						{
							particle.mCurrRow = 0;
							particle.mCurrColumn ++;
							// Again
							if ( particle.mCurrColumn >= emitter->mColumn )
								particle.mCurrColumn = 0;
						}
					}
				}
			}
			
			// Compute particle rotation offset by rotate speed
			particle.mUserRotation.x += emitter->mParticleRotateSpeed.x * floatelapse;
			particle.mUserRotation.y += emitter->mParticleRotateSpeed.y * floatelapse;
			particle.mUserRotation.z += emitter->mParticleRotateSpeed.z * floatelapse;

			// Compute particle move offset by direction and speed.
			Vector3 offset = particle.mDirection * particle.mSpeed * floatelapse;
			
			// Affect the particle by gravities.
			for ( _dword i = 0; i < emitter->mGravityArray.Number( ); i ++ )
			{
				ParticleGravity* gravity = SearchGravity( emitter->mGravityArray[i] );
				if ( gravity == _null )
					continue;

				gravity->AffectParticle( particle, elapse, emitter->mEmitWorldtransform );
			}

			// Move particle by speed and direction.
			if ( particle.mInternalFlags == 0 )
				particle.mPosition += offset;
			
			// Always equal to emitter position
			if ( emitter->mEmitterType == _EMITTER_STATIC )
			{
				if ( emitter->mAttachedTagPoint[0] )
				{
					particle.mPosition = emitter->mAttachedTagPoint[0]->GetPosition( );
					particle.mPosition += emitter->mPosition * Matrix4( ).Rotation( -Vector3::cYAxis, emitter->mAttachedTagPoint[0]->GetDirection( ) );

					particle.mDirection = emitter->mAttachedTagPoint[0]->GetDirection( );
				}
				else
				{
					particle.mPosition = emitter->mPosition;
				}
				
				Matrix4 worldtransform = GetRenderDevice( ).GetWorldTransform( );
				if ( emitter->mEmitWorldtransform == 0 )
					worldtransform = Matrix4::cIdentity;

				particle.mPosition *= worldtransform;
			}
			// Affect the particle by blockers.
			for ( _dword i = 0; i < emitter->mBlockerArray.Number( ); i ++ )
			{
				ParticleBlocker* blocker = SearchBlocker( emitter->mBlockerArray[i] );
				if ( blocker == _null )
					continue;

				blocker->BlockParticle( particle, oldposition, emitter->mEmitWorldtransform );
			}

			// Rotate particle by spin and direction.
			if ( particle.mSpin != 0.0 && particle.mNormal != Vector3::cOrigin )
			{
				float spin = particle.mSpin * ( (_float) elapse / 1000.0f );
				// Build a rotation matrix.
				Matrix4 rotation; rotation.Rotation( particle.mDirection, spin );

				// Rotate particle normal.
				particle.mNormal *= rotation;
			}
			
			// Clear internal flags.
			particle.mInternalFlags = 0;

			if ( mUpdateAABB )
			{
				Matrix4 worldtransform = GetRenderDevice( ).GetWorldTransform( );

				if ( emitter->mEmitWorldtransform )
					mBoundBox += particle.mPosition * worldtransform.Inverse( );
				else
					mBoundBox += particle.mPosition;
			}
		}
		// length is needed
		if ( emitter->mLength == -1 )
		{
			emitter->mLength = 0;
			for ( ParticleEmitter::ParticlePool::Iterator it = emitter->mParticlePool.GetHeadIterator( ); ; )
			{
				Particle& particle = it;
				it++;
				Particle& nextparticle = it;

				if ( !it.Valid( ) )
					break;

				emitter->mLength += ( particle.mPosition - nextparticle.mPosition ).Magnitude( );
			}
		}
	}
}

_dword ParticleSystem::GetVersion( ) const
{
	return mVersion;
}

_dword ParticleSystem::GetParticleNumber( ) const
{
	_dword particlenumber = 0;

	for ( _dword i = 0; i < mEmitterArray.Number( ); i ++ )
		particlenumber += mEmitterArray[i]->mParticlePool.Number( );

	return particlenumber;
}

_void ParticleSystem::ClearAllParticles( )
{
	for ( _dword i = 0; i < mEmitterArray.Number( ); i ++ )
	{
		ParticleEmitter* emitter = mEmitterArray[i];
		emitter->mParticlePool.Clear( );
		emitter->mCurrTickcount	 = 0;
		emitter->mLastTickcount	 = 0;
		emitter->mLastFrameTickcount = 0;
		emitter->mLastParticleID = 0;
		emitter->mLastEmitPosition = Vector3::cMaxVector;

		if ( emitter->mSkeletonAnimation != _null )
			emitter->mSkeletonAnimation->Reset( );
	}
}

_dword ParticleSystem::CreateEmitter( _dword emitterid )
{
	if ( emitterid == 0 )
	{
		mEmitterArray.Append( new ParticleEmitter( mEmitterIDCreator ) );
		mEmitterArray[mEmitterArray.Number( ) - 1]->mParticleSystem = this;
	
		return mEmitterIDCreator ++;
	}
	else
	{
		if ( SearchEmitter( emitterid ) != _null )
			return 0;
		else
		{
			// Make mEmitterIDCreator work
			if ( emitterid >= mEmitterIDCreator )
				mEmitterIDCreator = emitterid + 1;
			
			mEmitterArray.Append( new ParticleEmitter( emitterid ) );
		}

		mEmitterArray[mEmitterArray.Number( ) - 1]->mParticleSystem = this;

		return emitterid;
	}
}

_dword ParticleSystem::CreateGravity( _dword gravityid )
{
	mGravityArray.Append( new ParticleGravity( mGravityIDCreator ) );

	mGravityArray[ mGravityArray.Number( ) - 1 ]->mParticleSystem = this;

	return mGravityIDCreator ++;
}

_dword ParticleSystem::CreateUpdater( _dword updaterid )
{
	// make sure updaterid > mUpdaterid 
	if ( updaterid == 0 )
	{
		mUpdaterArray.Append( new ParticleUpdater( mUpdaterIDCreator ) );

		return mUpdaterIDCreator ++;
	}
	else
	{
		if ( SearchUpdater( updaterid ) != _null )
			return 0;
		else
		{
			// Make mUpdaterIDCreator work
			if ( updaterid >= mUpdaterIDCreator )
				mUpdaterIDCreator = updaterid + 1;

			mUpdaterArray.Append( new ParticleUpdater( updaterid ) );
		}
		return updaterid;
	}
}

_dword ParticleSystem::CreateBlocker( _dword blockerid )
{
	mBlockerArray.Append( new ParticleBlocker( mBlockerIDCreator ) );

	mBlockerArray[ mBlockerArray.Number( ) - 1 ]->mParticleSystem = this;

	return mBlockerIDCreator ++;
}

_void ParticleSystem::RemoveEmitter( _dword emitterid )
{
	for ( _dword i = 0; i < mEmitterArray.Number( ); i ++ )
	{
		if ( mEmitterArray[i]->mEmitterID == emitterid )
		{
			delete mEmitterArray[i];
			mEmitterArray.Remove( i );

			return;
		}
	}
}

_void ParticleSystem::RemoveGravity( _dword gravityid )
{
	for ( _dword i = 0; i < mGravityArray.Number( ); i ++ )
	{
		if ( mGravityArray[i]->mGravityID == gravityid )
		{
			delete mGravityArray[i];
			mGravityArray.Remove( i );

			return;
		}
	}
}

_void ParticleSystem::RemoveUpdater( _dword updaterid )
{
	for ( _dword i = 0; i < mUpdaterArray.Number( ); i ++ )
	{
		if ( mUpdaterArray[i]->mUpdaterID == updaterid )
		{
			delete mUpdaterArray[i];
			mUpdaterArray.Remove( i );

			return;
		}
	}
}

_void ParticleSystem::RemoveBlocker( _dword blockerid )
{
	for ( _dword i = 0; i < mBlockerArray.Number( ); i ++ )
	{
		if ( mBlockerArray[i]->mBlockerID == blockerid )
		{
			delete mBlockerArray[i];
			mBlockerArray.Remove( i );

			return;
		}
	}
}

_dword ParticleSystem::GetEmitterNumber( ) const
{
	return mEmitterArray.Number( );
}

_dword ParticleSystem::GetEmitterID( _dword index ) const
{
	if ( index >= mEmitterArray.Number( ) )
		return 0;

	return mEmitterArray[ index ]->mEmitterID;
}

_dword ParticleSystem::GetGravityNumber( ) const
{
	return mGravityArray.Number( );
}

_dword ParticleSystem::GetGravityID( _dword index ) const
{
	if ( index >= mGravityArray.Number( ) )
		return 0;

	return mGravityArray[ index ]->mGravityID;
}

_dword ParticleSystem::GetUpdaterNumber( ) const
{
	return mUpdaterArray.Number( );
}

_dword ParticleSystem::GetUpdaterID( _dword index ) const
{
	if ( index >= mUpdaterArray.Number( ) )
		return 0;

	return mUpdaterArray[ index ]->mUpdaterID;
}

_dword ParticleSystem::GetBlockerNumber( ) const
{
	return mBlockerArray.Number( );
}

_dword ParticleSystem::GetBlockerID( _dword index ) const
{
	if ( index >= mBlockerArray.Number( ) )
		return 0;

	return mBlockerArray[ index ]->mBlockerID;
}

_float ParticleSystem::GetEmitterLength( _dword emitterid )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return 0;

	return emitter->mLength;
}

_bool ParticleSystem::SetMaxParticleNumber( _dword emitterid, _dword maxparticlenumber )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	if ( emitter->mParticlePool.Size( ) != maxparticlenumber )
		emitter->mParticlePool.Resize( maxparticlenumber );

	return _true;
}

_bool ParticleSystem::GetMaxParticleNumber( _dword emitterid, _dword& maxparticlenumber ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	maxparticlenumber = emitter->mParticlePool.Size( );

	return _true;
}

_bool ParticleSystem::SetParticleDelay( _dword emitterid, _dword delaymin, _dword delaymax )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mMinParticleDelay = delaymin;
	emitter->mMaxParticleDelay = delaymax;

	return _true;
}

_bool ParticleSystem::GetParticleDelay( _dword emitterid, _dword& delaymin, _dword& delaymax ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	delaymin = emitter->mMinParticleDelay;
	delaymax = emitter->mMaxParticleDelay;

	return _true;
}

_bool ParticleSystem::SetParticleLife( _dword emitterid, _dword lifemin, _dword lifemax )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mMinParticleLife = lifemin;
	emitter->mMaxParticleLife = lifemax;

	return _true;
}

_bool ParticleSystem::GetParticleLife( _dword emitterid, _dword& lifemin, _dword& lifemax ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	lifemin = emitter->mMinParticleLife;
	lifemax = emitter->mMaxParticleLife;

	return _true;
}

_bool ParticleSystem::SetParticleSpeed( _dword emitterid, _float speedmin, _float speedmax )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mMinParticleSpeed = speedmin;
	emitter->mMaxParticleSpeed = speedmax;

	return _true;
}

_bool ParticleSystem::GetParticleSpeed( _dword emitterid, _float& speedmin, _float& speedmax ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	speedmin = emitter->mMinParticleSpeed;
	speedmax = emitter->mMaxParticleSpeed;

	return _true;
}

_bool ParticleSystem::SetParticleSpin( _dword emitterid, _float spinmin, _float spinmax )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mMinParticleSpin = spinmin;
	emitter->mMaxParticleSpin = spinmax;

	return _true;
}

_bool ParticleSystem::GetParticleSpin( _dword emitterid, _float& spinmin, _float& spinmax ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	spinmin = emitter->mMinParticleSpin;
	spinmax = emitter->mMaxParticleSpin;

	return _true;
}

_bool ParticleSystem::SetParticleColor( _dword emitterid, _bool fixrate, const Color& colormin, const Color& colormax )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mFixParticleColorRate	= fixrate;
	emitter->mMinParticleColor		= colormin;
	emitter->mMaxParticleColor		= colormax;

	return _true;
}

_bool ParticleSystem::GetParticleColor( _dword emitterid, _bool& fixrate, Color& colormin, Color& colormax ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	fixrate		= emitter->mFixParticleColorRate;
	colormin	= emitter->mMinParticleColor;
	colormax	= emitter->mMaxParticleColor;

	return _true;
}

_bool ParticleSystem::SetParticleSize( _dword emitterid, _bool fixrate, const Vector3& sizemin, const Vector3& sizemax )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mFixParticleSizeRate	= fixrate;
	emitter->mMinParticleSize		= sizemin;
	emitter->mMaxParticleSize		= sizemax;

	return _true;
}

_bool ParticleSystem::GetParticleSize( _dword emitterid, _bool& fixrate, Vector3& sizemin, Vector3& sizemax ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	fixrate	= emitter->mFixParticleSizeRate;
	sizemin	= emitter->mMinParticleSize;
	sizemax	= emitter->mMaxParticleSize;

	return _true;
}

_bool ParticleSystem::SetParticleRotation( _dword emitterid, const Vector3& rotation, const Vector3& rotatespeed )	
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mParticleRotation 		= rotation;
	emitter->mParticleRotateSpeed	= rotatespeed;

	return _true;
}

_bool ParticleSystem::GetParticleRotation( _dword emitterid, Vector3& rotation, Vector3& rotatespeed ) const		
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	rotation = emitter->mParticleRotation;
	rotatespeed = emitter->mParticleRotateSpeed;

	return _true;
}

_bool ParticleSystem::SetParticleTexture( _dword emitterid, StringPtr name )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	// Particle resouce ptr copy changed
	ClearEmitterParticles( emitterid );

	// Release old
	GetTextureFactory( ).ReleaseTexture( emitter->mTexture );
	
	// Create new
	if ( name != L"" )
		emitter->mTexture = GetTextureFactory( ).CreateTexture( name );

	return _true;
}

_bool ParticleSystem::GetParticleTexture( _dword emitterid, String& name, ITexture*& texture ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	if ( emitter->mTexture != _null )
	{
		name	= emitter->mTexture->GetName( );
		texture	= emitter->mTexture;
	}

	return _true;
}

_bool ParticleSystem::SetParticleSkeleton( _dword emitterid, StringPtr name )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	// Particle resouce ptr copy changed
	ClearEmitterParticles( emitterid );

	// Release old
	GetModelFactory( ).ReleaseSkeleton( emitter->mSkeleton );
	GetModelFactory( ).ReleaseMesh( emitter->mMesh );
	GetModelFactory( ).ReleaseSkeletonAnimation( emitter->mSkeletonAnimation );

	// Create new
	if ( name != L"" )
		emitter->mSkeleton = GetModelFactory( ).CreateSkeleton( name, _false );

	return _true;
}

_bool ParticleSystem::GetParticleSkeleton( _dword emitterid, String& name, ISkeleton*& skeleton ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	if ( emitter->mSkeleton != _null )
	{
		name	 = emitter->mSkeleton->GetName( );
		skeleton = emitter->mSkeleton;
	}

	return _true;
}

_bool ParticleSystem::SetParticleMesh( _dword emitterid, StringPtr name )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	// Particle resouce ptr copy changed
	ClearEmitterParticles( emitterid );

	// Release old
	GetModelFactory( ).ReleaseMesh( emitter->mMesh );
	
	// Create new
	if ( name != L"" )
	{
		if ( emitter->mEmitterType == IParticleSystem::_EMITTER_STATICSKIN )
		{
			if ( emitter->mAttachedSkeleton != _null && StringFormatter::GetExtention( name ) == L"skn" )
				emitter->mMesh = GetModelFactory( ).CreateMesh( emitter->mAttachedSkeleton, name );
			else
				emitter->mMesh = GetModelFactory( ).CreateMesh( name );
		}
		else
		{
			if ( emitter->mSkeleton != _null && StringFormatter::GetExtention( name ) == L"skn" )
				emitter->mMesh = GetModelFactory( ).CreateMesh( emitter->mSkeleton, name );
			else
				emitter->mMesh = GetModelFactory( ).CreateMesh( name );
		}
	}

	return _true;
}

_bool ParticleSystem::GetParticleMesh( _dword emitterid, String& name, IMesh*& mesh ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	if ( emitter->mMesh != _null )
	{
		name = emitter->mMesh->GetName( );
		mesh = emitter->mMesh;
	}

	return _true;
}

_bool ParticleSystem::SetParticleSkeletonAnimation( _dword emitterid, StringPtr name )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	// Particle resouce ptr copy changed
	ClearEmitterParticles( emitterid );

	// Release old
	GetModelFactory( ).ReleaseSkeletonAnimation( emitter->mSkeletonAnimation );

	// Create new
	if ( name != L"" )
	{
		if ( emitter->mSkeleton != _null )
			emitter->mSkeletonAnimation = GetModelFactory( ).CreateSkeletonAnimation( emitter->mSkeleton, name, _false );
	}

	return _true;
}

_bool ParticleSystem::GetParticleSkeletonAnimation( _dword emitterid, String& name, ISkeletonAnimation*& animation ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	if ( emitter->mSkeletonAnimation != _null )
	{
		name	  = emitter->mSkeletonAnimation->GetName( );
		animation = emitter->mSkeletonAnimation;
	}

	return _true;
}

_bool ParticleSystem::SetParticleBindEmitter( _dword emitterid, _dword bindemitterid )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mParticleBindEmitterID	= bindemitterid;

	return _true;
}

_bool ParticleSystem::GetParticleBindEmitter( _dword emitterid, _dword& bindemitterid ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	bindemitterid = emitter->mParticleBindEmitterID;

	return _true;
}

_bool ParticleSystem::SetParticleBindGravity( _dword emitterid, _dword bindgravityid )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mParticleBindGravityID	= bindgravityid;

	return _true;
}

_bool ParticleSystem::GetParticleBindGravity( _dword emitterid, _dword& bindgravityid ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	bindgravityid = emitter->mParticleBindGravityID;

	return _true;
}

_bool ParticleSystem::SetEmitterType( _dword emitterid, _dword emittertype, _dword directiontype, _dword normaltype )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mEmitterType	= emittertype;
	emitter->mDirectionType	= directiontype;
	emitter->mNormalType	= normaltype;

	return _true;
}

_bool ParticleSystem::SetEmitterParticleType( _dword emitterid, _dword type )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mParticleType = type;

	if ( emitter->mParticleType == IParticleSystem::_PARTICLE_MESHSPHERE )
		GetGeometryFactory( ).ReleaseGeometry( emitter->mGeometry );
	// create a geometry sphere
	if ( emitter->mParticleType == IParticleSystem::_PARTICLE_MESHSPHERE )
		emitter->mGeometry = GetGeometryFactory( ).CreateSphereGeometry( Geometry::_FORMATFLAG_POSITION | Geometry::_FORMATFLAG_DIFFUSE | Geometry::_FORMATFLAG_TEXCOORD1, 20, 30 );

	return _true;
}

_bool ParticleSystem::GetEmitterParticleType( _dword emitterid, _dword& type ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	type = emitter->mParticleType;

	return _true;
}

_bool ParticleSystem::GetEmitterEndTickcount( _dword emitterid, _dword& endtick ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	endtick = emitter->mEndTickcount;

	return _true;
}

_bool ParticleSystem::SetEmitterName( _dword emitterid, StringPtr name )		
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mName = name;

	return _true;
}

StringPtr ParticleSystem::GetEmitterName( _dword emitterid ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _null;

	return emitter->mName;
}

_bool ParticleSystem::SetEmitterEndTickcount( _dword emitterid, _dword endtick )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mEndTickcount = endtick;

	return _true;
}

_bool ParticleSystem::GetEmitterType( _dword emitterid, _dword& emittertype, _dword& directiontype, _dword& normaltype ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emittertype		= emitter->mEmitterType;
	directiontype	= emitter->mDirectionType;
	normaltype		= emitter->mNormalType;

	return _true;
}

_bool ParticleSystem::SetEmitterEBD( _dword emitterid, _float ebd )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mEmitByDistance = ebd;
	
	return _true;
}

_bool ParticleSystem::GetEmitterEBD( _dword emitterid, _float& ebd ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	ebd	= emitter->mEmitByDistance;
	
	return _true;
}

_bool ParticleSystem::SetEmitterRate( _dword emitterid, _dword emitrate, _dword emitdelay, _dword emitnumber )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mEmitRate		= emitrate;
	emitter->mEmitDelay		= emitdelay;
	emitter->mEmitNumber	= emitnumber;

	return _true;
}

_bool ParticleSystem::GetEmitterRate( _dword emitterid, _dword& emitrate, _dword& emitdelay, _dword& emitnumber ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitrate	= emitter->mEmitRate;
	emitdelay	= emitter->mEmitDelay;
	emitnumber	= emitter->mEmitNumber;

	return _true;
}

_bool ParticleSystem::SetEmitterSize( _dword emitterid, const Vector3& sizeinner, const Vector3& sizeouter )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mSizeInner	= sizeinner;
	emitter->mSizeOuter	= sizeouter;

	emitter->mSizeInner.Absolute( );
	emitter->mSizeOuter.Absolute( );

	return _true;
}

_bool ParticleSystem::GetEmitterSize( _dword emitterid, Vector3& sizeinner, Vector3& sizeouter ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	sizeinner	= emitter->mSizeInner;
	sizeouter	= emitter->mSizeOuter;

	return _true;
}

_bool ParticleSystem::SetEmitterPosition( _dword emitterid, const Vector3& position )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mPosition = position;

	return _true;
}

_bool ParticleSystem::GetEmitterPosition( _dword emitterid, Vector3& position ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	position = emitter->mPosition;

	return _true;
}

_bool ParticleSystem::SetEmitterDirection( _dword emitterid, const EulerAngles& rotation )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mRotation = rotation;
	emitter->mRotationMatrix.Rotation( rotation );

	return _true;
}

_bool ParticleSystem::SetEmitterTextureFrame( _dword emitterid, _dword column, _dword row, _dword time, _dword random )		
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mColumn		= column;
	emitter->mRow			= row;
	emitter->mTime			= time;
	emitter->mRandomFrame	= random;

	return _true;
}

_bool ParticleSystem::GetEmitterTextureFrame( _dword emitterid, _dword& column, _dword& row, _dword& time, _dword& random ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	column	= emitter->mColumn;
	row		= emitter->mRow;
	time	= emitter->mTime;
	random	= emitter->mRandomFrame;

	return _true;
}

_bool ParticleSystem::GetEmitterDirection( _dword emitterid, EulerAngles& rotation ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	rotation = emitter->mRotation;

	return _true;
}

_bool ParticleSystem::SetEmitterMesh( _dword emitterid, IMesh* mesh )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	if ( emitter->mAttachedMesh != _null )
		GetModelFactory( ).ReleaseMesh( emitter->mAttachedMesh );

	GetModelFactory( ).CloneMesh( mesh, _true, _false, _null );
			
	emitter->mAttachedMesh = mesh;
			
	return _true;
}

_bool ParticleSystem::GetEmitterMesh( _dword emitterid, IMesh*& mesh ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	if ( emitter->mEmitterType != IParticleSystem::_EMITTER_MESH )
		return _false;

	mesh = emitter->mAttachedMesh;

	return _true;
}

_bool ParticleSystem::SetEmitterGeometry( _dword emitterid, const Geometry& geometry )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	GetGeometryFactory( ).ReleaseGeometry( emitter->mAttachedGeometry );
	emitter->mAttachedGeometry = GetGeometryFactory( ).CloneGeometry( geometry, _true );
				
	return _true;
}

_bool ParticleSystem::GetEmitterGeometry( _dword emitterid, Geometry& geometry ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	if ( emitter->mEmitterType != IParticleSystem::_EMITTER_GEOMETRY )
		return _false;

	geometry = emitter->mAttachedGeometry;

	return _true;
}

_bool ParticleSystem::SetEmitterSkeleton( _dword emitterid, ISkeleton* skeleton )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	GetModelFactory( ).ReleaseSkeleton( emitter->mAttachedSkeleton );
	emitter->mAttachedSkeleton = GetModelFactory( ).CloneSkeleton( skeleton, _true );

	return _true;
}

_bool ParticleSystem::GetEmitterSkeleton( _dword emitterid, ISkeleton*& skeleton ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	if ( emitter->mEmitterType != IParticleSystem::_EMITTER_STATICSKIN )
		return _false;

	skeleton = emitter->mAttachedSkeleton;

	return _true;
}

_bool ParticleSystem::SetEmitterRenderMethod( _dword emitterid, IRenderMethod* rendermethod )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	GetTechniqueFactory( ).ReleaseRenderMethod( emitter->mRenderMethod );
	emitter->mRenderMethod = GetTechniqueFactory( ).CloneRenderMethod( rendermethod, _true );

	return _true;
}

IRenderMethod* ParticleSystem::GetEmitterRenderMethod( _dword emitterid )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _null;

	return emitter->mRenderMethod;
}

_dword ParticleSystem::GetEmitterParticleNumber( _dword emitterid ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return 0;

	return emitter->mParticlePool.Number( );
}

_void ParticleSystem::ClearEmitterParticles( _dword emitterid )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return;

	emitter->mParticlePool.Clear( );
	emitter->mCurrTickcount	 = 0;
	emitter->mLastTickcount	 = 0;
	emitter->mLastParticleID = 0;
	emitter->mLastFrameTickcount = 0;
}

_bool ParticleSystem::EnableEmitter( _dword emitterid )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mEnabled = _true;

	return _true;
}

_bool ParticleSystem::DisableEmitter( _dword emitterid )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	emitter->mEnabled = _false;

	return _true;
}

_bool ParticleSystem::IsEmitterEnabled( _dword emitterid )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	return emitter->mEnabled;
}

ITagPoint* ParticleSystem::CreateTagPoint( _dword gravityid )
{
	ParticleGravity* gravity = SearchGravity( gravityid );
	if ( gravity == _null )
		return _null;

	GetPhysicsFactory( ).ReleaseTagPoint( gravity->mAttachedTagPoint );
	gravity->mAttachedTagPoint = GetPhysicsFactory( ).CreateTagPoint( );

	return gravity->mAttachedTagPoint;
}

_bool ParticleSystem::SetEmitterTagPoint( _dword emitterid, _dword tagpointindex, ITagPoint* tagpoint )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null || tagpointindex > 1 )
		return _false;

	GetPhysicsFactory( ).ReleaseTagPoint( emitter->mAttachedTagPoint[ tagpointindex ] );
	emitter->mAttachedTagPoint[tagpointindex] = GetPhysicsFactory( ).CloneTagPoint( tagpoint, _true );
	
	return _true;
}

_bool ParticleSystem::SetGravityType( _dword gravityid, _dword type )
{
	ParticleGravity* gravity = SearchGravity( gravityid );
	if ( gravity == _null )
		return _false;

	gravity->mGravityType = type;

	return _true;
}

_bool ParticleSystem::GetGravityType( _dword gravityid, _dword& type ) const
{
	ParticleGravity* gravity = SearchGravity( gravityid );
	if ( gravity == _null )
		return _false;

	type = gravity->mGravityType;

	return _true;
}

_bool ParticleSystem::SetGravityPower( _dword gravityid, _float power, _float distance )
{
	ParticleGravity* gravity = SearchGravity( gravityid );
	if ( gravity == _null )
		return _false;

	gravity->mPower		= power;
	gravity->mDistance	= distance;

	return _true;
}

_bool ParticleSystem::GetGravityPower( _dword gravityid, _float& power, _float& distance ) const
{
	ParticleGravity* gravity = SearchGravity( gravityid );
	if ( gravity == _null )
		return _false;

	power		= gravity->mPower;
	distance	= gravity->mDistance;

	return _true;
}

_bool ParticleSystem::SetGravityPosition( _dword gravityid, const Vector3& position )
{
	ParticleGravity* gravity = SearchGravity( gravityid );
	if ( gravity == _null )
		return _false;

	gravity->mPosition = position;

	return _true;
}

_bool ParticleSystem::GetGravityPosition( _dword gravityid, Vector3& position ) const
{
	ParticleGravity* gravity = SearchGravity( gravityid );
	if ( gravity == _null )
		return _false;

	position = gravity->mPosition;

	return _true;
}

_bool ParticleSystem::SetGravityDirection( _dword gravityid, const Vector3& direction )
{
	ParticleGravity* gravity = SearchGravity( gravityid );
	if ( gravity == _null )
		return _false;

	gravity->mDirection = direction;

	return _true;
}

_bool ParticleSystem::SetGravityDirection( _dword gravityid, const EulerAngles& rotation )
{
	ParticleGravity* gravity = SearchGravity( gravityid );
	if ( gravity == _null )
		return _false;

	gravity->mDirection = Vector3::cZAxis * Matrix4( ).Rotation( rotation );

	return _true;
}

_bool ParticleSystem::GetGravityDirection( _dword gravityid, Vector3& direction ) const
{
	ParticleGravity* gravity = SearchGravity( gravityid );
	if ( gravity == _null )
		return _false;

	direction = gravity->mDirection;

	return _true;
}

_bool ParticleSystem::EnableGravity( _dword gravityid )
{
	ParticleGravity* gravity = SearchGravity( gravityid );
	if ( gravity == _null )
		return _false;

	gravity->mEnabled = _true;

	return _true;
}

_bool ParticleSystem::DisableGravity( _dword gravityid )
{
	ParticleGravity* gravity = SearchGravity( gravityid );
	if ( gravity == _null )
		return _false;

	gravity->mEnabled = _false;

	return _true;
}

_bool ParticleSystem::IsGravityEnabled( _dword gravityid )
{
	ParticleGravity* gravity = SearchGravity( gravityid );
	if ( gravity == _null )
		return _false;

	return gravity->mEnabled;
}

_bool ParticleSystem::SetUpdaterType( _dword updaterid, _dword type )
{
	ParticleUpdater* updater = SearchUpdater( updaterid );
	if ( updater == _null )
		return _false;

	updater->mUpdaterType = type;

	return _true;
}

_bool ParticleSystem::GetUpdaterType( _dword updaterid, _dword& type ) const
{
	ParticleUpdater* updater = SearchUpdater( updaterid );
	if ( updater == _null )
		return _false;

	type = updater->mUpdaterType;

	return _true;
}

_bool ParticleSystem::InsertUpdaterKeyframe( _dword updaterid, _float lifealpha, _float keyframe )
{
	ParticleUpdater* updater = SearchUpdater( updaterid );
	if ( updater == _null )
		return _false;

	for ( _dword i = 0; i < updater->mKeyframeArray.Number( ); i ++ )
	{
		if ( updater->mKeyframeArray[i].mObject1 < lifealpha )
			continue;

		if ( updater->mKeyframeArray[i].mObject1 == lifealpha )
		{
			updater->mKeyframeArray[i].mObject2 = keyframe;

			return _true;
		}

		if ( updater->mKeyframeArray[i].mObject1 > lifealpha )
		{
			updater->mKeyframeArray.Insert( ParticleUpdater::Keyframe( lifealpha, keyframe ), i );

			return _true;
		}
	}

	updater->mKeyframeArray.Append( ParticleUpdater::Keyframe( lifealpha, keyframe ) );

	return _true;
}

_bool ParticleSystem::RemoveUpdaterKeyframe( _dword updaterid, _float lifealpha )
{
	ParticleUpdater* updater = SearchUpdater( updaterid );
	if ( updater == _null )
		return _false;

	for ( _dword i = 0; i < updater->mKeyframeArray.Number( ); i ++ )
	{
		if ( updater->mKeyframeArray[i].mObject1 == lifealpha )
		{
			updater->mKeyframeArray.Remove( i );

			return _true;
		}
	}

	return _false;
}

_bool ParticleSystem::ClearUpdaterKeyframe( _dword updaterid )
{
	ParticleUpdater* updater = SearchUpdater( updaterid );
	if ( updater == _null )
		return _false;

	updater->mKeyframeArray.Clear( );

	return _true;
}

_dword ParticleSystem::GetUpdaterKeyframeNumber( _dword updaterid ) const
{
	ParticleUpdater* updater = SearchUpdater( updaterid );
	if ( updater == _null )
		return 0;

	return updater->mKeyframeArray.Number( );
}

_bool ParticleSystem::GetUpdaterKeyframe( _dword updaterid, _dword index, _float& lifealpha, _float& keyframe ) const
{
	ParticleUpdater* updater = SearchUpdater( updaterid );
	if ( updater == _null )
		return _false;

	if ( index >= updater->mKeyframeArray.Number( ) )
		return _false;

	lifealpha	= updater->mKeyframeArray[ index ].mObject1;
	keyframe	= updater->mKeyframeArray[ index ].mObject2;

	return _true;
}

_bool ParticleSystem::SetBlockerType( _dword blockerid, _dword type )
{
	ParticleBlocker* blocker = SearchBlocker( blockerid );
	if ( blocker == _null )
		return _false;

	blocker->mBlockerType = type;

	return _true;
}

_bool ParticleSystem::GetBlockerType( _dword blockerid, _dword& type ) const
{
	ParticleBlocker* blocker = SearchBlocker( blockerid );
	if ( blocker == _null )
		return _false;

	type = blocker->mBlockerType;

	return _true;
}

_bool ParticleSystem::SetBlockerPlane( _dword blockerid, const Plane& plane )
{
	ParticleBlocker* blocker = SearchBlocker( blockerid );
	if ( blocker == _null )
		return _false;

	blocker->mPlane = plane;

	return _true;
}

_bool ParticleSystem::GetBlockerPlane( _dword blockerid, Plane& plane ) const
{
	ParticleBlocker* blocker = SearchBlocker( blockerid );
	if ( blocker == _null )
		return _false;

	plane = blocker->mPlane;

	return _true;
}

_bool ParticleSystem::SetBlockerFunction( _dword blockerid, _dword function, _float parameter )
{
	ParticleBlocker* blocker = SearchBlocker( blockerid );
	if ( blocker == _null )
		return _false;

	blocker->mFunction	= function;
	blocker->mParameter	= parameter;

	return _true;
}

_bool ParticleSystem::GetBlockerFunction( _dword blockerid, _dword& function, _float& parameter ) const
{
	ParticleBlocker* blocker = SearchBlocker( blockerid );
	if ( blocker == _null )
		return _false;

	function	= blocker->mFunction;
	parameter	= blocker->mParameter;

	return _true;
}

_bool ParticleSystem::AttachGravity( _dword emitterid, _dword gravityid )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	for ( _dword i = 0; i < emitter->mGravityArray.Number( ); i ++ )
	{
		if ( emitter->mGravityArray[i] == gravityid )
			return _true;
	}

	emitter->mGravityArray.Append( gravityid );

	return _true;
}

_bool ParticleSystem::DetachGravity( _dword emitterid, _dword gravityid )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	for ( _dword i = 0; i < emitter->mGravityArray.Number( ); i ++ )
	{
		if ( emitter->mGravityArray[i] == gravityid )
		{
			emitter->mGravityArray.Remove( i );

			return _true;
		}
	}

	return _false;
}

_bool ParticleSystem::AttachUpdater( _dword emitterid, _dword updaterid )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	for ( _dword i = 0; i < emitter->mUpdaterArray.Number( ); i ++ )
	{
		if ( emitter->mUpdaterArray[i] == updaterid )
			return _true;
	}

	emitter->mUpdaterArray.Append( updaterid );

	return _true;
}

_bool ParticleSystem::DetachUpdater( _dword emitterid, _dword updaterid )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	for ( _dword i = 0; i < emitter->mUpdaterArray.Number( ); i ++ )
	{
		if ( emitter->mUpdaterArray[i] == updaterid )
		{
			emitter->mUpdaterArray.Remove( i );

			return _true;
		}
	}

	return _false;
}

_bool ParticleSystem::EnableUpdater( _dword updaterid )
{
	ParticleUpdater* updater = SearchUpdater( updaterid );
	if ( updater == _null )
		return _false;

	updater->mEnabled = _true;

	return _true;
}

_bool ParticleSystem::DisableUpdater( _dword updaterid )
{
	ParticleUpdater* updater = SearchUpdater( updaterid );
	if ( updater == _null )
		return _false;

	updater->mEnabled = _false;

	return _true;
}

_bool ParticleSystem::IsUpdaterEnabled( _dword updaterid )
{
	ParticleUpdater* updater = SearchUpdater( updaterid );
	if ( updater == _null )
		return _false;

	return updater->mEnabled;
}

_bool ParticleSystem::AttachBlocker( _dword emitterid, _dword blockerid )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	for ( _dword i = 0; i < emitter->mBlockerArray.Number( ); i ++ )
	{
		if ( emitter->mBlockerArray[i] == blockerid )
			return _true;
	}

	emitter->mBlockerArray.Append( blockerid );

	return _true;
}

_bool ParticleSystem::DetachBlocker( _dword emitterid, _dword blockerid )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	for ( _dword i = 0; i < emitter->mBlockerArray.Number( ); i ++ )
	{
		if ( emitter->mBlockerArray[i] == blockerid )
		{
			emitter->mBlockerArray.Remove( i );

			return _true;
		}
	}

	return _false;
}

_dword ParticleSystem::GetAttachedGravityNumber( _dword emitterid ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return 0;

	return emitter->mGravityArray.Number( );
}

_dword ParticleSystem::GetAttachedGravityID( _dword emitterid, _dword index ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return 0;

	if ( index >= emitter->mGravityArray.Number( ) )
		return 0;

	return emitter->mGravityArray[ index ];
}

_dword ParticleSystem::GetAttachedUpdaterNumber( _dword emitterid ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return 0;

	return emitter->mUpdaterArray.Number( );
}

_dword ParticleSystem::GetAttachedUpdaterID( _dword emitterid, _dword index ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return 0;

	if ( index >= emitter->mUpdaterArray.Number( ) )
		return 0;

	return emitter->mUpdaterArray[ index ];
}

_dword ParticleSystem::GetAttachedBlockerNumber( _dword emitterid ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return 0;

	return emitter->mBlockerArray.Number( );
}

_dword ParticleSystem::GetAttachedBlockerID( _dword emitterid, _dword index ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return 0;

	if ( index >= emitter->mBlockerArray.Number( ) )
		return 0;

	return emitter->mBlockerArray[ index ];
}

_void ParticleSystem::SetBindBoneName( _dword emitterid, _dword bindindex, StringPtr name )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );

	if ( emitter != _null && bindindex < 2 )
		emitter->mBindName[ bindindex ] = name;
}

StringPtr ParticleSystem::GetBindBoneName( _dword emitterid, _dword bindindex ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );

	if ( emitter != _null && bindindex < 2 )
		return emitter->mBindName[ bindindex ];	
	else
		return _null;
}

_bool ParticleSystem::DoAttachEmitterToTagPoint( _dword emitterid, ISkeleton* skeleton )
{
	if ( skeleton == _null )
		return _false;

	for ( int i = 0; i < 2; i ++ )
	{
		_dword boneid = skeleton->SearchBone( GetBindBoneName( emitterid, i ) );
		_dword dummyid = skeleton->SearchDummy( GetBindBoneName( emitterid, i ) );
		
		// bind a bone
		if ( boneid != -1 )
		{
			ITagPoint* tagpoint = skeleton->CreateBoneTagPoint( boneid );

			if ( SetEmitterTagPoint( emitterid, i, tagpoint ) == _false )
				return _false;
		}
		// bind a dummy
		else if ( dummyid != -1 )
		{
			ITagPoint* tagpoint = skeleton->CreateDummyTagPoint( dummyid );

			if ( SetEmitterTagPoint( emitterid, i, tagpoint ) == _false )
				return _false;
		}
		// set to _null
		else
		{
			SetEmitterTagPoint( emitterid, i, _null );
			return _false;
		}
	}
	return _true;
}

_void ParticleSystem::RenderParticles( )
{
	GetRenderDevice( ).PushAllRenderMethod( );

	for ( _dword i = 0; i < GetEmitterNumber( ); i ++ )
	{
		ParticleEmitter* emitter = mEmitterArray[i];

		IRenderMethod* particlerendermethod = GetEmitterRenderMethod( GetEmitterID( i ) );

		if ( particlerendermethod != _null )
			GetRenderDevice( ).SetRenderMethod( particlerendermethod );

		Matrix4 backupmatrix = GetRenderDevice( ).GetWorldTransform( );

		Vector3 sortposition;
		GetEmitterPosition( GetEmitterID( i ), sortposition );
		sortposition *= backupmatrix;

		// have done on emit it
		if ( mEmitterArray[ i ]->mEmitWorldtransform == 1 )
			GetRenderDevice( ).SetWorldTransform( Matrix4::cIdentity );
		
		// Get first particle of the particle system.
		Particle* particle = mEmitterArray[ i ]->mParticlePool.FirstElement( );
		
		if ( particle != _null && 
			 particle->mParticleType != IParticleSystem::_PARTICLE_PRIMITIVEIMAGESTRIP &&
			 emitter->mEmitterType != IParticleSystem::_EMITTER_STATICSKIN )
		{
			Vector3 particleposition = particle->mPosition * GetRenderDevice( ).GetWorldTransform( );

			if ( GetRenderDevice( ).GetViewFrustum( ).Inside( AxisAlignedBox( particleposition - Vector3( 100, 100, 100 ), particleposition + Vector3( 100, 100, 100 ) ) ) == _false )
				continue;
		}

		// For trail U value
		_float length = 0.0f;
		
		_dword type = -1;
		if ( particle != _null )
			type = particle->mParticleType;
		while ( particle != _null )
		{
			if ( mRenderAlpha != 1.0f )
			{
				Color color = Color( particle->mColor );
				color.a *= mRenderAlpha;
				particle->mColor = color;
			}

			if ( particle->mDelay != 0 )
			{
				particle = mEmitterArray[ i ]->mParticlePool.NextElement( particle );
				continue;
			}

			// Get previous particle by id.
			Particle* prevparticle = mEmitterArray[ i ]->mParticlePool.Index( particle->mPrevParticleID - 1 );

			switch ( particle->mParticleType )
			{
				case IParticleSystem::_PARTICLE_PRIMITIVEPOINT:
				{
					GetRenderDevice( ).Render( PrimitivePoint( particle->mPosition, particle->mColor ) );

					break;
				}

				case IParticleSystem::_PARTICLE_PRIMITIVELINE:
				{
					Vector3 sizex = particle->mDirection * particle->mSize.x * 0.5f;

					GetRenderDevice( ).Render( PrimitiveLine( particle->mPosition + sizex, particle->mPosition - sizex, particle->mColor ) );

					break;
				}

				case IParticleSystem::_PARTICLE_PRIMITIVERECT:
				{
					Vector3 sizex = Vector3::cZAxis;
					Vector3 sizey = Vector3::cXAxis;
					
					if ( emitter->mAttachedTagPoint[0] != _null && emitter->mEmitterType == _EMITTER_STATIC )
					{
						Matrix4 rotation = Matrix4( ).Rotation( emitter->mRotation )
							* Matrix4( ).FromTranslationRotation( Vector3::cOrigin, emitter->mAttachedTagPoint[0]->GetRotation( ) );

						sizex = Vector3::cZAxis * particle->mSize.x * rotation;
						sizey = Vector3::cXAxis * particle->mSize.y * rotation;
					}
					else
					{
						Vector3 direction = particle->mDirection;
						direction.Normalize( );

						Vector3 tangent = Vector3::Cross( particle->mDirection, particle->mNormal );
						tangent.Normalize( );

						Matrix4 userrotationx;userrotationx.Rotation( particle->mDirection, particle->mUserRotation.x );
						Matrix4 userrotationy;userrotationy.Rotation( particle->mNormal, particle->mUserRotation.y );
						Matrix4 userrotationz;userrotationz.Rotation( tangent, particle->mUserRotation.z );

						Matrix4 userrotation = userrotationx * userrotationy * userrotationz;

						sizex = direction * particle->mSize.x * userrotation;
						sizey = tangent * particle->mSize.y * userrotation;
					}

					GetRenderDevice( ).Render( PrimitiveQuadrangle( particle->mPosition, sizex, sizey, particle->mColor ) );

					break;
				}

				case IParticleSystem::_PARTICLE_PRIMITIVEPOINTSTRIP:
				{
					if ( prevparticle == _null )
						break;

					GetRenderDevice( ).Render( PrimitiveLine( particle->mPosition, prevparticle->mPosition, particle->mColor, prevparticle->mColor ) );

					break;
				}

				case IParticleSystem::_PARTICLE_PRIMITIVELINESTRIP:
				{
					if ( prevparticle == _null )
						break;

					Vector3 sizex1 = particle->mDirection * particle->mSize.x * 0.5f;
					Vector3 sizex2 = prevparticle->mDirection * prevparticle->mSize.x * 0.5f;

					Vector3 position1 = particle->mPosition + sizex1;
					Vector3 position2 = particle->mPosition - sizex1;
					Vector3 position3 = prevparticle->mPosition + sizex2;
					Vector3 position4 = prevparticle->mPosition - sizex2;

					GetRenderDevice( ).Render( PrimitiveTriangle( position1, position2, position3, particle->mColor, particle->mColor, prevparticle->mColor ) );
					GetRenderDevice( ).Render( PrimitiveTriangle( position2, position4, position3, particle->mColor, prevparticle->mColor, prevparticle->mColor ) );

					break;
				}

				case IParticleSystem::_PARTICLE_BILLBOARDSA:
				{
					_dword column, row, time, random;
					GetEmitterTextureFrame( GetEmitterID( i ), column, row, time, random );
					Vector2 texcoordlt( 0, 0 );
					Vector2 texcoordrb( 1, 1 );
					if ( column != 1 || row != 1 )
					{
						texcoordlt = Vector2( particle->mCurrRow / _float( row ),
							particle->mCurrColumn / _float( column ) );
						texcoordrb = Vector2( ( particle->mCurrRow + 1 ) / _float( row ),
							( particle->mCurrColumn + 1 ) / _float( column ) );
					}

					GetRenderDevice( ).Render( BillboardScreenAligned( particle->mPosition, Vector2( particle->mSize.x, particle->mSize.y ), texcoordlt, texcoordrb, particle->mColor, particle->mUserRotation.x, particle->mTexture, mParticleGroup, i ) );

					break;
				}

				case IParticleSystem::_PARTICLE_BILLBOARDWO:
				{
					_dword column, row, time, random;
					GetEmitterTextureFrame( GetEmitterID( i ), column, row, time, random );
					Vector2 texcoordlt( 0, 0 );
					Vector2 texcoordrb( 1, 1 );
					if ( column != 1 || row != 1 )
					{
						texcoordlt = Vector2( particle->mCurrRow / _float( row ),
							particle->mCurrColumn / _float( column ) );
						texcoordrb = Vector2( ( particle->mCurrRow + 1 ) / _float( row ),
							( particle->mCurrColumn + 1 ) / _float( column ) );
					}

					GetRenderDevice( ).Render( BillboardWorldOriented( particle->mPosition, Vector2( particle->mSize.x, particle->mSize.y ), texcoordlt, texcoordrb, particle->mColor, particle->mUserRotation.x, particle->mTexture, mParticleGroup, i ) );
					
					break;
				}

				case IParticleSystem::_PARTICLE_BILLBOARDAX:
				{
					_dword column, row, time, random;
					GetEmitterTextureFrame( GetEmitterID( i ), column, row, time, random );
					Vector2 texcoordlt( 0, 0 );
					Vector2 texcoordrb( 1, 1 );
					if ( column != 1 || row != 1 )
					{
						texcoordlt = Vector2( particle->mCurrRow / _float( row ),
							particle->mCurrColumn / _float( column ) );
						texcoordrb = Vector2( ( particle->mCurrRow + 1 ) / _float( row ),
							( particle->mCurrColumn + 1 ) / _float( column ) );
					}

					Vector3 direction = particle->mDirection;
					if ( emitter->mAttachedTagPoint[0] != _null && emitter->mEmitterType == _EMITTER_STATIC )
					{
						direction *= emitter->mRotationMatrix;
					}

					GetRenderDevice( ).Render( BillboardAxial( particle->mPosition, direction, Vector2( particle->mSize.x, particle->mSize.y ), texcoordlt, texcoordrb, particle->mColor, particle->mTexture, mParticleGroup, i ) );

					break;
				}

				case IParticleSystem::_PARTICLE_MESH:
				{
					if ( particle->mMesh == _null )
						break;

					Matrix4 transform = Matrix4::cIdentity;
					if ( emitter->mEmitterType == IParticleSystem::_EMITTER_STATICSKIN )
					{
						transform = Matrix4( ).Scaling( particle->mSize )
							* Matrix4( ).Translation( particle->mPosition );
					}
					else
					{
						
						if ( emitter->mAttachedTagPoint[0] != _null && emitter->mEmitterType == _EMITTER_STATIC )
						{
							transform = Matrix4( ).Scaling( particle->mSize )
								* Matrix4( ).Rotation( emitter->mRotation )
								* Matrix4( ).FromTranslationRotation( Vector3::cOrigin, emitter->mAttachedTagPoint[0]->GetRotation( ) )
								* Matrix4( ).Translation( particle->mPosition );
						}
						else
						{
							transform = Matrix4( ).Scaling( particle->mSize ) 
								* Matrix4( ).Rotation( -Vector3::cYAxis, particle->mDirection )
								* Matrix4( ).Rotation( EulerAngles( particle->mUserRotation.x, particle->mUserRotation.y, particle->mUserRotation.z ) )
								* Matrix4( ).Translation( particle->mPosition );
						}
					}

					for ( _dword i = 0; i < particle->mMesh->GetModelNumber( ); i ++ )
					{
						Geometry& geometry = particle->mMesh->GetModelGeometry( i );
						geometry.mTextureFactor = particle->mColor;
						if ( particle->mTexture != _null && geometry.mTexture[0] != particle->mTexture )
						{
							GetTextureFactory( ).ReleaseTexture( geometry.mTexture[0] );
							geometry.mTexture[0] = GetTextureFactory( ).CloneTexture( particle->mTexture, _true );
						}
						
						GetRenderDevice( ).Render( geometry, transform );
					}
					break;
				}

				case IParticleSystem::_PARTICLE_PRIMITIVEIMAGESTRIP:
				{
					if ( prevparticle == _null )
						break;

					Vector3 sizex1 = particle->mDirection * particle->mSize.x * 0.5f;
					Vector3 sizex2 = prevparticle->mDirection * prevparticle->mSize.x * 0.5f;

					_float u1 = 1.0f - length / GetEmitterLength( GetEmitterID( i ) );
					length += ( particle->mPosition - prevparticle->mPosition ).Magnitude( );
					_float u2 = 1.0f - length / GetEmitterLength( GetEmitterID( i ) );
 					
					_float v1 = 0.0f;
					_float v2 = 1.0f;
					
					_dword column, row, time, random;
					GetEmitterTextureFrame( GetEmitterID( i ), column, row, time, random );
					Vector2 texcoordlt( 0, 0 );
					Vector2 texcoordrb( 1, 1 );
					if ( column != 1 || row != 1 )
					{
						texcoordlt = Vector2( emitter->mCurrRow / _float( row ),
							emitter->mCurrColumn / _float( column ) );
						texcoordrb = Vector2( ( emitter->mCurrRow + 1 ) / _float( row ),
							( emitter->mCurrColumn + 1 ) / _float( column ) );
					}

					u1 = texcoordlt.x + u1 * ( texcoordrb.x - texcoordlt.x );
					u2 = texcoordlt.x + u2 * ( texcoordrb.x - texcoordlt.x );
					v1 = texcoordlt.y + v1 * ( texcoordrb.y - texcoordlt.y );
					v2 = texcoordlt.y + v2 * ( texcoordrb.y - texcoordlt.y );

					Vector3 position1 = particle->mPosition;
					Vector2 texcoord1( u1, v1 );
					Vector3 position2 = particle->mPosition;
					Vector2 texcoord2( u1, v2 );
					Vector3 position3 = prevparticle->mPosition;
					Vector2 texcoord3( u2, v1 );
					Vector3 position4 = prevparticle->mPosition;
					Vector2 texcoord4( u2, v2 );

					if ( emitter->mStripExtend == 0 )
					{
						position1 += sizex1;
						position2 -= sizex1;
						position3 += sizex2;
						position4 -= sizex2;
					}
					else if ( emitter->mStripExtend == 1 || emitter->mStripExtend == 3 )
					{
						position1 += sizex1;
						position3 += sizex2;
					}
					else if ( emitter->mStripExtend == 2 || emitter->mStripExtend == 4 )
					{
						position2 -= sizex1;
						position4 -= sizex2;
					}

					GetRenderDevice( ).Render( PrimitiveTriangleImage( position1, position2, position3, texcoord1, texcoord2, texcoord3, particle->mColor, particle->mColor, prevparticle->mColor, particle->mTexture ) );
					GetRenderDevice( ).Render( PrimitiveTriangleImage( position2, position4, position3, texcoord2, texcoord4, texcoord3, particle->mColor, prevparticle->mColor, prevparticle->mColor, particle->mTexture ) );

					break;
				}

				case IParticleSystem::_PARTICLE_MESHCOLUMN:
				{
					break;
				}

				case IParticleSystem::_PARTICLE_MESHSPHERE:
				{
					Matrix4 transform = Matrix4( ).Scaling( particle->mSize ) 
						* Matrix4( ).Rotation( EulerAngles( particle->mUserRotation.x, particle->mUserRotation.y, particle->mUserRotation.z ) )
						* Matrix4( ).Translation( particle->mPosition );

					mEmitterArray[ i ]->mGeometry.mTextureFactor = particle->mColor;
					mEmitterArray[ i ]->mGeometry.mTexture[0] = GetTextureFactory( ).CloneTexture( particle->mTexture, _true );
					
					GetRenderDevice( ).Render( mEmitterArray[ i ]->mGeometry, transform );
					break;
				}

				case IParticleSystem::_PARTICLE_PRIMITIVEQUADRANGLEIMAGE:
				{
					Vector3 sizex = Vector3::cZAxis;
					Vector3 sizey = Vector3::cXAxis;
					
					if ( emitter->mAttachedTagPoint[0] != _null && emitter->mEmitterType == _EMITTER_STATIC )
					{
						Matrix4 rotation = Matrix4( ).Rotation( emitter->mRotation )
							* Matrix4( ).FromTranslationRotation( Vector3::cOrigin, emitter->mAttachedTagPoint[0]->GetRotation( ) );

						sizex = Vector3::cZAxis * particle->mSize.x * rotation;
						sizey = Vector3::cXAxis * particle->mSize.y * rotation;
					}
					else
					{
						Vector3 direction = particle->mDirection;
						direction.Normalize( );

						Vector3 tangent = Vector3::Cross( particle->mDirection, particle->mNormal );
						tangent.Normalize( );

						Matrix4 userrotationx;userrotationx.Rotation( tangent, particle->mUserRotation.x );
						Matrix4 userrotationy;userrotationy.Rotation( particle->mDirection, particle->mUserRotation.y );
						Matrix4 userrotationz;userrotationz.Rotation( particle->mNormal, particle->mUserRotation.z );

						Matrix4 userrotation = userrotationx * userrotationy * userrotationz;

						sizex = direction * particle->mSize.x * userrotation;
						sizey = tangent * particle->mSize.y * userrotation;
					}
					
					_dword column, row, time, random;
					GetEmitterTextureFrame( GetEmitterID( i ), column, row, time, random );
					Vector2 texcoordlt( 0, 0 );
					Vector2 texcoordrb( 1, 1 );
					if ( column != 1 || row != 1 )
					{
						texcoordlt = Vector2( particle->mCurrRow / _float( row ),
							particle->mCurrColumn / _float( column ) );
						texcoordrb = Vector2( ( particle->mCurrRow + 1 ) / _float( row ),
							( particle->mCurrColumn + 1 ) / _float( column ) );
					}

					GetRenderDevice( ).Render( PrimitiveQuadrangleImage( particle->mPosition, sizex, sizey, 
						texcoordlt, Vector2( texcoordrb.x, texcoordlt.y ), Vector2( texcoordlt.x, texcoordrb.y ), texcoordrb,
						particle->mColor, particle->mTexture ) );

					break;
				}
			}

			if ( mShowDirection )
			{
				GetRenderDevice( ).SetRenderMethod(
					GetTechniqueFactory( ).GetDefaultNormalRenderMethod( ) );

				GetRenderDevice( ).Render( PrimitiveLine( particle->mPosition,
					particle->mPosition + particle->mNormal * 2.0f, Color::cWhite, Color::cDarkBlue ) );

				GetRenderDevice( ).Render( PrimitiveLine( particle->mPosition,
					particle->mPosition + particle->mDirection * 2.0f, Color::cWhite, Color::cDarkGreen ) );
			}

			particle = mEmitterArray[ i ]->mParticlePool.NextElement( particle );
		}
		
		if ( mEmitterArray[ i ]->mEmitWorldtransform == 1 )
			GetRenderDevice( ).SetWorldTransform( backupmatrix );
	}

	GetRenderDevice( ).PopAllRenderMethod( );
}

_void ParticleSystem::Stop( _dword duration )
{
	if ( mStop )
		return;

	mStop = 1;

	for ( _dword i = 0; i < GetEmitterNumber( ); i ++ )
		DisableEmitter( GetEmitterID( i ) );

	// End existing particle.
	for ( _dword i = 0; i < mEmitterArray.Number( ); i ++ )
	{
		ParticleEmitter* emitter = mEmitterArray[i];

		for ( ParticleEmitter::ParticlePool::Iterator it = emitter->mParticlePool.GetHeadIterator( ); it.Valid( ); it ++ )
		{
			Particle& particle = it;
			if ( particle.mLife != -1 )
				continue;

			_dword limitlife = particle.mCurrTickcount + duration;
			if ( particle.mLife > limitlife )
			{
				particle.mLife = limitlife;
				particle.mStopTickCount = particle.mCurrTickcount;
			}
		}
	}
}

_void ParticleSystem::Reset( )
{
	ClearAllParticles( );
	mStop = 0;

	for ( _dword i = 0; i < GetEmitterNumber( ); i ++ )
		EnableEmitter( GetEmitterID( i ) );
}

_dword ParticleSystem::IsStop( ) const
{
	return mStop;
}

_bool ParticleSystem::SetTransOnEmit( _dword emitterid, _dword transonemit )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;
	emitter->mEmitWorldtransform = transonemit;
	return _true;
}

_bool ParticleSystem::GetTransOnEmit( _dword emitterid, _dword& transonemit ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;
	transonemit = emitter->mEmitWorldtransform;
	return _true;
}

_bool ParticleSystem::SetTransOnMarker( _dword emitterid, _dword transonmarker )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;
	emitter->mMarkerTransform = transonmarker;
	return _true;
}

_bool ParticleSystem::GetTransOnMarker( _dword emitterid, _dword& transonmarker ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;
	transonmarker = emitter->mMarkerTransform;
	return _true;
}

_bool ParticleSystem::SetEmitterUVInvert( _dword emitterid, _dword invert )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return 0;
	emitter->mStripExtend = invert;
	return 1;
}

_bool ParticleSystem::GetEmitterUVInvert( _dword emitterid, _dword& invert ) const
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return 0;
	invert = emitter->mStripExtend;
	return 1;
}

_void ParticleSystem::SetShowDirection( _dword showdirection )
{
	mShowDirection = showdirection;
}

_void ParticleSystem::GetShowDirection( _dword& showdirection ) const
{
	showdirection = mShowDirection;
}

_void ParticleSystem::SetUserData( _dword userdata )
{
	mUserData = userdata;
}

_dword ParticleSystem::GetUserData( ) const
{
	return mUserData;
}

_void ParticleSystem::EnableUpdateAABB( _bool enable )
{
	mUpdateAABB = enable;
}

const AxisAlignedBox& ParticleSystem::GetBoundBox( ) const
{
	return mBoundBox;
}

_void ParticleSystem::SetRenderAlpha( _float alpha )
{
	mRenderAlpha = alpha;
}

_bool ParticleSystem::SetParticleMeshRenderFlag( _dword emitterid, _dword renderflag )
{
	ParticleEmitter* emitter = SearchEmitter( emitterid );
	if ( emitter == _null )
		return _false;

	if ( emitter->mMesh == _null )
		return _false;

	for ( _dword i = 0; i < emitter->mMesh->GetModelNumber( ); i ++ )
	{
		Geometry& geometry = emitter->mMesh->GetModelGeometry( i );
		geometry.mRenderFlag = renderflag;
	}

	return _true;
}