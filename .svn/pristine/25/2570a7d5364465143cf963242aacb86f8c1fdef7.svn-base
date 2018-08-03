//============================================================================
// CallStack.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// DebugHelperLibrary Implementation
//----------------------------------------------------------------------------

DebugHelperLibrary::DebugHelperLibrary( )
{
	mDbgHelperDll				= _null;
	mSymInitializeFunc			= _null;
	mSymCleanupFunc				= _null;
	mSymSetOptionsFunc			= _null;
	mSymGetSymFromAddrFunc		= _null;
	mSymGetLineFromAddrFunc		= _null;
	mSymFunctionTableAccessFunc	= _null;
	mSymGetModuleBaseFunc		= _null;
	mStackWalkFunc				= _null;
}

//----------------------------------------------------------------------------
// CallStack Implementation
//----------------------------------------------------------------------------

_void CallStack::Cleanup( )
{
	DebugHelperLibrary* globaldbglib = GetFairyLandGlobal( ).mDebug;

	if ( globaldbglib != _null && globaldbglib->mSymCleanupFunc != _null )
	{
		typedef _bool (_stdcall *_SymCleanupFunc)( _handle );
		( *( (_SymCleanupFunc) globaldbglib->mSymCleanupFunc ) )( ::GetCurrentProcess( ) );
	}
}

