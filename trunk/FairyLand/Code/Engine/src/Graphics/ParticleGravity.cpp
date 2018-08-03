//============================================================================
// ParticleGravity.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// ParticleGravity Implementation
//----------------------------------------------------------------------------

ParticleGravity::ParticleGravity( _dword gravityid )
{
	mGravityID			= gravityid;
	mGravityType		= 0;
	mEnabled			= _true;

	mPower				= 1.0f;
	mDistance			= Math::cMaxFloat;
	mPosition			= Vector3::cOrigin;
	mDirection			= Vector3::cZAxis;

	mAttachedTagPoint	= _null;

	mParticleSystem		= _null;
}

ParticleGravity::~ParticleGravity( )
{
	GetPhysicsFactory( ).ReleaseTagPoint( mAttachedTagPoint );
}

_void ParticleGravity::AffectTagPoint( _dword elapse, _dword transonemit )
{
	if ( mAttachedTagPoint == _null )
		return;

	Particle particle;
	particle.mSpeed		= 100.0f;
	particle.mPosition	= mAttachedTagPoint->GetPosition( );
	particle.mDirection	= Vector3::cZAxis;
	particle.mNormal	= Vector3::cOrigin;

	AffectParticle( particle, elapse, transonemit );

	mAttachedTagPoint->SetPosition( particle.mPosition );
}

_void ParticleGravity::AffectParticle( Particle& particle, _dword elapse, _dword transonemit )
{
	if ( particle.mDirection == Vector3::cOrigin )
		return;

	Matrix4 worldtransform  = GetRenderDevice( ).GetWorldTransform( );
	Matrix3 normaltransform = GetRenderDevice( ).GetNormalTransform( );
	
	if ( transonemit == 0 )
	{
		worldtransform  = Matrix4::cIdentity;
		normaltransform = Matrix3::cIdentity;
	}

	Vector3 backupposition = mPosition;
	mPosition *= worldtransform;
	Vector3 backupdirection = mDirection;
	mDirection *= normaltransform;

	_float factor = (_float) elapse / 1000.0f;

	// The direction of force, compute by type of gravity.
	Vector3 forcedirection = Vector3::cOrigin, forcenormal = Vector3::cZAxis;

	switch ( mGravityType )
	{
		case IParticleSystem::_GRAVITY_POINT_CENTRIPETAL:
		case IParticleSystem::_GRAVITY_POINT_RADIAL:
		{
			forcedirection = ( mPosition - particle.mPosition ).Normalize( ) * mPower * factor;

			if ( mGravityType == IParticleSystem::_GRAVITY_POINT_RADIAL )
				forcedirection = - forcedirection;

			break;
		}

		case IParticleSystem::_GRAVITY_POINT_CIRCLING:
		case IParticleSystem::_GRAVITY_LINE_CIRCLING:
		{
			if ( mGravityType == IParticleSystem::_GRAVITY_POINT_CIRCLING )
			{
				forcedirection	= mPosition - particle.mPosition;
				forcenormal		= Vector3::Cross( forcedirection, particle.mDirection );
			}
			else
			{
				forcedirection	= Vector3::Plumb( mPosition - particle.mPosition, mDirection );
				forcenormal		= mDirection;
			}

			// Compute the radius of circle.
			_float radius = forcedirection.Magnitude( );

			// Compute the radian to circle around.
			_float radian = particle.mSpeed * factor / radius;

			// Build a rotation matrix.
			Matrix4 rotation; rotation.Rotation( forcenormal, radian );

			// Rotate particle position.
			particle.mPosition = mPosition + ( particle.mPosition - mPosition ) * rotation;

			// Rotate particle direction.
			particle.mDirection *= rotation;

			// Affect on normal too.
			if ( particle.mNormal != Vector3::cOrigin )
				particle.mNormal *= rotation;

			// Set flag, means no need to move by original direction. 
			particle.mInternalFlags = 1;

			mPosition = backupposition;
			mDirection = backupdirection;
			return;
		}

		case IParticleSystem::_GRAVITY_LINE_CENTRIPETAL:
		case IParticleSystem::_GRAVITY_LINE_RADIAL:
		{
			forcedirection = Vector3::Plumb( mPosition - particle.mPosition, mDirection ).Normalize( ) * mPower * factor;

			if ( mGravityType == IParticleSystem::_GRAVITY_LINE_RADIAL )
				forcedirection = - forcedirection;

			break;
		}

		case IParticleSystem::_GRAVITY_PLANE_CENTRIPETAL:
		case IParticleSystem::_GRAVITY_PLANE_RADIAL:
		{
			Plane forceplane( mPosition, mDirection );

			Vector3 position1 = particle.mPosition;
			Vector3 position2 = particle.mPosition + particle.mDirection * particle.mSpeed * factor;

			// Determine if particle move cross the plane.
			if ( forceplane.Distance( position1 ) * forceplane.Distance( position2 ) > 0.0f )
			{
				mPosition = backupposition;
				mDirection = backupdirection;
				return;
			}

			forcedirection = Vector3::Plumb( mPosition - particle.mPosition, mDirection ).Normalize( ) * mPower * factor;

			//forcedirection = ( mPosition - position ).Normalize( ) * mPower * factor;

			if ( mGravityType == IParticleSystem::_GRAVITY_PLANE_RADIAL )
				forcedirection = - forcedirection;

			break;
		}

		case IParticleSystem::_GRAVITY_PLANE_DIRECTIONAL:
		{
			forcedirection = mDirection * mPower * factor;

			break;
		}

		case IParticleSystem::_GRAVITY_RANDOM:
		{
			forcedirection.x = Math::Random( -1.0f, 1.0f );
			forcedirection.y = Math::Random( -1.0f, 1.0f );
			forcedirection.z = Math::Random( -1.0f, 1.0f );

			forcedirection = forcedirection.Normalize( ) * mPower * factor;

			break;
		}
	}

	// Get scale transform
	Vector3 normalvector = Vector3( 1.0f, 1.0f, 1.0f );
	static _float normallength = ( normalvector - Vector3( 0.0f, 0.0f, 0.0f ) ).Magnitude( );
	normalvector *= normaltransform;
	_float scale = ( normalvector - Vector3( 0.0f, 0.0f, 0.0f ) ).Magnitude( ) / normallength;
	
	forcedirection /= scale;

	particle.mDirection	+= forcedirection;
	particle.mPosition	+= forcedirection * particle.mSpeed * factor;

	// Update particle normal.
	if ( particle.mNormal != Vector3::cOrigin )
		particle.mNormal = Vector3::Plumb( particle.mNormal, particle.mDirection ).Normalize( );

	mPosition = backupposition;
	mDirection = backupdirection;
}