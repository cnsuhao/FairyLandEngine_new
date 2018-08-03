//============================================================================
// Plane.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Plane Implementation
//----------------------------------------------------------------------------

const Plane Plane::cXYPlane( 0.0f, 0.0f, 1.0f, 0.0f );
const Plane Plane::cYZPlane( 1.0f, 0.0f, 0.0f, 0.0f );
const Plane Plane::cZXPlane( 0.0f, 1.0f, 0.0f, 0.0f );

Plane::Plane( const Vector3& position, const Vector3& normal )
{
	Vector3 n = normal;
	n.Normalize( );

	a = n.x;
	b = n.y;
	c = n.z;
	d = - Vector3::Dot( position, n );
}

Plane::Plane( const Vector3& vector1, const Vector3& vector2, const Vector3& vector3 )
{
	Vector3 edge1	= vector2 - vector1;
	Vector3 edge2	= vector3 - vector1;
	Vector3 n		= Vector3::Cross( edge1, edge2 ).Normalize( );

	a = n.x;
	b = n.y;
	c = n.z;
	d = - Vector3::Dot( vector1, n );
}

Plane& Plane::operator *= ( const Matrix4& matrix )
{
	_float aa = matrix( 0, 0 ) * a + matrix( 0, 1 ) * b + matrix( 0, 2 ) * c;
	_float bb = matrix( 1, 0 ) * a + matrix( 1, 1 ) * b + matrix( 1, 2 ) * c;
	_float cc = matrix( 2, 0 ) * a + matrix( 2, 1 ) * b + matrix( 2, 2 ) * c;

	Vector3 vector = Vector3( a, b, c ) * -d * matrix;

	a = aa;
	b = bb;
	c = cc;
	d = - Vector3::Dot( vector, Vector3( aa, bb, cc ) );

	return *this;
}

Plane Plane::operator * ( const Matrix4& matrix ) const
{
	Plane returnplane( *this );

	return returnplane *= matrix;
}

Vector3 Plane::Reflect( const Vector3& vector ) const
{
	_float length = vector.Magnitude( );

	return ( Vector3( vector ).Normalize( ) + Vector3( a, b, c ) * 2.0f ) * length;
}