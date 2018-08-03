//============================================================================
// Console.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Console
//----------------------------------------------------------------------------

//! This class represents a console for dos-like programe, provides input & output function. 
class Console
{
public:

	#ifdef _TYPECHECK_ENABLE

	private:
	static _long PrintHelper( const _char* format, ... );
	static _long InputHelper( const _char* format, ... );
	public:

	// Multiversion of Print with different number of parameter.
	static _long Print( const _char* format )
	{
		return PrintHelper( format );
	}

	template < class T1 >
	static _long Print( const _char* format, T1 p1 )
	{
		TypeChecker< T1 > checker1( p1 );

		return PrintHelper( format, p1 );
	}

	template < class T1, class T2 >
	static _long Print( const _char* format, T1 p1, T2 p2 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );

		return PrintHelper( format, p1, p2 );
	}

	template < class T1, class T2, class T3 >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );

		return PrintHelper( format, p1, p2, p3 );
	}

	template < class T1, class T2, class T3, class T4 >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );

		return PrintHelper( format, p1, p2, p3, p4 );
	}

	template < class T1, class T2, class T3, class T4, class T5 >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );

		return PrintHelper( format, p1, p2, p3, p4, p5 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6 >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );

		return PrintHelper( format, p1, p2, p3, p4, p5, p6 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );

		return PrintHelper( format, p1, p2, p3, p4, p5, p6, p7 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8 >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );

		return PrintHelper( format, p1, p2, p3, p4, p5, p6, p7, p8 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9 >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
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

		return PrintHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa )
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

		return PrintHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb )
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

		return PrintHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc )
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

		return PrintHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd )
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

		return PrintHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe )
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

		return PrintHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf )
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

		return PrintHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg )
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

		return PrintHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph )
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

		return PrintHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi )
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

		return PrintHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI, class TJ >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi, TJ pj )
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

		return PrintHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI, class TJ, class TK >
	static _long Print( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi, TJ pj, TK pk )
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

		return PrintHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj, pk );
	}

	// Multiversion of Input with different number of parameter.
	static _long Input( const _char* format )
	{
		return InputHelper( format );
	}

	template < class T1 >
	static _long Input( const _char* format, T1 p1 )
	{
		TypeChecker< T1 > checker1( p1 );

		return InputHelper( format, p1 );
	}

	template < class T1, class T2 >
	static _long Input( const _char* format, T1 p1, T2 p2 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );

		return InputHelper( format, p1, p2 );
	}

	template < class T1, class T2, class T3 >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );

		return InputHelper( format, p1, p2, p3 );
	}

	template < class T1, class T2, class T3, class T4 >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );

		return InputHelper( format, p1, p2, p3, p4 );
	}

	template < class T1, class T2, class T3, class T4, class T5 >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );

		return InputHelper( format, p1, p2, p3, p4, p5 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6 >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );

		return InputHelper( format, p1, p2, p3, p4, p5, p6 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );

		return InputHelper( format, p1, p2, p3, p4, p5, p6, p7 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8 >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );

		return InputHelper( format, p1, p2, p3, p4, p5, p6, p7, p8 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9 >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
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

		return InputHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa )
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

		return InputHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb )
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

		return InputHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc )
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

		return InputHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd )
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

		return InputHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe )
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

		return InputHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf )
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

		return InputHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg )
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

		return InputHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph )
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

		return InputHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi )
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

		return InputHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI, class TJ >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi, TJ pj )
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

		return InputHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI, class TJ, class TK >
	static _long Input( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi, TJ pj, TK pk )
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

		return InputHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj, pk );
	}

	#else

	//! Print formatted output to the standard output stream.
	//! @param		format	Format-control string.
	//! @param		...			Optional arguments, datas need to be filled in the string.
	//! @return		The number of character print to console.
	static _long Print( const _char* format, ... );
	//! @return		The number of characters printed, or a negative value if an error occurs.
	//! Read formatted data from the standard input stream.
	//! @param		format	Format-control string.
	//! @param		...		Optional arguments, addresses of datas to be assigned.
	//! @return		The number of fields successfully converted and assigned, or a negative value if an error occurs.
	static _long Input( const _char* format, ... );

	#endif

	//! Read a character from the standard input stream.
	//! @param		none
	//! @return		The character read.
	static _char GetChar( );

	static _bool CreateConsole( );
	static _bool AttachParentConsole( );
};

};