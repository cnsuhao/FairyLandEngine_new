//============================================================================
//Camera.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Camera Implementation
//----------------------------------------------------------------------------

const Camera Camera::cNull( Vector3::cOrigin, Vector3::cOrigin, Vector3::cOrigin, 0.0f );

Camera::Camera( )
{
	mPosition	= Vector3( 100.0f, 100.0f, 100.0f );
	mLook		= Vector3::cOrigin;
	mUp			= Vector3::cZAxis;
	mRight		= Vector3::Cross( mUp, mLook - mPosition ).Normalize( );
	mFOVDegree	= 58.0f;// 58.0 / 1.3 ~= PI / 4( old value ) 
}

Camera::Camera( const Vector3& position, const Vector3& look, const Vector3& up, _float fovdegree )
{
	mPosition	= position;
	mLook		= look;
	mUp			= up;
	mRight		= Vector3::Cross( mUp, mLook - mPosition ).Normalize( );
	mFOVDegree	= fovdegree;// 58.0 / 1.3 ~= PI / 4( old value )
}

_void Camera::LookAt( const Vector3& look )
{
	Vector3 temp = mLook;

	mLook		= look;
	mPosition	= mLook + ( mPosition - temp );
	mRight		= Vector3::Cross( mUp, mLook - mPosition ).Normalize( );
}

_void Camera::Strafe( _float units )
{
	mLook		+= mRight * units;
	mPosition	+= mRight * units;
}

_void Camera::Fly( _float units )
{
	mLook		+= mUp * units;
	mPosition	+= mUp * units;
}

_void Camera::Radius( _float units )
{
	Vector3 direction = mPosition - mLook;

	Vector3 position = mPosition + direction.Normalize( ) * units;

	// Avoid #ind value
	if ( position.Equal( mLook ) )
		return;

	if ( Vector3::Dot( mPosition - mLook, position - mLook ) > 0.0f )
		mPosition = position;
}

_void Camera::Phi( _float radian )
{
	mPosition *= Matrix4( ).Translation( - mLook ) *
		Matrix4( ).Rotation( mUp, radian ) * Matrix4( ).Translation( mLook );

	mRight = Vector3::Cross( mUp, mLook - mPosition ).Normalize( );
}

_void Camera::Theta( _float radian )
{
	Vector3 vector1 = Vector3::Cross( mPosition - mLook, mRight );

	mPosition *= Matrix4( ).Translation( - mLook ) *
		Matrix4( ).Rotation( mRight, radian ) * Matrix4( ).Translation( mLook );

	Vector3 vector2 = Vector3::Cross( mPosition - mLook, mRight );

	if ( Vector3::Dot( vector1, mUp ) * Vector3::Dot( vector2, mUp ) < 0.0f )
		mUp = - mUp;
}

_void Camera::PhiReverse( _float radian )
{
	mLook *= Matrix4( ).Translation( - mPosition ) *
		Matrix4( ).Rotation( mUp, radian ) * Matrix4( ).Translation( mPosition );

	mRight = Vector3::Cross( mUp, mLook - mPosition ).Normalize( );
}

_void Camera::ThetaReverse( _float radian )
{
	Vector3 vector1 = Vector3::Cross( mLook - mPosition , mRight );

	mLook *= Matrix4( ).Translation( - mPosition ) *
		Matrix4( ).Rotation( mRight, radian ) * Matrix4( ).Translation( mPosition );

	Vector3 vector2 = Vector3::Cross( mLook - mPosition, mRight );

	if ( Vector3::Dot( vector1, mUp ) * Vector3::Dot( vector2, mUp ) < 0.0f )
		mUp = - mUp;
}

_void  Camera::SetFOVDegree( _float degree )
{
	mFOVDegree = degree;
}

_float Camera::GetFOVDegree( ) const
{
	return mFOVDegree;
}

_float Camera::GetFOVYDegree( ) const
{
	// Fovy is need for D3D
	return mFOVDegree / 1.3f;
}

