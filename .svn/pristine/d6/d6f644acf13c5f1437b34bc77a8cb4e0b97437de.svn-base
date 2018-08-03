//============================================================================
// Sphere.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Sphere
//----------------------------------------------------------------------------

class Sphere
{
public:
	Vector3	mCenter;
	_float	mRadius;

public:
	inline Sphere( );
	inline Sphere( const Vector3& center, _float radius );

	Sphere& operator *= ( const Matrix4& matrix );
	Sphere operator * ( const Matrix4& matrix ) const;
};

//----------------------------------------------------------------------------
// Sphere Implementation
//----------------------------------------------------------------------------

Sphere::Sphere( )
{
}

Sphere::Sphere( const Vector3& center, _float radius )
{
	mCenter = center;
	mRadius	= radius;
}

};