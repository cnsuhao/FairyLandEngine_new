//============================================================================
// ParticleEmitter.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandGraphics.h"

//----------------------------------------------------------------------------
// ParticleEmitter Implementation
//----------------------------------------------------------------------------

ParticleEmitter::ParticleEmitter( _dword emitterid ) : mParticlePool( 100 )
{
	mEmitterID				= emitterid;
	mEmitterType			= IParticleSystem::_EMITTER_POINT;
	mDirectionType			= IParticleSystem::_DIRECTION_SAMEASEMITTER;
	mNormalType				= IParticleSystem::_NORMAL_NOTCARE;
	mEnabled				= _true;

	mCurrTickcount			= 0;
	mLastTickcount			= 0;
	mLastParticleID			= 0;

	mEndTickcount			= -1;

	mParticleType			= IParticleSystem::_PARTICLE_PRIMITIVEPOINT;
	mMinParticleDelay		= 0;
	mMaxParticleDelay		= 0;
	mMinParticleLife		= 1000;
	mMaxParticleLife		= 1000;
	mMinParticleSpeed		= 1.0f;
	mMaxParticleSpeed		= 1.0f;
	mMinParticleSpin		= 0.0f;
	mMaxParticleSpin		= 0.0f;
	mFixParticleColorRate	= _true;
	mMinParticleColor		= Color::cWhite;
	mMaxParticleColor		= Color::cWhite;
	mFixParticleSizeRate	= _true;
	mMinParticleSize		= Vector3( 1.0f, 1.0f, 1.0f );
	mMaxParticleSize		= Vector3( 1.0f, 1.0f, 1.0f );
	mParticleBindEmitterID	= 0;
	mParticleBindGravityID	= 0;

	mEmitRate				= 1000;
	mEmitDelay				= 0;
	mEmitNumber				= 1;
	mSizeInner				= Vector3( 10.0f, 10.0f, 10.0f );
	mSizeOuter				= Vector3( 10.0f, 10.0f, 10.0f );
	mPosition				= Vector3::cOrigin;
	mRotation				= EulerAngles::cIdentity;
	mRotationMatrix			= Matrix4::cIdentity;

	mRenderMethod			= _null;
	mAttachedTagPoint[0]	= _null;
	mAttachedTagPoint[1]	= _null;
	mAttachedMesh			= _null;
	mAttachedSkeleton		= _null;
	
	mTexture				= _null;

	mSkeleton				= _null;
	mMesh					= _null;
	mSkeletonAnimation		= _null;

	mLength					= 0;

	mRandomFrame			= _true;
	mColumn					= 1;
	mRow					= 1;
	mTime					= 30;
	mCurrRow				= 0;
	mCurrColumn				= 0;
	mLastFrameTickcount		= 0;

	mSpeed					= 1.0f;
	mScale					= 1.0f;

	mParticleRotation		= Vector3( 0.0f, 0.0f, 0.0f );
	mParticleRotateSpeed	= Vector3( 0.0f, 0.0f, 0.0f );

	mParticleSystem			= _null;

	mEmitWorldtransform		= 1;

	mMarkerTransform		= 0;

	mStripExtend			= 0;

	mLastEmitPosition		= Vector3::cMaxVector;
	mEmitByDistance			= 0.0f;
}

ParticleEmitter::~ParticleEmitter( )
{
	GetGeometryFactory( ).ReleaseGeometry( mGeometry );
	GetGeometryFactory( ).ReleaseGeometry( mAttachedGeometry );

	GetPhysicsFactory( ).ReleaseTagPoint( mAttachedTagPoint[0] );
	GetPhysicsFactory( ).ReleaseTagPoint( mAttachedTagPoint[1] );

	GetTechniqueFactory( ).ReleaseRenderMethod( mRenderMethod );

	GetTextureFactory( ).ReleaseTexture( mTexture );

	GetModelFactory( ).ReleaseSkeleton( mSkeleton );
	GetModelFactory( ).ReleaseSkeleton( mAttachedSkeleton );

	GetModelFactory( ).ReleaseMesh( mMesh );
	GetModelFactory( ).ReleaseMesh( mAttachedMesh );

	GetModelFactory( ).ReleaseSkeletonAnimation( mSkeletonAnimation );
}

