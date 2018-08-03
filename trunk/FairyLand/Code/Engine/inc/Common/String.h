//============================================================================
// String.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// String
//----------------------------------------------------------------------------

//! This class represents a mutable null-terminated unicode string.

//! Differents from StringPtr, this class new and delete the memory used by the string, so it can be changed.
class String : public StringPtr
{
private:
	_dword	mLength;
	_dword	mSize;

public:
	//! Constructor, create a string only contains the terminal null.
	//! @param		none
	String( );
	//! Constructor, create a string by copy from another string.
	//! @param		string		Pointer to a null-terminated string.
	String( const _char* string );
	//! Constructor, create a string by copy some characters from another string.
	//! @param		string		Pointer to a null-terminated string.
	//! @param		number		The number of characters to be copy.
	String( const _char* string, _dword number );
	//! Copy-Constructor, create a string by copy another string object.
	//! @param		string	The second string object.
	String( const String& string );
	//! Destructor, delete the memory of the string.
	//! @param		none
	~String( );

	//! Copy the string from another one, and recreates memory to hold it.
	//! @param		string		Pointer to a null-terminated string.
	//! @return		The reference of current string.
	String& operator = ( const _char* string );
	//! Copy the string from another one, and recreates memory to hold it.
	//! @param		string		The string object.
	//! @return		The reference of current string.
	String& operator = ( const String& string );
	//! Append another string to the end of the current one.
	//! @param		string		Pointer to a null-terminated string.
	//! @return		The reference of current string.
	String& operator += ( const _char* string );
	//! Append another string to the end of the current one, and return a new one.
	//! @param		string		The string object.
	//! @return		A new string that buildup by the current one appended another.
	String operator + ( const _char* string ) const;
	//! Append a character to the end of the string.
	//! @param		character	The character to be append.
	//! @return		The reference of current string.
	String& operator += ( _char character );
	//! Append a character to the end of the string.
	//! @param		character	The character to be append.
	//! @return		A new string that buildup by the string appended a character.
	String operator + ( _char character ) const;

	//! Get the Length of the string, excluding the terminal null, overwrite parent class function.
	//! @param		none
	//! @return		The Length of the string.
	inline _dword Length( ) const;

	//! Get the number of bytes used by string, include the terminal null, overwrite parent class function.
	//! @param		none
	//! @return		The number of bytes used by string.
	inline _dword SizeOfBytes( ) const;

	//! Create a string with formatted datas.
	//! @param		format		Format-control string.
	//! @param		...			Optional arguments, datas need to be filled in the string.
	//! @return		The formated string.

	#ifdef _TYPECHECK_ENABLE

	private:
	String& FormatHelper( const _char* format, ... );
	public:

	// Multiversion of Format with different number of parameter.
	String& Format( const _char* format )
	{
		return FormatHelper( format );
	}

	template < class T1 >
	String& Format( const _char* format, T1 p1 )
	{
		TypeChecker< T1 > checker1( p1 );

		return FormatHelper( format, p1 );
	}

	template < class T1, class T2 >
	String& Format( const _char* format, T1 p1, T2 p2 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );

