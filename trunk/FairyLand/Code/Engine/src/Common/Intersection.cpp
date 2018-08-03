//============================================================================
// Intersection.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Intersection Implementation
//----------------------------------------------------------------------------

_bool Intersection::RayVSPlane( const Ray& ray, const Plane& plane, _float& distance )
{
	_float dotvalue = Vector3::Dot( plane.Normal( ), ray.mDirection );

	if ( Math::Abs( dotvalue ) < Math::cEpsilon )
		return _false;

	_float tempdistance = plane.Distance( ray.mOrigin ) / -dotvalue;

	if ( tempdistance < 0.0f )
		return _false;

	distance = tempdistance;

    return _true;
}

_bool Intersection::RayVSTriangle( const Ray& ray, const Triangle& triangle, _float& distance )
{
	Vector3 edge1 = triangle.mVector2 - triangle.mVector1;
	Vector3 edge2 = triangle.mVector3 - triangle.mVector1;

	Vector3 tvec, pvec = Vector3::Cross( ray.mDirection, edge2 );

	_float det = Vector3::Dot( edge1, pvec );

    if ( det >= 0.0f )
    {
        tvec = ray.mOrigin - triangle.mVector1;
    }
    else
    {
        tvec = triangle.mVector1 - ray.mOrigin;
        det = -det;
    }

	if ( det < Math::cEpsilon )
        return _false;

	_float u = Vector3::Dot( tvec, pvec );
    if( u < 0.0f || u > det )
        return _false;

	Vector3 qvec = Vector3::Cross( tvec, edge1 );

	_float v = Vector3::Dot( ray.mDirection, qvec );
    if( v < 0.0f || u + v > det )
        return _false;

	distance = Vector3::Dot( edge2, qvec ) / det;

	return _true;
}

