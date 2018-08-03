//============================================================================
// Quaternion.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Quaternion Implementation
//----------------------------------------------------------------------------

const Quaternion Quaternion::cIdentity( 0.0f, 0.0f, 0.0f, 1.0f );

Quaternion& Quaternion::operator *= ( const Quaternion& quaternion )
{
	_float qx1 = x, qy1 = y, qz1 = z, qw1 = w;

	x = qw1 * quaternion.x + qx1 * quaternion.w + qy1 * quaternion.z - qz1 * quaternion.y;
	y = qw1 * quaternion.y - qx1 * quaternion.z + qy1 * quaternion.w + qz1 * quaternion.x;
	z = qw1 * quaternion.z + qx1 * quaternion.y - qy1 * quaternion.x + qz1 * quaternion.w;
	w = qw1 * quaternion.w - qx1 * quaternion.x - qy1 * quaternion.y - qz1 * quaternion.z;

	return *this;
}

Quaternion& Quaternion::operator /= ( const Quaternion& quaternion )
{
	Quaternion tempquaternion = *this;
	*this = -quaternion;

	return *this *= tempquaternion;
}

Quaternion Quaternion::operator * ( const Quaternion& quaternion ) const
{
	Quaternion returnquaternion = *this;

	return returnquaternion *= quaternion;
}

Quaternion Quaternion::operator / ( const Quaternion& quaternion ) const
{
	Quaternion returnquaternion = *this;

	return returnquaternion /= quaternion;
}

_bool Quaternion::Equal( const Quaternion& quaternion, _float margin ) const
{
	if ( Math::Abs( x - quaternion.x ) > margin )
		return _false;

	if ( Math::Abs( y - quaternion.y ) > margin )
		return _false;

	if ( Math::Abs( z - quaternion.z ) > margin )
		return _false;

	if ( Math::Abs( w - quaternion.w ) > margin )
		return _false;

	return _true;
}

Quaternion& Quaternion::Identity( )
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;

	return *this;
}

Quaternion& Quaternion::Normalize( )
{
	_float magnitude = Magnitude( );

	if ( magnitude > Math::cEpsilon )
	{
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}

	return *this;
}

_float Quaternion::Magnitude( ) const
{
	return Math::Sqrt( x * x + y * y + z * z + w * w );
}

_float Quaternion::MagnitudeSquared( ) const
{
	return x * x + y * y + z * z + w * w;
}

Quaternion& Quaternion::RotationX( _float radian )
{
	x = Math::Sin( radian * 0.5f );
	y = 0.0f;
	z = 0.0f;
	w = Math::Cos( radian * 0.5f );

	return *this;
}

Quaternion& Quaternion::RotationY( _float radian )
{
	x = 0.0f;
	y = Math::Sin( radian * 0.5f );
	z = 0.0f;
	w = Math::Cos( radian * 0.5f );

	return *this;
}

Quaternion& Quaternion::RotationZ( _float radian )
{
	x = 0.0f;
	y = 0.0f;
	z = Math::Sin( radian * 0.5f );
	w = Math::Cos( radian * 0.5f );

	return *this;
}

Quaternion& Quaternion::Rotation( const Vector3& axis, _float radian )
{
	_float sinvalue = Math::Sin( radian * 0.5f );

	x = sinvalue * axis.x;
	y = sinvalue * axis.y;
	z = sinvalue * axis.z;
	w = Math::Cos( radian * 0.5f );

	return *this;
}