_void ParticleEmitter::SetParticleProperty( Particle& particle )
{
	particle.mParticleType		= mParticleType;
	particle.mInternalFlags		= 0;
	particle.mCurrTickcount		= 0;
	particle.mLastTickcount		= 0;
	particle.mPrevParticleID	= 0;
	particle.mUserRotation		= mParticleRotation;

	particle.mTexture			= mTexture;

	particle.mSkeleton			= mSkeleton;
	particle.mMesh				= mMesh;
	particle.mSkeletonAnimation = mSkeletonAnimation;

	particle.mDelay = Math::Random( mMinParticleDelay, mMaxParticleDelay );

	// Random particle life.
	particle.mLife = Math::Random( mMinParticleLife, mMaxParticleLife );

	// Random particle speed.
	particle.mSpeed = Math::Random( mMinParticleSpeed, mMaxParticleSpeed );
	particle.mEmitSpeed = particle.mSpeed;

	// Random particle spin.
	particle.mSpin = Math::Random( mMinParticleSpin, mMaxParticleSpin );
	particle.mEmitSpin = particle.mSpin;

	// Random particle color.
	if ( mFixParticleColorRate == _true )
	{
		particle.mColor = Color::Lerp( mMinParticleColor, mMaxParticleColor, Math::Random( 0.0f, 1.0f ) );
	}
	else
	{
		particle.mColor = Color( Math::Random( mMinParticleColor.r, mMaxParticleColor.r ), 
								 Math::Random( mMinParticleColor.g, mMaxParticleColor.g ), 
								 Math::Random( mMinParticleColor.b, mMaxParticleColor.b ), 
								 Math::Random( mMinParticleColor.a, mMaxParticleColor.a ) );
	}
	particle.mEmitColor = particle.mColor;

	// Random particle size.
	if ( mFixParticleSizeRate == _true )
	{
		particle.mSize = Vector3::Lerp( mMinParticleSize, mMaxParticleSize, Math::Random( 0.0f, 1.0f ) );
	}
	else
	{
		particle.mSize = Vector3( Math::Random( mMinParticleSize.x, mMaxParticleSize.x ),
								  Math::Random( mMinParticleSize.y, mMaxParticleSize.y ),
								  Math::Random( mMinParticleSize.z, mMaxParticleSize.z ) );
	}

	particle.mEmitSize = particle.mSize;

	// 2D frame 
	if ( mRandomFrame )
	{
		particle.mCurrColumn = Math::Random( 0, mColumn - 1 );
		particle.mCurrRow	 = Math::Random( 0, mRow - 1 );
	}
	else
	{
		particle.mCurrColumn = 0;
		particle.mCurrRow	 = 0;
	}
	particle.mLastFrameTickcount	= 0;
	particle.mStopTickCount			= 0;

	// Emitter scale
	particle.mSpeed		*= mScale;
	particle.mEmitSpeed *= mScale;
	particle.mSpin		*= mScale;
	particle.mEmitSpin	*= mScale;
	particle.mSize		*= mScale;
	particle.mEmitSize  *= mScale;
}

