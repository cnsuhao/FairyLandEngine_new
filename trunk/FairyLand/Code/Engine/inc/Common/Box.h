//============================================================================
// Box.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Box
//----------------------------------------------------------------------------

class Box
{
public:
	Vector3	mVectors[8];

public:
	inline Box( );
	inline Box( const Vector3& minvector, const Vector3& maxvector );

	inline Box& operator *= ( const Matrix4& matrix );
	inline Box operator * ( const Matrix4& matrix ) const;
};

//----------------------------------------------------------------------------
// Box Implementation
//----------------------------------------------------------------------------

Box::Box( )
{
}

Box::Box( const Vector3& minvector, const Vector3& maxvector )
{
	mVectors[0] = Vector3( minvector.x, minvector.y, minvector.z );
	mVectors[1] = Vector3( maxvector.x, minvector.y, minvector.z );
	mVectors[2] = Vector3( minvector.x, maxvector.y, minvector.z );
	mVectors[3] = Vector3( maxvector.x, maxvector.y, minvector.z );
	mVectors[4] = Vector3( minvector.x, minvector.y, maxvector.z );
	mVectors[5] = Vector3( maxvector.x, minvector.y, maxvector.z );
	mVectors[6] = Vector3( minvector.x, maxvector.y, maxvector.z );
	mVectors[7] = Vector3( maxvector.x, maxvector.y, maxvector.z );
}

Box& Box::operator *= ( const Matrix4& matrix )
{
	mVectors[0] *= matrix;
	mVectors[1] *= matrix;
	mVectors[2] *= matrix;
	mVectors[3] *= matrix;
	mVectors[4] *= matrix;
	mVectors[5] *= matrix;
	mVectors[6] *= matrix;
	mVectors[7] *= matrix;

	return *this;
}

Box Box::operator * ( const Matrix4& matrix ) const
{
	Box returnbox( *this );

	return returnbox *= matrix;
}

};