Camera& Camera::operator *= ( const Matrix4& transform )
{
	Vector3 translation;
	Matrix3 rotation;
	transform.ToTranslationRotation( translation, rotation );

	mPosition	*= transform;
	mLook		*= transform;
	mUp			*= rotation;
	mRight		 = Vector3::Cross( mUp, mLook - mPosition ).Normalize( );

	return *this;
}

Camera Camera::operator * ( const Matrix4& transform ) const
{
	Camera returncamera = *this;

	return returncamera *= transform;
}

// ===============================================================
// CameraControl lerp camera
// ===============================================================

CameraControl::CameraControl( const Camera& source, _float speed, const Vector3& positionweight, const Vector3& lookweight ) : 
	mSpeed( speed ),
	mLinearLerp( _true ),
	mPositionWeight( positionweight ), 
	mLookWeight( lookweight ), 
	mDuration( 200 ), 
	mTick( 0 ), 
	mSourceCamera( source ), 
	mTargetCamera( source ), 
	mCurrentCamera( source )
{
}

CameraControl::~CameraControl( )
{
}

_void CameraControl::SetLinearLerp( _bool linear )
{
	mLinearLerp = linear;
}

_void CameraControl::SetCameraSpeed( _float speed )
{
	mSpeed = speed;
}

_float CameraControl::GetCameraSpeed( ) const
{
	return mSpeed;
}

_void  CameraControl::SetPositionWeight( const Vector3& weight )
{
	mPositionWeight = weight;
}

const Vector3& CameraControl::GetPositionWeight( ) const
{
	return mPositionWeight;
}

_void  CameraControl::SetLookWeight( const Vector3& weight )
{
	mLookWeight = weight;
}

const Vector3& CameraControl::GetLookWeight( ) const
{
	return mLookWeight;
}

_dword CameraControl::GetCurrentTick( ) const
{
	return mTick;
}

_void CameraControl::SetTargetCamera( const Camera& target, _bool lerp )
{
	if ( lerp )
	{
		mTick = 0;
		mTargetCamera = target;
		mSourceCamera = mCurrentCamera;

		_float positiondistance = Vector3( mSourceCamera.mPosition - mTargetCamera.mPosition ).MagnitudeSquared( );
		_float lookdistance = Vector3( mSourceCamera.mLook - mTargetCamera.mLook ).MagnitudeSquared( );
		_float maxdistance = Math::Sqrt( Math::Max( positiondistance, lookdistance ) );
		mDuration = _dword( maxdistance / mSpeed );
		mTick	  = mDuration;
	}
	else
	{
		// To target directly
		mCurrentCamera = target;
		mTargetCamera = target;
		mSourceCamera = target;

		mDuration = 200;
		mTick	  = 0;
	}
}

const Camera& CameraControl::GetTargetCamera( ) const
{
	return mTargetCamera;
}

const Camera& CameraControl::GetCamera( ) const
{
	return mCurrentCamera;
}

