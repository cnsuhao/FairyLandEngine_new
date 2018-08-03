//============================================================================
// Region.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Region
//----------------------------------------------------------------------------

class Region
{
public:
	const static Region cNullRegion;
	const static Region cInitRegion;

	union
	{
		struct
		{
			Vector2	mLeftTop;
			Vector2	mRightBottom;
		};

		struct
		{
			_float	l;
			_float	t;
			_float	r;
			_float	b;
		};
	};

public:
	inline Region( );
	inline Region( _float l, _float t, _float r, _float b );
	inline Region( const Vector2& lefttop, const Vector2& rightbottom );
	inline Region( const Rect& rect );

	inline _bool operator == ( const Region& region ) const;
	inline _bool operator != ( const Region& region ) const;

	inline Region& operator += ( const Vector2& point );

	inline _float Width( ) const;
	inline _float Height( ) const;

	static Region Intersect( const Region& region1, const Region& region2 );
	static Region Union( const Region& region1, const Region& region2 );
};

//----------------------------------------------------------------------------
// Region Implementation
//----------------------------------------------------------------------------

Region::Region( )
{
}

Region::Region( _float ll, _float tt, _float rr, _float bb )
	: l( ll ), t( tt ), r( rr ), b( bb )
{
}

Region::Region( const Vector2& lefttop, const Vector2& rightbottom )
	: mLeftTop( lefttop ), mRightBottom( rightbottom )
{
}

Region::Region( const Rect& rect )
	: mLeftTop( (_float) rect.l, (_float) rect.t ), mRightBottom( (_float) rect.r, (_float) rect.b )
{
}

_bool Region::operator == ( const Region& region ) const
{
	return mLeftTop == region.mLeftTop && mRightBottom == region.mRightBottom;
}

_bool Region::operator != ( const Region& region ) const
{
	return mLeftTop != region.mLeftTop || mRightBottom != region.mRightBottom;
}

Region& Region::operator += ( const Vector2& point )
{
	mLeftTop.x		= Math::Min( mLeftTop.x, point.x );
	mLeftTop.y		= Math::Min( mLeftTop.y, point.y );
	mRightBottom.x	= Math::Max( mRightBottom.x, point.x );
	mRightBottom.y	= Math::Max( mRightBottom.y, point.y );

	return *this;
}

_float Region::Width( ) const
{
	return mRightBottom.x - mLeftTop.x;
}

_float Region::Height( ) const
{
	return mRightBottom.y - mLeftTop.y;
}

};