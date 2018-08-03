//============================================================================
// Camera.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Camera
//----------------------------------------------------------------------------

class Camera
{
public:
	const static Camera cNull;

public:
	Vector3	mPosition;
	Vector3	mLook;
	Vector3	mUp;
	Vector3	mRight;
	_float  mFOVDegree;

public:
	Camera( );
	Camera( const Vector3& position, const Vector3& look, const Vector3& up, _float fovdegree = 58.0f );

	_void LookAt( const Vector3& look );

	_void Strafe( _float units );
	_void Fly( _float units );

	_void Radius( _float units );
	_void Phi( _float radian );
	_void Theta( _float radian );

	_void PhiReverse( _float radian );
	_void ThetaReverse( _float radian );

	_void  SetFOVDegree( _float degree );
	_float GetFOVDegree( ) const;
	_float GetFOVYDegree( ) const;

	Camera& operator *= ( const Matrix4& transform );
	Camera operator * ( const Matrix4& transform ) const;

	inline _bool operator == ( const Camera& camera ) const;
	inline _bool operator != ( const Camera& camera ) const;
};

//----------------------------------------------------------------------------
// Camera Implementation
//----------------------------------------------------------------------------

_bool Camera::operator == ( const Camera& camera ) const
{
	return mPosition == camera.mPosition && mLook == camera.mLook && mUp == camera.mUp && mRight == camera.mRight && mFOVDegree == camera.mFOVDegree;
}

_bool Camera::operator != ( const Camera& camera ) const
{
	return mPosition != camera.mPosition || mLook != camera.mLook || mUp != camera.mUp || mRight != camera.mRight || mFOVDegree != camera.mFOVDegree;
}

//----------------------------------------------------------------------------
// CameraControl
//----------------------------------------------------------------------------

class CameraControl
{
private:
	// Camera move type
	_float	mSpeed;
	_bool	mLinearLerp;
	Vector3 mPositionWeight;
	Vector3 mLookWeight;

	// For lerp
	_dword	mTick;
	_dword  mDuration;

	Camera mTargetCamera;
	Camera mSourceCamera;

	Camera mCurrentCamera;

public:
	CameraControl( const Camera& source, _float speed, const Vector3& positionweight, const Vector3& lookweight );
	~CameraControl( );

public:
	// Use linear or radius lerp
	_void  SetLinearLerp( _bool linear );

	// Use this speed decide duration
	_void  SetCameraSpeed( _float speed );
	_float GetCameraSpeed( ) const;
	
	// Set speed weight in x y z direction
	_void  SetPositionWeight( const Vector3& weight );
	const Vector3& GetPositionWeight( ) const;
	_void  SetLookWeight( const Vector3& weight );
	const Vector3& GetLookWeight( ) const;
	
	_dword GetCurrentTick( ) const;

	// Set target camera, Use the max distance( look, position, up ) and speed decide duration
	_void SetTargetCamera( const Camera& target, _bool lerp = _true );
	// Get target camera
	const Camera& GetTargetCamera( ) const;
	// Get current camera
	const Camera& GetCamera( ) const;

	_void Update( _dword elapse );
};

};