//============================================================================
// File.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// File
//----------------------------------------------------------------------------

//! This class represents a system file, provides create/write/read functions.
class File : public KernelObject
{
public:
	//! Flag of how to create a file.
	enum _CREATE_FLAG
	{
		//! Creates a new file, if the file exists, the function will fail.
		_CREATE_NEW		= 1,
		//! Creates a new file, if the file exists, the function overwrites it.
		_CREATE_ALWAYS	= 2,
		//! Opens the file, if the file does not exist, the function will fail.
		_OPEN_EXISTING	= 3,
		//! Opens the file if the file exists, or create it if the it does not exist.
		_OPEN_ALWAYS	= 4,
	};

	//! Flag of how to operate a file.
	enum _OPERATION_FLAG
	{
		//! Perform all SocketOperations on a file.
		_OPERATION_ALL		= 0x10000000,
		//! Execute a file.
		_OPERATION_EXECUTE	= 0x20000000,
		//! Write into a file.
		_OPERATION_WRITE	= 0x40000000,
		//! Read from a file.
		_OPERATION_READ		= 0x80000000,
	};

	//! Flag of how to share the file with other processes.
	enum _SHARE_FLAG
	{
		//! Do not share the file.
		_SHARE_NONE				= 0x00000000,
		//! Other processes can only read the file.
		_SHARE_READ				= 0x00000001,
		//! Other processes can only write the file.
		_SHARE_WRITE			= 0x00000002,
		//! Delete on close.
		_SHARE_DELETEONCLOSE	= 0x10000000,

		_SHARE_MASK				= 0x0FFFFFFF,
	};

public:
	//! Open (or create) a file.
	//! @param		filename		The name of the file to open (or to create).
	//! @param		createflag		Flag of how to create a file, one of _CREATE_FLAG.
	//! @param		operateflag		Flag of how to operate a file, one of _SocketOperation_FLAG.
	//! @param		shareflag		Flag of how to share the file with other processes, one of _SHARE_FLAG.
	//! @param		overlapped		True if the file is being opened or created for asynchronous I/O.
	//! @return		True indicates success, false indicates failure.
	_bool Open( StringPtr filename, _dword createflag, _dword operateflag, _dword shareflag = _SHARE_NONE, _bool overlapped = _false );
	//! Read from a file.
	//! @param		buffer			Pointer to the buffer that receives the data read from the file.
	//! @param		length			Number of bytes to be read from the file.
	//! @param		bytesread		Pointer to the number of bytes read.
	//! @param		overlapped		Overlapped I/O structure.
	//! @return		True indicates success, false indicates failure.
	_bool Read( _void* buffer, _dword length, _dword* bytesread = _null, Overlapped* overlapped = _null );
	//! Write into a file.
	//! @param		buffer			Pointer to the buffer containing the data to write to the file.
	//! @param		length			Number of bytes to write to the file.
	//! @param		byteswritten	Pointer to the number of bytes written.
	//! @param		overlapped		Overlapped I/O structure.
	//! @return		True indicates success, false indicates failure.
	_bool Write( const _void* buffer, _dword length, _dword* byteswritten = _null, Overlapped* overlapped = _null );
	//! Clears the buffers of the file and causes all buffered data to be written to the file.
	//! @param		none
	//! @return		True indicates success, false indicates failure.
	_bool Flush( );

	_bool WriteUnicodeFlag( );
	_bool WriteEndline( _bool unicode = _true );
	_bool WriteString( StringPtr string, _bool unicode = _true );
	
	#ifdef _TYPECHECK_ENABLE

	private:
	_bool WriteFormatStringHelper( const _char* format, ... );
	public:

	// Multiversion of Format with different number of parameter.
	_bool WriteFormatString( const _char* format )
	{
		return WriteFormatStringHelper( format );
	}

	template < class T1 >
	_bool WriteFormatString( const _char* format, T1 p1 )
	{
		TypeChecker< T1 > checker1( p1 );

		return WriteFormatStringHelper( format, p1 );
	}

	template < class T1, class T2 >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );

		return WriteFormatStringHelper( format, p1, p2 );
	}

	template < class T1, class T2, class T3 >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );

		return WriteFormatStringHelper( format, p1, p2, p3 );
	}

	template < class T1, class T2, class T3, class T4 >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );

		return WriteFormatStringHelper( format, p1, p2, p3, p4 );
	}

	template < class T1, class T2, class T3, class T4, class T5 >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6 >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8 >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9 >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
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

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa )
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

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb )
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

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc )
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

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd )
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

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe )
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

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf )
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

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg )
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

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph )
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

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi )
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

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI, class TJ >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi, TJ pj )
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

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA, class TB, class TC, class TD, class TE, class TF, class TG, class TH, class TI, class TJ, class TK >
	_bool WriteFormatString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa, TB pb, TC pc, TD pd, TE pe, TF pf, TG pg, TH ph, TI pi, TJ pj, TK pk )
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

		return WriteFormatStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc, pd, pe, pf, pg, ph, pi, pj, pk );
	}

	#else

	_bool WriteFormatString( const _char* format, ... );

	#endif

	//! Move the file pointer from the begin of the file.
	//! @param		distance		Number of bytes to move.
	//! @return		True indicates success, false indicates failure.
	//! @remarks	A positive distance moves the file pointer forward in the file,
	//!				and a negative value moves the file pointer backward.
	_bool SeekFromBegin( _long distance );
	//! Move the file pointer from current position.
	//! @param		distance		Number of bytes to move.
	//! @return		True indicates success, false indicates failure.
	//! @remarks	A positive distance moves the file pointer forward in the file,
	//!				and a negative value moves the file pointer backward.
	_bool SeekFromCurrent( _long distance );
	//! Move the file pointer from the end of the file.
	//! @param		distance		Number of bytes to move.
	//! @return		True indicates success, false indicates failure.
	//! @remarks	A positive distance moves the file pointer forward in the file,
	//!				and a negative value moves the file pointer backward.
	_bool SeekFromEnd( _long distance );

	//! Get the current offset of file pointer of the file.
	//! @param		none
	//! @return		The offset of file pointer from begin of the file in number of bytes.
	_dword GetOffset( ) const;

	//! Get the size, in bytes, of the file.
	//! @param		none
	//! @return		Size of the file in bytes, or -1 indicates failure.
	_dword GetSize( ) const;
	//! Set the size of the file, maybe truncate or extend it.
	//! @param		size			Size of the file in bytes.
	//! @return		True indicates success, false indicates failure.
	_bool SetSize( _dword size );
};

};