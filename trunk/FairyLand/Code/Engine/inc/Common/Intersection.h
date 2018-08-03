//============================================================================
// Intersection.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Intersection
//----------------------------------------------------------------------------

class Intersection
{
public:
	static _bool RayVSPlane( const Ray& ray, const Plane& plane, _float& distance );
	static _bool RayVSTriangle( const Ray& ray, const Triangle& triangle, _float& distance );
	static _bool RayVSBox( const Ray& ray, const Box& box, _float& distance );
	static _bool RayVSAABox( const Ray& ray, const AxisAlignedBox& box, _float& distance );
	static _bool RayVSAABox( const Ray& ray, const AxisAlignedBox& box );

	static _bool RayVSMesh( const Ray& ray, _dword vertexsize, _dword ptype, _dword pnumber, const _byte* vertexbuffer, const _byte* indexbuffer, _float& distance );
	static _bool RayVSMesh( const Ray& ray, _dword vertexsize, _dword ptype, _dword pnumber, const _byte* vertexbuffer, const _dword* indexbuffer, _float& distance );

	static _bool AABoxVSAABox( const AxisAlignedBox& box1, const AxisAlignedBox& box2 );
	static _bool SphereVSSphere( const Sphere& sphere1, const Sphere& sphere2 );

	static _bool AABoxVSPlane( const AxisAlignedBox& box, const Plane& plane );
	static _bool AABoxVSPlane( const AxisAlignedBox& box, const Plane& plane, const Vector3& direction, _float& distance );

	static _bool SphereVSPlane( const Sphere& sphere, const Plane& plane );
	static _bool SphereVSPlane( const Sphere& sphere, const Plane& plane, const Vector3& direction, _float& distance );
};

};