_void ParticleEmitter::SetParticlePosition( Particle& particle )
{
	switch ( mEmitterType )
	{	
		case IParticleSystem::_EMITTER_POINT:
		case IParticleSystem::_EMITTER_STATIC:
		{
			// Set particle position.
			particle.mPosition = mPosition;

			break;
		}
		
		case IParticleSystem::_EMITTER_BOX:
		{
			_float x = Math::Random( -mSizeOuter.x, mSizeOuter.x ) * 0.5f;
			_float y = Math::Random( -mSizeOuter.y, mSizeOuter.y ) * 0.5f;
			_float z = Math::Random( -mSizeOuter.z, mSizeOuter.z ) * 0.5f;

			if ( Math::Abs( x ) < mSizeInner.x * 0.5f && Math::Abs( y ) < mSizeInner.y * 0.5f && Math::Abs( z ) < mSizeInner.z * 0.5f )
			{
				switch ( Math::Random( 3 ) )
				{
					case 0: x = Math::Random( mSizeInner.x, mSizeOuter.x ) * Math::RandomSign( ) * 0.5f; break;
					case 1: y = Math::Random( mSizeInner.y, mSizeOuter.y ) * Math::RandomSign( ) * 0.5f; break;
					case 2: z = Math::Random( mSizeInner.z, mSizeOuter.z ) * Math::RandomSign( ) * 0.5f; break;
				}
			}

			// Random particle position.
			particle.mPosition = mPosition + Vector3( x, y, z ) * mRotationMatrix;

			break;
		}

		case IParticleSystem::_EMITTER_CONE:
		{
			_float angle	= Math::Random( 0.0f, Math::c2Pi );
			_float height	= Math::Random( 0.0f, mSizeOuter.z );
			_float radius	= Math::Random( 0.0f, 1.0f );
			_float radio	= mSizeOuter.z != 0.0f ? height / mSizeOuter.z : 0.0f;

			_float radiusx  = 0.0f;
			_float radiusy	= 0.0f;

			if ( height > mSizeInner.z )
			{
				radiusx = mSizeInner.x + ( mSizeOuter.x - mSizeInner.x ) * radius;
				radiusy	= mSizeInner.y + ( mSizeOuter.y - mSizeInner.y ) * radius;
			}
			else
			{
				radiusx = mSizeOuter.x * radius;
				radiusy	= mSizeOuter.y * radius;
			}

			radiusx *= radio;
			radiusy *= radio;

			// Random particle position.
			particle.mPosition = mPosition + Vector3( Math::Sin( angle ) * radiusx, Math::Cos( angle ) * radiusy, height ) * mRotationMatrix;

			break;
		}

		case IParticleSystem::_EMITTER_CIRCLE:
		{
			_float angle	= Math::Random( 0.0f, Math::c2Pi );
			_float height	= Math::Random( -mSizeOuter.z, mSizeOuter.z );
			_float radius	= Math::Random( 0.0f, 1.0f );

			_float radiusx  = 0.0f;
			_float radiusy	= 0.0f;

			if ( Math::Abs( height ) <= mSizeInner.z )
			{
				radiusx = mSizeInner.x + ( mSizeOuter.x - mSizeInner.x ) * radius;
				radiusy	= mSizeInner.y + ( mSizeOuter.y - mSizeInner.y ) * radius;
			}
			else
			{
				radiusx = mSizeOuter.x * radius;
				radiusy	= mSizeOuter.y * radius;
			}

			// Random particle position.
			particle.mPosition = mPosition + Vector3( Math::Sin( angle ) * radiusx, Math::Cos( angle ) * radiusy, height ) * mRotationMatrix;

			break;
		}

		case IParticleSystem::_EMITTER_SPHERE:
		case IParticleSystem::_EMITTER_HEMISPHERE:
		{
			// Make more particle at equator
			static _float randomarray[10] = { 0.0f, 0.05f, 0.05f, 0.1f, 0.1f, 
											  0.2f, 0.2f, 0.3f, 0.3f, 0.4f };
			_float angle0	= Math::Random( Math::cPi * randomarray[Math::Random( _dword( 0 ), _dword( 9 ) )], Math::cPi * 0.5f );
			_float angle1	= Math::Random( 0.0f, Math::c2Pi );
			_float angle2	= Math::Random( 0.0f, Math::cPi );
			_float radius	= Math::Random( mSizeInner.x, mSizeOuter.x );
			_float radius1	= Math::Sin( angle0 ) * radius;
			_float radius2	= Math::Cos( angle0 ) * radius;

			// If emitter is full sphere, make both half
			if ( mEmitterType == IParticleSystem::_EMITTER_SPHERE )
				radius2 *= Math::RandomSign( );

			// Random particle position.
			particle.mPosition = mPosition + Vector3( Math::Sin( angle1 ) * radius1, Math::Cos( angle1 ) * radius1, radius2 ) * mRotationMatrix;

			break;
		}

		case IParticleSystem::_EMITTER_MESH:
		{
			if ( mAttachedMesh == _null || mAttachedMesh->GetModelNumber( ) == 0 )
				break;

			_dword modelindex = Math::Random( mAttachedMesh->GetModelNumber( ) );

			Geometry geometry = mAttachedMesh->GetModelGeometry( modelindex );

			if ( geometry.mVertexBuffer == _null || geometry.mVertexSize == 0 )
				break;

			_dword vertexindex = Math::Random( geometry.mVertexBuffer->GetSize( ) / geometry.mVertexSize );

			Vector3* modelvertex = (Vector3*) geometry.mVertexBuffer->Lock( vertexindex * geometry.mVertexSize, geometry.mVertexSize );
			if ( modelvertex != _null )
			{
				particle.mPosition	= *modelvertex;
				particle.mNormal	= Vector3::cOrigin;
			}
			geometry.mVertexBuffer->Unlock( );

			break;
		}

		case IParticleSystem::_EMITTER_GEOMETRY:
		{
			if ( mAttachedGeometry.mVertexBuffer == _null || mAttachedGeometry.mVertexSize == 0 )
				break;

			_dword vertexindex = Math::Random( mAttachedGeometry.mVertexBuffer->GetSize( ) / mAttachedGeometry.mVertexSize );

			Vector3* modelvertex = (Vector3*) mAttachedGeometry.mVertexBuffer->Lock( vertexindex * mAttachedGeometry.mVertexSize, mAttachedGeometry.mVertexSize );
			if ( modelvertex != _null )
			{
				particle.mPosition	= *modelvertex;
				particle.mNormal	= Vector3::cOrigin;
			}
			mAttachedGeometry.mVertexBuffer->Unlock( );

			break;
		}

		case IParticleSystem::_EMITTER_STATICSKIN:
		{
			particle.mPosition	= Vector3::cOrigin;
			particle.mNormal	= Vector3::cXAxis;

			break;
		}
	}
}

