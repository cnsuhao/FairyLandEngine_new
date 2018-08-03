//============================================================================
// ParticlePlayer.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// ParticlePlayer Implementation
//----------------------------------------------------------------------------

ParticlePlayer::ParticlePlayer( )
{
	mTransform		= Matrix4::cIdentity;
	mScale			= 1.0f;

	mUniqueID		= 0;

	mUseOrtho		= _false;

	mTotalElapse	= 0;

	IncreaseRefCount( );
}

ParticlePlayer::~ParticlePlayer( )
{
	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
		GetParticleFactory( ).ReleaseParticleSystem( mParticleArray[i].mParticleSystem );
}

_dword ParticlePlayer::GetParticleSystemNumber( ) const
{
	return mParticleArray.Number( );
}

StringPtr ParticlePlayer::GetParticleSystemName( _dword index ) const
{
	if ( index >= mParticleArray.Number( ) ) 
		return L"";

	return mParticleArray[ index ].mParticleName;
}

const IParticleSystem* ParticlePlayer::GetParticleSystem( _dword index ) const
{
	if ( index >= mParticleArray.Number( ) ) 
		return _null;

	return mParticleArray[ index ].mParticleSystem;
}

const IParticleSystem* ParticlePlayer::GetParticleSystemByID( _dword id ) const
{
	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
	{
		if ( mParticleArray[i].mParticleID == id )
		{
			return mParticleArray[i].mParticleSystem;
		}
	}
	return _null;
}

_void ParticlePlayer::AppendResourcePath( StringPtr path )
{
	for ( _dword i = 0; i < mSearchResourcePath.Number( ); i ++ )
	{
		if ( mSearchResourcePath[i].CompareLowercase( path ) )
		{
			return;
		}
	}

	mSearchResourcePath.Append( String( path ) );
}

_void ParticlePlayer::RemoveResourcePath( StringPtr path )
{
	for ( _dword i = 0; i < mSearchResourcePath.Number( ); i ++ )
	{
		if ( mSearchResourcePath[i].CompareLowercase( path ) )
		{
			mSearchResourcePath.Remove( i );
		}
	}
}

_dword ParticlePlayer::Play( StringPtr filename, StringPtr name, const Geometry& geometry )
{
	IResourceManager* resourcemanager = GetResourceManagerPointer( );
	FL_ASSERT( resourcemanager != _null )

	for ( _dword i = 0; i < mSearchResourcePath.Number( ); i ++ )
		resourcemanager->InsertSearchPath( mSearchResourcePath[i] );

	// Filename should be full path
	IParticleSystem* particlesystem = GetParticleFactory( ).CreateParticleSystem( filename );

	for ( _dword i = 0; i < mSearchResourcePath.Number( ); i ++ )
		resourcemanager->RemoveSearchPath( mSearchResourcePath[i] );
		
	if ( particlesystem == _null )
		return -1;

	ParticleDesc particledesc;
	particledesc.mParticleSystem	= particlesystem;
	particledesc.mParticleName		= name;
	particledesc.mParticleID		= mUniqueID;
	particledesc.mBindMatrix		= Matrix4::cIdentity;
	particledesc.mLifeLimit			= -1;
	particledesc.mDelay 			= 0;
	
	// Append new particle into array.
	mParticleArray.Append( particledesc );
	mUniqueID ++;

	for ( _dword i = 0; i < particlesystem->GetEmitterNumber( ); i ++ )
	{
		_dword emitterid = particlesystem->GetEmitterID( i );
		
		_dword type, dir, normal;
		particlesystem->GetEmitterType( emitterid, type, dir, normal );

		if ( type != IParticleSystem::_EMITTER_GEOMETRY )
			continue;

		particlesystem->SetEmitterGeometry( emitterid, geometry );
	}

	particlesystem->Reset( );

	return particledesc.mParticleID;
}

_dword ParticlePlayer::Play( StringPtr filename, StringPtr name, const Matrix4& bindmatrix, _dword delay )
{
	return Play( filename, 0, name, delay, -1, _null, _null, bindmatrix, 1.0f );
}

