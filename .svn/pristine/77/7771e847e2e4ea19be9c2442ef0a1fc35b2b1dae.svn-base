//============================================================================
// StringFormatter.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// StringFormatter
//----------------------------------------------------------------------------

//! This class provides useful functions of string format.
class StringFormatter
{
public:

	#ifdef _TYPECHECK_ENABLE

	private:
	static String FormatStringHelper( const _char* format, ... );
	static StringPtr FormatBufferHelper( _char* buffer, _dword size, const _char* format, ... );
	public:

	// Multiversion of FormatString with different number of parameter.
	static String FormatString( const _char* format )
	{
		return FormatStringHelper( format );
	}

	template < class T1 >
	static String FormatString( const _char* format, T1 p1 )
	{
		TypeChecker< T1 > checker1( p1 );

		return FormatStringHelper( format, p1 );
	}

	template < class T1, class T2 >
	static String FormatString( const _char* format, T1 p1, T2 p2 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );

		return FormatStringHelper( format, p1, p2 );
	}

	template < class T1, class T2, class T3 >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );

		return FormatStringHelper( format, p1, p2, p3 );
	}

	template < class T1, class T2, class T3, class T4 >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );

		return FormatStringHelper( format, p1, p2, p3, p4 );
	}

	template < class T1, class T2, class T3, class T4, class T5 >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );

		return FormatStringHelper( format, p1, p2, p3, p4, p5 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6 >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8 >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9 >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );
		TypeChecker< TE > checkere( pe );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );
		TypeChecker< TE > checkere( pe );
		TypeChecker< TF > checkerf( pf );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );
		TypeChecker< TE > checkere( pe );
		TypeChecker< TF > checkerf( pf );
		TypeChecker< TG > checkerg( pg );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );
		TypeChecker< TE > checkere( pe );
		TypeChecker< TF > checkerf( pf );
		TypeChecker< TG > checkerg( pg );
		TypeChecker< TH > checkerh( ph );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );
		TypeChecker< TE > checkere( pe );
		TypeChecker< TF > checkerf( pf );
		TypeChecker< TG > checkerg( pg );
		TypeChecker< TH > checkerh( ph );
		TypeChecker< TI > checkeri( pi );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI, class TJ >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi, TJ pj )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );
		TypeChecker< TE > checkere( pe );
		TypeChecker< TF > checkerf( pf );
		TypeChecker< TG > checkerg( pg );
		TypeChecker< TH > checkerh( ph );
		TypeChecker< TI > checkeri( pi );
		TypeChecker< TJ > checkerj( pj );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI, class TJ, class TK >
	static String FormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi, TJ pj, TK pk )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );
		TypeChecker< TE > checkere( pe );
		TypeChecker< TF > checkerf( pf );
		TypeChecker< TG > checkerg( pg );
		TypeChecker< TH > checkerh( ph );
		TypeChecker< TI > checkeri( pi );
		TypeChecker< TJ > checkerj( pj );
		TypeChecker< TK > checkerk( pk );

		return FormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj, pk );
	}

	// Multiversion of FormatBuffer with different number of parameter.
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format )
	{
		return FormatBufferHelper( buffer, size, format );
	}

	template < class T1 >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1 )
	{
		TypeChecker< T1 > checker1( p1 );

		return FormatBufferHelper( buffer, size, format, p1 );
	}

	template < class T1, class T2 >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );

		return FormatBufferHelper( buffer, size, format, p1, p2 );
	}

	template < class T1, class T2, class T3 >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3 );
	}

	template < class T1, class T2, class T3, class T4 >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4 );
	}

	template < class T1, class T2, class T3, class T4, class T5 >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6 >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6, p7 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8 >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6, p7, p8 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9 >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );
		TypeChecker< TE > checkere( pe );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );
		TypeChecker< TE > checkere( pe );
		TypeChecker< TF > checkerf( pf );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );
		TypeChecker< TE > checkere( pe );
		TypeChecker< TF > checkerf( pf );
		TypeChecker< TG > checkerg( pg );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );
		TypeChecker< TE > checkere( pe );
		TypeChecker< TF > checkerf( pf );
		TypeChecker< TG > checkerg( pg );
		TypeChecker< TH > checkerh( ph );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );
		TypeChecker< TE > checkere( pe );
		TypeChecker< TF > checkerf( pf );
		TypeChecker< TG > checkerg( pg );
		TypeChecker< TH > checkerh( ph );
		TypeChecker< TI > checkeri( pi );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI, class TJ >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi, TJ pj )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );
		TypeChecker< TE > checkere( pe );
		TypeChecker< TF > checkerf( pf );
		TypeChecker< TG > checkerg( pg );
		TypeChecker< TH > checkerh( ph );
		TypeChecker< TI > checkeri( pi );
		TypeChecker< TJ > checkerj( pj );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI, class TJ, class TK >
	static StringPtr FormatBuffer( _char* buffer, _dword size, const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi, TJ pj, TK pk )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );
		TypeChecker< T9 > checker9( p9 );
		TypeChecker< TA > checkera( pa );
		TypeChecker< TB > checkerb( pb );
		TypeChecker< TC > checkerc( pc );
		TypeChecker< TD > checkerd( pd );
		TypeChecker< TE > checkere( pe );
		TypeChecker< TF > checkerf( pf );
		TypeChecker< TG > checkerg( pg );
		TypeChecker< TH > checkerh( ph );
		TypeChecker< TI > checkeri( pi );
		TypeChecker< TJ > checkerj( pj );
		TypeChecker< TK > checkerk( pk );

		return FormatBufferHelper( buffer, size, format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj, pk );
	}

	#else

	//! Create a string with formatted datas.
	//! @param		format		Format-control string.
	//! @param		...			Optional arguments, datas need to be filled in the string.
	//! @return		The formated string.
	static String		FormatString( const _char* format, ... );

	//! Create a string with formatted datas, the string will copy into the buffer specified.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		format		Format-control string.
	//! @param		...			Optional arguments, datas need to be filled in the string.
	//! @return		The formated string.
	static StringPtr	FormatBuffer( _char* buffer, _dword size, const _char* format, ... );

	#endif

	//! Create a string with formatted datas.
	//! @param		format		Format-control string.
	//! @param		arguments	Pointer to list of arguments.
	//! @return		The formated string.
	static String		FormatStringV( const _char* format, const _void* arguments );

	//! Create a string with formatted datas, the string will copy into the buffer specified.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		format		Format-control string.
	//! @param		arguments	Pointer to list of arguments.
	//! @return		The formated string.
	static StringPtr	FormatBufferV( _char* buffer, _dword size, const _char* format, const _void* arguments );

	static String		FormatBytesString( _qword bytes );
	static StringPtr	FormatBytesBuffer( _char* buffer, _dword size, _qword bytes );

	static String		FormatNumberString( _dword number );
	static StringPtr	FormatNumberBuffer( _char* buffer, _dword size, _dword number );

	static String		FormatTimeString( _dword millisecond );
	static StringPtr	FormatTimeBuffer( _char* buffer, _dword size, _dword millisecond );

	static _dword		FormatCommandLine( _char* cmdline, _char* commands[], _dword cmdnumber, StringPtr delimiter = L" \t" );
	static _dword		FormatConfigString( const _char* source, Array< Pair< String, String > >& output, _char keydiv = L'=', _char argdiv = L',' );
	static _dword		FormatConfigWord( const _char* source, _char endword );

	static _char*		CopyString( _char* desbuffer, const _char* srcbuffer, _dword number = -1 );
	static _char*		AppendString( _char* desbuffer, const _char* srcbuffer, _dword number = -1 );
	static _char*		LowercaseString( _char* string );
	static _char*		UppercaseString( _char* string );
	static _dword		StringLength( const _char* string );

	static _chara*		CopyString( _chara* desbuffer, const _chara* srcbuffer, _dword number = -1 );

	static _chara*		LowercaseString( _chara* string );
	static _chara*		UppercaseString( _chara* string );
	static _dword		StringLength( const _chara* string );

	static _void		SplitString( StringPtr source, Array< String >& output, StringPtr division = L"\r\n", StringPtr trim = L" \t" );

	static _void		Replace( _char* buffer, _dword buffersize, StringPtr srcstring, StringPtr desstring );

	//! Convert an ansi string to unicode.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		string		The ansi string.
	//! @return		The pointer to the unicode string, it must be delete after use.
	static _charw* AnsiToUnicode( _charw* buffer, _dword size, const _chara* string, _dword* charcopy = _null );
	//! Convert an ansi string to unicode.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		string		The ansi string.
	//! @param		number		The number of characters to convert.
	//! @return		The pointer to the unicode string, it must be delete after use.
	static _charw* AnsiToUnicode( _charw* buffer, _dword size, const _chara* string, _dword number, _dword* charcopy = _null );
	//! Convert an unicode string to ansi.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		string		The unicode string.
	//! @return		The pointer to the ansi string, it must be delete after use.
	static _chara* UnicodeToAnsi( _chara* buffer, _dword size, const _charw* string, _dword* charcopy = _null );
	//! Convert an unicode string to ansi.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		string		The unicode string.
	//! @param		number		The number of characters to convert.
	//! @return		The pointer to the ansi string, it must be delete after use.
	static _chara* UnicodeToAnsi( _chara* buffer, _dword size, const _charw* string, _dword number, _dword* charcopy = _null );

	//! Convert an ansi string to unicode.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		string		The UTF8 string.
	//! @return		The pointer to the unicode string, it must be delete after use.
	static _charw* UTF8ToUnicode( _charw* buffer, _dword size, const _chara* string, _dword* charcopy = _null );
	//! Convert an ansi string to unicode.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		string		The UTF8 string.
	//! @param		number		The number of characters to convert.
	//! @return		The pointer to the unicode string, it must be delete after use.
	static _charw* UTF8ToUnicode( _charw* buffer, _dword size, const _chara* string, _dword number, _dword* charcopy = _null );
	//! Convert an unicode string to UTF-8.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		string		The unicode string.
	//! @return		The pointer to the UTF-8 string, it must be delete after use.
	static _chara* UnicodeToUTF8( _chara* buffer, _dword size, const _charw* string, _dword* charcopy = _null );
	//! Convert an unicode string to ansi.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		string		The unicode string.
	//! @param		number		The number of characters to convert.
	//! @return		The pointer to the UTF-8 string, it must be delete after use.
	static _chara* UnicodeToUTF8( _chara* buffer, _dword size, const _charw* string, _dword number, _dword* charcopy = _null );

	//! Determine the path is a full path of not, a full path will begin with "X:" or "\\"
	static _bool IsFullpath( StringPtr fullpath );

	//! Get the file name from the full path.
	//! @param		fullpath	The full path of a file.
	//! @param		extention	Whether need the extention of the the file name.
	//! @param		wholepath	Whether get whole path or not.
	//! @return		The file name without the path name.
	static String GetFileName( StringPtr fullpath, _bool extention = _true, _bool wholepath = _false );
	//! Get the file name from the full path, the string will copy into the buffer specified.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		fullpath	The full path of a file.
	//! @param		extention	Whether need the extention of the the file name.
	//! @param		wholepath	Whether get whole path or not.
	//! @return		The file name without the path name.
	static StringPtr GetFileName( _char* buffer, _dword size, StringPtr fullpath, _bool extention = _true, _bool wholepath = _false );

	//! Get the path name from the full path.
	//! @param		fullpath	The full path of a file.
	//! @param		wholepath	Whether get whole path or not.
	//! @return		The path name without the file name.
	static String GetPathName( StringPtr fullpath, _bool wholepath = _true );
	//! Get the path name from the full path, the string will copy into the buffer specified.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		fullpath	The full path of a file.
	//! @param		wholepath	Whether get whole path or not.
	//! @return		The path name without the file name.
	static StringPtr GetPathName( _char* buffer, _dword size, StringPtr fullpath, _bool wholepath = _true );

	//! Get the file extention from the full path.
	//! @param		fullpath	The full path of a file.
	//! @return		The file extention without the file name and path name.
	static String GetExtention( StringPtr fullpath );
	//! Get the file extention from the full path, the string will copy into the buffer specified.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		fullpath	The full path of a file.
	//! @return		The file extention without the file name and path name.
	static StringPtr GetExtention( _char* buffer, _dword size, StringPtr fullpath );

	//! Remove the file extention from the full path.
	//! @param		fullpath	The full path of a file.
	//! @return		The file name without extention.
	static String RemoveExtention( StringPtr fullpath );
	//! Remove the file extention from the full path, the string will copy into the buffer specified.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		fullpath	The full path of a file.
	//! @return		The file name without extention.
	static StringPtr RemoveExtention( _char* buffer, _dword size, StringPtr fullpath );

	//! Combine some string into one buffer.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		srcstring	The source string will copy to the buffer.
	//! @param		offset		The offset from the beginning of the buffer.
	//! @return		The length copied, include the terminal null.
	static _dword StringPtrToBuffer( _char* buffer, _dword size, StringPtr srcstring, _dword offset = 0 );

	//! Divide string from a buffer.
	//! @param		buffer		The buffer used to copy string.
	//! @param		size		The max size of buffer in number of characters.
	//! @param		index		The offset of the buffer to search.
	//! @return		The pointer to the buffer.
	static _char* BufferToStringPtr( _char* buffer, _dword size, _dword& offset );

	// Filter profanity from a string, and replace with a certain character.
	//! @param		string		The string to filters.
	//! @param		profanity	The profanity list, use as filter.
	//! @param		replacer	The character used to replace profanity.
	//! @return		True if some characters been replaced, or false if nothing changed.
	static _bool FilterProfanityFromString( String& string, const Array< String >& profanity, _char replacer );

	// Filter profanity from a string, and replace with a certain character.
	//! @param		string		The string to filters.
	//! @param		profanity	The profanity list in regular expression , use as filter.
	//! @param		replacer	The character used to replace profanity.
	//! @return		True if some characters been replaced, or false if nothing changed.
	static _bool FilterProfanityFromString( String& string, const Array< RegularExpression* >& profanity, _char replacer );
};

};