_void ParticleEmitter::SetParticleDirection( Particle& particle )
{
	// Compute the direction of emitter by rotate zaxis.
	Vector3 direction = Vector3::cZAxis * mRotationMatrix;

	// Compute the center point by project particle position onto emitter direction.
	Vector3 center = particle.mPosition != mPosition ? Vector3::Project( particle.mPosition - mPosition, direction ) + mPosition : mPosition;

	_bool randomnormal = _true;

	// Set particle direction by emitter.
	switch ( mDirectionType )
	{
		case IParticleSystem::_DIRECTION_SAMEASEMITTER:
		case IParticleSystem::_DIRECTION_REVERSEFROMEMITTER:
		{
			particle.mDirection	= direction;

			// Reverse direction.
			if ( mDirectionType == IParticleSystem::_DIRECTION_REVERSEFROMEMITTER )
				particle.mDirection	= - particle.mDirection;

			// Dont care normal.
			if ( mNormalType == IParticleSystem::_NORMAL_NOTCARE )
				break;

			// Cant compute normal, just skip it now.
			if ( particle.mPosition == center )
				break;

			Vector3 temp = particle.mPosition - center;

			// Set particle normal by emitter.
			if ( mNormalType == IParticleSystem::_NORMAL_FORWARD )
				particle.mNormal = Vector3::Cross( particle.mDirection, temp );
			else if ( mNormalType == IParticleSystem::_NORMAL_BACKWARD )
				particle.mNormal = - Vector3::Cross( particle.mDirection, temp );
			else if ( mNormalType == IParticleSystem::_NORMAL_OUTSIDE )
				particle.mNormal = temp;
			else if ( mNormalType == IParticleSystem::_NORMAL_INSIDE )
				particle.mNormal = - temp;
			else
				break;

			randomnormal = _false;

			break;
		}

		case IParticleSystem::_DIRECTION_POINT_CENTRIPETAL:
		case IParticleSystem::_DIRECTION_POINT_RADIAL:
		{
			if ( particle.mPosition == mPosition )
				particle.mDirection = -direction;
			else
				particle.mDirection	= mPosition - particle.mPosition;

			// Reverse direction.
			if ( mDirectionType == IParticleSystem::_DIRECTION_POINT_RADIAL )
				particle.mDirection	= - particle.mDirection;

			// Dont care normal.
			if ( mNormalType == IParticleSystem::_NORMAL_NOTCARE )
				break;

			// Cant compute normal, just skip it now.
			if ( particle.mPosition == mPosition )
				break;

			Vector3 temp = Vector3::Plumb( direction, particle.mDirection );

			// Set particle normal by emitter.
			if ( mNormalType == IParticleSystem::_NORMAL_FORWARD )
				particle.mNormal = temp;
			else if ( mNormalType == IParticleSystem::_NORMAL_BACKWARD )
				particle.mNormal = - temp;
			else if ( mNormalType == IParticleSystem::_NORMAL_OUTSIDE )
				particle.mNormal = Vector3::Cross( particle.mDirection, temp );
			else if ( mNormalType == IParticleSystem::_NORMAL_INSIDE )
				particle.mNormal = - Vector3::Cross( particle.mDirection, temp );
			else
				break;

			randomnormal = _false;

			break;
		}

		case IParticleSystem::_DIRECTION_POINT_TANGENT:
		case IParticleSystem::_DIRECTION_POINT_REVERSETANGENT:
		{
			if ( particle.mPosition == center )
				particle.mDirection = direction;
			else
				particle.mDirection	= Vector3::Cross( center - particle.mPosition, direction );

			// Reverse direction.
			if ( mDirectionType == IParticleSystem::_DIRECTION_POINT_REVERSETANGENT )
				particle.mDirection	= - particle.mDirection;

			// Dont care normal.
			if ( mNormalType == IParticleSystem::_NORMAL_NOTCARE )
				break;

			// Cant compute normal, just skip it now.
			if ( particle.mPosition == center )
				break;

			Vector3 temp = Vector3::Plumb( direction, mPosition - particle.mPosition );

			// Set particle normal by emitter.
			if ( mNormalType == IParticleSystem::_NORMAL_FORWARD )
				particle.mNormal = temp;
			else if ( mNormalType == IParticleSystem::_NORMAL_BACKWARD )
				particle.mNormal = - temp;
			else if ( mNormalType == IParticleSystem::_NORMAL_OUTSIDE )
				particle.mNormal = Vector3::Cross( particle.mDirection, temp );
			else if ( mNormalType == IParticleSystem::_NORMAL_INSIDE )
				particle.mNormal = - Vector3::Cross( particle.mDirection, temp );
			else
				break;

			randomnormal = _false;

			break;
		}

		case IParticleSystem::_DIRECTION_LINE_CENTRIPETAL:
		case IParticleSystem::_DIRECTION_LINE_RADIAL:
		{
			if ( particle.mPosition == center )
				particle.mDirection = direction;
			else
				particle.mDirection	= center - particle.mPosition;

			// Reverse direction.
			if ( mDirectionType == IParticleSystem::_DIRECTION_LINE_RADIAL )
				particle.mDirection	= - particle.mDirection;

			// Dont care normal.
			if ( mNormalType == IParticleSystem::_NORMAL_NOTCARE )
				break;

			// Cant compute normal, just skip it now.
			if ( particle.mPosition == center )
				break;

			// Set particle normal by emitter.
			if ( mNormalType == IParticleSystem::_NORMAL_FORWARD )
				particle.mNormal = direction;
			else if ( mNormalType == IParticleSystem::_NORMAL_BACKWARD )
				particle.mNormal = - direction;
			else if ( mNormalType == IParticleSystem::_NORMAL_OUTSIDE )
				particle.mNormal = Vector3::Cross( particle.mDirection, direction );
			else if ( mNormalType == IParticleSystem::_NORMAL_INSIDE )
				particle.mNormal = - Vector3::Cross( particle.mDirection, direction );
			else
				break;

			randomnormal = _false;

			break;
		}

		case IParticleSystem::_DIRECTION_LINE_TANGENT:
		case IParticleSystem::_DIRECTION_LINE_REVERSETANGENT:
		{
			if ( particle.mPosition == center )
				particle.mDirection = direction;
			else
				particle.mDirection	= Vector3::Cross( center - particle.mPosition, direction );

			// Reverse direction.
			if ( mDirectionType == IParticleSystem::_DIRECTION_LINE_REVERSETANGENT )
				particle.mDirection	= - particle.mDirection;

			// Dont care normal.
			if ( mNormalType == IParticleSystem::_NORMAL_NOTCARE )
				break;

			// Cant compute normal, just skip it now.
			if ( particle.mPosition == center )
				break;

			// Set particle normal by emitter.
			if ( mNormalType == IParticleSystem::_NORMAL_FORWARD )
				particle.mNormal = direction;
			else if ( mNormalType == IParticleSystem::_NORMAL_BACKWARD )
				particle.mNormal = - direction;
			else if ( mNormalType == IParticleSystem::_NORMAL_OUTSIDE )
				particle.mNormal = Vector3::Cross( particle.mDirection, direction );
			else if ( mNormalType == IParticleSystem::_NORMAL_INSIDE )
				particle.mNormal = - Vector3::Cross( particle.mDirection, direction );
			else
				break;

			randomnormal = _false;

			break;
		}

		case IParticleSystem::_DIRECTION_RANDOM:
		{
			particle.mDirection.x = Math::Random( -1.0f, 1.0f );
			particle.mDirection.y = Math::Random( -1.0f, 1.0f );
			particle.mDirection.z = Math::Random( -1.0f, 1.0f );

			break;
		}
	}

	// Normalize particle direction.
	particle.mDirection.Normalize( );

	if ( mNormalType == IParticleSystem::_NORMAL_NOTCARE )
	{
		particle.mNormal = Vector3( 0.0f, 0.0f, 1.0f );//Vector3::cOrigin;
	}
	else
	{
		if ( mNormalType == IParticleSystem::_NORMAL_FIXED )
		{
			Vector3 normal = Vector3::cXAxis;

			particle.mNormal = normal * Quaternion( ).Rotation( Vector3::cZAxis, particle.mDirection );
		}
		// Random particle normal.
		else if ( randomnormal )
		{ 
			Vector3 normal = Vector3::cXAxis * Matrix4( ).RotationZ( Math::Random( 0.0f, Math::c2Pi ) );

			particle.mNormal = normal * Quaternion( ).Rotation( Vector3::cZAxis, particle.mDirection );
		}

		// Normalize particle normal.
		particle.mNormal.Normalize( );
	}

	Matrix4 scaletransform;
	scaletransform.Scaling( mScale, mScale, mScale );
	particle.mPosition  *= scaletransform;
	particle.mNormal    *= scaletransform;
	particle.mDirection *= scaletransform;
}

