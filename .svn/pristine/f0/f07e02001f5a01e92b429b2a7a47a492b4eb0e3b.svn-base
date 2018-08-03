//============================================================================
// Diagnose.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Fatal Implementation
//----------------------------------------------------------------------------

_void Fatal::ReportError( const _char* error, const _char* filename, _dword linenumber, _bool popupdialog )
{
	String information = StringFormatter::FormatString( L"Build Date : %s\r\nFatal : %s\r\n%s:%d\r\n\r\n", _BUILDDATE, error, filename, linenumber );

	information += L"================================================================================\r\n"
				   L"Call stack\r\n";

	// Get call stack when exception occur.
	CallStack callstack = CallStack::GetCurrentCallStack( );

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

		(* (Exception::OnCatchException) catchfunctionpointer )( information );

		System::SetEnvironment( L"FairyLandException", catchfunctionpointer );
	}

	if ( popupdialog )
	{
		// Copy error information to clipboard.
		Clipboard::Copy( information );

		String instruction = L"The clipboard now contains this text above.";

		String message = information + instruction;

		// Show the error dialog.
		::MessageBox( _null, message, L"Possibility Space Diagnose System",
			MB_OK | MB_SETFOREGROUND | MB_DEFAULT_DESKTOP_ONLY );
	}
}

//----------------------------------------------------------------------------
// Assert Implementation
//----------------------------------------------------------------------------

_void Assert::ReportError( const _char* error, const _char* filename, _dword linenumber, _bool popupdialog )
{
	String information = StringFormatter::FormatString( L"Build Date : %s\r\nAssert Failed : %s\r\n%s:%d\r\n\r\n", _BUILDDATE, error, filename, linenumber );

	information += L"================================================================================\r\n"
				   L"Call stack\r\n";

	// Get call stack when exception occur.
	CallStack callstack = CallStack::GetCurrentCallStack( );

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
		(* (Exception::OnCatchException) catchfunctionpointer )( information );

	if ( popupdialog )
	{
		// Copy error information to clipboard.
		Clipboard::Copy( information );

		String instruction = L"\r\n\r\nThe clipboard now contains this text above.\r\n\r\n"
							 L"Press ABORT to exit the program immediately.\r\n"
							 L"Press RETRY to break if under debugger and abort otherwise.\r\n"
							 L"Press IGNORE to ignore the assertion and continue program execution.";

		String message = information + instruction;

		// Show the error dialog.
		_long retval = ::MessageBox( _null, message, L"Possibility Space Diagnose System",
			MB_ABORTRETRYIGNORE | MB_SETFOREGROUND | MB_DEFAULT_DESKTOP_ONLY );

		if ( retval == IDRETRY )
		{
			__asm int 3
		}
		else if ( retval == IDIGNORE )
		{
			return;
		}
		else
		{
			::ExitProcess( 3 );
		}
	}
}

//----------------------------------------------------------------------------
// Trace Implementation
//----------------------------------------------------------------------------

