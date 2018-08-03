//============================================================================
// Plane.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Plane
//----------------------------------------------------------------------------

class Plane
{
public:
	const static Plane cXYPlane;
	const static Plane cYZPlane;
	const static Plane cZXPlane;

	_float	a;
	_float	b;
	_float	c;
	_float	d;

public:
	inline Plane( );
	inline Plane( _float aa, _float bb, _float cc, _float dd );
	Plane( const Vector3& position, const Vector3& normal );
	Plane( const Vector3& vector1, const Vector3& vector2, const Vector3& vector3 );

	inline Plane operator + ( ) const;
	inline Plane operator - ( ) const;

	inline	_bool	operator == ( const Plane& plane ) const;
	inline	_bool	operator != ( const Plane& plane ) const;

	inline Vector3 Normal( ) const;
	inline _float Distance( const Vector3& vector ) const;

	Plane& operator *= ( const Matrix4& matrix );
	Plane operator * ( const Matrix4& matrix ) const;

	Vector3 Reflect( const Vector3& vector ) const;
};

//----------------------------------------------------------------------------
// Plane Implementation
//----------------------------------------------------------------------------

Plane::Plane( )
{
}

Plane::Plane( _float aa, _float bb, _float cc, _float dd )
	: a( aa ), b( bb ), c( cc ), d( dd )
{
}

Plane Plane::operator + ( ) const
{
	return Plane( a, b, c, d );
}

Plane Plane::operator - ( ) const
{
	return Plane( -a, -b, -c, -d );
}

_bool	Plane::operator == ( const Plane& plane ) const
{
	return	a == plane.a && b == plane.b && c == plane.c && d == plane.d;
}

_bool	Plane::operator != ( const Plane& plane ) const
{
	return	a != plane.a || b != plane.b || c != plane.c || d != plane.d;
}

Vector3 Plane::Normal( ) const
{
	return Vector3( a, b, c );
}

_float Plane::Distance( const Vector3& vector ) const
{
	return a * vector.x + b * vector.y + c * vector.z + d;
}

};