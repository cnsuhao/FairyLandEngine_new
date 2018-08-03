//============================================================================
// CallStack.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

extern "C" void* __cdecl _exception_info( );

namespace FL
{

//----------------------------------------------------------------------------
// DebugHelperLibrary
//----------------------------------------------------------------------------

struct DebugHelperLibrary
{
	_void*	mDbgHelperDll;
	_void*	mSymInitializeFunc;
	_void*	mSymCleanupFunc;
	_void*	mSymSetOptionsFunc;
	_void*	mSymGetSymFromAddrFunc;
	_void*	mSymGetLineFromAddrFunc;
	_void*	mSymFunctionTableAccessFunc;
	_void*	mSymGetModuleBaseFunc;
	_void*	mStackWalkFunc;

	DebugHelperLibrary( );
};

//----------------------------------------------------------------------------
// CallStackFrame
//----------------------------------------------------------------------------

struct CallStackFrame
{
	enum { _MAX_STRING_LENGTH = 512 };

	_char	mFuncName[ _MAX_STRING_LENGTH ];
	_char	mFileName[ _MAX_STRING_LENGTH ];
	_dword	mLineNumber;
};

//----------------------------------------------------------------------------
// CallStack
//----------------------------------------------------------------------------

class CallStack : public Link< CallStackFrame >
{
public:
	inline static CallStack GetCurrentCallStack( );
	inline static CallStack CreateCallStack( const _char* funcname, const _char* filename, _dword linenumber );

	static _void Cleanup( );

	_void Create( _void* exception );
	_void Create( const _char* funcname, const _char* filename, _dword linenumber );
	_void Release( );
};

//----------------------------------------------------------------------------
// CallStack Implementation
//----------------------------------------------------------------------------

CallStack CallStack::GetCurrentCallStack( )
{
	CallStack callstack;

	#ifdef _EXCEPTION_ENABLE

	__try { *( (_byte*) 0 ) = 0; }
	__except( callstack.Create( _exception_info( ) ), 1 ) { }

	#endif

	return callstack;
}

CallStack CallStack::CreateCallStack( const _char* funcname, const _char* filename, _dword linenumber )
{
	CallStack callstack;

	callstack.Create( funcname, filename, linenumber );

	return callstack;
}

};