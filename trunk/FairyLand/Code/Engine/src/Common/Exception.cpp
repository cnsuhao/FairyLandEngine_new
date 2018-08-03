//============================================================================
// Exception.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Exception Implementation
//----------------------------------------------------------------------------

_bool Exception::sThrowException = _false;

_long Exception::ExceptionFilter( _void* exception )
{
	EXCEPTION_POINTERS* exppointer	= (EXCEPTION_POINTERS*) exception;
	EXCEPTION_RECORD*	exprecord	= exppointer->ExceptionRecord;
	CONTEXT*			context		= exppointer->ContextRecord;

	_char procname[1024];
	::GetModuleFileName( _null, procname, 1024 );
	_char pathname[1024];
	StringFormatter::GetPathName( pathname, 1024, procname );
	_char filename[1024];
	StringFormatter::GetFileName( filename, 1024, procname, _false );

	_void* minidumpfunc = _null;

	Library dbghelpdll;

	// Load function from dll.
	if ( dbghelpdll.Load( L"Dbghelp.dll" ) == _true )
	{
		minidumpfunc = dbghelpdll.GetFunction( L"MiniDumpWriteDump" );

		if ( minidumpfunc != _null )
		{
			Time curtime = System::GetCurrentLocalTime( );

			// Build log file name from the current time and exe file name.
			_char logfilename[1024];
			StringFormatter::FormatBuffer( logfilename, 1024, L"%s\\Log\\[%.4d-%.2d-%.2d %.2d-%.2d-%.2d(%.4d)] %s [EXP].dmp",
				(const _char*) pathname, curtime.mYear, curtime.mMonth, curtime.mDay, curtime.mHour, curtime.mMinute, curtime.mSecond, curtime.mMilliseconds, (const _char*) filename );

			// Create log file to dump exception.
			File dumpfile;
			if ( dumpfile.Open( logfilename, File::_CREATE_ALWAYS, File::_OPERATION_WRITE ) == _true )
			{
				// Dump exception to the file.
				MINIDUMP_EXCEPTION_INFORMATION expinfo;
				expinfo.ThreadId			= ::GetCurrentThreadId( );
				expinfo.ExceptionPointers	= exppointer;
				expinfo.ClientPointers		= 0;

				typedef _bool (_stdcall *_MiniDumpWriteDumpFunc)( _void*, _dword, _void*, _dword, _void*, _void*, _void* );

				( *( (_MiniDumpWriteDumpFunc) minidumpfunc ) )( ::GetCurrentProcess( ),
					::GetCurrentProcessId( ), (_handle) dumpfile, MiniDumpNormal, &expinfo, _null, _null );
			}
		}

		dbghelpdll.Free( );
	}

	String information = StringFormatter::FormatString( L"Build Date : %s\r\nException Code : %08X\r\nFault address  : %08X\r\n\r\n",
		_BUILDDATE, exprecord->ExceptionCode, exprecord->ExceptionAddress );

	information += L"================================================================================\r\n"
				   L"Call stack\r\n";

	// Get call stack when exception occur.
	CallStack callstack;
	callstack.Create( exception );

	for ( CallStack::Iterator it = callstack.GetHeadIterator( ); it.Valid( ); it ++ )
	{
		const CallStackFrame& stackframe = it;

		_char buffer[1024];
		StringFormatter::FormatBuffer( buffer, 1024, L"%s => %s:%u\r\n",
			stackframe.mFuncName, stackframe.mFileName, stackframe.mLineNumber );

		information += buffer;
	}

	information += L"================================================================================\r\n";

	callstack.Release( );

	CallStack::Cleanup( );

	_bool asserterror = Trace::IsAssertErrorEnabled( );

	Trace::EnableAssertError( _false );
	PS_ERROR1( L"\r\n%s", (const _char*) information )
	Trace::EnableAssertError( asserterror );

	_void* catchfunctionpointer = _null;
	System::GetEnvironment( L"FairyLandException", catchfunctionpointer );

	if ( catchfunctionpointer != _null )
	{
		System::SetEnvironment( L"FairyLandException", (_void*) _null );

		(* (OnCatchException) catchfunctionpointer )( information );

		System::SetEnvironment( L"FairyLandException", catchfunctionpointer );
	}

#ifdef _ASSERT_POPUP_ERROR

	// Copy error information to clipboard.
	Clipboard::Copy( information );

	information += L"\r\n\r\nThe clipboard now contains this text above, and the exception had been dump to file.";

	// Show the error dialog.
	::MessageBox( _null, information, L"Possibility Space Diagnose System",
		MB_OK | MB_SETFOREGROUND | MB_DEFAULT_DESKTOP_ONLY );

#endif

	return sThrowException ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER;
}

_void Exception::Initialize( _bool throwexception )
{
	::SetUnhandledExceptionFilter( (PTOP_LEVEL_EXCEPTION_FILTER) ExceptionFilter );

	sThrowException = throwexception;
}

_void Exception::Finalize( )
{
	::SetUnhandledExceptionFilter( _null );
}

_void Exception::SetCatchExceptionCallback( OnCatchException funcpointer )
{
	System::SetEnvironment( L"FairyLandException", (_void*) funcpointer );
}