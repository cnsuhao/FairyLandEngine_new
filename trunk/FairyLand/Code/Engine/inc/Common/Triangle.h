//============================================================================
// Triangle.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Triangle
//----------------------------------------------------------------------------

class Triangle
{
public:
	Vector3	mVector1;
	Vector3	mVector2;
	Vector3	mVector3;

public:
	inline Triangle( );
	inline Triangle( const Vector3& vector1, const Vector3& vector2, const Vector3& vector3 );

	Triangle& operator *= ( const Matrix4& matrix );
	Triangle  operator * ( const Matrix4& matrix ) const;

	inline Vector3 MiddleVector( );
};

//----------------------------------------------------------------------------
// Triangle Implementation
//----------------------------------------------------------------------------

Triangle::Triangle( )
{
}

Triangle::Triangle( const Vector3& vector1, const Vector3& vector2, const Vector3& vector3 )
	: mVector1( vector1 ), mVector2( vector2 ), mVector3( vector3 )
{
}

Vector3 Triangle::MiddleVector( )
{
	return Vector3( ( mVector1.x + mVector2.x + mVector3.x ) / 3.0f,
		( mVector1.y + mVector2.y + mVector3.y ) / 3.0f, ( mVector1.z + mVector2.z + mVector3.z ) / 3.0f );
}

};