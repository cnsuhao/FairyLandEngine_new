//============================================================================
// Typedef.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

#include "xmmintrin.h"

#define _null	0
#define _true	1
#define _false	0

namespace FL
{

//! 8-bits ansi character.
typedef char				_chara;
//! 16-bits unicode character.
typedef wchar_t				_charw;
//! 16-bits unicode character.
typedef wchar_t				_char;
//! Signed 8-bits integer.
typedef signed char			_tiny;
//! Signed 16-bits integer.
typedef signed short		_short;
//! Signed 32-bits integer.
typedef signed int			_long;
//! Signed 64-bits integer.
typedef signed __int64		_large;
//! Unsigned 8-bits integer.
typedef unsigned char		_byte;
//! Unsigned 16-bits integer.
typedef unsigned short		_word;
//! Unsigned 32-bits integer.
typedef unsigned long		_dword;
//! Unsigned 64-bits integer.
typedef unsigned __int64	_qword;
//! 32-bits floating point number.
typedef float				_float;
//! 64-bits floating point number.
typedef double				_double;
//! Boolean value, 0 indicates false, other values indicate true.
typedef int					_bool;
//! Any type.
typedef void				_void;
//! Handle to a win32 kernel object.
typedef void*				_handle;
//! 64-bits float list.
typedef __m64				_float2;
//! 128-bits float list.
typedef __m128				_float4;

#define _STRING( s )	L##s
#define _WIDEN( s )		_STRING( s )
#define _FILENAME		_WIDEN( __FILE__ )
#define _BUILDDATE		_WIDEN( __DATE__ )
#define _BUILDTIME		_WIDEN( __TIME__ )
#define _LINENUMBER		__LINE__

#ifdef _TYPECHECK_ENABLE

template < class T >
struct TypeChecker;

#endif

};