//============================================================================
// Diagnose.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Fatal
//----------------------------------------------------------------------------

//! This class provides PS_FATAL macro to report error and terminate process.
class Fatal
{
public:
	//! @param		error			The error string, that is the parameter of the macro.
	//!								For example, PS_FATAL( "cant reach here" ) will create error string as "cant reach here".
	//! @param		filename		The name of the source file that throw the error.
	//! @param		linenumber		The line number of the source file where the assert failed.
	//! @param		popupdialog		Pop up the fatal dialog or not.
	static _void ReportError( const _char* error, const _char* filename, _dword linenumber, _bool popupdialog );
};

//----------------------------------------------------------------------------
// Assert
//----------------------------------------------------------------------------

//! This class provides FL_ASSERT macro to check some expression.
class Assert
{
public:
	//! Show a dialog to report error when assert failed.
	//! @param		error			The error string, that is the boolean expression.
	//!								For example, FL_ASSERT( a + b > c ) will create error string as "a + b > c".
	//! @param		filename		The name of the source file that throw the error.
	//! @param		linenumber		The line number of the source file where the assert failed.
	//! @param		popupdialog		Pop up the assert dialog or not.
	static _void ReportError( const _char* error, const _char* filename, _dword linenumber, _bool popupdialog );
};

//----------------------------------------------------------------------------
// Trace
//----------------------------------------------------------------------------

//! This class provides PS_TRACE macro to print some information in debug window.
class Trace
{
private:
	//! The target of trace information, debug window in IDE / trace window / log file.
	_dword		mTraceTarget;
	//! The handle of trace window.
	_handle		mTraceWindow;
	//! The handle of trace font used in trace window.
	_handle		mTraceFont;
	//! The handle of edit control used in trace window.
	_handle		mTraceEdit;
	//! The handle of log file.
	_handle		mTraceFile;
	//! The handle of log file.
	_handle		mErrorFile;
	//! Trace time at each trace message.
	_bool		mAutoTraceTime;
	//! Make an assert if tracing error.
	_bool		mAssertError;
	//! Trace text count.
	_dword		mTraceCount;
	//! Trace file size.
	_dword		mTraceFileSize;

	//! The trace window callback function.
	//! @param		window			Handle to the window.
	//! @param		msgid			The message ID.
	//! @param		wparam			Specifies additional message information.
	//! @param		lparam			Specifies additional message information.
	//! @return		Result of the message processing, depends on the message sent.
	static _long _stdcall TraceWndProc( _handle window, _dword msgid, _dword wparam, _dword lparam );

public:
	//! The target of trace information.
	enum _TRACE_TARGET
	{
		//! Skip trace.
		_TARGET_NONE		= 0,

		//! Trace to the output window of IDE, the default target.
		_TARGET_IDEOUTPUT	= 1,
		//! Trace to a popup window.
		_TARGET_TRACEWINDOW	= 2,
		//! Trace to a log file.
		_TARGET_TRACEFILE	= 3,
		//! Trace to stand console.
		_TARGET_CONSOLE		= 4,

		_TRACET_SINGLEFILE	= 0x40000000,
		_TRACET_LOCALFILE	= 0x80000000,
	};

	//! Set the target of trace information, all following trace information will be put there.
	//! @param		target		The target of trace information, one of _TRACE_TARGET.
	static _void	SetTraceTarget( _dword target, _handle targethandle = _null, _dword maxfilesize = 1024 * 1024 * 200 );
	static _dword	GetTraceTarget( );
	static _dword	GetTraceCount( );

	static _bool	IsIDEOutputPresent( );

	static _void	EnableAutoTraceTime( _bool enable );
	static _bool	IsAutoTraceTimeEnabled( );

	static _void	EnableAssertError( _bool enable );
	static _bool	IsAssertErrorEnabled( );

	static _void	TraceStringTime( );
	static _void	TraceErrorTime( );

	private:
	static _void	TraceStringHelper( const _char* format, ... );
	static _void	TraceErrorHelper( const _char* format, ... );
	public:

	#ifdef _TYPECHECK_ENABLE

	// Trace String.
	static _void TraceString( const _char* format )
	{
		return TraceStringHelper( format );
	}

