//============================================================================
// Ray.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Ray
//----------------------------------------------------------------------------

class Ray
{
public:
	union
	{
		struct
		{
			_float	x, y, z;
			_float	i, j, k;
		};

		struct
		{
			Vector3	mOrigin;
			Vector3	mDirection;
		};
	};

public:
	inline Ray( );
	inline Ray( _float x, _float y, _float z, _float i, _float j, _float k );
	inline Ray( const Vector3& origin, const Vector3& direction );
	Ray( _float x, _float y, const Matrix4& viewtransform, const Matrix4& projectiontransform );

	inline _bool operator == ( const Ray& ray ) const;
	inline _bool operator != ( const Ray& ray ) const;

	inline _bool Equal( const Ray& ray, _float margin = Math::cEpsilon ) const;

	inline Ray& operator *= ( const Matrix4& matrix );
	inline Ray  operator * ( const Matrix4& matrix ) const;

	inline Vector3 DistanceVector( _float distance ) const;
};

//----------------------------------------------------------------------------
// Ray Implementation
//----------------------------------------------------------------------------

Ray::Ray( )
{
}

Ray::Ray( _float xx, _float yy, _float zz, _float ii, _float jj, _float kk )
	: x( xx ), y( yy ), z( zz ), i( ii ), j( jj ), k( kk )
{
}

Ray::Ray( const Vector3& origin, const Vector3& direction )
	: mOrigin( origin ), mDirection( direction )
{
}

_bool Ray::operator == ( const Ray& ray ) const
{
	return mOrigin == ray.mOrigin && mDirection == ray.mDirection;
}

_bool Ray::operator != ( const Ray& ray ) const
{
	return mOrigin != ray.mOrigin || mDirection != ray.mDirection;
}

_bool Ray::Equal( const Ray& ray, _float margin ) const
{
	return mOrigin.Equal( ray.mOrigin ) && mDirection.Equal( ray.mDirection );
}

Ray& Ray::operator *= ( const Matrix4& matrix )
{
	mOrigin *= matrix;
	mDirection = Vector4( mDirection, 0.0f ) * matrix;

	return *this;
}

Ray Ray::operator * ( const Matrix4& matrix ) const
{
	return Ray( mOrigin * matrix, Vector4( mDirection, 0.0f ) * matrix );
}

Vector3 Ray::DistanceVector( _float distance ) const
{
	return mOrigin + mDirection * distance;
}

};