_dword ParticleEmitter::GetAvailableParticleNumber( )
{
	// No particle to bind
	if ( mParticleBindEmitterID != 0 )
	{
		ParticleEmitter* emitter = mParticleSystem->SearchEmitter( mParticleBindEmitterID );
		if ( emitter == _null )
			return 0;
		else if ( emitter->mParticlePool.Number( ) == 0 )
			return 0;
	}
	// Disable
	if ( mEnabled == _false )
		return 0;

	// User Emitter
	if ( mEmitterType == IParticleSystem::_EMITTER_USER )
		return 0;

	// Max particle number reached.
	if ( mParticlePool.Number( ) >= mParticlePool.Size( ) )
		return 0;

	// Time not elapse.
	if ( mCurrTickcount == mLastTickcount )
		return 0;

	// Not ready to emit first particle.
	if ( mCurrTickcount <= mEmitDelay )
		return 0;

	// Out of end tickcount
	if ( mCurrTickcount >= mEndTickcount )
		return 0;

	_dword emitnumber = 0;

	// It's time to first emit.
	if ( mLastTickcount <= mEmitDelay )
		emitnumber += mEmitNumber;

	// Maybe still need to emit some times.
	if ( mLastTickcount > mEmitDelay && mEmitRate > 0 )
	{
		_dword temp1 = ( mLastTickcount - mEmitDelay ) / mEmitRate;
		_dword temp2 = ( mCurrTickcount - mEmitDelay ) / mEmitRate;
		
		emitnumber += ( temp2 - temp1 ) * mEmitNumber;
	}

	// Emit by distance
	if ( mEmitByDistance != 0.0f )
	{
		Vector3 EBDdirection = Vector3::cZAxis;
		_float	EBDdistance  = 0.0f;

		Particle newparticle;
		CreateParticle( newparticle );

		EBDdirection = newparticle.mPosition - mLastEmitPosition;
		EBDdistance  = EBDdirection.Magnitude( );
		emitnumber	 = _dword( EBDdistance / mEmitByDistance );

		if ( emitnumber > 30 )
			emitnumber = 30;
	}

	// Not exceed max particle number.
	if ( mParticlePool.Number( ) + emitnumber >= mParticlePool.Size( ) )
		emitnumber = mParticlePool.Size( ) - mParticlePool.Number( );

	return emitnumber;
}

