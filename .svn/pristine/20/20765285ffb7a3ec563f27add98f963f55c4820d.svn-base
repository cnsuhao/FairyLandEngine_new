//============================================================================
// Console.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Console Implementation
//----------------------------------------------------------------------------

#ifdef _TYPECHECK_ENABLE

_long Console::PrintHelper( const _char* format, ... )
{
	FL_ASSERT( format != _null )

	_char buffer[4096];
	::_vsnwprintf( buffer, 4096, format, (_chara*)( &format ) + sizeof( format ) );
	buffer[4095] = 0;

	_dword written = 0;
	::WriteConsole( ::GetStdHandle( STD_OUTPUT_HANDLE ), buffer, StringPtr( buffer ).Length( ), &written, _null );

	return written;

	//return ::vwprintf( format, (_chara*)( &format ) + sizeof( format ) );
}

_long Console::InputHelper( const _char* format, ... )
{
	_long paramnumber = 0;

	// Find how many parameters we need.
	for ( _long i = 0; format[i] != 0; i ++ )
	{
		if ( format[i] == '%' && format[ i + 1 ] != '%' && format[ i + 1 ] != '*' )
			paramnumber ++;
	}

	paramnumber *= sizeof( _void* );

	_chara* parameters = (_chara*)( &format ) + sizeof( format );

	// Make a local stack, and fill local stack the way wscanf likes it.
	__asm sub esp, paramnumber;
	__asm mov ecx, paramnumber;
	__asm mov esi, parameters;
	__asm mov edi, esp;
	__asm rep movsb;

	_long retval = ::wscanf( format );

	__asm add esp, paramnumber;

	return retval;
}

#else

_long Console::Print( const _char* format, ... )
{
	return ::vwprintf( format, (_chara*)( &format ) + sizeof( format ) );
}

_long Console::Input( const _char* format, ... )
{
	_long paramnumber = 0;

	// Find how many parameters we need.
	for ( _long i = 0; format[i] != 0; i ++ )
	{
		if ( format[i] == '%' && format[ i + 1 ] != '%' && format[ i + 1 ] != '*' )
			paramnumber ++;
	}

	paramnumber *= sizeof( _void* );

	_chara* parameters = (_chara*)( &format ) + sizeof( format );

	// Make a local stack, and fill local stack the way wscanf likes it.
	__asm sub esp, paramnumber;
	__asm mov ecx, paramnumber;
	__asm mov esi, parameters;
	__asm mov edi, esp;
	__asm rep movsb;

	_long retval = ::wscanf( format );

	__asm add esp, paramnumber;

	return retval;
}

#endif

_char Console::GetChar( )
{
	return ::getwchar( );
}

_bool Console::CreateConsole( )
{
	return ::AllocConsole( );
}

_bool Console::AttachParentConsole( )
{
	return _true;

	//return ::AttachConsole( ATTACH_PARENT_PROCESS );
}