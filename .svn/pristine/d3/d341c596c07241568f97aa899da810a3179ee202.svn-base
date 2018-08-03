//============================================================================
// Frustum.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Frustum
//----------------------------------------------------------------------------

class Frustum
{
public:
	Vector3	mVectors[8];
	Plane	mPlanes[6];

public:
	inline Frustum( );
	Frustum( const Matrix4& viewtransform, const Matrix4& projectiontransform );

	_bool	operator == ( const Frustum& frustum ) const;
	_bool	operator != ( const Frustum& frustum ) const;

	inline _float Depth( ) const;

	_bool Inside( const AxisAlignedBox& box ) const;
	_bool Inside( const Box& box ) const;
};

//----------------------------------------------------------------------------
// Frustum Implementation
//----------------------------------------------------------------------------

Frustum::Frustum( )
{
}

_float Frustum::Depth( ) const
{
	return Math::Abs( mPlanes[1].Distance( mVectors[0] ) );
}

};