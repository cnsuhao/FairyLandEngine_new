//============================================================================
// Interlocked.cpp
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#include "FairyLandCommon.h"

//----------------------------------------------------------------------------
// Interlocked Implementation
//----------------------------------------------------------------------------

_long Interlocked::SetValue( _long& target, _long value )
{
	return ::InterlockedExchange( (LONG*) &target, value );
}

_dword Interlocked::SetValue( _dword& target, _dword value )
{
	return ::InterlockedExchange( (LONG*) &target, value );
}