_void CallStack::Create( _void* exception )
{
	DebugHelperLibrary* globaldbglib = GetFairyLandGlobal( ).mDebug;

	if ( globaldbglib == _null )
	{
		globaldbglib = (DebugHelperLibrary*) ::HeapAlloc( ::GetProcessHeap( ), 0, sizeof( DebugHelperLibrary ) );

		_handle dbghelperdll = ::LoadLibrary( L"Dbghelp.dll" );

		// Load function from dll.
		if ( dbghelperdll != _null )
		{
			globaldbglib->mDbgHelperDll					= dbghelperdll;
			globaldbglib->mSymInitializeFunc			= ::GetProcAddress( (HMODULE) dbghelperdll, "SymInitialize" );
			globaldbglib->mSymCleanupFunc				= ::GetProcAddress( (HMODULE) dbghelperdll, "SymCleanup" );
			globaldbglib->mSymSetOptionsFunc			= ::GetProcAddress( (HMODULE) dbghelperdll, "SymSetOptions" );
			globaldbglib->mSymGetSymFromAddrFunc		= ::GetProcAddress( (HMODULE) dbghelperdll, "SymGetSymFromAddr" );
			globaldbglib->mSymGetLineFromAddrFunc		= ::GetProcAddress( (HMODULE) dbghelperdll, "SymGetLineFromAddr" );
			globaldbglib->mSymFunctionTableAccessFunc	= ::GetProcAddress( (HMODULE) dbghelperdll, "SymFunctionTableAccess" );
			globaldbglib->mSymGetModuleBaseFunc			= ::GetProcAddress( (HMODULE) dbghelperdll, "SymGetModuleBase" );
			globaldbglib->mStackWalkFunc				= ::GetProcAddress( (HMODULE) dbghelperdll, "StackWalk" );
		}

		GetFairyLandGlobal( ).mDebug = globaldbglib;
	}

	if ( globaldbglib == _null )
		return;

	if ( globaldbglib->mSymInitializeFunc == _null )
		return;

	if ( globaldbglib->mSymSetOptionsFunc == _null )
		return;

	if ( globaldbglib->mSymGetSymFromAddrFunc == _null )
		return;

	if ( globaldbglib->mSymGetLineFromAddrFunc == _null )
		return;

	if ( globaldbglib->mSymFunctionTableAccessFunc == _null )
		return;

	if ( globaldbglib->mSymGetModuleBaseFunc == _null )
		return;

	if ( globaldbglib->mStackWalkFunc == _null )
		return;

	typedef _dword (_stdcall *_SymSetOptionsFunc)( _dword );
	( *( (_SymSetOptionsFunc) globaldbglib->mSymSetOptionsFunc ) )( SYMOPT_CASE_INSENSITIVE | SYMOPT_DEFERRED_LOADS );

	typedef _bool (_stdcall *_SymInitializeFunc)( _void*, _char*, _bool );
	( *( (_SymInitializeFunc) globaldbglib->mSymInitializeFunc ) )( ::GetCurrentProcess( ), _null, _true );

	EXCEPTION_POINTERS* exppointer	= (EXCEPTION_POINTERS*) exception;
	EXCEPTION_RECORD*	exprecord	= exppointer->ExceptionRecord;
	CONTEXT*			context		= exppointer->ContextRecord;

	// Create a frame of stack, prepare to walk stack.
	STACKFRAME stackframe = { 0 };
	stackframe.AddrPC.Offset	= context->Eip;
	stackframe.AddrPC.Mode		= AddrModeFlat;
	stackframe.AddrStack.Offset	= context->Esp;
	stackframe.AddrStack.Mode	= AddrModeFlat;
	stackframe.AddrFrame.Offset	= context->Ebp;
	stackframe.AddrFrame.Mode	= AddrModeFlat;

	CallStackFrame callstackframe;

	while ( _true )
	{
		// Get the next call stack, if cant find, exit walk.
		typedef _bool (_stdcall *_StackWalkFunc)( _dword, _void*, _void*, _void*, _void*, _void*, _void*, _void*, _void* );
		if ( ( *( (_StackWalkFunc) globaldbglib->mStackWalkFunc ) )( IMAGE_FILE_MACHINE_I386, ::GetCurrentProcess( ), ::GetCurrentThread( ), &stackframe, context, _null, globaldbglib->mSymFunctionTableAccessFunc, globaldbglib->mSymGetModuleBaseFunc, _null ) == 0 )
			break;

		Memory::MemCpy( callstackframe.mFuncName, L"Unknown", 16 );
		Memory::MemCpy( callstackframe.mFileName, L"Unknown", 16 );
		callstackframe.mLineNumber = 0;

		if ( stackframe.AddrFrame.Offset != 0 )
		{
			_dword offset = 0;

			_byte symbolbuffer[ sizeof( IMAGEHLP_SYMBOL ) + CallStackFrame::_MAX_STRING_LENGTH ];
			IMAGEHLP_SYMBOL* symbol	= (IMAGEHLP_SYMBOL*) symbolbuffer;

			symbol->SizeOfStruct	= sizeof( symbolbuffer );
			symbol->MaxNameLength	= CallStackFrame::_MAX_STRING_LENGTH;

			// Get function symbol from address.
			typedef _bool (_stdcall *_SymGetSymFromAddrFunc)( _void*, _dword, _void*, _void* );
			if ( ( *( (_SymGetSymFromAddrFunc) globaldbglib->mSymGetSymFromAddrFunc ) )( ::GetCurrentProcess( ), stackframe.AddrPC.Offset, &offset, symbol ) == _true )
			{
				// The symbol->Name is ansi, so convert it to unicode.
				StringFormatter::AnsiToUnicode( callstackframe.mFuncName, CallStackFrame::_MAX_STRING_LENGTH, symbol->Name );

				if ( StringPtr( L"FL::CallStack::GetCurrentCallStack" ) == callstackframe.mFuncName )
					continue;
			}

			IMAGEHLP_LINE lineinfo = { 0 };

			// Get file name & line number from address.
			typedef _bool (_stdcall *_SymGetLineFromAddrFunc)( _void*, _dword, _void*, _void* );
			if ( ( *( (_SymGetLineFromAddrFunc) globaldbglib->mSymGetLineFromAddrFunc ) )( ::GetCurrentProcess( ), stackframe.AddrPC.Offset, &offset, &lineinfo ) == _true )
			{
				// The lineinfo.FileName is ansi, so convert it to unicode.
				StringFormatter::AnsiToUnicode( callstackframe.mFileName, CallStackFrame::_MAX_STRING_LENGTH, lineinfo.FileName );

				callstackframe.mLineNumber = lineinfo.LineNumber;
			}
		}

		// Create link node to store stack frame.
		Node* linknode = (Node*) ::HeapAlloc( ::GetProcessHeap( ), 0, sizeof( Node ) );
		linknode->mPrev		= _null;
		linknode->mNext		= _null;
		linknode->mElement	= callstackframe;

		// Insert to stack frame list.
		Link< CallStackFrame >::InsertTail( linknode );
	}
}

_void CallStack::Create( const _char* funcname, const _char* filename, _dword linenumber )
{
	// Create link node to store stack frame.
	Node* linknode = (Node*) ::HeapAlloc( ::GetProcessHeap( ), 0, sizeof( Node ) );
	linknode->mPrev		= _null;
	linknode->mNext		= _null;

	if ( funcname != _null && filename != _null )
	{
		StringFormatter::CopyString( linknode->mElement.mFuncName, funcname );
		StringFormatter::CopyString( linknode->mElement.mFileName, filename );
	}
	else
	{
		linknode->mElement.mFuncName[0] = 0;
		linknode->mElement.mFileName[0] = 0;
	}

	linknode->mElement.mLineNumber = linenumber;

	// Insert to stack frame list.
	Link< CallStackFrame >::InsertTail( linknode );
}

_void CallStack::Release( )
{
	while ( mHead != _null )
	{
		Node* temp = mHead; mHead = temp->mNext;

		::HeapFree( ::GetProcessHeap( ), 0, temp );
	}

	Link< CallStackFrame >::Clear( );
}