_void CameraControl::Update( _dword elapse )
{
	if ( elapse == 0 )
		return;

	if ( mTick > elapse )
		mTick -= elapse;
	else 
		mTick = 0;
	
	_float backupalpha = 1.0f;
	// If mDuration == 0, mTick should be 0, then backupalpha == 1.0f
	if ( mTick != 0 )
	{
		backupalpha = ( 1.0f - _float( mTick ) / _float( mDuration ) );
	}

	//backupalpha = Math::Sqrt( backupalpha );
	
	if ( mLinearLerp )
	{
		// Apply weight in x y z direction
		_float alpha = Math::Clamp( backupalpha * mPositionWeight.x, 0.0f, 1.0f );
		_float positionx = mSourceCamera.mPosition.x * ( 1.0f - alpha ) + mTargetCamera.mPosition.x * alpha;
		alpha = Math::Clamp( backupalpha * mPositionWeight.y, 0.0f, 1.0f );
		_float positiony = mSourceCamera.mPosition.y * ( 1.0f - alpha ) + mTargetCamera.mPosition.y * alpha;
		alpha = Math::Clamp( backupalpha * mPositionWeight.z, 0.0f, 1.0f );
		_float positionz = mSourceCamera.mPosition.z * ( 1.0f - alpha ) + mTargetCamera.mPosition.z * alpha;

		alpha = Math::Clamp( backupalpha * mLookWeight.x, 0.0f, 1.0f );
		_float lookx = mSourceCamera.mLook.x * ( 1.0f - alpha ) + mTargetCamera.mLook.x * alpha;
		alpha = Math::Clamp( backupalpha * mLookWeight.y, 0.0f, 1.0f );
		_float looky = mSourceCamera.mLook.y * ( 1.0f - alpha ) + mTargetCamera.mLook.y * alpha;
		alpha = Math::Clamp( backupalpha * mLookWeight.z, 0.0f, 1.0f );
		_float lookz = mSourceCamera.mLook.z * ( 1.0f - alpha ) + mTargetCamera.mLook.z * alpha;

		Vector3 position( positionx, positiony, positionz );
		Vector3 look( lookx, looky, lookz );

		alpha = backupalpha;
		Vector3 up	= Vector3::Lerp( mSourceCamera.mUp, mTargetCamera.mUp, alpha );
		
		mCurrentCamera = Camera( position, look, up );
	}
	else 
	{
		if ( mTick == 0 )
		{
			mCurrentCamera = mTargetCamera;
		}
		else
		{
			Vector3 oldspherical = ( mSourceCamera.mLook - mSourceCamera.mPosition ).Cartesian2Spherical( );
			Vector3 newspherical = ( mTargetCamera.mLook - mTargetCamera.mPosition ).Cartesian2Spherical( );

			_float radius	= newspherical.x - oldspherical.x;
			_float theta	= newspherical.y - oldspherical.y;
			// Find near way
			_float theta2	= 2.0f * Math::cPi - Math::Abs( theta );
			if ( Math::Abs( theta ) > theta2 )
			{
				if ( theta > 0.0f )
				{
					theta = -theta2;
				}
				else
				{
					theta = theta2;
				}
			}
			_float phi		= newspherical.z - oldspherical.z;
			// Find near way
			_float phi2		= 2.0f * Math::cPi - Math::Abs( phi );
			if ( Math::Abs( phi ) > phi2 )
			{
				if ( phi > 0.0f )
				{
					phi = -phi2;
				}
				else
				{
					phi = phi2;
				}
			}

			mCurrentCamera = mSourceCamera;
			mCurrentCamera.Radius( radius * backupalpha * mPositionWeight.x );
			mCurrentCamera.Theta( theta * backupalpha * mPositionWeight.y );
			mCurrentCamera.Phi( phi * backupalpha * mPositionWeight.z );

			_float alpha = Math::Clamp( backupalpha * mLookWeight.x, 0.0f, 1.0f );
			_float lookx = mSourceCamera.mLook.x * ( 1.0f - alpha ) + mTargetCamera.mLook.x * alpha;
			alpha = Math::Clamp( backupalpha * mLookWeight.y, 0.0f, 1.0f );
			_float looky = mSourceCamera.mLook.y * ( 1.0f - alpha ) + mTargetCamera.mLook.y * alpha;
			alpha = Math::Clamp( backupalpha * mLookWeight.z, 0.0f, 1.0f );
			_float lookz = mSourceCamera.mLook.z * ( 1.0f - alpha ) + mTargetCamera.mLook.z * alpha;

			mCurrentCamera.LookAt( Vector3( lookx, looky, lookz ) );
		}
	}
	
	_float	fov	= mSourceCamera.GetFOVDegree( ) * ( 1.0f - backupalpha ) + mTargetCamera.GetFOVDegree( ) * backupalpha;
	mCurrentCamera.SetFOVDegree( fov );
}