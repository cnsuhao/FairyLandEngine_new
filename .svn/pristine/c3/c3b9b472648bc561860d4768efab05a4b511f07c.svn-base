//============================================================================
// Point.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Point
//----------------------------------------------------------------------------

//! This class represents a 2D point ( or a 2D size ) with integer coordinates.
class Point
{
public:
	//! The zero point.
	const static Point cZeroPoint;
	//! The maximum point.
	const static Point cMaxPoint;
	//! The minimum point.
	const static Point cMinPoint;

	//! X-component of the point.
	_long	x;
	//! Y-component of the point.
	_long	y;

public:
	//! Constructor, create a point without sets x, y components.
	//! @param		none
	inline Point( );
	//! Constructor, create a point and sets x, y components.
	//! @param		xx			X-component of the point.
	//! @param		yy			Y-component of the point.
	inline Point( _long xx, _long yy );

	//! Get positive point.
	//! @param		none
	//! @return		The positive point.
	inline Point operator + ( ) const;
	//! Get negative point.
	//! @param		none
	//! @return		The negative point.
	inline Point operator - ( ) const;

	//! Compare two points, that means compare each component of the points.
	//! @param		point		The point to compare with.
	//! @return		True if they are identical, false otherwise.
	inline _bool operator == ( const Point& point ) const;
	//! Compare two points, that means compare each component of the points.
	//! @param		point		The point to compare with.
	//! @return		True if they are not identical, false otherwise.
	inline _bool operator != ( const Point& point ) const;

	//! Add a point to current point, and save result in current one.
	//! @param		point		The point to add.
	//! @return		The result point of addition.
	inline Point& operator += ( const Point& point );
	//! Subtract a point from current point, and save result in current one.
	//! @param		point		The point to subtract.
	//! @return		The result point of subtraction.
	inline Point& operator -= ( const Point& point );

	inline Point& operator *= ( _long value );
	inline Point& operator /= ( _long value );

	//! Add a point to current point, return a new point to save result.
	//! @param		point		The point to add.
	//! @return		The result point of addition.
	inline Point operator + ( const Point& point ) const;
	//! Subtract a point from current point, return a new point to save result.
	//! @param		point		The point to subtract.
	//! @return		The result point of subtraction.
	inline Point operator - ( const Point& point ) const;

	inline Point operator * ( _long value ) const;
	inline Point operator / ( _long value ) const;

	inline _long Length( ) const;
};

//----------------------------------------------------------------------------
// Point Implementation
//----------------------------------------------------------------------------

Point::Point( )
{
}

Point::Point( _long xx, _long yy ) : x( xx ), y( yy )
{
}

Point Point::operator + ( ) const
{
	return Point( x, y ); 
}

Point Point::operator - ( ) const
{
	return Point( -x, -y );
}

_bool Point::operator == ( const Point& point ) const
{
	return x == point.x && y == point.y;
}

_bool Point::operator != ( const Point& point ) const
{
	return x != point.x || y != point.y;
}

Point& Point::operator += ( const Point& point )
{
	x += point.x;
	y += point.y;

	return *this;
}

Point& Point::operator -= ( const Point& point )
{
	x -= point.x;
	y -= point.y;

	return *this;
}

Point& Point::operator *= ( _long value )
{
	x *= value;
	y *= value;

	return *this;
}

Point& Point::operator /= ( _long value )
{
	x /= value;
	y /= value;

	return *this;
}

Point Point::operator + ( const Point& point ) const
{
	return Point( x + point.x, y + point.y );
}

Point Point::operator - ( const Point& point ) const
{
	return Point( x - point.x, y - point.y );
}

Point Point::operator * ( _long value ) const
{
	return Point( x * value, y * value );
}

Point Point::operator / ( _long value ) const
{
	return Point( x / value, y / value );
}

_long Point::Length( ) const
{
	return Math::Abs( x ) + Math::Abs( y );
}

};