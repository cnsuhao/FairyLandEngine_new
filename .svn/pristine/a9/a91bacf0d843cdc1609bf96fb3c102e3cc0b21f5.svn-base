//============================================================================
// Global.h
//
// Copyright 2006-2009 Possibility Space Incorporated.
//============================================================================

#pragma once

namespace FL
{

//----------------------------------------------------------------------------
// Global
//----------------------------------------------------------------------------

class Global
{
private:
	StringPtr	mName;
	_dword		mValue;
	Global*		mNext;

public:
	Global( StringPtr name );
	~Global( );

	static _bool  SetValue( StringPtr name, _dword value );
	static _dword GetValue( StringPtr name );

	inline _dword GetValue( ) const;
};

//----------------------------------------------------------------------------
// Global Implementation
//----------------------------------------------------------------------------

_dword Global::GetValue( ) const
{
	return mValue;
}

};