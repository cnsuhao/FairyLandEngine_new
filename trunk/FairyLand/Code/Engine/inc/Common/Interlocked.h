//============================================================================
// Interlocked.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Interlocked
//----------------------------------------------------------------------------

class Interlocked
{
public:
	static _long  SetValue( _long& target, _long value );
	static _dword SetValue( _dword& target, _dword value );
};

};