//============================================================================
// Rect.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Rect
//----------------------------------------------------------------------------

//! This class represents a 2D axis-aligned rectangle with integer coordinates.
class Rect
{
public:
	//! The empty rectangle.
	const static Rect cNullRect;

	//! The left-component of the rectangle.
	_long	l;
	//! The top-component of the rectangle.
	_long	t;
	//! The right-component of the rectangle.
	_long	r;
	//! The bottom-component of the rectangle.
	_long	b;

public:
	//! Constructor, create a rectangle without sets components.
	//! @param		none
	inline Rect( );
	//! Constructor, create a rectangle and sets components.
	//! @param		ll			Left-component of the rectangle.
	//! @param		tt			Top-component of the rectangle.
	//! @param		rr			Right-component of the rectangle.
	//! @param		bb			Bottom-component of the rectangle.
	inline Rect( _long ll, _long tt, _long rr, _long bb );

	//! Get positive rectangle.
	//! @param		none
	//! @return		The positive rectangle.
	inline Rect operator + ( ) const;
	//! Get negative rectangle.
	//! @param		none
	//! @return		The negative rectangle.
	inline Rect operator - ( ) const;

	//! Compare two rectangle, that means compare each component of the rectangles.
	//! @param		rect		The rectangle to compare with.
	//! @return		True if they are identical, false otherwise.
	inline _bool operator == ( const Rect& rect ) const;
	//! Compare two rectangle, that means compare each component of the rectangles.
	//! @param		rect		The rectangle to compare with.
	//! @return		True if they are not identical, false otherwise.
	inline _bool operator != ( const Rect& rect ) const;

	inline Rect& operator += ( const Rect& rect );
	inline Rect& operator -= ( const Rect& rect );

	inline Rect operator + ( const Rect& rect ) const;
	inline Rect operator - ( const Rect& rect ) const;

	inline Rect& operator *= ( int scale );
	inline Rect& operator /= ( int scale );

	inline Rect operator * ( int scale ) const;
	inline Rect operator / ( int scale ) const;

	//! Add a point to current rectangle ( offset ), and save result in current one.
	//! @param		point		The point to add.
	//! @return		The result rectangle of addition.
	inline Rect& operator += ( const Point& point );
	//! Subtract a point from current rectangle ( offset ), and save result in current one.
	//! @param		point		The point to subtract.
	//! @return		The result rectangle of subtraction.
	inline Rect& operator -= ( const Point& point );

	//! Add a point to current rectangle ( offset ), return a new rectangle to save result.
	//! @param		point		The point to add.
	//! @return		The result rectangle of addition.
	inline Rect operator + ( const Point& point ) const;
	//! Subtract a point from current rectangle ( offset ), return a new rectangle to save result.
	//! @param		point		The point to subtract.
	//! @return		The result rectangle of subtraction.
	inline Rect operator - ( const Point& point ) const;

	//! Calculates the width of the rectangle.
	//! @param		none
	//! @return		The width of the rectangle.
	inline _long Width( ) const;
	//! Calculates the height of the rectangle.
	//! @param		none
	//! @return		The height of the rectangle.
	inline _long Height( ) const;

	//! Get the center point of the rectangle.
	//! @param		none
	//! @return		The center point of the rectangle.
	inline Point Center( ) const;

	//! Get the left-top point of the rectangle.
	//! @param		none
	//! @return		The left-top point of the rectangle.
	inline Point LeftTop( ) const;
	//! Get the left-bottom point of the rectangle.
	//! @param		none
	//! @return		The left-bottom point of the rectangle.
	inline Point LeftBottom( ) const;
	//! Get the right-top point of the rectangle.
	//! @param		none
	//! @return		The right-top point of the rectangle.
	inline Point RightTop( ) const;
	//! Get the right-top bottom of the rectangle.
	//! @param		none
	//! @return		The right-top bottom of the rectangle.
	inline Point RightBottom( ) const;

	//! Increases the width and height of the rectangle by moving its sides away from its center.
	//! @param		width		The number of units to inflate the left and right sides.
	//! @param		height		The number of units to inflate the top and bottom sides.
	//! @remarks	The inflate will happen at both sides, so the width and height of the result rectangle will increase two times arguments.
	inline Rect& Inflate( _long width, _long height );
	//! Increases the width and height of the rectangle by moving its sides away from its center.
	//! @param		size		The number of units to inflate both left-right sides and top-bottom sides.
	//! @remarks	The inflate will happen at both sides, so the width and height of the result rectangle will increase two times arguments.
	inline Rect& Inflate( const Point& size );
	//! Decreases the width and height of the rectangle by moving its sides toward its center.
	//! @param		width		The number of units to deflate the left and right sides.
	//! @param		height		The number of units to deflate the top and bottom sides.
	//! @remarks	The deflate will happen at both sides, so the width and height of the result rectangle will decrease two times arguments.
	inline Rect& Deflate( _long width, _long height );
	//! Decreases the width and height of the rectangle by moving its sides toward its center.
	//! @param		size		The number of units to deflate both left-right sides and top-bottom sides.
	//! @remarks	The deflate will happen at both sides, so the width and height of the result rectangle will decrease two times arguments.
	inline Rect& Deflate( const Point& size );