_long Trace::TraceWndProc( _handle window, _dword msgid, _dword wparam, _dword lparam )
{
	Trace* globaltrace = GetFairyLandGlobal( ).mTrace;

	if ( globaltrace == _null )
		return 0;

	switch ( msgid )
	{
		case WM_CREATE:
		{
			globaltrace->mTraceFont = ::CreateFont( 12, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Lucida Console" );

			_dword editstyle = WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE | ES_READONLY | ES_AUTOVSCROLL | ES_MULTILINE;

			globaltrace->mTraceEdit = ::CreateWindowEx( 0, L"edit", L"", editstyle, 0, 0, 0, 0,
				(HWND) window, (HMENU) 1, ::GetModuleHandle( _null ), _null );

			::SendMessage( (HWND) globaltrace->mTraceEdit, WM_SETFONT, (_dword) globaltrace->mTraceFont, 0 );

			break;
		}

		case WM_SIZE:
		{
			_long w = Dword( lparam ).LoWord( ), h = Dword( lparam ).HiWord( );

			::MoveWindow( (HWND) globaltrace->mTraceEdit, 0, 20, w, h - 20, _false );

			break;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT paintstruct;

			::BeginPaint( (HWND) window, &paintstruct );

			RECT clientrect;
			::GetClientRect( (HWND) window, &clientrect );
			clientrect.bottom = 20;

			::SelectObject( paintstruct.hdc, (HFONT) globaltrace->mTraceFont );

			::DrawText( paintstruct.hdc, L"PS Dash Trace Window", -1,
				&clientrect, DT_CENTER | DT_VCENTER | DT_SINGLELINE );

			::EndPaint( (HWND) window, &paintstruct );

			break;
		}

		case WM_NCHITTEST:
		{
			return HTCAPTION;
		}

		case WM_USER + 10:
		case WM_USER + 20:
		{
			if ( ::SendMessage( (HWND) globaltrace->mTraceEdit, EM_GETLINECOUNT, 0, 0 ) > 1000 )
			{
				::SendMessage( (HWND) globaltrace->mTraceEdit, EM_SETSEL, 0, -1 );
				::SendMessage( (HWND) globaltrace->mTraceEdit, EM_REPLACESEL, 0, (_dword) L"" );
			}

			_dword length = ::GetWindowTextLength( (HWND) globaltrace->mTraceEdit );

			::SendMessage( (HWND) globaltrace->mTraceEdit, EM_SETSEL, length, length );
			::SendMessage( (HWND) globaltrace->mTraceEdit, EM_REPLACESEL, 0, lparam );
			::SendMessage( (HWND) globaltrace->mTraceEdit, WM_VSCROLL, SB_BOTTOM, 0 );

			if ( msgid == WM_USER + 20 )
				delete[] (_char*) lparam;

			break;
		}
	}

	return (_long) ::DefWindowProc( (HWND) window, msgid, wparam, lparam );
}

_void Trace::SetTraceTarget( _dword target, _handle targethandle, _dword maxfilesize )
{
	Trace* globaltrace = GetFairyLandGlobal( ).mTrace;

	if ( target == _TARGET_NONE )
	{
		delete globaltrace;
		return;
	}

	if ( globaltrace == _null )
		globaltrace = new Trace;

	_bool localfile  = ( target & _TRACET_LOCALFILE ) != 0;
	_bool singlefile = ( target & _TRACET_SINGLEFILE ) != 0;

	target &= ~_TRACET_LOCALFILE;
	target &= ~_TRACET_SINGLEFILE;

	globaltrace->mTraceTarget	= target;
	globaltrace->mTraceFileSize	= maxfilesize;

	if ( globaltrace->mTraceTarget == _TARGET_TRACEWINDOW )
	{
		if ( targethandle != _null )
		{
			::DuplicateHandle( ::GetCurrentProcess( ), targethandle, ::GetCurrentProcess( ),
				&globaltrace->mTraceWindow, 0, 0, DUPLICATE_SAME_ACCESS );
		}

		if ( globaltrace->mTraceWindow == _null )
		{
			WNDCLASSEX windowclass		= { sizeof( WNDCLASSEX ) };
			windowclass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
			windowclass.lpfnWndProc		= (WNDPROC) TraceWndProc;
			windowclass.hInstance		= ::GetModuleHandle( _null );
			windowclass.hbrBackground	= (HBRUSH) ::GetStockObject( WHITE_BRUSH );
			windowclass.lpszClassName	= L"PS Dash Trace Window";

			::RegisterClassEx( &windowclass );

			globaltrace->mTraceWindow = ::CreateWindowEx( WS_EX_TOOLWINDOW | WS_EX_TOPMOST, windowclass.lpszClassName,
				L"", WS_POPUP | WS_BORDER | WS_THICKFRAME, 0, 0, 400, 300, _null, _null, _null, _null );
		}

		::ShowWindow( (HWND) globaltrace->mTraceWindow, SW_SHOW );
	}

	if ( globaltrace->mTraceTarget == _TARGET_TRACEFILE )
	{
		if ( targethandle != _null )
		{
			::DuplicateHandle( ::GetCurrentProcess( ), targethandle, ::GetCurrentProcess( ),
				&globaltrace->mTraceFile, 0, 0, DUPLICATE_SAME_ACCESS );
		}

		if ( globaltrace->mTraceFile == _null )
		{
			_char buffer[1024];
			::GetModuleFileName( _null, buffer, 1024 );

			String pathname = StringFormatter::GetPathName( buffer );
			String filename = StringFormatter::GetFileName( buffer );

			if ( localfile )
				pathname = FileSystem::GetCurrentDir( );

			String logfilename, errfilename;

			if ( singlefile )
			{
				// Build log file name from the current time and exe file name.
				logfilename = StringFormatter::FormatString( L"%s\\%s [Trace].log",
					(const _char*) pathname, (const _char*) filename );

				errfilename = StringFormatter::FormatString( L"%s\\%s [Error].log",
					(const _char*) pathname, (const _char*) filename );
			}
			else
			{
				// Create directory if needed.
				FileSystem::CreateDir( pathname + L"\\Log" );

				// Build log file name from the current time and exe file name.
				logfilename = StringFormatter::FormatString( L"%s\\Log\\[%s] %s [Trace].log", (const _char*) pathname,
					(const _char*) System::GetCurrentLocalTime( ).ToString( ), (const _char*) filename );

				errfilename = StringFormatter::FormatString( L"%s\\Log\\[%s] %s [Error].log", (const _char*) pathname,
					(const _char*) System::GetCurrentLocalTime( ).ToString( ), (const _char*) filename );
			}

			globaltrace->mTraceFile = ::CreateFile( logfilename, GENERIC_WRITE, FILE_SHARE_READ,
				_null, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, _null );

			globaltrace->mErrorFile = ::CreateFile( errfilename, GENERIC_WRITE, FILE_SHARE_READ,
				_null, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, _null );

			if ( globaltrace->mTraceFile != _null )
			{
				_word unicodeflag = 0xFEFF; _dword bytes = 0;
				::WriteFile( globaltrace->mTraceFile, &unicodeflag, sizeof( _word ), &bytes, _null );
			}

			if ( globaltrace->mErrorFile != _null )
			{
				_word unicodeflag = 0xFEFF; _dword bytes = 0;
				::WriteFile( globaltrace->mErrorFile, &unicodeflag, sizeof( _word ), &bytes, _null );
			}
		}
	}

	if ( globaltrace->mTraceTarget != _TARGET_TRACEWINDOW && globaltrace->mTraceWindow != _null )
		::ShowWindow( (HWND) globaltrace->mTraceWindow, SW_HIDE );
}

_dword Trace::GetTraceTarget( )
{
	Trace* globaltrace = GetFairyLandGlobal( ).mTrace;

	if ( globaltrace == _null )
		return 0;

	return globaltrace->mTraceTarget;
}

_dword Trace::GetTraceCount( )
{
	Trace* globaltrace = GetFairyLandGlobal( ).mTrace;

	if ( globaltrace == _null )
		return 0;

	return globaltrace->mTraceCount;
}

_bool Trace::IsIDEOutputPresent( )
{
	return ::IsDebuggerPresent( );
}

_void Trace::EnableAutoTraceTime( _bool enable )
{
	Trace* globaltrace = GetFairyLandGlobal( ).mTrace;

	if ( globaltrace == _null )
		return;

	globaltrace->mAutoTraceTime = enable;
}

_bool Trace::IsAutoTraceTimeEnabled( )
{
	Trace* globaltrace = GetFairyLandGlobal( ).mTrace;

	if ( globaltrace == _null )
		return _false;

	return globaltrace->mAutoTraceTime;
}

_void Trace::EnableAssertError( _bool enable )
{
	Trace* globaltrace = GetFairyLandGlobal( ).mTrace;

	if ( globaltrace == _null )
		return;

	globaltrace->mAssertError = enable;
}

_bool Trace::IsAssertErrorEnabled( )
{
	Trace* globaltrace = GetFairyLandGlobal( ).mTrace;

	if ( globaltrace == _null )
		return _false;

	return globaltrace->mAssertError;
}

_void Trace::TraceStringTime( )
{
	Time currenttime = System::GetCurrentSystemTime( );

	Trace::TraceString( L"[%.4d/%.2d/%.2d %.2d:%.2d:%.2d] ", currenttime.mYear, currenttime.mMonth, currenttime.mDay,
		currenttime.mHour, currenttime.mMinute, currenttime.mSecond );
}

_void Trace::TraceErrorTime( )
{
	Time currenttime = System::GetCurrentSystemTime( );

	Trace::TraceError( L"[%.4d/%.2d/%.2d %.2d:%.2d:%.2d] ", currenttime.mYear, currenttime.mMonth, currenttime.mDay,
		currenttime.mHour, currenttime.mMinute, currenttime.mSecond );
}

_void Trace::TraceStringHelper( const _char* format, ... )
{
	Trace* globaltrace = GetFairyLandGlobal( ).mTrace;

	if ( globaltrace == _null )
		return;

	if ( globaltrace->mAutoTraceTime )
	{
		globaltrace->mAutoTraceTime = _false;
		TraceStringTime( );
		globaltrace->mAutoTraceTime = _true;
	}

	globaltrace->mTraceCount ++;

	_char buffer[2048];
	StringFormatter::FormatBufferV( buffer, 2048, format, (_chara*)( &format ) + sizeof( format ) );

	switch ( globaltrace->mTraceTarget )
	{
		case _TARGET_IDEOUTPUT:
		{
			::OutputDebugString( buffer );

			break;
		}

		case _TARGET_TRACEWINDOW:
		{
			if ( globaltrace->mTraceWindow == _null )
				break;

			if ( ::GetWindowThreadProcessId( (HWND) globaltrace->mTraceWindow, _null ) == ::GetCurrentThreadId( ) )
			{
				::SendMessage( (HWND) globaltrace->mTraceWindow, WM_USER + 10, 0, (_dword) buffer );
			}
			else
			{
				_char* newbuffer = new _char[2048];
				::wcscpy( newbuffer, buffer );

				::SendNotifyMessage( (HWND) globaltrace->mTraceWindow, WM_USER + 20, 0, (_dword) newbuffer );
			}

			break;
		}

		case _TARGET_TRACEFILE:
		{
			if ( globaltrace->mTraceFile != _null )
			{
				if ( ::GetFileSize( globaltrace->mTraceFile, _null ) > globaltrace->mTraceFileSize )
				{
					::SetFilePointer( globaltrace->mTraceFile, 0, _null, FILE_BEGIN );
					::SetEndOfFile( globaltrace->mTraceFile );

					_word unicodeflag = 0xFEFF; _dword bytes = 0;
					::WriteFile( globaltrace->mTraceFile, &unicodeflag, sizeof( _word ), &bytes, _null );
				}
 
				_dword bytes = 0;
				::WriteFile( globaltrace->mTraceFile, buffer, StringPtr( buffer ).SizeOfBytes( ) - sizeof( _char ), &bytes, _null );
			}

			break;
		}

		case _TARGET_CONSOLE:
		{
			Console::Print( L"%s", buffer );

			break;
		}
	}
}

_void Trace::TraceErrorHelper( const _char* format, ... )
{
	Trace* globaltrace = GetFairyLandGlobal( ).mTrace;

	if ( globaltrace == _null )
		return;

	if ( globaltrace->mAutoTraceTime )
	{
		globaltrace->mAutoTraceTime = _false;
		TraceErrorTime( );
		globaltrace->mAutoTraceTime = _true;
	}

	globaltrace->mTraceCount ++;

	_char buffer[2048];
	StringFormatter::FormatBufferV( buffer, 2048, format, (_chara*)( &format ) + sizeof( format ) );

	switch ( globaltrace->mTraceTarget )
	{
		case _TARGET_IDEOUTPUT:
		{
			::OutputDebugString( buffer );

			break;
		}

		case _TARGET_TRACEWINDOW:
		{
			if ( globaltrace->mTraceWindow == _null )
				break;

			if ( ::GetWindowThreadProcessId( (HWND) globaltrace->mTraceWindow, _null ) == ::GetCurrentThreadId( ) )
			{
				::SendMessage( (HWND) globaltrace->mTraceWindow, WM_USER + 10, 0, (_dword) buffer );
			}
			else
			{
				_char* newbuffer = new _char[2048];
				::wcscpy( newbuffer, buffer );

				::SendNotifyMessage( (HWND) globaltrace->mTraceWindow, WM_USER + 20, 0, (_dword) newbuffer );
			}

			break;
		}

		case _TARGET_TRACEFILE:
		{
			if ( globaltrace->mErrorFile != _null )
			{
				if ( ::GetFileSize( globaltrace->mErrorFile, _null ) > globaltrace->mTraceFileSize )
				{
					::SetFilePointer( globaltrace->mErrorFile, 0, _null, FILE_BEGIN );
					::SetEndOfFile( globaltrace->mErrorFile );

					_word unicodeflag = 0xFEFF; _dword bytes = 0;
					::WriteFile( globaltrace->mErrorFile, &unicodeflag, sizeof( _word ), &bytes, _null );
				}

				_dword bytes = 0;
				::WriteFile( globaltrace->mErrorFile, buffer, StringPtr( buffer ).SizeOfBytes( ) - sizeof( _char ), &bytes, _null );
			}

			break;
		}

		case _TARGET_CONSOLE:
		{
			Console::Print( L"%s", buffer );

			break;
		}
	}

	if ( globaltrace->mAssertError )
	{
		// Show the error dialog.
		_long retval = ::MessageBox( _null, buffer, L"Possibility Space Diagnose System",
			MB_ABORTRETRYIGNORE | MB_SETFOREGROUND | MB_DEFAULT_DESKTOP_ONLY );

		if ( retval == IDRETRY )
		{
			__asm int 3
		}
		else if ( retval == IDIGNORE )
		{
			return;
		}
		else
		{
			::ExitProcess( 3 );
		}
	}
}

#ifndef _TYPECHECK_ENABLE

_void Trace::TraceString( const _char* format, ... )
{
	_char buffer[2048];
	StringFormatter::FormatBufferV( buffer, 2048, format, (_chara*)( &format ) + sizeof( format ) );

	TraceStringHelper( L"%s", buffer );
}

_void Trace::TraceError( const _char* format, ... )
{
	_char buffer[2048];
	StringFormatter::FormatBufferV( buffer, 2048, format, (_chara*)( &format ) + sizeof( format ) );

	TraceErrorHelper( L"%s", buffer );
}

#endif

Trace::Trace( )
{
	mTraceTarget	= _null;
	mTraceWindow	= _null;
	mTraceFont		= _null;
	mTraceEdit		= _null;
	mTraceFile		= _null;
	mErrorFile		= _null;
	mAutoTraceTime	= _false;
	mAssertError	= _false;
	mTraceCount		= 0;
	mTraceFileSize	= 0;

	GetFairyLandGlobal( ).mTrace = this;
}

Trace::~Trace( )
{
	GetFairyLandGlobal( ).mTrace = _null;

	if ( mTraceWindow != _null )
		::CloseWindow( (HWND) mTraceWindow );

	if ( mTraceFont != _null )
		::DeleteObject( (HFONT) mTraceFont );

	if ( mTraceFile != _null )
		::CloseHandle( (HANDLE) mTraceFile );

	if ( mErrorFile != _null )
		::CloseHandle( (HANDLE) mErrorFile );
}

//----------------------------------------------------------------------------
// VTuneAnalyzer
//----------------------------------------------------------------------------

VTuneAnalyzer::VTuneAnalyzer( )
{
	BeginAnalyse( );
}

VTuneAnalyzer::~VTuneAnalyzer( )
{
	StopAnalyse( );
}

_void VTuneAnalyzer::BeginAnalyse( )
{
#ifdef _VTUNE_ENABLE
	VTResume( );
#endif
}

_void VTuneAnalyzer::StopAnalyse( )
{
#ifdef _VTUNE_ENABLE
	VTPause( );
#endif
}