_dword ParticlePlayer::Play( StringPtr filename, _dword priority, StringPtr name, _dword delay, 
	_dword lifelimit, ISkeleton* skeleton, IMesh* mesh, const Matrix4& bindmatrix, _float scale )
{

	IResourceManager* resourcemanager = GetResourceManagerPointer( );
	FL_ASSERT( resourcemanager != _null )

	for ( _dword j = 0; j < mSearchResourcePath.Number( ); j ++ )
		resourcemanager->InsertSearchPath( mSearchResourcePath[j] );
	
	// For cache system, filename should be full path
	IParticleSystem* particlesystem = GetParticleFactory( ).CreateParticleSystem( filename );

	for ( _dword j = 0; j < mSearchResourcePath.Number( ); j ++ )
		resourcemanager->RemoveSearchPath( mSearchResourcePath[j] );
		
	if ( particlesystem == _null )
		return -1;

	ParticleDesc particledesc;
	particledesc.mParticleSystem	= particlesystem;
	particledesc.mParticleName		= name;
	particledesc.mParticleID		= mUniqueID;
	particledesc.mColorPriority		= priority;
	particledesc.mBindMatrix		= bindmatrix;
	particledesc.mLifeLimit			= lifelimit;
	particledesc.mDelay				= delay;
	particledesc.mScale				= scale;
	
	// Append new particle into array.
	mParticleArray.Append( particledesc );
	mUniqueID ++;

	particlesystem->SetScale( scale );

	for ( _dword i = 0; i < particlesystem->GetEmitterNumber( ); i ++ )
	{
		_dword emitterid = particlesystem->GetEmitterID( i );
		particlesystem->DoAttachEmitterToTagPoint( emitterid, skeleton );
		particlesystem->SetEmitterSkeleton( emitterid, skeleton );

		_dword type, dir, normal;
		particlesystem->GetEmitterType( emitterid, type, dir, normal );

		if ( type != IParticleSystem::_EMITTER_MESH )
			continue;

		particlesystem->SetEmitterMesh( emitterid, mesh );
	}

	particlesystem->Reset( );

	return particledesc.mParticleID;
}

_void ParticlePlayer::Stop( StringPtr name, _dword duration, _dword findall )
{
	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
	{
		ParticleDesc& particledesc = mParticleArray[i];

		if ( particledesc.mParticleName == name )
		{
			particledesc.mParticleSystem->Stop( duration );

			if ( !findall )
				break;
		}
	}
}

_void ParticlePlayer::Stop( _dword id, _dword duration )
{
	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
	{
		ParticleDesc& particledesc = mParticleArray[i];

		if ( particledesc.mParticleID == id )
		{
			particledesc.mParticleSystem->Stop( duration );

			break;
		}
	}
}

_void ParticlePlayer::StopAll( _dword duration )
{
	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
		mParticleArray[i].mParticleSystem->Stop( duration );
}

_void ParticlePlayer::Update( _dword elapse, const Matrix4& transform )
{
	mTotalElapse += elapse;
	if ( mTotalElapse < 16 )
		return;

	elapse = mTotalElapse;
	mTotalElapse = 0;

	// Update
	mTransform = transform;

	// Remove finish particle system
	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
	{
		ParticleDesc& particledesc = mParticleArray[i];

		// Never remove important particle
		if ( particledesc.mColorPriority > 0 )
			continue;

		if ( particledesc.mParticleSystem->IsFinished( ) == _false )
			continue;

		if ( particledesc.mParticleSystem->GetParticleNumber( ) > 0 )
			continue;

		GetParticleFactory( ).ReleaseParticleSystem( particledesc.mParticleSystem );
			
		mParticleArray.Remove( i ); i --;
	}
		
	Matrix4 backupworldtransform = GetRenderDevice( ).GetWorldTransform( );

	// Update remain particle systems.
	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
	{
		ParticleDesc& particledesc = mParticleArray[i];

		if ( particledesc.mDelay > elapse )
		{
			particledesc.mDelay -= elapse;
		}
		else
		{
			particledesc.mDelay = 0;
		}

		if ( particledesc.mDelay == 0 )
		{
			GetRenderDevice( ).SetWorldTransform( particledesc.mBindMatrix * mTransform );
			particledesc.mParticleSystem->Update( elapse );
		}
	}

	// Limit life and animation end
	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
	{
		ParticleDesc& particledesc = mParticleArray[i];

		// The particle system reach life limit, we stop the particle system.
		if ( particledesc.mParticleSystem->GetCurrentTickcount( ) > particledesc.mLifeLimit )
			particledesc.mParticleSystem->Stop( );
	}

	GetRenderDevice( ).SetWorldTransform( backupworldtransform );
}

_void ParticlePlayer::Render( )
{
	Matrix4 backupworldtransform = GetRenderDevice( ).GetWorldTransform( );
	Matrix4 backupviewtransfrom	 = GetRenderDevice( ).GetViewTransform( );
	Matrix4 backupprojtransfrom	 = GetRenderDevice( ).GetProjectionTransform( );

	if ( mUseOrtho )
	{
		const Viewport& viewport = GetRenderDevice( ).GetViewport( );

		_float x = viewport.w / 2.0f;
		_float z = viewport.h / 2.0f;

		GetRenderDevice( ).SetViewTransform( Matrix4::CreateLookAtLH( Vector3( x, 1000.0f, -z ), Vector3( x, 0.0f, -z ), Vector3::cZAxis ) );
		GetRenderDevice( ).SetProjectionTransform( Matrix4::CreateOrthoLH( x * 2.0f, z * 2.0f, 0.1f, 5000.0f ) );
	}

	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
	{
		ParticleDesc& particledesc = mParticleArray[i];

		GetRenderDevice( ).SetWorldTransform( particledesc.mBindMatrix * mTransform );
		GetRenderDevice( ).Render( particledesc.mParticleSystem );
	}

	GetRenderDevice( ).SetWorldTransform( backupworldtransform );

	if ( mUseOrtho )
	{
		GetRenderDevice( ).SetViewTransform( backupviewtransfrom );
		GetRenderDevice( ).SetProjectionTransform( backupprojtransfrom );
	}
}