_bool Intersection::RayVSBox( const Ray& ray, const Box& box, _float& distance )
{
	if ( ray.mOrigin.x >= box.mVectors[0].x && ray.mOrigin.x <= box.mVectors[7].x &&
		 ray.mOrigin.y >= box.mVectors[0].y && ray.mOrigin.y <= box.mVectors[7].y &&
		 ray.mOrigin.z >= box.mVectors[0].z && ray.mOrigin.z <= box.mVectors[7].z )
	{
		distance = 0.0f;

		return _true;
	}

	_float tempdistance1 = 0.0f, tempdistance2 = -1.0f;

	if ( RayVSTriangle( ray, Triangle( box.mVectors[0], box.mVectors[1], box.mVectors[2] ), tempdistance1 ) == _true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
		tempdistance2 = tempdistance1;
	if ( RayVSTriangle( ray, Triangle( box.mVectors[2], box.mVectors[1], box.mVectors[3] ), tempdistance1 ) == _true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
		tempdistance2 = tempdistance1;

	if ( RayVSTriangle( ray, Triangle( box.mVectors[4], box.mVectors[6], box.mVectors[5] ), tempdistance1 ) == _true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
		tempdistance2 = tempdistance1;
	if ( RayVSTriangle( ray, Triangle( box.mVectors[5], box.mVectors[6], box.mVectors[7] ), tempdistance1 ) == _true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
		tempdistance2 = tempdistance1;

	if ( RayVSTriangle( ray, Triangle( box.mVectors[4], box.mVectors[5], box.mVectors[0] ), tempdistance1 ) == _true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
		tempdistance2 = tempdistance1;
	if ( RayVSTriangle( ray, Triangle( box.mVectors[0], box.mVectors[5], box.mVectors[1] ), tempdistance1 ) == _true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
		tempdistance2 = tempdistance1;

	if ( RayVSTriangle( ray, Triangle( box.mVectors[1], box.mVectors[5], box.mVectors[4] ), tempdistance1 ) == _true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
		tempdistance2 = tempdistance1;
	if ( RayVSTriangle( ray, Triangle( box.mVectors[4], box.mVectors[5], box.mVectors[7] ), tempdistance1 ) == _true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
		tempdistance2 = tempdistance1;

	if ( RayVSTriangle( ray, Triangle( box.mVectors[2], box.mVectors[3], box.mVectors[6] ), tempdistance1 ) == _true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
		tempdistance2 = tempdistance1;
	if ( RayVSTriangle( ray, Triangle( box.mVectors[6], box.mVectors[3], box.mVectors[7] ), tempdistance1 ) == _true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
		tempdistance2 = tempdistance1;

	if ( RayVSTriangle( ray, Triangle( box.mVectors[4], box.mVectors[0], box.mVectors[6] ), tempdistance1 ) == _true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
		tempdistance2 = tempdistance1;
	if ( RayVSTriangle( ray, Triangle( box.mVectors[6], box.mVectors[0], box.mVectors[2] ), tempdistance1 ) == _true && ( tempdistance2 < 0.0f || tempdistance2 > tempdistance1 ) )
		tempdistance2 = tempdistance1;

	if ( tempdistance2 < 0.0f )
		return _false;

	distance = tempdistance2;

	return _true;
}

_bool Intersection::RayVSAABox( const Ray& ray, const AxisAlignedBox& box, _float& distance )
{
	return RayVSBox( ray, Box( box.mMinVector, box.mMaxVector ), distance );
}

_bool Intersection::RayVSAABox( const Ray& ray, const AxisAlignedBox& box )
{
	if ( ray.i < 0 )
	{
		if ( ray.j < 0 )
		{
			if ( ray.k < 0 )
			{
				if ( ( ray.x < box.x1 ) || ( ray.y < box.y1 ) || ( ray.z < box.z1 ) )
					return _false;

				_float xa = box.x1 - ray.x;
				_float ya = box.y1 - ray.y;
				_float za = box.z1 - ray.z;
				_float xb = box.x2 - ray.x;
				_float yb = box.y2 - ray.y;
				_float zb = box.z2 - ray.z;

				if ( ( ray.i * ya - ray.j * xb < 0 ) ||
					 ( ray.i * yb - ray.j * xa > 0 ) ||
					 ( ray.i * zb - ray.k * xa > 0 ) ||
					 ( ray.i * za - ray.k * xb < 0 ) ||
					 ( ray.j * za - ray.k * yb < 0 ) ||
					 ( ray.j * zb - ray.k * ya > 0 ) )
					return _false;

				return _true;
			}
			else
			{
				if ( ( ray.x < box.x1 ) || ( ray.y < box.y1 ) || ( ray.z > box.z2 ) )
					return _false;

				_float xa = box.x1 - ray.x;
				_float ya = box.y1 - ray.y;
				_float za = box.z1 - ray.z;
				_float xb = box.x2 - ray.x;
				_float yb = box.y2 - ray.y;
				_float zb = box.z2 - ray.z;

				if ( ( ray.i * ya - ray.j * xb < 0 ) ||
					 ( ray.i * yb - ray.j * xa > 0 ) ||
					 ( ray.i * zb - ray.k * xb > 0 ) ||
					 ( ray.i * za - ray.k * xa < 0 ) ||
					 ( ray.j * za - ray.k * ya < 0 ) ||
					 ( ray.j * zb - ray.k * yb > 0 ) )
					return _false;

				return _true;
			}
		}
		else 
		{
			if ( ray.k < 0 )
			{
				if ( ( ray.x < box.x1 ) || ( ray.y > box.y2 ) || ( ray.z < box.z1 ) )
					return _false;

				_float xa = box.x1 - ray.x;
				_float ya = box.y1 - ray.y;
				_float za = box.z1 - ray.z;
				_float xb = box.x2 - ray.x;
				_float yb = box.y2 - ray.y;
				_float zb = box.z2 - ray.z;

				if ( ( ray.i * ya - ray.j * xa < 0 ) ||
					 ( ray.i * yb - ray.j * xb > 0 ) ||
					 ( ray.i * zb - ray.k * xa > 0 ) ||
					 ( ray.i * za - ray.k * xb < 0 ) ||
					 ( ray.j * zb - ray.k * yb < 0 ) ||
					 ( ray.j * za - ray.k * ya > 0 ) )
					return _false;

				return _true;
			}
			else
			{
				if ( ( ray.x < box.x1 ) || ( ray.y > box.y2 ) || ( ray.z > box.z2 ) )
					return _false;

				_float xa = box.x1 - ray.x;
				_float ya = box.y1 - ray.y;
				_float za = box.z1 - ray.z;
				_float xb = box.x2 - ray.x;
				_float yb = box.y2 - ray.y;
				_float zb = box.z2 - ray.z;

				if ( ( ray.i * ya - ray.j * xa < 0 ) ||
					 ( ray.i * yb - ray.j * xb > 0 ) ||
					 ( ray.i * zb - ray.k * xb > 0 ) ||
					 ( ray.i * za - ray.k * xa < 0 ) ||
					 ( ray.j * zb - ray.k * ya < 0 ) ||
					 ( ray.j * za - ray.k * yb > 0 ) )
					return _false;

				return _true;
			}
		}
	}
	else
	{
		if ( ray.j < 0 )
		{
			if ( ray.k < 0 )
			{
				if ( ( ray.x > box.x2 ) || ( ray.y < box.y1 ) || ( ray.z < box.z1 ) )
					return _false;

				_float xa = box.x1 - ray.x;
				_float ya = box.y1 - ray.y;
				_float za = box.z1 - ray.z;
				_float xb = box.x2 - ray.x;
				_float yb = box.y2 - ray.y;
				_float zb = box.z2 - ray.z;

				if ( ( ray.i * yb - ray.j * xb < 0 ) ||
					 ( ray.i * ya - ray.j * xa > 0 ) ||
					 ( ray.i * za - ray.k * xa > 0 ) ||
					 ( ray.i * zb - ray.k * xb < 0 ) ||
					 ( ray.j * za - ray.k * yb < 0 ) ||
					 ( ray.j * zb - ray.k * ya > 0 ) )
					return _false;

				return _true;
			}
			else
			{
				if ( ( ray.x > box.x2 ) || ( ray.y < box.y1 ) || ( ray.z > box.z2 ) )
					return _false;

				_float xa = box.x1 - ray.x;
				_float ya = box.y1 - ray.y;
				_float za = box.z1 - ray.z;
				_float xb = box.x2 - ray.x;
				_float yb = box.y2 - ray.y;
				_float zb = box.z2 - ray.z;

				if ( ( ray.i * yb - ray.j * xb < 0 ) ||
					 ( ray.i * ya - ray.j * xa > 0 ) ||
					 ( ray.i * za - ray.k * xb > 0 ) ||
					 ( ray.i * zb - ray.k * xa < 0 ) ||
					 ( ray.j * za - ray.k * ya < 0 ) ||
					 ( ray.j * zb - ray.k * yb > 0 ) )
					return _false;

				return _true;
			}
		}
		else
		{
			if ( ray.k < 0 )
			{
				if ( ( ray.x > box.x2 ) || ( ray.y > box.y2 ) || ( ray.z < box.z1 ) )
					return _false;

				_float xa = box.x1 - ray.x;
				_float ya = box.y1 - ray.y;
				_float za = box.z1 - ray.z;
				_float xb = box.x2 - ray.x;
				_float yb = box.y2 - ray.y;
				_float zb = box.z2 - ray.z;

				if ( ( ray.i * yb - ray.j * xa < 0 ) ||
					 ( ray.i * ya - ray.j * xb > 0 ) ||
					 ( ray.i * za - ray.k * xa > 0 ) ||
					 ( ray.i * zb - ray.k * xb < 0 ) ||
					 ( ray.j * zb - ray.k * yb < 0 ) ||
					 ( ray.j * za - ray.k * ya > 0 ) )
					return _false;

				return _true;
			}
			else
			{
				if ( ( ray.x > box.x2 ) || ( ray.y > box.y2 ) || ( ray.z > box.z2 ) )
					return _false;

				_float xa = box.x1 - ray.x;
				_float ya = box.y1 - ray.y;
				_float za = box.z1 - ray.z;
				_float xb = box.x2 - ray.x;
				_float yb = box.y2 - ray.y;
				_float zb = box.z2 - ray.z;

				if ( ( ray.i * yb - ray.j * xa < 0 ) ||
					 ( ray.i * ya - ray.j * xb > 0 ) ||
					 ( ray.i * za - ray.k * xb > 0 ) ||
					 ( ray.i * zb - ray.k * xa < 0 ) ||
					 ( ray.j * zb - ray.k * ya < 0 ) ||
					 ( ray.j * za - ray.k * yb > 0 ) )
					return _false;

				return _true;
			}
		}
	}

	return _false;
}

_bool Intersection::RayVSMesh( const Ray& ray, _dword vertexsize, _dword ptype, _dword pnumber, const _byte* vertexbuffer, const _byte* indexbuffer, _float& distance )
{
	if ( vertexbuffer == _null )
		return _false;

	if ( ptype != Geometry::_GEOMETRY_TRIANGLELIST && ptype != Geometry::_GEOMETRY_TRIANGLESTRIP && ptype != Geometry::_GEOMETRY_TRIANGLEFAN )
		return _false;

	_float nearest = -1.0f, tempdistance = 0.0f;

	// No index.
	if ( indexbuffer == _null )
	{
		for ( _dword i = 0; i < pnumber; i ++ )
		{
			_dword a = 0, b = 0, c = 0;

			switch ( ptype )
			{
				case Geometry::_GEOMETRY_TRIANGLELIST:
				{
					 a = i * 3 + 0;
					 b = i * 3 + 1;
					 c = i * 3 + 2;

					 break;
				}

				case Geometry::_GEOMETRY_TRIANGLESTRIP:
				{
					a = i + 0;
					b = i + 1 + ( i % 2 );
					c = i + 2 - ( i % 2 );

					break;
				}

				case Geometry::_GEOMETRY_TRIANGLEFAN:
				{
					a = 0;
					b = i + 1;
					c = i + 2;

					break;
				}
			}

			Vector3 vector1 = *( (Vector3*) ( (_byte*) vertexbuffer + a * vertexsize ) ); 
			Vector3 vector2 = *( (Vector3*) ( (_byte*) vertexbuffer + b * vertexsize ) );
			Vector3 vector3 = *( (Vector3*) ( (_byte*) vertexbuffer + c * vertexsize ) );

			if ( Intersection::RayVSTriangle( ray, Triangle( vector1, vector2, vector3 ), tempdistance ) == _true )
			{
				if ( nearest < 0.0f || nearest > tempdistance )
					nearest = tempdistance;
			}
		}
	}
	else
	{
		for ( _dword i = 0; i < pnumber; i ++ )
		{
			_dword a = 0, b = 0, c = 0;

			switch ( ptype )
			{
				case Geometry::_GEOMETRY_TRIANGLELIST:
				{
					a = ( (_word*) indexbuffer )[ i * 3 + 0 ];
					b = ( (_word*) indexbuffer )[ i * 3 + 1 ];
					c = ( (_word*) indexbuffer )[ i * 3 + 2 ];

					break;
				}

				case Geometry::_GEOMETRY_TRIANGLESTRIP:
				{
					a = ( (_word*) indexbuffer )[ i + 0 ];
					b = ( (_word*) indexbuffer )[ i + 1 + ( i % 2 ) ];
					c = ( (_word*) indexbuffer )[ i + 2 - ( i % 2 ) ];

					break;
				}

				case Geometry::_GEOMETRY_TRIANGLEFAN:
				{
					a = ( (_word*) indexbuffer )[ 0 ];
					b = ( (_word*) indexbuffer )[ i + 1 ];
					c = ( (_word*) indexbuffer )[ i + 2 ];

					break;
				}
			}

			Vector3 vector1 = *( (Vector3*) ( (_byte*) vertexbuffer + a * vertexsize ) ); 
			Vector3 vector2 = *( (Vector3*) ( (_byte*) vertexbuffer + b * vertexsize ) );
			Vector3 vector3 = *( (Vector3*) ( (_byte*) vertexbuffer + c * vertexsize ) );

			if ( Intersection::RayVSTriangle( ray, Triangle( vector1, vector2, vector3 ), tempdistance ) == _true )
			{
				if ( nearest < 0.0f || nearest > tempdistance )
					nearest = tempdistance;
			}
		}
	}

	if ( nearest >= 0.0f )
		distance = nearest;

	return nearest >= 0.0f;
}

_bool Intersection::RayVSMesh( const Ray& ray, _dword vertexsize, _dword ptype, _dword pnumber, const _byte* vertexbuffer, const _dword* indexbuffer, _float& distance )
{
	if ( vertexbuffer == _null )
		return _false;

	if ( indexbuffer == _null )
		return	_false;

	if ( ptype != Geometry::_GEOMETRY_TRIANGLELIST && ptype != Geometry::_GEOMETRY_TRIANGLESTRIP && ptype != Geometry::_GEOMETRY_TRIANGLEFAN )
		return _false;

	_float nearest = -1.0f, tempdistance = 0.0f;
	
	for ( _dword i = 0; i < pnumber; i ++ )
	{
		_dword a = 0, b = 0, c = 0;

		switch ( ptype )
		{
		case Geometry::_GEOMETRY_TRIANGLELIST:
			{
				a = indexbuffer[ i * 3 + 0 ];
				b = indexbuffer[ i * 3 + 1 ];
				c = indexbuffer[ i * 3 + 2 ];

				break;
			}

		case Geometry::_GEOMETRY_TRIANGLESTRIP:
			{
				a = indexbuffer[ i + 0 ];
				b = indexbuffer[ i + 1 + ( i % 2 ) ];
				c = indexbuffer[ i + 2 - ( i % 2 ) ];

				break;
			}

		case Geometry::_GEOMETRY_TRIANGLEFAN:
			{
				a = indexbuffer[ 0 ];
				b = indexbuffer[ i + 1 ];
				c = indexbuffer[ i + 2 ];

				break;
			}
		}

		Vector3 vector1 = *( (Vector3*) ( (_byte*) vertexbuffer + a * vertexsize ) ); 
		Vector3 vector2 = *( (Vector3*) ( (_byte*) vertexbuffer + b * vertexsize ) );
		Vector3 vector3 = *( (Vector3*) ( (_byte*) vertexbuffer + c * vertexsize ) );

		if ( Intersection::RayVSTriangle( ray, Triangle( vector1, vector2, vector3 ), tempdistance ) == _true )
		{
			if ( nearest < 0.0f || nearest > tempdistance )
				nearest = tempdistance;
		}
	}

	if ( nearest >= 0.0f )
		distance = nearest;

	return nearest >= 0.0f;
}

_bool Intersection::AABoxVSAABox( const AxisAlignedBox& box1, const AxisAlignedBox& box2 )
{
	_float x1 = Math::Max( box1.mMinVector.x, box2.mMinVector.x );
	if ( x1 >= box1.mMaxVector.x || x1 >= box2.mMaxVector.x )
		return _false;

	_float x2 = Math::Min( box1.mMaxVector.x, box2.mMaxVector.x );
	if ( x2 <= box1.mMinVector.x || x2 <= box2.mMinVector.x )
		return _false;

	_float y1 = Math::Max( box1.mMinVector.y, box2.mMinVector.y );
	if ( y1 >= box1.mMaxVector.y || y1 >= box2.mMaxVector.y )
		return _false;

	_float y2 = Math::Min( box1.mMaxVector.y, box2.mMaxVector.y );
	if ( y2 <= box1.mMinVector.y || y2 <= box2.mMinVector.y )
		return _false;

	_float z1 = Math::Max( box1.mMinVector.z, box2.mMinVector.z );
	if ( z1 >= box1.mMaxVector.z || z1 >= box2.mMaxVector.z )
		return _false;

	_float z2 = Math::Min( box1.mMaxVector.z, box2.mMaxVector.z );
	if ( z2 <= box1.mMinVector.z || z2 <= box2.mMinVector.z )
		return _false;

	return _true;
}

_bool Intersection::SphereVSSphere( const Sphere& sphere1, const Sphere& sphere2 )
{
	return Math::Abs( sphere1.mRadius ) + Math::Abs( sphere2.mRadius ) >
		( sphere1.mCenter - sphere2.mCenter ).Magnitude( );
}

_bool Intersection::AABoxVSPlane( const AxisAlignedBox& box, const Plane& plane )
{
	_float d1 = plane.Distance( Vector3( box.mMinVector.x, box.mMinVector.y, box.mMinVector.z ) );
	_float d2 = plane.Distance( Vector3( box.mMaxVector.x, box.mMaxVector.y, box.mMaxVector.z ) );

	_float t = d1 * d2;
	if ( t <= Math::cEpsilon )
		return _true;

	t *= plane.Distance( Vector3( box.mMaxVector.x, box.mMinVector.y, box.mMinVector.z ) );
	if ( t <= Math::cEpsilon )
		return _true;

	t *= plane.Distance( Vector3( box.mMinVector.x, box.mMaxVector.y, box.mMinVector.z ) );
	if ( t <= Math::cEpsilon )
		return _true;

	t *= plane.Distance( Vector3( box.mMaxVector.x, box.mMaxVector.y, box.mMinVector.z ) );
	if ( t <= Math::cEpsilon )
		return _true;

	t *= plane.Distance( Vector3( box.mMinVector.x, box.mMinVector.y, box.mMaxVector.z ) );
	if ( t <= Math::cEpsilon )
		return _true;

	t *= plane.Distance( Vector3( box.mMaxVector.x, box.mMinVector.y, box.mMaxVector.z ) );
	if ( t <= Math::cEpsilon )
		return _true;

	t *= plane.Distance( Vector3( box.mMinVector.x, box.mMaxVector.y, box.mMaxVector.z ) );
	if ( t <= Math::cEpsilon )
		return _true;

	return _false;
}

_bool Intersection::AABoxVSPlane( const AxisAlignedBox& box, const Plane& plane, const Vector3& direction, _float& distance )
{
	distance = 0.0f;

	_float t, d[8]; Vector3 vectors[8];
	d[0] = plane.Distance( vectors[0] = Vector3( box.mMinVector.x, box.mMinVector.y, box.mMinVector.z ) );
	d[1] = plane.Distance( vectors[1] = Vector3( box.mMaxVector.x, box.mMaxVector.y, box.mMaxVector.z ) );

	if ( ( t = d[0] * d[1] ) <= Math::cEpsilon )
		return _true;

	d[2] = plane.Distance( vectors[2] = Vector3( box.mMaxVector.x, box.mMinVector.y, box.mMinVector.z ) );
	if ( ( t *= d[2] ) <= Math::cEpsilon )
		return _true;

	d[3] = plane.Distance( vectors[3] = Vector3( box.mMinVector.x, box.mMaxVector.y, box.mMinVector.z ) );
	if ( ( t *= d[3] ) <= Math::cEpsilon )
		return _true;

	d[4] = plane.Distance( vectors[4] = Vector3( box.mMaxVector.x, box.mMaxVector.y, box.mMinVector.z ) );
	if ( ( t *= d[4] ) <= Math::cEpsilon )
		return _true;

	d[5] = plane.Distance( vectors[5] = Vector3( box.mMinVector.x, box.mMinVector.y, box.mMaxVector.z ) );
	if ( ( t *= d[5] ) <= Math::cEpsilon )
		return _true;

	d[6] = plane.Distance( vectors[6] = Vector3( box.mMaxVector.x, box.mMinVector.y, box.mMaxVector.z ) );
	if ( ( t *= d[6] ) <= Math::cEpsilon )
		return _true;

	d[7] = plane.Distance( vectors[7] = Vector3( box.mMinVector.x, box.mMaxVector.y, box.mMaxVector.z ) );
	if ( ( t *= d[7] ) <= Math::cEpsilon )
		return _true;

	// Not intersecte, find the nearest point.
	_dword n = 0; t = Math::Abs( d[0] );
	for ( _dword i = 1; i < 8; i ++ )
	{
		if ( Math::Abs( d[i] ) < t )
		{
			t = Math::Abs( d[i] );
			n = i;
		}
	}

	// Find the distance from the point to the plane.
	return RayVSPlane( Ray( vectors[n], direction ), plane, distance );
}

_bool Intersection::SphereVSPlane( const Sphere& sphere, const Plane& plane )
{
	return Math::Abs( plane.Distance( sphere.mCenter ) ) <= Math::Abs( sphere.mRadius );
}

_bool Intersection::SphereVSPlane( const Sphere& sphere, const Plane& plane, const Vector3& direction, _float& distance )
{
	distance = 0.0f;

	if ( Math::Abs( plane.Distance( sphere.mCenter ) ) <= Math::Abs( sphere.mRadius ) )
		return _true;

	return RayVSPlane( Ray( sphere.mCenter, direction ), Plane( plane.a, plane.b, plane.c, plane.d - Math::Abs( sphere.mRadius ) ), distance );
}