_void ParticleEmitter::CreateParticle( Particle& particle )
{
	SetParticleProperty( particle );
	SetParticlePosition( particle );
	SetParticleDirection( particle );

	Matrix4 worldtransform  = GetRenderDevice( ).GetWorldTransform( );
	Matrix3 normaltransform = GetRenderDevice( ).GetNormalTransform( );

	// Get scale transform
	Vector3 normalvector = Vector3( 1.0f, 1.0f, 1.0f );

	_float normallength = ( normalvector - Vector3( 0.0f, 0.0f, 0.0f ) ).Magnitude( );
	normalvector *= normaltransform;

	_float scale = ( normalvector - Vector3( 0.0f, 0.0f, 0.0f ) ).Magnitude( ) / normallength;

	Matrix4 scaletransform = Matrix4( ).Scaling( scale, scale, scale );

	if ( mEmitWorldtransform == 0 )
	{
		worldtransform = Matrix4::cIdentity;
		normaltransform = Matrix3::cIdentity;
		scaletransform = Matrix4::cIdentity;
		scale = 1.0f;
	}

	if ( mAttachedTagPoint[0] != _null && mAttachedTagPoint[1] != _null )
	{
		Vector3 point1 = mAttachedTagPoint[0]->GetPosition( );
		Vector3 point2 = mAttachedTagPoint[1]->GetPosition( );

		Vector3 pointtopoint = point2 - point1;
		_float	size = pointtopoint.Magnitude( );

		if ( mStripExtend == 0 )
		{
			particle.mPosition = point1 + pointtopoint.Normalize( ) * ( size / 2.0f );
			particle.mSize.x = size;
		}
		else if ( mStripExtend == 1 || mStripExtend == 2 )
		{
			particle.mPosition = point1;
		}
		else if ( mStripExtend == 3 || mStripExtend == 4 )
		{
			particle.mPosition = point2;
		}

		particle.mDirection = pointtopoint.Normalize( );
	}
	else if ( mAttachedTagPoint[0] != _null )
	{
		_bool onlyz = _false;
		for ( _dword i = 0; i < mGravityArray.Number( ); i ++ )
		{
			ParticleGravity* gravity = mParticleSystem->SearchGravity( mGravityArray[i] );
			if ( gravity == _null )
				continue;

			if ( gravity->mGravityType == IParticleSystem::_GRAVITY_POINT_CIRCLING ||
				 gravity->mGravityType == IParticleSystem::_GRAVITY_LINE_CIRCLING )
			{
				onlyz = _true;
			}
		}

		if ( onlyz )
		{
			particle.mPosition.z += mAttachedTagPoint[0]->GetPosition( ).z;
		}
		else
		{
			particle.mPosition	+= mAttachedTagPoint[0]->GetPosition( );
		}
		
		if ( particle.mParticleType == IParticleSystem::_PARTICLE_PRIMITIVELINESTRIP
			|| particle.mParticleType == IParticleSystem::_PARTICLE_PRIMITIVEIMAGESTRIP )
		{
			particle.mDirection	 = mAttachedTagPoint[0]->GetDirection( );
		}
		else if ( mMarkerTransform == 1 )
		{
			Quaternion rotation = Quaternion( ).Rotation( -Vector3::cYAxis, mAttachedTagPoint[0]->GetDirection( ) );
			particle.mDirection *= Matrix3( ).FromRotation( rotation );
		}
	}
	// This emitter is binded to another emitter
	else if ( mParticleBindEmitterID != 0 )
	{
		ParticleEmitter* emitter = mParticleSystem->SearchEmitter( mParticleBindEmitterID );

		_dword number = emitter->mParticlePool.Number( );
		if ( number == 0 )
			return;

		_dword breakindex = Math::Random( 0, number - 1 ), index = 0;

		for ( ParticleEmitter::ParticlePool::Iterator it = emitter->mParticlePool.GetHeadIterator( ); it.Valid( ); it ++ )
		{
			Particle& emitterparticle = it;
			
			if ( index == breakindex )
			{
				particle.mPosition *= Matrix4( ).FromTranslationRotation( emitterparticle.mPosition, emitterparticle.mEmitWorldRotation );
				particle.mDirection *= Matrix4( ).FromTranslationRotation( Vector3::cOrigin, emitterparticle.mEmitWorldRotation );
				return;
			}

			index ++;
		}
	}

	particle.mPosition	*= worldtransform;

	particle.mDirection	*= normaltransform;
	particle.mDirection.Normalize( );

	particle.mNormal	*= normaltransform;
	particle.mNormal.Normalize( );

	normaltransform.ToRotation( particle.mEmitWorldRotation );

 	particle.mSize		*= scaletransform;
	particle.mEmitSize	*= scaletransform;
	particle.mSpeed		*= scale;
	particle.mEmitSpeed	*= scale;
	particle.mSpin		*= scale;
	particle.mEmitSpin	*= scale;
}