	template < class T1 >
	static _void TraceString( const _char* format, T1 p1 )
	{
		TypeChecker< T1 > checker1( p1 );

		return TraceStringHelper( format, p1 );
	}

	template < class T1, class T2 >
	static _void TraceString( const _char* format, T1 p1, T2 p2 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );

		return TraceStringHelper( format, p1, p2 );
	}

	template < class T1, class T2, class T3 >
	static _void TraceString( const _char* format, T1 p1, T2 p2, T3 p3 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );

		return TraceStringHelper( format, p1, p2, p3 );
	}

	template < class T1, class T2, class T3, class T4 >
	static _void TraceString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );

		return TraceStringHelper( format, p1, p2, p3, p4 );
	}

	template < class T1, class T2, class T3, class T4, class T5 >
	static _void TraceString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );

		return TraceStringHelper( format, p1, p2, p3, p4, p5 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6 >
	static _void TraceString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );

		return TraceStringHelper( format, p1, p2, p3, p4, p5, p6 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
	static _void TraceString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );

		return TraceStringHelper( format, p1, p2, p3, p4, p5, p6, p7 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8 >
	static _void TraceString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );

		return TraceStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9 >
	static _void TraceString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
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

		return TraceStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA >
	static _void TraceString( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa )
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

		return TraceStringHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa );
	}

	// Trace Error.
	static _void TraceError( const _char* format )
	{
		return TraceErrorHelper( format );
	}

	template < class T1 >
	static _void TraceError( const _char* format, T1 p1 )
	{
		TypeChecker< T1 > checker1( p1 );

		return TraceErrorHelper( format, p1 );
	}

	template < class T1, class T2 >
	static _void TraceError( const _char* format, T1 p1, T2 p2 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );

		return TraceErrorHelper( format, p1, p2 );
	}

	template < class T1, class T2, class T3 >
	static _void TraceError( const _char* format, T1 p1, T2 p2, T3 p3 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );

		return TraceErrorHelper( format, p1, p2, p3 );
	}

	template < class T1, class T2, class T3, class T4 >
	static _void TraceError( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );

		return TraceErrorHelper( format, p1, p2, p3, p4 );
	}

	template < class T1, class T2, class T3, class T4, class T5 >
	static _void TraceError( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );

		return TraceErrorHelper( format, p1, p2, p3, p4, p5 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6 >
	static _void TraceError( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );

		return TraceErrorHelper( format, p1, p2, p3, p4, p5, p6 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7 >
	static _void TraceError( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );

		return TraceErrorHelper( format, p1, p2, p3, p4, p5, p6, p7 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8 >
	static _void TraceError( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8 )
	{
		TypeChecker< T1 > checker1( p1 );
		TypeChecker< T2 > checker2( p2 );
		TypeChecker< T3 > checker3( p3 );
		TypeChecker< T4 > checker4( p4 );
		TypeChecker< T5 > checker5( p5 );
		TypeChecker< T6 > checker6( p6 );
		TypeChecker< T7 > checker7( p7 );
		TypeChecker< T8 > checker8( p8 );

		return TraceErrorHelper( format, p1, p2, p3, p4, p5, p6, p7, p8 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9 >
	static _void TraceError( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9 )
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

		return TraceErrorHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	}

	template < class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class TA >
	static _void TraceError( const _char* format, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, TA pa )
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

		return TraceErrorHelper( format, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa );
	}

	#else

	//! Trace information to debug window.
	//! @param		format		Format-control string.
	//! @param		...			Optional arguments, datas need to be filled in the string.
	static _void TraceString( const _char* format, ... );

	//! Trace information to debug window.
	//! @param		format		Format-control string.
	//! @param		...			Optional arguments, datas need to be filled in the string.
	static _void TraceError( const _char* format, ... );

	#endif

	//! Constructor, initialize trace system.
	//! @param		none.
	Trace( );
	//! Destructor, finalize trace system.
	//! @param		none.
	~Trace( );
};

//----------------------------------------------------------------------------
// VTuneAnalyzer
//----------------------------------------------------------------------------

class VTuneAnalyzer
{
public:
	VTuneAnalyzer( );
	~VTuneAnalyzer( );

	static _void BeginAnalyse( );
	static _void StopAnalyse( );
};

//----------------------------------------------------------------------------
// PS_FATAL Macro
//----------------------------------------------------------------------------

#ifdef _FATAL_ENABLE

	#ifdef _FATAL_POPUP_ERROR

	#define PS_FATAL( x ) \
		{ \
			FL::Fatal::ReportError( x, _FILENAME, (_long) _LINENUMBER, _true ); \
		}

	#else

	#define PS_FATAL( x ) \
		{ \
			FL::Fatal::ReportError( x, _FILENAME, (_long) _LINENUMBER, _false ); \
		}

	#endif

#else
	#define PS_FATAL( x ) \
		{ \
		}
#endif

//----------------------------------------------------------------------------
// FL_ASSERT Macro
//----------------------------------------------------------------------------

#ifdef _ASSERT_ENABLE

	#ifdef _ASSERT_POPUP_ERROR

	#define FL_ASSERT( x ) \
		{ \
			if ( (x) == 0 ) \
				FL::Assert::ReportError( L#x, _FILENAME, (_long) _LINENUMBER, _true ); \
		}

	#define FL_ASSERT2( x, y ) \
		{ \
			if ( (x) == 0 ) \
				FL::Assert::ReportError( y, _FILENAME, (_long) _LINENUMBER, _true ); \
		}

	#else

	#define FL_ASSERT( x ) \
		{ \
			if ( (x) == 0 ) \
				FL::Assert::ReportError( L#x, _FILENAME, (_long) _LINENUMBER, _false ); \
		}

	#define FL_ASSERT2( x, y ) \
		{ \
			if ( (x) == 0 ) \
				FL::Assert::ReportError( y, _FILENAME, (_long) _LINENUMBER, _false ); \
		}

	#endif

#else

	#define FL_ASSERT( x ) \
		{ \
		}

	#define FL_ASSERT2( x, y ) \
		{ \
		}

#endif

//----------------------------------------------------------------------------
// PS_VERIFY Macro
//----------------------------------------------------------------------------

#ifdef _VERIFY_ENABLE

	#ifdef _ASSERT_POPUP_ERROR

	#define PS_VERIFY( x ) \
		{ \
			if ( (x) == 0 ) \
				FL::Assert::ReportError( L#x, _FILENAME, (_long) _LINENUMBER, _true ); \
		}

	#else

	#define PS_VERIFY( x ) \
		{ \
			if ( (x) == 0 ) \
				FL::Assert::ReportError( L#x, _FILENAME, (_long) _LINENUMBER, _false ); \
		}

	#endif

#else

	#define PS_VERIFY( x ) { (x); }

#endif

//----------------------------------------------------------------------------
// PS_TRACE Macro
//----------------------------------------------------------------------------

#ifdef _TRACE_ENABLE

	#define PS_TRACETIME( ) \
		{ \
			Trace::TraceStringTime( ); \
		}
	#define PS_TRACE( x ) \
		{ \
			Trace::TraceString( x ); \
		}
	#define PS_TRACE1( x, a ) \
		{ \
			Trace::TraceString( x, a ); \
		}
	#define PS_TRACE2( x, a, b ) \
		{ \
			Trace::TraceString( x, a, b ); \
		}
	#define PS_TRACE3( x, a, b, c ) \
		{ \
			Trace::TraceString( x, a, b, c ); \
		}
	#define PS_TRACE4( x, a, b, c, d ) \
		{ \
			Trace::TraceString( x, a, b, c, d ); \
		}
	#define PS_TRACE5( x, a, b, c, d, e ) \
		{ \
			Trace::TraceString( x, a, b, c, d, e ); \
		}
	#define PS_TRACE6( x, a, b, c, d, e, f ) \
		{ \
			Trace::TraceString( x, a, b, c, d, e, f ); \
		}
	#define PS_TRACE7( x, a, b, c, d, e, f, g ) \
		{ \
			Trace::TraceString( x, a, b, c, d, e, f, g ); \
		}
	#define PS_TRACE8( x, a, b, c, d, e, f, g, h ) \
		{ \
			Trace::TraceString( x, a, b, c, d, e, f, g, h ); \
		}
	#define PS_TRACE9( x, a, b, c, d, e, f, g, h, i ) \
		{ \
			Trace::TraceString( x, a, b, c, d, e, f, g, h, i ); \
		}
	#define PS_TRACE10( x, a, b, c, d, e, f, g, h, i, j ) \
		{ \
			Trace::TraceString( x, a, b, c, d, e, f, g, h, i, j ); \
		}

	#define PS_ERRORTIME( ) \
		{ \
			Trace::TraceErrorTime( ); \
		}
	#define PS_ERROR( x ) \
		{ \
			Trace::TraceError( x ); \
		}
	#define PS_ERROR1( x, a ) \
		{ \
			Trace::TraceError( x, a ); \
		}
	#define PS_ERROR2( x, a, b ) \
		{ \
			Trace::TraceError( x, a, b ); \
		}
	#define PS_ERROR3( x, a, b, c ) \
		{ \
			Trace::TraceError( x, a, b, c ); \
		}
	#define PS_ERROR4( x, a, b, c, d ) \
		{ \
			Trace::TraceError( x, a, b, c, d ); \
		}
	#define PS_ERROR5( x, a, b, c, d, e ) \
		{ \
			Trace::TraceError( x, a, b, c, d, e ); \
		}
	#define PS_ERROR6( x, a, b, c, d, e, f ) \
		{ \
			Trace::TraceError( x, a, b, c, d, e, f ); \
		}
	#define PS_ERROR7( x, a, b, c, d, e, f, g ) \
		{ \
			Trace::TraceError( x, a, b, c, d, e, f, g ); \
		}
	#define PS_ERROR8( x, a, b, c, d, e, f, g, h ) \
		{ \
			Trace::TraceError( x, a, b, c, d, e, f, g, h ); \
		}
	#define PS_ERROR9( x, a, b, c, d, e, f, g, h, i ) \
		{ \
			Trace::TraceError( x, a, b, c, d, e, f, g, h, i ); \
		}
	#define PS_ERROR10( x, a, b, c, d, e, f, g, h, i, j ) \
		{ \
			Trace::TraceError( x, a, b, c, d, e, f, g, h, i, j ); \
		}

#else

	#define PS_TRACETIME( ) \
		{ \
		}
	#define PS_TRACE( x ) \
		{ \
		}
	#define PS_TRACE1( x, a ) \
		{ \
		}
	#define PS_TRACE2( x, a, b ) \
		{ \
		}
	#define PS_TRACE3( x, a, b, c ) \
		{ \
		}
	#define PS_TRACE4( x, a, b, c, d ) \
		{ \
		}
	#define PS_TRACE5( x, a, b, c, d, e ) \
		{ \
		}
	#define PS_TRACE6( x, a, b, c, d, e, f ) \
		{ \
		}
	#define PS_TRACE7( x, a, b, c, d, e, f, g ) \
		{ \
		}
	#define PS_TRACE8( x, a, b, c, d, e, f, g, h ) \
		{ \
		}
	#define PS_TRACE9( x, a, b, c, d, e, f, g, h, i ) \
		{ \
		}
	#define PS_TRACE10( x, a, b, c, d, e, f, g, h, i, j ) \
		{ \
		}

	#define PS_ERRORTIME( ) \
		{ \
		}
	#define PS_ERROR( x ) \
		{ \
		}
	#define PS_ERROR1( x, a ) \
		{ \
		}
	#define PS_ERROR2( x, a, b ) \
		{ \
		}
	#define PS_ERROR3( x, a, b, c ) \
		{ \
		}
	#define PS_ERROR4( x, a, b, c, d ) \
		{ \
		}
	#define PS_ERROR5( x, a, b, c, d, e ) \
		{ \
		}
	#define PS_ERROR6( x, a, b, c, d, e, f ) \
		{ \
		}
	#define PS_ERROR7( x, a, b, c, d, e, f, g ) \
		{ \
		}
	#define PS_ERROR8( x, a, b, c, d, e, f, g, h ) \
		{ \
		}
	#define PS_ERROR9( x, a, b, c, d, e, f, g, h, i ) \
		{ \
		}
	#define PS_ERROR10( x, a, b, c, d, e, f, g, h, i, j ) \
		{ \
		}

#endif

};