	//! Determines whether the specified point lies within the rectangle.
	//! @param		point		The point to determine.
	//! @return		True if the point lies within the rectangle, false otherwise.
	inline _bool PointInRect( const Point& point ) const;

	//! Makes a rectangle equal to the intersection of two existing ones.
	//! @param		rect1		The source rectangle 1.
	//! @param		rect2		The source rectangle 2.
	//! @return		The intersection rectangle that contains both source rectangles.
	static Rect Intersect( const Rect& rect1, const Rect& rect2 );
	//! Makes a rectangle equal to the union of two existing ones.
	//! @param		rect1		The source rectangle 1.
	//! @param		rect2		The source rectangle 2.
	//! @return		The union rectangle that contains both source rectangles.
	static Rect Union( const Rect& rect1, const Rect& rect2 );
};

//----------------------------------------------------------------------------
// Rect Implementation
//----------------------------------------------------------------------------

Rect::Rect( )
{
}

Rect::Rect( _long ll, _long tt, _long rr, _long bb ) : l( ll ), t( tt ), r( rr ), b( bb )
{
}

Rect Rect::operator + ( ) const
{
	return Rect( l, t, r, b );
}

Rect Rect::operator - ( ) const
{
	return Rect( -l, -t, -r, -b );
}

_bool Rect::operator == ( const Rect& rect ) const
{
	return l == rect.l && t == rect.t && r == rect.r && b == rect.b;
}

_bool Rect::operator != ( const Rect& rect ) const
{
	return l != rect.l || t != rect.t || r != rect.r || b != rect.b;
}

Rect& Rect::operator += ( const Rect& rect )
{
	l += rect.l;
	t += rect.t;
	r += rect.r;
	b += rect.b;

	return *this;
}

Rect& Rect::operator -= ( const Rect& rect )
{
	l -= rect.l;
	t -= rect.t;
	r -= rect.r;
	b -= rect.b;

	return *this;
}

Rect Rect::operator + ( const Rect& rect ) const
{
	return Rect( l + rect.l, t + rect.t, r + rect.r, b + rect.b );
}

Rect Rect::operator - ( const Rect& rect ) const
{
	return Rect( l - rect.l, t - rect.t, r - rect.r, b - rect.b );
}

Rect& Rect::operator *= ( int scale )
{
	l *= scale;
	t *= scale;
	r *= scale;
	b *= scale;

	return *this;
}

Rect& Rect::operator /= ( int scale )
{
	l /= scale;
	t /= scale;
	r /= scale;
	b /= scale;

	return *this;
}

Rect Rect::operator * ( int scale ) const
{
	return Rect( l * scale, t * scale, r * scale, b * scale );
}

Rect Rect::operator / ( int scale ) const
{
	return Rect( l / scale, t / scale, r / scale, b / scale );
}

Rect& Rect::operator += ( const Point& point )
{
	l += point.x;
	t += point.y;
	r += point.x;
	b += point.y;

	return *this;
}

Rect& Rect::operator -= ( const Point& point )
{
	l -= point.x;
	t -= point.y;
	r -= point.x;
	b -= point.y;

	return *this;
}

Rect Rect::operator + ( const Point& point ) const
{
	return Rect( l + point.x, t + point.y, r + point.x, b + point.y );
}

Rect Rect::operator - ( const Point& point ) const
{
	return Rect( l - point.x, t - point.y, r - point.x, b - point.y );
}

_long Rect::Width( ) const
{
	return r - l;
}

_long Rect::Height( ) const
{
	return b - t;
}

Point Rect::Center( ) const
{
	return Point( ( l + r ) / 2, ( t + b ) / 2 );
}

Point Rect::LeftTop( ) const
{
	return Point( l, t );
}

Point Rect::LeftBottom( ) const
{
	return Point( l, b );
}

Point Rect::RightTop( ) const
{
	return Point( r, t );
}

Point Rect::RightBottom( ) const
{
	return Point( r, b );
}

Rect& Rect::Inflate( _long width, _long height )
{
	l -= width;
	r += width;
	t -= height;
	b += height;

	return *this;
}

Rect& Rect::Inflate( const Point& size )
{
	l -= size.x;
	r += size.x;
	t -= size.y;
	b += size.y;

	return *this;
}

Rect& Rect::Deflate( _long width, _long height )
{
	l += width;
	r -= width;
	t += height;
	b -= height;

	return *this;
}

Rect& Rect::Deflate( const Point& size )
{
	l += size.x;
	r -= size.x;
	t += size.y;
	b -= size.y;

	return *this;
}

_bool Rect::PointInRect( const Point& point ) const
{
	return point.x >= l && point.x <= r && point.y >= t && point.y <= b;
}

};