		return FormatHelper( format, p1, p2 );
	}

	template < class T1, class T2, class T3 >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );

		return FormatHelper( format, p1, p2, p3 );
	}

	template < class T1, class T2, class T3, class T4 >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );

		return FormatHelper( format, p1, p2, p3, p4 );
	}

	template < class T1, class T2, class T3, class T4, class T5 >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );

		return FormatHelper( format, p1, p2, p3, p4, p5 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6 >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );

		return FormatHelper( format, p1, p2, p3, p4, p5, p6 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );

		return FormatHelper( format, p1, p2, p3, p4, p5, p6, p7 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8 >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );

		return FormatHelper( format, p1, p2, p3, p4, p5, p6, p7, p8 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9 >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
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

		return FormatHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa )
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

		return FormatHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb )
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

		return FormatHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc )
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

		return FormatHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd )
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

		return FormatHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe )
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

		return FormatHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf )
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

		return FormatHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg )
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

		return FormatHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph )
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

		return FormatHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi )
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

		return FormatHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI, class TJ >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi, TJ pj )
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

		return FormatHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI, class TJ, class TK >
	String& Format( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi, TJ pj, TK pk )
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

		return FormatHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj, pk );
	}

	#else

	String& Format( const _char* format, ... );

	#endif

	//! Copy the string from another string, and recreates memory to hold it.
	//! @param		string		Pointer to a null-terminated string.
	//! @param		length		The length of string.
	//! @return		The reference of current string.
	String& CopyString( const _char* string, _dword length );

	//! Create a string with formatted datas.
	//! @param		format		Format-control string.
	//! @param		arguments	Pointer to list of arguments.
	//! @return		The formated string.
	String& FormatV( const _char* format, const _void* arguments );

	//! Convert the string to lowercase.
	//! @return		The pointer of the string with lowercase.
	String& Lowercase( );
	//! Convert the string to uppercase.
	//! @return		The pointer of the string with uppercase.
	String& Uppercase( );

	//! Insert a character into the index of the string.
	//! @param		index		The index of the string
	//! @param		character	The character to be inserted.
	//! @remark		The character will be inserted before the index.<br>
	//!				For example, "Hello".Insert( 0, 'W' ) will results "WHello",
	//!				and "Hello".Insert( 5, 'W' ) will results "HelloW".
	_void Insert( _dword index, _char character );
	//! Insert a substring into the index of the string.
	//! @param		index		The index of the string.
	//! @param		string		The substring to be inserted.
	//! @remark		The substring will be inserted before the index.<br>
	//!				For example, "Hello".Insert( 0, "World" ) will results "WorldHello",
	//!				and "Hello".Insert( 5, "World" ) will results "HelloWorld".
	_void Insert( _dword index, StringPtr string );
	//! Remove some characters at the index of the string.
	//! @param		index		The index of the string.
	//! @param		number		The number of characters to be removed.
	//! @remark		The character will be removed start from the index.<br>
	//!				For example, "Hello".Remove( 0, 2 ) will results "llo",
	//!				"Hello".Remove( 4, 1 ) will results "Hell",
	//!				and "Hello".Remove( 4, 2 ) will failed ( return false ).
	_void Remove( _dword index, _dword number );
	//! Search and replace a substring to another.
	//! @param		srcstring	The substring to be searched.
	//! @param		desstring	The substring to be replaced.
	//! @return		True if successfully searched & replaced, false otherwise.
	_bool Replace( StringPtr srcstring, StringPtr desstring );
	//! Search and replace a character to another one..
	//! @param		index		The index of character to be replaced.
	//! @param		character	The character to be used.
	//! @return		True if successfully searched & replaced, false otherwise.
	_bool Replace( _dword index, _char character );
	//! Search and replace a character to another one..
	//! @param		index		The index of character to be replaced.
	//! @param		number		The number for character to be replaced.
	//! @param		character	The character to be used.
	//! @return		True if successfully searched & replaced, false otherwise.
	_bool Replace( _dword index, _dword number, _char character );

	_void ReplaceAll( _char oldchar, _char newchar );

	String& TrimLeft( _char character );
	String& TrimRight( _char character );
	String& TrimBoth( _char character );

	String& TrimLeft( StringPtr charset );
	String& TrimRight( StringPtr charset );
	String& TrimBoth( StringPtr charset );

	String SubString( _dword start, _dword length ) const;
	String LeftSubString( _char delimiter ) const;
	String LeftSubString( StringPtr delimiter ) const;
	String RightSubString( _char delimiter ) const;
	String RightSubString( StringPtr delimiter ) const;
};

//----------------------------------------------------------------------------
// String Implementation
//----------------------------------------------------------------------------

_dword String::Length( ) const
{
	return mLength;
}

_dword String::SizeOfBytes( ) const
{
	return ( mLength + 1 ) * sizeof( _char );
}

};