Quaternion& Quaternion::Rotation( const Vector3& srcvector, const Vector3& desvector )
{
	Vector3 vector1 = srcvector;
	vector1.Normalize( );

	Vector3 vector2 = desvector;
	vector2.Normalize( );

	_float dotvalue = Vector3::Dot( vector1, vector2 );
	if ( dotvalue >= 1.0f - Math::cEpsilon )
		return ( *this = Quaternion::cIdentity );

	if ( dotvalue <= Math::cEpsilon - 1.0f )
		return Rotation( Vector3::cZAxis, Math::cPi );

	_float sqrtvalue = Math::Sqrt( ( 1.0f + dotvalue ) * 2.0f );
	if ( sqrtvalue < Math::cEpsilon )
		return ( *this = Quaternion::cIdentity );

    Vector3 axis = Vector3::Cross( vector1, vector2 );

	x = axis.x / sqrtvalue;
	y = axis.y / sqrtvalue;
	z = axis.z / sqrtvalue;
	w = sqrtvalue * 0.5f;

	return *this;
}

_void Quaternion::GetAxisAndAngle( Vector3& axis, _float& radian ) const
{
	_float length = x * x + y * y + z * z;

	if ( length > Math::cEpsilon )
	{
		length = Math::Sqrt( length );

		axis.x = x / length;
		axis.y = y / length;
		axis.z = z / length;
		radian = 2.0f * Math::Acos( w );
	}
	else
	{
		axis.x = 0.0f;
		axis.y = 0.0f;
		axis.z = 1.0f;
		radian = 0.0f;
	}

	axis.Normalize( );
}

_float Quaternion::Dot( const Quaternion& quaternion1, const Quaternion& quaternion2 )
{
	return quaternion1.x * quaternion2.x + quaternion1.y * quaternion2.y + quaternion1.z * quaternion2.z + quaternion1.w * quaternion2.w;
}

Quaternion Quaternion::Slerp( const Quaternion& quaternion1, const Quaternion& quaternion2, _float factor )
{
	factor = Math::Clamp( factor, 0.0f, 1.0f );

	_float dot = Dot( quaternion1, quaternion2 );

	_bool flip = dot < 0.0f;

	if ( dot < 0.0f )
		dot = -dot;

	_float divide = 0.0f;

	if ( 1.0f - dot < Math::cEpsilon )
	{
		divide = 1.0f - factor;
	}
	else
	{
		_float theta	 = Math::Acos( dot );
		_float recipsqrt = 1.0f / Math::Sin( theta );

		divide	= Math::Sin( ( 1.0f - factor ) * theta ) * recipsqrt;
		factor	= Math::Sin( factor * theta ) * recipsqrt;
	}

	if ( flip )
		factor = -factor;

	return Quaternion(
		quaternion1.x * divide + quaternion2.x * factor,
		quaternion1.y * divide + quaternion2.y * factor,
		quaternion1.z * divide + quaternion2.z * factor,
		quaternion1.w * divide + quaternion2.w * factor );
}

_void Quaternion::PrecomputedSlerpParameters( const Quaternion& quaternion1, const Quaternion& quaternion2, _float& dot, _float& theta, _float& recipsqrt )
{
	dot = Dot( quaternion1, quaternion2 );

	theta	  = Math::Acos( dot < 0.0f ? -dot : dot );
	recipsqrt = 1.0f / Math::Sin( theta );
}

Quaternion Quaternion::Slerp( const Quaternion& quaternion1, const Quaternion& quaternion2, _float dot, _float theta, _float recipsqrt, _float factor )
{
	factor = Math::Clamp( factor, 0.0f, 1.0f );

	_bool flip = dot < 0.0f;

	if ( dot < 0.0f )
		dot = -dot;

	_float divide = 0.0f;

	if ( 1.0f - dot < Math::cEpsilon )
	{
		divide = 1.0f - factor;
	}
	else
	{
		divide	= Math::Sin( ( 1.0f - factor ) * theta ) * recipsqrt;
		factor	= Math::Sin( factor * theta ) * recipsqrt;
	}

	if ( flip )
		factor = -factor;

	return Quaternion(
		quaternion1.x * divide + quaternion2.x * factor,
		quaternion1.y * divide + quaternion2.y * factor,
		quaternion1.z * divide + quaternion2.z * factor,
		quaternion1.w * divide + quaternion2.w * factor );
}