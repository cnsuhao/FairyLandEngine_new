//============================================================================
// Exception.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Exception
//----------------------------------------------------------------------------

//! This class catch the unhandled exception, and report to user.
class Exception
{
private:
	//! Throw exception to windows again or not.
	static _bool	sThrowException;

public:
	typedef _void (*OnCatchException)( StringPtr expstring );

	//! The exception filter function, used to catch the unhandled exception.
	//! @param		exception		Exception pointer.
	//! @return		Function code of how to process exception, always be EXCEPTION_EXECUTE_HANDLER.
	static _long __stdcall ExceptionFilter( _void* exception );

public:
	//! Initialize exception system.
	//! @param		throwexception	Throw exception to windows again or not.
	static _void Initialize( _bool throwexception = _false );

	//! Finalize exception system.
	//! @param		none.
	static _void Finalize( );

	//! Set call back function to catch exception.
	//! @param		funcpointer		Callback function pointer.
	static _void SetCatchExceptionCallback( OnCatchException funcpointer );
};

};