_bool ParticlePlayer::GetUserColor( Color& color )
{
	_bool found = _false;
	_dword currentpriority = 0;
	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
	{
		_dword j = mParticleArray.Number( ) - i - 1;

		ParticleDesc& particledesc = mParticleArray[j];

		_float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;

		if ( particledesc.mParticleSystem->GetUpdaterCurrentData( 1000, a ) == _false ||
			 particledesc.mParticleSystem->GetUpdaterCurrentData( 1001, r ) == _false ||
			 particledesc.mParticleSystem->GetUpdaterCurrentData( 1002, g ) == _false ||
			 particledesc.mParticleSystem->GetUpdaterCurrentData( 1003, b ) == _false )
		{
			// Have high priority but not in this mode
			if ( particledesc.mColorPriority > currentpriority )
			{
				color = Color( r, g, b, a );
				return _false;
			}
			
			continue;
		}

		// We care this particle
		if ( particledesc.mColorPriority >= currentpriority )
		{
			// Have to set color again
			if ( particledesc.mColorPriority > currentpriority )
				found = _false;

			// First time
			if ( found == _false )
			{
				color = Color( r, g, b, a );
			}
			else
			{
				color *= Color( r, g, b, a );
			}

			found = _true;
			currentpriority = particledesc.mColorPriority;
		}
	}

	return found;
}

_bool ParticlePlayer::GetUserAdditiveColor( Color& color )
{
	_bool found = _false;
	_dword currentpriority = 0;
	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
	{
		_dword j = mParticleArray.Number( ) - i - 1;

		ParticleDesc& particledesc = mParticleArray[j];

		_float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;

		if ( particledesc.mParticleSystem->GetUpdaterCurrentData( 2000, a ) == _false ||
			 particledesc.mParticleSystem->GetUpdaterCurrentData( 2001, r ) == _false ||
			 particledesc.mParticleSystem->GetUpdaterCurrentData( 2002, g ) == _false ||
			 particledesc.mParticleSystem->GetUpdaterCurrentData( 2003, b ) == _false )
		{
			// Have high priority but not in this mode
			if ( particledesc.mColorPriority > currentpriority )
			{
				color = Color( r, g, b, a );
				return _false;
			}
			
			continue;
		}

		// We care this particle
		if ( particledesc.mColorPriority >= currentpriority )
		{
			// Have to set color again
			if ( particledesc.mColorPriority > currentpriority )
				found = _false;

			// First time
			if ( found == _false )
			{
				color = Color( r, g, b, a );
			}
			else
			{
				color *= Color( r, g, b, a );
			}

			found = _true;
			currentpriority = particledesc.mColorPriority;
		}
	}

	return found;
}

_dword ParticlePlayer::GetParticleNumber( )
{
	_dword count = 0;

	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
	{
		ParticleDesc& particledesc = mParticleArray[i];

		count += particledesc.mParticleSystem->GetParticleNumber( );
	}

	return count;
}

_dword ParticlePlayer::SearchParticleID( StringPtr name, _bool carestopped ) const
{
	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
	{
		const ParticleDesc& particledesc = mParticleArray[i];

		// No care stopped and is stoped
		if ( carestopped == _false && particledesc.mParticleSystem->IsStop( ) )
			continue;

		if ( particledesc.mParticleName.CompareLowercase( name ) )
			return particledesc.mParticleID;
	}

	return -1;
}

_void ParticlePlayer::SetBindMatrixByID( _dword id, const Matrix4& matrix )
{
	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
	{
		ParticleDesc& particledesc = mParticleArray[i];

		if ( particledesc.mParticleID == id )
			particledesc.mBindMatrix = matrix;
	}
}

_void ParticlePlayer::SetUseOrtho( _bool enable )
{
	mUseOrtho = enable;
}

_bool ParticlePlayer::GetUseOrtho( ) const
{
	return mUseOrtho;
}

_void ParticlePlayer::SetRenderAlpha( _float alpha )
{
	for ( _dword i = 0; i < mParticleArray.Number( ); i ++ )
	{
		ParticleDesc& particledesc = mParticleArray[i];

		if ( particledesc.mParticleSystem != _null )
			particledesc.mParticleSystem->SetRenderAlpha( alpha );
	}
}